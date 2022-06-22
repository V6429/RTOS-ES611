#ifndef LPC2148_TIMER1
#define LPC2148_TIMER1

#include <LPC214x.h>
#include "gpio.h"

#define TIMERMR0BIT 0x01
#define TIMERMR1BIT 0x02
#define TIMERMR2BIT 0x04
#define TIMERMR3BIT 0x08

enum feature
{
    disable = 0u,
    enable = 1
};
enum matchregister
{
    MR0 = 0u,
    MR1,
    MR2,
    MR3

};

// set the timer peripheral to timer or counter mode
// mode1 for timerperipheral is timer
// mode2 for timerperipheral is counter
void TIMER1_setmode(char mode); // done

// read and clear interrupt
char TIMER1_readITR(char mr_no);                                                                // done
void TIMER1_clearITR(char mr_no);                                                               // done
void Timer1_setPR(int pr_value);                                                                // done
void Timer1_setMCR(char mr_no, char intr_onmatch, char resetTC_onmatch, char stopTCPC_onmatch); // done
void Timer1_setMR(char mr_no, int mr_value);                                                    // done
void TIMER1_start();                                                                            // done
void TIMER1_reset();                                                                            // done
char pll_apbdividervalue();                                                                     // done

#endif
