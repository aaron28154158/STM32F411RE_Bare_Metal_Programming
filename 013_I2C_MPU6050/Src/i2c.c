#include "stm32f4xx.h"

#define AHB1ENR_GPIOBEN				(1UL << 1)
#define APB1ENR_I2C1EN				(1UL << 21)

#define CLEAR_ONE_BITS				(1UL)
#define CLEAR_TWO_BITS				(0x03UL)
#define CLEAR_FOUR_BITS				(0x0FUL)
#define MODER_ALTERNATE_FUNCTION	(0x02UL)
#define MODER8_POS					(16)
#define MODER9_POS					(18)

#define OTYPER_OPEN_DRAIN			(1UL)
#define OTYPER_OT8_POS				(8)
#define OTYPER_OT9_POS				(9)

#define PUPDR_PULL_UP				(1UL)
#define PUPDR8_POS					(16)
#define PUPDR9_POS					(18)

#define AFRH_AF04					(0x04UL)
#define AFRH8_POS					(0)
#define AFRH9_POS					(4)

#define CR1_SWRST					(1UL << 15)
#define CR2_FREQ_16MHZ				(1UL << 4)
/*	APB1 頻率 	: 16 MHz
 * 	I2C  目標頻率 : 100 kHz (標準模式)
 *
 * */
#define I2C_CCR_100KHZ_16MHZ		(80)
#define I2C_TRISE_STANDARD_16MHZ	(17)
#define CCR_CCR						I2C_CCR_100KHZ_16MHZ
#define TRISE_TRISE					I2C_TRISE_STANDARD_16MHZ

#define CR1_PE						(1UL << 0)

void I2C1_INIT(void)
{
	/* Enable clock access to GPIOB */
	RCC->AHB1ENR |= AHB1ENR_GPIOBEN;

	/* Set PB8 and PB9 mode to alternate function */
	GPIOB->MODER &= ~(CLEAR_TWO_BITS << MODER8_POS);
	GPIOB->MODER &= ~(CLEAR_TWO_BITS << MODER9_POS);
	GPIOB->MODER |= (MODER_ALTERNATE_FUNCTION << MODER8_POS);
	GPIOB->MODER |= (MODER_ALTERNATE_FUNCTION << MODER9_POS);

	/* Set PB8 and PB9 output type to open drain */
	GPIOB->OTYPER &= ~(CLEAR_ONE_BITS << OTYPER_OT8_POS);
	GPIOB->OTYPER &= ~(CLEAR_ONE_BITS << OTYPER_OT9_POS);
	GPIOB->OTYPER |= (OTYPER_OPEN_DRAIN << OTYPER_OT8_POS);
	GPIOB->OTYPER |= (OTYPER_OPEN_DRAIN << OTYPER_OT9_POS);

	/* Enable Pull Up for PB8 and PB9 */
	GPIOB->PUPDR &= ~(CLEAR_TWO_BITS << PUPDR8_POS);
	GPIOB->PUPDR &= ~(CLEAR_TWO_BITS << PUPDR9_POS);
	GPIOB->PUPDR |= (PUPDR_PULL_UP << PUPDR8_POS);
	GPIOB->PUPDR |= (PUPDR_PULL_UP << PUPDR9_POS);

	/* Set PB8, PB9 Alternate function type to I2C1_SCL, I2C1_SDA */
	GPIOB->AFR[1] &= ~(CLEAR_FOUR_BITS << AFRH8_POS);
	GPIOB->AFR[1] &= ~(CLEAR_FOUR_BITS << AFRH9_POS);
	GPIOB->AFR[1] |= (AFRH_AF04 << AFRH8_POS);
	GPIOB->AFR[1] |= (AFRH_AF04 << AFRH9_POS);

	/* Enable clock access to I2C1 */
	RCC->APB1ENR |= APB1ENR_I2C1EN;

	/* Enter reset mode */
	I2C1->CR1 |= CR1_SWRST;

	/* Come out of reset mode */
	I2C1->CR1 &= ~CR1_SWRST;

	/* Set Peripheral clock frequency */
	I2C1->CR2 = (1UL << 4);

	/* Set I2C to standard mode, 100kHz clock */
	I2C1->CCR = CCR_CCR;

	/* Set rise time */
	I2C1->TRISE = TRISE_TRISE;

	/* Enable I2C module */
	I2C1->CR1 |= CR1_PE;
}

#define SR2_BUSY					(1UL << 1)
#define CR1_START					(1UL << 8)
#define SR1_SB						(1UL << 0)
#define SR1_ADDR					(1UL << 1)
#define SR1_TxE						(1UL << 7)
#define CR1_ACK						(1UL << 10)
#define CR1_STOP					(1UL << 9)
#define SR1_RxNE					(1UL << 6)

