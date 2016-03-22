/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"

#include "usart.h"
#include "spi.h"
#include "sd.h"
#include "ff.h"
#include "delay.h"
#include "sdfs_app.h"
#include "rtc.h"


/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup GPIO_Examples GPIO
  * @{
  */

/** @addtogroup InputOutput
  * @{
  */


/* Global functions ----------------------------------------------------------------------------------------*/

u8 MPU_Data [33] ;

char standard = 28;

/*********************************************************************************************************//**
* @brief  Configures GPTM0 for time estimate.
* @retval None
***********************************************************************************************************/
void PDMA_Configuration(void)
{
	PDMACH_InitTypeDef PDMACH_InitStructure;	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
	//Config Clock
	CKCUClock.Bit.PDMA       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	//Config USART
	USART_RxPDMACmd(HT_USART0, ENABLE);

	//PDMA

	PDMACH_InitStructure.PDMACH_SrcAddr   = (u32) HT_USART0_BASE;
	PDMACH_InitStructure.PDMACH_DstAddr   = (u32) &MPU_Data;

	PDMACH_InitStructure.PDMACH_AdrMod    = SRC_ADR_FIX | DST_ADR_LIN_INC|AUTO_RELOAD;
	PDMACH_InitStructure.PDMACH_Priority  = H_PRIO;
	PDMACH_InitStructure.PDMACH_BlkCnt    = 33;
	PDMACH_InitStructure.PDMACH_BlkLen    = 1;
	PDMACH_InitStructure.PDMACH_DataSize  = WIDTH_8BIT;

	PDMA_Config(PDMA_CH2, &PDMACH_InitStructure);  
	PDMA_IntConig(PDMA_CH2, (PDMA_INT_GE | PDMA_INT_TC), ENABLE);
	PDMA_EnaCmd(PDMA_CH2, ENABLE);
	PDMA_SwTrigCmd(PDMA_CH2, ENABLE);
}

int Data_Check(u8 data, u8 standard)
{
	if(data >standard && data <=(0xff-standard))
		return 0;
	else 
		return 1;
}







/**
 * @brief Ö÷º¯Êı
 */
int main(void)
{
	extern vu32 gwTimeDisplay;
	
	/* Initialize devices */
	SYSTICK_Config();
	Init_USART(HT_USART0,115200);		
	PDMA_Configuration();
	SD_SPI_Init();
	TryInitSD();
	
	sdfs_app_test();
	RTC_ConfigOnline();
	
	/* main loop */           	
	while (1)
	{
		int i = 0;
		if(PDMA_GetFlagStatus(PDMA_CH2, PDMA_FLAG_TC) == SET)
		{
			PDMA_ClearFlag(PDMA_CH2, PDMA_INT_TC);
			for(i = 0 ; i <3 ; i++)
			{			
				if(MPU_Data[i*11+1] == 0x51 && (Data_Check(MPU_Data[i*11+3],standard) == 0 || 
						Data_Check(MPU_Data[i*11+5],standard) == 0 || Data_Check(MPU_Data[i*11+7],standard) == 0))
				{					
					USART_SendData(HT_USART1,0x55);
				}
			}
		}
		
		if (gwTimeDisplay == 1)
		{
			/* Display current time.
				Current time is sum of the RTC counter value and the init time(stored in PWRCU_BAKREG_1 register).
				The init time (PWRCU_BAKREG_1 register) will be clear if the RTC Match flag(CMFLAG) is set.
				Refer to RTC_IRQHandler. */
			Time_Display(RTC_GetCounter() + PWRCU_ReadBackupRegister(PWRCU_BAKREG_1));
			gwTimeDisplay = 0;
		}
	}
}



#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
  */

  while (1)
  {
  }
}
#endif


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
