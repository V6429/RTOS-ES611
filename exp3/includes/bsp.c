#include "bsp.h"

// /// timing 

// int GLOBAL_PULSE_COUNT = 0; // used to share info between pulse starter and irq
// int GLOBAL_DELAY = 0;

// int GLOBAL_NOOFTIMERRUNS = 0;
// int GLOBAL_TIMER_PENDING_RUNTIME = 0;

// int GLOBAL_TIMERLASTRUN = 0;
// int GLOBAL_INTERRUPTUPDATE = 0;


// void init_system(){
//     GPIO_config(PORT1,LED4,MODE1);
//     GPIO_setoutput(PORT1,LED4);
//     GPIO_config(PORT1,LED2,MODE1);
//     GPIO_setoutput(PORT1,LED2);
//     // PINSEL2 &= 0xFFFFFFF7; // Pins P1.25-16 are used as GPIO pins when bit 3 is zero
//     // IO1DIR |= (1 << LED4);

//     // configure timer
//     T1CTCR = 0x00; // setting the timer/counter peripheral to timer mode
//     T1TCR = 0x02;  // reset the timer contents in case some other function has used this
//     T1PR = 0;      // prescale_value  keeping it zero
//     // T1MR0 = 0xdeadbeef; // dummy
//     T1MCR = 0x00000003; //   generate interrupt,reset TC on match,donot stop TC

//     // configure interrupt
//     VICIntSelect &= ~BIT(5); // VICIntSelect by default 4th bit is 0 representing IRQ
//     VICVectCntl11 = 0x25;    // this is a enabled,timer1 interrupt
//     VICVectAddr11 = (unsigned long int)__interruptroutine;

// }

// //TODO:
//  void __interruptroutine()
// {  	
//     toggleLED(LED2); 
//     GLOBAL_INTERRUPTUPDATE = 1;
//     if (GLOBAL_NOOFTIMERRUNS > 0)
//     {
//         GLOBAL_NOOFTIMERRUNS--;
//     }

//     if (GLOBAL_NOOFTIMERRUNS == 0 && !GLOBAL_TIMERLASTRUN) 
//         GLOBAL_TIMERLASTRUN = 1;

//     T1IR = TIMERMR0BIT;
//     VICVectAddr = 1;

// }