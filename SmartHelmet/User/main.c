/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"

#include "usart.h"
#include "spi.h"
#include "sd.h"

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
void GPTM_Configuration(void)
{
	GPTM_TimeBaseInitTypeDef TimeBaseInit;
	 
	TimeBaseInit.CounterMode = GPTM_CNT_MODE_UP;
	TimeBaseInit.CounterReload = SystemCoreClock / (1000 * 2);
	TimeBaseInit.Prescaler = 1;
	TimeBaseInit.PSCReloadTime = GPTM_PSC_RLD_IMMEDIATE;
	GPTM_TimeBaseInit(HT_GPTM0, &TimeBaseInit);
	GPTM_Cmd(HT_GPTM0, ENABLE);
}


/**
 * @brief	��ʱָ����msʱ��
 */
void delay_ms(u32 ms)
{
	GPTM_SetCounter(HT_GPTM0, 0);

	while (ms--)
	{
		GPTM_ClearFlag(HT_GPTM0, GPTM_FLAG_UEV);
		while (!GPTM_GetFlagStatus(HT_GPTM0, GPTM_FLAG_UEV));
	}
}


//��ȡSD����ָ�����������ݣ���ͨ������1���
//sec�����������ַ���
void SD_Read_Sectorx(u32 sec)
{
	u8 buf[512] = {0};
	u16 i;
	if(SD_ReadDisk(buf,sec,1) == 1)	//��ȡ0����������
	{	
		printf("SECTOR 0 DATA:\r\n");
		for(i=0;i<512;i++)
			printf("%x ",buf[i]);//��ӡsec��������    	   
		printf("\r\nDATA ENDED\r\n");
	}
}


int main(void)
{
	int i;
	int receivedat;
	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.GPTM0      = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	GPTM_Configuration();               /* GPTM configuration                                                 */

	/* Initialize devices */
	InitUSART0(115200);
	
	/* ��ʼ����SD�йص����� */
	SD_SPI_Init();
	
	/* test SPI */
	for(i = 0; i < 10; ++i)
	{
		receivedat = SPI0_ReadWriteByte(i);
		printf("Sent %d\r\n", i);
		printf("Receive %d\r\n", receivedat);
	}
	
	while(SD_Init())	//��ⲻ��SD��
	{
		printf("SD Card Error!\r\n");					
		delay_ms(500);
	}
	
	SD_Read_Sectorx(0);
	
	/* main loop */                                                
	while (1)
	{
		
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
