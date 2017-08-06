/**
  ******************************************************************************
  * @file    flash.c 
  * @author  Nguyen Van Hoa Vu - Pham Son Loc - Robot Club BK HCM
  * @version
  * @date
  * @brief	Main flash body
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t flash_data[];
uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t NbrOfPage = 0x00;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
volatile TestStatus MemoryProgramStatus = PASSED;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
Function name: Flash_Init
Decription:
*******************************************************************************/
void Flash_Init(void)
{
    /* Porgram FLASH Bank1 ********************************************************/       
    /* Unlock the Flash Bank1 Program Erase controller */
    FLASH_UnlockBank1();

    /* Define the number of page to be erased */
    NbrOfPage = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR) / FLASH_PAGE_SIZE;
}

/*******************************************************************************
Function name: Flash_Write
Decription:
*******************************************************************************/
void Flash_Write(void)
{
    uint16_t i = 0;
    
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

    /* Erase the FLASH pages */
    for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
        FLASHStatus = FLASH_ErasePage(BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
    }
    Address = BANK1_WRITE_START_ADDR;

    while((Address < BANK1_WRITE_END_ADDR) && (FLASHStatus == FLASH_COMPLETE))
    {
        FLASHStatus = FLASH_ProgramWord(Address, flash_data[i]);
        Address = Address + 4;
        i++;
        if (i == 100) break;
    } 
}

/*******************************************************************************
Function name: Flash_Read
Decription:
*******************************************************************************/
void Flash_Read(void)
{
	uint16_t i = 0;
    
    /* Check the correctness of written data */
    Address = BANK1_WRITE_START_ADDR;

    while((Address < BANK1_WRITE_END_ADDR) && (MemoryProgramStatus != FAILED))
    {	  
        flash_data[i]=(*(__IO uint32_t*) Address);
        Address += 4;
        i++;
        if (i == 100) break;
    }
}

/*******************************************************************************
Function name: Flash_Del_Data
Decription:
*******************************************************************************/
void Flash_Del_Data(void)
{
    uint16_t i = 0;
	for (i=0; i<100; i++) flash_data[i]=0;
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
