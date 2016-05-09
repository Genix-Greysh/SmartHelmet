#include "delay.h"

/* �����Ҫ��ʱ�ĺ���ֵ����ÿһ�εĵδ��ж�ʱֵ������һ */
static vu32 DelayTime;

/**
 * @brief  ����̬����DelayTime��ֵ����һ��
 * @param  
 * @retval 	None
 * @notes	�ú�������ht32f165x_it.c�µ�SysTick_Handler�������á�
 */
void TimingDelay(void)
{
  if(DelayTime != 0)
  {
    DelayTime--;
  }
}


/**
 * @brief	����SYSTICK����������ʱ
 * @param  
 * @retval 	None
 */
void SYSTICK_Config(void)
{
	/* SYSTICK configuration */
	SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK); // Default : 9MHz on chip
	SYSTICK_SetReloadValue(9000);                 // (9k/9MHz) = 1ms on chip
	SYSTICK_IntConfig(ENABLE);
}


/**
 * @brief	��ʱָ����mSec����ֵ
 * @param  	mSec  ��Ҫ��ʱ�ĺ���ֵ
 * @retval 	None
 */
void delay_ms(u32 mSec)
{
    /* Enable the SysTick Counter */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);

    DelayTime = mSec;

    while(DelayTime != 0);

    /* Disable SysTick Counter */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
    /* Clear SysTick Counter */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}
