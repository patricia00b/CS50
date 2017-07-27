# Implement a program that calculates the minimum number of coins required to give a user change.

# $ ./greedy
# O hai! How much change is owed?
# 0.41
# 4

# prompt user for amount of change
# always use largest change possible
# keep track of coins used
# print final number of coins

import cs50

def main():
    d = get_pos_amount()  #gets amount in dollars
    #converts to cents and rounds to nearest int
    c = int(round(d*100))
    n = 0
    n += c // 25
    c %= 25
    
    n += c // 10
    c %= 10
    
    n += c // 5
    c %= 5
    
    n += c
    
    print(n)
    
def get_pos_amount():
    while True:
        print("O hai! How much change is owed?")
        f = cs50.get_float()
        if f >= 0:
            break #breaks true cicle
    return f
    
if __name__ == "__main__":
    main()
