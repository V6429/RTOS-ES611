#include<stdio.h>
#include<string.h>

int s;
int prev_id;
int cs_status;

struct Task{
    int ID;
    int priority;
    char* status;
    char request_cs;
    int release_time;
    char CPU_access;
} ;



int lowest(int one ,int two ,int three)
{

    int a[3];
    a[0] = one ;
    a[1] = two ;
    a[2] = three;
    int min = a[0];
    for(int i=0; i<3 ; i++)
    {
        if(a[i]<min)
        min = a[i];
    }
    return min;
}

int highest(struct Task *t1 ,struct Task *t2,struct Task*t3)
{
    int max = 0;
     int a[3];
    a[0] = t1->priority;
    a[1] = t2->priority;
    a[2] = t3->priority;
    for(int i=0; i<3 ; i++)
    {
        if(a[i]>max)
        max = a[i];
    }
    return max;
}

int check_cstask_priority(struct Task *t1 ,struct Task *t2,struct Task*t3,int max)
{
  if( max == 0)
  {
  (*t1).status = "TRUE";
  return 0;
  }
  if(  max == 1)
  {
  t2->status = "TRUE";
  return 1;
  }
  if(  max == 2)
  {
  t3->status = "TRUE";
  return 2;
  }


}

void printTaskInfo(struct Task *t1)
{

    printf("Task Information: \n");
    printf("Task ID: %d \n",t1->ID);
    printf("Priortity: %d \n",t1->priority);
    printf("Aquired Lock: %s \n",t1->status);
    printf("***************************************** \n\n");
}
void main()
{
    int temp;
    struct Task t[3];
    int cs_status=0;

    t[0].ID=1;
    t[0].priority=0;
    t[0].status = "FALSE";
    t[0].request_cs = 0x01;
    t[0].release_time = 1;
    t[0].CPU_access = 0;
    printf("List of all tasks: \n");
     printf("***************************************** \n");
    printTaskInfo(&t[0]);

    t[1].ID=2;
    t[1].priority=1;
    t[1].status = "FALSE";
    t[1].request_cs = 0x00;
    t[1].release_time = 2;
    t[1].CPU_access = 0;
    printTaskInfo(&t[1]);


    t[2].ID = 3;
    t[2].priority=2;
    t[2].status = "FALSE";
    t[2].request_cs = 0x01;
    t[2].release_time = 3;
    t[2].CPU_access = 0;

    printTaskInfo(&t[2]);

    if(cs_status == 0)
    {

       printf("No Tasks have locked Resource \n\n");
       int min = lowest(t[0].priority,t[1].priority,t[2].priority);
       int max = highest(&t[0],&t[1],&t[2]);
      // int prior = check_cstask_priority(&t[0],&t[1],&t[2],max);
       int earliest = lowest(t[0].release_time,t[1].release_time,t[2].release_time);
      // printf("\np=%d\n",prior);
       printf("\nmax=%d\n",max);
       printf("\nmin=%d\n",min);

    if(t[max].release_time == earliest)
    {
         t[max].CPU_access = 1;
         printf("Highesr Priority task has released first \n");
        if(t[max].request_cs == 0x01)
        {
            t[max].status = "TRUE";
            if(t[max].CPU_access == 1)
            {
                printf("Highest Priority task has the Resource and the CPU i.e: \n");
                printTaskInfo(&t[max]);
                cs_status = 1;
            }
        }
        else
        {
        printf("Highest Priority task doesnt need Resource i.e: \n");
        }
    }
    else if(t[min].release_time == earliest)
    {
        printf("Lowest Priority task is released first\n");
        
        if(t[min].request_cs == 0x01)
        {
        t[min].status = "TRUE";
        t[min].CPU_access =1;
        t[max].CPU_access = 0;
        printf("Lowest Priority task has the Resource and CPU i.e: \n");
        printTaskInfo(&t[min]);
        printf("Highest Priority task is released second\n");
        printf("Highest Priority task status: \n");
        printTaskInfo(&t[max]);
        printf("Highest Priorty occupies CPU \n");
         t[min].CPU_access =0;
        t[max].CPU_access = 1;
        printf("Lowest Priority task status: \n");
        printTaskInfo(&t[min]);
        printf("Highest Priority task status: \n");
        printTaskInfo(&t[max]);

        if(t[max].status == "FALSE")
        {
        printf("Highest priority task is blocked \n");
        }

        printf("Therefore premption and priority inheritance is happenning..........\n \n \n \n");
        temp = t[min].priority;
        t[min].priority=t[max].priority;
        t[max].priority = temp;
        t[min].status = "TRUE";
        t[max].status = "FALSE";
        printf("Previously Lowest Priority task now has the Resource and CPU i.e:\n");
        printTaskInfo(&t[min]);
        printf("Previously Highest Priority task current status:\n");
        printTaskInfo(&t[max]);
         temp = t[min].priority;
        t[min].priority=t[max].priority;
        t[max].priority = temp;
        t[min].status = "FALSE";
        t[max].status = "TRUE";
        printf("After critical section execution: \n");
         printf("Lowest Priority task status: \n");
        printTaskInfo(&t[min]);
        printf("Highest Priority task status: \n");
        printTaskInfo(&t[max]);

        }
    }

    }



}
