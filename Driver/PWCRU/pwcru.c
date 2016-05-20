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
  /*���ñ�������*/   
	PWRCU_DeInit();
	/* ʹ��LSE OSC*/
//  RTC_LSESMConfig(RTC_LSESM_NORMAL);
//  RTC_LSECmd(ENABLE);
//	/* �ȴ�ֱ��LSE����                                                                               */
//  while (CKCU_GetClockReadyStatus(CKCU_FLAG_LSERDY) == RESET);
//  /* ѡ��LSE��ΪRTC��ʱ��Դ                                                                    */
//  RTC_ClockSourceConfig(RTC_SRC_LSE);
//  /* ����RTCÿ��������ʱ��Ϊ1s                                                                           */
//  RTC_SetPrescaler(RTC_RPRE_32768);
//  /* ���ñȽ�ֵ                                                                                      */
//  RTC_SetCompare(0xFFFFFFFF);
//  /* ʹ��RTC                                                                                        */
//  RTC_Cmd(ENABLE);
//  /* ʹ��RTC�Ƚϻ���ʱ��                                                             */
//  RTC_WakeupConfig(RTC_WAKEUP_CM, ENABLE);
	
	/* Configure Button pin as input floating                                                                 */
  HT32F_DVB_PBInit(BUTTON_KEY1, BUTTON_MODE_GPIO);  //PE-1
	/**�ⲿ�жϲ��ֵ��趨**/
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
