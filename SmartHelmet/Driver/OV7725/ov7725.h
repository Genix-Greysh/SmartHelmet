/***************************************************************************************
*        File Name              :	ov7725.h
*        CopyRight              :	Zoisite
*        ModuleName             :	ov7725
*
*        CPU                    :	Intel i5-4210u
*        RTOS                   :	Windows 7 pro
*
*        Create Data            :	
*        Author/Corportation    :   Gaunthan/Zoisite
*
*        Abstract Description   :	OV7725驱动函数
*
*--------------------------------Revision History--------------------------------------
*       No      version     Data        Revised By      Item        Description
*       1       v1.0        2016/3/22   Gaunthan                    Create this file
*		2		v1.1		2016/3/28	Gaunthan					开始移植工作
*		3		v2.0		2016/04/04	Gaunthan					完成摄像头模块
***************************************************************************************/

/**************************************************************
*        Multi-Include-Prevent Section
**************************************************************/
#ifndef _OV7725_H
#define _OV7725_H

/**************************************************************
*        Debug switch Section
**************************************************************/
/**
 * @brief 	OV7725调试开关
 */
#define OV7725_DEBUG_ON 1

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

/* 通过FIFO读取一个像素，其中用到了PC0~PC7引脚  */
#define READ_FIFO_PIXEL(RGB565)   									\
	do{																\
	  RGB565 = 0;													\
	  FIFO_RCLK_H();												\
	  FIFO_RCLK_L();												\
	  RGB565 = (GPIO_ReadInData(HT_GPIOC) & 0xff) << 8;				\
	  FIFO_RCLK_H();												\
	  FIFO_RCLK_L();												\
	  RGB565 |= GPIO_ReadInData(HT_GPIOC) & 0xff;					\
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
	

	
/* 设备低位ID */
#define OV7725_ID       0x21
	
	
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
 *  @name	Ov7725_VSYNC_Init
 *  @brief	初始化OV7725 VSYNC引脚
 *  @param  None        
 *  @return None
 *  @notice
 */
void Ov7725_VSYNC_Init(void);								


/**
 *  @name	Ov7725_GPIO_Config
 *  @brief	配置OV7725所用到的引脚
 *  @param  None        
 *  @return None
 *  @notice
 */
void Ov7725_GPIO_Config(void);


/**
 *  @name	Ov7725_Init
 *  @brief	初始化OV7725
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7725_Init(void);

/**
 *  @name	Ov7725_TryInit
 *  @brief	带超时计数地初始化OV7725
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7725_TryInit(void);


/**
 * @name	Ov7725_IfPhotoCaptured
 * @brief	判断FIFO是否已经采集完一帧图像
 * @param	NOne
 * @return	1 FIFO已经采集到一帧图形
			0 未采集完
 * @notes	当Ov7725_vsync为2时，FIFO采集完一帧图像 
 */
int Ov7725_IfPhotoCaptured(void);


/**
 * @name	Ov7725_ClearCaptureFlag
 * @brief	清除已采集标志，使FIFO重新开始获取图像帧
 * @param	None
 * @return	None
 * @notes	当Ov7725_vsync为0时，FIFO重新开始读取图像帧
 */
void Ov7725_ClearCaptureFlag(void);


/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
