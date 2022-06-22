
#include "gpio.h"
#define BIT(x) (1 << x)

void GPIO_config(char port, char pin, char mode)
{
  if ((port == PORT0) && (pin <= 15))
  {
    pinsel0_config(pin, mode);
  }

  if ((port == PORT0) && (pin >= 16) && (pin <= 31))
  {
    pinsel1_config(pin, mode);
  }

  if (port == PORT1)
  {
    pinsel2_config(pin, mode);
  }
}

void pinsel0_config(char pin, char mode)
{
  char var = pin * 2;
  PINSEL0 = PINSEL0 & (~(BIT(var) | BIT(var + 1)));
  PINSEL0 = PINSEL0 | (mode << var);
}

void pinsel1_config(char pin, char mode)
{
  char var = (pin - 16) * 2;
  PINSEL1 = PINSEL1 & (~(BIT(var) | BIT(var + 1)));
  PINSEL1 = PINSEL1 | (mode << var);
}

void pinsel2_config(char pin, char mode)
{
  char var = (pin - 16) * 2;
  PINSEL2 = PINSEL2 & (~(BIT(var) | BIT(var + 1)));
  PINSEL2 = PINSEL2 | (mode << var);
}

void GPIO_setinput(char port, char pin)
{

  if (port == PORT0)
  {
    IO0DIR = IO0DIR & (~BIT(pin));
  }
  if (port == PORT1)
  {
    IO1DIR = IO1DIR & (~BIT(pin));
  }
}

char GPIO_readinput(char port, char pin)
{
  if (port == PORT0)
  {
    return ((IO0PIN & BIT(pin)) ? HIGH : LOW);
  }
  if (port == PORT1)
  {
    return ((IO1PIN & BIT(pin)) ? HIGH : LOW);
  }
}

void GPIO_setoutput(char port, char pin)
{
  if (port == PORT0)
    IO0DIR = IO0DIR | (BIT(pin));
  else if (port == PORT1)
    IO1DIR = IO1DIR | (BIT(pin));
}

void GPIO_write(char port, char pin, char state)
{
  if (port == PORT0)
  {
    if (state == HIGH)
      IO0SET = BIT(pin);
    else if (state == LOW)
      IO0CLR = BIT(pin);
  }
  else if (port == PORT1)
  {
    if (state == HIGH)
      IO1SET = BIT(pin);
    else if (state == LOW)
      IO1CLR = BIT(pin);
  }
}
