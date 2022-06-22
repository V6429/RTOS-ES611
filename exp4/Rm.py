
#!/usr/bin/python3
# Rate monotonic scheduling is a priority algorithm that belongs to the static priority scheduling category of Real Time Operating Systems. 
# It is preemptive in nature. The priority is decided according to the cycle time of the processes that are involved.
# If the process has a small job duration, then it has the highest priority. Thus if a process with highest priority starts execution, 
# It will preempt the other running processes. The priority of a process is inversely proportional to the period it will run for.


# Task with lowest time period has highest priority and the task with highest period will have lowest priority.


import random
import matplotlib.pyplot as plt
from math import gcd  as hcf # to calculte hyper period
from math import pow,floor,ceil
from colorama import Fore,Back,Style
import sys

# Event driven, Static priority, Preemptive
# Priority assigned based on periods (rate of occurrences)
# Shorter period (highest rate of occurrence) has highest priority





class task:

    ###TASK PARAMETERS
    taskname='A TASK'
    priority=0
    period=0
    exectime=0
    taskno=0


    #######INTERNAL VARIABLES 
    _remainingexectime=0
    _remaingtimeforrelease=0
    _remingnoofinstances=0
    _isreleased=0


    def __init__(self,exectime,period,taskno):
        self.exectime=exectime
        self.period=period
        self.taskno=taskno
        pass

    def setPriority(self,value):
        self.priority=value

    def __lt__(self,other):
        return self.period  < other.period

    def __str__(self) -> str:
        print("taskno   =",self.taskno)
        print("Taskname =",self.taskname)
        print("Exectime =",self.exectime)
        print("Period   =",self.period)
        print("Priority =",self.priority)
        return "----------------------------------"



