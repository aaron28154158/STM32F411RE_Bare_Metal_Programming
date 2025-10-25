#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "led.h"
#include "uart.h"
#include "exti.h"

/* ✅ 使用標誌位而非直接處理 */
volatile uint8_t button_pressed = 0;

int main(void)
{
    LED_INIT();
    UART2_TXRX_INIT();
    PC13_EXTI_INIT();

    while(1)
    {
        if (button_pressed)
        {
            button_pressed = 0;

            printf("BTN pressed... \n");
            LED_TOGGLE();
        }
    }
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & LINE13)
    {
        /* Clear flag */
        EXTI->PR = LINE13;

        /* Set flag for main loop */
        button_pressed = 1;
    }
}
