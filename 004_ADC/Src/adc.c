#include "stm32f4xx.h"
#include <stdint.h>

#define GPIOAEN				(1UL << 0)
#define ADC1EN				(1UL << 8)

#define MODER_ANALOG		(0x03UL)
#define MODER1_POS			(2)
#define MODER1_ANALOG_MODE	(MODER_ANALOG << MODER1_POS)

#define ADC1_CH1			(1UL << 0)
#define ADC1_SEQ_LEN_1		(0x00)

#define ADC1_CR1_RES		(0x00UL)
#define ADC1_CR2_ADON		(1UL << 0)
#define ADC1_CR2_SWSTART	(1UL << 30)

#define ADC1_SR_EOC				(1UL << 1)

void PA1_ADC_INIT(void)
{
	/* Configure the ADC GPIO Pin */
	/* Enable clock access to GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* Set the mode of PA1 to analog */
	GPIOA->MODER |= MODER1_ANALOG_MODE;

	/* Configure the ADC module */
	/* Enable clock access to ADC */
	RCC->APB2ENR |= ADC1EN;

	/* Set resolution to 12-bit */
	ADC1->CR1 |= ADC1_CR1_RES;

	/* Conversion sequence start */
	ADC1->SQR3 = ADC1_CH1;

	/* Conversion sequence length */
	ADC1->SQR1 = ADC1_SEQ_LEN_1;

	/* Enable ADC module */
	ADC1->CR2 |= ADC1_CR2_ADON;
}

void ADC_Start_Conversion(void)
{
	ADC1->CR2 |= ADC1_CR2_SWSTART;
}

uint32_t ADC_Read(void)
{
	/* Wait for conversion to be complete */
	while(!(ADC1->SR & ADC1_SR_EOC)) {}

	/* Read converted result */
	return ADC1->DR;
}
