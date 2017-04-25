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
#include <pthread.h>    
#include <semaphore.h> 

// Mutexes must be global and readable by all functions and threads
sem_t engine_mutex; // Mutex to guard the critical section of avaliable engines
sem_t fuselage_mutex; // Mutex to guard the critical section of available fuselages
sem_t fuel_tank_mutex; // Mutex to guard the critical section of available fuel tanks
sem_t launch_pad_mutex; // Mutex to guard the critical section of the launch_pad (only one)
sem_t kerbal_mutex; // Mutex to gaur the critical section of avaliable kerbals

/* Function: verifyNum
 * Parameters: string
 * returns: 1 if it is a valid number, 0 otherwise
 * Description: Takes a string and verifies that it is
 * a number that is greater than 0 and less than 256
 */
int verifyNum(char* str){
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


// This may need to be someting diffferent that void (void) once
// we fill it in.
void kerbalThread(void) {

    // This is where we do the infinite for loop specified in the
    // assignment PDF. I just made it a return for the time-being.
    return; 
}

/* Function: main
 * Parameters: run arguments
 * returns: nothing
 * Description: This is the main function, not much more to say
 */
int main(int argv, char* args[]){
    int counter = 0; // counter for general purpose counting
    int engines, fuselages, fuel_tanks, kerbals;
    // check to see if there are not the correct amount of args
    if( argv != 5 ){
        printf("Invalid number of arguments\n");
        printf("4 numeric arguments needed!\n");
        return 0;
    }
        
    //go through the number inputs and verify them
    for(counter = 1; counter < argv; counter++){
        if(!verifyNum(args[counter])){
            printf("Argument %d is invalid!\n", counter);
            return 0;
        }
    }
    
    engines = atoi(args[1]);
    fuselages = atoi(args[2]);
    fuel_tanks = atoi(args[3]);
    kerbals = atoi(args[4]);

    // THIS IS ONLY FOR VERIFICATION ON FUNCTIONALITY/DEBUGGING
    printf("####################\n#  DEBUG (DELETE ME BEFORE TURNING IN):"
           "\n#\tENG = %d\n#\tFUSLG = %d\n#\tFUELTNKS = %d\n#\tKERBALS = "
           "%d\n####################\n",
           engines,
           fuselages,
           fuel_tanks,
           kerbals);    

    // Initializes mutex to keep track of number of resources
    sem_init(&engine_mutex, 0, engines);
    sem_init(&fuselage_mutex, 0, fuselages);
    sem_init(&fuel_tank_mutex, 0, fuel_tanks);
    sem_init(&kerbal_mutex, 0, kerbals);
    sem_init(&launch_pad_mutex, 0, 1); // Always only 1 launch pad

/*

    This section is where we spawn threads. Since we have to dynamically
    allocate them (up to 255 kerbals that each require a thread), I think
    we should do something like make a large array of pointers for the
    threads to point to. Something like:

    void * kerbals[255];

    and then spawn threads at the pointers in a for loop
    (ref http://www.cprogramming.com/tutorial/function-pointers.html
    or google "spwan threads dynamically in a loop" or something). I'll have
    to investigate this more. Check out Prof Bakkers semaphores.c file for the
    pthread_create(), pthread_join(), sem_wait(), and sem_post() commands. I
    think we have to used sem_trywait() as well in this assignment juding by
    the way his flow in the functionality skips my the first time if resources
    are not avalable and then waits the second time until they are available.  

*/

    sem_destroy(&kerbal_mutex);
    sem_destroy(&engine_mutex);
    sem_destroy(&fuselage_mutex);
    sem_destroy(&fuel_tank_mutex);
    sem_destroy(&launch_pad_mutex);
    
}




