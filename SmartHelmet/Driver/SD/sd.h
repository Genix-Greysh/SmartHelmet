/***************************************************************************************
*        File Name              :	sd.h
*        CopyRight              :	Zoisite
*        ModuleName             :	sd
*
*        CPU                    :	Intel i5-4210u
*        RTOS                   :	Windows 7 pro
*
*        Create Data            :	
*        Author/Corportation    :   Gaunthan/Zoisite
*
*        Abstract Description   :	offer API of SD card
*
*--------------------------------Revision History--------------------------------------
*      	No      version     Data        	Revised By      Item        Description
*       1       v1.0        2016/3/16       Gaunthan                    Create this file
*		2		v2.0		2016/3/20		Gaunthan		fix			�ɹ���ֲ��HT������
***************************************************************************************/
/**************************************************************
*        Multi-Include-Prevent Section
**************************************************************/
#ifndef _SD_H
#define _SD_H

/**************************************************************
*        Debug switch Section
**************************************************************/

/**
  * @brief 	���Կ���
  */
#define SD_DEBUG_ON 1

/**************************************************************
*        Include File Section
**************************************************************/
#include "ht32.h"
#include "ht32_board.h"
#include "spi.h"
#include "delay.h"

/**************************************************************
*        Macro Define Section
**************************************************************/

/**
  * @brief 	Ĭ��������С512
  */
#define SECTOR_SIZE 512



/**
  * @brief 	SD�����Ͷ���  
  */
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06


/**
  * @brief 	SD��ָ��� 
  */
#define CMD0    0       //����λ
#define CMD1	1
#define CMD8    8       //����8 ��SEND_IF_COND
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define CMD23   23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define CMD41   41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00


/**
  * @brief	����д���Ӧ������
  */
#define SD_DATA_OK                0x05
#define SD_DATA_CRC_ERROR         0x0B
#define SD_DATA_WRITE_ERROR       0x0D
#define SD_DATA_OTHER_ERROR       0xFF


/**
  * @brief	SD����Ӧ�����
  */
#define SD_RESPONSE_NO_ERROR      0x00
#define SD_IN_IDLE_STATE          0x01
#define SD_ERASE_RESET            0x02
#define SD_ILLEGAL_COMMAND        0x04
#define SD_COM_CRC_ERROR          0x08
#define SD_ERASE_SEQUENCE_ERROR   0x10
#define SD_ADDRESS_ERROR          0x20
#define SD_PARAMETER_ERROR        0x40
#define SD_RESPONSE_FAILURE       0xFF


/**
  * @brief  Dummy byte
  */
#define SD_DUMMY_BYTE 0xFF


/**
  * @brief	������ʼ���ƶ���
  */
#define SD_BEGIN_END_FLAG 0xFE


/**
  * @brief	ͨ��SPI��дһ���ֽ�
  */
#define SD_SPI_ReadWriteByte(dat) SPI0_ReadWriteByte(dat)


/**
  * @brief	������Ϣ����궨��
  */
#if SD_DEBUG_ON
	#define SD_DEBUG(fmt,args...) printf (fmt ,##args)
#else
	#define SD_DEBUG(fmt,args...)
#endif
   
/**************************************************************
*        Struct Define Section
**************************************************************/


/**************************************************************
*        Prototype Declare Section
**************************************************************/


/**
 * @brief  	��ʼ��SD��ص����ź��豸��SD��Ƭѡ�ӵ�PE12
 * @param  
 * @retval 	None
 */
void SD_SPI_Init(void);


/**
 * @brief  	��ʼ��SD��ʹ����빤��ģʽ
 * @param  
 * @retval 	0 ������
 */
u8 SD_Init(void);


/**
 *  @name SD_ReadDisk
 *	@description ��SD������
 *	@param 	buf ���ݻ�����
			sector ������
			cnt Ҫ��ȡ��������Ŀ
 *	@return ���������1 �ɹ���������ʧ��
 *  @notice
 */
u8 SD_ReadDisk(u8 *buf, u32 sector, u8 cnt);


/**
 *  @name SD_WriteDisk
 *	@description дSD������
 *	@param 	buf ���ݻ�����
			sector ������
			cnt Ҫд���������Ŀ
 *	@return ���������1 �ɹ���������ʧ��
 *  @notice
 */
u8 SD_WriteDisk(const u8 *buf, u32 sector, u8 cnt);


/**
 *  @name SD_RecvData
 *	@description ��sd����ȡһ�����ȵ����ݣ����浽������buf��
 *	@param 	buf ���ݻ�����
			len Ҫ��ȡ�����ݳ���
 *	@return 1 ��ȡ�ɹ�
			0 ��ȡʧ��	
 *  @notice
 */
u8 SD_RecvData(u8 *buf,u16 len);


/**
 * @brief 	����ʱ�˳���SD����ʼ������
 * @param 	
 * @return 	1 ��ʼ���ɹ�
			0 ��ʼ��ʧ��
 */
u8 TryInitSD(void);


/**
 * @brief 	�鿴ĳ������������
 * @param 	sector ������
 * @return 	None
 */
void ViewSector(u8 sector);


/**************************************************************
*        End-Multi-Include-Prevent Section
**************************************************************/
#endif
