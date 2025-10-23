#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "led.h"
#include "uart.h"
#include "adc.h"

#define CPACR_FULL_ACCESS			(0x0FUL)
#define CPACR_FULL_ACCESS_POS		(20)
#define CPACR_FULL_ACCESS_MODE		CPACR_FULL_ACCESS << CPACR_FULL_ACCESS_POS

static void delay_ms(uint32_t ms);

uint32_t adc_value;
float voltage;

int main(void)
{
	SCB->CPACR |= CPACR_FULL_ACCESS_MODE;

	UART2_TXRX_INIT();

    uint32_t ipsr_at_start = __get_IPSR();

    UART2_TXRX_INIT();

    printf("\n=== IPSR at main() start === \n");
    printf("IPSR (hex):     0x%08lX \n", ipsr_at_start);
    printf("IPSR (decimal): %d \n", (int) ipsr_at_start);

    if(ipsr_at_start == 0)
    {
        printf("Status: Thread Mode (Correct!) \n");
    }
    else
    {
        printf("Status: ERROR! IPSR should be 0! \n");
        printf("Exception: %d \n", (int) ipsr_at_start);
    }

    printf("============================ \n\n");

	PA1_ADC_INIT();
	delay_ms(1);

    while(1)
    {
    	/* Start ADC conversion */
		ADC_Start_Conversion();

		/* Read ADC value */
		adc_value = ADC_Read();

		/* Convert to voltage */
		voltage = (adc_value * 3.3f) / 4095.0f;

		/* Display results */
		printf("ADC Value: %4lu  |  Voltage: %.3f V \n", adc_value, voltage);

		/* Delay 500ms between readings */
		delay_ms(500);
    }
}

static void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < ms; i++)
    {
        for(volatile uint32_t j = 0; j < 4000; j++);
    }
}

