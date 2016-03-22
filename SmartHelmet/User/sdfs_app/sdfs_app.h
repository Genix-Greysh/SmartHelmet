/***************************************************************************************
*        File Name              :	sdfs_app.h
*        CopyRight              :	Zoisite
*        ModuleName             :	sdfs
*
*        CPU                    :	Intel i5-4210u
*        RTOS                   :	Windows 7 pro
*
*        Create Data            :	
*        Author/Corportation    :   Gaunthan/Zoisite
*
*        Abstract Description   :	基于SD底层IO驱动构建的文件系统FATFS的应用。如打开文件、读写文件等。
*
*--------------------------------Revision History--------------------------------------
*       No      version     Data        	Revised By      Item        Description
*       1       v1.0       	2016/3/22       Gaunthan                    Create this file
*
***************************************************************************************/
/**************************************************************
*        Multi-Include-Prevent Section
**************************************************************/
#ifndef _SDFS_APP_H
#define _SDFS_APP_H

/**************************************************************
*        Debug switch Section
**************************************************************/

#define SDFS_APP_DEBUG_ON 1

/**************************************************************
*        Include File Section
**************************************************************/
#include "ht32.h"
#include "ht32_board.h"
#include "sd.h"
#include "ff.h"

/**************************************************************
*        Macro Define Section
**************************************************************/

/**
 * @brief	调试信息输出宏
 */
#if SDFS_APP_DEBUG_ON
	#define SDFS_APP_DEBUG(fmt,args...) printf (fmt ,##args)
#else
	#define SDFS_APP_DEBUG(fmt,args...)
#endif


/**************************************************************
*        Struct Define Section
**************************************************************/


/**************************************************************
*        Prototype Declare Section
**************************************************************/

/**
 * @brief 	测试文件系统是否正常工作
 * @param  
 * @retval 	None
 */
void sdfs_app_test(void);


/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
