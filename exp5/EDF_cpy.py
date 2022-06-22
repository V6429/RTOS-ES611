
#!/usr/bin/python3
# Rate monotonic scheduling is a priority algorithm that belongs to the static priority scheduling category of Real Time Operating Systems. 
# It is preemptive in nature. The priority is decided according to the cycle time of the processes that are involved.
# If the process has a small job duration, then it has the highest priority. Thus if a process with highest priority starts execution, 
# It will preempt the other running processes. The priority of a process is inversely proportional to the period it will run for.


# Task with lowest time period has highest priority and the task with highest period will have lowest priority.


from asyncio import tasks
from logging import error
from pickle import TRUE
import copy
import re
import time
import os
import matplotlib.pyplot as plt
from math import gcd  as hcf # to calculte hyper period
from math import pow,floor,ceil
from colorama import Fore,Back,Style
import random

# Event driven, Static priority, Preemptive
# Priority assigned based on periods (rate of occurrences)
# Shorter period (highest rate of occurrence) has highest priority





class task:
    taskname='A TASK'
    priority=0
    period=0
    exectime=0
    taskno=0
    deadline=0


    ############################################internal vars for the simulation
    _nextdeadlinein=0
    _remainingexectime=0
    _previousreleasttick=0
    _remingnoofinstances=0
    _isreleased=0
    ############################# some python functions to control this taskcontrol block 

    def __init__(self,exectime,period,taskno,deadline):
        self.exectime=exectime
        self.period=period
        self.taskno=taskno
        self.deadline=deadline

        pass

    def setPriority(self,value):
        self.priority=value

    def __lt__(self,other):
        # the one with lowest deadline
        return self._nextdeadlinein  < other._nextdeadlinein

    def __str__(self) -> str:
        print("\ntaskno   =",self.taskno)
        # print("Taskname =",self.taskname)
        print("ReminExe =",self._remainingexectime)
        print("Exectime =",self.exectime)
        print("Period   =",self.period)
        print("Priority =",self.priority)
        print("Dead line=",self.deadline)
        print("nxt dead line=",self._nextdeadlinein)
        return "----------------------------------"



