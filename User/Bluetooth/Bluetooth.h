#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include <stdio.h>
#include "usart.h"
#include "delay.h"

#define BLUETOOTH_DEBUG_ON 1

#if BLUETOOTH_DEBUG_ON
	#define BLUETOOTH_DEBUG(fmt,args...) printf (fmt ,##args)
#else
	#define BLUETOOTH_DEBUG(fmt,args...)
#endif

/**
 * @brief  车祸信号处理函数
 * @param  
 * @retval None
 */
void CrashFunction(void);

#endif
