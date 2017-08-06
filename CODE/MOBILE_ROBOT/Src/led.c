/**
  ******************************************************************************
  * @file    led.c 
  * @author  Pham Son Loc - Robot Club BK HCM
  * @version
  * @date
  * @brief	 Display led 7-segments module
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "led.h"

#include "systick.h"

/* Private variables ---------------------------------------------------------*/
int16_t num, num_old;
int8_t unit, ten, hundred, thousand;

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
Function name: Led7_Init
Decription: Initialize Led 7-segments Module
Input: None
Output: None
*******************************************************************************/
void Led7_Init(void)
{
    /* GPIO Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure CLK_Pin in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin =  Led7_CLK_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    if (Led7_CLK_Port == GPIOC)
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    else
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(Led7_CLK_Port, &GPIO_InitStructure);
    
    /* Configure DAT_Pin in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin =  Led7_DAT_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    if (Led7_DAT_Port == GPIOC)
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    else
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(Led7_DAT_Port, &GPIO_InitStructure);
}

/*******************************************************************************
Function name: clock7
Decription: Clock for shift bit using 74HC164
Input: None
Output: None
*******************************************************************************/
void clock7(void)
{
	Led7_CLK_Port->BSRR = Led7_CLK_Pin;
	Led7_CLK_Port->BRR  = Led7_CLK_Pin;
}

/*******************************************************************************
Function name: outLed7
Decription: Out data using 74HC164
Input: 8 bits data
Output: None
*******************************************************************************/
void outLed7(uint8_t data)
{
    uint8_t i, temp, mask = 0x80;
    for (i=0; i < 8; i++)
    {
        temp = data & mask;
        if (temp == 0)
            Led7_DAT_Port->BSRR = Led7_DAT_Pin;
        else
            Led7_DAT_Port->BRR = Led7_DAT_Pin;
        clock7();
        
        mask = mask >> 1;
    }
}

/*******************************************************************************
Function name: calcNum
Decription: Calculate unit, ten, hundred, thousand
Input: None
Output: None
*******************************************************************************/
void calcNum(void)
{
	if (num == num_old)
		return;
	else
	{
		num_old = num;
		
		if (num < 0)
			num = 10000 + num;
		
		// Calculate num
		unit 		= (uint16_t)(num%10);
		ten 		= (uint16_t)(num%100 / 10);
		hundred		= (uint16_t)(num%1000 / 100);
		thousand	=	(uint16_t)(num / 1000); 
	}		
}

/*******************************************************************************
Function name: updateNum
Decription: Update num and scan led
Input: None
Output: None
*******************************************************************************/
void updateNum(void)
{
    static uint8_t led_cnt = 0;
	static uint8_t led_num = 0;
    
    led_cnt++;
    
    if (led_cnt == 4)
    {
        led_cnt = 0;
        
        led_num++;
        calcNum();
        
        switch(led_num)
        {
            case 1:  // scan first led, first led displays ONE unit
                outLed7(0x70 + (int8_t)unit);
                break;
                            
            case 2:  // scan second led, this led display TEN unit
                if ((thousand == 0) && (hundred == 0) && (ten == 0))
                    outLed7(0xFF);
                else
                    outLed7(0xB0 + (int8_t)ten);
                break;
                            
            case 3:  // scan third led, this led display HUNDRED unit
                if ((thousand == 0) && (hundred == 0))
                    outLed7(0xFF);
                else
                    outLed7(0xD0 + (int8_t)hundred);
                break;
                            
            case 4:  // scan fourth led, this led display THOUSAND unit
                if (thousand == 0)
                    outLed7(0xF0);
                else
                    outLed7(0xE0 + (int8_t)thousand);
                led_num = 0;
                    break;
                            
            default: 
                led_num = 0; break;
        }
    }
}

void blinky(uint8_t time)
{
    while (time--)
    {
        outLed7(0xFF);
        delay_ms(200);
        outLed7(0x00);
        delay_ms(200);
    }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