class rm:

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
    exec_q=[]
    exec_trace=[]

    idletask=task(-1,-1,-1)
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

    def assignPriorities(self):
        self.tasks.sort()
        #TODO implement the opposit case
        if(self.uselowvalforHighpriority):
            pri=0
            for i in self.tasks:
                i.setPriority(pri)
                pri +=1


    def checkSchedulablity(self):
        ## necessary condition        
        util=0
        for i in self.tasks:
            util=util+(i.exectime/i.period)

        if util <= 1 :
            print("THIS TASK LIST IS satisfies necessary condition  Utilisation=",util)
        else:
            print("THIS TASK LIST does not satisfy necessary condition utilisation=",util)
            return False

        ## sufficient condition
        # upper bound 
        U_bound=self.numoftasks*(pow(2,1/self.numoftasks)-1)

        if not( util <=U_bound):
            print("Sufficient condition satisfied")
        else:
            print("Sufficient condition failed.....",Fore.BLACK,"CHECK RTA",Fore.RESET)

        return True              


    def checkRta(self):

        self.tasks.sort()# sorting the task list , putting highest priority first

        for i in self.tasks:
            print("checking task ",i.taskno,"with pri = ",i.priority)
            Rprev=i.exectime
            Rnew=0
            

            while True:
                Rnew=0

                ### ADD Ri/Tj *c j
                for j in self.tasks:
                    if(j==i):
                        break
                    Rnew += ceil((Rprev / j.period))*j.exectime
                ### add Ci    
                Rnew+=i.exectime

                if(Rprev==Rnew):
                    break
                Rprev=Rnew

            
            if(Rnew <= i.period):
                print("THIS TASK is schedulable, Response time",Rnew," <= the deadline",i.period)
            else:
                print(Fore.Red,"THIS TASK is  NOTschedulable, Response time",Rnew," !<= the deadline",i.period,Fore.RESET)
                return False
        
        return True
            




    def calculateHyperPeriod(self):
        # the following is an algoritham    using gcd to calculate lcm of a given set of numbers
        # lcm of a&b = a*b/hcf(a,b)
        lcm =1
        for i in self.tasks:
            lcm=lcm*i.period//hcf(lcm,i.period)

        # the following is some dummy stuff
        return lcm



    def getTaskParmeters(self):
        while True:
            try:
                print("Enter the number of tasks")
                self.numoftasks=int(input() or "4")
                print("Enter period of tasks seperated by comas:")
                self.timeperiods=[ int(i) for i in (input() or ("3,6,5,10")).split(",")]
                print("Enter exec times seperated by comas:")
                self.exec_times=[ int(i) for i in (input() or "1,1,1,2").split(",") ]
                count=0
                for i,j in zip(self.exec_times,self.timeperiods):
                    taskobj=task(i,j,count)
                    count+=1
                    self.tasks.append(taskobj)
                break
            except :
                print("Error in input format\n\n")
        return True

    def configureIdleTask(self):
        self.idletask.period=self.hyperpriod  ## ensure that this has a lowest priority
        self.idletask._remainingexectime=self.hyperpriod+20
        

    def assignPriorities(self):
        self.tasks.sort()
        #TODO implement the opposit case
        if(self.uselowvalforHighpriority):
            pri=0
            for i in self.tasks:
                i.setPriority(pri)
                pri +=1
        
            #dummy stuff for idle task    # putting highest value that is lowest priority
            self.idletask.priority=pri



    ## The scheduling decisions are t    priorities=[]aken online, under system runtime. 
    ## It is preemptive online scheduling where the task with the highest priority among all ready tasks at the moment will execute.
   

    def schedule_Dispatch_Simulate(self):

        self.configureIdleTask()
        self.readyQ=[]
        taskinexec=self.idletask
        readyQupdated=0
        taskrelease=0
        
        self.hyperpriod=self.calculateHyperPeriod()
        for i in self.tasks:
            i._remingnoofinstances=self.hyperpriod/i.exectime
            i._remaingtimeforrelease=0
            i._remainingexectime=i.exectime

        
        print("\n\n______________SIMULATING________________")
        print(Fore.BLUE,"Task no -1 is IDLE task\n",Style.RESET_ALL)
        # this is like systick
        for tick in range(self.hyperpriod):

            # emulating relaease 
            for j in self.tasks:
                if (tick % j.period == 0):
                    j._isreleased=1
                    j._remainingexectime=j.exectime
                    taskrelease=1
                    
            
            if(taskrelease):
                ## build the ready q again
                for j in self.tasks:
                    if(j._isreleased):
                        self.readyQ.append(j)
                        j._isreleased=0
                        print(Fore.RED,"BLAAAH ",Fore.RESET)
                readyQupdated=1
                self.readyQ.sort()  #self.tasks.sort()
                    
                    # print(j)
                    # print(readyQ[-1])


            #execution stuff
            ###printing the ready q
            print("READY QUEUE b4 exec",end="--->")
            for i in self.readyQ:
                if(i.taskname==self.idletask.taskname):
                    print("IDLE_TASK",end= " ")
                else:
                    print(i.taskno,end= " ")
            ############################


           
           # case task in execution complete
            if(taskinexec._remainingexectime==0):  
                print("\nExecution completed for",taskinexec.taskno)   
                taskinexec=self.readyQ[0]
                self.readyQ.remove(taskinexec)
                readyQupdated=0
                print(Fore.RED,"BLAAAH ",Fore.RESET)
            elif(readyQupdated):   # update on ready Q
                readyQupdated=0
                #case premption switch happens   NOTE > because higher val (9999) means lower priority
                if(len(self.readyQ) > 0):
                    if(taskinexec.priority > self.readyQ[0].priority):  
                        print(Fore.BLUE,"\nContext switch",Fore.RESET)
                        temptask=self.readyQ[0]
                        self.readyQ.remove(temptask)
                        if(taskinexec._remainingexectime): ## if no remaining time we throw it away
                            self.readyQ.append(taskinexec)
                        taskinexec=temptask


           # the execution
            if taskinexec._remainingexectime > 0:
                taskinexec._remainingexectime -=1
                


            print("\n ####################")
            print(Fore.GREEN,"Slot :",tick," Taskin exec is",taskinexec.taskno,"With Remaining exec time =",taskinexec._remainingexectime)
            print(Style.RESET_ALL,"######################\n\n")
            
            self.exec_trace.append(taskinexec)

    # taskinexec=self.readyQ[0]

            pass

        pass




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



    # def updateSchedule(self):
    #     pass

    # def dispatcher(self):
    #     pass

    def prettyPrint(self):
        pass

    
    def __str__(self) -> str:
        print("----------------------------------")
        print("----------------------------------")
        for i in self.tasks:
            print(i)
        return("----------------------------------")




if __name__=="__main__":
    X=rm()
    X.getTaskParmeters()
    X.hyperpriod=X.calculateHyperPeriod()
    print("hyper period",X.hyperpriod)
    X.assignPriorities()

    if(X.checkSchedulablity() and X.checkRta()):
        X.schedule_Dispatch_Simulate()
        X.plot_trace()
        print(X)
    else:
        sys.exit()
    pass


