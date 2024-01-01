 //HW1 on Operation Systems
//by Alex Pitkin and Ilya Vorotyntsev

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <math.h>
#include <time.h>
#define BUFF_SIZE 128



int main(int argc, char* argv[]){

    char* arr_name[BUFF_SIZE], * arr_amount[BUFF_SIZE],
          * flyer = (char*)malloc(sizeof(char)), *tmp_str,
          *p_flyer, *start_search_pos;
    char path[BUFF_SIZE] = "Black_Friday/", product_line[BUFF_SIZE],
         ready_row[BUFF_SIZE], product_price[BUFF_SIZE] = "",
         total_cost_str[BUFF_SIZE], buff, sale[4];
    int pid, from, rbytes, name_ch = 0, flyer_index = 0, 
        i, j, amount_pr = 0, size_sale = 0, wbytes;
    float total_cost = 0;
    
    //CONFIRMING DELETING FILE
    char confirm[BUFF_SIZE], fileName[BUFF_SIZE];
    int ret;
    
    //FOR DATE
    time_t current_time;
    int day, month, year;
    char day_str[3], month_str[3], year_str[5];

    //CHEKING AMOUNT ARGUMENTS
    if (argc != 3){
        printf("ERROR: wrong amount of arguments!\n");
        return (-1);
    }


    //CHECKING IF A COMPANY EXISTS
    strcat(path,argv[1]);
    strcat(path,".txt");
    
    if(access(path, F_OK) != 0)
    {
        printf("Company %s Not Found! Try to make a Flyer first.\n", argv[1]);
        exit(1);
    }

    printf(">>Welcome to MakeOrder programm!<<\n");

    //PRINTING FLYER QITH GetFlyer.c
    if((pid = fork()) == 0)
    {
        execl("GetFlyer","GetFlyer",argv[1],NULL);
        perror("error");
        exit(1);
    }
    wait();//we dont want to continue withouit printing

    //GETTING PRODUCTS, AMOUNTS AND PUTTING DATA INTO RELEVANT ARR
    printf("\nEnter wanted products and amount (STOP to finish):\n");
    while (1)
    {
        fgets(product_line, BUFF_SIZE, stdin);
        product_line[strlen(product_line) - 1] = '\0'; //deleting the '\n' symbol

        if (!strcmp(product_line, "STOP")) {//last argument in product row
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

        //saving the amount of product into array
        arr_amount[amount_pr] = (char*)malloc(strlen(product_line) - name_ch - 1);//-1 becauase of " "
        tmp_str = product_line;
        tmp_str += name_ch + 1; //mooving a pointer to thee price
        strncpy(arr_amount[amount_pr], tmp_str, strlen(product_line) - name_ch - 1);
        arr_amount[amount_pr][strlen(product_line) - name_ch - 1] = '\0';

        //ready for the next product
        amount_pr++;
    }

    //CHECKING IF USER ENTER A RELEVANT INFO, AND PRODUCTS IN FLYER
    if((from = open(path, O_RDONLY)) == -1) {perror("ERROR: can't open file to read from it!"); return -1;}
    //copying all flyer, because we want to work with str
    while ((rbytes = read(from, &buff, 1)) > 0){//copying all file into flyer str
        flyer[flyer_index] = buff;
        flyer_index++;
        flyer = (char*)realloc(flyer, strlen(flyer)+1);
    }
    close(from);
    //searching and saving the sale amount
    p_flyer = strstr(flyer, "% off");
    start_search_pos = p_flyer + 7; //STARTING THE SEARCH FROM THIS POINT
    while(*p_flyer != ' '){//going back and summing the amount of chars in sale
        p_flyer--;
        size_sale++;
    }
    p_flyer++;//moving pointer to the start of sale 
    i = 0;
    while(size_sale > 0){//saving sale
        sale[i] = *p_flyer;
        p_flyer++; i++;
        size_sale--;
    }
    sale[strlen(sale)-1] = '\0';

    //FIRST ROW OF THE ORDER
    strcpy(path, "Black_Friday/");
    strcat(path, argv[1]);
    strcat(path, "_Order/");
    strcat(path, argv[2]);
    strcat(path, ".txt");
    strcpy(ready_row, argv[1]);
    strcat(ready_row, " Order\n\n");
    if ((from = open(path, O_RDWR | O_CREAT, 0664)) == -1)
	{   
	    perror("Error: Can't create name.txt file\n"); 
        return(-1);
	}
    if (( wbytes = write(from, ready_row, strlen(ready_row))) == -1 ){
		printf("Cannot write ROW into %s.txt", argv[2]); 
        return(-1); 
    }
    
    //CHECKING PRODUCTS IN STORE AND ADDING THEM TO TOTAL COST
    //ADDING PRODUCT INTO ORDER IF HE IS IN THE STORE
    for(i = 0; i < amount_pr; i++){
        p_flyer = strstr(flyer, arr_name[i]);//searching the product
        if(p_flyer == NULL){//product not found
            printf("ERROR: in %s store no such product like %s!\nNOT ADDED TO ORDER\n", argv[1], arr_name[i]);
        }
        else{//product found
            strcpy(ready_row, arr_name[i]);
            strcat(ready_row, " - ");
            strcat(ready_row, arr_amount[i]);
            strcat(ready_row, "\n\n");
            if (( wbytes = write(from, ready_row, strlen(ready_row))) == -1 ){
		        printf("Cannot write ROW into %s.txt", argv[2]); 
                return(-1); 
            }
            //seraching and collecting price like the sale
            p_flyer = strstr(p_flyer, "NIS\n");
            size_sale = -1;
            while(*p_flyer != '.'){//counting chars
                size_sale++;
                p_flyer--;
            }
            p_flyer++;
            j = 0;
            while(size_sale >= 0){//saving price
                product_price[j] = *p_flyer;
                p_flyer++; j++;
                size_sale--;
            }
            product_price[strlen(product_price) - 1] = '\0';
            total_cost += atoi(product_price) * atoi(arr_amount[i]);//calc total price
            strcpy(product_price, "");
        }
        p_flyer = start_search_pos;//ready to serach the next product
    }
    
    //TOTAL PRICE
    strcpy(ready_row,"Total price: ");
    total_cost = total_cost * ((100.0 - (float)(atoi(sale))) / 100.0);
    sprintf(total_cost_str, "%.1f", total_cost);       
    strcat(ready_row, total_cost_str);
    strcat(ready_row, " NIS\n\n");
    if (( wbytes = write(from, ready_row, strlen(ready_row))) == -1 ){
		    printf("Cannot write ROW into %s.txt", argv[2]); 
            return(-1); 
    }

    //USER CONFIRM
    printf("Total price: %.1f NIS (Confirm to approve/else cancel)\n", total_cost);
    scanf("%s", confirm);
    if (strcmp(confirm, "Confirm")){// not confermed
        printf("Cancelling the order..\n");
        ret = remove(path);
        if (ret == 0){
            printf("File deleted successfully\n");
            exit(1);
        }    
        else{
            printf("Unable to delete file %s\n", fileName);
            exit(1);
        }
    }

    //DATE
    current_time = time(NULL);
    struct tm tm = *localtime(&current_time);
    sprintf(day_str, "%d", tm.tm_mday);
    strcpy(ready_row, day_str);
    strcat(ready_row, "/");
    sprintf(month_str, "%d", tm.tm_mon+1);
    strcat(ready_row, month_str);
    strcat(ready_row, "/");
    sprintf(year_str, "%d", tm.tm_year+1900);
    strcat(ready_row, year_str);
    strcat(ready_row, "\n");
    if (( wbytes = write(from, ready_row, strlen(ready_row))) == -1 ){
		    printf("Cannot write ROW into %s.txt", argv[2]); 
            return(-1); 
    }

    //READ MODE
    if((pid = fork()) == 0)
    {
        execlp("chmod","chmod","-w",path,NULL);
        perror("error PERMISSION");
        exit(1);
        
    }
    wait();//we dont want to continue withouit changing mode

    //ENDING MAKING ORDER
    printf("Order created successfully!\n// %s.txt Created in %s_Order Dir with Read Mode\n", argv[2], argv[1]);

    free(flyer);
    close(from);
    return 0;
}