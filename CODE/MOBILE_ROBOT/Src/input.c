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
#include "input.h"

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
void Input_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	/* GPIOx clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    /* Disable the Serial Wire Jtag Debug Port SWJ-DP */
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    
    // Input pin
    GPIO_InitStructure.GPIO_Pin = BUT0_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUT0_Port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = BUT1_Pin;
    GPIO_Init(BUT1_Port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = BUT2_Pin;
    GPIO_Init(BUT2_Port, &GPIO_InitStructure);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
