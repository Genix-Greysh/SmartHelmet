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

/**************************************************************
*        Macro Define Section
**************************************************************/


/**************************************************************
*        Struct Define Section
**************************************************************/

extern u8 MPU_Data [33] ;

extern float X_Axis ;
extern float Y_Axis ;
extern float Z_Axis ;

/**************************************************************
*        Prototype Declare Section
**************************************************************/

/**
 * @brief  得出三个方向轴的加速度
 * @param  
 * @retval None
 */
void Axis_DataTransfrom(void );

/**
 * @brief  PDMA的初始化。
 * @param  
 * @retval None
 */
void PDMA_Configuration(void);

/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif


