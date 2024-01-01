//HW1 on Operation Systems
//by Alex Pitkin and Ilya Vorotyntsev

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFF_SIZE 128

int main(int argc, char* argv[]){
    
    
    //because we know whic programs we use, so we prepare all arguments
    char * argsComp[2] = {"GetNumComp",NULL},
           arg_name[BUFF_SIZE],
         * argsOrder[3] = {"GetOrderNum", arg_name,NULL};
    int pid;
    
    int fd, rbytes, char_index = 0;
    char symbol, name[BUFF_SIZE] = "";
    
    
    if (argc != 1){
        printf("ERROR: wrong amount of arguments!\n");
        return (-1);
    }

    //VIA GETNUMCOMP PRINTING AMOUNT COMPANIES
    if((pid = fork()) == 0){
        execv("GetNumComp", argsComp);// -> for our progs
    }
    wait();

    //VIA GETORDERNUM AMOUNT OF ORDERS
    if((fd = open("Black_Friday/camp_partic.txt", O_RDONLY, NULL)) == -1){
        printf("ERROR: Could not open the file.\n");
        return -1;;
    }
    
    while((rbytes = read(fd, &symbol, 1)) > 0){
        if(symbol == '\n'){//each company line ends with \n, so it can say amount of companies
            name[strlen(name)] = '\0';
            strcpy(arg_name, name);
            if((pid = fork()) == 0){
                execv("GetOrderNum", argsOrder);
            }
            wait();
            strcpy(name,"");// ready for the next
            char_index = 0;
        }
        else{
            name[char_index] = symbol;
            char_index++;
        }    
    }
    close(fd);
    return 0;
}    