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
*        Abstract Description   :	����SD�ײ�IO�����������ļ�ϵͳFATFS��Ӧ�á�����ļ�����д�ļ��ȡ�
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
 * @brief	������Ϣ�����
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
 * @brief 	�����ļ�ϵͳ�Ƿ���������
 * @param  
 * @retval 	None
 */
void sdfs_app_test(void);


/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
