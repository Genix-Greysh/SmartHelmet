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
*        Abstract Description   :	ʵ��SCCBͨ��Э��
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

/*�豸��ַ����*/
#define DEV_ADR  ADDR_OV7725 			 
#define ADDR_OV7725   0x42


/* ����ʱ������ꡣ���У�SCLΪPD4��SDAΪPD5 */

/* ����SCL����Ϊ���ģʽ */
#define SCL_OUT_MODE GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4, GPIO_DIR_OUT)

/* ����SCL����Ϊ����ģʽ */
#define SCL_IN_MODE GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4, GPIO_DIR_IN)

/* ����SCL */
#define SCL_H GPIO_SetOutBits(HT_GPIOD, GPIO_PIN_4)
/* ����SCL */
#define SCL_L GPIO_ClearOutBits(HT_GPIOD, GPIO_PIN_4)
/* ��ȡSCL */
#define SCL_read GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_4)


/* ����SDA����Ϊ���ģʽ */
#define SDA_OUT_MODE GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_5, GPIO_DIR_OUT)

/* ����SDA����Ϊ����ģʽ */
#define SDA_IN_MODE GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_5, GPIO_DIR_IN)

/* ����SDA */
#define SDA_H GPIO_SetOutBits(HT_GPIOD, GPIO_PIN_5)
/* ����SDA */
#define SDA_L GPIO_ClearOutBits(HT_GPIOD, GPIO_PIN_5)
/* ��ȡSDA */
#define SDA_read GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_5)


/**************************************************************
*        Struct Define Section
**************************************************************/


/**************************************************************
*        Prototype Declare Section
**************************************************************/

/**
 * @brief	SCCB�������á�ѡ��PD7(SCCB_SCL)��PD8(SCCB_SDA)
 * @param	
 * @return	None
 */
void SCCB_GPIO_Config(void);
	

/**
 * @brief	���豸дһ�ֽ�����
 * @param 	WriteAddress ��д���ַ 	
			SendByte ��д������
			DeviceAddress: ��������
 * @retval 	1 �ɹ�д��
			0 ʧ��
 */
int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte );


/**
 * @brief	���豸��ȡһ�ֽ�����
 * @param 	pBuffer ��Ŷ������� 	
			length ����������	
			ReadAddress ��������ַ		 
			DeviceAddress ��������
 * @retval 	1 �ɹ�����
			0 ʧ��
 */
int SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress);


/**
 * @brief	SCCB��ʼ�ź�
 * @param 	 
 * @retval 	None
 */
int SCCB_Start(void);

/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
