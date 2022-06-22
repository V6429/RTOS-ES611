#include "send_sensordata.h"

char sensor_pins[SENSORNO_MAX];
char sensor_ports[SENSORNO_MAX];
char sensor_type[SENSORNO_MAX];
// char sensor_ID[SENSORNO_MAX] = {0xff};
char sensors_used = 0;
char SENDBUFFER[BUFFERSIZE];

void sensor_add(char port, char pin, char type)
{
    sensor_ports[sensors_used] = port;
    sensor_pins[sensors_used] = pin;
    sensor_type[sensors_used] = type;
    sensors_used++;
}

void sensor_remove(char port, char pin)
{
    int i = 0;
    for (i = 0; i < sensors_used; i++)
        if ((sensor_ports[i] == port) && (sensor_pins[i] == pin))
        {
            for (; i < (sensors_used - 1); i++)
            {
                sensor_pins[i] = sensor_pins[i + 1];
                sensor_ports[i] = sensor_ports[i + 1];
                sensor_type[i] = sensor_type[i + 1];
            }
            sensors_used--;
            break;
        }
}

void sensor_init()
{	
	set_pll();
    int i = 0;
    for (i = 0; i < sensors_used; i++)
    {
        GPIO_config(sensor_ports[i], sensor_pins[i], MODE1);
        GPIO_setinput(sensor_ports[i], sensor_pins[i]);
    }

    // TODO UART
    inituart(BAUDRATE1); // 9600 // recive isr on  slot 5
    
    // configure interrupt
    VICIntSelect &= ~BIT(5); // VICIntSelect by default 4th bit is 0 representing IRQ
    VICVectCntl11 = 0x25;    // this is a enabled,timer1 interrupt
    VICVectAddr11 = (unsigned long int)sendroutine;
    // timer interrupt is not enbled here
#ifdef DEBUG_LED
    GPIO_config(PORT1, LED4, MODE1);
    GPIO_setoutput(PORT1, LED4);
#endif
}

void sensor_startprocess(int repeat_interval)
{
    VICIntEnable |= BIT(5); // enable interrupt for timer1
                            // configure timer & its interrupt

    // T1CTCR = 0x00; // setting the timer/counter peripheral to timer mode
    // T1TCR = 0x02;  // reset the timer contents in case some other function has used this
    // T1MR0 = repeat_interval;

    TIMER1_setmode(MODE1); // timer
    TIMER1_reset();
    Timer1_setMR(MR0,repeat_interval);    

    if (VPBDIV == 0x01) // pclk==60mhz     PCLK is same as CCLK i.e 60Mhz	when 01
    {
       Timer1_setPR(59999);
#ifdef DEBUG_LED
        GPIO_config(PORT1, LED2, MODE1);
        GPIO_setoutput(PORT1, LED2);
        toggleLED(LED2);
#endif
    }                        // Prescaler value for 1ms
    else if (VPBDIV == 0x00) // APB bus clock is one fourth of the processor clock
    {
        Timer1_setPR(15000);         // 239996;       // 1ms when
#ifdef DEBUG_LED
        GPIO_config(PORT1, LED1, MODE1);
        GPIO_setoutput(PORT1, LED1);
        toggleLED(LED1);
#endif
    }
    // T1MCR = 0x00000003; // generate interrupt,reset TC on match,donot stop TC
    Timer1_setMCR(MR0,HIGH,HIGH,LOW);
    // T1TCR = 0x01;       // start timer
    TIMER1_start();
}

void sensor_stopprocess()
{
    T1TCR = 0x02;
}

__irq void sendroutine()
{
    int i = 0;
    for (i = 0; i < sensors_used; i++)
    {
        snprintf(SENDBUFFER, BUFFERSIZE, "Sensor %d on P%d.%d reads=%d\n", i, (int)sensor_ports[i],
                 (int)sensor_pins[i], (int)GPIO_readinput(sensor_ports[i], sensor_pins[i]));
        uartSendString(SENDBUFFER);
    }
#ifdef DEBUG_LED
    toggleLED(LED4);
    //   GPIO_write(PORT1,  LED4,  HIGH);
#endif

    // T1IR = TIMERMR0BIT; // resetting interupt on channel MR0
    TIMER1_clearITR(MR0);
    VICVectAddr = 1;    // clearing on NVIC side
}