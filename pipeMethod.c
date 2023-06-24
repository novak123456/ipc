#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int main() {

int fd[2]; //a file descriptor

pid_t pid;//process id variable

//create the pipe, file descrptors: fd[0] read, fd[1] read,
if(pipe(fd)==-1){
fprintf(stderr,"Pipe fail");
return 1;
}
//fork a child process
pid = fork();//fork() only work on UNIX like sys

if(pid<0){ //error occurred
fprintf(stderr,"Fork failed");
return 1;
}

/*-------------------Producer Method-------------------*/
if(pid>0){
//Generate and Store all randomNumbers in to producer_randomNumber.txt
    FILE* file;
    file = fopen("producer_randomNumber.txt", "w");
    if (file == NULL) {
        printf("fail to create a producer file!");
        return -1;
    }
int randomNUmbers[BUFFER_SIZE];
srand(time(NULL));
for(int i = 0; i < BUFFER_SIZE; i++) {
      int randomNumber = rand() % 1000 + 1;        
      randomNUmbers[i]=randomNumber;        
      fprintf(file,"%d\n",randomNumber); //write to the .txt file
}
fclose(file);  

//close the unused end of pipe
close(fd[0]);
//Write to the pipe
write( fd[1],randomNUmbers,sizeof(randomNUmbers));
close(fd[1]);
}

/*-------------------Consumer Method-------------------*/
if(pid==0){
FILE *file;
file = fopen("consumer_randomNumber.txt","w");
if(file==NULL){
printf("fail to create a producer file!");
return -1;
}

//close the write end of pipe
close(fd[1]);
int receivedArray[BUFFER_SIZE];
read(fd[0],receivedArray,sizeof(receivedArray));
printf("Child process received: ");
printf("\n");
for(int i=0; i< BUFFER_SIZE; i++){
printf("%d",receivedArray[i]);
fprintf(file,"%d\n",receivedArray[i]); //write to the .txt file
printf("\n");}
fclose(file);  
close(fd[0]);

}
    return 0;
}