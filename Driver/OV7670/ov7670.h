/***************************************************************************************
*        File Name              :	ov7670.h
*        CopyRight              :	Zoisite
*        ModuleName             :	ov7670
*
*        CPU                    :	Intel i5-4210u
*        RTOS                   :	Windows 7 pro
*
*        Create Data            :	
*        Author/Corportation    :   Gaunthan/Zoisite
*
*        Abstract Description   :	ov7670驱动函数
*
*--------------------------------Revision History--------------------------------------
*       No      version     Data        Revised By      Item        Description
*       1       v1.0        2016/04/02  Gaunthan                    Create this file
***************************************************************************************/


/**************************************************************
*        Multi-Include-Prevent Section
**************************************************************/
#ifndef _OV7670_H
#define _OV7670_H

/**************************************************************
*        Debug switch Section
**************************************************************/
/**
 * @brief 	OV7670调试开关
 */
#define OV7670_DEBUG_ON 1

/**************************************************************
*        Include File Section
**************************************************************/
#include "ht32.h"
#include "sccb.h"

/**************************************************************
*        Macro Define Section
**************************************************************/

#define FIFO_CS_H()     GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_4) 
#define FIFO_CS_L()     GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_4)  /*拉低使FIFO输出使能*/

#define FIFO_WRST_H()   GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_2) 	 /*拉高允许FIFO写(数据from摄像头)指针运动 */
#define FIFO_WRST_L()   GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_2)  /*拉低使FIFO写(数据from摄像头)指针复位*/

#define FIFO_RRST_H()   GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_3)	 /*拉高允许FIFO读(数据从FIFO输出)指针运动 */
#define FIFO_RRST_L()   GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_3)  /*拉低使FIFO读(数据从FIFO输出)指针复位 */

#define FIFO_RCLK_H()   GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_5) 	  
#define FIFO_RCLK_L()   GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_5)  /*FIFO输出数据时钟*/

#define FIFO_WE_H()     GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_6)	 /*拉高使FIFO写允许*/
#define FIFO_WE_L()     GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_6)

/* 通过FIFO读取一个像素 */
#define READ_FIFO_PIXEL(RGB565)   						\
	do{													\
	  RGB565 = 0;										\
	  FIFO_RCLK_L();									\
	  RGB565 = (HT_GPIOB->DINR) & 0xff00;				\
	  FIFO_RCLK_H();									\
	  FIFO_RCLK_L();									\
	  RGB565 |= (HT_GPIOB->DINR >>8) & 0x00ff;			\
	  FIFO_RCLK_H();									\
	}while(0)

/* 使FIFO进入准备状态 */
#define FIFO_PREPARE               						\
	do{													\
	  FIFO_RRST_L();									\
	  FIFO_RCLK_L();									\
	  FIFO_RCLK_H();									\
	  FIFO_RRST_H();									\
	  FIFO_RCLK_L();									\
	  FIFO_RCLK_H();									\
	}while(0)

/* 设备ID */
#define OV7670_ID       0x73

									
/*
  * Basic window sizes.  These probably belong somewhere more globally
  * useful.
  */
#define VGA_WIDTH       640
#define VGA_HEIGHT      480
#define QVGA_WIDTH      320
#define QVGA_HEIGHT     240
#define CIF_WIDTH       352
#define CIF_HEIGHT      288
#define QCIF_WIDTH      176
#define QCIF_HEIGHT     144
									
/* 帧高度 */
#define FRAME_HEIGHT QVGA_HEIGHT
									
/* 帧宽度 */
#define FRAME_WIDTH QVGA_WIDTH

/**************************************************************
*        Struct Define Section
**************************************************************/


/**************************************************************
*        Prototype Declare Section
**************************************************************/

									
/**
 *  @name	VSYNC_Init
 *  @brief	初始化OV7670 VSYNC引脚
 *  @param  None        
 *  @return None
 *  @notice
 */
void VSYNC_Init(void);								


/**
 *  @name	Ov7670_GPIO_Config
 *  @brief	配置OV7670所用到的引脚
 *  @param  None        
 *  @return None
 *  @notice
 */
void Ov7670_GPIO_Config(void);


/**
 *  @name	Ov7670_Init
 *  @brief	初始化OV7670
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7670_Init(void);

/**
 *  @name	Ov7670_TryInit
 *  @brief	带超时计数地初始化OV7670
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7670_TryInit(void);


/**
 *	@name	ImageLineDetect
 *	@brief	采集一帧图像的某一行
 *	@param	None
 *	@return	None
 *	@notice	由于函数栈过小，本函数使用了全局数组来存放采集到的数据
 */		
void ImageLineDetect(void);


/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
