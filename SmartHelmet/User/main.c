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
 * @brief ������
 */
/**Tested Fine**/
 
int main(void)
{	
	
	bool sysWorking = TRUE;
	
	/* Initialize devices */
	SYSTICK_Config();
	Init_USART(HT_USART0,115200);		
	Init_USART(HT_USART1,115200);		
<<<<<<< HEAD
	PDMA_Configuration();	//����PDMA
	
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
	
=======
	PDMA_Configuration();

	
	
	/* Init SD and mount it */
	SD_SPI_Init();
	SD_TryInit();
	sdfs_app_mnt();
	
	/* Init Camera */
	Ov7725_GPIO_Config();
	Ov7725_TryInit();
	Ov7725_VSYNC_Init();		
	
	pwrcu_init();
	
>>>>>>> ae162a8109fce85f3d620dfea9f5e6fe9731e32d
	/* main loop */           	
	while (1)
	{	
		printf("Enter Sleep Mode...\n");
<<<<<<< HEAD
		Enter_DeepSleepMode();	//����˯��ģʽ
		PDMA_Configuration();	//����PDMA
		sysWorking = TRUE; 
=======
		Enter_DeepSleepMode();
		sysWorking = TRUE;
>>>>>>> ae162a8109fce85f3d620dfea9f5e6fe9731e32d
		
		printf("Enter Main_loop...\n");
		/* main loop */ 
		while(sysWorking)	/* ϵͳ��ʼ���� */
		{	
			if(PDMA_GetFlagStatus(PDMA_CH7, PDMA_FLAG_TC) == SET)
			{
<<<<<<< HEAD
				if(IS_MPU_RUNNING)
				{
					if(TRUE == IsAccident())
					{
						printf("Accident!%f %f %f %f\n", Axis[0], Axis[1], Axis[2], Square(Axis[0]) + Square(Axis[1]) + Square(Axis[2]));
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, SET);
						delay_ms(1000);
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);
					}
					else
						printf("Not!%f %f %f %f\n", Axis[0], Axis[1], Axis[2], Square(Axis[0]) + Square(Axis[1]) + Square(Axis[2]));
				}
				else 
					printf("%d", MPU_Data[0]);
				PDMA_ClearFlag(PDMA_CH7, PDMA_INT_TC);
			}
	
			//sdfs_app_savePhoto();
=======
				if(MPU_Data[0] == 0x55)
				{
					Axis_GetFinalData();	//������յļ��ٶ�
					if(Square(Axis[0]) + Square(Axis[1]) + Square(Axis[2])> 1.0)
					{
						printf("MPU6050 Test OK!\r\n");
					}
				}			
				PDMA_ClearFlag(PDMA_CH7, PDMA_INT_TC);
			}
	
			sdfs_app_savePhoto();
>>>>>>> ae162a8109fce85f3d620dfea9f5e6fe9731e32d
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
