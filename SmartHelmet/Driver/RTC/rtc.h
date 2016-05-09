/***************************************************************************************
*        File Name              :	rtc.h
*        CopyRight              :	Zoisite
*        ModuleName             :	rtc
*
*        CPU                    :	Intel i5-4210u
*        RTOS                   :	Windows 7 pro
*
*        Create Data            :	
*        Author/Corportation    :   Gaunthan/Zoisite
*
*        Abstract Description   :	HT板载RTC配置与封装时间获取函数
*
*--------------------------------Revision History--------------------------------------
*       No      version     Data        Revised By      Item        Description
*       1       v1.0        2016/3/22   Gaunthan                    Create this file
*
***************************************************************************************/
/**************************************************************
*        Multi-Include-Prevent Section
**************************************************************/
#ifndef _RTC_H
#define _RTC_H

/**************************************************************
*        Debug switch Section
**************************************************************/

#define RTC_DEBUG_ON 0

/**************************************************************
*        Include File Section
**************************************************************/
#include "ht32.h"
#include "ht32_board.h"

/**************************************************************
*        Macro Define Section
**************************************************************/

/**
  * @brief	调试信息输出宏定义
  */
#if RTC_DEBUG_ON
	#define RTC_DEBUG(fmt,args...) printf (fmt ,##args)
#else
	#define RTC_DEBUG(fmt,args...)
#endif

/**************************************************************
*        Struct Define Section
**************************************************************/


/**************************************************************
*        Prototype Declare Section
**************************************************************/


/**
 * @brief	通过串口0交互式配置RTC
 * @param  
 * @return	None
 */
void RTC_ConfigOnline(void);


/**
 * @brief	获取RTC计数值
 * @param  
 * @return	一个u32类型的计数值
 */
u32 RTC_getTime(void);

/*********************************************************************************************************//**
  * @brief  Displays the input time.
  * @param  wTimeVar: Displays time.
  * @retval None
  ***********************************************************************************************************/
void Time_Display(u32 wTimeVar);


/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
