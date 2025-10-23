#include "stm32f4xx.h"
#include <stdint.h>
/* SYST_CSR == SysTick->CTRL
 * SYST_RVR == SysTick->LOAD
 * SYST_CVR == SysTick->VAL
 * */
#define SYSTICK_LOAD			15999
#define SYSTICK_CTRL_ENABLE		(1UL << 0)
#define SYSTICK_CTRL_CLKSOURCE  (1UL << 2)
#define SYSTICK_CTRL_COUNTFLAG	(1UL << 16)

void SySTick_INIT(void)
{
	/* Reload with number of clocks per millisecond */
	SysTick->LOAD = SYSTICK_LOAD;

	/* Clear SysTick current value register */
	SysTick->VAL = 0;

	/* Enable SysTick and select internal clock source */
	SysTick->CTRL = SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_CLKSOURCE;
}

void SysTick_Delay_ms(uint32_t delay)
{
	for(int i = 0; i < delay; i++)
	{
		/* Wait until the COUNTFLAG is set */
		while((SysTick->CTRL & SYSTICK_CTRL_COUNTFLAG) == 0) {}
	}
}
