from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached disables caching of responses
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter that formats values as US dollars (USD)
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (i.e. disk) (instead of signed cookies) 
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configures CS50’s SQL module to use finance.db, a SQLite database 
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required # decorator ensures that, if a user tries to visit any of those routes, he or she will first be redirected to login so as to log in.
def index():
    """ html with user portfolio
    stock owned, shared for each stock, current price of each stock, total value of each holding 
    user corrent cash balance
    grand total of user cash + stock total values """
    
    #obtain user cash
    user_id=session.get("user_id")
    user_cash = db.execute("SELECT cash FROM users WHERE id=:user_id", user_id = user_id)
    #obtain user grand total
    grand_total = user_cash[0]["cash"]
    
    #obtain purchases info
    purchases = db.execute("SELECT symbol, shares FROM purchases WHERE id=:user_id",user_id=user_id)
    
    #iterate over every purchase and obtain atributes
    for stock in purchases:
        symbol = str(stock["symbol"])
        shares = int(stock["shares"])
        quote = lookup(symbol)
        stock["name"] = quote["name"]
        stock["price"] = "{:.2f}".format(quote["price"])
        stock["total"] = "{:.2f}".format(quote["price"] * shares)
        stock["grand_total"] = quote["price"]*shares
        grand_total += stock["grand_total"]
    # render index page with some given values
    return render_template("index.html", purchases = purchases, user_cash = user_cash, grand_total = grand_total)

@app.route("/buy", methods=["GET", "POST"]) #post is when user submits the shares to buy while on the buy page, get is when user first enters the buy page
@login_required # decorator ensures that, if a user tries to visit any of those routes, he or she will first be redirected to login so as to log in.
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        #check if valid input
        try:
            quote = lookup(request.form.get("symbol"))
            shares_bought = int(request.form.get("shares"))
        except:
            return apology("Please select share to buy.")
        
        #ensures user selects symbol and share to buy
        if quote == "" or shares_bought == "":
            return apology("Please select share to buy.")
            
        #checks if the user can afford stock
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id=:user_id",user_id = user_id)
        user_cash = cash[0]["cash"]
        price = round(float(quote["price"]),2)
        current_cost = round(float(shares_bought * price),2)
        
        if((not user_cash) or ((current_cost)>user_cash)):
            return apology("Sorry, not enough money available.")
        else: #if user can afford add it to portfolio
            #update user cash
            user_id = session["user_id"]
            symbol=quote["symbol"]
            db.execute("UPDATE users SET cash = (cash - :purchase) where id =:user_id", purchase = current_cost, user_id = user_id)
            #update user owned number of shares
            db.execute("UPDATE purchases SET shares = shares + :shares_bought WHERE id = :user_id AND symbol = :symbol", user_id=user_id,symbol=symbol,shares_bought=shares_bought)
            #add buy it to user purchases table
            db.execute("INSERT INTO purchases (symbol, shares, price, id) VALUES (:symbol, :shares, :price, :user_id);", symbol=symbol, shares=shares_bought, price=price, user_id=user_id)
            #update user history
            db.execute("INSERT INTO history (id,symbol,shares,price,date) VALUES (:user_id,:symbol,:shares,:price,datetime('now'))",user_id=user_id, symbol=symbol,shares=shares_bought,price=price)

            #redirect user to index
            flash('Bough!')
            return redirect(url_for("index"))

    else:
        return render_template("buy.html")
            
@app.route("/history")
@login_required # decorator ensures that, if a user tries to visit any of those routes, he or she will first be redirected to login so as to log in.
def history():
    """Show history of transactions.
    whether stock was sold or bought 
    stock symbol, purchase/sold price, number of shares sold/bought
    time and date of transaction"""
    
    #get user history db info
    id=session["user_id"]
    history = db.execute("SELECT symbol, shares, price, date FROM history WHERE id = :id ORDER BY date DESC", id=id)
    
    for row in history:
        symbol = row["symbol"]
        shares = row["shares"]
        price = row["price"]
        date = row["date"]

    return render_template("history.html", history = history)
    
    
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username: db.execute (from CS50’s library) to query finance.db
        username=request.form.get("username")
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=username)

        # ensure username exists and password is correct: pwd_context.verify to compare hashes of users' passwords
        if len(rows) != 1:
            return apology("invalid username and/or password")
        if not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")
        
        #if everything done correctly
        # remember which user has logged in: login "remembers" that a user is logged in by storing his or her user_id, an INTEGER, in session. 
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        flash('Loged In')
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""
    #logout simply clears session, effectively logging a user out.
    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required # decorator ensures that, if a user tries to visit any of those routes, he or she will first be redirected to login so as to log in.
