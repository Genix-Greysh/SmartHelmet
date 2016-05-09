#include "usart.h"

/**
 * @brief  ³õÊ¼»¯´®¿Ú	
 * @param  
 * @retval None
 */
void Init_USART(HT_USART_TypeDef * usart , u32 baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
	/* Enable peripheral clock of AFIO, USART                */
	CKCUClock.Bit.AFIO   = 1;
	
	if(usart == HT_USART0)
		CKCUClock.Bit.USART0 = 1;
	else if (usart == HT_USART1)
		CKCUClock.Bit.USART1 = 1;
	else 
		;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	
	/* Config AFIO mode as USART0_Rx and USART0_Tx function.  */
	if(usart == HT_USART0)
		AFIO_GPAConfig(AFIO_PIN_2 | AFIO_PIN_3, AFIO_MODE_6);     // TX (PA2), RX (PA3)
	else if (usart == HT_USART1)
		AFIO_GPAConfig(AFIO_PIN_4 | AFIO_PIN_5, AFIO_MODE_6);     // TX (PA4), RX (PA5)
	else 
		;
	

	/* USART configuration ----------------------------------*/
	/* USART configured as follow:
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

	USART_Init(usart, &USART_InitStructure);
	USART_TxCmd(usart, ENABLE);
	USART_RxCmd(usart, ENABLE);
}
