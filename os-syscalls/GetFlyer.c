//HW1 on Operation Systems
//by Alex Pitkin and Ilya Vorotyntsev

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFF_SIZE 128

int main(int argc, char* argv[]){

    if (argc != 2){
        printf("ERROR: wrong amount of arguments!\n");
        return (-1);
    }

    char path[BUFF_SIZE] = "Black_Friday/";
	int id;

    //creating a path to the file
    strcat(path,argv[1]);
    strcat(path,".txt");

	if(access(path, F_OK) != 0)//checks if process can access the file pathname, F_OK tests for the existence
    {
        printf("Company %s Not Found! Try to make a Flyer first.\n", argv[1]);
        exit(1);
    }

	if((id = fork()) == 0)
    {
        execlp("cat","cat",path,NULL);
        perror("ERROR: Cannot open company flyer!");
        exit(1);
    }
    wait(); //we dont want to return into Adv_shell before print
    return 0;

    
    return 0;
}
