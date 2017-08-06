/**
  ******************************************************************************
  * @file    main.c
  * @author
  * @version
  * @date
  * @brief	Main program mobile robot
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "motor.h"
#include "systick.h"
#include "uart.h"
#include "control.h"
#include "buzzer.h"
#include "led.h"
#include "input.h"
#include "adc.h"
#include "flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void init(void);

/* Private functions ---------------------------------------------------------*/
void init(void)
{
    SysTick_Init(1000);
    Motor_Init();
    Motor_Stop();
    UART1_Init(9600);
	Buzzer_Init();
    Led7_Init();
    Input_Init();
    Flash_Init();
    Get_ADC_Parameter_Flash();
    Sensor_Init();
}

/*******************************************************************************
Function name: Main program
Decription: None
Input: None
Output: None
*******************************************************************************/
int main(void)
{
    // Initialize system
    init();
    delay_ms(1000);
    
    while (1)
    {
        if (BLACK)
        {
            display_flag = 0;
            outLed7(0x01);
            delay_ms(200);
            
            Get_High_Parameter();
            
            blinky(3);
            
            display_flag = 1;
        }
        if (WHITE)
        {
            display_flag = 0;
            outLed7(0x02);
            delay_ms(200);
            
            Get_Low_Parameter();
            
            blinky(3);
            
            display_flag = 1;
        }
        
        Robot_Control_Via_UART();
    }
}

/*******************************************************************************
Function name: USE_FULL_ASSERT
Decription: None
Input: None
Output: None
*******************************************************************************/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif
