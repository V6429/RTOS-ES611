
#include <stdio.h>
#include <pthread.h>
#define NTHREADS 5

// ######################the  semaphore
_Atomic int sem=1;
void wait()
{

    while (1)
        if (sem > 0)
        {
            sem--;
            return;
        }
}


void post()
{
    sem++;
}

//#############################################################
void somecriticalwork(int someparameter){

    int i=0;
    for(i=0;i<20;i+someparameter)
    printf("\n%d is in critical area",someparameter);

}



//##########################################################

void *taskinaction(void *arg){
    int val=*(int *)arg;
    int j=0;
    printf("\nTHIS IS TASK %d \n\n",val);
    while(1){

        wait();
        for(j=0;j<100;j += val)
            somecriticalwork(val);
        post();

    }



    return NULL;
}



void main(){

    // simulating tasks
    pthread_t tasks[4];
    int i=0;
    
    for(i=0;i<4;i++)
    pthread_create(&tasks[i],NULL,&taskinaction,&i);



    while(1);
    return 0;
}