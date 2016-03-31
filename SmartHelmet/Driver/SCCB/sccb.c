#include "sccb.h"


/**
 * @brief	SCCB�������á�ѡ��PD4(SCCB_SCL)��PD5(SCCB_SDA)
 * @param	
 * @return	None
 */
void SCCB_GPIO_Config(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
	/* Enable PD */
	CKCUClock.Bit.PD         = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	/* ����PD4��PD5Ϊ������� */
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4 | GPIO_PIN_5, GPIO_DIR_OUT);
	
	/* ʹ��PD4��PD5������� */
	GPIO_InputConfig(HT_GPIOD, GPIO_PIN_4 | GPIO_PIN_5, ENABLE);	
}


/**
 * @brief	��SCCB������ʱʹ��
 * @param 	 
 * @retval 	None
 */
static void SCCB_delay(void)
{	
   uint16_t i = 400; 
   while(i) 
   { 
     i--; 
   } 
}


/**
 * @brief	SCCB��ʼ�ź�
 * @param 	 
 * @retval 	None
 */
 int SCCB_Start(void)
{
	SDA_H;
	SCCB_delay();
	
	SCL_H;
	SCCB_delay();
	
	if(!SDA_read)
		return DISABLE;	/* SDA��Ϊ�͵�ƽ������æ,�˳� */
	
	SDA_L;
	SCCB_delay();
	
	if(SDA_read) 
		return DISABLE;	/* SDA��Ϊ�ߵ�ƽ�����߳���,�˳� */
	
	SCL_L;
	SCCB_delay();
	
	return ENABLE;
}


/**
 * @brief	SCCBֹͣ�ź�
 * @param 	 
 * @retval 	None
 */
static void SCCB_Stop(void)
{
	SCL_L;
	SCCB_delay();
	SDA_L;
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	SDA_H;
	SCCB_delay();
}


/**
 * @brief	SCCBӦ��ʽ
 * @param 	 
 * @retval 	None
 */
static void SCCB_Ack(void)
{	
	SCL_L;
	SCCB_delay();
	SDA_L;
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	SCL_L;
	SCCB_delay();
}


/**
 * @brief	SCCB ��Ӧ��ʽ
 * @param 	 
 * @retval 	None
 */
static void SCCB_NoAck(void)
{	
	SCL_L;
	SCCB_delay();
	SDA_H;
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	SCL_L;
	SCCB_delay();
}



/**
 * @brief	SCCB �ȴ�Ӧ��
 * @param 	 
 * @retval 	����Ϊ:=1��ACK,=0��ACK
 */
static int SCCB_WaitAck(void) 	
{
	int status;
	
	SCL_L;
	SCCB_delay();
	SDA_H;			
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	if(SDA_read)
		status = DISABLE;
	else
		status = ENABLE;
	SCL_L;
	return status;
}


/**
 * @brief	��SCCB����MSB��ʽ����һ���ֽ�
 * @param 	SendByte ���͵�����
 * @retval 	None
 */
static void SCCB_SendByte(uint8_t SendByte) 
{
    uint8_t i = 8;
    while(i--)
    {
        SCL_L;
        SCCB_delay();
		if(SendByte & 0x80)
			SDA_H;  
		else 
			SDA_L;   
        SendByte <<= 1;
        SCCB_delay();
		SCL_H;
        SCCB_delay();
    }
    SCL_L;
}


/**
 * @brief	��SCCB����MSB��ʽ����һ���ֽ�
 * @param 	
 * @retval 	SCCB���߷��ص�����
 */
static int SCCB_ReceiveByte(void)  
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      SCCB_delay();
	  SCL_H;
      SCCB_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}



/**
 * @brief	���豸дһ�ֽ����� * @param 	WriteAddress ��д���ַ 	
			SendByte ��д������
			DeviceAddress: ��������
 * @retval 	1 �ɹ�д��
			0 ʧ��
 */
int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte )
{		
    if(!SCCB_Start())
	{
	    return DISABLE;
	}
    SCCB_SendByte( DEV_ADR );                    /* ������ַ */
    if( !SCCB_WaitAck() )
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    SCCB_SendByte((uint8_t)(WriteAddress & 0x00FF));   /* ���õ���ʼ��ַ */      
    SCCB_WaitAck();	
    SCCB_SendByte(SendByte);
    SCCB_WaitAck();   
    SCCB_Stop(); 
    return ENABLE;
}



/**
 * @brief	���豸��ȡһ���ֽ�����
 * @param 	pBuffer ��Ŷ������� 	
			length ����������	
			ReadAddress ��������ַ		 
			DeviceAddress ��������
 * @retval 	1 �ɹ�����
			0 ʧ��
 */
int SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)
{	
    if(!SCCB_Start())
	{
	    return DISABLE;
	}
    SCCB_SendByte( DEV_ADR );         /* ������ַ */
    if( !SCCB_WaitAck() )
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    SCCB_SendByte( ReadAddress );     /* ���õ���ʼ��ַ */      
    SCCB_WaitAck();	
    SCCB_Stop(); 
	
    if(!SCCB_Start())
	{
		return DISABLE;
	}
    SCCB_SendByte( DEV_ADR + 1 );     /* ������ַ */ 
    if(!SCCB_WaitAck())
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    while(length)
    {
      *pBuffer = SCCB_ReceiveByte();
      if(length == 1)
	  {
		  SCCB_NoAck();
	  }
      else
	  {
		SCCB_Ack(); 
	  }
      pBuffer++;
      length--;
    }
    SCCB_Stop();
    return ENABLE;
}

