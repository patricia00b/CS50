#include <stdio.h>
#include <cs50.h>

/*program that recreates this half-pyramid using hashes (#) for blocks.
 first prompt the user for the half-pyramid’s height, a non-negative integer no greater than 23.
 If the user fails to provide a non-negative integer no greater than 23, you should re-prompt for the same again.
*/

int ask(){
    printf("Height: ");
    int height = get_int();
    return height;
}

int check(height)
{
    if (height >= 0 && height <= 23)
    {   
        return height;
    }
    else
    {
        return -1;  // -1 acts as a flag
    }
}



int main(void){
    int h = ask(); // asks and gets the input number for the height
    int result = check(h);  // 
    while (result == -1)
    {
        h = ask();
        result = check(h);
    }
    

    for(int l = 0; l<h;l++)
    {
        for(int b = h ;b>1+l;b--)  
        {
            printf(" ");
        }
        for(int c = 0;c<1+l;c++) 
        {
            printf("#");
        }
        for( int m = 0; m< 2;m++)
        {
            printf(" ");
        }
        for(int d = 0;d<l+1;d++) 
        {
            printf("#");
        }
        printf("\n");
    }
}
