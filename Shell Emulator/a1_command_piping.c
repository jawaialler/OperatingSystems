/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/ 

//Please enter your name and McGill ID below
//Name: Chloe Grosdidier
//McGill ID: 260662138


#include <stdio.h>
#include <unistd.h>
int main()
{

    int nbytes;
    char readbuffer[120];
    char *arg[] = {"ls", NULL};

    //set up pipe to transfer the string between processes
    int fd[2];
    pipe(fd);

    if (fork() == 0)
    {//parent
        //rewire output from here
        close(1); //close current output
        dup(fd[1]); //place the receptor of the pipe in stdout
        execvp(arg[0], arg); //execute ls
    }
    else
    {//child
        //rewire input here
        close(0); //close current input
        dup(fd[0]); //place the output of the pipe in stdin
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer)); //read the pipe output
        printf("%s", readbuffer); //print the string in stdout
    }
    return 0;
}
