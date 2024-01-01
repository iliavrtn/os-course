//HW1 on Operation Systems
//by Alex Pitkin and Ilya Vorotyntsev

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFF_SIZE 128

void clear_stdin();

void clear_stdin()
{
    // keep reading 1 more char as long as the end of the stream, indicated by
    // `EOF` (end of file), and the end of the line, indicated by the newline
    // char inserted into the stream when you pressed Enter, have NOT been
    // reached
    while (1)
    {
        int c = getc(stdin);
        if (c == EOF || c == '\n')
        {
            break;
        }
    }
}

int main(int argc, char* argv[]){

    char* arr_name[BUFF_SIZE], * arr_price[BUFF_SIZE], product_line[BUFF_SIZE], txt_choice[BUFF_SIZE], *tmp_str;
    int amount_pr = 0, name_ch = 0, choice;

    char path[BUFF_SIZE] = "Black_Friday/", ready_row[BUFF_SIZE];
    int fd_to, wbytes, i, j;

    //CHEKING AMOUNT ARGUMENTS
    if (argc != 3){
        printf("ERROR: wrong amount of arguments!\n");
        return (-1);
    }
    
    printf(">>Welcome to CreateFlyer programm!<<\n");

    printf("Insert Item and Item cost ('Stop' to finish):\n");
    while(1) {
        
        fgets(product_line, BUFF_SIZE, stdin);
        product_line[strlen(product_line) - 1] = '\0'; //deleting the '\n' symbol

        if (!strcmp(product_line, "Stop")) {//last argument in product row
            break;
        }

        //counting the amount of chars in price
        for (name_ch = (strlen(product_line) - 1); name_ch > 0; name_ch--){
            if (product_line[name_ch] == ' ')
                break;        
        }
        //saving the name of product into array
        arr_name[amount_pr] = (char*)malloc(name_ch);
        strncpy(arr_name[amount_pr], product_line, name_ch);
        arr_name[amount_pr][name_ch] = '\0';

        //saving the price of product into array
        arr_price[amount_pr] = (char*)malloc(strlen(product_line) - name_ch - 1);//-1 becauase of " "
        tmp_str = product_line;
        tmp_str += name_ch + 1; //mooving a pointer to thee price
        strncpy(arr_price[amount_pr], tmp_str, strlen(product_line) - name_ch - 1);
        arr_price[amount_pr][strlen(product_line) - name_ch - 1] = '\0';

        //ready for the next product
        amount_pr++;
    }
    printf("All products was collected sucessfully!\n");
    
    //starting a choice
    printf("What is your choice?\n1.Enjoyed\n2.Shop Now and Have Fun\n3.Have Fun\n4.Other\n");
    scanf("%d", &choice);
    while (choice < 1 && choice > 4) {
        printf("There is no such option, choose a relevant one(1-4)!\n>");
        scanf("%d", &choice);
    }
    switch (choice) {
        case 1:
            strcpy(txt_choice, "Enjoyed");
            break;
        
        case 2:
            strcpy(txt_choice, "Shop Now and Have Fun");
            break;
        
        case 3:
            strcpy(txt_choice, "Have Fun");
            break;

        case 4:
            printf("Insert your word:\n>>");
            clear_stdin();//without cleaning we receive automatically \n
            fgets(txt_choice, BUFF_SIZE, stdin);
            txt_choice[strlen(txt_choice) - 1] = '\0'; //deleting the '\n' symbol
            break;

        default:
            printf("Error: choice is not correct! May cause damage..");
    }

    //now we starting to REALLY WORK with flyer
    
    //creating a name_Order directory
    strcat(path,argv[1]);
    strcat(path,"_Order");
    if (mkdir(path, 0777) == -1)
	{
	    printf("Error: Can't create %s_Order directory\n", argv[1]);
        return(-1);
	}
    
    
    //CAMP_PARTIC.TXT OPEN AND ADD
    if ((fd_to = open("/home/braude/Black_Friday/camp_partic.txt", O_WRONLY | O_APPEND, 0664)) == -1)
	{   
	    perror("Error: Can't open camp_partic.txt file to write in it.\n"); return(-1);
	}

    //adding a company name to file
    if (( wbytes = write( fd_to, argv[1], strlen(argv[1]))) == -1 ){
		perror( "Cannot write c_name into camp_partic.txt" ); 
        return( -1 ); 
    }
    //adding a space between names
    if(( wbytes = write( fd_to, "\n", 1)) == -1 ){
		perror( "Cannot write space into camp_partic.txt" ); 
        return( -1 ); 
    }
    close(fd_to);

    //starting a flyer.txt
    //creating a relevant path
    strcpy(path,"Black_Friday/");
    strcat(path,argv[1]);
    strcat(path,".txt");
    //opened file
    if ((fd_to = open(path, O_RDWR | O_CREAT, 0664)) == -1)
	{   
	    perror("Error: Can't create name.txt file\n"); 
        return(-1);
	}
    //printing all relevant info into flyer.txt
    for (i = 0; i < amount_pr + 3; i++){
        if (i == 0){ //first row - name Sale
            strcpy(ready_row, "");
            for(j = 0; j < (50 - (strlen(argv[1]) + 5)) / 2; j++){
                strcat(ready_row," ");
            }
            strcat(ready_row, argv[1]);
            strcat(ready_row," Sale\n\n");
            if (( wbytes = write( fd_to, ready_row, strlen(ready_row))) == -1 ){
		        printf( "Cannot write %d ROW into %s.txt", i, argv[1]); 
                return( -1 ); 
            }
        }
        else if (i == 1){ //second row - percent of sale
            strcpy(ready_row, "");
            for(j = 0; j < (50 - (strlen(argv[2]) + 5)) / 2; j++){
                strcat(ready_row," ");
            }
            strcat(ready_row, argv[2]);
            strcat(ready_row,"% off\n\n");
            if (( wbytes = write( fd_to, ready_row, strlen(ready_row))) == -1 ){
		        printf( "Cannot write %d ROW into %s.txt", i, argv[1]); 
                return( -1 ); 
            }
        }
        else if (i == (amount_pr + 2)){ //last row - from choice option
            strcpy(ready_row, "\n");
            for(j = 0; j < (50 - strlen(txt_choice)) / 2; j++){
                strcat(ready_row," ");
            }
            strcat(ready_row, txt_choice);
            strcat(ready_row, "\n");
            if (( wbytes = write( fd_to, ready_row, strlen(ready_row))) == -1 ){
		        printf( "Cannot write %d ROW into %s.txt", i, txt_choice); 
                return( -1 ); 
            }
        }
        //other rows with all products
        else{
            strcpy(ready_row, arr_name[i - 2]);
            for(j = 0; j < 50 - ( strlen(arr_name[i - 2]) + strlen(arr_price[i - 2])); j++){
                strcat(ready_row,".");
            }
            strcat(ready_row, arr_price[i - 2]);
            strcat(ready_row, "NIS\n");
            if (( wbytes = write( fd_to, ready_row, strlen(ready_row))) == -1 ){
		        printf( "Cannot write %d ROW into %s.txt", i, argv[1]); 
                return( -1 ); 
            }
        }
    } //+3 because we have 3 extra rows
    printf("Successfully created ");
    printf("// %s.txt created, %s_Order dir created.\n", argv[1], argv[1]);
    return 0;
}