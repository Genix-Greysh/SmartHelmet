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
char num[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};//�������ַ���ʾ

/**
 * @brief  		������ٶ������Է��͸��ֻ�����ʾ
 * @param  		char *data:�洢�����͵�����
 * @retval 		void
 */
void PackDatasForBlueTooth(char *data)
{
	int i, j;
	
	for(i = 0; i < 3; ++i)
		*data++ = 'b';//ǰ�����ַ�Ϊ'b',��data������ǰ�ƶ�һλ
	
	for(i = 0; i < 3; ++i)//xyz����ѭ��
	{
		if(Axis[i] < 0)
		{
			Axis[i] = -Axis[i];
			*(data + i * 6) = num[1];//��������λΪ1
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
 * @brief ������
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

	PDMA_Configuration();	//����PDMA
	
	/* Init Camera */
	Ov7725_GPIO_Config();
	
	/*���÷���������C9*/
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_9, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);
	
	/*���ù�������C12*/
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_12, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_12, RESET);
	
	/*���ù�������C8*/
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_8, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_8, RESET);
	
	/*����debug����ΪC10*/
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_10, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_10, GPIO_PR_DOWN);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_10, ENABLE);
	
	pwrcu_init();
	
	/* main loop */           	
	while (1)
	{	
		Enter_DeepSleepMode();	//����˯��ģʽ
		PDMA_Configuration();	//����PDMA
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
		while(sysWorking)	/* ϵͳ��ʼ���� */
		{					
			if(PDMA_GetFlagStatus(PDMA_CH7, PDMA_FLAG_TC) == SET)
			{
				if(IS_MPU_RUNNING)
				{
					if(SET == GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_10))//����C10���������debugģʽ
					{	
						//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, SET);//�����õģ���led�Ƿ������ܷ���
						count++;
						if(count >= 10)
						{
							PackDatasForBlueTooth(data);//������ٶ����ݸ�����
							printf("%s\n", data);
							count = 0;
						}
					}
						
					if(TRUE == IsAccident())
					{
						//printf("Accident!%f %f %f %f\n", Axis[0], Axis[1], Axis[2], Square(Axis[0]) + Square(Axis[1]) + Square(Axis[2]));
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, SET);
						
						PackDatasForBlueTooth(data);//������ٶ����ݸ�����
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
