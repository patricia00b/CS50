import nltk #splits a tweet in a list of words
import sys
import os

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives): #takes path to list of positive and negative words as argument
        """
        Initialize Analyzer.
        Loads positive and negative words into memory in such a way that analyze can access them
        """
        # TODO
        
        self.positives = positives
        self.negatives = negatives
        self.positives_words = [] #list with positive words
        self.negatives_words = [] #list with negative words

        try: #load positive words
            with open(self.positives, 'r') as pwords:
                for lines in pwords:
                    if not lines.startswith(';') or not lines.startswith('\n'):
                        self.positives_words.append(lines.strip('\n'))
        except IOError:
            print("Impossible to open positive words text")
        
        try: #load negative words
            with open(self.negatives, 'r') as nwords:
                for lines in nwords:
                    if not lines.startswith(';') or not lines.startswith('\n'):
                        self.negatives_words.append(lines.strip('\n'))
        except IOError:
            print("Impossible to open negative words text")



    def analyze(self, text): #takes the text to be analyzed for sentiment
        """
        Analyze text for sentiment, returning its score.
        Analyse each word in text a value (1,-1,0)
        Calculate text total score
        """
        #initialize inicial score to 0
        score = 0
        #Create tokenizer instance
        tokenizer = nltk.tokenize.TweetTokenizer()
        #create list of words in a tweets
        tokens = tokenizer.tokenize(text)
        
        #iterate over tokens(list of words)
        for word in tokens:
            #check if word is positive or negative
            if word.lower() in self.positives_words:
                score+=1
            if word.lower() in self.negatives_words:
                score-=1
            #neutral if its neither, doesnt add anything, 0
        return score
