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
*        Abstract Description   :	OV7725��������
*
*--------------------------------Revision History--------------------------------------
*       No      version     Data        Revised By      Item        Description
*       1       v1.0        2016/3/22   Gaunthan                    Create this file
*		2		v1.1		2016/3/28	Gaunthan					��ʼ��ֲ����
*		3		v2.0		2016/04/04	Gaunthan					�������ͷģ��
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
 * @brief 	OV7725���Կ���
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
#define FIFO_CS_L()     GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_4)  /*����ʹFIFO���ʹ��*/

#define FIFO_WRST_H()   GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_2) 	 /*��������FIFOд(����from����ͷ)ָ���˶� */
#define FIFO_WRST_L()   GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_2)  /*����ʹFIFOд(����from����ͷ)ָ�븴λ*/

#define FIFO_RRST_H()   GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_3)	 /*��������FIFO��(���ݴ�FIFO���)ָ���˶� */
#define FIFO_RRST_L()   GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_3)  /*����ʹFIFO��(���ݴ�FIFO���)ָ�븴λ */

#define FIFO_RCLK_H()   GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_5) 	  
#define FIFO_RCLK_L()   GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_5)  /*FIFO�������ʱ��*/

#define FIFO_WE_H()     GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_6)	 /*����ʹFIFOд����*/
#define FIFO_WE_L()     GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_6)

/* ͨ��FIFO��ȡһ�����أ������õ���PC0~PC7����  */
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
	

	
/* �豸��λID */
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
 *  @name	Ov7725_VSYNC_Init
 *  @brief	��ʼ��OV7725 VSYNC����
 *  @param  None        
 *  @return None
 *  @notice
 */
void Ov7725_VSYNC_Init(void);								


/**
 *  @name	Ov7725_GPIO_Config
 *  @brief	����OV7725���õ�������
 *  @param  None        
 *  @return None
 *  @notice
 */
void Ov7725_GPIO_Config(void);


/**
 *  @name	Ov7725_Init
 *  @brief	��ʼ��OV7725
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7725_Init(void);

/**
 *  @name	Ov7725_TryInit
 *  @brief	����ʱ�����س�ʼ��OV7725
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7725_TryInit(void);


/**
 * @name	Ov7725_IfPhotoCaptured
 * @brief	�ж�FIFO�Ƿ��Ѿ��ɼ���һ֡ͼ��
 * @param	NOne
 * @return	1 FIFO�Ѿ��ɼ���һ֡ͼ��
			0 δ�ɼ���
 * @notes	��Ov7725_vsyncΪ2ʱ��FIFO�ɼ���һ֡ͼ�� 
 */
int Ov7725_IfPhotoCaptured(void);


/**
 * @name	Ov7725_ClearCaptureFlag
 * @brief	����Ѳɼ���־��ʹFIFO���¿�ʼ��ȡͼ��֡
 * @param	None
 * @return	None
 * @notes	��Ov7725_vsyncΪ0ʱ��FIFO���¿�ʼ��ȡͼ��֡
 */
void Ov7725_ClearCaptureFlag(void);


/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
