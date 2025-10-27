#include "led.h"

#define GPIOAEN					(1UL << 0)

#define MODER_MASK				(0x03UL)
#define MODER_OUTPUT      		(1UL)
#define PA5_MODER_POS           (10)

#define PIN5					(1UL << 5)
#define LED_PIN					(PIN5)

void LED_INIT(void)
{
	/* Enable clock access to GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* Set PA5 as an output pin */
	GPIOA->MODER &= ~(MODER_MASK << PA5_MODER_POS);
	GPIOA->MODER |= MODER_OUTPUT << PA5_MODER_POS;

	LED_OFF();
}

void LED_ON(void)
{
	GPIOA->BSRR = LED_PIN;
}

void LED_OFF(void)
{
	GPIOA->BSRR = (LED_PIN << 16);
}

void LED_TOGGLE(void)
{
	if(GPIOA->IDR & LED_PIN)
	{
		LED_OFF();
	}
	else
	{
		LED_ON();
	}
}
