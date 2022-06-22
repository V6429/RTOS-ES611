#ifndef BSP_timing
#define BSP_timing

#include "gpio.h"
#include "uart.h"
#include "spi0.h"
#include "utils.h"

#define BUFFERSIZE 100
#define TIMEOUT 20
#define HIGH 1
#define LOW  0

#define BIT(x)  (1<<(x))

///TODO: INTERRUPT LIBRARY,TIMER LIBRARY

void init_system();
void setInterval(int time);
void setCallBackFunction(void *funcptr);
void enableUartDebug(char set);

void startRepeatedProcess();
void stopRepeatedProcess();

void setUARTBuffer();
void setSPIBuffer();
void __interruptroutine()


#endif