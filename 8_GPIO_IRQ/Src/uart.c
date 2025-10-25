#include "uart.h"

#define GPIOAEN                 (1UL << 0)
#define USART2EN				(1UL << 17)

#define MODER_CLEAR_MASK 		(0x03UL)
#define MODER_ALTERNATE	(0x02UL)
#define MODER2_POS				(4)
#define MODER3_POS				(6)
#define MODER2_ALTERNATE_MODE 	MODER_ALTERNATE << MODER2_POS
#define MODER3_ALTERNATE_MODE 	MODER_ALTERNATE << MODER3_POS

#define AFR_CLEAR_MASK			(0x0FUL)
#define AFRL_AF07				(0x07UL)
#define AFRL2_POS				(8)
#define AFRL3_POS				(12)
#define AFRL2_AF07				(AFRL_AF07 << AFRL2_POS)
#define AFRL3_AF07				(AFRL_AF07 << AFRL3_POS)

#define CR1_RE					(1UL << 2)
#define CR1_TE					(1UL << 3)
#define CR1_UE					(1UL << 13)
#define CR1_OVER8 				(1UL << 15)
#define SR_RXNE					(1UL << 5)
#define SR_TXE					(1UL << 7)

#define SYS_FREQ				16000000
#define APB1_CLK				SYS_FREQ
#define UART_BAUDRATE			115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_brr(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);

int __io_putchar(int ch)
{
	UART2_WRITE(ch);
	return ch;
}

void UART2_TXRX_INIT(void)
{
	/* Configure UART GPIO Pin */
	/* Enable clock access to GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* Set PA2 mode to Alternate function mode */
	GPIOA->MODER &= ~(MODER_CLEAR_MASK << MODER2_POS);
	GPIOA->MODER |= MODER2_ALTERNATE_MODE;

	/* Set PA2 Alternate function type to USART2_TX (AF07) */
	GPIOA->AFR[0] &= ~(AFR_CLEAR_MASK << AFRL2_POS);
	GPIOA->AFR[0] |= AFRL2_AF07;

	/* Set PA3 mode to Alternate function mode */
	GPIOA->MODER &= ~(MODER_CLEAR_MASK << MODER3_POS);
	GPIOA->MODER |= MODER3_ALTERNATE_MODE;

	/* Set PA3 Alternate function type to USART2_RX (AF07) */
	GPIOA->AFR[0] &= ~(AFR_CLEAR_MASK << AFRL3_POS);
	GPIOA->AFR[0] |= AFRL3_AF07;


	/* Configure UART Module */
	/* Enable clock access to USART2 */
	RCC->APB1ENR |= USART2EN;

	/* Configure BaudRate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/* Configure the Transfer direction */
	USART2->CR1 |= CR1_RE;
	USART2->CR1 |= CR1_TE;

	/* Enable UART Module */
	USART2->CR1 |= CR1_UE;

}

char UART2_READ(void)
{
	/* Make sure the read data register is not empty */
	while(!(USART2->SR & SR_RXNE)) {}

	/* Read data */
	return USART2->DR;
}

void UART2_WRITE(int ch)
{
	/* Make sure the transmit data register is empty */
	while(!(USART2->SR & SR_TXE)) {}

	/* Write to transmit data register */
	USART2->DR = (ch & 0xFF);
}


static uint16_t compute_uart_brr(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    // uint32_t over8 = (USARTx->CR1 & CR1_OVER8) >> 15; // Check the OVER8 bit
    uint32_t usartdiv_x100; // Use integer math to avoid floats, scaled by 100
    uint16_t mantissa;
    uint16_t fraction;
    uint16_t brr_val;

	usartdiv_x100 = (PeriphClk * 100) / (BaudRate * 16);

	mantissa = usartdiv_x100 / 100;
	uint32_t fraction_x100 = usartdiv_x100 % 100;

	uint32_t div_f_x100_rounded = (fraction_x100 * 16) + 50;

	fraction = div_f_x100_rounded / 100;

	if (fraction >= 16)
	{
		mantissa++;
		fraction = 0;
	}

	brr_val = (mantissa << 4) | (fraction & 0x0F);

    return brr_val;
}

// And you would call it like this:
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    // Make sure to set OVER8 bit in CR1 *before* calling this function if you want 8x oversampling.
	USARTx->BRR = compute_uart_brr(USARTx, PeriphClk, BaudRate);
}
