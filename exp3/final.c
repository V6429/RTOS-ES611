#include <lpc214x.h>
#include "utils.h"

int GLOBAL_PULSE_COUNT = 0; // used to share info between pulse starter and irq
int GLOBAL_DELAY = 0;

int GLOBAL_NOOFTIMERRUNS = 0;
int GLOBAL_TIMER_PENDING_RUNTIME = 0;

int GLOBAL_TIMERLASTRUN = 0;
int GLOBAL_INTERRUPTUPDATE = 0;

void initpulsing_led();
void startpulsingled(int blink_nooftimes, unsigned int delay);
__irq void ledpulsingroutine();

int main()
{
    int i = 0;
    int pulses = -1;
    int delay = 90;
    IO1DIR = (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4);

    initpulsing_led();
    startpulsingled(pulses, delay); // -1 for infinite blinks


    while (1)
    {  // do somethin else here

   
        

        if (GLOBAL_INTERRUPTUPDATE)
        {	 
            GLOBAL_INTERRUPTUPDATE = 0;

            // on the last run case: a 71 multiple
            if (GLOBAL_TIMER_PENDING_RUNTIME == 0 && GLOBAL_TIMERLASTRUN)
            {		
                 toggleLED(LED4);
				 /// put stuff here as per requirement


				 ///////////

                if (GLOBAL_PULSE_COUNT-- > 0 | pulses == -1) //  updating pulses
                {  	if(pulses==-1)
					GLOBAL_PULSE_COUNT = -1;
                    startpulsingled(GLOBAL_PULSE_COUNT, delay); // do the entire process again	  this is a TODO
                }
                GLOBAL_TIMERLASTRUN = 0;

            } // on the last run case: NOT a 71 multiple
            else if (GLOBAL_TIMER_PENDING_RUNTIME > 0 && GLOBAL_TIMERLASTRUN)
            {	
                startpulsingled(pulses, GLOBAL_TIMER_PENDING_RUNTIME);
                GLOBAL_TIMER_PENDING_RUNTIME = 0;
                GLOBAL_INTERRUPTUPDATE = 0;
            }
            else
            {                               // on not the last run
                GLOBAL_INTERRUPTUPDATE = 0; // mcr will automatically reset and go counting again
                                            //  GLOBAL_NOOFTIMERRUNS decrement done by routine
            }
        }
    }

    return 0;
}

void initpulsing_led()
{ // this function configures led pulsing

    PINSEL2 &= 0xFFFFFFF7; // Pins P1.25-16 are used as GPIO pins when bit 3 is zero
    IO1DIR |= (1 << LED4);

    // configure timer
    T1CTCR = 0x00; // setting the timer/counter peripheral to timer mode
    T1TCR = 0x02;  // reset the timer contents in case some other function has used this
    T1PR = 0;      // prescale_value  keeping it zero
    // T1MR0 = 0xdeadbeef; // dummy
    T1MCR = 0x00000003; //   generate interrupt,reset TC on match,donot stop TC

    // configure interrupt
    VICIntSelect &= ~BIT(5); // VICIntSelect by default 4th bit is 0 representing IRQ
    VICVectCntl11 = 0x25;    // this is a enabled,timer1 interrupt
    VICVectAddr11 = (unsigned long int)ledpulsingroutine;
}

void startpulsingled(int blink_nooftimes, unsigned int delay)
{
    // this function initializes  global values
    GLOBAL_TIMER_PENDING_RUNTIME = 0;
    GLOBAL_PULSE_COUNT = blink_nooftimes;
    GLOBAL_DELAY = delay;
    if (delay - 71 <= 0)
    {
        GLOBAL_NOOFTIMERRUNS = 0;
        GLOBAL_TIMERLASTRUN = 1;

    }
    else
    {
        // GLOBAL_NOOFTIMERRUNS=delay/71.582878825;  /// actual  timer 71.56
        // 2^32 = 4,294,967,295
        // 60 000 000
        // ----->71.58278825
        // ----->reminder is 34967295 cycles  (the 0.582878825 part)
        // enter the adjustment cycles
        // if we are using the FFFF FFFF value on complete runs
        // we will have serous trouble
        // lot of clock cycles can get miscalculated & lost in float to int or int to float conversion
        // this error increases leniarly with no of runs
        // so sticking to MR value
        GLOBAL_NOOFTIMERRUNS = delay / 71;
        GLOBAL_TIMER_PENDING_RUNTIME = delay % 71;
    }
    VICIntEnable |= BIT(5); // enable interrupt for timer1

    if (!GLOBAL_NOOFTIMERRUNS) // if a single run set match register appropriatly
    {
        T1MR0 = 60000000 * delay;
    }
    else
    {
        T1MR0 =0xFDEA70FF;// 0xFDEA70FF; // 0xFDEA7100-1;  60Mhz x 71sec // max value 4260000000 at 71 seconds
        // 2158EFF
    }

    // start the clock
    T1TCR = 0x01; // stop resetting,start counting

    // 4294967295 / 60000000
}

__irq void ledpulsingroutine()
{  	toggleLED(LED2); 
    GLOBAL_INTERRUPTUPDATE = 1;
    if (GLOBAL_NOOFTIMERRUNS > 0)
    {
        GLOBAL_NOOFTIMERRUNS--;
    }

    if (GLOBAL_NOOFTIMERRUNS == 0 && !GLOBAL_TIMERLASTRUN) 
        GLOBAL_TIMERLASTRUN = 1;

    T1IR = TIMERMR0BIT;
    VICVectAddr = 1;


}
