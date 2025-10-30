#ifndef FPU_H_
#define FPU_H_

#include "stm32f4xx.h"

#define CPACR_FULL_ACCESS			(0x0FUL)
#define CPACR_FULL_ACCESS_POS		(20)
#define CPACR_FULL_ACCESS_MODE		CPACR_FULL_ACCESS << CPACR_FULL_ACCESS_POS

static void FPU_Enable(void)
{
    /* Enable FPU */
    SCB->CPACR |= CPACR_FULL_ACCESS_MODE;  // CP10 and CP11 Full Access
}

#endif
