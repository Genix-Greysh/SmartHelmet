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
#include "pwcru.h"

//全局变量 
extern bool flag;  			//用于检测是否跳出第二级while循环


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
 * @brief 主函数
 */

int main(void)
{
	extern vu32 gwTimeDisplay;
	flag = FALSE;
	
	/* Initialize devices */
	SYSTICK_Config();
	Init_USART(HT_USART0,115200);		
	Init_USART(HT_USART1,115200);		

	PDMA_Configuration();
	SD_SPI_Init();
	TryInitSD();
	
	sdfs_app_test();
<<<<<<< HEAD
	pwrcu_init();
	
	/* main loop */ 
	while(1)
	{	
	  Enter_DeepSleepMode();
=======
>>>>>>> ff37b79040d625bdd43c4d1c67f2925221fee8bf
	
		while (flag == TRUE)
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
	
			}
			flag = FALSE;
	
			delay_ms(500);
			flag = TRUE;
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



