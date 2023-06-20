/****************************************************/
// MSP432P401R
// ADC�ɼ�
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/13
/****************************************************/

/*********************
 *
 * ���ɼ���ѹ 3.3V
 *
 * ADC�ɼ����ţ�
 * ��· Ϊ P5.5
 * ˫· Ϊ P5.5 P5.4
 * ��· Ϊ P5.5 P5.4 P5.3
 *
 ************************/

#include "adc.h"
#include "usart.h"

// ��ʱ��  M*N*21us
#define N 1 // ��������
#define M 2 // ����ͨ������

uint16_t resultsBuffer[M];

uint16_t X[1024], Y[1024], ii = 0;
bool ADC_Flag = 1;

void ADC_Config(void)
{
    /* ���ø��������FPU */
    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();



    /* Initializing ADC (MCLK/1/1) */
    MAP_ADC14_enableModule();                                                                 // ʹ��ADC14ģ��
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_4, ADC_DIVIDER_2, ADC_NOROUTE); // ��ʼ��ADC ʱ�� ��Ƶ  ͨ�� 2.4MHz

#if M == 1
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5, GPIO_TERTIARY_MODULE_FUNCTION); // ģ������
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);                                                    // ��ͨ������ ���ת��true
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);       // ʹ���ڲ���Դ��ѹ�ο� �ǲ������false
    MAP_ADC14_enableInterrupt(ADC_INT0);                                                                    // ADCͨ��0���ж�

#elif M == 2
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN7 | GPIO_PIN6 | GPIO_PIN5 | GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true); // ��ͨ������ ���ת��true
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_EXTPOS_VREFNEG_EXTNEG, ADC_INPUT_A0, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_EXTPOS_VREFNEG_EXTNEG, ADC_INPUT_A1, false);
    MAP_ADC14_enableInterrupt(ADC_INT1);

#elif M == 3
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5 | GPIO_PIN4 | GPIO_PIN3, GPIO_TERTIARY_MODULE_FUNCTION); //
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM2, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A0, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A1, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A2, false);
    MAP_ADC14_enableInterrupt(ADC_INT2);

#endif
    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14); // ADCģ����ж�
    MAP_Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION); // �Զ�����

    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();        // ʹ�ܿ�ʼת��(������ �Զ�ADC�ϵ�)
    MAP_ADC14_toggleConversionTrigger(); // ������һ����������
}

void ADC14_IRQHandler(void)
{
    uint_fast64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

#if M == 1
    if (ADC_INT0 & status)
#elif M == 2
    if (ADC_INT1 & status)
#elif M == 3
    if (ADC_INT2 & status)
#endif
    {
        MAP_ADC14_getMultiSequenceResult(resultsBuffer);

        if (ADC_Flag)
        {
            X[ii] = resultsBuffer[0];
            Y[ii] = resultsBuffer[1];
            ii++;
        }
        if (ii >= 1024)
            ADC_Flag = 0;

        // LCD_TFT_pixel(resultsBuffer[0]/128 ,128 - resultsBuffer[1]/128, Blue);

        //        for (i = 0; i < M; i++)
        //            printf("[%d]:%d\r\t", i, resultsBuffer[i]);
        //        printf("\r\n");
    }
}