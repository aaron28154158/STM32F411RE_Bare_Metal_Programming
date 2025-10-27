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
// #define SR_RXNE					(1UL << 5)
#define SR_TXE					(1UL << 7)

#define SYS_FREQ				16000000
#define APB1_CLK				SYS_FREQ
#define UART_BAUDRATE			115200

/* IRQ -------------------*/
/* 中斷會在以下任一情況發生時觸發:
條件 1: RXNE = 1 (Receive data register Not Empty) -> 接收緩衝區有資料
條件 2: ORE = 1 (Overrun Error) -> 接收溢出錯誤
*/
#define CR1_RXNEIE						(1UL << 5) // RX Not Empty Interrupt Enable

/* DMA -------------------*/
#define AHB1ENR_DMA1EN					(1UL << 21)
#define DMA_SxCR_EN						(1UL << 0)
#define DMA_SxCR_CHSEL_CH4				(0x04UL << 25)
#define DMA_SxCR_MINC					(1UL << 10)
#define DMA_SxCR_DIR_MEM_TO_PERIPH		(1UL << 6)
#define DMA_SxCR_TCIE					(1UL << 4)
#define USART_CR3_DMAT					(1UL << 7)

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_brr(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);

int __io_putchar(int ch)
{
	UART2_WRITE(ch);
	return ch;
}

void DMA1_STREAM6_INIT(uint32_t src, uint32_t dst, uint16_t len)
{
	/* Enable clock access to DMA */
	RCC->AHB1ENR |= AHB1ENR_DMA1EN;

	/* Disable DMA1 Stream6 */
	DMA1_Stream6->CR &= ~(DMA_SxCR_EN);

	/* Wait until DMA1 Stream6 is disabled */
	while(DMA1_Stream6->CR & DMA_SxCR_EN){}

	/* Clear all interrupt flags of Stream6 */
	DMA1->HIFCR |= (1UL << 16);
	DMA1->HIFCR |= (0x0F << 18);

	/* Set the destination buffer */
	DMA1_Stream6->PAR = dst;

	/* Set the source buffer */
	DMA1_Stream6->M0AR = src;

	/* Set length */
	DMA1_Stream6->NDTR = len;

	/* Select Stream6 CH4 */
	DMA1_Stream6->CR |= DMA_SxCR_CHSEL_CH4;

	/* Enable memory increment */
	DMA1_Stream6->CR |= DMA_SxCR_MINC;

	/* Configure transfer direction */
	DMA1_Stream6->CR |= DMA_SxCR_DIR_MEM_TO_PERIPH;

	/* Enable direct mode and disable FIFO */
	DMA1_Stream6->FCR = 0;

	/* Enable UART2 transmitter DMA */
	USART2->CR3 |= USART_CR3_DMAT;

	/* Enable DMA transfer complete interrupt */
	DMA1_Stream6->CR |= DMA_SxCR_TCIE;

	/* Enable DMA1 Stream6 */
	DMA1_Stream6->CR |= DMA_SxCR_EN;

	/* Enable DMA interrupt */
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void UART2_TXRX_IRQ_INIT(void)
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

	/* Enable RXNE interrupt */
	USART2->CR1 |= CR1_RXNEIE;

	/* Enable UART2 interrupt in NVIC */
	NVIC_EnableIRQ(USART2_IRQn);

	/* Enable UART Module */
	USART2->CR1 |= CR1_UE;

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
