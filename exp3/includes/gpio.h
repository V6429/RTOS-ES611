#ifndef GPIO_LPC
#define GPIO_LPC
#define HIGH 1
#define LOW 0
#define BIT(x) (1 << x)
#include <lpc214x.h>
// mode1 = gpio
// mode2 = alternate function
// mode3 = alternate function
// mode4 = alternate function

enum
{
	MODE1 = 0u, // gpio
	MODE2,		// alternate1 refer datasheet
	MODE3,		// alternate
	MODE4
}; // alternate

enum
{
	PORT0 = 0,
	PORT1
};
// set mode(functionality) for a pin
void GPIO_config(char port, char pin, char mode);

// for respective ports
// port0
void pinsel0_config(char pin, char mode);
void pinsel1_config(char pin, char mode);
// port1
void pinsel2_config(char pin, char mode);

void GPIO_setinput(char port, char pin);
char GPIO_readinput(char port, char pin);

void GPIO_setoutput(char port, char pin);
void GPIO_write(char port, char pin, char state);

#endif
// char choose_mode(char mode);