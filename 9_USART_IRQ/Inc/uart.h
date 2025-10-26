#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"

#define SR_RXNE					(1UL << 5)
#define SR_ORE					(1UL << 3)

void UART2_TXRX_INIT(void);
void UART2_TXRX_IRQ_INIT(void);
char UART2_READ(void);
void UART2_WRITE(int ch);

#endif
