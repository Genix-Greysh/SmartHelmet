/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "MPU6050.h"
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



/*********************************************************************************************************//**
* @brief  Configures GPTM0 for time estimate.
* @retval None
***********************************************************************************************************/





/**
 * @brief Ö÷º¯Êý
 */
int main(void)
{
	extern vu32 gwTimeDisplay;
	
	/* Initialize devices */
	SYSTICK_Config();
	Init_USART(HT_USART0,115200);		
	Init_USART(HT_USART1,115200);		

	PDMA_Configuration();
	SD_SPI_Init();
	TryInitSD();
	
	sdfs_app_test();
	
	/* main loop */           	
	while (1)
	{
		if(PDMA_GetFlagStatus(PDMA_CH2, PDMA_FLAG_TC) == SET)
		{
			Axis_DataTransfrom();
			PDMA_ClearFlag(PDMA_CH2, PDMA_INT_TC);
			if( X_Axis*X_Axis+Y_Axis*Y_Axis+Z_Axis*Z_Axis >150)
			{
				USART_SendData(HT_USART1,0x55);
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



