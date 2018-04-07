#include <stdio.h>  //for printf and scanf
#include <stdlib.h> //for malloc

#define LOW 0
#define HIGH 199
#define START 53

//compare function for qsort
//you might have to sort the request array
//use the qsort function 
// an argument to qsort function is a function that compares 2 quantities
//use this there.
int cmpfuncincrease (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int cmpfuncdecrease (const void * a, const void * b) {
   return ( *(int*)b - *(int*)a );
}

//function to swap 2 integers
void swap(int *a, int *b)
{
    if (*a != *b){
        *a = (*a ^ *b);
        *b = (*a ^ *b);
        *a = (*a ^ *b);
        return;
    }
}

//Prints the sequence and the performance metric
void printSeqNPerformance(int *request, int numRequest)
{
    int i, last, acc = 0;
    last = START;
    printf("\n");
    printf("%d", START);
    for (i = 0; i < numRequest; i++){
        printf(" -> %d", request[i]);
        acc += abs(last - request[i]);
        last = request[i];
    }
    printf("\nPerformance : %d\n", acc);
    return;
}

//access the disk location in FCFS
void accessFCFS(int *request, int numRequest)
{
    //simplest part of assignment
    printf("\n----------------\n");
    printf("FCFS :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SSTF
void accessSSTF(int *request, int numRequest)
{
    int currentPosition = START;
    int *closest;

    for (int i=0; i<numRequest; i++){
        for (int j=i; j<numRequest; j++){
            if (abs(currentPosition - request[j]) < abs(currentPosition - &closest)){ //if distance from current to request[j] is smaller than the current smallest distance 
                closest = request[j]; //update current closest
            }
            currentPosition = &closest; //update new current position with the smallest distance found, so as to compare rest of array according to that position
            swap(closest,request[i]); //update array         
        }
    }

    printf("\n----------------\n");
    printf("SSTF :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SCAN
void accessSCAN(int *request, int numRequest){
    
    int currentPosition = START;
    int *smaller,*bigger,nbSmaller,nbBigger,*merged,nbMerged;
    nbMerged = 0;

    //seperate the input array into 2
    for(int i=0;i<numRequest;i++){
        if(request[i] < currentPosition){
            smaller[nbSmaller] = request[i];
            nbSmaller++;
        }
        else{
            bigger[nbBigger] = request[i];
            nbBigger++;
        } 
    }

    if(currentPosition < HIGH/2 ){ //if we begin in the lower part of the disk, go down the disk
        qsort(smaller,nbSmaller, sizeOf(int), cmpfuncdecrease);
        qsort(bigger,nbBigger,sizeOf(int), cmpfuncincrease);
        //merge into new array, and put in the first track in between
        for(int i=0;i<nbSmaller;i++){
            merge[nbMerged] = smaller[i];
            nbMerged++;
        }
        if(nbBigger != 0){//ensures we don't write the first track if there are no more elements
            merge[nbMerged] = LOW; //insert first track, then increment
            nbMerged++;
            for(int i=0;i<nbBigger;i++){
                merge[nbMerged] = bigger[i];
                nbMerged++;
            }
         }
    }
    else if(currentPosition >= HIGH/2){ //if we begin in the higher part of the disk, go up the disk
        qsort(smaller, nbSmaller, sizeOf(int), cmpfuncincrease);
        qsort(bigger,nbBigger,sizeOf(int), cmpfuncdecrease);
        //merge into new array, and put in the last track in between
        for(int i=0;i<nbBigger;i++){
           merge[nbMerged] = bigger[i];
           nbMerged++;
        }
        if(nbSmaller != 0){//ensures we don't write the last track if there are no more elements
            merge[nbMerged] = HIGH;//insert last track, then decrement
            nbMerged++;
            for(int i=0;i<nbSmaller;i++){
                merge[nbMerged] = smaller[i];
                nbMerged++;
            }
        }
    }

    printf("\n----------------\n");
    printf("SCAN :");
    printSeqNPerformance(merge, nbMerged);
    printf("----------------\n");
    return;
}

//access the disk location in CSCAN
void accessCSCAN(int *request, int numRequest)
{
    
    int currentPosition = START;
    int *smaller,*bigger,nbSmaller,nbBigger,*merged,nbMerged;
    nbMerged = 0;

    //seperate the input array into 2
    for(int i=0;i<numRequest;i++){
        if(request[i] < currentPosition){
            smaller[nbSmaller] = request[i];
            nbSmaller++;
        }
        else{
            bigger[nbBigger] = request[i];
            nbBigger++;
        } 
    }

    if(currentPosition < HIGH/2 ){ //if we begin in the lower part of the disk, go down the disk
        qsort(smaller,nbSmaller, sizeOf(int), cmpfuncdecrease);
        qsort(bigger,nbBigger,sizeOf(int), cmpfuncdecrease);
        //merge into new array, and put in the first and last tracks in between
        for(int i=0;i<nbSmaller;i++){
            merge[nbMerged] = smaller[i];
            nbMerged++;
        }
        if(nbBigger != 0){//ensures we don't write the extreme tracks if there are no more elements
            merge[nbMerged] = LOW; //insert first track, then increment nb
            nbMerged++;
            merge[nbMerged] = HIGH;//insert last track, then increment nb
            nbMerged++;
            for(int i=0;i<nbBigger;i++){
                merge[nbMerged] = bigger[i];
                nbMerged++;
            }
         }
    }
    else if(currentPosition >= HIGH/2){ //if we begin in the higher part of the disk, go up the disk
        qsort(smaller, nbSmaller, sizeOf(int), cmpfuncincrease);
        qsort(bigger,nbBigger,sizeOf(int), cmpfuncincrease);
        //merge into new array, and put in the last and first tracks in between
        for(int i=0;i<nbBigger;i++){
           merge[nbMerged] = bigger[i];
           nbMerged++;
        }
        if(nbSmaller != 0){//ensures we don't write the extreme tracks if there are no more elements
            merge[nbMerged] = HIGH;//insert last track, then increment nb
            nbMerged++;
            merge[nbMerged] = LOW; //insert first track, then increment nb
            nbMerged++;
            for(int i=0;i<nbSmaller;i++){
                merge[nbMerged] = smaller[i];
                nbMerged++;
            }
        }
    }

    printf("\n----------------\n");
    printf("CSCAN :");
    printSeqNPerformance(merge, nbMerged);
    printf("----------------\n");
    return;
}

//access the disk location in LOOK
void accessLOOK(int *request, int numRequest){
    int currentPosition = START;
    int *smaller,*bigger,nbSmaller,nbBigger,*merged,nbMerged;
    nbMerged = 0;

    //seperate the input array into 2
    for(int i=0;i<numRequest;i++){
        if(request[i] < currentPosition){
            smaller[nbSmaller] = request[i];
            nbSmaller++;
        }
        else{
            bigger[nbBigger] = request[i];
            nbBigger++;
        } 
    }

    if(currentPosition < HIGH/2 ){ //if we begin in the lower part of the disk, go down the disk
        qsort(smaller,nbSmaller, sizeOf(int), cmpfuncdecrease);
        qsort(bigger,nbBigger,sizeOf(int), cmpfuncincrease);
        //merge into new array
        for(int i=0;i<nbSmaller;i++){
            merge[nbMerged] = smaller[i];
            nbMerged++;
        }
        for(int i=0;i<nbBigger;i++){
            merge[nbMerged] = bigger[i];
            nbMerged++;
        }
    }
    else if(currentPosition >= HIGH/2){ //if we begin in the higher part of the disk, go up the disk
        qsort(smaller, nbSmaller, sizeOf(int), cmpfuncincrease);
        qsort(bigger,nbBigger,sizeOf(int), cmpfuncdecrease);
        //merge into new array, and put in the last track in between
        for(int i=0;i<nbBigger;i++){
           merge[nbMerged] = bigger[i];
           nbMerged++;
        }
        for(int i=0;i<nbSmaller;i++){
            merge[nbMerged] = smaller[i];
            nbMerged++;
        }
    }

    printf("\n----------------\n");
    printf("LOOK :");
    printSeqNPerformance(merge, nbMerged);
    printf("----------------\n");
    return;
}

//access the disk location in CLOOK
void accessCLOOK(int *request, int numRequest){
    int currentPosition = START;
    int *smaller,*bigger,nbSmaller,nbBigger,*merged,nbMerged;
    nbMerged = 0;

    //seperate the input array into 2
    for(int i=0;i<numRequest;i++){
        if(request[i] < currentPosition){
            smaller[nbSmaller] = request[i];
            nbSmaller++;
        }
        else{
            bigger[nbBigger] = request[i];
            nbBigger++;
        } 
    }

    if(currentPosition < HIGH/2 ){ //if we begin in the lower part of the disk, go down the disk
        qsort(smaller,nbSmaller, sizeOf(int), cmpfuncdecrease);
        qsort(bigger,nbBigger,sizeOf(int), cmpfuncdecrease);
        //merge into new array
        for(int i=0;i<nbSmaller;i++){
            merge[nbMerged] = smaller[i];
            nbMerged++;
        }
        for(int i=0;i<nbBigger;i++){
            merge[nbMerged] = bigger[i];
            nbMerged++;
        }
    }
    else if(currentPosition >= HIGH/2){ //if we begin in the higher part of the disk, go up the disk
        qsort(smaller, nbSmaller, sizeOf(int), cmpfuncincrease);
        qsort(bigger,nbBigger,sizeOf(int), cmpfuncincrease);
        //merge into new array, and put in the last track in between
        for(int i=0;i<nbBigger;i++){
           merge[nbMerged] = bigger[i];
           nbMerged++;
        }
        for(int i=0;i<nbSmaller;i++){
            merge[nbMerged] = smaller[i];
            nbMerged++;
        }
    }

    printf("\n----------------\n");
    printf("CLOOK :");
    printSeqNPerformance(merge,nbMerged);
    printf("----------------\n");
    return;
}

int main()
{
    int *request, numRequest, i,ans;

    //allocate memory to store requests
    printf("Enter the number of disk access requests : ");
    scanf("%d", &numRequest);
    request = malloc(numRequest * sizeof(int));

    printf("Enter the requests ranging between %d and %d\n", LOW, HIGH);
    for (i = 0; i < numRequest; i++)
    {
        scanf("%d", &request[i]);
    }

    printf("\nSelect the policy : \n");
    printf("----------------\n");
    printf("1\t FCFS\n");
    printf("2\t SSTF\n");
    printf("3\t SCAN\n");
    printf("4\t CSCAN\n");
    printf("5\t LOOK\n");
    printf("6\t CLOOK\n");
    printf("----------------\n");
    scanf("%d",&ans);

    switch (ans)
    {
    //access the disk location in FCFS
    case 1: accessFCFS(request, numRequest);
        break;

    //access the disk location in SSTF
    case 2: accessSSTF(request, numRequest);
        break;

        //access the disk location in SCAN
     case 3: accessSCAN(request, numRequest);
        break;

        //access the disk location in CSCAN
    case 4: accessCSCAN(request,numRequest);
        break;

    //access the disk location in LOOK
    case 5: accessLOOK(request,numRequest);
        break;

    //access the disk location in CLOOK
    case 6: accessCLOOK(request,numRequest);
        break;

    default:
        break;
    }
    return 0;
}