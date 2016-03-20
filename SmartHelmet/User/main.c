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


int main(void)
{
	int i, j;

	/* Initialize devices */
	InitUSART0(115200);
	
	/* 初始化与SD有关的引脚 */
	SD_SPI_Init();
	
	/* 初始化SD卡 */
	while(SD_Init())	//检测不到SD卡
	{
		printf("SD Card Error!\r\n");
		for(i = 0xffff; i > 0; --i)
			for(j = 110; j > 0; --j);
	}
	printf("Init SD Card OK!\r\n");

	ViewSector(0);
	
	printf("Randomly generate :\r\n");
	for(i = 0; i < SECTOR_SIZE; ++i)
	{
		buf[i] = 1;
		printf("%x ", buf[i]);
	}
	
	SD_WriteDisk(buf, 0, 1);
	
	ViewSector(0);
	
	/* main loop */                                                
	while (1)
	{
		printf("In while(1)");
		while(1);
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
