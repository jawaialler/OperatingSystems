#include <stdio.h>
#include <stdlib.h>

/*
Implementation of isSafe() as described in the slides
*/
int isSafe(){

    int isSafe = 0;

    //Work temp vectors
    //Finish temp vectors

    int finish[P] = {0};

    int work[P] = {0};


    //Step 1: initialize
    for(int j;j< ???;j++){
        Work[j] = Avail[j];
        Finish[i] = false
    }

    //Step 2: find a process Pi such that

    if (Finish[i] = false AND Need[i][j] <= Work[j]){
        //Step 4: 
        for(all i){
            if (Finish[i] = false){
                isSafe = false;
            }
            break;
        }
    }
    else{
        //Step 3:
        Work[j] = Work[j] + Hold[i][j];
        Finish[i] = true;
    }



    return isSafe;

}
/*
Implementation of Bankers Algorithm as described in the slides
returns 1 if safe allocation 0 if not safe
*/
int bankers_algorithm(int pr_id, int* request_vector){

    //everytime a process Pi posts a request in REQ[j]

    //Step 1: verify that a process matches its needs
    if REQ[j] > Need[i][j] { //abort
        break;
    }

    //Step 2: check if the requested amount if available
    if  REQ[j] > Avail[j] {
        break;
    }

    //Step 3: provisional allocation
    Avail[j] = Avail[j] - REQ[j];
    Hold[i][j] = Hold[i][j] + REQ[j];
    Need[i][j] = Need[i][j] - REQ[j];

    return isSafe();

}

/*
Simulates processes running on the system.

*/
void* process_simulator(void* pr_id){

}

/*
Simulates a fault occuring on the system.

*/
void* fault_simulator(void* pr_id){

}

/*
Checks for deadlock
*/
void* deadlock_checker(void * pr_id){

}

/*
Simulates resource requests by processes 
*/
void request_simulator(int pr_id, int* request_vector){

}


int main()
{

    //Initialize all inputs to banker's algorithm

    //create threads simulating processes (process_simulator)

    // create a thread that takes away resources from the available pool (fault_simulator)

    //create a thread to check for deadlock (deadlock_checker)

    return 0;
}