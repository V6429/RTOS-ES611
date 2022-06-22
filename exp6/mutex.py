from threading import *

prev_id = 1
sum = 0
stack = {}



def lock(id):
    global s
    global prev_id
    
    while (prev_id!=id) & (s==1) :
        pass
    if (prev_id != id ):
        if (s==0):
       # print("same task %d as before"%(id))
            s=1
            prev_id = id
    

def unlock(id):
    global s
    global prev_id
    s=0   


def running(var):
    global sum
    for j in range(1000000):
        lock(var)
        #print("Thread = %d, Iteration- %d"%(var,j))
        sum = sum + 1
        
    unlock(var)
    print(sum)


if __name__ =="__main__":
    for i in range(6):
        t = Thread(target=running , args = (i,))
        t.start() 




