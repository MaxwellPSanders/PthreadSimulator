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
sem_t resource_mutex; // Mutex to guard the critical section of available resources
sem_t launch_pad_mutex; // Mutex to guard the critical section of the launch_pad (only one)
sem_t count_mutex; // Mutex that will let the kerbals have a nice unique number

int MY_NUM = 0; //the number of the kerbals, will count up
int* ENGINES, *FUSELAGES, *FUEL_TANKS; //these will hold 0 allocated arrays of resources
int ENGINE_COUNT, FUSELAGE_COUNT, FUEL_TANK_COUNT;

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


/* Function: kerbalTask
 * Parameters: kerbal_id
 * returns: nothing
 * Description: This is the kerbal function, loops and does all
 * the tasks kerbals are supposed to do
 */
void *kerbalTask(void *arg) {
    int kerbal_id; // id of the current thread
    int grabbed[] = {0,0,0}; // whether or not it grabbed any resources in this iteration
    int eng_index = -1; // index of the engine grabbed
    int fus_index = -1; // index of the fuselage grabbed
    int fuel_index = -1; // index of the fueltank grabbed
    int wait_notif = 1; // used for the wait notification
    int i; // general purpose counter

    //gives the kerbal a unique identifier, doesn't matter the order
    sem_wait(&count_mutex);
        MY_NUM ++;
        kerbal_id = MY_NUM;
    sem_post(&count_mutex);

    //while loops because they are better than for loops
    while(1){
        //try to grab the resources, if you can't then sleep and try again
        sem_wait(&resource_mutex);
            //loop through each array if you don't have a part grab one  
            if(eng_index == -1){ 
                grabbed[0] = 0;
                for(i = 0; i < ENGINE_COUNT; i ++){
                    if(!ENGINES[i]){
                        grabbed[0] = 1; 
                        eng_index = i; 
                        ENGINES[i] = 1;
                        break;
                    }
                }
            } 
            if(fus_index == -1){
                grabbed[1] = 0;
                for(i = 0; i < FUSELAGE_COUNT; i ++){
                    if(!FUSELAGES[i]){
                        grabbed[1] = 1; 
                        fus_index = i; 
                        FUSELAGES[i] = 1;
                        break;
                    }
                } 
            }
            if(fuel_index == -1){
                for(i = 0; i < FUEL_TANK_COUNT; i ++){
                    grabbed[2] = 0;
                    if(!FUEL_TANKS[i] && fuel_index == -1){
                        grabbed[2] = 1; 
                        fuel_index = i; 
                        FUEL_TANKS[i] = 1;
                        break;
                    }
                }
            }
            //check if it grabbed all of them
            if(grabbed[0] &&
               grabbed[1] &&
               grabbed[2]){
                printf("Kerbal %d: Enters into the assembly facility\n", kerbal_id);
                break;   
            }

            //notify that you are waiting
            if(wait_notif){
                printf("Kerbal %d: Waiting for the assembly facility\n", kerbal_id); 
                wait_notif--;
            }            

        sem_post(&resource_mutex);
            
        //didn't grab anything so wait
        sleep(1);
    }
    sem_post(&resource_mutex);

    sleep(15); // what a riveting assembly
    printf("Kerbal %d: Assembles Engine %d, Fuselage %d, Fuel Tank %d\n",
                kerbal_id, eng_index + 1, fus_index + 1, fuel_index + 1);

    //try to launch the rocket, fly it, land it
    if(!sem_trywait(&launch_pad_mutex)){}        
    else{ 
        printf("Kerbal %d: Waiting for launch pad\n", kerbal_id);
        sem_wait(&launch_pad_mutex);
    }
        printf("Kerbal %d: Arriving at launch pad\n", kerbal_id);
        for(i = 10; i > 0; i --){
             printf("Kerbal %d: Launch countdown %d ...\n", kerbal_id, i);
             sleep(1);
        }
        printf("Kerbal %d: Lift off!\n", kerbal_id);
    sem_post(&launch_pad_mutex);
       
    sleep(5); //what an intense and exciting flight
    printf("Kerbal %d: Landed!\n", kerbal_id);

    //wait to return everything
    sleep(5);
    sem_wait(&resource_mutex);
        printf("Kerbal %d: Dissasembles Engine %d, Fuselage %d, Fuel Tank %d\n",
                kerbal_id, eng_index + 1, fus_index + 1, fuel_index + 1);
        ENGINES[eng_index] = 0;           
        FUSELAGES[fus_index] = 0;           
        FUEL_TANKS[fuel_index] = 0;           
    sem_post(&resource_mutex);
    return NULL;   
}

/* Function: main
 * Parameters: run arguments
 * returns: nothing
 * Description: This is the main function, not much more to say
 */
int main(int argv, char* args[]){
    int counter = 0; // counter for general purpose counting
    pthread_t* kerbals; //arrays holding all of the resources
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
    
    ENGINES = (int*) calloc(atoi(args[1]), sizeof(int));
    FUSELAGES = (int*) calloc(atoi(args[2]), sizeof(int));
    FUEL_TANKS = (int*) calloc(atoi(args[3]), sizeof(int));
    kerbals = (pthread_t*) malloc(atoi(args[4])*sizeof(pthread_t));

    ENGINE_COUNT = atoi(args[1]);
    FUSELAGE_COUNT = atoi(args[2]);
    FUEL_TANK_COUNT = atoi(args[3]);

    // Initializes mutex to keep track of number of resources
    sem_init(&resource_mutex, 0, 1);
    sem_init(&launch_pad_mutex, 0, 1); // Always only 1 launch pad
    sem_init(&count_mutex, 0, 1); 

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

    int kerbal_count = atoi(args[4]); //holds how many kerbals there are
    //create all the pthreads, they will run kerbal task
    for(counter = 0; counter < kerbal_count; counter ++){
          if(pthread_create(&kerbals[counter], NULL, kerbalTask, NULL )){
              perror("Error creating thread: ");
              exit( EXIT_FAILURE ); 
          }        
    }

 
    //wait for all the pthreads
    for(counter = 0; counter < kerbal_count; counter ++){
        if(pthread_join(kerbals[counter], NULL)){
            perror("Problem with pthread_join: ");
        }
    }

    sem_destroy(&resource_mutex);
    sem_destroy(&launch_pad_mutex);
    sem_destroy(&count_mutex);    
}




