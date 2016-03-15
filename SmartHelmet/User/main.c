/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"

#include "usart.h"
#include "spi.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup GPIO_Examples GPIO
  * @{
  */

/** @addtogroup InputOutput
  * @{
  */


/* Global functions ----------------------------------------------------------------------------------------*/
int main(void)
{
	int input;
	int senddat = 0;
	int receivedat;
	
	/* Initialize devices */
	InitUSART0();
	InitSPI0();
	
	
	/* main loop */                                                
	while (1)
	{
		SPI0_SENDDATA(senddat);
		printf("Sent %d\r\n", senddat);

		SPI0_GETDATA(receivedat);
		printf("Receive %d\r\n", receivedat);
		
		if(255 == senddat++)
			senddat = 0;
	}	

}

#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
  */

  while (1)
  {
  }
}
#endif


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
