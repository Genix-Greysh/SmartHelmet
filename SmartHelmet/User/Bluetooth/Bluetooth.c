#include "Bluetooth.h"

/**
 * @brief  �����źŴ�����
 * @param  
 * @retval None
 */
void CrashFunction(void)
{
	u8 recSignal[2] = {0};
	u8 flag;	//�жϳ�������ʱ���ƶ����Ƿ��������
	while(1)
	{
		printf("aaa\n");//���ͳ��������ź�

		/*�����ƶ��˷����ġ�ok���ź�*/
		recSignal[0] = getchar();
		recSignal[1] = getchar();
	 
		if(recSignal[0] == 'o' && recSignal[1] == 'k')
		{
			//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//�رշ�����
			//������յ�ac�����������ķ��������յ�wt������Ǵ��󴥷�����
			recSignal[0] = getchar();
			recSignal[1] = getchar();

			if(recSignal[0] == 'a' && recSignal[1] == 'c')
			{
				//�ƶ����������
				flag = getchar();
				if(flag != '0')//ȷ�ϳ���ʱ��app�����ַ�'0'
					continue;
				
				//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//�رշ�����
				break;
			}
			else if(recSignal[0] == 'w' && recSignal[1] == 't')
			{
				GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);//�رշ�����
				break;
			}
			else{
				continue;//�ƶ��˳�����
			}
		}
		
	}
}

