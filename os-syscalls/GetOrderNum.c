//HW1 on Operation Systems
//by Alex Pitkin and Ilya Vorotyntsev

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


#define BUFF_SIZE 128

int main(int argc, char* argv[]){

    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;
    char path[BUFF_SIZE] = "Black_Friday/";

    strcat(path,argv[1]);
    strcat(path,"_Order");
    
    
    if (argc != 2){
        printf("ERROR: wrong amount of arguments!\n");
        return (-1);
    }
    //OPEN DIR AND STARTING TO COUNT ALL FILES
    dirp = opendir(path);
    if (dirp == NULL){
        printf("ERROR: Company %s Not Found!\n", argv[1]);
        exit(1);
    }
    while ((entry = readdir(dirp)) != NULL) {
        file_count++;
    }
    closedir(dirp);
    printf("%s ---- %d Orders\n", argv[1], file_count - 2);//need -2
    return 0;
}