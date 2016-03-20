#include "sd.h"

u8 SD_Type = 0;	/* 存放SD卡的类型 */



/**
 * @brief  将SPI设置为低速模式
 * @param  
 * @retval None
 */
static void SD_SPI_SetLowSpeed(void)
{
 	SPI0_SetSpeed(SPI_BaudRatePrescaler_256);	//设置到低速模式	
}


/**
 * @brief  将SPI设置为高速模式
 * @param  
 * @retval None
 */
static void SD_SPI_SetHighSpeed(void)
{
 	SPI0_SetSpeed(SPI_BaudRatePrescaler_8);	//设置到高速模式	
}


/**
 * @brief	等待SD卡准备好
 * @param  
 * @retval 	1 已准备
			0 等待超时
 */
static u8 SD_WaitReady(void)
{
	u32 t = 0;
	
	while(++t < 0XFFFFFF)	//等待 
	{
		if(SD_SPI_ReadWriteByte(SD_DUMMY_BYTE) == SD_DUMMY_BYTE)	//总线空闲		
			return 1;		  	
	}	
	
	return 0;
}


/**
 * @brief	取消片选中SD卡
 * @param  
 * @retval 	None
 */
void SD_DisSelect(void)
{
	/* 取消片选设备 */
	SPI_SoftwareSELCmd(HT_SPI0, SPI_SEL_INACTIVE);
	
	//提供额外的8个时钟
 	SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);	
}


/**
 * @brief	片选中SD卡，使其工作
 * @param  
 * @retval 	1 选中成功
			0 选中失败
 */
u8 SD_Select(void)
{	
	/* 片选设备 */
	SPI_SoftwareSELCmd(HT_SPI0, SPI_SEL_ACTIVE);
	
	if(SD_WaitReady())	//等待成功
		return 1;	
	else				//等待失败
	{
		SD_DisSelect();
		return 0;		
	}
}


/**
 * @brief  	初始化SD相关的引脚和设备。SD的片选接到PE12
 * @param  
 * @retval 	None
 */
void SD_SPI_Init(void)
{
	/* 初始化SPI0 */
	SPI0_Init();
}
	

/**
 * @brief  	向SD卡发送一个命令
 * @param	cmd 命令 
			arg 命令参数
			crc crc校验值
 * @retval 	SD卡返回的响应
 */
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;	
	u8 retry = 0; 
	
	SD_DisSelect();		//取消上次片选
	if(!SD_Select())	//重新片选
		return SD_DUMMY_BYTE;	//片选失败
	
	//打包命令、参数以及校验和
	
    SD_SPI_ReadWriteByte(cmd | 0x40);	//8位命令，最高位恒为1
    SD_SPI_ReadWriteByte(arg >> 24);	//将32位的参数拆分成4字节，逐一发送
    SD_SPI_ReadWriteByte(arg >> 16);
    SD_SPI_ReadWriteByte(arg >> 8);
    SD_SPI_ReadWriteByte(arg);	  
    SD_SPI_ReadWriteByte(crc); 			//8位校验和
	
	if(cmd == CMD12)	//对于停止命令，加发空闲数据
		SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);	
	
    //等待响应，或超时退出
	retry = 0X1F;
	do
	{
		r1 = SD_SPI_ReadWriteByte(0xFF);
	}while((r1 & 0X80) && retry--);	 //r1最高位若为1则不是响应，继续等待

    return r1;
}


/**
 *  @name SD_GetResponse
 *	@description 等待SD卡回应
 *	@param response 要获得的回应
 *	@return	SD_RESPONSE_NO_ERROR 成功得到回应
			SD_RESPONSE_FAILURE 等待回应超时失败或回应不匹配
 *  @notice
 */
u8 SD_GetResponse(u8 response)
{
	u16 Count = 0xFFFF;//等待次数	   						  
	
	while ((SD_SPI_ReadWriteByte(SD_DUMMY_BYTE) != response) && Count)
		Count--;	//等待得到准确的回应  	  
	if(Count == 0)
		return SD_RESPONSE_FAILURE;		//得到回应失败   
	else 
		return SD_RESPONSE_NO_ERROR;	//正确回应
}


