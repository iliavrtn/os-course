//HW1 on Operation Systems
//by Alex Pitkin and Ilya Vorotyntsev

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFF_SIZE 128

int main(){
    
    int fd_to, pid, amount_args = 1;
    char* command_line,* args[5],* token;
    
    //creating a Black_Friday directory
    if (mkdir("Black_Friday", 0777) == -1)
	{
	    printf("Error: Can't create Black_Friday directory\n");
		exit(1);
	}
	printf("// Black_Friday dir created, ");
    
    //creating a camp_partic.txt file
    if ((fd_to = open("/home/braude/Black_Friday/camp_partic.txt", O_WRONLY | O_CREAT, 0664)) == -1)
	{   
	    perror("Error: Can't create camp_partic.txt file\n"); return(-1);
	}
	printf("camp_partic.txt created\n");
	close(fd_to);

    //starting to work with AdvShell
    printf("AdvShell>");
    fgets(command_line, BUFF_SIZE, stdin);
    command_line[strlen(command_line) - 1] = '\0';
   
    while (strcmp(command_line,"exit")){ //only exit finishing the program

        pid = fork();
        if (pid == 0){ //child make all the work
            token = strtok(command_line," ");// taking the command
            args[0] = token;
            
            while ((token = strtok(NULL," ")) != NULL){ //taking all the args, if they exists
                args[amount_args] = token;
                amount_args++;
            }
            
            args[amount_args] = NULL; //SHOULD PUT IT in the end of the arguments
            
            //starting a needable command
            execvp(args[0], args);// -> for build in progs
            execv(args[0], args);// -> for our progs
            
            printf("%s command is Not Supported, try again!\n", args[0]); //if exec is fail -> no such command
            return(-1);//we should exit from ben
        }
        wait(); //dad should wait to the child
        amount_args = 1; //for the next arguments
        
        //ready for the next command_line
        printf("AdvShell>");
        fgets(command_line, BUFF_SIZE, stdin);
        command_line[strlen(command_line) - 1] = '\0';

    }
    execl("exit", "exit", NULL);
    //if exit is not working
    printf("Error: the 'exit' program is not working!\n");
    return 1;
} 