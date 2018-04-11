#include <stdio.h>
#include <stdlib.h>

//declare variables
int *avail, *max, *hold, *need;
int i_nbProcess, j_nbResource;

//mutex lock
pthread_mutex_t mutex;

/*
Implementation of isSafe() as described in the slides
*/
int isSafe(){

    int *finish = malloc(i_nbProcess * sizeof(int));
    int *work = malloc(num_resources * sizeof(int));

    //Step 1: initialize
    for(int i=0; i<i_nbProcess; i++){
        finish[i] = 0;
    }
    for(int j=0; j<j_nbResource; j++){
        work[j] = avail[j];
    }

    for(int i=0; i<i_nbProcess; i++){
        for(int j=0; j<j_nbResource; j++){
            //Step 2: find a process Pi such that 
            if (finish[i] == 0 && need[i][j] <= work[j]){
                //Step 3:
                work[j] = work[j] + hold[i][j];
                finish[i] = 1;
            }
        }
    }
    //Step 4: 
    for(int i=0; i<j_nbResource; i++){
        if (finish[j] == 0){
            return false;
        }
    }    

    return true;

}
/*
Implementation of Bankers Algorithm as described in the slides
returns 1 if safe allocation 0 if not safe
*/
int bankers_algorithm(int pr_id, int* request_vector){

    //everytime a process Pi posts a request in REQ[j]

    //Step 1: verify that a process matches its needs
    if request_vector[j] > Need[i][j] { //abort
        break;
    }

    //Step 2: check if the requested amount if available
    if  request_vector[j] > Avail[j] {
        break;
    }

    //Step 3: provisional allocation
    Avail[j] = Avail[j] - request_vector[j];
    Hold[i][j] = Hold[i][j] + request_vector[j];
    Need[i][j] = Need[i][j] - request_vector[j];

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

    //init the mutex and semaphores
    pthread_mutex_init(&mutex,NULL);

    //Initialize all inputs to banker's algorithm

    //get number of processes
    printf("Enter number of processes:/n");
    scanf("%d",&i_nbProcess);

    //get nb of distinct resources
    printf("Enter number of resources:/n");
    scanf("%d",&j_nbResource);

    //allocate memory for each array
    avail = malloc(nbResources * sizeof(int));
    
    //max = malloc(i_nbProcess* j_nbResource * sizeof(int));
    max = malloc(i_nbProcess* sizeof(int));
    for( int j=0; j < m; j++){
        max[i] = malloc(j_nbResource * sizeof(int));
    }

    //hold = malloc(i_nbProcess* j_nbResource * sizeof(int));
    hold = malloc(i_nbProcess* sizeof(int));
    for( int j=0; j < m; j++){
        hold[i] = malloc(j_nbResource * sizeof(int));
    }
    
    //need = malloc(i_nbProcess* j_nbResource * sizeof(int));
    need = malloc(i_nbProcess* sizeof(int));
    for( int j=0; j < m; j++){
        need[i] = malloc(j_nbResource * sizeof(int));
    }

    //get amount of each resource
    printf("Enter amount of each resource:/n");
    for(int j = 0; j<j_nbResource;j++){
        printf("For resource %d:",j);
        scanf("%d",&avail[j]);
    }

    //get max resource claim per process/resource
    printf("Enter the max number of resource claim per process: /n");
    for(int i = 0; i<i_nbProcess;i++){
        for(int j = 0; j<j_nbResource;i++){
            printf("Max number of resource %d for process %d: ",j,i); 
            scanf("%d",&max[i][j]);
            hold[i][j] = 0;
            need[i][j] = max[i][j] - hold[i][j];
        }
    }

    //create threads simulating processes (process_simulator)
    pthread_t processThreads[i_nbProcess];
    for(int i=0; i<i_nbProcess;i++){
        pthread_create(&processThreads[i], NULL, process_simulator,(void*)(intptr_t)i);
    }

    //create a thread that takes away resources from the available pool (fault_simulator)
    pthread_t faultThread;
    pthread_create(&faultThread, NULL, fault_simulator,(void*)(intptr_t)i);
    

    //create a thread to check for deadlock (deadlock_checker)
    pthread_t deadlockThread;
    pthread_create(&deadlockThread, NULL, process_simulator,(void*)(intptr_t)i);
    

    free(max);
    free(hold);
    free(avail);
    free(need);
    pthread_exit(NULL);
    return 0;
}