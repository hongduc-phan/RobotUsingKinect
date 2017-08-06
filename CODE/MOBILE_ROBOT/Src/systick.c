/**
  ******************************************************************************
  * @file    systick.c 
  * @author  Robot Club BK HCM
  * @version
  * @date
  * @brief	Main systick body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "systick.h"

#include "led.h"
#include "control.h"
#include "adc.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t cnt0 = 0, cnt1 = 0;
uint16_t led_cnt = 0;
uint8_t display_flag = 1;

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
Function name: SysTick_Handler
Decription: This function handles SysTick Handler.
Input: None
Output: None
*******************************************************************************/
void SysTick_Handler(void)
{
	cnt0++;
	cnt1++;
	led_cnt++;
    
	// Read sensor and out led
    if (display_flag)
    {
        outLed7(sensor_disp);
        Read_Sensor();
    }
    
    // Trackline PID
    TrackLinePID(PID_flag, 0, v_trackline);
}

/*******************************************************************************
Function name: SysTick_Init
Decription: This function initialize SysTick Timebase.
Input: None
Output: None
*******************************************************************************/
void SysTick_Init(uint32_t time)
{
if (SysTick_Config(SystemCoreClock / time))
  { 
    /* Capture error */ 
    while (1);
  }
}

/*******************************************************************************
Function name: delay_ms
Decription: Delay ms
Input: None
Output: None
*******************************************************************************/
void delay_ms(uint16_t time)
{
	cnt1 = 0;
	while (cnt1 < time);
}

/*******************************************************************************
Function name: delay_us
Decription: Delay us
Input: None
Output: None
*******************************************************************************/
void delay_us(uint32_t time)
{
	time *= 14;	// 72 Mhz/5 = 14; 5 for while loop
	while (time--);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/

