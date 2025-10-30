#ifndef TIM_H_
#define TIM_H_

#define SR_UIF							(1UL << 0)
#define DIER_UIE						(1UL << 0)

void TIM2_1HZ_INIT(void);
void TIM2_IRQ_1HZ_INIT(void);
void TIM2_PA5_OUTPUT_COMPARE_INIT(void);
void TIM2_PA6_INPUT_CAPTURE_INIT(void);
void Wait_SR_UIF(void);
void Wait_SR_CC1IF(void);
void Clear_SR_UIF(void);
uint32_t Read_Capture_Value(void);

#endif
