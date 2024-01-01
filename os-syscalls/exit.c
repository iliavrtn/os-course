#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    int pid;
    //linux command to deleting dir with all files
    char * args[3] = {"rm", "-rf", "Black_Friday"}; 

    if (argc != 1){
        printf("ERROR: wrong amount of arguments!\n");
        return (-1);
    }
    if((pid = fork()) == 0){
        execvp(args[0], args);// -> for build in progs
    }
    wait();
    printf("Goodbye..\n");    
    return 0;
}
