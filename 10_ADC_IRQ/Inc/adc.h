#ifndef ADC_H_
#define ADC_H_

#define ADC1_SR_EOC			(1UL << 1)

void PA1_ADC_INIT(void);
void PA1_ADC_IRQ_INIT(void);
void ADC_Start_Conversion(void);
uint32_t ADC_Read(void);

#endif
