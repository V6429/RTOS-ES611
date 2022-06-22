#from concurrent.futures import thread
#from multiprocessing import Semaphore
import random
from threading import *
from colorama import Fore,Back,Style
color_matrix = [Fore.RED,Fore.BLUE,Fore.GREEN,Fore.MAGENTA,Fore.YELLOW]

def setsemaphore():
    global s
    s = 0

def wait():
    global s
    while s==1:
        pass
    s=1

def signal():
    global s
    s=0   


def running(var):
    color=random.choice(color_matrix)
    wait()
    #print(ident)
    for i in range(10):
        print(color,var,"is running")
    print(Fore.RESET)
    signal()
  


if __name__ == "__main__":  


    setsemaphore()

    for i in range(6):
        t = Thread(target=running , args = (i,))
        t.start()
