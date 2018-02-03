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

    int fd[2];
    pipe(fd);

    if (fork() == 0)
    {
        //rewire output from here
        close(1);
        dup(fd[1]);
        execvp(arg[0], arg); 
    }
    else
    {
        //rewire input here
        close(0);
        dup(fd[0]);
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        printf("%s", readbuffer);
    }
    return 0;
}
