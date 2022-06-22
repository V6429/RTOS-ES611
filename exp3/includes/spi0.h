#ifndef SPI0
#define SPI0
#include "gpio.h"
//-------------MASTER OPERATION
// 1. Set the SPI clock counter register to the desired clock rate.
// 2. Set the SPI control register to the desired settings.
// 3. Write the data to transmitted to the SPI data register. This write starts the SPI data
// transfer.
// 4. Wait for the SPIF bit in the SPI status register to be set to 1. The SPIF bit will be set
// after the last cycle of the SPI data transfer.
// 5. Read the SPI status register.
// 6. Read the received data from the SPI data register (optional).
// 7. Go to step 3 if more data is required to transmit.


//----------------OPERATIONS
// 1. Set the SPI control register to the desired settings.
// 2. Write the data to transmitted to the SPI data register (optional). Note that this can only
// be done when a slave SPI transfer is not in progress.
// 3. Wait for the SPIF bit in the SPI status register to be set to 1. The SPIF bit will be set
// after the last sampling clock edge of the SPI data transfer.
// 4. Read the SPI status register.
// 5. Read the received data from the SPI data register (optional).
// 6. Go to step 2 if more data is required to transmit


extern int __intupdate;
extern char RX,TX;
extern char SPI_BUFFER[100];

/// user should call setcs after init
void spi0_masterinit();
void spi0_slaveinit();
void spi0_setCs(char port,char pin);

void spi0_slavecntrl(char enable);
char spi0_masterwrite(char data);
char spi0_masterread();


char spi0_slavewrite(char data);
char spi0_slaveread();

__irq void spi0_slaveroutine();

#endif