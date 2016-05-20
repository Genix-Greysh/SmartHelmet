#include "Bluetooth.h"

/**
 * @brief  �����źŴ�����
 * @param  
 * @retval None
 */
void CrashFunction(void)
{
	u8 recSignal[3] = {0};
	u8 ch;
	u8 flag;	//�жϳ�������ʱ���ƶ����Ƿ��������
	while(1)
	{
		printf("aaa\n");//���ͳ��������ź�

		/*�����ƶ��˷����ġ�ok���ź�*/
		while('o' != (ch = getchar()));
		
		recSignal[0] = ch;
		recSignal[1] = getchar();
		
		BLUETOOTH_DEBUG("%c,%c\n", recSignal[0], recSignal[1]);   
		
		if(recSignal[1] == 'k')
		{
			//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//�رշ�����
			//������յ�ac�����������ķ��������յ�wt������Ǵ��󴥷�����
			recSignal[0] = getchar();
			recSignal[1] = getchar();
			recSignal[2] = '\0';
			
			if(recSignal[0] == 'a' && recSignal[1] == 'c')
			{
				//�ƶ����������
				flag = getchar();
				if(flag != '0')//ȷ�ϳ���ʱ��app�����ַ�'0'
					continue;
				BLUETOOTH_DEBUG("ac success!\n");
				//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//�رշ�����
				break;
			}
			else if(recSignal[0] == 'w' && recSignal[1] == 't')
			{
				GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//�رշ�����
				BLUETOOTH_DEBUG("wt success! %s\n", recSignal);
				break;
			}
			else{
				BLUETOOTH_DEBUG("ac wt error!\n");
				continue;//�ƶ��˳�����
			}
		}
		else{
			BLUETOOTH_DEBUG("ok error %s\n", recSignal);
		}
		
	}
}

