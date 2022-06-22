#ifndef sensor_send
#define sensor_send

#include <lpc214x.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "gpio.h"
#include "uart.h"
#include "spi0.h"
#include "timer1.h"
#include "utils.h"


#define DEBUG_LED




#define BUFFERSIZE 50
#define TIMEOUT 20
#define HIGH 1
#define LOW 0
#define BIT(x) (1 << (x))

#define SENSORNO_MAX 20
enum sensor_type
{
    DIGITAL = 0u,
    ANALOG
};


//#include "linkedlist.h"
// /**
//  * @brief Sensor_Node element for linked list.
//  * Modify this as per requirement.
//  *
//  */
// typedef struct sensor
// {
//     int sensor_number;
//     char sensor_type;
//     char sensor_lastvalue;
//     char sensor_port;
//     char sensor_pin;
//     struct node *nextNode;
// } NODE;

extern char sensor_pins[SENSORNO_MAX];
extern char sensor_ports[SENSORNO_MAX];
extern char sensor_type[SENSORNO_MAX];
extern char sensors_used;
// char sensor_ID[SENSORNO_MAX] = {0xff};

void sensor_add(char port, char pin, char type);
void sensor_remove(char port, char pin);
void sensor_init();
void sensor_startprocess(int repeat_interval);
void sensor_stopprocess();
__irq void sendroutine();

#endif