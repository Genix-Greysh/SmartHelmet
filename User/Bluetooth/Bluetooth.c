#include "Bluetooth.h"

/**
 * @brief  车祸信号处理函数
 * @param  
 * @retval None
 */
void CrashFunction(void)
{
	u8 recSignal[2] = {0};
	u8 flag;	//判断车祸发生时，移动端是否完成任务
	while(1)
	{
		printf("aaa\n");//发送车祸发生信号

		/*接受移动端发来的‘ok’信号*/
		recSignal[0] = getchar();
		recSignal[1] = getchar();
	 
		if(recSignal[0] == 'o' && recSignal[1] == 'k')
		{
			//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//关闭蜂鸣器
			//如果接收到ac，则代表车祸真的发生；接收到wt则代表是错误触发车祸
			recSignal[0] = getchar();
			recSignal[1] = getchar();

			if(recSignal[0] == 'a' && recSignal[1] == 'c')
			{
				//移动端完成任务
				flag = getchar();
				if(flag != '0')//确认车祸时，app发送字符'0'
					continue;
				
				//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//关闭蜂鸣器
				break;
			}
			else if(recSignal[0] == 'w' && recSignal[1] == 't')
			{
				GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//关闭蜂鸣器
				break;
			}
			else{
				continue;//移动端出故障
			}
		}
		
	}
}

