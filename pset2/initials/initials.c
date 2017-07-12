#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // gets user input as a string
    string s = get_string();
    
    // if the input is not null
    if(s!= NULL)
    {   // gets the string last index
        int finalsize = strlen(s)-1;
        // checks the input real size without blanks
        for (int j= finalsize; s[j] != '\0'; j--)
        {
            if (s[finalsize] == ' ')
            {
                finalsize = j;
            }
        }
        // if the user input doesnt start with blanks
        if(s[0]!= ' ')
        {
            // prints the first letter of the name
            printf("%c", (char) toupper(s[0]));
        
            //iterates over the input
            for (int i = 0;s[i]!='\0'; i++)
            {
                // if the iteration finds
                if(s[i] == ' ' && i< finalsize)
                {
                    printf("%c",(char) toupper(s[i+1]));
                }
            } 
        }
        // if the user input starts with blanks
        else
        {
            //iterates over the input
            for (int i = 0;s[i]!='\0'; i++)
            {
                // if the iteration finds
                if(s[i] == ' ' && s[i+1] != ' ' && i< finalsize)
                {
                    printf("%c",(char) toupper(s[i+1]));
                }
            } 
        }
        printf("\n");
    }
}
