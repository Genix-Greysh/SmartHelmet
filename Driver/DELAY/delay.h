/***************************************************************************************
*        File Name              :	delay.h
*        CopyRight              :	Zoisite
*        ModuleName             :	delay
*
*        CPU                    :	Intel i5-4210u
*        RTOS                   :	Windows 7 pro
*
*        Create Data            :	
*        Author/Corportation    :   Gaunthan/Zoisite
*
*        Abstract Description   :	offer API of USART device
*
*--------------------------------Revision History--------------------------------------
*       No      version     Data        	Revised By      Item        Description
*       1       v1.0        2016/3/21       Gaunthan                    Create this file
*
***************************************************************************************/
/**************************************************************
*        Multi-Include-Prevent Section
**************************************************************/
#ifndef _DELAY_H
#define _DELAY_H

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


/**************************************************************
*        Prototype Declare Section
**************************************************************/

/**
 * @brief  ����̬����DelayTime��ֵ����һ��
 * @param  
 * @retval 	None
 * @notes	�ú�������ht32f165x_it.c�µ�SysTick_Handler�������á�
 */
void TimingDelay(void);


/**
 * @brief	����SYSTICK����������ʱ
 * @param  
 * @retval 	None
 */
void SYSTICK_Config(void);


/**
 * @brief	��ʱָ����mSec����ֵ
 * @param  	mSec  ��Ҫ��ʱ�ĺ���ֵ
 * @retval 	None
 */
void delay_ms(u32 mSec);

/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
