/***************************************************************************************
*        File Name              :	MPU6050.h
*        CopyRight              :	Zoisite
*        ModuleName             :	MPU6050
*
*        CPU                    :	Intel i5-4210u
*        RTOS                   :	Windows 7 pro
*
*        Create Data            :	
*        Author/Corportation    :   Gaunthan/Zoisite
*
*        Abstract Description   :	offer API of MPU6050 device
*
*--------------------------------Revision History--------------------------------------
*       No      version     Data        Revised By      Item        Description
*       1       v1.0        2016/       Subai                    Create this file
*
***************************************************************************************/
/**************************************************************
*        Multi-Include-Prevent Section
**************************************************************/
#ifndef _MPU6050_H
#define _MPU6050_H

/**************************************************************
*        Debug switch Section
**************************************************************/


/**************************************************************
*        Include File Section
**************************************************************/
#include "ht32.h"
#include "ht32_board.h"
#include "math.h"

/**************************************************************
*        Macro Define Section
**************************************************************/
#define PI 3.1415926
#define Square(x) ((x) * (x))
#define IS_MPU_RUNNING (MPU_Data[0] == 0x55)

/**************************************************************
*        Struct Define Section
**************************************************************/
extern u8 MPU_Data [33];

extern float Axis[3];
extern float Angle[3];
extern float Angular[3];
extern float FinalAxis[3];

/**************************************************************
*        Prototype Declare Section
**************************************************************/

/**
 * @brief  �������������ĽǶ�
 * @param  
 * @retval None
 */
void Angle_DataTransfrom(void);

/**
 * @brief  �ó�����������Ľ��ٶ�
 * @param  
 * @retval None
 */
void Angular_DataTransFrom(void);

/**
 * @brief  �ó�����������ļ��ٶ�
 * @param  
 * @retval None
 */
void Axis_DataTransfrom(void);

/**
 * @brief  �����õļ��ٶȣ�ȥ����׼ֵ
 * @param  
 * @retval None
 */
void Axis_GetFinalData(void);

/**
 * @brief  PDMA�ĳ�ʼ����
 * @param  
 * @retval None
 */
void PDMA_Configuration(void);

/**
 * @brief  		������ת����ٶȵ�����ƽ������
 * @param  		void
 */
void DealWithRotation(void);

/**
 * @brief  		ͨ�������ٶȵľ��ұ仯���ж��Ƿ�������
 * @param  		void
 * @retval 		TRUE����ʾ���������ˣ�FALSE����ʾ��û�з�������
 */
bool IsAccident(void);
/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif


