/**
  ******************************************************************************
  * @file    motor.h 
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__MOTOR_H
#define __MOTOR_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Private define ------------------------------------------------------------*/
// PWM Constants
#define	CLK_PRE			5000000
#define	DUTY_MAX		10000	// Lay CLOCK sau khi chia tan / DUTY_MAX => PWM Freq
								// => PWM Frequency 500 Hz							
#define	DUTY_MIN		0

// Motor 1 - Motor Left
#define	Motor1_Dir_Pin	GPIO_Pin_1
#define	Motor1_Dir_Port	GPIOB
#define	Motor1_PWM_Pin	GPIO_Pin_0
#define	Motor1_PWM_Port	GPIOB

// Motor 2 - Motor Right
#define	Motor2_Dir_Pin	GPIO_Pin_11
#define	Motor2_Dir_Port	GPIOB
#define	Motor2_PWM_Pin	GPIO_Pin_10
#define	Motor2_PWM_Port	GPIOB


/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Motor_Init(void);
void Motor1_Speed(int16_t speed);
void Motor2_Speed(int16_t speed);
void Motor_Speed(int16_t l_speed, int16_t r_speed);
void Motor_Stop(void);

#endif
