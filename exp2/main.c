#include <lpc214x.h>
#include <string.h>
#include <stdlib.h>
#include "gpio.h"

int main()
{

	char pin = 18;

	int value;

	PINSEL2 = PINSEL1 = PINSEL0 = 0xFFFFFFFF;

	GPIO_config(PORT0, 15, MODE1);
	while (1)
	{
		GPIO_config(PORT0, 30, MODE1);
		GPIO_setinput(PORT0, 30);
		value = GPIO_readinput(0, 30);
		if (value == 1)
		{
			GPIO_setoutput(PORT0, pin);
			GPIO_write(PORT0, pin, HIGH);
		}
	}
	return 0;
}
