#include "timer1.h"

// set the timer peripheral to timer or counter mode
void TIMER1_setmode(char mode)
{
    if (mode == MODE1)
        T1CTCR = 0x00; // setting the timer/counter peripheral to timer mode
}

// config interrupt
char TIMER1_readITR(char mr_no)
{
    switch (mr_no)
    { // returns bit high or low
    case 0:
        return (1 && (T1IR & TIMERMR0BIT));
    case 1:
        return (1 && (T1IR & TIMERMR1BIT));
    case 2:
        return (1 && (T1IR & TIMERMR2BIT));
    case 3:
        return (1 && (T1IR & TIMERMR3BIT));
    default:
        return 0;
    }
}
void TIMER1_clearITR(char mr_no)
{
    // Writing a logic one to the corresponding IR bit will reset
    //  the interrupt. Writing a zero has no effect.
    switch (mr_no)
    { // returns bit high or low
    case 0:
        T1IR= TIMERMR0BIT;
        break;
    case 1:
        T1IR =TIMERMR1BIT;
        break;
    case 2:
        T1IR= TIMERMR2BIT;
        break;
    case 3:
        T1IR =TIMERMR3BIT;
        break;
    }
}

void Timer1_setPR(int pr_value)
{
    T1PR = pr_value;
}

void Timer1_setMCR(char mr_no, char intr_onmatch, char resetTC_onmatch, char stopTCPC_onmatch)
{
    // 3 bit config stopTCPC_onmatch is temp  below
    stopTCPC_onmatch = (stopTCPC_onmatch << 2) | (resetTC_onmatch << 1) | (intr_onmatch);
    switch (mr_no)
    {
    case 0:
        T1MCR = T1MCR & (~0x07);      // clearing bits
        T1MCR = T1MCR | stopTCPC_onmatch; // adding new first 3 bits
        break;
    case 1:
        T1MCR = T1MCR & (~(0x07 << 3));      // clearing bits
        T1MCR = T1MCR | (stopTCPC_onmatch << 3); // adding new first 3 bits
        break;
    case 2:
        T1MCR = T1MCR & (~(0x07 << 6));      // clearing bits
        T1MCR = T1MCR | (stopTCPC_onmatch << 6); // adding new first 3 bits
        break;
    case 3:
        T1MCR = T1MCR & (~(0x07 << 9));      // clearing bits
        T1MCR = T1MCR | (stopTCPC_onmatch << 9); // adding new first 3 bits
        break;
    }
}

void Timer1_setMR(char mr_no, int mr_value)
{
    switch (mr_no)
    {
    case 0:
        T1MR0 = mr_value;
        break;
    case 1:
        T1MR1 = mr_value;
        break;
    case 2:
        T1MR2 = mr_value;
        break;
    case 3:
        T1MR3 = mr_value;
        break;
    }
}

void TIMER1_start()
{
    T1TCR = 0x01; // start the TC register
}

void TIMER1_reset()
{
    T1TCR = 0x02;
    //   When one, the Timer Counter and the Prescale Counter
    // are synchronously reset on the next positive edge of
    // PCLK. The counters remain reset until TCR[1] is
    // returned to zer
}
char pll_apbdividervalue()
{
    // only first two bits are defined on vpbdiv
    if ((VPBDIV & 0x03) == 0x01) // pclk==60mhz     PCLK is same as CCLK i.e 60Mhz	when 01
        return 1;
    if ((VPBDIV & 0x03) == 0x00) //// APB bus clock is one fourth of the processor clock
        return 4;
    if ((VPBDIV & 0x03) == 0x02) //// APB bus clock is one half of the processor clock
        return 2;
}
