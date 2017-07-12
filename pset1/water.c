/*
Implement a program that reports a user’s water usage, converting minutes spent in the shower to bottles of drinking water.

$ ./water
Minutes: 1
Bottles: 12

192 ounces of water per minute
1-minute shower is akin to using 192 ÷ 16 = 12 bottles of water. 

program that prompts the user for the length of his or her shower in minutes (as a positive integer) and then prints the equivalent number of bottles of water (as an integer)
*/

#include <stdio.h>
#include <cs50.h>

int main(void){
    printf("Minutes: ");
    int shower_time = get_int();
    printf("Bottles: %i\n", 12*shower_time);
}

