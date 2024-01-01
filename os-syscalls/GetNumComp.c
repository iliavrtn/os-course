//HW1 on Operation Systems
//by Alex Pitkin and Ilya Vorotyntsev

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFF_SIZE 128

int main(int argc, char* argv[]){
    
    if (argc != 1){
        printf("ERROR: wrong amount of arguments!\n");
        return (-1);
    }

    char symbol;
    int fd, rbytes;
    int counter_companies = 0;

    //OPEN FILE AND START COUNTING \n
    if((fd = open("Black_Friday/camp_partic.txt", O_RDONLY, NULL)) == -1){
        printf("ERROR: Could not open the file.\n");
        return -1;
    }
    
    while((rbytes = read(fd, &symbol, 1)) > 0){
        if(symbol == '\n')//each company line ends with \n, so it can say amount of companies
            counter_companies++;
    }
    printf("There are %d companies participating in campaign\n", counter_companies);
    close(fd);

    return 0;
}