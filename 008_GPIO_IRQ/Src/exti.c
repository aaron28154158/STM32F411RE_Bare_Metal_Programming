#include "stm32f4xx.h"

#define GPIOCEN						(1UL << 2)
#define APB2ENR_SYSCFGEN			(1UL << 14)

#define MODER_INPUT		 			(0x00)
#define MODER13_POS					(26)
#define MODER13_INPUT_MODE  		(MODER_INPUT << MODER13_POS)

#define THREE_BITS_CLEAR			(0x03UL)
#define FOUR_BITS_CLEAR				(0x0FUL)
#define MODER13_CLEAR_BIT			(THREE_BITS_CLEAR << MODER13_POS)


#define SYSCFG_PCx_PIN				(0x02UL)
#define SYSCFG_EXTICR4_EXTI13_POS	(4)
#define SYSCFG_EXTICR4_PC13			(SYSCFG_PCx_PIN << SYSCFG_EXTICR4_EXTI13_POS)

#define SYSCFG_EXTICR4_CLEAR		(FOUR_BITS_CLEAR << SYSCFG_EXTICR4_EXTI13_POS)

#define EXTI_IMR_MR13				(1UL << 13)
#define EXTI_FTSR_TR13				(1UL << 13)

void PC13_EXTI_INIT(void)
{
	/* Disable global interrupts */
	__disable_irq();
	/* Enable clock access for GPIOC */
	RCC->AHB1ENR |= GPIOCEN;

	/* Set PC13 as input mode */
	GPIOC->MODER &= ~(MODER13_CLEAR_BIT);
	GPIOC->MODER |= MODER13_INPUT_MODE;

	/* Enable clock access to STSCFG */
	RCC->APB2ENR |= APB2ENR_SYSCFGEN;

	/* Select PORTC for EXTI13 */
	SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_CLEAR);
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_PC13;

	/* Select falling edge trigger */
	EXTI->FTSR |= EXTI_FTSR_TR13;

	/* Enable EXTI line in NVIC */
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* UnMask EXTI13 */
	EXTI->IMR |= EXTI_IMR_MR13;

	/* Enable global interrupts */
	__enable_irq();
}