/**
 *  @name SD_RecvData
 *	@description 从sd卡读取一定长度的内容，保存到缓冲区buf中
 *	@param 	buf 数据缓存区
			len 要读取的数据长度
 *	@return 1 读取成功
			0 读取失败	
 *  @notice
 */
u8 SD_RecvData(u8 *buf,u16 len)
{	
	u8 tmp;
	
	//等待SD卡发回数据起始令牌
	if(SD_GetResponse(SD_BEGIN_END_FLAG) != SD_RESPONSE_NO_ERROR)
		return 0;
	
	//开始接收数据
    while(len--)
    {
        tmp = SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);
      
		/* 由于合泰的板只支持256B大小的数组，故只能读取256B字节的数据到buf */
//		if(256 < len)
			*buf++ = tmp;
//		else
//			return 1;
    }
	
    //下面是2个伪CRC（dummy CRC）
    SD_SPI_ReadWriteByte(0xFF);
    SD_SPI_ReadWriteByte(0xFF);		
	
    return 1;//读取成功
}



/**
 *  @name SD_SendBlock
 *	@description 向sd卡写入一个数据包的内容，共512字节
 *	@param 	buf 数据缓存区
			cmd 指令
 *	@return 1 发送成功
			0 发送失败	
 *  @notice
 */
u8 SD_SendBlock(u8 *buf, u8 cmd)
{	
	u16 cnt;
	u8 r1;
	
	if(!SD_WaitReady())
		return 0;	//等待准备失效
	
	SD_SPI_ReadWriteByte(cmd);
	
	if(cmd != 0XFD)//不是结束指令
	{
		for(cnt = 0;cnt < 512;cnt++)
			SD_SPI_ReadWriteByte(buf[cnt]);
	    SD_SPI_ReadWriteByte(0xFF);//忽略crc
	    SD_SPI_ReadWriteByte(0xFF);
		r1 = SD_SPI_ReadWriteByte(0xFF);//接收响应
		if((r1 & 0x1F) != 0x05)
			return 0;	//响应错误									  					    
	}						 									  					    
    return 1;	//写入成功
}



/**
 * @brief  	初始化SD，使其进入工作模式
 * @param  
 * @retval 	0 卡错误
 */
u8 SD_Init(void)
{
	int i;
	u8 r1;		//存放命令响应
	u16 retry;	//存放超时计数
	u8 buf[4];	//临时缓冲区

	
	/* SD卡初始化阶段，CLK时钟最大不能超过400KHz */
	SD_SPI_SetLowSpeed();
	
	/* 上电延时至少74个时钟周期 */
	for(i = 0; i < 10; ++i)
		SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);
	retry = 20;
	while((r1 = SD_SendCmd(CMD0, 0, 0x95)) != 0x01 && retry-- > 0)	//进入IDLE状态，等待正确返回
		;
	if(r1 == 0x01)	//进入IDLE状态成功
	{
		//根据SD卡工作电压区分SD卡种类
		if(SD_SendCmd(CMD8, 0x1AA, 0x87) == 1)		//SD V2.0
		{
			for(i = 0;i < 4;i++)
				buf[i] = SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);	//Get trailing return value of R7 resp
			if(buf[2] == 0X01 && buf[3] == 0XAA)	//卡是否支持2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SD_SendCmd(CMD55, 0, 0X01);	//发送CMD55
					r1 = SD_SendCmd(CMD41, 0x40000000, 0X01);	//发送CMD41
				}while(r1 && retry--);
				if(retry && SD_SendCmd(CMD58, 0, 0X01) == 0)	//鉴别SD2.0卡版本开始
				{
					for(i = 0;i < 4;i++)
						buf[i] = SD_SPI_ReadWriteByte(0XFF);//得到OCR值
					if(buf[0] & 0x40)	//检查CCS
						SD_Type = SD_TYPE_V2HC;    
					else 
						SD_Type = SD_TYPE_V2;   
				}
			}
		}
		else	//SD V1.x/ MMC	V3
		{
			SD_SendCmd(CMD55, 0, 0X01);		//发送CMD55
			r1 = SD_SendCmd(CMD41, 0, 0X01);	//发送CMD41
			if(r1 <= 1)
			{		
				SD_Type = SD_TYPE_V1;
				retry = 0XFFFE;
				do //等待退出IDLE模式
				{
					SD_SendCmd(CMD55, 0, 0X01);	//发送CMD55
					r1 = SD_SendCmd(CMD41, 0, 0X01);//发送CMD41
				}while(r1 && retry--);
			}
			else	//MMC卡不支持CMD55+CMD41识别
			{
				SD_Type = SD_TYPE_MMC;//MMC V3
				retry = 0XFFFE;
				do //等待退出IDLE模式
				{											    
					r1 = SD_SendCmd(CMD1,0,0X01);//发送CMD1
				}while(r1 && retry--);  
			}
			if(retry == 0 || SD_SendCmd(CMD16, 512, 0X01) != 0)
				SD_Type = SD_TYPE_ERR;//错误的卡
		}
	}

	SD_DisSelect();			//取消片选
	SD_SPI_SetHighSpeed();	//正常传输设置为高速
	
	if(SD_Type)	//卡错误，返回0
		return 0;
	else if(r1)
		return r1; 
	else
		return 0xaa;	//其他错误	
}



