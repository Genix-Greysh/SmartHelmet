#include "Bluetooth.h"
#include "usart.h"
#include "delay.h"

/**
 * @brief  车祸信号处理函数
 * @param  
 * @retval None
 */
 void CrashFunction()
 {
	 u8 sendSignal = 0;
	 u8 recSignal = 0;
	 while(1)
	 {
	 	 //发送事故发生信号
		 putchar(sendSignal);
		 //接受确认信号
		 recSignal = getchar();
		 //接受处理类型信号
		 recSignal = getchar();
		 
		 if(recSignal == 0x11)
		 {
			 //车祸真的发生
			 recSignal = getchar();
			 if(recSignal == 0x11)
			 {
				//移动端完成任务
					break;
			 }
			 else
			 {
				 //移动端完成任务失败
				 continue;
			 
			 }
		 }
		 else
		 {
			 //误触发
			 break;
			
		 }	 
	 }
	 
 }
 