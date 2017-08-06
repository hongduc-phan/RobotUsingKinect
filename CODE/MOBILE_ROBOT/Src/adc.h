/**
  ******************************************************************************
  * @file    adc.h 
  * @author  Robot Club BK HCM
  * @version
  * @date
  * @brief
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)

#define ADC_High_Addr      0
#define ADC_Low_Addr       16

/* Extern variables ----------------------------------------------------------*/
extern uint8_t sensor, sensor_disp;
extern uint16_t ADC_Val[8];
extern uint8_t l4, l3, l2, l1, r1, r2, r3, r4;
extern uint8_t sen_sum;

/* Private function prototypes -----------------------------------------------*/
void delay_adc(uint16_t num);
void Sensor_Init(void);
void Read_Sensor(void);
void Get_ADC_Parameter_Flash(void);
void Calc_ADC_Sample(void);
void Push_ADC_Parameter_Flash(void);
void Get_High_Parameter(void);
void Get_Low_Parameter(void);

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
