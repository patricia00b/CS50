/*
Implement a program that encrypts messages using Caesar’s cipher, per the below.

$ ./caesar 13
plaintext:  HELLO
ciphertext: URYYB
*/

//\[c_i = (p_i + k) \bmod 26\]
#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    string key[10];
    int n = strlen(argv[1]);
    if (argc == 2 && (isalpha(argv[1])))
    {
        for (int i = 0; i<n;i++)
        {
            char c = ((argv[1][i]) - 65);
            key[i] = &c;
        }

        printf("plaintext: ");
        string plaintext = get_string();

        printf("ciphertext: ");
        
        //iterate over the several key values
        for(int k = 0;k<n;k++)
        {
            int kvalue = *key[k];
            
            // iterate over strings in argv
            for (int j = 0,m = strlen(plaintext); j < m; j++)
            {
                int c = plaintext[j];
                if (isupper(c)) // if the char is capitalized
                {
                    c-= 65; // to alpha
                    c = ((c+kvalue)%26); //encription
                    c+=(65); //back to ascii
                    printf("%c", c);
                }
                else if(islower(c))
                {   c-= 97;
                    c = ((c+kvalue)%26); // encription
                    c+= 97;
                    printf("%c", c);
                }
                else
                {
                    printf("%c", c);
                    k--; // so it doesnt count
                }
            }
        }
        
        printf("\n");
    }
    else
    {
        printf("Error, please submit a non-negative integer single command-line argument\n");
        return 1;
    }
}
