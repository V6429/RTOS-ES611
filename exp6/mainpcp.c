
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>


/*=============================================================================================================================
    DESCRIPTION : THIS FILE CONTAINS THE CODE TO IMPLEMENT PRIORITY CEILING PROTOCOL FOR 3 TASKS AND 3 SEMAPHORE
===============================================================================================================================*/
// Define total semaphores to be accessed by each task (Size_A1 indicates no of semaphores where A1,A2,A3 defines task)
#define Size_A1 1
#define Size_A2 2
#define Size_A3 2


// Define semaphores to be accessed by each task
int arr_S1[Size_A1] = {3};
int arr_S2[Size_A2] = {1,2};
int arr_S3[Size_A3] = {1,2};

// Global variables
int Ceil_S1, Ceil_S2, Ceil_S3;                                                            // Store ceil values
bool S1, S2, S3;                                                                          // Semaphore flags
int count1=1,count2=1,count3=1;                                                           // Indicates the number every task is restarted
int  temp_priority;

typedef struct Task{
  int Task_no;
  int Priority;                                                                           // Holds Task no and Priority (similar to TCB)
} Task;

Task Created_task,Task_arr[3];                                                            // Array of tasks
/*==========================================================================================================================================================================================
 * Function name      : Success
 * Function prototype : bool Success(char S)
 * Description        : This function sets the semaphore flags of each task to true
 * Input Arg          : S - semaphore no
 * Return type        : bool
===========================================================================================================================================================================================*/
bool Success(char S){
  printf("Condition satisfied \n");
  if (S==S1)
    S1 = 1;
  if (S==S2)
    S2 = 1;
  if (S==S3)
    S3 = 1;
  return;
}

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 10 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}
/*==========================================================================================================================================================================================
 * Function name      : Failed
 * Function prototype : void Failed(int Task_no)
 * Description        : This function blocks the task if semaphore cannot be accessed
 * Input Arg          : int Task_no
 * Return type        : void
===========================================================================================================================================================================================*/
void Failed(int Task_no){
  printf("Task %d \n",Task_no);
  printf(" Blocked \n");
  return;
}
/*==========================================================================================================================================================================================
 * Function name      : Get_Semaphore
 * Function prototype : bool Get_Semaphore(int Semaphore_no, int Task_no, int Ceil, int count)
 * Description        : This function controls the access of the semaphores
 * Input Arg          : int Semaphore_no, int Task_no, int Ceil, int count
 * Return type        : bool
===========================================================================================================================================================================================*/
bool Get_Semaphore(int Semaphore_no, int Task_no, int Ceil, int count){
  printf("");
  printf("Priority of task trying to acquire semaphore %d : \n",Task_arr[Task_no-1].Priority);

  // For semaphore 1
  if (Semaphore_no==1)
  {
     printf("Ceil value of previously held semaphore : %d \n ",Ceil);
     if(Task_arr[Task_no-1].Priority > Ceil)
    {
      S1 = Success(S1);
      return S1;
    }
    else
    {
      Failed(Task_no);
      return 0;
    }
  }
  // For semaphore 2
  if (Semaphore_no==2)
  {
    printf("Ceil value of previously held semaphore : %d \n", Ceil);
    if(Task_arr[Task_no-1].Priority > Ceil)
    {
      S2 = Success(S2);
      return S2;
    }
    else
    {
      Failed(Task_no);
      return 0;
    }
  }

  // For semaphore 3
  if (Semaphore_no==3)
  {
    if(count==1)                                                                          // Grant access to semaphore if it is being tried to acquire for 1st time among all tasks
    {
      S3 = Success(S3);
      return S3;
    }
    else
    {
      printf("Ceil value of previously held semaphore : %d \n \n",Ceil);
      if(Task_arr[Task_no-1].Priority > Ceil)
      {
        S3 = Success(S3);
        return S3;
      }
      else
      {
        Failed(Task_no);
        return 0;
      }
    }
  }
}
/*==========================================================================================================================================================================================
 * Function name      : Running_q
 * Function prototype : void Running_q(Task RunningTask)
 * Description        : This function emulates the working of execution queue and puts the task into execution
 * Input Arg          : Task RunningTask
 * Return type        : none
===========================================================================================================================================================================================*/
void Running_q(Task RunningTask){
  int Sem_state;
  // Acquire semaphore
  if (RunningTask.Task_no == 1)
  {
    if(count1==1)
    {
      printf("Task id of running task: %d \n",RunningTask.Task_no);
      printf("Priority : %d \n ",RunningTask.Priority);
      delay(100);
      printf("\n");
      printf("................. Trying to acquire semaphore S3 ...................\n");
      Sem_state = Get_Semaphore(3,RunningTask.Task_no,Ceil_S3,count1);  // 1st call
      count1=count1+1;
      if(Sem_state==1)
      {
        printf("Semaphore S3 acquired \n");
        printf("Execution in progress...\n");
        delay(10);
        Running_q(Task_arr[1]);                                                           // MPT trying to execute
      }
      else
      {
        count1=count1+1;
      }
    }
    if(count1==2)
    {
      count1=count1+1;
      printf("Task 1 continuing execution using semaphore... \n");
      delay(20);
      // HPT preempting LPT
      printf("");
      printf("||||| High priority task preempting LPT ||||| \n");
      Running_q(Task_arr[2]);
      if(Sem_state==1)
      {
        printf("Semaphore S1 acquired by HPT \n");
        printf("Execution in progress... \n");
        delay(30);
        Running_q(Task_arr[0]);                                                         // MPT trying to execute
      }
      else
      {
        count1=count1+1;
      }
    }
    if(count1==3)
    {
      Sem_state = Get_Semaphore(2,RunningTask.Task_no,0,count1);
      count1=count1+1;
      if(Sem_state == 1)
      {
        printf("Semaphore S2 locked \n");
        delay(30);
        S2 = 0;
        printf("Semaphore S2 released \n");
        delay(30);
        S3 = 0;
        printf("Semaphore S3 released \n");
        printf("LPT execution completed! \n");
        Running_q(Task_arr[1]);
      }
    }
  }

  if (RunningTask.Task_no == 2)
  {
    if(count2==1)
    {
      printf("");
      printf("\n\n||||| MPT preempting LPT ||||| \n");
      delay(10);
      printf("\n");
      printf("....... Trying to acquire semaphore S2 .......\n");
      Sem_state = Get_Semaphore(2,RunningTask.Task_no,Ceil_S3,count2);
      count2=count2+1;
      if(Sem_state==1)
      {
        printf("Semaphore S2 acquired\n");
        printf("Execution in progress...\n");
        delay(10);
      }
      else
      {
        printf("Failed to acquire Semaphore S1\n");
        printf("Continue LPT execution with new priority\n");
        temp_priority = Task_arr[0].Priority;
        Task_arr[0].Priority = Task_arr[RunningTask.Task_no-1].Priority;                // Inherit priority of MPT
        printf("New priority: %d",Task_arr[0].Priority);
        Running_q(Task_arr[0]);                                                         // LPT continues to execute
      }
    }
    else
    {
      count2=count2+1;
      printf("\n");
      printf("....... MPT trying to lock semaphore S2 .......\n ");
      Sem_state = Get_Semaphore(2,RunningTask.Task_no,0,count2);
      if(Sem_state==1)
      {
        printf("Semaphore S2 acquired\n");
        printf("Execution in progress...\n");
        delay(10);
        printf("\n");
        printf("....... MPT trying to lock semaphore S3 .......\n");
        Sem_state = Get_Semaphore(3,RunningTask.Task_no,0,count2);
        if(Sem_state==1)
        {
          printf("Semaphore S3 acquired \n");
          printf("Execution in progress...\n");
          delay(10);
          S3=0;
          printf("Releasing semaphore S3 \n");
          S2 = 0;
          printf("Releasing semaphore S2 \n");
          printf("MPT Execution completed! \n");
        }
      }
    }
  }
  if (RunningTask.Task_no == 3)
  {
    if(count3==1)
    {
      printf("HPT preempted LPT \n");
      delay(10);
      printf("\n");
      printf("....... Trying to acquire semaphore S1 .......\n");
      count3=count3+1;
      Sem_state = Get_Semaphore(1,RunningTask.Task_no,Ceil_S3,count3);
      if(Sem_state==1)
      {
        printf("Semaphore S1 acquired\n");
        printf("Execution in progress...\n");
        delay(10);
        printf("Releasing Semaphore S1\n");
        S1 = 0;
        printf("HPT execution completed!\n");
      }
      Task_arr[0].Priority =  temp_priority;
      printf("LPT priority set back to older! Now priority is: %d\n",Task_arr[0].Priority);
      Running_q(Task_arr[0]);                                                                   // LPT continues to execute
    }
  }
}
/*==========================================================================================================================================================================================
 * Function name      : Compute_ceil
 * Function prototype : int Compute_ceil(int* Array, int size_arr )
 * Description        : This function computes the ceil of each semaphore
 * Input Arg          : int* Array, int size_arr
 * Return type        : int
===========================================================================================================================================================================================*/
int Compute_ceil(int* Array, int size_arr ){
  int index = 0;
  int max_val = Array[index];
  if (size_arr == 1)
  {
    return max_val;
  }
  else
  {
    for(int i=0;i<=size_arr;i++)
    {
      if (max_val<Array[i])
      {
        max_val = Array[i];
      }
    }
    return max_val;
  }
}
/*==========================================================================================================================================================================================
 * Function name      : Create_task
 * Function prototype : Task Create_task(int Task_no)
 * Description        : This function creates task and records task no and its priority
 * Input Arg          : int Task_no
 * Return type        : Task
===========================================================================================================================================================================================*/
Task Create_task(int Task_no){
  Created_task.Task_no = Task_no;
  Created_task.Priority = Task_no;
  printf("\n Successful creation of : %d",Created_task.Task_no);
  return Created_task;
}
/*==========================================================================================================================================================================================
 * Function name      : setup
 * Function prototype : void setup()
 * Description        : This function controls the completed execution of PCP
 * Input Arg          : None
 * Return type        : void
===========================================================================================================================================================================================*/
void main() {

  printf("\n ******************************Implementation of priority ceiling protocol*******************************");
  printf("\n *** NOTE: Higher number is considered to have higher priority and priority of each task is its task number ***");

  printf("Creating tasks");
  Task_arr[0] = Create_task(1);
  Task_arr[1] = Create_task(2);
  Task_arr[2] = Create_task(3);

  printf( "\n ****************************Displaying details of task requiring semaphore******************************");
  printf("Task 1 : \n");
  for(int index=0; index<Size_A1; index++)
  {
    printf("\n S: %d",(arr_S1[index]));
    printf("\t");
  }
  printf("");

  printf("\n Task 2 : \n");
  for(int index=0; index<Size_A2; index++)
  {
    printf("\n S: %d",arr_S2[index]);
    printf("\t");
  }
  printf("");

  printf("\n Task 3 : \n");
  for(int index=0; index<Size_A3; index++)
  {
    printf("\n S:%d",arr_S3[index]);
    printf("\t");
  }
  printf("");
  printf("\nComputing ceil of each semaphore\n");
  Ceil_S1 = Compute_ceil(arr_S1,Size_A1);
  Ceil_S2 = Compute_ceil(arr_S2,Size_A2);
  Ceil_S3 = Compute_ceil(arr_S3,Size_A3);
  printf("Ceil of S1 : %d \n",Ceil_S1);
  printf("Ceil of S2 : %d \n",Ceil_S2);
  printf("Ceil of S3 : %d \n",Ceil_S3);

  printf("\n*******************************************BEGIN EXECUTION**********************************************");
  Running_q(Task_arr[0]);
   Running_q(Task_arr[1]);
    Running_q(Task_arr[2]);
 

    
  printf("\nExiting");

}
