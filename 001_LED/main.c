
/* Base addresses for peripherals */
#define PERIPHERAL_BASE         (0x40000000UL)
#define AHB1_PERIPHERAL_OFFSET  (0x00020000UL)
#define AHB1_PERIPHERAL_BASE    (PERIPHERAL_BASE + AHB1_PERIPHERAL_OFFSET)

/* GPIOA peripheral addresses and registers */
#define GPIOA_OFFSET            (0x0000UL)
#define GPIOA_BASE              (AHB1_PERIPHERAL_BASE + GPIOA_OFFSET)

#define GPIO_MODER_OFFSET       (0x00UL)
#define GPIOA_MODER_REGISTER    (*(volatile unsigned int *)(GPIOA_BASE + GPIO_MODER_OFFSET))

#define GPIO_ODR_OFFSET         (0x14UL)
#define GPIOA_ODR_REGISTER      (*(volatile unsigned int *)(GPIOA_BASE + GPIO_ODR_OFFSET))

/* RCC (Reset and Clock Control) peripheral addresses and registers */
#define RCC_OFFSET              (0x3800UL)
#define RCC_BASE                (AHB1_PERIPHERAL_BASE + RCC_OFFSET)
#define RCC_AHB1ENR_OFFSET      (0x30UL)
#define RCC_AHB1ENR_REGISTER    (*(volatile unsigned int *)(RCC_BASE + RCC_AHB1ENR_OFFSET))

/* Bit masks and positions for configuration */
#define GPIOAEN                 (1UL << 0)

#define MODER_OUTPUT_MASK       (0x01UL)
#define MODER5_POS              (10)

#define PIN5                    (1UL << 5)
#define LED_PIN                 PIN5


int main(void)
{
    /* 1. Enable clock access to GPIOA peripheral */
    RCC_AHB1ENR_REGISTER |= GPIOAEN;

    /* 2. Set PA5 as an output pin */
    GPIOA_MODER_REGISTER |= (MODER_OUTPUT_MASK << MODER5_POS);


    while(1)
    {
        /* 3. Turn on the LED */
        GPIOA_ODR_REGISTER |= LED_PIN;
    }
}
