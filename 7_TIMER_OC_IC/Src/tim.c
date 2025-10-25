#include "stm32f4xx.h"

#define AHB1ENR_GPIOAEN					(1UL << 0)
#define MODER_ALTERNATE_FUNCTION		(0x02UL)
#define MODER5_POS						(10)
#define MODER6_POS						(12)
#define MODER5_ALTERNATE_FUNCTION_MODE	(MODER_ALTERNATE_FUNCTION << MODER5_POS)
#define MODER6_ALTERNATE_FUNCTION_MODE	(MODER_ALTERNATE_FUNCTION << MODER6_POS)

#define AFRL_AF01						(1UL)
#define AFRL_AF02						(0x02UL)
#define AFRL5_POS						(20)
#define AFRL6_POS						(24)
#define AFRL5_AF01						(AFRL_AF01 << AFRL5_POS)
#define AFRL6_AF02						(AFRL_AF02 << AFRL6_POS)

#define APB1ENR_TIM2EN					(1UL << 0)
#define APB1ENR_TIM3EN					(1UL << 1)
#define CR1_CEN							(1UL << 0)
#define SR_UIF							(1UL << 0)
#define SR_CC1IF						(1UL << 1)
#define EGR_UG							(1UL << 0)

#define CCMR_OC1M_TOGGLE				(0x03UL)
#define CCMR_OC1M_TOGGLE_POS			(4)
#define CCMR_OC1M_TOGGLE_MODE			(CCMR_OC1M_TOGGLE << CCMR_OC1M_TOGGLE_POS)

#define CCMR_CC1S_OUTPUT				(0x00UL)
#define CCMR_CC1S_INPUT_TI1				(1UL << 0)

#define CCER_CC1E						(1UL << 0)

void TIM2_1HZ_INIT(void)
{
	/* Enable clock access to TIM2 */
	RCC->APB1ENR |= APB1ENR_TIM2EN;

	/* Set PreScaler value */
	TIM2->PSC = 1600 - 1;

	/* Set Auto-Reload value */
	TIM2->ARR = 10000 - 1;

	/* Clear counter */
	TIM2->CNT = 0;

	/* Update generation */
	TIM2->EGR = EGR_UG;

	/* Enable counter */
	TIM2->CR1 |= CR1_CEN;
}

void TIM2_PA5_OUTPUT_COMPARE_INIT(void)
{
	/* Enable clock access to GPIOA */
	RCC->AHB1ENR |= AHB1ENR_GPIOAEN;

	/* Set PA5 mode to alternate function */
	GPIOA->MODER |= MODER5_ALTERNATE_FUNCTION_MODE;

	/* Set PA5 alternate function type to TIM2_CH1 (AF01) */
	GPIOA->AFR[0] |= AFRL5_AF01;

	/* _________________________________ */

	/* Enable clock access to TIM2 */
	RCC->APB1ENR |= APB1ENR_TIM2EN;

	/* Set PreScaler value */
	TIM2->PSC = 1600 - 1;

	/* Set Auto-Reload value */
	TIM2->ARR = 10000 - 1;

	/* Set output compare toggle mode */
	TIM2->CCMR1 = CCMR_OC1M_TOGGLE_MODE;

	/* Enable TIM2 CH1  */
	TIM2->CCER |= CCER_CC1E;

	/* Clear counter */
	TIM2->CNT = 0;

	/* Update generation */
	TIM2->EGR = EGR_UG;

	/* Enable counter */
	TIM2->CR1 |= CR1_CEN;
}

void TIM2_PA6_INPUT_CAPTURE_INIT(void)
{
	/* Enable clock access to GPIOA */
	RCC->AHB1ENR |= AHB1ENR_GPIOAEN;

	/* Set PA6 mode to alternate function */
	GPIOA->MODER |= MODER6_ALTERNATE_FUNCTION_MODE;

	/* Set PA6 alternate function type to TIM3_CH1 (AF02) */
	GPIOA->AFR[0] |= AFRL6_AF02;

	/* Enable clock access to TIM3 */
	RCC->APB1ENR |= APB1ENR_TIM3EN;

	/* Set PreScaler */
	TIM3->PSC = 16000 -1;

	/* Set CH1 to input mode */
	TIM3->CCMR1 |= CCMR_CC1S_INPUT_TI1;

	/* Enable TIM3 CH1  */
	TIM3->CCER |= CCER_CC1E;

	/* Enable TIM3 */
	TIM3->CR1 |= CR1_CEN;
}

void Wait_SR_UIF(void)
{
	while(!(TIM2->SR & SR_UIF)) {};
}

void Wait_SR_CC1IF(void)
{
	while(!(TIM3->SR & SR_CC1IF)) {};
}

void Clear_SR_UIF(void)
{
	TIM2->SR &= ~SR_UIF;
}

uint32_t Read_Capture_Value(void)
{
	return TIM3->CCR1;
}
