# Implement a program that encrypts messages using Caesar’s cipher, per the below.

# $ ./caesar 13
# plaintext:  HELLO
# ciphertext: URYYB

import cs50
import sys


def main():
    if len(sys.argv) == 2: #argc
        #get the key
        key = int(sys.argv[1])
        #get plaintext
        print("plaintext: ", end ="")
        ptext = cs50.get_string()
        #ENCIPHER
        #for each character in plaintext string
        for c in ptext:
            #if alphabetical shift character acc with key, preserving case
            if c.isalpha():
                if c.isupper:
                    print(chr(((ord(c) - 65 + key) % 26) + 65), end ="")
                if c.islower():
                    print(chr(((ord(c) - 65 + key) % 26) + 65), end = "")
    else:
        print("Usage: python caesar.py [KEY]")
    print()

if __name__ == "__main__":
    main()
    
