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
#ifndef	__BUZZER_H
#define __BUZZER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define BUZZER_Pin			GPIO_Pin_12
#define	BUZZER_Port			GPIOB
#define BUZZER_ClockSource	RCC_APB2Periph_GPIOB

#define BUZZER_ON	GPIO_WriteBit(BUZZER_Port, BUZZER_Pin, Bit_SET)
#define BUZZER_OFF	GPIO_WriteBit(BUZZER_Port, BUZZER_Pin, Bit_RESET)

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Buzzer_Init(void);
void beep(uint16_t time);
void beep_long(uint8_t n_time, uint16_t time);

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
