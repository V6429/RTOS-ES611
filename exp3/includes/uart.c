#include "uart.h"
#include "utils.h"

#define INTERRUPTIDENTIFICATIONMASK 0x0000000E

// TODO AUTOBAUDRATE
int _INTERUPDATE = 0, BUFRTAIL = 0, BUFRHEAD = 0;
char __TX = 0, __RX = 0;
char BUFFER[100];


char readRBR()
{
    // Disable DLAB before use
    return U0RBR;
}

void writeTHR(char transmitdata)
{
    // Disable DLAB before use
    while (!(U0LSR & (1 << 5)))
        ;
    U0THR = transmitdata;
}

void uartSendString(char *str)
{
    while (*str != '\0')
        writeTHR(*str++);

}
void enableDLAB()
{
    U0LCR |= 0x80;
}

void disableDLAB()
{
    U0LCR &= ~(0x80); //~(0x80)
}

void set_DLL_DLM_FDR(char dll, char dlm, char fdr)
{
    U0DLL = dll;
    U0DLM = dlm;
    U0FDR = (long int)fdr; // will also work with implicit typecast
                           // done here for note
                           // fdr mul val should be greater or equal to 0
    // pclk=15mhz
    // set dll=97
    // set dlm=0
    // get 9600 baud rate
    // fdr mul val=1 ad div val=0
}

void enableInterrupt(char intreg)
{
    U0IER = intreg;
    // 0x3    Receive Data Available interrupt
    //          THRE interrupt
    //
}

void enableFIFO()
{
    U0FCR |= 0x01;
}

void disableFIFO()
{
    U0FCR &= ~(0x01);
}

void cleanFIFOregs()
{
    U0FCR |= 0x06;
}

void setFIFOtriggerLevel(char triglevel)
{
    U0FCR &= ~(0x03 << 6); // clearing the bits
    U0FCR |= triglevel << 6;
}

void setLineCtrlReg()
{
    U0LCR &= ~(0x80); // clearing all bits ecept DLAB
    U0LCR |= 0x0B;    // // 8 bit //1 status // 1 parity
}
char getlinestatus()
{

    return U0LSR;
}

void enabletransmit()
{
    U0TER = 0x80;
}

void disabletransmit()
{
    U0TER &= ~0x80;
}

__irq void uart0routine()
{
    LED4_ON();
    _INTERUPDATE = 1;
    //  uartSendString("********** I AM INSIDE THE DAMN ROUTINE********** ");
    //  int iir_value;
    if (U0LSR & 0x01)
    {
        __RX = U0RBR;
        // U0THR = __RX;
        BUFFER[BUFRTAIL++] = __RX; // adding to buffer
    }
    VICVectAddr = U0IIR; // two things in one shot
}



void inituart(int baudrate)
{

    /// configure ports	 
	PINSEL0 &= (~0x0000000F) ;
    PINSEL0 = PINSEL0 | 0x00000005; /* Enable UART0 Rx0 and Tx0 pins of UART0 */
    enableDLAB();                 // DLAB bit 1
	if(baudrate==9600)
    set_DLL_DLM_FDR(97, 0, 0x10); // setting baud rate for 9600
    disableDLAB();

    // enable interrupt on uart
    enableInterrupt(0x01);
    setLineCtrlReg();
    // configure vic

    VICIntSelect &= ~(1 << 6); // irq
    VICVectAddr5 = (unsigned long int)uart0routine;
    VICVectCntl5 = 0x06 | 1 << 5; // setting int number and enabling it
    VICIntEnable |= 1 << 6;
    // setFIFOtriggerLevel(0x0);


    // enableFIFO();
    enabletransmit();

    // dne /// set a rx trig level
    // use lsr on main
    // clear the iir
}
