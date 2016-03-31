#include "sccb.h"


/**
 * @brief	SCCB引脚配置。选用PD4(SCCB_SCL)与PD5(SCCB_SDA)
 * @param	
 * @return	None
 */
void SCCB_GPIO_Config(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
	/* Enable PD */
	CKCUClock.Bit.PD         = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	/* 配置PD4、PD5为输出引脚 */
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4 | GPIO_PIN_5, GPIO_DIR_OUT);
	
	/* 使能PD4、PD5输入控制 */
	GPIO_InputConfig(HT_GPIOD, GPIO_PIN_4 | GPIO_PIN_5, ENABLE);	
}


/**
 * @brief	供SCCB总线延时使用
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
 * @brief	SCCB起始信号
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
		return DISABLE;	/* SDA线为低电平则总线忙,退出 */
	
	SDA_L;
	SCCB_delay();
	
	if(SDA_read) 
		return DISABLE;	/* SDA线为高电平则总线出错,退出 */
	
	SCL_L;
	SCCB_delay();
	
	return ENABLE;
}


/**
 * @brief	SCCB停止信号
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
 * @brief	SCCB应答方式
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
 * @brief	SCCB 无应答方式
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
 * @brief	SCCB 等待应答
 * @param 	 
 * @retval 	返回为:=1有ACK,=0无ACK
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
 * @brief	在SCCB上以MSB格式发送一个字节
 * @param 	SendByte 发送的数据
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
 * @brief	在SCCB上以MSB格式接收一个字节
 * @param 	
 * @retval 	SCCB总线返回的数据
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
 * @brief	向设备写一字节数据 * @param 	WriteAddress 待写入地址 	
			SendByte 待写入数据
			DeviceAddress: 器件类型
 * @retval 	1 成功写入
			0 失败
 */
int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte )
{		
    if(!SCCB_Start())
	{
	    return DISABLE;
	}
    SCCB_SendByte( DEV_ADR );                    /* 器件地址 */
    if( !SCCB_WaitAck() )
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    SCCB_SendByte((uint8_t)(WriteAddress & 0x00FF));   /* 设置低起始地址 */      
    SCCB_WaitAck();	
    SCCB_SendByte(SendByte);
    SCCB_WaitAck();   
    SCCB_Stop(); 
    return ENABLE;
}



/**
 * @brief	向设备读取一定字节数据
 * @param 	pBuffer 存放读出数据 	
			length 待读出长度	
			ReadAddress 待读出地址		 
			DeviceAddress 器件类型
 * @retval 	1 成功读入
			0 失败
 */
int SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)
{	
    if(!SCCB_Start())
	{
	    return DISABLE;
	}
    SCCB_SendByte( DEV_ADR );         /* 器件地址 */
    if( !SCCB_WaitAck() )
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    SCCB_SendByte( ReadAddress );     /* 设置低起始地址 */      
    SCCB_WaitAck();	
    SCCB_Stop(); 
	
    if(!SCCB_Start())
	{
		return DISABLE;
	}
    SCCB_SendByte( DEV_ADR + 1 );     /* 器件地址 */ 
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

