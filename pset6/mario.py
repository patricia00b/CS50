#Implement a program that prints out a half-pyramid of a specified height, per the below.
#
#$ ./mario
#Height: 5
#     ##
#    ###
#   ####
#  #####
# ######

#1. prompt the user and validade input
#draw half pyramid

import cs50

def main():
    n = get_positive_int()
    #draw half pyramid: spaces, #, new line
    b = n-1
    h = 2
    for i in range(n):
        print(" "*b, end = "")
        print("#"*h) #prints # + new line
        b=b-1
        h=h+1

def get_positive_int():
    while True:
        print("Height: ", end="")
        n = cs50.get_int()
        if n >= 0:
            break
    return n

if __name__ == "__main__":
    main()
    
