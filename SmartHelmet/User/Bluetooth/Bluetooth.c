#include "Bluetooth.h"
#include "usart.h"
#include "delay.h"

/**
 * @brief  �����źŴ�����
 * @param  
 * @retval None
 */
 void CrashFunction()
 {
	 u8 sendSignal = 0;
	 u8 recSignal = 0;
	 while(1)
	 {
	 	 //�����¹ʷ����ź�
		 putchar(sendSignal);
		 //����ȷ���ź�
		 recSignal = getchar();
		 //���ܴ��������ź�
		 recSignal = getchar();
		 
		 if(recSignal == 0x11)
		 {
			 //������ķ���
			 recSignal = getchar();
			 if(recSignal == 0x11)
			 {
				//�ƶ����������
					break;
			 }
			 else
			 {
				 //�ƶ����������ʧ��
				 continue;
			 
			 }
		 }
		 else
		 {
			 //�󴥷�
			 break;
			
		 }	 
	 }
	 
 }
 