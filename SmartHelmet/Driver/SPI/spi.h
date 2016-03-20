/***************************************************************************************
*        File Name              :	spi.h
*        CopyRight              :	Zoisite
*        ModuleName             :	spi
*
*        CPU                    :	Intel i5-4210u
*        RTOS                   :	Windows 7 pro
*
*        Create Data            :	
*        Author/Corportation    :   Gaunthan/Zoisite
*
*        Abstract Description   :	offer API of SPI device
*
*--------------------------------Revision History--------------------------------------
*       No      version     Data        Revised By      Item        Description
*       1       v1.0        2016/       Gaunthan                    Create this file
*
***************************************************************************************/
/**************************************************************
*        Multi-Include-Prevent Section
**************************************************************/
#ifndef _SPI_H
#define _SPI_H

/**************************************************************
*        Debug switch Section
**************************************************************/


/**************************************************************
*        Include File Section
**************************************************************/
#include "ht32.h"
#include "ht32_board.h"

/**************************************************************
*        Macro Define Section
**************************************************************/

/**
 * @brief  通过SPIx发送数据
 * @param  SPIx，SPI设备
 * @param  data，要发送的数据
 * @retval None
 */
#define SPI_SENDDATA(SPIx, data)								\
	do															\
	{															\
		while (!SPI_GetFlagStatus(SPIx, SPI_FLAG_TXBE));		\
		SPI_SendData(SPIx, (u32)(data)); 						\
	}while(0);

/**
 * @brief  通过SPIx获取数据
 * @param  SPIx，SPI设备
 * @param  buf，存储获取到的数据
 * @retval None
 */
#define SPI_GETDATA(SPIx, buf)									\
	do															\
	{															\
		while (!SPI_GetFlagStatus(SPIx, SPI_FLAG_RXBNE));		\
		buf = SPI_ReceiveData(SPIx);							\
	}while(0)
	
/**
 * @brief  通过SPI0发送数据
 * @param  data，要发送的数据
 * @retval None
 */
#define SPI0_SENDDATA(data) SPI_SENDDATA(HT_SPI0, data)

	
/**
 * @brief  通过SPI0获取数据
 * @param  buf，存储获取到的数据
 * @retval None
 */
#define SPI0_GETDATA(buf) SPI_GETDATA(HT_SPI0, buf)
	
	
/** @defgroup SPI_BaudRate_Prescaler 
  * @{
  */

#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0000)
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0001)
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0003)
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0007)
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x000e)
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x001f)
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x003f)
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x007f)
#define IS_SPI_BAUDRATE_PRESCALER(PRESCALER) (((PRESCALER) == SPI_BaudRatePrescaler_2) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_4) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_8) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_16) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_32) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_64) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_128) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_256))

/**************************************************************
*        Struct Define Section
**************************************************************/


/**************************************************************
*        Prototype Declare Section
**************************************************************/

/**
 * @brief  初始化SPI0
 * @param  
 * @retval None
 */
void SPI0_Init(void);


/**
 * @brief  通过SPI0发送、读取一个字节
 * @param  txData 要发送的字节
 * @retval 读取的字节
 */
u8 SPI0_ReadWriteByte(u8 txData);


/**
 * @brief  设置SPI0的通信速率
 * @param  SpeedSet 分频值，可取如下：
				SPI_BaudRatePrescaler_2   2   	(SPI 36M@sys 72M)
				SPI_BaudRatePrescaler_8   8  	(SPI 9M@sys 72M)
				SPI_BaudRatePrescaler_16  16 	(SPI 4.5M@sys 72M)
				SPI_BaudRatePrescaler_256 256 	(SPI 281.25K@sys 72M)
 * @retval None
 */
void SPI0_SetSpeed(u8 speedSet);


/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
