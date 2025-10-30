#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "led.h"
#include "uart.h"

static void delay_ms(uint32_t ms);

char key;

int main(void)
{
	LED_INIT();
	UART2_TXRX_INIT();
	delay_ms(100);

    while(1)
    {
    	key = UART2_READ();

    	if(key == '1')
    	{
    		LED_ON();
    		printf("LED ON \n");
    	}
    	else if(key == '0')
    	{
    		LED_OFF();
    		printf("LED OFF \n");
    	}
    	else
    	{
    		printf("Unknown command \n");
    	}
    }
}

static void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < ms; i++)
    {
        for(volatile uint32_t j = 0; j < 4000; j++);
    }
}

