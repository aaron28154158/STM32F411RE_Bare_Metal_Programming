#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "led.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

uint32_t timestamp = 0;

/* Connect a jumper wire from PA5 to PA6 */
int main(void)
{
	TIM2_PA5_OUTPUT_COMPARE_INIT();
	TIM2_PA6_INPUT_CAPTURE_INIT();

    while(1)
    {
    	Wait_SR_CC1IF();

    	timestamp = Read_Capture_Value();
    }
}



