#include "usart.h"

/**
 * @brief  ³õÊ¼»¯´®¿Ú0	
 * @param  
 * @retval None
 */
void InitUSART0(u32 baudRate)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	USART_InitTypeDef USART_InitStructure;

	/* Enable peripheral clock of AFIO, USART0                */
	CKCUClock.Bit.AFIO   = 1;
	CKCUClock.Bit.USART0 = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	/* Config AFIO mode as USART0_Rx and USART0_Tx function.  */
	AFIO_GPAConfig(AFIO_PIN_2 | AFIO_PIN_3, AFIO_MODE_6);     // TX (PA2), RX (PA3)

	/* USART0 configuration ----------------------------------*/
	/* USART0 configured as follow:
		- BaudRate = 'baudRate' baud
		- Word Length = 8 Bits
		- One Stop Bit
		- None parity bit
	*/
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

	USART_Init(HT_USART0, &USART_InitStructure);
	USART_TxCmd(HT_USART0, ENABLE);
	USART_RxCmd(HT_USART0, ENABLE);
}
