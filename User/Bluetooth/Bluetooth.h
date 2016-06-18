#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include <stdio.h>
#include "usart.h"
#include "delay.h"

#define BLUETOOTH_DEBUG_ON 0

#if BLUETOOTH_DEBUG_ON
	#define BLUETOOTH_DEBUG(fmt,args...) printf (fmt ,##args)
#else
	#define BLUETOOTH_DEBUG(fmt,args...)
#endif

/**
 * @brief  �����źŴ�����
 * @param  
 * @retval 1��ʾ����������0��ʾ�󴥷�
 */
int CrashFunction(void);

#endif
