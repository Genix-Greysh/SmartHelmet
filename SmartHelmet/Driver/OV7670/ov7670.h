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
*        Abstract Description   :	ov7670��������
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
 * @brief 	OV7670���Կ���
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
#define FIFO_CS_L()     GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_4)  /*����ʹFIFO���ʹ��*/

#define FIFO_WRST_H()   GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_2) 	 /*��������FIFOд(����from����ͷ)ָ���˶� */
#define FIFO_WRST_L()   GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_2)  /*����ʹFIFOд(����from����ͷ)ָ�븴λ*/

#define FIFO_RRST_H()   GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_3)	 /*��������FIFO��(���ݴ�FIFO���)ָ���˶� */
#define FIFO_RRST_L()   GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_3)  /*����ʹFIFO��(���ݴ�FIFO���)ָ�븴λ */

#define FIFO_RCLK_H()   GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_5) 	  
#define FIFO_RCLK_L()   GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_5)  /*FIFO�������ʱ��*/

#define FIFO_WE_H()     GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_6)	 /*����ʹFIFOд����*/
#define FIFO_WE_L()     GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_6)

/* ͨ��FIFO��ȡһ������ */
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

/* ʹFIFO����׼��״̬ */
#define FIFO_PREPARE               						\
	do{													\
	  FIFO_RRST_L();									\
	  FIFO_RCLK_L();									\
	  FIFO_RCLK_H();									\
	  FIFO_RRST_H();									\
	  FIFO_RCLK_L();									\
	  FIFO_RCLK_H();									\
	}while(0)

/* �豸ID */
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
									
/* ֡�߶� */
#define FRAME_HEIGHT QVGA_HEIGHT
									
/* ֡��� */
#define FRAME_WIDTH QVGA_WIDTH

/**************************************************************
*        Struct Define Section
**************************************************************/


/**************************************************************
*        Prototype Declare Section
**************************************************************/

									
/**
 *  @name	VSYNC_Init
 *  @brief	��ʼ��OV7670 VSYNC����
 *  @param  None        
 *  @return None
 *  @notice
 */
void VSYNC_Init(void);								


/**
 *  @name	Ov7670_GPIO_Config
 *  @brief	����OV7670���õ�������
 *  @param  None        
 *  @return None
 *  @notice
 */
void Ov7670_GPIO_Config(void);


/**
 *  @name	Ov7670_Init
 *  @brief	��ʼ��OV7670
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7670_Init(void);

/**
 *  @name	Ov7670_TryInit
 *  @brief	����ʱ�����س�ʼ��OV7670
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7670_TryInit(void);


/**
 *	@name	ImageLineDetect
 *	@brief	�ɼ�һ֡ͼ���ĳһ��
 *	@param	None
 *	@return	None
 *	@notice	���ں���ջ��С��������ʹ����ȫ����������Ųɼ���������
 */		
void ImageLineDetect(void);


/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
