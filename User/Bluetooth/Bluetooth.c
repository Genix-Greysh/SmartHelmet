#include "Bluetooth.h"

/**
 * @brief  车祸信号处理函数
 * @param  
 * @retval 1表示车祸发生，0表示误触发
 */
int CrashFunction(void)
{
	u8 recSignal[3] = {0};
	u8 ch;
	u8 flag;	//判断车祸发生时，移动端是否完成任务
	while(1)
	{
		printf("aaa\n");//发送车祸发生信号

		/*接受移动端发来的‘ok’信号*/
		while('o' != (ch = getchar()));
		
		recSignal[0] = ch;
		recSignal[1] = getchar();
		
		BLUETOOTH_DEBUG("%c,%c\n", recSignal[0], recSignal[1]);   
		
		if(recSignal[1] == 'k')
		{
			//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//关闭蜂鸣器
			//如果接收到ac，则代表车祸真的发生；接收到wt则代表是错误触发车祸
			recSignal[0] = getchar();
			recSignal[1] = getchar();
			recSignal[2] = '\0';
			
			if(recSignal[0] == 'a' && recSignal[1] == 'c')
			{
				//移动端完成任务
				flag = getchar();
				if(flag != '0')//确认车祸时，app发送字符'0'
					continue;
				BLUETOOTH_DEBUG("ac success!\n");
				//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//关闭蜂鸣器
				return 1;
			}
			else if(recSignal[0] == 'w' && recSignal[1] == 't')
			{
				GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//关闭蜂鸣器
				BLUETOOTH_DEBUG("wt success! %s\n", recSignal);
				return 0;
			}
			else{
				BLUETOOTH_DEBUG("ac wt error! %s\n", recSignal);
				continue;//移动端出故障
			}
		}
		else{
			BLUETOOTH_DEBUG("ok error %s\n", recSignal);
		}
	}
}

