#include "sdfs_app.h"

/* 用于文件系统。由于HT板堆栈空间较小，因此需要将myfat定义为全局的 */
FATFS myfat;

/* 默认的目录 */
const char *root = "";


/**
 * @brief 	测试文件系统是否正常工作
 * @param  
 * @retval 	None
 */void sdfs_app_test(void)
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
