#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "systick.h"
#include "led.h"
#include "uart.h"
#include "exti.h"
#include "adc.h"

void function(uint16_t);
static volatile uint16_t g_adc_data;
static volatile uint8_t g_adc_flag = 0;

int main(void)
{

    UART2_TXRX_INIT();
    PA1_ADC_IRQ_INIT();
    ADC_Start_Conversion();

    while(1)
    {
		uint16_t local_adc_data;
		__disable_irq();
		g_adc_flag = 0;
		local_adc_data = g_adc_data;
		__enable_irq();

		function(local_adc_data);

		ADC_Start_Conversion();
    }
}

void function(uint16_t local_adc_data)
{
	printf("ADC value : %u \n", local_adc_data);
}

void ADC_IRQHandler(void)
{
	/* Check for EOC in SR */
	if((ADC1->SR & ADC1_SR_EOC))
	{
		g_adc_data = ADC1->DR;
		g_adc_flag = 1;
	}
}
