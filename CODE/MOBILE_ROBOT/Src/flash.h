/**
  ******************************************************************************
  * @file    flash.h 
  * @author  Nguyen Van Hoa Vu - Robot Club BK HCM
  * @version
  * @date
  * @brief
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
// FLASH_PAGE_SIZE  2 Kbytes
#define FLASH_PAGE_SIZE    ((uint16_t)0x800)

// Range 2 Kbytes
#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08008000)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x08008800)

#define Vset_PC_Addr 	20
#define Kp_Vel_Addr		21
#define	Ki_Vel_Addr		23
#define	Kd_Vel_Addr		25

/* Extern variables ----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

extern  uint16_t flash_data[100];		// Data array

/* Private function prototypes -----------------------------------------------*/
void Flash_Init(void);
void Flash_Write(void);
void Flash_Read(void);
void Flash_Del_Data(void);

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
