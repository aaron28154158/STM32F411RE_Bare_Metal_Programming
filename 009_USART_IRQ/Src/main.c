#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "systick.h"
#include "led.h"
#include "uart.h"
#include "exti.h"

void function(uint8_t);
static volatile uint8_t uart_rx_data;
static volatile uint8_t uart_rx_flag = 0;

int main(void)
{
    LED_INIT();
    UART2_TXRX_IRQ_INIT();
    SySTick_INIT();

    uint8_t local_rx_data;

    while(1)
    {
    	if(uart_rx_flag)
    	{
    		__disable_irq();
    		uart_rx_flag = 0;
    		local_rx_data = uart_rx_data;
    		__enable_irq();
    		function(local_rx_data);
    	}
    	else
    	{
    		LED_OFF();
    		__WFI();
    	}
    }
}

void function(uint8_t local_rx_data)
{
	if(local_rx_data == '1')
	{
		LED_ON();
	}
	else
	{
		LED_OFF();
	}
}

void USART2_IRQHandler(void)
{
	if((USART2->SR & SR_ORE))
	{
		(void)USART2->DR;
	}
	/* Check if RXNE is ready*/
	if((USART2->SR & SR_RXNE))
	{
		uart_rx_data = USART2->DR;
		uart_rx_flag = 1;
	}
}
