/**
  ******************************************************************************
  * @file    motor.c 
  * @author
  * @version
  * @date
  * @brief	Control PWM
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "motor.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t PrescalerValue;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
Function name: Motor_Init
Decription:
*******************************************************************************/
void Motor_Init(void)
{
	/* GPIOx clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Disable the Serial Wire Jtag Debug Port SWJ-DP */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);
	
	/* GPIO Configuration */
	// DIR Pins
	GPIO_InitStructure.GPIO_Pin = Motor1_Dir_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Motor1_Dir_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Motor2_Dir_Pin;
	GPIO_Init(Motor2_Dir_Port, &GPIO_InitStructure);
	
	// PWM Pins
	GPIO_InitStructure.GPIO_Pin = Motor1_PWM_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Motor1_PWM_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Config TIM */
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ((SystemCoreClock) / CLK_PRE) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = DUTY_MAX - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/* Config PWM Mode - TIM3 Channel 1, 3 */
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
    
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

/*******************************************************************************
Function name: Motor1_Speed
Decription:
*******************************************************************************/
void Motor1_Speed(int16_t speed)
{
	if (speed >= 0)
	{
		Motor1_Dir_Port->BSRR = Motor1_Dir_Pin;
		TIM3->CCR3 = speed;
	}
	else
	{
		speed = -speed;
		Motor1_Dir_Port->BRR = Motor1_Dir_Pin;
		TIM3->CCR3 = speed;
	}
}
		
/*******************************************************************************
Function name: Motor2_Speed
Decription:
*******************************************************************************/
void Motor2_Speed(int16_t speed)
{
	if (speed >= 0)
	{
		Motor2_Dir_Port->BSRR = Motor2_Dir_Pin;
		TIM2->CCR3 = speed;
	}
	else
	{
		speed = -speed;
		Motor2_Dir_Port->BRR = Motor2_Dir_Pin;
		TIM2->CCR3 = speed;
	}
}

/*******************************************************************************
Function name: Motor_Init
Decription:
*******************************************************************************/
void Motor_Speed(int16_t l_speed, int16_t r_speed)
{
	Motor1_Speed(r_speed);
	Motor2_Speed(l_speed);
}

/*******************************************************************************
Function name: Motor_Init
Decription:
*******************************************************************************/
void Motor_Stop(void)
{
	Motor_Speed(0, 0);
}
