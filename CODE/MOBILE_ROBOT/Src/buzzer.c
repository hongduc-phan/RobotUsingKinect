/**
  ******************************************************************************
  * @file    input.c 
  * @author  Robot Club BK HCM
  * @version
  * @date
  * @brief	Main input body
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "buzzer.h"

#include "systick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
Function name:
Decription:
Input: None
Output: None
*******************************************************************************/
void Buzzer_Init(void)
{
	GPIO_InitTypeDef             GPIO_InitStructure;
	
    /* GPIOx clock enable */
    RCC_APB2PeriphClockCmd(BUZZER_ClockSource, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = BUZZER_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_Port, &GPIO_InitStructure);
	
	BUZZER_OFF;
}

/*******************************************************************************
Function name: beep
Decription: Turn on buzzer
*******************************************************************************/
void beep(uint16_t time)
{
	cnt0 = 0;
	while (cnt0 < time)
		BUZZER_ON;
	BUZZER_OFF;
}


/*******************************************************************************
Function name:
Decription:
*******************************************************************************/
void beep_long(uint8_t n_time, uint16_t time)
{
	while (n_time > 0)
	{
		beep(time);
		delay_ms(40);
		n_time--;
	}		
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
