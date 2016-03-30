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

/**
 * @brief 主函数
 */
int main(void)
{	
	/* ov7725 场中断变量 */
	extern volatile uint8_t Ov7725_vsync ;

	/* Initialize devices */
	SYSTICK_Config();
	Init_USART(HT_USART0,115200);		
	Init_USART(HT_USART1,115200);		
	PDMA_Configuration();
	SD_SPI_Init();
	TryInitSD();
	Ov7725_GPIO_Config();

	/* ov7725 寄存器配置初始化 */
	while(Ov7725_Init() != SUCCESS)
	{
		printf("Init ov7725 error.\r\n");
		delay_ms(500);
	}
	printf("Init ov7725 success.\r\n");
	
	/* ov7725 场信号线初始化 */
	VSYNC_Init();	
	Ov7725_vsync = 0;
	
	/* main loop */           	
	while (1)
	{
//		if(PDMA_GetFlagStatus(PDMA_CH2, PDMA_FLAG_TC) == SET)
//		{
//			Axis_DataTransfrom();
//			PDMA_ClearFlag(PDMA_CH2, PDMA_INT_TC);
//			if( X_Axis*X_Axis+Y_Axis*Y_Axis+Z_Axis*Z_Axis >150)
//			{
//				USART_SendData(HT_USART1,0x55);
//			}
//		}
		
//		SCL_OUT_MOTE;
//		SCL_IN_MOTE;
//		SCL_H;
//		
//		delay_ms(500);
//		printf("SCL input value : %d\r\n", SCL_read);
//		printf("SCL output value : %d\r\n", GPIO_ReadOutBit(HT_GPIOD, GPIO_PIN_4));		
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



