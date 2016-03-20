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

u8 MPU_Data [33] ;

char standard = 28;

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
 * @brief	延时指定的ms时间
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


//读取SD卡的指定扇区的内容，并通过串口1输出
//sec：扇区物理地址编号
void SD_Read_Sectorx(u32 sec)
{
	u8 buf[512] = {0};
	u16 i;
	if(SD_ReadDisk(buf,sec,1) == 1)	//读取0扇区的内容
	{	
		printf("SECTOR 0 DATA:\r\n");
		for(i=0;i<512;i++)
			printf("%x ",buf[i]);//打印sec扇区数据    	   
		printf("\r\nDATA ENDED\r\n");
	}
}


int main(void)
{	

	/* Initialize devices */
	Init_USART( HT_USART0,115200);		
	Init_USART( HT_USART1,115200);	
	
	PDMA_Configuration();
	
	
	
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
