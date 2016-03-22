#include "rtc.h"

vu32 gwTimeDisplay = 0;



/*********************************************************************************************************//**
 * @brief Configures the RTC.
 * @retval None
 * @details RTC configuration as following:
 *  - S/W reset backup domain.
 *  - Configure Low Speed External clock (LSE).
 *    - The LSE use fast startup mode.
 *      Shortly startup time but higher power consumption.
 *    - Enable the LSE.
 *    - Waits till LSE is ready.
 *  - Configure RTC.
 *    - Select LSE as RTC Clock Source.
 *    - Enable the RTC Second interrupt.
 *    - RTC prescaler = 32768 to generate 1 second interrupt.
 *    - Enable clear RTC counter by match function.
 *
 ************************************************************************************************************/
void RTC_Configuration(void)
{
	/* Reset Backup Domain                                                                                    */
	PWRCU_DeInit();

	/* Configure Low Speed External clock (LSE)                                                               */
	RTC_LSESMConfig(RTC_LSESM_FAST);
	RTC_LSECmd(ENABLE);
	while (CKCU_GetClockReadyStatus(CKCU_FLAG_LSERDY) == RESET);

	/* Configure the RTC                                                                                      */
	RTC_ClockSourceConfig(RTC_SRC_LSE);
	RTC_IntConfig(RTC_INT_CSEC, ENABLE);
	RTC_SetPrescaler(RTC_RPRE_32768);

	/* Restart counter when match event occurred                                                              */
	RTC_CMPCLRCmd(ENABLE);
}


/*********************************************************************************************************//**
  * @brief  Scan an unsigned decimal number.
  * @return An unsigned decimal value.
  ***********************************************************************************************************/
u32 ScanUnsignedDecimal(void)
{
  u8 bChar = 0;
  u32 wNbr = 0;
  while (1)
  {
    bChar = getchar();
    if ((bChar >= '0') && (bChar <= '9'))
    {
      wNbr = (wNbr * 10) + (bChar - '0');
    }
    else
    {
      break;
    }

  }
  return wNbr;
}


/*********************************************************************************************************//**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @return Enter time, unit is second within a day.
  ***********************************************************************************************************/
u32 Time_Regulate(void)
{
  u32 Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

  printf("\r\n==============Time Settings=====================================");
  printf("\r\n  Please Set Hours");
  printf("\r\n");
  do
  {
    Tmp_HH = ScanUnsignedDecimal();
  } while (Tmp_HH > 23);
  printf(":  %u", (unsigned int)Tmp_HH);

  printf("\r\n  Please Set Minutes");
  printf("\r\n");
  do
  {
    Tmp_MM = ScanUnsignedDecimal();
  } while (Tmp_MM > 59);
  printf(":  %u", (unsigned int)Tmp_MM);

  printf("\r\n  Please Set Seconds");
  printf("\r\n");
  do
  {
    Tmp_SS = ScanUnsignedDecimal();
  } while (Tmp_SS > 59);
  printf(":  %u", (unsigned int)Tmp_SS);

  return((Tmp_HH * 3600 + Tmp_MM * 60 + Tmp_SS));
}


/**
 * @brief	通过串口0交互式配置RTC
 * @param  
 * @return	None
 */
void RTC_ConfigOnline(void)
{
	/* Enable Backup Domain PCLK and waits until it can be accessed                                           */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.BKP        = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	if (PWRCU_CheckReadyAccessed() != PWRCU_OK)
	{
		while (1);
	}
	
	/* Enable NVIC RTC interrupt                                                                              */
	NVIC_EnableIRQ(RTC_IRQn);
	
	/* Check if the Power On Reset flag is set                                                                */
	if (PWRCU_GetFlagStatus() == PWRCU_FLAG_BAKPOR)
	{
		RTC_DEBUG("\r\n\n Power On Reset occurred....");
	}
	
	if (PWRCU_ReadBackupRegister(PWRCU_BAKREG_0) != 0xA55A)
	{
		u32 wInitTime = 0;
		/* Backup data register value is not correct or not yet programmed (when
			the first time the program is executed) */

		RTC_DEBUG("\r\n\n RTC not yet configured....");

		/* RTC Configuration                                                                                    */
		RTC_Configuration();

		RTC_DEBUG("\r\n RTC configured....");

		/* Adjust time by values entered by the user on the hyperterminal,
		Then store the init time to PWRCU_BAKREG_1. */
		wInitTime = Time_Regulate();
		PWRCU_WriteBackupRegister(PWRCU_BAKREG_1, wInitTime);

		/* Reset RTC Counter when Time is 23:59:59                                                              */
		RTC_SetCompare(86400 - wInitTime);

		PWRCU_WriteBackupRegister(PWRCU_BAKREG_0, 0xA55A);
		/* Enable RTC                                                                                           */
		RTC_Cmd(ENABLE);
	}
	else
	{
		RTC_DEBUG("\r\n No need to configure RTC....");
	}
}


/**
 * @brief	获取RTC计数值
 * @param  
 * @return	一个u32类型的计数值
 */
u32 RTC_getTime(void)
{
	return RTC_GetCounter();
}

/*********************************************************************************************************//**
  * @brief  Displays the input time.
  * @param  wTimeVar: Displays time.
  * @retval None
  ***********************************************************************************************************/
void Time_Display(u32 wTimeVar)
{
  u32 THH = 0, TMM = 0, TSS = 0;

  /* Compute  hours                                                                                         */
  THH = wTimeVar/3600;
  /* Compute minutes                                                                                        */
  TMM = (wTimeVar % 3600)/60;
  /* Compute seconds                                                                                        */
  TSS = (wTimeVar % 3600)% 60;

  printf("Time: %02u:%02u:%02u\r", (unsigned int)THH, (unsigned int)TMM, (unsigned int)TSS);
}
