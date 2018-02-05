/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/ 

//Please enter your name and McGill ID below
//Name: Chloe Grosdidier
//McGill ID: 260662138

//all the header files you would require
#include <stdio.h>  //for standard IO
#include <unistd.h> //for execvp/dup/dup2
#include <string.h> //for string manipulation
#include <stdlib.h> //for fork  
#include <ctype.h>  //for character type check (isnum,isalpha,isupper etc)
#include <sys/wait.h>//for waitpid
#include <fcntl.h>  //open function to open a file. type "man 2 open" in terminal
#include <time.h>   //to handle time

//pointer to Linked list head
struct node *head_job = NULL;

//pointer to
struct node *current_job = NULL;

//global variable used to store process id of process
//that has been just created
//can be used while adding a job to linked list
pid_t process_id;

//flag variable to check if redirection of output is required
int isred = 0;

//global job number variable to ensure no two have the same id
int job_number = 1;

//structure of a single node
//donot modify this structure
struct node
{
    int number;        //the job number
    int pid;           //the process id of the process
    char *cmd;         //string to store the command name
    time_t spawn;      //time to store the time it was spawned
    struct node *next; //when another process is called you add to the end of the linked list
};

// Add a job to the linked list
void addToJobList(char *args[])
{
    //allocate memory for the new job
    struct node *job = malloc(sizeof(struct node));
    //If the job list is empty, create a new head
    if (head_job == NULL)
    {
        //init the job number with 1
        job->number = job_number;
        //set its pid from the global variable process_id
        job->pid = process_id;
        //cmd can be set to arg[0]
        job->cmd = args[0];
        //set the job->next to point to NULL.
        job->next = NULL;
        //set the job->spawn using time function
        job->spawn = (unsigned int)time(NULL);
        //set head_job to be the job
        head_job = job;
        //set current_job to be head_job
        current_job = head_job;
    }

    //Otherwise create a new job node and link the current node to it
    else
    {
        //point current_job to head_job
        current_job = head_job;
        
        //traverse the linked list to reach the last job
       while (current_job->next != NULL) {
		 current_job = current_job->next; //go through linked list
		  }

        //init all values of the job like above num,pid,cmd.spawn
	job_number++;
        job->number = job_number; //new job gets the next available number
        job->pid = process_id; //assign id from global pid_t
        job->cmd = args[0];
        job->spawn = (unsigned int)time(NULL);

        //make next of current_job point to job
        current_job->next = job;

        //make job to be current_job
         current_job = job;
        
        //set the next of job to be NULL
        job->next = NULL;
        
    }
}

//Function to refresh job list
//Run through jobs in linked list and check
//if they are done executing then remove it
void refreshJobList()
{
    //pointer require to perform operation 
    //on linked list
    struct node *current_job;
    struct node *prev_job;
    
    //variable to store returned pid 
    pid_t ret_pid;

    //perform init for pointers
    current_job = head_job;
    prev_job = head_job;

    //traverse through the linked list
    while (current_job != NULL){
        //use waitpid to init ret_pid variable
        ret_pid = waitpid(current_job->pid, NULL, WNOHANG);
        //one of the below needs node removal from linked list
        if (ret_pid == 0){
		//if equal to 0, then the process is not done yet
		//continue going through the list
			//if (current_job != head_job){
				prev_job = current_job; //only update prev_job to current job if it is not the first job in the list
			//}
			current_job = current_job->next; //continue going through linked list
		}
        else {
		//if waitpid returns a number that is not 0 or not -1, then it means the 
		//process has changed status, and so is done
		//remove the job from the linked list
		
        /*	if (current_job == head_job){//if the job to remove is the 1st in the list
				current_job = current_job->next; //update current job
				free(head_job);
                head_job = current_job; //remove the job by updating the head to point to the next job
                prev_job = current_job;
            }
			else if (current_job->next == NULL){//if the job to remove is the last of the list
                free(current_job);
                current_job = prev_job;
            	prev_job->next = NULL; //update the previous job to point to null instead of the job to delete	
			}
			else { //any other job
				current_job = current_job->next; //update current job pointer
				free(prev_job->next);
                prev_job->next = current_job; //make previous job point to next job instead of the job to delete
			}       
        }
        */
                if(current_job->next == NULL) //then this node is at the very end
                {
                    free(current_job);        //deallocate memory
                    current_job = NULL;       //set to NULL to be save before reassigning it again
                    current_job = prev_job;
                    current_job->next = NULL; //now the previous job is the at the end of the list
                } 
                else if(current_job == head_job && (current_job->next != NULL)) //remove the job from the front of the list
                {
                    free(current_job);            //deallocate the memory
                    head_job = head_job->next;    //the second job in line becomes the new head_job
                    prev_job = head_job;          //now everything points to the new head_job
                    current_job = head_job; 
                }
                else if(current_job->next != NULL) //if it reaches this point, then the job is somewhere in between the head and the tail
                {
                    current_job = current_job->next;
                    free(prev_job->next);           //deallocate what current_job was previosuly pointing to
                    prev_job->next = NULL;
                    prev_job->next = current_job;   //and then connect previos job to the new current job
                }
            }

    }
    return;
}

