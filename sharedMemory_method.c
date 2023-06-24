#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <time.h>

#define BUFFER_SIZE 100

struct RingBuffer{
int data[BUFFER_SIZE];
int read_index;
int write_index;
sem_t empty_slots;
sem_t filled_slots;
};

struct RingBuffer *buffer;

/*----------Producer----------*/
int producer(){
//Store all randomNumbers in to producer_randomNumber.txt
FILE *file;
file = fopen("producer_randomNumber.txt","w");
if(file==NULL){
printf("fail to create a producer file!");
return -1;
}

int randomNUmbers[BUFFER_SIZE];

srand(time(NULL));
for(int i = 0; i < BUFFER_SIZE; i++) {
      int randomNumber = rand() % 1000 + 1;        
      //array[i]=malloc(12*sizeof(char));
      //printf(array[i],"%d",randomNumber);
        randomNUmbers[i]=randomNumber;        
        fprintf(file,"%d\n",randomNumber); //write to the .txt file
}
       
       
for(int i=0; i<BUFFER_SIZE; i++){
sem_wait(&buffer->empty_slots);  //if empty_slots if >0,it will be deceased and access allowed; if ==0,Producer is blocked
buffer->data[buffer->write_index] = randomNUmbers[i];
printf("Producer is producing item: %d\n",buffer->data[buffer->write_index]);
buffer->write_index = (buffer->write_index + 1)%BUFFER_SIZE;  //because it is Ring buffer so next element is this way.
sem_post(&buffer->filled_slots);//increase the semaphore by one
}      

fclose(file);  
}


/*----------Consumer----------*/
int consumer(){

//Store all received randomNumbers in to consumer_randomNumber.txt
FILE *file;
file = fopen("consumer_randomNumber","w");
if(file==NULL){
printf("fail to create a consumer file!");
return -1;
}

for(int i=0; i<BUFFER_SIZE; i++){
sem_wait(&buffer->filled_slots); //if filled_slots: >0, it will be decreased by one; ==0,no resources avalible, the consumer process will be blocked

int item = buffer->data[buffer->read_index];
fprintf(file,"%d\n",item); //write to the .txt file

printf("Consumer is reading data : %d\n",item);
buffer->read_index = (buffer->read_index+1)% BUFFER_SIZE;
sem_post(&buffer->empty_slots);  //incement the semaphore pointed to by empty_slots by one


} fclose(file); }



int main(){

//allocate space for Ring buffer pointer
buffer = mmap(NULL,sizeof(struct RingBuffer),PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1,0);
//Initialize index
buffer->read_index =0;
buffer->write_index =0;
//initialize semaphores
sem_init(&buffer->empty_slots,1,BUFFER_SIZE);
sem_init(&buffer->filled_slots,1,0);

//Fork a child process whose pid == 0
pid_t pid = fork();
if(pid<0){
fprintf(stderr,"fork fail.");
}else if(pid==0){
//sleep(5);//Let consumer process delay 5 seconds
consumer();
}else{
producer();
wait(NULL); //wait for child to finish
}

//After both producer and consumer finished, it destroy the semaphores and unmap the shared memory
sem_destroy(&buffer->empty_slots);
sem_destroy(&buffer->filled_slots);
munmap(buffer,sizeof(struct RingBuffer()));



return 0;
}

