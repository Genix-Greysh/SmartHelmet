#include "Bluetooth.h"

/**
 * @brief  车祸信号处理函数
 * @param  
 * @retval None
 */
void CrashFunction(void)
{
	u8 recSignal[2] = {0};
	while(1)
	{
		printf("aaa\n");//发送车祸发生信号

		/*接受移动端发来的‘ok’信号*/
		recSignal[0] = getchar();
		recSignal[1] = getchar();
		getchar();
	 
		if(recSignal[0] == 'o' && recSignal[1] == 'k')
		{
			//如果接收到ac，则代表车祸真的发生；接收到wt则代表是错误触发车祸
			recSignal[0] = getchar();
			recSignal[1] = getchar();
			getchar();
			if(recSignal[0] == 'a' && recSignal[1] == 'c')
			{
				//移动端完成任务
				printf("app success!\n");
				break;
			}
			else if(recSignal[0] == 'w' && recSignal[1] == 't')
			{
				printf("wrong accident!\n");
				break;
			}
			else{
				printf("app failed!\n");
				continue;//移动端出故障
			}
		}
		
	}
}

