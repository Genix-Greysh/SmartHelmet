#include "spi.h"

/**
* @brief 用于初始化SPI的结构体。由于需要动态更改SPI的通信速率，需要设置为全局的
 */
SPI_InitTypeDef SPI_InitStructure;


/**
 * @brief  初始化SPI0
 * @param  
 * @retval None
 */
void SPI0_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};

	/* Enable PA & SPI0 & AFIO */
	CKCUClock.Bit.PA         = 1;
	CKCUClock.Bit.SPI0       = 1;
	CKCUClock.Bit.AFIO       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	/* SPI0_SEL idle state is HIGH */
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_8, GPIO_PR_UP);

	/* Configure related IO to SPI0 */
	AFIO_GPAConfig(AFIO_PIN_4 | AFIO_PIN_5 | AFIO_PIN_6 | AFIO_PIN_7, AFIO_MODE_5);

	/* SPI0 configuration */
	SPI_InitStructure.SPI_Mode = SPI_MASTER;
	SPI_InitStructure.SPI_FIFO = SPI_FIFO_DISABLE;
	SPI_InitStructure.SPI_DataLength = SPI_DATALENGTH_8;
	SPI_InitStructure.SPI_SELMode = SPI_SEL_SOFTWARE;
	SPI_InitStructure.SPI_SELPolarity = SPI_SELPOLARITY_LOW;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_FIRST;
	SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
	SPI_InitStructure.SPI_RxFIFOTriggerLevel = 0;
	SPI_InitStructure.SPI_TxFIFOTriggerLevel = 0;
	SPI_InitStructure.SPI_ClockPrescaler = 4;
	SPI_Init(HT_SPI0, &SPI_InitStructure);

	/* Set SEL as output mode for slave select */
	SPI_SELOutputCmd(HT_SPI0, ENABLE);

	/* Enable SPI0 */
	SPI_Cmd(HT_SPI0, ENABLE);
}


/**
 * @brief  通过SPI0发送、读取一个字节
 * @param  txData 要发送的字节
 * @retval 读取的字节
 */
u8 SPI0_ReadWriteByte(u8 txData)
{		
	u8 timeout = 200;			
	
	/* 检查SPI是否可发送 */
	while (!SPI_GetFlagStatus(HT_SPI0, SPI_FLAG_TXBE))
	{
		if(--timeout == 0)
			return 0;
	}	
	
	/* 通过SPI0发送一个字节 */
	SPI_SendData(HT_SPI0, (u32)txData); 
	
	timeout = 200;

	/* 检查SPI是否可接收 */
	while (!SPI_GetFlagStatus(HT_SPI0, SPI_FLAG_RXBNE))
	{
		if(--timeout == 0)
			return 0;
	}	
	
	/* 返回通过SPI0接收的字节 */
	return SPI_ReceiveData(HT_SPI0);  			    
}


/**
 * @brief  设置SPI0的通信速率
 * @param  SpeedSet 分频值，可取如下：
				SPI_BaudRatePrescaler_2   2   	(SPI 36M@sys 72M)
				SPI_BaudRatePrescaler_8   8  	(SPI 9M@sys 72M)
				SPI_BaudRatePrescaler_16  16 	(SPI 4.5M@sys 72M)
				SPI_BaudRatePrescaler_256 256 	(SPI 281.25K@sys 72M)
 * @retval None
 */
void SPI0_SetSpeed(u8 speedSet)
{
	/* 配置SPI0的时钟分频 */
	SPI_InitStructure.SPI_ClockPrescaler = speedSet;
	SPI_Init(HT_SPI0, &SPI_InitStructure);
	
	/* 使能 SPI0 */
	SPI_Cmd(HT_SPI0, ENABLE);
}

