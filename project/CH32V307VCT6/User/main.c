/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"
#include "led.h"
#include "foc.h"
#include "adc.h"
#include "tim.h"
#include "spi.h"
#include "fast_math.h"
/* Global typedef */


/* Global define */

/* Global Variable */


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
extern ExtU rtU;
int main(void)
{


    u8 i = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	TIM1_PWMOut_Init();           //互补pwm初始化

	Current_sample_init();        //电流采样初始化
	Delay_Init();
	USART_Printf_Init(115200);  //串口3 B10
	GPIO_Toggle_INIT();
	All_foc_init();             //foc初始化
	//spi_init();

	printf("SystemClk:%d\r\n",SystemCoreClock);

	printf("This is printf example\r\n");

	while(1)
    {
	    //printf("%0.2f\r\n",rtU.theta);
	    //printf("%0.2f\r\n",rtY.ia);
	    //printf("%0.2f,%0.2f\r\n",rtY.ialpha,rtY.ibeta);
	    //printf("%0.2f,%0.2f\r\n",rtY.iq,rtU.uq);
	    printf("%0.2f,%0.2f\r\n",rtU.ualpha,rtU.ubeta);
//	    Delay_Ms(100);
//	    GPIO_WriteBit(GPIOA, GPIO_Pin_15, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
	  //  spi_rx=SPI1_ReadWriteByte(0x3fff);
	}
}

