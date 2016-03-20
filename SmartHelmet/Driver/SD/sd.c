#include "sd.h"

u8 SD_Type = 0;	/* ���SD�������� */



/**
 * @brief  ��SPI����Ϊ����ģʽ
 * @param  
 * @retval None
 */
static void SD_SPI_SetLowSpeed(void)
{
 	SPI0_SetSpeed(SPI_BaudRatePrescaler_256);	//���õ�����ģʽ	
}


/**
 * @brief  ��SPI����Ϊ����ģʽ
 * @param  
 * @retval None
 */
static void SD_SPI_SetHighSpeed(void)
{
 	SPI0_SetSpeed(SPI_BaudRatePrescaler_8);	//���õ�����ģʽ	
}


/**
 * @brief	�ȴ�SD��׼����
 * @param  
 * @retval 	1 ��׼��
			0 �ȴ���ʱ
 */
static u8 SD_WaitReady(void)
{
	u32 t = 0;
	
	while(++t < 0XFFFFFF)	//�ȴ� 
	{
		if(SD_SPI_ReadWriteByte(SD_DUMMY_BYTE) == SD_DUMMY_BYTE)	//���߿���		
			return 1;		  	
	}	
	
	return 0;
}


/**
 * @brief	ȡ��Ƭѡ��SD��
 * @param  
 * @retval 	None
 */
void SD_DisSelect(void)
{
	/* ȡ��Ƭѡ�豸 */
	SPI_SoftwareSELCmd(HT_SPI0, SPI_SEL_INACTIVE);
	
	//�ṩ�����8��ʱ��
 	SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);	
}


/**
 * @brief	Ƭѡ��SD����ʹ�乤��
 * @param  
 * @retval 	1 ѡ�гɹ�
			0 ѡ��ʧ��
 */
u8 SD_Select(void)
{	
	/* Ƭѡ�豸 */
	SPI_SoftwareSELCmd(HT_SPI0, SPI_SEL_ACTIVE);
	
	if(SD_WaitReady())	//�ȴ��ɹ�
		return 1;	
	else				//�ȴ�ʧ��
	{
		SD_DisSelect();
		return 0;		
	}
}


/**
 * @brief  	��ʼ��SD��ص����ź��豸��SD��Ƭѡ�ӵ�PE12
 * @param  
 * @retval 	None
 */
void SD_SPI_Init(void)
{
	/* ��ʼ��SPI0 */
	SPI0_Init();
}
	

/**
 * @brief  	��SD������һ������
 * @param	cmd ���� 
			arg �������
			crc crcУ��ֵ
 * @retval 	SD�����ص���Ӧ
 */
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;	
	u8 retry = 0; 
	
	SD_DisSelect();		//ȡ���ϴ�Ƭѡ
	if(!SD_Select())	//����Ƭѡ
		return SD_DUMMY_BYTE;	//Ƭѡʧ��
	
	//�����������Լ�У���
	
    SD_SPI_ReadWriteByte(cmd | 0x40);	//8λ������λ��Ϊ1
    SD_SPI_ReadWriteByte(arg >> 24);	//��32λ�Ĳ�����ֳ�4�ֽڣ���һ����
    SD_SPI_ReadWriteByte(arg >> 16);
    SD_SPI_ReadWriteByte(arg >> 8);
    SD_SPI_ReadWriteByte(arg);	  
    SD_SPI_ReadWriteByte(crc); 			//8λУ���
	
	if(cmd == CMD12)	//����ֹͣ����ӷ���������
		SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);	
	
    //�ȴ���Ӧ����ʱ�˳�
	retry = 0X1F;
	do
	{
		r1 = SD_SPI_ReadWriteByte(0xFF);
	}while((r1 & 0X80) && retry--);	 //r1���λ��Ϊ1������Ӧ�������ȴ�

    return r1;
}


/**
 *  @name SD_GetResponse
 *	@description �ȴ�SD����Ӧ
 *	@param response Ҫ��õĻ�Ӧ
 *	@return	SD_RESPONSE_NO_ERROR �ɹ��õ���Ӧ
			SD_RESPONSE_FAILURE �ȴ���Ӧ��ʱʧ�ܻ��Ӧ��ƥ��
 *  @notice
 */