//Function that list all the jobs
void listAllJobs()
{
    struct node *current_job;
    int ret_pid;

    //refresh the linked list
    refreshJobList();
    //init current_job with head_job
    current_job = head_job;
    //heading row print only once.
    printf("\nID\tPID\tCmd\tstatus\tspawn-time\n");    
    //traverse the linked list and print using the following statement for each job
    while(current_job != NULL){
		printf("%d\t%d\t%s\tRUNNING\t%s\n", current_job->number, current_job->pid, current_job->cmd, ctime(&(current_job->spawn))); //print specs
        current_job = current_job->next; //continue going through linked list
    }
    return;
}

// wait till the linked list is empty
// you would have to look for a place 
// where you would call this function.
// donot modify this function
void waitForEmptyLL(int nice, int bg)
{
    if (nice == 1 && bg == 0)
    {
        while (head_job != NULL)
        {
	    printf("checking linked list/n");
            sleep(1);
            refreshJobList();
        }
    }
    return;
}

//function to perform word count
 int wordCount(char *filename,char* flag)
 {
     int cnt =0;
	 char buffer[2000];
	 int bytes_read; 
	FILE *file;
	file = fopen(filename, "r");
 // opening file
     //if flag is l 
     //count the number of lines in the file 
     //set it in cnt

     if (flag == NULL){
        fprintf(stderr,"Unrecognized flag\n");
        return 0;     
     }
      if (file == NULL ) {
        fprintf(stderr, "The file does not exist\n");
        return 0;
    }

fclose(file);

int fd = open(filename, O_RDONLY); 

	 if (!strcmp("-l", flag)) {
		 while (bytes_read = read(fd, buffer, sizeof(buffer))) {
			 int i;
			 for (i = 0; i < bytes_read; i++) {
				 if (buffer[i] == '\n') {
					 cnt++;
				 }
			 }
		 }
	 }

     //if flag is w
     //count the number of words in the file
     //set it in cnt
	 else if (!strcmp("-w", flag)) {
		 while (bytes_read = read(fd, buffer, sizeof(buffer))) {
			 int i;
			 for (i = 0; i < bytes_read; i++) {
				 if (buffer[i] == '\n' || buffer[i] == ' ') {
					 cnt++;
				 }
			 }
             if (buffer[bytes_read-1] == '.'){
                 cnt++;
             }
		 }

	 }
     else{
        fprintf(stderr, "Unrecognized flag\n");
        return 0;
     }
     close(fd);
     return cnt;
 }

// function to augment waiting times for a process
// donot modify this function
void performAugmentedWait()
{
    int w, rem;
    time_t now;
    srand((unsigned int)(time(&now)));
    w = rand() % 15;
    printf("sleeping for %d\n", w);
    rem = sleep(w);
    return;
}

//simulates running process to foreground
//by making the parent process wait for
//a particular process id.
int waitforjob(char *jobnc)
{//TO-DO THIS
    struct node *trv;
    int jobn = atoi(jobnc);
    trv = head_job;
    //traverse through linked list and find the corresponding job
    //hint : traversal done in other functions too

    while(trv!=NULL){
        if(trv->number == jobn){
	    printf("Bringing jobno %d and pid %d to foreground", jobn, trv->pid);
	    waitpid(trv->pid, NULL, WUNTRACED);
        return 0; //check if it works
        }
        trv = trv->next;
    }
    printf("Job number %d does not exist",jobn);
         //if correspoding job is found 
        //use its pid to make the parent process wait.
        //waitpid with proper argument needed here
    
    
    return -1;
}

// splits whatever the user enters and sets the background/nice flag variable
// and returns the number of tokens processed
// donot modify this function
int getcmd(char *prompt, char *args[], int *background, int *nice)
{
    int length, i = 0;
    char *token, *loc;
    char *line = NULL;
    size_t linecap = 0;
    printf("%s", prompt);
    length = getline(&line, &linecap, stdin);
    if (length <= 0)
    {
        exit(-1);
    }
    // Check if background is specified..
    if ((loc = index(line, '&')) != NULL)
    {
        *background = 1;
        *loc = ' ';
    }
    else
        *background = 0;
    while ((token = strsep(&line, " \t\n")) != NULL)
    {
        for (int j = 0; j < strlen(token); j++)
            if (token[j] <= 32)
                token[j] = '\0';
        if (strlen(token) > 0)
        {
            if (!strcmp("nice", token))
            {
                *nice = 1;
            }
            else
            {
                args[i++] = token;
            }
        }
    }
    return i;
}

