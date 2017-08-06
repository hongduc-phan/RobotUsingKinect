/**
  ******************************************************************************
  * @file    template.c 
  * @author  Robot Club BK HCM
  * @version
  * @date
  * @brief	Main template body
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

#include "flash.h"
#include "input.h"
#include "systick.h"
#include "led.h"
#include "buzzer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t sensor, sensor_disp;
uint8_t sensor_temp[3];
uint16_t ADC_Val[8];
uint16_t ADC_High_Val[8];
uint16_t ADC_Low_Val[8];
uint16_t ADC_Sample_Val[8];
uint16_t ADC_Distance[8];
uint16_t ADC_Temp0[8], ADC_Temp1[8], ADC_Temp2[8];

uint8_t l4, l3, l2, l1, r1, r2, r3, r4;
uint8_t sen_sum, sen_left, sen_right;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
Function name: delay_adc(uint16_t)
Decription: delay nano seconds
*******************************************************************************/
void delay_adc(uint16_t num)
{
	__IO uint32_t index = 0;
	for(index = (18 * num); index != 0; index--)
	{
	}
}

/*******************************************************************************
Function name: ADC_Init
Decription:
*******************************************************************************/
void Sensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	
    /* GPIO and ADC clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    /* Configure GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* DMA1 channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Val;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 8;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);	

    /* Init ADC1 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 8;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,5,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,6,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,7,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,8,ADC_SampleTime_1Cycles5);

    /* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
    
    /* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
    
    /* Enable ADC1 reset calibration register */   
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));
     
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*******************************************************************************
Function name: Read_Sensor
Decription:
*******************************************************************************/
void Read_Sensor(void)
{
    uint8_t i, mask = 0x01, mask1 = 0x80;
	uint8_t temp = 0;
    sensor_disp = 0;
    
	for (i=0; i<8; i++)
	{
		if (ADC_Val[i] < ADC_Sample_Val[i])
		{
			temp |= mask;
            sensor_disp |= mask1;
		}
		
		mask = mask << 1;
        mask1 >>= 1;
	}
	// Backup old value
	sensor_temp[2] = sensor_temp[1];
	sensor_temp[1] = sensor_temp[0];
	// Get new value
	sensor_temp[0] = temp;
    
    // Lay gia tri moi nhat
    if ((sensor_temp[2] == sensor_temp[1]) && (sensor_temp[1] == sensor_temp[0]))
        sensor = sensor_temp[0];
	
	// Tach gia tri cam bien
	mask = 0x01;
	l4 = l3 = l2 = l1 = 0;
	r4 = r3 = r2 = r1 = 0;
	
	if (sensor & mask)
		l4  = 1;
	
	mask = mask << 1;
	if (sensor & mask)
		l3 = 1;
	
	mask = mask << 1;
	if (sensor & mask)
		l2 = 1;
	
	mask = mask << 1;
	if (sensor & mask)
		l1 = 1;
	
	mask = mask << 1;
	if (sensor & mask)
		r1 = 1;
	
	mask = mask << 1;
	if (sensor & mask)
		r2 = 1;
	
	mask = mask << 1;
	if (sensor & mask)
		r3 = 1;
	
	mask = mask << 1;
	if (sensor & mask)
		r4 = 1;
	
	sen_left = l4 + l3 + l2 + l1;
	sen_right = r4 + r3 + r2 + r1;
	sen_sum = l4 + l3 + l2 + l1 + r1 + r2 + r3 + r4;
}

/*******************************************************************************
Function name: Get_ADC_Parameter_Flash
Decription:
*******************************************************************************/
void Get_ADC_Parameter_Flash(void)
{
    uint8_t i;
    
    delay_ms(50);
    
    // Doc FLASH
    Flash_Read();
    
    // Lay gia tri ADC tu Flash
    for (i=0; i<8; i++)
    {
        ADC_High_Val[i] = flash_data[i];
        ADC_Low_Val[i] = flash_data[8 + i];
    }
    
    delay_ms(50);
    
    // Calculate ADC Sample
    Calc_ADC_Sample();
}

/*******************************************************************************
Function name: Get_ADC_Parameter
Decription:
*******************************************************************************/
void Calc_ADC_Sample(void)
{
    uint8_t i;
    
    for (i=0; i<8; i++)
    {
        ADC_Distance[i] = ADC_High_Val[i] - ADC_Low_Val[i];
    }
    
    for (i=0; i<8; i++)
    {
        ADC_Sample_Val[i] = ADC_Low_Val[i] + ADC_Distance[i]*4 / 10;
    }
}

/*******************************************************************************
Function name: Push_ADC_Parameter_Flash
Decription:
*******************************************************************************/
void Push_ADC_Parameter_Flash(void)
{
    uint8_t i;
    
    for (i=0; i<8; i++)
    {
        flash_data[i] = ADC_High_Val[i];
        flash_data[8+i] = ADC_Low_Val[i];
    }

    // Write FLASH
    Flash_Write();
}

void Get_High_Parameter(void)
{
	uint8_t i;
	
	delay_ms(100);
	
	// Lay lan 1
	for (i=0; i<8; i++)
	{
		ADC_Temp0[i] = ADC_Val[i];
	}
	
	// Lay lan 2
	for (i=0; i<8; i++)
	{
		ADC_Temp1[i] = ADC_Val[i];
	}
	
	// Lay lan 3
	for (i=0; i<8; i++)
	{
		ADC_Temp2[i] = ADC_Val[i];
	}
	
	// Lay trung binh
	for (i=0; i<8; i++)
	{
		ADC_High_Val[i] = (ADC_Temp0[i] + ADC_Temp1[i] + ADC_Temp2[i]) / 3;
	}
	
	// Tinh lai mau
	Calc_ADC_Sample();
	
	// Luu vao flash
	Push_ADC_Parameter_Flash();
	
	//beep_long(2, 80);
}

void Get_Low_Parameter(void)
{
	uint8_t i;
	
	delay_ms(100);
	
	// Lay lan 1
	for (i=0; i<8; i++)
	{
		ADC_Temp0[i] = ADC_Val[i];
	}
	
	// Lay lan 2
	for (i=0; i<8; i++)
	{
		ADC_Temp1[i] = ADC_Val[i];
	}
	
	// Lay lan 3
	for (i=0; i<8; i++)
	{
		ADC_Temp2[i] = ADC_Val[i];
	}
	
	// Lay trung binh
	for (i=0; i<8; i++)
	{
		ADC_Low_Val[i] = (ADC_Temp0[i] + ADC_Temp1[i] + ADC_Temp2[i]) / 3;
	}
	
	// Tinh lai mau
	Calc_ADC_Sample();
	
	// Luu vao flash
	Push_ADC_Parameter_Flash();
	
	//beep_long(2, 80);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
