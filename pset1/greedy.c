// Implement a program that calculates the minimum number of coins required to give a user change.
//you want to minimize the number of coins you’re dispensing for each customer, lest you have to press levers more times than are necessary
//coins available are quarters (25¢), dimes (10¢), nickels (5¢), and pennies (1¢).
//If the user fails to provide a non-negative value, your program should re-prompt the user for a valid amount again and again until the user complies.

/*$ ./greedy
O hai! How much change is owed?
0.41
4
*/
#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    int counter = 0;
    printf("O hai! How much change is owed?\n");
    float amount = get_float();   
    while(amount < 0) // while amount is a negative value asks for a new value
    {
        printf("How much change is owed?\n");
        amount = get_float();   
    }
    amount = round(amount*100);
    while(amount>=25) // use quarters
    {
        amount-=25;
        counter++;
    }
    while(amount>=10) // use quarters
    {
        amount-=10;
        counter++;
    }
    while(amount>=5) // use quarters
    {
        amount-=5;
        counter++;
    }
    while(amount>=1) // use quarters
    {
        amount-=1;
        counter++;
    }
    printf("%i\n", counter);
}
