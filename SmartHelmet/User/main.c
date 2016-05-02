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
#include "Bluetooth.h"

/**
 * @brief 主函数
 */
/**Tested Fine**/
 
int main(void)
{	
	bool sysWorking = TRUE;
	int count;
	
	/* Initialize devices */
	SYSTICK_Config();
	Init_USART(HT_USART0, 9600);		
	Init_USART(HT_USART1, 115200);		

	PDMA_Configuration();	//配置PDMA
	
	/* Init SD and mount it */
	SD_SPI_Init();
	SD_TryInit();
	sdfs_app_mnt();
	
	/* Init Camera */
	Ov7725_GPIO_Config();
	Ov7725_TryInit();
	Ov7725_VSYNC_Init();
	
	GPIO_DirectionConfig(HT_GPIOC , GPIO_PIN_9, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);
	
	pwrcu_init();
	

	/* main loop */           	
	while (1)
	{	
//		printf("Enter Sleep Mode...\n");
		Enter_DeepSleepMode();	//进入睡眠模式
		PDMA_Configuration();	//配置PDMA
		sysWorking = TRUE; 

		
//		printf("Enter Main_loop...\n");
		/* main loop */ 
		while(sysWorking)	/* 系统开始工作 */
		{	
			if(PDMA_GetFlagStatus(PDMA_CH7, PDMA_FLAG_TC) == SET)
			{
				if(IS_MPU_RUNNING)
				{
					if(TRUE == IsAccident())
					{
//						printf("Accident!%f %f %f %f\n", Axis[0], Axis[1], Axis[2], Square(Axis[0]) + Square(Axis[1]) + Square(Axis[2]));
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, SET);
						CrashFunction();
						for(count = 0; count < 2; count++)
						{
							sdfs_app_savePhoto();
							delay_ms(50);
						}
						
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);
					}
					else
//						printf("Not!%f %f %f %f\n", Axis[0], Axis[1], Axis[2], Square(Axis[0]) + Square(Axis[1]) + Square(Axis[2]));
						;
				}
				else
//					printf("%d", MPU_Data[0]);
					;
				PDMA_ClearFlag(PDMA_CH7, PDMA_INT_TC);
			}
			
			delay_ms(100);
			if ( GPIO_ReadInBit(HT_GPIOE, GPIO_PIN_1) == 1)   
					sysWorking = FALSE;
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
