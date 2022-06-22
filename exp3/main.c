#include <lpc214x.h>
#include "includes/send_sensordata.h"

int main(){

    // sensoradd(params)
    // sensorremove(params)
    // initsensorprocess
    // start
    // stop

	//for debug
	//PINSEL0=0xFFFFFFFF;
	//PINSEL1=0;
    
    sensor_add(PORT0,15,DIGITAL);
    sensor_add(PORT0,13,DIGITAL);
    sensor_add(PORT0,12,DIGITAL);
    sensor_add(PORT0,30,DIGITAL);
    sensor_init();
    sensor_startprocess(5000);


    while(1){
        // do something
    }

    return 0;
}