u8 SD_GetResponse(u8 response)
{
	u16 Count = 0xFFFF;//�ȴ�����	   						  
	
	while ((SD_SPI_ReadWriteByte(SD_DUMMY_BYTE) != response) && Count)
		Count--;	//�ȴ��õ�׼ȷ�Ļ�Ӧ  	  
	if(Count == 0)
		return SD_RESPONSE_FAILURE;		//�õ���Ӧʧ��   
	else 
		return SD_RESPONSE_NO_ERROR;	//��ȷ��Ӧ
}


/**
 *  @name SD_RecvData
 *	@description ��sd����ȡһ�����ȵ����ݣ����浽������buf��
 *	@param 	buf ���ݻ�����
			len Ҫ��ȡ�����ݳ���
 *	@return 1 ��ȡ�ɹ�
			0 ��ȡʧ��	
 *  @notice
 */
u8 SD_RecvData(u8 *buf,u16 len)
{	
	u8 tmp;
	
	//�ȴ�SD������������ʼ����
	if(SD_GetResponse(SD_BEGIN_END_FLAG) != SD_RESPONSE_NO_ERROR)
		return 0;
	
	//��ʼ��������
    while(len--)
    {
        tmp = SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);
      
		/* ���ں�̩�İ�ֻ֧��256B��С�����飬��ֻ�ܶ�ȡ256B�ֽڵ����ݵ�buf */
//		if(256 < len)
			*buf++ = tmp;
//		else
//			return 1;
    }
	
    //������2��αCRC��dummy CRC��
    SD_SPI_ReadWriteByte(0xFF);
    SD_SPI_ReadWriteByte(0xFF);		
	
    return 1;//��ȡ�ɹ�
}



/**
 *  @name SD_SendBlock
 *	@description ��sd��д��һ�����ݰ������ݣ���512�ֽ�
 *	@param 	buf ���ݻ�����
			cmd ָ��
 *	@return 1 ���ͳɹ�
			0 ����ʧ��	
 *  @notice
 */
u8 SD_SendBlock(u8 *buf, u8 cmd)
{	
	u16 cnt;
	u8 r1;
	
	if(!SD_WaitReady())
		return 0;	//�ȴ�׼��ʧЧ
	
	SD_SPI_ReadWriteByte(cmd);
	
	if(cmd != 0XFD)//���ǽ���ָ��
	{
		for(cnt = 0;cnt < 512;cnt++)
			SD_SPI_ReadWriteByte(buf[cnt]);
	    SD_SPI_ReadWriteByte(0xFF);//����crc
	    SD_SPI_ReadWriteByte(0xFF);
		r1 = SD_SPI_ReadWriteByte(0xFF);//������Ӧ
		if((r1 & 0x1F) != 0x05)
			return 0;	//��Ӧ����									  					    
	}						 									  					    
    return 1;	//д��ɹ�
}



/**
 * @brief  	��ʼ��SD��ʹ����빤��ģʽ
 * @param  
 * @retval 	0 ������
 */