class EDF:

    #TODO:
    # 1.GET TASK ATTRIBUTES ()
    # 2. check schedulablity,rm analysis stuff

    numoftasks=0
    exec_times=[]
    timeperiods=[]
    readyQ=[]
    blockQ=[]
    tasks=[]  #### 
    priorities=[]
    deadlines=[]
    exec_q=[]
    exec_trace=[]

    idletask=task(-1,-1,-1,-1)
    idletask.taskname="IDLE task"
    uselowvalforHighpriority=1
    hyperpriod=0

    def clearStuff(self):
        print("RESETTING THE CLASSS VALUES")
        self.numoftasks=0
        self.exec_times=[]
        self.timeperiods=[]
        self.readyQ=[]
        self.blockQ=[]
        self.tasks=[] 
        self.priorities=[]
        self.uselowvalforHighpriority=1
        self.hyperpriod=0
        return True

    def getTaskParmeters(self):
        while True:
            try:
                print("Enter the number of tasks")
                self.numoftasks=int(input() or "4")
                print("Enter period of tasks seperated by comas:")
                self.timeperiods=[ int(i) for i in (input() or ("3,6,5,10")).split(",")]
                print("Enter Deadlines seperated by comas(ENTER SAME AS ABOVE):")
                self.deadlines=[ int(i) for i in (input() or ("3,6,5,10") ).split(",") ]
                print("Enter exec times seperated by comas:")
                self.exec_times=[ int(i) for i in (input() or "1,1,1,3").split(",") ]

                
                count=0
                for i,j,k in zip(self.exec_times,self.timeperiods,self.deadlines):
                    taskobj=task(i,j,count,k)
                    taskobj.taskname=str(count+1)
                    count+=1
                    self.tasks.append(taskobj)
                break
            except :
                print("Error in input format",e,"\n\n")
            
        return True

    def calculateHyperPeriod(self):
        # the following is an algoritham    using gcd to calculate lcm of a given set of numbers
        # lcm of a&b = a*b/hcf(a,b)
        lcm =1
        for i in self.tasks:
            lcm=lcm*i.period//hcf(lcm,i.period)

        # the following is some dummy stuff
        return lcm



    def checkSchedulablity(self):
        ## necessary condition        
        util=0
        for i in self.tasks:
            print(util)
            util=util+(i.exectime/i.period)
            
        if util <= 1 :
            print("THIS TASK LIST IS satisfies necessary condition  Utilisation=",util)
            return True
        else:
            print("THIS TASK LIST does not satisfy necessary condition utilisation=",util)
            return False
        




    def configureIdleTask(self):
        # for simulation
        self.idletask.period=self.hyperpriod  ## ensure that this has a lowest priority
        self.idletask._remainingexectime=self.hyperpriod+20
        self.idletask.deadline=self.hyperpriod+20
        self.idletask._nextdeadlinein= self.hyperpriod+20
        pass


    def checkPDA(self):
        
        L_list=[]
        for tick in range(0,self.hyperpriod,1): # strt,stop(-1),step
            for i in self.tasks:
                if(tick % i.period ==0):
                    L_list.append([tick+i.deadline,i])

        # l is populted
        for L in L_list:
            l=0
            # CALCULATE VALUEs
            for task in self.tasks:
                l+=floor(( (L[0]-task.deadline)/task.period )+1)*task.exectime
            # print(Fore.RED,l,Style.RESET_ALL)
            
            ### CHECK CALCULATED VAL WITH DEADLINE
            if(not(L[0]>=l)):
                print(Fore.RED,"Failed at",L[0],"FOR THE TASK",L[1],Fore.RESET)
                return False
            else:
                print(Fore.GREEN,"Passed at",L[0],"FOR THE TASK",L[1],Fore.RESET)
                
        print(Fore.GREEN,"PDA passed",Style.RESET_ALL)        
        return True
    


    ## Dynamic scheduling
    def schedule_Dispatch_Simulate(self):
        
        self.configureIdleTask()
        self.readyQ=[]
        taskinexec=self.idletask
        self.hyperpriod=self.calculateHyperPeriod()

        for i in self.tasks:
            i._remingnoofinstances=self.hyperpriod/i.exectime
            i._previousreleasttick=0
            i._remainingexectime=i.exectime

        
        print("\n\n______________SIMULATING________________")
        print(Fore.YELLOW,"FYI:Task no -1 is IDLE task\n",Style.RESET_ALL)

        # this is like systick
        for tick in range(self.hyperpriod):

            # emulating relaease 
            for j in self.tasks:
                if (tick % j.period == 0):
                    j._isreleased=1
                    j._previousreleasttick=tick
                    j._remainingexectime=j.exectime
                    j._nextdeadlinein=tick+j.deadline
                    


            ## build the ready q
            for j in self.tasks:
                if(j._isreleased):
                    print(Fore.BLUE,"NEW TASK ADDED TO THE READY Q",Style.RESET_ALL)
                    j._isreleased=0
                    self.readyQ.append(j)
                    print(Fore.BLUE,"\n",j.taskno,Fore.RESET)
                    

            #execution stuff
            self.readyQ.sort() # puts the one with lowest deadline first

            ########printing ready Q
            print("READY QUEUE b4 exec",end="---> ")
            for i in self.readyQ:
                if(i.taskname==self.idletask.taskname):
                    print("IDLE_TASK",end= " ")
                else:
                    print(i.taskno,"( deadline @",i._nextdeadlinein,")",end= "| ")


            # swapping one with lower deadline   or when a task finishes
            if((taskinexec._nextdeadlinein > self.readyQ[0]._nextdeadlinein ) or taskinexec._remainingexectime==0):
                # print("\nstartted swap ")
                # print(self.readyQ[0]._nextdeadlinein )
                
                # internaly swapping one with longer waitime to first
                for t in range(0,len(self.readyQ)):
                    
                    # print("69xxxx69")
                    # for z in self.readyQ:
                    #     print(z._nextdeadlinein ,"========",z.taskno)
                    
                    if(self.readyQ[0]._nextdeadlinein == self.readyQ[t]._nextdeadlinein):
                        if(self.readyQ[0]._previousreleasttick > self.readyQ[t]._previousreleasttick):
                            self.readyQ[t],self.readyQ[0]=self.readyQ[0],self.readyQ[t]
                            # swap=copy.deepcopy(t)
                            # print("SWAP=",swap.taskno)
                            # t=copy.deepcopy(self.readyQ[0])
                            # print("t=",self.readyQ[t].taskno)
                            # # self.readyQ[0]=copy.deepcopy(swap)
                            # print("qo=",self.readyQ[0].taskno)

                            # print("swapped",self.readyQ[t].taskno,"   ",self.readyQ[0].taskno)

                    # print("xxxxxx")
                    # for z in self.readyQ:
                    #     print(z._nextdeadlinein ,"========",z.taskno)

                # print("READY QUEUE  ready for swapc",end="--xxxxx-> ")
                # for i in self.readyQ:
                #     print(i.taskno,"( deadline @",i._nextdeadlinein,")",end= "| ")
                    

                if(taskinexec._remainingexectime==0):
                    # print("\nin_looopooop")
                    taskinexec=self.readyQ[0]
                    self.readyQ.remove(taskinexec)
                    # print(taskinexec)
                    # print("--------------aaaa--------------xxxx---------")
                    # print("READY QUEU-----sss---------> ")
                    # for i in self.readyQ:
                    #     if(i.taskname==self.idletask.taskname):
                    #         print("IDLE_TASK",end= " ")
                    #     else:
                    #         print(i.taskno,"( deadline @",i._nextdeadlinein,")",end= "| ")

                else:
                    print(Fore.BLUE,"CONTEXT SITCH",Fore.RESET)
                    swap=taskinexec
                    taskinexec=self.readyQ[0]
                    self.readyQ.remove(taskinexec)
                    self.readyQ.append(swap)
  

                    

                
            
            
           

        #    # case task in execution complete
        #     if(taskinexec._remainingexectime==0):  
        #         print("\nExecution completed for",taskinexec.taskno)
        #         if(tick==7):
        #             print(taskinexec)
        #             print("-------")
        #             for z in self.readyQ:
        #                 print(z)


        #         #one which is waiting for longer time should be selected for execution 
        #         for t in self.readyQ:
        #             if(self.readyQ[0]._nextdeadlinein != t._nextdeadlinein):
        #                 break
        #             elif(self.readyQ[0]._previousreleasttick > t._previousreleasttick):
        #                 swap=t
        #                 t=self.readyQ[0]
        #                 self.readyQ[0]=swap

        #         taskinexec=self.readyQ[0]
                
        #         self.readyQ.remove(taskinexec)
        #         print("YOLO")

        #     else :   
        #         #case premption switch happens   NOTE > because higher means lower priority
        #         if(len(self.readyQ) > 0):
        #             if(taskinexec._nextdeadlinein > self.readyQ[0]._nextdeadlinein):
        #                 #one which is waiting for longer time should be selected for exxcution
        #                 for t in self.readyQ:
        #                     if(self.readyQ[0]._nextdeadlinein != t._nextdeadlinein):
        #                         break
        #                     elif(self.readyQ[0]._previousreleasttick > t._previousreleasttick):
        #                         swap=t
        #                         t=self.readyQ[0]
        #                         self.readyQ[0]=swap

        #                 print("\nContext switch")
        #                 self.readyQ.append(taskinexec)
        #                 taskinexec=self.readyQ[0]   # we already have highest priority at front 
        #                 self.readyQ.remove(taskinexec)





           # the execution
            if taskinexec._remainingexectime > 0:
                taskinexec._remainingexectime -=1
                # taskinexec._nextdeadlinein -=1
                print(Fore.YELLOW)
                print(taskinexec)
                print(Fore.RESET)


                


            print("\n ####################")
            print(Fore.GREEN,"Slot :",tick," Taskin exec is",taskinexec.taskno,"With Remaining exec time =",taskinexec._remainingexectime)
            print(Style.RESET_ALL,"######################\n\n")

            self.exec_trace.append(taskinexec)
            x=input()

            




    def plot_trace(self):
        print("TRACE!!!")
        color_matrix = ['Violet','Indigo','Blue','Green','Yellow','Orange','Red','Brown','Purple']

        # Adding title to the plot
        plt.title('Trace')
        plt.xlabel('HYPERPERIOD ticks')
        plt.ylabel('TASKNAME')
        # plt.set_xlim(0,self.hyperpriod) 
        d1={}


        for i in range(self.numoftasks):
            d1[i]=[]
        for tick in range(len(self.exec_trace)):
            if(self.exec_trace[tick].taskno != -1):
                d1[self.exec_trace[tick].taskno].append((tick,1))
    
        for taskname in d1:
            x_1=d1[taskname]
            y_1 = (taskname, 1)
            print(y_1)
            print(x_1)
            plt.broken_barh(x_1, y_1, facecolors =random.choice(color_matrix))
            x=input("-------")

        plt.yticks([ i for i in range(1,self.numoftasks+1)])
        plt.xticks([ i for i in range(0,self.hyperpriod+1)])
        plt.show()


        return
            


    def prettyPrint(self):
        pass


    def __str__(self):
        for i in self.tasks:
            print(i)
        return("----------------------------------")


if __name__=="__main__":
    
    X=EDF()
    X.getTaskParmeters()
    X.hyperpriod=X.calculateHyperPeriod()
    print("hyper period",X.hyperpriod)
    X.checkSchedulablity()
    if(X.checkPDA()):
        X.schedule_Dispatch_Simulate()
        X.plot_trace()

    
    pass


