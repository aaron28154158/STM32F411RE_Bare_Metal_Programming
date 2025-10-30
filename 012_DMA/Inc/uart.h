#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"

#define SR_RXNE					(1UL << 5)
#define SR_ORE					(1UL << 3)
#define HISR_TCIF6				(1UL << 21)
#define HIFCR_CTCIF6			(1UL << 21)

void UART2_TXRX_INIT(void);
void UART2_TXRX_IRQ_INIT(void);
char UART2_READ(void);
void UART2_WRITE(int ch);
int __io_putchar(int ch);
void DMA1_STREAM6_INIT(uint32_t src, uint32_t dst, uint16_t len);

#endif
