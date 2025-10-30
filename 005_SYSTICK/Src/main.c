#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "led.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

int main(void)
{
	LED_INIT();
	UART2_TXRX_INIT();
	SySTick_INIT();

    while(1)
    {
    	printf("A second passed... \n");
    	LED_TOGGLE();

    	SysTick_Delay_ms(1000);
    }
}



