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

#define NUM_PHOTO 10
char num[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};//数字以字符显示

/**
 * @brief  		打包加速度数据以发送给手机端显示
 * @param  		char *data:存储待发送的数据
 * @retval 		void
 */
void PackDatasForBlueTooth(char *data)
{
	int i, j;
	
	for(i = 0; i < 3; ++i)
		*data++ = 'b';//前三个字符为'b',且data依次向前移动一位
	
	for(i = 0; i < 3; ++i)//xyz三次循环
	{
		if(Axis[i] < 0)
		{
			Axis[i] = -Axis[i];
			*(data + i * 6) = num[1];//负数符号位为1
		}
		else
			*(data + i * 6) = num[0];
		
		Axis[i] *= 1000;
		for(j = 5; j >= 1; --j)
		{
			*(data + i * 6 + j) = num[(int)Axis[i] % 10];
			Axis[i] /= 10.0;
		}
	}
}

/**
 * @brief 主函数
 */
/**Tested Fine**/
 
int main(void)
{	
	
	bool sysWorking = TRUE;
	int count = 0;
	char data[21];
	
	/* Initialize devices */
	SYSTICK_Config();
	Init_USART(HT_USART0,9600);		
	Init_USART(HT_USART1,115200);		

	PDMA_Configuration();	//配置PDMA
	
	/* Init Camera */
	Ov7725_GPIO_Config();
	
	/*配置蜂鸣器引脚C9*/
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_9, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);
	
	/*配置共阳引脚C12*/
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_12, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_12, RESET);
	
	/*配置共地引脚C8*/
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_8, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_8, RESET);
	
	/*配置debug引脚为C10*/
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_10, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_10, GPIO_PR_DOWN);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_10, ENABLE);
	
	pwrcu_init();
	
	/* main loop */           	
	while (1)
	{	
		Enter_DeepSleepMode();	//进入睡眠模式
		PDMA_Configuration();	//配置PDMA
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_12, SET);
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_8, SET);
		delay_ms(50);
		
		/* Init SD and mount it */
		SD_SPI_Init();
		SD_TryInit();
		sdfs_app_mnt();
		
		/* Init Camera */
		Ov7725_GPIO_Config();
		Ov7725_TryInit();
		Ov7725_VSYNC_Init();
		sysWorking = TRUE; 
		
		//printf("Enter Main_loop...\n");
		/* main loop */ 
		while(sysWorking)	/* 系统开始工作 */
		{					
			if(PDMA_GetFlagStatus(PDMA_CH7, PDMA_FLAG_TC) == SET)
			{
				if(IS_MPU_RUNNING)
				{
					if(SET == GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_10))//拉高C10引脚则进入debug模式
					{	
						//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, SET);//测试用的，从led是否亮看能否工作
						count++;
						if(count >= 10)
						{
							PackDatasForBlueTooth(data);//打包加速度数据给蓝牙
							printf("%s\n", data);
							count = 0;
						}
					}
						
					if(TRUE == IsAccident())
					{
						//printf("Accident!%f %f %f %f\n", Axis[0], Axis[1], Axis[2], Square(Axis[0]) + Square(Axis[1]) + Square(Axis[2]));
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, SET);
						
						PackDatasForBlueTooth(data);//打包加速度数据给蓝牙
						printf("%s\n", data);
						
						for(count = 0; count < NUM_PHOTO; count++)
						{
							sdfs_app_savePhoto();
							delay_ms(50);
						}
						CrashFunction();
						
					}
					//else
						//printf("Not!%f %f %f %f\n", Axis[0], Axis[1], Axis[2], Square(Axis[0]) + Square(Axis[1]) + Square(Axis[2]));
				}
				//else
					//printf("%d", MPU_Data[0]);
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
