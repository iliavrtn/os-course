#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>

// Define number of patients
#define N 10

// Node - patient
typedef struct node
{
    int num;
    struct node* next;
}*PNode;

// Semaphores
// Patients
sem_t OutOfClinic, OnSofa, P_InTreatment, P_DoPayment, P_Paid;
// Dentist
sem_t Dent_InWork, Dent_GetPayment;
// For critical sections
sem_t mutex;

int NumOfPatients = 0; // Current number of patients in clinic
PNode Head_Stand = NULL, Head_OnSofa = NULL; // Link Lists for queues management, for standing and for sitting

// adding a patient to a queue
PNode addNode(PNode head , int num){
    PNode patient = (PNode)malloc(sizeof(struct node)), queue;
    if(!patient)// memory was not allocated 
        exit(1);
    // "building" a patient
    patient->num = num;
    patient->next = NULL;

    if(head == NULL)// first patient in the clinic
    {
        head = patient;
    }
    else{
        queue = head;
        while(queue->next != NULL){// going to the end of queue
            queue = queue->next;
        }
        queue->next = patient;// adding a new patient to a queue
    }
    return head;
}

// next patient can enter
PNode getFirst(PNode head,int* curStand)
{
    if(head == NULL)// no patients in clinic
        return head;

    PNode patient = head;
    *curStand = head->num;
    head = head->next;
    free(patient);// patient is entering and already not in queue 
    
    return head;
}

//Patient thread
void* Patient(void *num)
{
    int* patient_num = (int*) num;

    while(1)
    {
      sem_wait(&mutex); // lock the if statement to avoid context switch
      if(NumOfPatients < N)
      {
            sem_post(&mutex); // release 

            sem_wait(&mutex); // lock the node adding to avoid context switch
            Head_Stand = addNode(Head_Stand, *patient_num); // head current patient number into the stands node
            sem_post(&mutex); // release

            sem_wait(&mutex); // lock the counter increasing to avoid context switch
            NumOfPatients++; // update current patients in clinic
            sem_post(&mutex); // release

            printf("I'm Patient #%d, I got into clinic\n", *patient_num); 
            sleep(1);    
            
            sem_wait(&OnSofa); // wait to sit on sofa

            sem_wait(&mutex); // lock the node operation to avoid context switch
            Head_Stand = getFirst(Head_Stand, patient_num); // get the first standing patient to sit him on the sofa 
            sem_post(&mutex); // release

            printf("I'm Patient #%d, I'm sitting on the sofa\n", *patient_num);
            Head_OnSofa = addNode(Head_OnSofa,*patient_num); // add patient for the queue of treatment waiting 
            sleep(1);

            sem_wait(&P_InTreatment); // wait for treatment

            sem_wait(&mutex); // lock the node operation to avoid context switch
            Head_OnSofa = getFirst(Head_OnSofa, patient_num); // get the first sitting patient to start a treatment
            sem_post(&mutex); // release
            
            printf("I'm Patient #%d, I'm getting treatment\n", *patient_num);
            sleep(1);

            sem_post(&OnSofa); // the sofa got one more place    
            sem_post(&Dent_InWork); // wake up a dental to treat the patient

            sem_wait(&P_DoPayment); // wait for dental to finish the treatment
            printf("I'm Patient #%d, I'm paying now\n", *patient_num); 
            sem_post(&Dent_GetPayment); //pay to dental 
            sem_wait(&P_Paid); // wait for dental to accept the payment
            sleep(1); 
        }
        else
        {
            sem_post(&mutex); // release the if statement lock
            printf("I'm Patient #%d, I'm out of clinic\n",*patient_num);
            sleep(1);
            sem_wait(&OutOfClinic); // wait untill one patient in the clinic will get out
        }
    }
}

//Dentail thread
void* Dental(void* num)
{
    int* dentist_num = (int*) num;
    while(1){
           sem_wait(&Dent_InWork); // wait untill patient is up for a treatment
           printf("I'm Dental Hygienist #%d, I'm working now\n", *dentist_num);
           
           sem_post(&P_InTreatment); // notify finished treatment for the patient
           sem_post(&P_DoPayment); // notify patient can pay
           sleep(1);
         
           sem_wait(&Dent_GetPayment); // wait for cash desk
           printf("I'm Dental Hygienist #%d, I'm getting a payment\n", *dentist_num);
           sem_post(&P_Paid); // accept patient's payment 
           sleep(1);

           sem_wait(&mutex); // lock the counter decreasing to avoid context switch
           NumOfPatients--; // decrese the counter
           sem_post(&mutex); // release

           sem_post(&OutOfClinic); // notify for the patients which are waiting out of clinic that someone has got out
    }
}

// initializing semaphores and starting threads
void main()
{
    pthread_t patients[N+2] , dentals[3]; // array of threads for patients and dentals
    int i, pIndex[N+2]; // indexes array for sending to the create thread function

    //Init semaphores
    sem_init(&OutOfClinic,0,0);// amount of patients out the clinic, because only 10 can enter 
    sem_init(&OnSofa,0,4);// max 4 patients can sit
    sem_init(&P_InTreatment,0,3);// max 3 patients can be in treatment
    sem_init(&P_Paid,0,1);// only one cash box
    sem_init(&Dent_InWork,0,0);
    sem_init(&Dent_GetPayment,0,0);
    sem_init(&mutex,0,1); // locking critical sections

    // create threads for patients
    for(i=0 ; i<N+2 ; i++){
        pIndex[i] = i;
        pthread_create(&patients[i], NULL, Patient, (void*)(pIndex+i));
    }   

    //create threads for dentals
    for(i=0 ; i<3 ; i++){
        pIndex[i] = i;
        pthread_create(&dentals[i], NULL, Dental, (void*)(pIndex+i));
    }
    
    // wait for patients threads
    for(i=0 ; i<N+2 ; i++)
        pthread_join(patients[i], NULL);
    // wait for dentals threads
    for(i=0 ; i<3 ; i++)
        pthread_join(dentals[i], NULL);
}