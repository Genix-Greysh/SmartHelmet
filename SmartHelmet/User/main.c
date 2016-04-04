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
#include "ov7725.h"
#include "pwcru.h"

/**
 * @brief Ö÷º¯Êý
 */
int main(void)
{	
	int sysWorking = FALSE;
	
	/* Initialize devices */
	SYSTICK_Config();
	Init_USART(HT_USART0,115200);		
	Init_USART(HT_USART1,115200);		
	PDMA_Configuration();
	pwrcu_init();
	
	/* Init SD and mount it */
	SD_SPI_Init();
	SD_TryInit();
	sdfs_app_mnt();
	
	/* Init Camera */
	Ov7725_GPIO_Config();
	Ov7725_TryInit();
	Ov7725_VSYNC_Init();		
	
	/* main loop */           	
	while (1)
	{	
		sdfs_app_savePhoto();
		delay_ms(100);
//		/* main loop */ 
//		while(1)
//		{	
//			Enter_DeepSleepMode();
//		
//			while (sysWorking)
//			{
//				if(PDMA_GetFlagStatus(PDMA_CH2, PDMA_FLAG_TC) == SET)
//				{
//					Axis_DataTransfrom();
//					PDMA_ClearFlag(PDMA_CH2, PDMA_INT_TC);
//					if( X_Axis*X_Axis+Y_Axis*Y_Axis+Z_Axis*Z_Axis >150)
//					{
//						USART_SendData(HT_USART1,0x55);
//					}
//				}
//		
//				sysWorking = FALSE;
//			}
//			flag = TRUE;
//			delay_ms(500);
//		}
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