void I2C1_byteRead(char source_addr, char memory_addr, char* data)
{
	volatile int tmp;

	/* Wait until bus not busy */
	while(I2C1->SR2 & SR2_BUSY) {}

	/* Generate start */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_SB)) {}

	/* Transmit slave address + Write */
	I2C1->DR = source_addr << 1;

	/* wait until address flag is set */
	while(!(I2C1->SR1 & SR1_ADDR)) {}

	/* Clear address flag */
	tmp = I2C1->SR1;
	tmp = I2C1->SR2;
	(void)tmp;

	/* Wait until transmitter data register empty */
	while(!(I2C1->SR1 & SR1_TxE)) {}

	/* Send memory address */
	I2C1->DR = memory_addr;

	/* Wait until transmitter data register empty */
	while(!(I2C1->SR1 & SR1_TxE)) {}

	/* Generate start */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_SB)) {}

	/* Transmit slave address + Read */
	I2C1->DR = source_addr << 1 | 1;

	/* wait until address flag is set */
	while(!(I2C1->SR1 & SR1_ADDR)) {}

	/* Disable Acknowledge */
	I2C1->CR1 &= ~(CR1_ACK);

	/* Clear address flag */
    tmp = I2C1->SR1;
    tmp = I2C1->SR2;
    (void)tmp;

	/* Generate stop after data received */
	I2C1->CR1 |= CR1_STOP;

	/* Wait until RxNE flag is set */
	while(!(I2C1->SR1 & SR1_RxNE)) {}

	/* Read data from DR */
	*(data++) = I2C1->DR;
}

void I2C1_burstRead(char source_addr, char memory_addr, int n, char* data)
{
	volatile int tmp;

	/* Wait until bus not busy */
	while(I2C1->SR2 & SR2_BUSY) {}

	/* Generate start */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_SB)) {}

	/* Transmit slave address + Write */
	I2C1->DR = source_addr << 1;

	/* wait until address flag is set */
	while(!(I2C1->SR1 & SR1_ADDR)) {}

	/* Clear address flag */
	tmp = I2C1->SR1;
	tmp = I2C1->SR2;
	(void)tmp;

	/* Wait until transmitter data register empty */
	while(!(I2C1->SR1 & SR1_TxE)) {}

	/* Send memory address */
	I2C1->DR = memory_addr;

	/* Wait until transmitter data register empty */
	while(!(I2C1->SR1 & SR1_TxE)) {}

	/* Generate start */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_SB)) {}

	/* Transmit slave address + Read */
	I2C1->DR = source_addr << 1 | 1;

	/* wait until address flag is set */
	while(!(I2C1->SR1 & SR1_ADDR)) {}

	/* Enable Acknowledge */
	I2C1->CR1 |= CR1_ACK;

	/* Clear address flag */
    tmp = I2C1->SR1;
    tmp = I2C1->SR2;
    (void)tmp;

	while(n > 0UL)
	{
		/* If one byte */
		if(n == 1UL)
		{
			/* Disable Acknowledge */
			I2C1->CR1 &= ~(CR1_ACK);

			/* Generate stop after data received */
			I2C1->CR1 |= CR1_STOP;

			/* Wait until RxNE flag is set */
			while(!(I2C1->SR1 & SR1_RxNE)) {}

			/* Read data from DR */
			*(data++) = I2C1->DR;

			break;
		}
		else
		{
			/* Wait until RxNE flag is set */
			while(!(I2C1->SR1 & SR1_RxNE)) {}

			/* Read data from DR */
			*(data++) = I2C1->DR;

			n--;
		}
	}
}

#define SR1_BTF						(1UL << 2)

void I2C1_burstWrite(char source_addr, char memory_addr, int n, char* data)
{
	volatile int tmp;

	/* Wait until bus not busy */
	while(I2C1->SR2 & SR2_BUSY) {}

	/* Generate start */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_SB)) {}

	/* Transmit slave address + Write */
	I2C1->DR = source_addr << 1;

	/* wait until address flag is set */
	while(!(I2C1->SR1 & SR1_ADDR)) {}

	/* Clear address flag */
	tmp = I2C1->SR1;
	tmp = I2C1->SR2;
	(void)tmp;

	/* Wait until transmitter data register empty */
	while(!(I2C1->SR1 & SR1_TxE)) {}

	/* Send memory address */
	I2C1->DR = memory_addr;

	for(int i = 0; i < n; i++)
	{
		/* Wait until transmitter data register empty */
		while(!(I2C1->SR1 & SR1_TxE)) {}

		/* Transmit memory address */
		I2C1->DR = *data++;
	}

	/* Wait until transmitter data register empty */
	while(!(I2C1->SR1 & SR1_TxE)) {}
	/* Wait until transfer finished */
	while(!(I2C1->SR1 & SR1_BTF)) {}

	/* Generate stop after data received */
	I2C1->CR1 |= CR1_STOP;
}
