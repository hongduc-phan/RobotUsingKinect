/**
  ******************************************************************************
  * @file    uart.c 
  * @author  Robot Club BK HCM
  * @version
  * @date
  * @brief	Module UART
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "uart.h"

#include "led.h"
#include "systick.h"

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#if RX_BUFF_LEN < 255
__IO uint8_t RxCounter = 0, RxLength = 0;
#else
__IO uint16_t RxCounter = 0; RxLength = 0;
#endif

#if TX_BUFF_LEN < 255
__IO uint8_t TxCounter = 0 , TxLength = 0;
#else
__IO uint16_t TxCounter = 0; TxLength = 0;
#endif

uint8_t TxBuffer[TX_BUFF_LEN];
uint8_t RxBuffer[RX_BUFF_LEN];

uint16_t PackedLength = 0;
uint8_t RxPackedIncomeFlag;
uint8_t TxSendingFlag;

uint8_t data_uart;


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
Function name: USART1_IRQHandler
Decription: This function handles USARTy global interrupt request
Input: None
Output: None
*******************************************************************************/
void USART1_IRQHandler(void)
{
	uint8_t data;
	
	/* USART Receive Interrupt */
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		// Read one byte from the receive data register
		data = USART_ReceiveData(USART1);
		
		if (RxCounter > RX_BUFF_LEN)	RxCounter = 0;
		
		RxBuffer[RxCounter] = data;
		RxCounter++;
		
		if (RxCounter ==  2)
		{
			if (RxBuffer[0] == STX)
			{
				PackedLength = RxBuffer[1];
			}
			else
			{
				RxCounter = 0;
			}
		}
		
		if (RxCounter == PackedLength)
		{
			if (data == ETX)
			{
				RxPackedIncomeFlag = 1;
				RxCounter = 0;
			}
		}
	}	
}

/*******************************************************************************
Function name: UART1_Init
Decription:	Initialize UART1
Input: uint32_t baud_rate
Output: None
*******************************************************************************/
void UART1_Init(uint32_t baud_rate)
{
	/* GPIO Configure */
	/* Enable GPIOA and AFIO clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	/* Enable UART clock, if using USART2 or USART3 ... we must use RCC_APB1PeriphClockCmd(RCC_APB1Periph_USARTx, ENABLE) */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Configure USART1 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART1 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  	*/
	USART_InitStructure.USART_BaudRate = baud_rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	/* USART configuration */
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable the USART1 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable USART interupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
}

/*******************************************************************************
Function name: UART_Send_Frame
Decription:	
Input: None
Output: None
*******************************************************************************/
void UART_Send_Frame(uint8_t data_length)
{
    uint8_t i;
    
    // Khoi tao frame gui
    TxBuffer[0] = STX;
    TxBuffer[1] = data_length;
    TxBuffer[data_length+2] = ETX;
    
    // Bat dau gui frame
    TxSendingFlag = 1;
    
    for (i=0; i<(data_length+3); i++)
    {
        USART_SendData(UART_Communication, TxBuffer[i]);
        
        /* Loop until the end of transmission */
        while (USART_GetFlagStatus(UART_Communication, USART_FLAG_TC) == RESET);
    }
    
    // Ket thuc gui frame
    TxSendingFlag = 0;
}

/*******************************************************************************
Function name: PUTCHAR_PROTOTYPE
Decription:	Retargets the C library printf function to the USART.
Input: None
Output: None
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(UART_Communication, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(UART_Communication, USART_FLAG_TC) == RESET)
    {}

    return ch;
}
