#include "spi.h"

/**
 * @brief  初始化SPI0
 * @param  
 * @retval None
 */
void InitSPI0(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	SPI_InitTypeDef SPI_InitStructure;

	/* Enable SPI0 & AFIO */
	CKCUClock.Bit.PA         = 1;
	CKCUClock.Bit.SPI0       = 1;
	CKCUClock.Bit.AFIO       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	/* SPI0_SEL idle state is HIGH */
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_8, GPIO_PR_UP);

	/* Configure related IO to SPI0 */
	AFIO_GPAConfig(AFIO_PIN_5 | AFIO_PIN_6 | AFIO_PIN_7 | AFIO_PIN_8, AFIO_MODE_5);

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


