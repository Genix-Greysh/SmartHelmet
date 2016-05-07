#include "delay.h"

/* 存放需要延时的毫秒值，在每一次的滴答中断时值被减少一 */
static vu32 DelayTime;

/**
 * @brief  将静态变量DelayTime的值减少一。
 * @param  
 * @retval 	None
 * @notes	该函数将被ht32f165x_it.c下的SysTick_Handler函数调用。
 */
void TimingDelay(void)
{
  if(DelayTime != 0)
  {
    DelayTime--;
  }
}


/**
 * @brief	配置SYSTICK以适用于延时
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
 * @brief	延时指定的mSec毫秒值
 * @param  	mSec  需要延时的毫秒值
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
