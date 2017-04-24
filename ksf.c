/*
 * Name: David Theodore and Maxwell Sanders
 * ID #: 1001122820 and 1001069652
 * Programming assignment 4
 * Description: PThread Kerbal Simulator
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <ctype.h>

/* Function: verify_num
 * Parameters: string
 * returns: 1 if it is a valid number, 0 otherwise
 * Description: Takes a string and verifies that it is
 * a number that is greater than 0 and less than 256
 */
int verify_num(char* str){
    // if the string length is greater than 3 then it is larger than 255
    if(strlen(str) > 3){
        return 0;
    }

    // if there are non-numeric characters
    int i; // generic counter variable
    for(i = 0; i < strlen(str); i ++){
        if(!isdigit(str[i])){
            printf("Non-Numeric or negative input detected!\n");
            return 0;
        }
    }

    // check to see if it is greater than 255 or equal to 0
    int num = atoi(str); // number variable to hold the number
    if(num > 255 || num == 0){
        printf("Number needs to be within the range of 1-255 inclusive!\n");
        return 0;
    }

    // all is good with this number
    return 1;
}

/* Function: main
 * Parameters: run arguments
 * returns: nothing
 * Description: This is the main function, not much more to say
 */
int main(int argv, char* args[]){
    int counter = 0; // counter for general purpose counting

    // check to see if there are not the correct amount of args
    if( argv != 5 ){
        printf("Invalid number of arguments\n");
        printf("4 numeric arguments needed!\n");
        return 0;
    }

        
    //go through the number inputs and verify them
    for(counter = 1; counter < argv; counter++){
        if(!verify_num(args[counter])){
            printf("Argument %d is invalid!\n", counter);
            return 0;
        }
    }
}
