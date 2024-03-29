/**
  ******************************************************************************
  * @file    uart.h 
  * @author  Robot Club BK HCM
  * @version
  * @date
  * @brief
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__UART_H
#define __UART_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

#include <stdio.h>

/* Private define ------------------------------------------------------------*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

// UART1
#define     USART1_Pin_Tx   GPIO_Pin_9
#define     USART1_Pin_Rx   GPIO_Pin_10
#define     USART1_PORT     GPIOA

// UART2
#define     USART2_Pin_Tx   GPIO_Pin_0
#define     USART2_Pin_Rx   GPIO_Pin_0
#define     USART2_PORT     GPIOA

// UART3
#define     USART3_Pin_Tx   GPIO_Pin_0
#define     USART3_Pin_Rx   GPIO_Pin_0
#define     USART3_PORT     GPIOA

// Define frame
#define 	STX		0x02
#define 	ETX		0x03

#define 	RX_BUFF_LEN		250
#define		TX_BUFF_LEN		250

#define     UART_Communication  USART1

/* Extern variables ----------------------------------------------------------*/
extern uint8_t TxBuffer[TX_BUFF_LEN];
extern uint8_t RxBuffer[RX_BUFF_LEN];

extern uint8_t RxPackedIncomeFlag;
extern uint8_t TxSendingFlag;

extern uint8_t data_uart;

/* Private function prototypes -----------------------------------------------*/
void UART1_Init(uint32_t baud_rate);
void UART_Send_Frame(uint8_t data_length);

/* Interrupt Service Routine */
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

#endif
