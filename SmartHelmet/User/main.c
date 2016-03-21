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


#define SECTOR_SIZE 512

u8 buf[SECTOR_SIZE];

u8 MPU_Data [33] ;

char standard = 28;

/*********************************************************************************************************//**
* @brief  Configures GPTM0 for time estimate.
* @retval None
***********************************************************************************************************/
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
 * @brief 	查看某个扇区的内容
 * @param 	sector 扇区号
 * @return 	None
 */
void ViewSector(u8 sector)
{
	int i;

	if(!SD_ReadDisk(buf, sector, 1))
		printf("\r\nRead SD failed.\r\n");
	else
	{
		printf("\r\nView sector %d: \r\n", sector);
		for(i = 0; i < SECTOR_SIZE; ++i)
			printf("%x ", buf[i]);
	
		printf("\r\nSector %d END...\r\n", sector);
	}	
	
}

/**
 * @brief 	用于测试SD卡
 * @param 	
 * @return 	None
 */
void TestForSD(void)
{
	int i, j;
	
	/* 初始化SD卡 */
	while(SD_Init())	//检测不到SD卡
	{
		printf("SD Card Error!\r\n");
		for(i = 0xffff; i > 0; --i)
			for(j = 110; j > 0; --j);
	}
	printf("Init SD Card OK!\r\n");
	printf("Randomly generate numbers to write SD Card:\r\n");
	for(i = 0; i < SECTOR_SIZE; ++i)
	{
		buf[i] = 1;
		printf("%x ", buf[i]);
	}
	
	if(SD_WriteDisk(buf, 0, 1))
		printf("\r\nWrite Sector %d succeeded\r\n", 0);
	else
		printf("Wrtie failed.\r\n");
	ViewSector(0);
}

/**
 * @brief 主函数
 */
int main(void)
{
	/* Initialize devices */
	Init_USART( HT_USART0,115200);		
	PDMA_Configuration();
	SD_SPI_Init();
	
	
	TestForSD();
	
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
