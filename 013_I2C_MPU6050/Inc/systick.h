#ifndef SYSTICK_H_
#define SYSTICK_H_

void SySTick_INIT(void);
void SysTick_Delay_ms(uint32_t delay);
void SySTick_IRQ_INIT(void);

#endif
