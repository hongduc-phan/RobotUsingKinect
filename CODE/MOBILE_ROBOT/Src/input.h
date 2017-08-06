/**
  ******************************************************************************
  * @file    input.h 
  * @author  Robot Club BK HCM
  * @version
  * @date
  * @brief
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__INPUT_H
#define __INPUT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define BUT0_Pin     GPIO_Pin_15
#define BUT0_Port    GPIOA
#define BUT1_Pin     GPIO_Pin_3
#define BUT1_Port    GPIOB
#define BUT2_Pin     GPIO_Pin_4
#define BUT2_Port    GPIOB

#define BLACK       (!GPIO_ReadInputDataBit(BUT1_Port, BUT1_Pin))
#define WHITE       (!GPIO_ReadInputDataBit(BUT2_Port, BUT2_Pin))
#define OK          (!GPIO_ReadInputDataBit(BUT0_Port, BUT0_Pin))

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Input_Init(void);

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