// this initialises the args to All null.
// donot modify
void initialize(char *args[])
{
    for (int i = 0; i < 20; i++)
    {
        args[i] = NULL;
    }
    return;
}

int main(void)
{
    //args is a array of charater pointers
    //where each pointer points to a string
    //which may be command , flag or filename
    char *args[20];

    //flag variables for background, status and nice
    //bg set to 1 if the command is to executed in background
    //nice set to 1 if the command is nice
    //status  
    int bg, status, nice;

    //variable to store the process id.
    pid_t pid;

    //variable to store the file des
    //helpful in output redirection
    int fd1, fd2;

    //your terminal executes endlessly unless 
    //exit command is received
    while (1)
    {
        //init background to zero
        bg = 0;
        //init nice to zero
        nice = 0;
        //init args to null
        initialize(args);
        //get the user input command
        int cnt = getcmd("\n>> ", args, &bg, &nice);
        //keep asking unless the user enters something
        while (!(cnt >= 1))
            cnt = getcmd("\n>> ", args, &bg, &nice);

        //use the if-else ladder to handle built-in commands
        //built in commands don't need redirection
        //also no need to add them to jobs linked list
        //as they always run in foreground
        if (!strcmp("jobs", args[0]))
        {
            //call the listalljobs function
            listAllJobs();
        }
        else if (!strcmp("exit", args[0]))
        {
            //exit the execution of endless while loop 
            exit(0);
        }
        else if (!strcmp("fg", args[0]))
        {
            //bring a background process to foregrounf
            waitforjob(args[1]);
        }
        else if (!strcmp("cd", args[0]))
        {
            int result = 0;
            if(args[1] == NULL){// if no destination directory given
				chdir(getenv("HOME"));// change to home directory 
            }
	    else if (chdir(args[1]) != 0){
	printf("cd: %s: No such file or directory", args[1]);


	    }
        }
        else if (!strcmp("pwd", args[0]))
        {
            //use getcwd and print the current working directory
			char directory[200];
			if (getcwd(directory, sizeof(directory)) == NULL) {
				printf("error"); //handle this maybe
			}
			else {
				printf("%s\n", directory);
			}
        }
        else if(!strcmp("wc",args[0]))
        {
            //call the word count function	
            int counted = wordCount(args[2],args[1]);
		    printf("%d\n", counted);
        }
        else
        {
            //Now handle the executable commands here 
            /* the steps can be..:
            (1) fork a child process using fork()
            (2) the child process will invoke execvp()
            (3) if background is not specified, the parent will wait,
                otherwise parent starts the next command... */


            //hint : samosas are nice but often there 
            //is a long waiting line for it.

            if (nice ==1){
                waitForEmptyLL(nice, bg);
            }


            //create a child
            pid = fork();

            //to check if it is parent
            if (pid > 0)
            {
                //we are inside parent
                //printf("inside the parent\n");
                if (bg == 0)
                {
                    //FOREGROUND
                    // waitpid with proper argument required
                    waitpid(pid, NULL, WUNTRACED);
                }
                else
                {
                    //BACKGROUND
                    process_id = pid;
                    addToJobList(args);
                    // waitpid with proper argument required
                   // waitpid(pid, NULL, WNOHANG);
                }
            }
            else
            {
                // we are inside the child

                //introducing augmented delay
                performAugmentedWait();

                //check for redirection
                //now you know what does args store
                //check if args has ">"
                //if yes set isred to 1
                //else set isred to 0

                //if (*args[1] == '>'){
		int index =0;
		while(args[index]!=NULL){
			if (!strcmp(">", args[index])){
                    	isred = 1;
			break;
               		}
			else{
			isred = 0;
			}
		index++;
                }


                //if redirection is enabled
                 if (isred == 1)
                {
                    //open file and change output from stdout to that  
                    //make sure you use all the read write exec authorisation flags
                    //while you use open (man 2 open) to open file
                    int fileoutputfd = open(args[index+1], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
                   // int oldoutputfd = dup(1);
                    dup2(fileoutputfd, 1);

                    //set ">" and redirected filename to NULL
                    args[index] = NULL;
                    args[index + 1] = NULL;

                    //run your command
                    execvp(args[0], args);

                    //restore to stdout
                    fflush(stdout);
                }
                else
                {
                    //simply execute the command.
                    execvp(args[0], args);
                }
            }
        }
    }

    return 0;
}
