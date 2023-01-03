/*
 * adc.c
 *
 *  Created on: 2022Äê12ÔÂ14ÈÕ
 *      Author: admin
 */
#include "adc.h"
#include "foc.h"

u16 Curbuf[3];
s16 Calibrattion_Val = 0;
void ADC_Function_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};
    NVIC_InitTypeDef NVIC_InitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigInjecConv_T1_CC4;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

  ADC_InjectedSequencerLengthConfig(ADC1, 3);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_71Cycles5);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_71Cycles5);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_71Cycles5);
  ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    ADC_ITConfig( ADC1, ADC_IT_JEOC, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_BufferCmd(ADC1, DISABLE);   //disable buffer
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    Calibrattion_Val = Get_CalibrationValue(ADC1);

    ADC_BufferCmd(ADC1, ENABLE);   //enable buffer
}


void Current_sample_init(void)
{

    ADC_Function_Init();

}


void ADC1_2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
u16 ee;
//u16 ADC_val[3];
void ADC1_2_IRQHandler()
{


    if(ADC_GetITStatus( ADC1, ADC_IT_JEOC)){
        Curbuf[0] = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
        Curbuf[1] = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_2);
        Curbuf[2] = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_3);
        ee+=1;
//        rtU.theta+=0.001;
//        if(rtU.theta>2*pi)rtU.theta-=2*pi;
        foc_step();
#if 0
        printf("ADC Extline trigger conversion...\r\n");
        printf( "JADC%04d\r\n", Get_ConversionVal(ADC_val));
#endif
    }

    ADC_ClearITPendingBit( ADC1, ADC_IT_JEOC);
}
