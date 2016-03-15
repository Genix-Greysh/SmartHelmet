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
void InitSPI0(void);

/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
