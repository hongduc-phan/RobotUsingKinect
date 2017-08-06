/**
  ******************************************************************************
  * @file    common.h 
  * @author  Robot Club BK HCM
  * @version
  * @date
  * @brief	 Common header
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern GPIO_InitTypeDef GPIO_InitStructure;
extern NVIC_InitTypeDef NVIC_InitStructure;
extern USART_InitTypeDef USART_InitStructure;
extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
extern TIM_OCInitTypeDef  TIM_OCInitStructure;
extern EXTI_InitTypeDef   EXTI_InitStructure;
extern SPI_InitTypeDef   SPI_InitStructure;
extern I2C_InitTypeDef  I2C_InitStructure;
/* Private function prototypes -----------------------------------------------*/

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
