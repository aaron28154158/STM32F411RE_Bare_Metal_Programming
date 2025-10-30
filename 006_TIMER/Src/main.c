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
	TIM2_1HZ_INIT();

    while(1)
    {
    	Wait_SR_UIF();
    	Clear_SR_UIF();

    	printf("A second passed... \n");

    	LED_TOGGLE();
    }
}



