#include "sdfs_app.h"

/* �����ļ�ϵͳ������HT���ջ�ռ��С�������Ҫ��myfat����Ϊȫ�ֵ� */
FATFS myfat;

/* Ĭ�ϵ�Ŀ¼ */
const char *root = "";


/**
 * @brief 	�����ļ�ϵͳ�Ƿ���������
 * @param  
 * @retval 	None
 */void sdfs_app_test(void)
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
