/**
  ******************************************************************************
  * @file    control.h 
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__CONTROL_H
#define __CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Private define ------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern uint8_t PID_flag;
extern int16_t v_trackline;

/* Private function prototypes -----------------------------------------------*/
void Robot_Control_Via_UART(void);
float Calc_E(void);
void TrackLinePID(uint8_t enable, int16_t err_set, int16_t speed);

#endif

