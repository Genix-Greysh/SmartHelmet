/***************************************************************************************
*        File Name              :	sccb.h
*        CopyRight              :	Zoisite
*        ModuleName             :	sccb
*
*        CPU                    :	Intel i5-4210u
*        RTOS                   :	Windows 7 pro
*
*        Create Data            :	
*        Author/Corportation    :   Gaunthan/Zoisite
*
*        Abstract Description   :	实现SCCB通信协议
*
*--------------------------------Revision History--------------------------------------
*       No      version     Data        Revised By      Item        Description
*       1       v1.0        2016/3/22   Gaunthan                    Create this file
*
***************************************************************************************/
/**************************************************************
*        Multi-Include-Prevent Section
**************************************************************/
#ifndef _SCCB_H
#define _SCCB_H

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

/*设备地址定义*/
#define DEV_ADR  ADDR_OV7725 			 
#define ADDR_OV7725   0x42


/* 总线时序操作宏。其中，SCL为PD4、SDA为PD5 */

/* 设置SCL引脚为输出模式 */
#define SCL_OUT_MODE GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4, GPIO_DIR_OUT)

/* 设置SCL引脚为输入模式 */
#define SCL_IN_MODE GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4, GPIO_DIR_IN)

/* 拉高SCL */
#define SCL_H GPIO_SetOutBits(HT_GPIOD, GPIO_PIN_4)
/* 拉低SCL */
#define SCL_L GPIO_ClearOutBits(HT_GPIOD, GPIO_PIN_4)
/* 读取SCL */
#define SCL_read GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_4)


/* 设置SDA引脚为输出模式 */
#define SDA_OUT_MODE GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_5, GPIO_DIR_OUT)

/* 设置SDA引脚为输入模式 */
#define SDA_IN_MODE GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_5, GPIO_DIR_IN)

/* 拉高SDA */
#define SDA_H GPIO_SetOutBits(HT_GPIOD, GPIO_PIN_5)
/* 拉低SDA */
#define SDA_L GPIO_ClearOutBits(HT_GPIOD, GPIO_PIN_5)
/* 读取SDA */
#define SDA_read GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_5)


/**************************************************************
*        Struct Define Section
**************************************************************/


/**************************************************************
*        Prototype Declare Section
**************************************************************/

/**
 * @brief	SCCB引脚配置。选用PD7(SCCB_SCL)与PD8(SCCB_SDA)
 * @param	
 * @return	None
 */
void SCCB_GPIO_Config(void);
	

/**
 * @brief	向设备写一字节数据
 * @param 	WriteAddress 待写入地址 	
			SendByte 待写入数据
			DeviceAddress: 器件类型
 * @retval 	1 成功写入
			0 失败
 */
int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte );


/**
 * @brief	向设备读取一字节数据
 * @param 	pBuffer 存放读出数据 	
			length 待读出长度	
			ReadAddress 待读出地址		 
			DeviceAddress 器件类型
 * @retval 	1 成功读入
			0 失败
 */
int SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress);


/**
 * @brief	SCCB起始信号
 * @param 	 
 * @retval 	None
 */
int SCCB_Start(void);

/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
