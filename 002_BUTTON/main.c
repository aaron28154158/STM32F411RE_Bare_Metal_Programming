#include "stm32f4xx.h"

#define GPIOAEN                 (1UL << 0)
#define GPIOCEN					(1UL << 2)

#define MODER_OUTPUT_MASK       (1UL)
#define MODER_INPUT_MASK       	(0UL)
#define MODER5_POS              (10)
#define MODER13_POS             (26)

#define OUTPUT_MODE				MODER_OUTPUT_MASK << MODER5_POS
#define INPUT_MODE				MODER_INPUT_MASK << MODER13_POS

#define PUPDR_PULL_UP_MASK   	(1UL)
#define PUPDR13_POS				(26)
#define PULL_UP					PUPDR_PULL_UP_MASK << PUPDR13_POS

#define PIN5                    (1UL << 5)
#define PIN13					(1UL << 13)
#define LED_PIN                 PIN5
#define BUTTON_PIN				PIN13

int main(void)
{
    /* 1. Enable clock access to GPIOA peripheral */
    RCC->AHB1ENR |= GPIOAEN;
    RCC->AHB1ENR |= GPIOCEN;

    /* 2. Set PA5 as an output pin */
    GPIOA->MODER &= ~(0x03UL << MODER5_POS);
    GPIOA->MODER |= OUTPUT_MODE;

    /* 3. Set PC13 as an output pin */
    GPIOC->MODER &= ~(0x03UL << MODER13_POS);
    GPIOC->MODER |= INPUT_MODE;

    /* !!! Set PC13 as Pull-Up */
    GPIOC->PUPDR &= ~(0x03UL << PUPDR13_POS);
    GPIOC->PUPDR |= PULL_UP;

    while(1)
    {
        /* 4. Check if BUTTON is pressed */
    	if(!(GPIOC->IDR & BUTTON_PIN))
    	{
    		GPIOA->BSRR = LED_PIN;
    	}
    	else
    	{
    		GPIOA->BSRR = (LED_PIN << 16);
    	}
    }
}
