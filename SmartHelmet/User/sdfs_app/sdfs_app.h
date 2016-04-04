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
*		2		v1.1		2016/04/04		Gaunthan					Add bmp relative functions
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
#include "ov7725.h"

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


#define GETR_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )RGB565) >>11)<<3))		  	//����8λ R
#define GETG_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )(RGB565 & 0x7ff)) >>5)<<2)) 	//����8λ G
#define GETB_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )(RGB565 & 0x1f))<<3)))       	//����8λ B


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


/**
 * @brief 	��OV7725��ȡһ֡ͼ����bmp��ʽд��SD��
 * @param	filename �ļ���
			width ͼ��֡�Ŀ��
			height ͼ��֡�ĸ߶�
 * @retval 	1 �����ɹ�
			0 ����ʧ��
 */
int sdfs_app_writeBmp(char *filename, int width, int height);


/**
 * @brief 	����SD��
 * @param	
 * @retval 	1 �����ɹ�
			0 ����ʧ��
 */
int sdfs_app_mnt(void);


/**
 *  @name	sdfs_app_savePhoto
 *  @brief	����ʱ�����س�ʼ��OV7725
 *  @param  None        
 *  @return None
 *  @notice
 */
void sdfs_app_savePhoto(void);

/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
