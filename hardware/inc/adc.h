/****************************************************/
// MSP432P401R
// ADC采集
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/13
/****************************************************/

#ifndef __ADC_H
#define __ADC_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define ADC_TRIGGER_TA0_C1 ADC_TRIGGER_SOURCE1
#define ADC_TRIGGER_TA0_C2 ADC_TRIGGER_SOURCE2
#define ADC_TRIGGER_TA1_C1 ADC_TRIGGER_SOURCE3
#define ADC_TRIGGER_TA1_C2 ADC_TRIGGER_SOURCE4
#define ADC_TRIGGER_TA2_C1 ADC_TRIGGER_SOURCE5
#define ADC_TRIGGER_TA2_C2 ADC_TRIGGER_SOURCE6
#define ADC_TRIGGER_TA3_C1 ADC_TRIGGER_SOURCE7

extern uint16_t resultsBuffer[2];

extern uint16_t X[1024], Y[1024], ii;
extern bool ADC_Flag;
void ADC_Config(void);

#endif
