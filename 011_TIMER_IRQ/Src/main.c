#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "systick.h"
#include "led.h"
#include "uart.h"
#include "exti.h"
#include "adc.h"
#include "tim.h"

void function(void);
static volatile uint8_t g_timer_flag = 0;

int main(void)
{
    LED_INIT();
    UART2_TXRX_INIT();
    TIM2_IRQ_1HZ_INIT();

    while(1)
    {
    	if(g_timer_flag)
    	{
    		// NVIC_DisableIRQ(TIM2_IRQn); 禁用整個 TIM2 中斷
    		g_timer_flag = 0;
    		// NVIC_EnableIRQ(TIM2_IRQn);

    		function();
    		TIM2->DIER |= DIER_UIE; // 啟用 Update Interrupt
    	}
    	else
    	{
    		__WFI();
    	}
    }
}

void function(void)
{
	printf("A second passed \n");
	LED_TOGGLE();
}

void TIM2_IRQHandler(void)
{
	if(TIM2->SR & SR_UIF)
	{
		TIM2->SR &= ~(SR_UIF);
		g_timer_flag = 1;

		TIM2->DIER &= ~DIER_UIE; // 只禁用 Update Interrupt
	}
}
