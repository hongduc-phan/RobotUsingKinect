/**
  ******************************************************************************
  * @file    led.h 
  * @author  Pham Son Loc - Robot Club BK HCM
  * @version
  * @date
  * @brief	 Display led 7-segments module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Private define ------------------------------------------------------------*/
#define Led7_CLK_Pin			GPIO_Pin_14
#define Led7_CLK_Port           GPIOB
#define Led7_DAT_Pin			GPIO_Pin_15
#define Led7_DAT_Port           GPIOB

/* Extern variables ----------------------------------------------------------*/
extern int16_t num;

/* Private function prototypes -----------------------------------------------*/
void Led7_Init(void);
void clock7(void);
void outLed7(uint8_t data);
void calcNum(void);
void updateNum(void);
void blinky(uint8_t time);

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
