#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>

//declare variables
int *avail, **max, **hold, **need;
int i_nbProcess, j_nbResource;

//mutex lock
pthread_mutex_t mutex;

/*
Implementation of isSafe() as described in the slides
*/
int isSafe(){

    printf("Checking if allocation is safe...\n");

    int *finish = malloc(i_nbProcess * sizeof(int));
    int *work = malloc(j_nbResource * sizeof(int));

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
    for(int j=0; j<j_nbResource; j++){
        if (finish[j] == 0){
            return 0;
        }
    }    

    return 1;

}
/*
Implementation of Bankers Algorithm as described in the slides
returns 1 if safe allocation 0 if not safe
*/
int bankers_algorithm(int pr_id, int* request_vector){
    int process_id = (intptr_t)pr_id;
    //everytime a process Pi posts a request in REQ[j]

    //Step 1: verify that a process matches its needs
    for (int j=0; j<j_nbResource;j++){
        if (request_vector[j] > need[process_id][j]) { //abort
            return 0;
        }   
    }

    //Step 2: check if the requested amount if available
    for(int j=0; j<j_nbResource;j++){
        if (request_vector[j] > avail[j]) {
            return 0;
        }
    }

    //Step 3: provisional allocation
    for(int j=0; j<j_nbResource;j++){
        avail[j] = avail[j] - request_vector[j];
        hold[process_id][j] = hold[process_id][j] + request_vector[j];
        need[process_id][j] = need[process_id][j] - request_vector[j];
    }

    if (isSafe()){
        printf("System is safe: allocating \n");
        return 1;
    }
    else{
        //remove allocation
        printf("Allocation is not safe: cancelling \n");
        for(int j=0; j<j_nbResource;j++){
            avail[j] = avail[j] + request_vector[j];
            hold[process_id][j] = hold[process_id][j] - request_vector[j];
            need[process_id][j] = need[process_id][j] + request_vector[j];
        }
        return 0;
    }

}

/*
Checks if processes are completed.
*/
int processComplete(int process){
    for (int j=0;j<j_nbResource;j++){
        if(need[process][j] > 0){
            return 0;
        }
    }
    return 1;
}


/*
Simulates processes running on the system.
*/
void* process_simulator(void* pr_id){
    int process_id = (intptr_t)pr_id;

    while(processComplete(process_id) == 0){ //until the process is done, keep trying to allocate
        //generate random resource request vector
        int *requestvector = malloc(j_nbResource * sizeof(int));

        printf("Requesting resources for process %d \n",process_id);

        for ( int j=0;j<j_nbResource;j++){
            if(need[process_id][j] != 0){
                requestvector[j] = rand() % need[process_id][j] + 1;
            }
            else{
                requestvector[j] = 0;
            }
        }

        printf("The resource vector requested by process %d is: \n",process_id);
        for(int j=0;j<j_nbResource;j++){
            printf("%d ",requestvector[j]);
        }
        printf("\n");
        
        //calls bankers alg to see if allocation of reqest vector is safe or not

        //entering critical section so lock

        pthread_mutex_lock(&mutex); //lock
        int safe = bankers_algorithm(process_id, requestvector);
        pthread_mutex_unlock(&mutex); //unlock
        
        while(safe != 1){//busy waiting on bankers until allocation is safe
            sleep(1);
            pthread_mutex_lock(&mutex); //lock
            safe = bankers_algorithm(process_id, requestvector);
            pthread_mutex_unlock(&mutex); //unlock
        }

        //if safe, grant the resources
        printf("The resource vector ");
        for(int j=0;j<j_nbResource;j++){
            printf("%d ",requestvector[j]);
        }
        printf("for process %d is granted \n",process_id,);
        sleep(3); //sleep to simulate process running
    }

    pthread_mutex_lock(&mutex);
    //releases resources if it gets its needs met
    for(int j=0; j<j_nbResource;j++){
        avail[j] = avail[j] + hold[process_id][j];
        hold[process_id][j] = 0;
        //need is not needed anymore, so just dont touch
    }
    pthread_mutex_unlock(&mutex);
    printf("Process %d has finished \n",process_id);
}

/*
Simulates a fault occuring on the system.
*/
void* fault_simulator(void* pr_id){
    int fault_id = (intptr_t)pr_id;
    printf("Simulating fault \n");
    while(1){
        //thread running in bg removing resources with probablility describe in spec
    }
}

/*
Checks for deadlock
*/
void* deadlock_checker(){ //run periodically
    while(1){
        //simple checking if deadlock has occured due to resource fault

        sleep(5);
    }
}

int main()
{

    //init the mutex and semaphores
    pthread_mutex_init(&mutex,NULL);

    //Initialize all inputs to banker's algorithm

    //get number of processes
    printf("Enter number of processes:");
    scanf("%d",&i_nbProcess);

    //get nb of distinct resources
    printf("Enter number of resources:");
    scanf("%d",&j_nbResource);

    //allocate memory for each array
    avail = malloc(j_nbResource * sizeof(int));
    
    //max = malloc(i_nbProcess* j_nbResource * sizeof(int));
    max = malloc(i_nbProcess * sizeof(int));
    for( int j=0; j < i_nbProcess; j++){
        max[j] = malloc(j_nbResource * sizeof(int));
    }

    //hold = malloc(i_nbProcess* j_nbResource * sizeof(int));
    hold = malloc(i_nbProcess * sizeof(int));
    for( int j=0; j < i_nbProcess; j++){
        hold[j] = malloc(j_nbResource * sizeof(int));
    }
    
    //need = malloc(i_nbProcess* j_nbResource * sizeof(int));
    need = malloc(i_nbProcess * sizeof(int));
    for( int j=0; j < i_nbProcess; j++){
        need[j] = malloc(j_nbResource * sizeof(int));
    }

    //get amount of each resource
    printf("Enter available resources: \n");
    for(int j = 0; j<j_nbResource;j++){
        printf("For resource %d:",j);
        scanf("%d",&avail[j]);
    }

    //get max resource claim per process/resource
    printf("Enter maximum resources each process can claim: \n");
    for(int i = 0; i<i_nbProcess;i++){
        for(int j = 0; j<j_nbResource;j++){
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
/*
    //create a thread that takes away resources from the available pool (fault_simulator)
    pthread_t faultThread;
    pthread_create(&faultThread, NULL, fault_simulator,(void*)(intptr_t)i_nbProcess+1);
    
    //create a thread to check for deadlock (deadlock_checker)
    pthread_t deadlockThread;
    pthread_create(&deadlockThread, NULL, process_simulator,(void*)(intptr_t)i_nbProcess+2);
 */   
    //free the memory
    free(max);
    free(hold);
    free(avail);
    free(need);
    pthread_exit(NULL);

    return 0;
}