/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/ 

//Please enter your name and McGill ID below
//Name: Chloe Grosdidier
//McGill ID: 260662138


#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){

    printf("First : Print to stdout \n");

    //open file to write to
    int fd = open("redirectout.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
      
    //copy address of the old location, aka console 
    int oldfd = dup(1);
 
    //change where output goes
    dup2(fd, 1);

    printf("Second : Print to redirectout.txt \n");

    //close file, renders the location available again
    close(fd);

    //change where output goes again
    dup2(oldfd, 1);

    printf("Third : Print to stdout \n");

    return 0;
}
