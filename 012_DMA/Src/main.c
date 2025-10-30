#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "systick.h"
#include "led.h"
#include "uart.h"
#include "exti.h"
#include "adc.h"
#include "tim.h"

void DMA_function(void);
static volatile uint8_t g_dma_flag = 0;

int main(void)
{
	char message[31] = "Hello from STM32 DMA transfer \n";

    LED_INIT();
    UART2_TXRX_INIT();
    DMA1_STREAM6_INIT((uint32_t) message, (uint32_t) &(USART2->DR), 31);

    while(1)
    {
    	if(g_dma_flag)
    	{
    		g_dma_flag = 0;
    		DMA_function();
    	}
    }
}

void DMA_function(void)
{
	LED_ON();
}

void DMA1_Stream6_IRQHandler(void)
{
	/* Check for transfer complete interrupt */
	if(DMA1->HISR & HISR_TCIF6)
	{
		/* Clear flag */
		DMA1->HIFCR |= HIFCR_CTCIF6;
		g_dma_flag = 1;
	}
}
