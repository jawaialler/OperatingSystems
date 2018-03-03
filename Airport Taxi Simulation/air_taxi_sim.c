/*
 ----------------- COMP 310/ECSE 427 Winter 2018 -----------------
 Authors: Dimitri Gallos - skeleton
 Chloe Grosdidier - implementation
 -----------------------------------------------------------------
 I declare that the awesomeness below is a genuine piece of work
 and falls under the McGill code of conduct, to the best of my knowledge.
 -----------------------------------------------------------------
 */

//Please enter your name and McGill ID below
//Name: Chloe Grosdidier
//McGill ID: 260662138

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>

int BUFFER_SIZE = 100; //size of queue
int ONE_ID_POS = 1000000; //passenger id follows 1ZZZYYY
int Z_ID_POS = 1000;
int Y_ID_POS = 1;
pthread_mutex_t mutex; //shared semaphore
sem_t fillCount; //producer semaphore
sem_t emptyCount; //consumer semapore

// A structure to represent a queue
struct Queue{
    int front, rear, size;
    unsigned capacity;
    int* array;
};
 
// function to create a queue of given capacity. 
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity){
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}
 
// Queue is full when size becomes equal to the capacity 
int isFull(struct Queue* queue){
    return ((queue->size ) >= queue->capacity);
}
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue){
    return (queue->size == 0);
}
 
// Function to add an item to the queue.  
// It changes rear and size
void enqueue(struct Queue* queue, int item){
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue. 
// It changes front and size
int dequeue(struct Queue* queue){
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue* queue){
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}
 
// Function to get rear of queue
int rear(struct Queue* queue){
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

void print(struct Queue* queue){
    if (queue->size == 0){
        return;
    }
    
    for (int i = queue->front; i < queue->front +queue->size; i++){
        
        printf(" Element at position %d is %d \n ", i % (queue->capacity ), queue->array[i % (queue->capacity)]);
    }
    
}

struct Queue* queue;

/*Producer Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform */
void *FnAirplane(void* cl_id){

    //TO-DO: implement this
//The producer's job is to generate data, put it into the buffer, and start again

//producer: either go to sleep or discard data if the buffer is full.
// The next time the producer puts data into the buffer, it wakes up the sleeping consumer. 
//The solution can be reached by means of inter-process communication, typically using semaphores.
    while(1){
        srand(time(NULL));
        int num_passengersDropped = rand()%6 + 5; //generate the number of passengers going to taxi platform (5-10)
        printf("Airplaine %d arrives with %d passengers\n",cl_id, num_passengersDropped);

//put some mutex shit here
        if(isFull(queue)){
            printf("Platform is full: Rest of passengers of plane 2 take the bus\n",cl_id);
        }
        else {
            for(int p = 0; p < num_passengersDropped; p++){
                pthread_mutex_lock(&mutex);
                int passenger_id = (1*ONE_ID_POS) + (cl_id*Z_ID_POS) + (p*Y_ID_POS);
                printf("Passenger %d of airplane %d arrives to platform\n",passenger_id, cl_id);
                enqueue(queue, passenger_id);
                pthread_mutex_unlock(&mutex);
            }   
        }
    sleep(1); //sleep thread for 1 second = 1 hour
    }
}

/* Consumer Function: simulates a taxi that takes n time to take a passenger home and come back to the airport */
void *FnTaxi(void* pr_id){

  //TO-DO: implement this
// consumer is consuming the data (i.e., removing it from the buffer), one piece at a time

//The problem is to make sure that the producer won't try to add data into the buffer if it's full and that the consumer won't try to remove data from an empty buffer
// next time the consumer removes an item from the buffer, it notifies the producer, who starts to fill the buffer again. 
    printf("Taxi driver %d arrives\n",pr_id);

    if(isEmpty(queue)){
        printf("Taxi driver %d waits for passengers to enter the platform\n",pr_id);
    }
    else{
        pthread_mutex_lock(&mutex);
        int passenger_boarding = dequeue(queue);
        printf("Taxi driver %d picked up client %d from the platform\n",pr_id, passenger_boarding);
        pthread_mutex_unlock(&mutex);
    }
    
    srand(time(NULL));
    int tripTime = (rand()%21 + 10)/60; //generate the time the taxi takes to drop someone (10min-30min = 0.167 hours to 0.5 hours = 0.167 seconds to 0.5 seconds )
    usleep(tripTime);
}




int main(int argc, char *argv[]){

  int num_airplanes;
  int num_taxis;

  num_airplanes=atoi(argv[1]);
  num_taxis=atoi(argv[2]);
  
  printf("You entered: %d airplanes per hour\n",num_airplanes);
  printf("You entered: %d taxis\n", num_taxis);
  
  
  //initialize queue
  queue = createQueue(BUFFER_SIZE);
  
  //declare arrays of threads and initialize semaphore(s)

  sem_init(&fillCount, 0, BUFFER_SIZE);
  sem_init(&emptyCount, 0, 0);
  pthread_mutex_init(&mutex);

  pthread_t airplaneThread[num_airplanes];
  pthread_t taxiThread[num_taxis];
  

  //create arrays of integer pointers to ids for taxi / airplane threads
  int *airplane_ids[num_airplanes];
  int *taxi_ids[num_taxis]; 
  
  //create threads for airplanes

   for(int i=0; i< num_airplanes; i++){
       // start a new thread for every airplane required
    pthread_create(&airplaneThread[i], NULL, FnAirplane,&i);
    printf("Creating airplane thread %d\n",i);
   }

  //create threads for taxis
    for(int j=0; j< num_taxis; j++){
        // start a new thread for every taxi required
     pthread_create(&taxiThread[j], NULL, FnTaxi,&j);
    }
  
  pthread_exit(NULL);
}