/**
 *  @name SD_ReadDisk
 *	@description 读SD卡扇区
 *	@param 	buf 数据缓冲区
			sector 扇区号
			cnt 要读取的扇区数目
 *	@return 操作结果，1 成功；其他，失败
 *  @notice
 */
u8 SD_ReadDisk(u8 *buf, u32 sector, u8 cnt)
{
	u8 r1;
	if(SD_Type != SD_TYPE_V2HC)
		sector <<= 9;//转换为字节地址
	if(cnt == 1)
	{
		r1 = SD_SendCmd(CMD17, sector, 0X01);//读命令
		if(r1 == 0)//指令发送成功
		{
			r1 = SD_RecvData(buf, 512);//接收512个字节	   
		}
	}
	else
	{
		r1 = SD_SendCmd(CMD18, sector, 0X01);//连续读命令
		do
		{
			r1 = SD_RecvData(buf, 512);//接收512个字节	 
			buf += 512;  
		}while(--cnt && r1 == 1); 	
		SD_SendCmd(CMD12, 0, 0X01);	//发送停止命令
	}   
	SD_DisSelect();//取消片选
	
	return r1;//
}


/**
 *  @name SD_WriteDisk
 *	@description 写SD卡扇区
 *	@param 	buf 数据缓冲区
			sector 扇区号
			cnt 要写入的扇区数目
 *	@return 操作结果，1 成功；其他，失败
 *  @notice
 */
u8 SD_WriteDisk(u8 *buf, u32 sector, u8 cnt)
{
	u8 r1;
	if(SD_Type != SD_TYPE_V2HC)
		sector *= 512;//转换为字节地址
	if(cnt == 1)
	{
		r1 = SD_SendCmd(CMD24,sector,0X01);//读命令
		if(r1 == 0)//指令发送成功
		{
			r1 = SD_SendBlock(buf,0xFE);//写512个字节	   
		}
	}
	else
	{
		if(SD_Type != SD_TYPE_MMC)
		{
			SD_SendCmd(CMD55,0,0X01);	
			SD_SendCmd(CMD23,cnt,0X01);//发送指令	
		}
 		r1 = SD_SendCmd(CMD25,sector,0X01);//连续读命令
		if(r1 == 0)
		{
			do
			{
				r1 = SD_SendBlock(buf,0xFC);//接收512个字节	 
				buf += 512;  
			}while(--cnt && r1 == 1);
			r1 = SD_SendBlock(0,0xFD);//接收512个字节 
		}
	}   
	SD_DisSelect();//取消片选
	return r1;//
}	


