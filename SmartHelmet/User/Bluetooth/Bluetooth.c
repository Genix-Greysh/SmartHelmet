#include "Bluetooth.h"

/**
 * @brief  �����źŴ�����
 * @param  
 * @retval None
 */
void CrashFunction(void)
{
	u8 recSignal[2] = {0};
	while(1)
	{
		printf("aaa\n");//���ͳ��������ź�

		/*�����ƶ��˷����ġ�ok���ź�*/
		recSignal[0] = getchar();
		recSignal[1] = getchar();
		getchar();
	 
		if(recSignal[0] == 'o' && recSignal[1] == 'k')
		{
			//������յ�ac�����������ķ��������յ�wt������Ǵ��󴥷�����
			recSignal[0] = getchar();
			recSignal[1] = getchar();
			getchar();
			if(recSignal[0] == 'a' && recSignal[1] == 'c')
			{
				//�ƶ����������
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
				continue;//�ƶ��˳�����
			}
		}
		
	}
}