u8 SD_Init(void)
{
	int i;
	u8 r1;		//���������Ӧ
	u16 retry;	//��ų�ʱ����
	u8 buf[4];	//��ʱ������

	
	/* SD����ʼ���׶Σ�CLKʱ������ܳ���400KHz */
	SD_SPI_SetLowSpeed();
	
	/* �ϵ���ʱ����74��ʱ������ */
	for(i = 0; i < 10; ++i)
		SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);
	retry = 20;
	while((r1 = SD_SendCmd(CMD0, 0, 0x95)) != 0x01 && retry-- > 0)	//����IDLE״̬���ȴ���ȷ����
		;
	if(r1 == 0x01)	//����IDLE״̬�ɹ�
	{
		//����SD��������ѹ����SD������
		if(SD_SendCmd(CMD8, 0x1AA, 0x87) == 1)		//SD V2.0
		{
			for(i = 0;i < 4;i++)
				buf[i] = SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);	//Get trailing return value of R7 resp
			if(buf[2] == 0X01 && buf[3] == 0XAA)	//���Ƿ�֧��2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SD_SendCmd(CMD55, 0, 0X01);	//����CMD55
					r1 = SD_SendCmd(CMD41, 0x40000000, 0X01);	//����CMD41
				}while(r1 && retry--);
				if(retry && SD_SendCmd(CMD58, 0, 0X01) == 0)	//����SD2.0���汾��ʼ
				{
					for(i = 0;i < 4;i++)
						buf[i] = SD_SPI_ReadWriteByte(0XFF);//�õ�OCRֵ
					if(buf[0] & 0x40)	//���CCS
						SD_Type = SD_TYPE_V2HC;    
					else 
						SD_Type = SD_TYPE_V2;   
				}
			}
		}
		else	//SD V1.x/ MMC	V3
		{
			SD_SendCmd(CMD55, 0, 0X01);		//����CMD55
			r1 = SD_SendCmd(CMD41, 0, 0X01);	//����CMD41
			if(r1 <= 1)
			{		
				SD_Type = SD_TYPE_V1;
				retry = 0XFFFE;
				do //�ȴ��˳�IDLEģʽ
				{
					SD_SendCmd(CMD55, 0, 0X01);	//����CMD55
					r1 = SD_SendCmd(CMD41, 0, 0X01);//����CMD41
				}while(r1 && retry--);
			}
			else	//MMC����֧��CMD55+CMD41ʶ��
			{
				SD_Type = SD_TYPE_MMC;//MMC V3
				retry = 0XFFFE;
				do //�ȴ��˳�IDLEģʽ
				{											    
					r1 = SD_SendCmd(CMD1,0,0X01);//����CMD1
				}while(r1 && retry--);  
			}
			if(retry == 0 || SD_SendCmd(CMD16, 512, 0X01) != 0)
				SD_Type = SD_TYPE_ERR;//����Ŀ�
		}
	}

	SD_DisSelect();			//ȡ��Ƭѡ
	SD_SPI_SetHighSpeed();	//������������Ϊ����
	
	if(SD_Type)	//�����󣬷���0
		return 0;
	else if(r1)
		return r1; 
	else
		return 0xaa;	//��������	
}



/**
 *  @name SD_ReadDisk
 *	@description ��SD������
 *	@param 	buf ���ݻ�����
			sector ������
			cnt Ҫ��ȡ��������Ŀ
 *	@return ���������1 �ɹ���������ʧ��
 *  @notice
 */
u8 SD_ReadDisk(u8 *buf, u32 sector, u8 cnt)
{
	u8 r1;
	if(SD_Type != SD_TYPE_V2HC)
		sector <<= 9;//ת��Ϊ�ֽڵ�ַ
	if(cnt == 1)
	{
		r1 = SD_SendCmd(CMD17, sector, 0X01);//������
		if(r1 == 0)//ָ��ͳɹ�
		{
			r1 = SD_RecvData(buf, 512);//����512���ֽ�	   
		}
	}
	else
	{
		r1 = SD_SendCmd(CMD18, sector, 0X01);//����������
		do
		{
			r1 = SD_RecvData(buf, 512);//����512���ֽ�	 
			buf += 512;  
		}while(--cnt && r1 == 1); 	
		SD_SendCmd(CMD12, 0, 0X01);	//����ֹͣ����
	}   
	SD_DisSelect();//ȡ��Ƭѡ
	
	return r1;//
}


/**
 *  @name SD_WriteDisk
 *	@description дSD������
 *	@param 	buf ���ݻ�����
			sector ������
			cnt Ҫд���������Ŀ
 *	@return ���������1 �ɹ���������ʧ��
 *  @notice
 */
u8 SD_WriteDisk(u8 *buf, u32 sector, u8 cnt)
{
	u8 r1;
	if(SD_Type != SD_TYPE_V2HC)
		sector *= 512;//ת��Ϊ�ֽڵ�ַ
	if(cnt == 1)
	{
		r1 = SD_SendCmd(CMD24,sector,0X01);//������
		if(r1 == 0)//ָ��ͳɹ�
		{
			r1 = SD_SendBlock(buf,0xFE);//д512���ֽ�	   
		}
	}
	else
	{
		if(SD_Type != SD_TYPE_MMC)
		{
			SD_SendCmd(CMD55,0,0X01);	
			SD_SendCmd(CMD23,cnt,0X01);//����ָ��	
		}
 		r1 = SD_SendCmd(CMD25,sector,0X01);//����������
		if(r1 == 0)
		{
			do
			{
				r1 = SD_SendBlock(buf,0xFC);//����512���ֽ�	 
				buf += 512;  
			}while(--cnt && r1 == 1);
			r1 = SD_SendBlock(0,0xFD);//����512���ֽ� 
		}
	}   
	SD_DisSelect();//ȡ��Ƭѡ
	return r1;//
}	


