#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // if no command line input is given returns error message
    if (argc != 2)
    {
        printf("missing command-line argument\n");
        return 1;
    }
    //declares key and its length
    int key[50];
    int n = strlen(argv[1]);
    
    //if input is correct
    if (argc == 2)
    {   //iterates over the argv
        for (int a = 0; a<n;a++)
        {
            //if argv is not alphanumeric returns error message
            if (isalpha(argv[1][a])==0)
            {
                printf("Error\n");
                return 1;
            }
            else
            {   //encryption algorithm
                if(isupper(argv[1][a]))
                {
                    char c = ((argv[1][a]) - 65); 
                    key[a] = c;  
                }
                else
                {
                    char c = ((argv[1][a]) - 97);
                    key[a] = c;                      
                }
            }
        }
    }
    
    printf("plaintext: ");
    string plaintext = get_string();

    printf("ciphertext: ");
        
    int k = 0;
    for (int j = 0,m = strlen(plaintext); j < m; j++) //iterates over plaintext
    {
        int c = plaintext[j];

        if (isalpha(c))
        {
            int kvalue = key[k];

            if (isupper(c)) 
            {
                c-= 65; // to alpha
                c = ((c+kvalue)%26); //encryption algorithm
                c+=(65); //back to ascii
                printf("%c", c);
            }
            if(islower(c))
            {   
                c-= 97;
                c = ((c+kvalue)%26); // encryption algorithm
                c+= 97;
                printf("%c", c);
            }
            k++;
            if (k == n)
            {
                k = 0;
            }
        }
        else
        {
            printf("%c", c);
        }
    }
    printf("\n");
    return 0;
}
