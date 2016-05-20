#ifndef _PWCRU_H
#define _PWCRU_H
/**
* @brief 初始化电源管理模块,并有开启睡眠模式的接口
 */


/**
 * @brief  初始化电源管理模块
 * @param  
 * @retval None
 */
void pwrcu_init(void);

/**
 * @brief  进入睡眠模式
 * @param  
 * @retval None
 */
void Enter_DeepSleepMode(void);

#endif 