def quote():
    """Get stock quote."""
    
    if request.method == "POST":
        quote = lookup(request.form.get("symbol")) #gets current yahoo symbols
        if not quote:
            apology("Invalid symbol.")
        return render_template("quoted.html", name = quote["name"], symbol = quote["symbol"], price = quote["price"])
        
    else:
        return render_template("quote.html")
    
    return apology("TODO")

@app.route("/register", methods=["GET","POST"])
def register():
    """Register user."""
    
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("Missing username!")

        # ensure password1 was submitted
        elif not request.form.get("password"):
            return apology("Missing password.")
        
        # ensure password2 was submitted
        elif not request.form.get("password2"):
            return apology("Missing password.")
        
        #ensure 1st and 2nd password match
        elif request.form.get("password") != request.form.get("password2"):
            return apology("Passwords don't match.")
        
        #check for duplicate user ERROS TO FIX
        #userRows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        #if request.form.get("username") in userRows[0]["username"]:
        #    return apology("duplicate user")
        
        #add user to databate so they are stored and can log in again
        username=request.form.get("username")
        #store hash password with encrypt funtion
        #hash = pwd_context.encrypt(request.form.get("password"))
        passw = request.form.get("password")
        hash=pwd_context.encrypt(passw)
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash);",username=username,hash = hash)        
        #if username already exist, failure
        #if not result:
        #    return apology("Username already exist.")
            
        #once they are stored, log in automatically
        #store id in session
        user_id = db.execute("SELECT id FROM users WHERE username is :username", username = username) 
        session['user_id'] = user_id[0]['id']
        
        #redirect user to index page
        flash('Registered')
        return redirect(url_for('login'))
        
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required #  decorator ensures that, if a user tries to visit any of those routes, he or she will first be redirected to login so as to log in.
def sell():
    """Sell shares of stock.
    remove stock from user portfolio
    update user_cash """
    #if user reaches route via post
    if request.method == "POST":
        #ensures proper usage, symbol and number of shares provided
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Invalid selection. Please select a symbol and number of shares to sell.")
        
        #get current stock info
        quote = lookup(request.form.get("symbol"))
        
        #ensure data validity
        if quote == None:
            return apology("Invalid symbol.")
        if not request.form.get("shares").isdigit():
            return apology("Invalid quantity.")
            
        # get number of shares to sell
        shares_tosell = int(request.form.get("shares"))
        
        #info
        symbol = quote["symbol"]
        id = session["user_id"]
        price = round(float(quote["price"]),2)
        sale = round(float(shares_tosell * price),2)

        #obtain current shares owned by the user from database
        stocks = []
        stocks = db.execute("SELECT shares FROM purchases WHERE id = :id AND symbol = :symbol", id=id, symbol=symbol)
        
        final_shares = int(stocks[0]["shares"] - shares_tosell)

        #validade user has enough shares to sell
        if stocks == []:
            return apology("you don't own any of this stock")
        if final_shares<0:
            return apology("invalid number of shares")
        
        #if input is valid, sell shares
        #update remove shares sold from user purchases
        db.execute("UPDATE purchases SET shares = shares - :shares_tosell WHERE id = :id AND symbol = :symbol", id=id, shares_tosell = shares_tosell, symbol = symbol)
        
        #check if user stopped having that symbol shares for deletion
        if final_shares <= 0:
            db.execute("DELETE FROM purchases WHERE id = :id AND symbol = :symbol", id=id, symbol = symbol)

        #update user_cash balance
        db.execute("UPDATE users SET cash = cash + :sale WHERE id = :id", sale = sale, id=id)        
        
        #update user history
        db.execute("INSERT INTO history (id,symbol,shares,price,date) VALUES (:id,:symbol,:final_shares,:price,datetime('now'))",id=id, symbol=symbol,final_shares=final_shares,price=price)
    
        #redirect user to index page
        flash('Sold!')
        return redirect(url_for("index"))
        
    else:
        return render_template("sell.html")
