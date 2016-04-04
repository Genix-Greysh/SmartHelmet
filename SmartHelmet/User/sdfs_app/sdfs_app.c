#include "sdfs_app.h"

/* �����ļ�ϵͳ������HT���ջ�ռ��С�������Ҫ��myfat����Ϊȫ�ֵ� */
FATFS myfat;

/* Ĭ�ϵ�Ŀ¼ */
const char *root = "";

u8 BGR_Buf[FRAME_WIDTH * 3];

/* bmp�ļ�ͷ����Ϣͷ����54���ֽڡ�Ϊ���Ч�ʣ�����Ϊȫ�ֵġ� */
u8 bmpHeader[54] =
{
	/* bitmap file header */
	0x42, 0x4d,   
	0, 0, 0, 0, 
	0, 0,         
	0, 0,     
	54, 0, 0, 0, 
	
	/* bitmap infomation header */
	40,0, 0, 0, 
	0, 0, 0, 0, 
	0, 0, 0, 0, 
	1, 0, 
	24, 0, 			/* 24 bits color */
	0, 0, 0, 0, 	/* comporession */
	0, 0, 0, 0, 	/* bize of bitmap */
	0, 0, 0, 0, 	
	0, 0, 0, 0,
	0, 0, 0, 0, 
	0, 0, 0, 0
};



#if SDFS_APP_DEBUG_ON
/**
 * @brief 	�����ļ�ϵͳ�Ƿ���������
 * @param  
 * @retval 	None
 */
void sdfs_app_test(void)
{
	/* �ļ����� */
    FIL fil;
 
	/* ����SD�� */
	if(f_mount(&myfat, root, 0) == FR_OK)
	{
		SDFS_APP_DEBUG("Mount SD successfully\r\n");
		if (f_open(&fil, "testfile.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK)
		{
			SDFS_APP_DEBUG("Open file successfully.\r\n");
			if (f_printf(&fil, "Hello,World!\n") > 0)
				SDFS_APP_DEBUG("Puts file successfully.\r\n");
			else
				SDFS_APP_DEBUG("Puts file failed.\r\n");
		}
		else
			SDFS_APP_DEBUG("Open file failed.\r\n");
		f_close(&fil);
	}
	else
		SDFS_APP_DEBUG("Mount failed\r\n");
}
#endif


/**
 * @brief 	����SD��
 * @param	
 * @retval 	1 �����ɹ�
			0 ����ʧ��
 */
int sdfs_app_mnt(void)
{
	/* ע��һ�����������߼���Ϊ1 */
	if(f_mount(&myfat, root, 1) != FR_OK)
	{
		SDFS_APP_DEBUG("Mount SD failed.\r\n");
		return 0;
	}
	return 1;
}


/**
 * @brief 	��OV7725��ȡһ֡ͼ����bmp��ʽд��SD��
 * @param	filename �ļ���
			width ͼ��֡�Ŀ��
			height ͼ��֡�ĸ߶�
 * @retval 	1 �����ɹ�
			0 ����ʧ��
 */
int sdfs_app_writeBmp(char *filename, int width, int height)
{
	FIL bmpFil;		/* �ļ����� */
	long fileSize;
	UINT bw;
	int i, j, k;
	u16 pixel;
	u8 zeroBytes[4] = {0, 0, 0, 0};
	u8 fillCnt = width % 4;
	FRESULT res;
	
	
	/* ��*��*3(һ��������Ҫ3���ֽڴ��RGB��Ϣ) +������ֽ� + ͷ����Ϣ */
	fileSize = (long)width * (long)height * 3 + height * fillCnt + 54;		

	/* �ļ���С 4���ֽ� */
	bmpHeader[2] = (unsigned char)(fileSize &0x000000ff);
	bmpHeader[3] = (fileSize >> 8) & 0x000000ff;
	bmpHeader[4] = (fileSize >> 16) & 0x000000ff;
	bmpHeader[5] = (fileSize >> 24) & 0x000000ff;
	
	/* λͼ�� 4���ֽ� */
	bmpHeader[18] = width & 0x000000ff;
	bmpHeader[19] = (width >> 8) & 0x000000ff;
	bmpHeader[20] = (width >> 16) & 0x000000ff;
	bmpHeader[21] = (width >> 24) & 0x000000ff;
	
	/* λͼ�� 4���ֽ� */
	bmpHeader[22] = height & 0x000000ff;
	bmpHeader[23] = (height >> 8) & 0x000000ff;
	bmpHeader[24] = (height >> 16) & 0x000000ff;
	bmpHeader[25] = (height >> 24) & 0x000000ff;
	
	/* �½�һ���ļ� */
	if ((res = f_open(&bmpFil, filename, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK)
	{
		SDFS_APP_DEBUG("Open file failed.\r\nError Code : %d\r\n", res);
		return 0;
	}		
	
	/* ��Ԥ�ȶ���õ�bmpͷ����Ϣд���ļ����� */
	if(f_write(&bmpFil, bmpHeader, sizeof(bmpHeader), &bw) != FR_OK)
	{
		SDFS_APP_DEBUG("Write file failed.Byte written : %u\r\n", bw);
		return 0;	
	}
	
	/* ׼����FIFO�ɼ����� */
	FIFO_PREPARE;
	
	/* ���ɼ���������д���ļ� */
	for(i = 0; i < height; ++i)
	{
		for(j = 0, k = 0; j < width; ++j, k +=3 )	/* ��RGB565����ת����д���ļ� */
		{
			READ_FIFO_PIXEL(pixel);
			BGR_Buf[k] 		= GETB_FROM_RGB16(pixel);
			BGR_Buf[k + 1] 	= GETG_FROM_RGB16(pixel);
			BGR_Buf[k + 2] 	= GETR_FROM_RGB16(pixel);						
		}
		f_write(&bmpFil, &BGR_Buf, sizeof(u8) * width * 3, &bw);
		
		if(fillCnt != 0)	/* ����4�ֽڶ�������Ҫ���� */
		{	
			f_write(&bmpFil, zeroBytes, sizeof(u8) * fillCnt, &bw);
		}
	}		
	
	f_close(&bmpFil); 
	return 1;
}



/**
 *  @name	sdfs_app_savePhoto
 *  @brief	����ʱ�����س�ʼ��OV7725
 *  @param  None        
 *  @return None
 *  @notice
 */
void sdfs_app_savePhoto()
{
	extern u8 SD_Type;			/* ���SD�������� */
	char filename[32];			/* �ļ��������� */
	static u16 num = 0;			/* �ļ���׺ */
	
	/* SD������������ͷ��ͼ�οɲɼ� */
	if(SD_Type != SD_TYPE_ERR && Ov7725_IfPhotoCaptured())
	{
			SDFS_APP_DEBUG("Begin to save photo...\r\n");
			if(num == (u16)29360)	/* SD������ԼΪ7GB��BMP�ļ���СԼ250K����Լ�ܴ��29360�ļ� */
				return;
			sprintf((char *)filename, "/%d.bmp", num++);  /* �ļ�����������Ϊ8 */
			if(sdfs_app_writeBmp(filename, FRAME_WIDTH , FRAME_HEIGHT))
				SDFS_APP_DEBUG("Photo saved.\r\n");
			else
				SDFS_APP_DEBUG("Cannot write bmp to SD.\r\n");		
			
			Ov7725_ClearCaptureFlag();
	}
	else
		SDFS_APP_DEBUG("Camera module not work.\r\n");
}
