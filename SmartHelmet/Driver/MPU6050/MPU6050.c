#include "MPU6050.h"
float X_Axis ;
float Y_Axis ;
float Z_Axis ;
u8 MPU_Data [] ;

/**
 * @brief  PDMA的初始化。
 * @param  
 * @retval None
 */
void PDMA_Configuration(void)
{
	PDMACH_InitTypeDef PDMACH_InitStructure;	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
	//Config Clock
	CKCUClock.Bit.PDMA       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	//Config USART
	USART_RxPDMACmd(HT_USART0, ENABLE);

	//PDMA

	PDMACH_InitStructure.PDMACH_SrcAddr   = (u32) HT_USART0_BASE;
	PDMACH_InitStructure.PDMACH_DstAddr   = (u32) &MPU_Data;

	PDMACH_InitStructure.PDMACH_AdrMod    = SRC_ADR_FIX | DST_ADR_LIN_INC|AUTO_RELOAD;
	PDMACH_InitStructure.PDMACH_Priority  = H_PRIO;
	PDMACH_InitStructure.PDMACH_BlkCnt    = 33;
	PDMACH_InitStructure.PDMACH_BlkLen    = 1;
	PDMACH_InitStructure.PDMACH_DataSize  = WIDTH_8BIT;

	PDMA_Config(PDMA_CH2, &PDMACH_InitStructure);  
	PDMA_IntConig(PDMA_CH2, (PDMA_INT_GE | PDMA_INT_TC), ENABLE);
	PDMA_EnaCmd(PDMA_CH2, ENABLE);
	PDMA_SwTrigCmd(PDMA_CH2, ENABLE);
}




/**
 * @brief  得出三个方向轴的加速度
 * @param  
 * @retval None
 */
void Axis_DataTransfrom()
{
	int i = 0;
	for(i = 0 ; i <3 ; i++)
	{			
		if(MPU_Data[i*11+1] == 0x51)
		{
			X_Axis = ((short)(MPU_Data[3]<<8 | MPU_Data[2]))/32768.0*16; 
			Y_Axis = ((short)(MPU_Data[5]<<8 | MPU_Data[4]))/32768.0*16;      //Y轴加速度
      Z_Axis = ((short)(MPU_Data[7]<<8 | MPU_Data[6]))/32768.0*16;      //Z轴加速度		
		}
	}
	return;
}




