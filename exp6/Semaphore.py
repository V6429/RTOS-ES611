#from concurrent.futures import Thread
#from multiprocessing import Semaphore
import random
from threading import *
from colorama import Fore,Back,Style
color_matrix = [Fore.RED,Fore.BLUE,Fore.GREEN,Fore.MAGENTA,Fore.YELLOW,Fore.CYAN,Fore.LIGHTGREEN_EX,Fore.WHITE]


s= 0
max_sem = 0


def setsemaphore(sema):
    global s
    global max_sem
    s=sema
    max_sem=sema

def wait():
    global s
    if s<0:
        s=0
    while s<=0:
        pass
    s=s-1

def signal():
    global s
    s=s+1
    if s>=max_sem:
        s=max_sem
    else:
        pass  




def running(var):
    wait()
    ## -------------- critical section starts here --------------------- ##
    color=random.choice(color_matrix)
    for i in range(10):
        print(color,var,"is running")
    print(color,"XXXXXXXXXXXXXXXXXXXX",var,"XXXXXXXXXXXXXXXXXXXX",end=" ")
    print(Fore.RESET)
    #### ----------------------- ends here --------------------------------- ##
    signal()
  

if __name__ == "__main__":  
    setsemaphore(2)


    t1 = Thread(target=running , args = (1,))
    t2 = Thread(target=running , args = (2,))
    t3 = Thread(target=running , args = (3,))
    t4 = Thread(target=running , args = (4,))
    t5 = Thread(target=running , args = (5,))
    t6 = Thread(target=running , args = (6,))
    t1.start()
    t2.start()
    t3.start()
    t4.start()
    t5.start()
    t6.start()


