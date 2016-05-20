#include "pwcru.h"
#include "delay.h"

void pwrcu_init(void)
{
	/* Enable Backup Domain PCLK and waits until it can be accessed                                           */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.BKP        = 1;
	CKCUClock.Bit.EXTI       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	if (PWRCU_CheckReadyAccessed() != PWRCU_OK)
  {
    while (1);
  }
  /*重置备份区域*/   
	PWRCU_DeInit();
	/* 使能LSE OSC*/
//  RTC_LSESMConfig(RTC_LSESM_NORMAL);
//  RTC_LSECmd(ENABLE);
//	/* 等待直到LSE就绪                                                                               */
//  while (CKCU_GetClockReadyStatus(CKCU_FLAG_LSERDY) == RESET);
//  /* 选择LSE作为RTC的时钟源                                                                    */
//  RTC_ClockSourceConfig(RTC_SRC_LSE);
//  /* 设置RTC每次跳动的时间为1s                                                                           */
//  RTC_SetPrescaler(RTC_RPRE_32768);
//  /* 设置比较值                                                                                      */
//  RTC_SetCompare(0xFFFFFFFF);
//  /* 使能RTC                                                                                        */
//  RTC_Cmd(ENABLE);
//  /* 使能RTC比较唤醒时间                                                             */
//  RTC_WakeupConfig(RTC_WAKEUP_CM, ENABLE);
	
	/* Configure Button pin as input floating                                                                 */
  HT32F_DVB_PBInit(BUTTON_KEY1, BUTTON_MODE_GPIO);  //PE-1
	/**外部中断部分的设定**/
	/* Connect Button EXTI Channel to Button GPIO Pin                                                         */
  AFIO_EXTISourceConfig((AFIO_EXTI_CH_Enum)KEY1_BUTTON_EXTI_CHANNEL, (AFIO_ESS_Enum)KEY1_BUTTON_GPIO_ID);
	
	/* Enable and set EXTI Event Wakeup interrupt to the lowest priority                                      */
  NVIC_SetPriority(EVWUP_IRQn, 0x4);
  NVIC_EnableIRQ(EVWUP_IRQn);
  EXTI_WakeupEventIntConfig(ENABLE);
}

void Enter_DeepSleepMode(void)
{
	//printf("Enter Sleep Mode...\n");
	//delay_ms(500);
	/* Configure button EXTI Channel on low level                                                           */
	EXTI_WakeupEventConfig(KEY1_BUTTON_EXTI_CHANNEL, EXTI_WAKEUP_LOW_LEVEL, ENABLE);  //PE - 1  EXTI -1(channel)
	
	//Before entering the DeepSleep-Mode, do some configuration
	//Code
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_12, RESET);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_8, RESET);
	
	/* Request to enter Deep Sleep mode 1 (The LDO in low power mode)                                       */
	PWRCU_DeepSleep1(PWRCU_SLEEP_ENTRY_WFI);
	
	//the MCU Wakes up, and recovers the configuration
	
}
