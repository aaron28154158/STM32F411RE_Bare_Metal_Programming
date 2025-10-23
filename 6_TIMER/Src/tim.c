#include "stm32f4xx.h"

#define APB1ENR_TIM2EN			(1UL << 0)
#define CR1_CEN					(1UL << 0)
#define SR_UIF					(1UL << 0)
#define EGR_UG					(1UL << 0)

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

void Wait_SR_UIF(void)
{
	while(!(TIM2->SR & SR_UIF)) {};
}

void Clear_SR_UIF(void)
{
	TIM2->SR &= ~SR_UIF;
}
