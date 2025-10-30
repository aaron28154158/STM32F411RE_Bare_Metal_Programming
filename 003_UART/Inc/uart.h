#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
void UART2_TXRX_INIT(void);
char UART2_READ(void);
void UART2_WRITE(int ch);

#endif
