#include "sdfs_app.h"

/* 用于文件系统。由于HT板堆栈空间较小，因此需要将myfat定义为全局的 */
FATFS myfat;

/* 默认的目录 */
const char *root = "";

u8 BGR_Buf[FRAME_WIDTH * 3];

/* bmp文件头与信息头，共54个字节。为提高效率，定义为全局的。 */
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
 * @brief 	测试文件系统是否正常工作
 * @param  
 * @retval 	None
 */
void sdfs_app_test(void)
{
	/* 文件对象 */
    FIL fil;
 
	/* 挂载SD卡 */
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
 * @brief 	挂载SD卡
 * @param	
 * @retval 	1 操作成功
			0 操作失败
 */
int sdfs_app_mnt(void)
{
	/* 注册一个工作区，逻辑号为1 */
	if(f_mount(&myfat, root, 1) != FR_OK)
	{
		SDFS_APP_DEBUG("Mount SD failed.\r\n");
		return 0;
	}
	return 1;
}


/**
 * @brief 	从OV7725获取一帧图像并以bmp格式写入SD卡
 * @param	filename 文件名
			width 图像帧的宽度
			height 图形帧的高度
 * @retval 	1 操作成功
			0 操作失败
 */
int sdfs_app_writeBmp(char *filename, int width, int height)
{
	FIL bmpFil;		/* 文件对象 */
	long fileSize;
	UINT bw;
	int i, j, k;
	u16 pixel;
	u8 zeroBytes[4] = {0, 0, 0, 0};
	u8 fillCnt = width % 4;
	FRESULT res;
	
	
	/* 宽*高*3(一个像素需要3个字节存放RGB信息) +补充的字节 + 头部信息 */
	fileSize = (long)width * (long)height * 3 + height * fillCnt + 54;		

	/* 文件大小 4个字节 */
	bmpHeader[2] = (unsigned char)(fileSize &0x000000ff);
	bmpHeader[3] = (fileSize >> 8) & 0x000000ff;
	bmpHeader[4] = (fileSize >> 16) & 0x000000ff;
	bmpHeader[5] = (fileSize >> 24) & 0x000000ff;
	
	/* 位图宽 4个字节 */
	bmpHeader[18] = width & 0x000000ff;
	bmpHeader[19] = (width >> 8) & 0x000000ff;
	bmpHeader[20] = (width >> 16) & 0x000000ff;
	bmpHeader[21] = (width >> 24) & 0x000000ff;
	
	/* 位图高 4个字节 */
	bmpHeader[22] = height & 0x000000ff;
	bmpHeader[23] = (height >> 8) & 0x000000ff;
	bmpHeader[24] = (height >> 16) & 0x000000ff;
	bmpHeader[25] = (height >> 24) & 0x000000ff;
	
	/* 新建一个文件 */
	if ((res = f_open(&bmpFil, filename, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK)
	{
		SDFS_APP_DEBUG("Open file failed.\r\nError Code : %d\r\n", res);
		return 0;
	}		
	
	/* 将预先定义好的bmp头部信息写进文件里面 */
	if(f_write(&bmpFil, bmpHeader, sizeof(bmpHeader), &bw) != FR_OK)
	{
		SDFS_APP_DEBUG("Write file failed.Byte written : %u\r\n", bw);
		return 0;	
	}
	
	/* 准备从FIFO采集数据 */
	FIFO_PREPARE;
	
	/* 将采集到的数据写入文件 */
	for(i = 0; i < height; ++i)
	{
		for(j = 0, k = 0; j < width; ++j, k +=3 )	/* 将RGB565像素转化后写入文件 */
		{
			READ_FIFO_PIXEL(pixel);
			BGR_Buf[k] 		= GETB_FROM_RGB16(pixel);
			BGR_Buf[k + 1] 	= GETG_FROM_RGB16(pixel);
			BGR_Buf[k + 2] 	= GETR_FROM_RGB16(pixel);						
		}
		f_write(&bmpFil, &BGR_Buf, sizeof(u8) * width * 3, &bw);
		
		if(fillCnt != 0)	/* 不是4字节对齐则需要补齐 */
		{	
			f_write(&bmpFil, zeroBytes, sizeof(u8) * fillCnt, &bw);
		}
	}		
	
	f_close(&bmpFil); 
	return 1;
}



/**
 *  @name	sdfs_app_savePhoto
 *  @brief	带超时计数地初始化OV7725
 *  @param  None        
 *  @return None
 *  @notice
 */
void sdfs_app_savePhoto()
{
	extern u8 SD_Type;			/* 存放SD卡的类型 */
	char filename[32];			/* 文件名缓冲区 */
	static u16 num = 0;			/* 文件后缀 */
	
	/* SD卡可用且摄像头有图形可采集 */
	if(SD_Type != SD_TYPE_ERR && Ov7725_IfPhotoCaptured())
	{
			SDFS_APP_DEBUG("Begin to save photo...\r\n");
			if(num == (u16)29360)	/* SD卡容量约为7GB，BMP文件大小约250K，大约能存放29360文件 */
				return;
			sprintf((char *)filename, "/%d.bmp", num++);  /* 文件名长度限制为8 */
			if(sdfs_app_writeBmp(filename, FRAME_WIDTH , FRAME_HEIGHT))
				SDFS_APP_DEBUG("Photo saved.\r\n");
			else
				SDFS_APP_DEBUG("Cannot write bmp to SD.\r\n");		
			
			Ov7725_ClearCaptureFlag();
	}
	else
		SDFS_APP_DEBUG("Camera module not work.\r\n");
}
