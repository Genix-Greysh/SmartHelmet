/*********************************************************************************************************//**
 * @file    GPIO/InputOutput/ht32f165x_it.c
 * @version $Rev:: 929          $
 * @date    $Date:: 2015-09-16 #$
 * @brief   This file provides all interrupt service routine.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ov7725.h"
#include "ht32_board.h"
#include "delay.h"


/* Macro define -------------------------------------------*/
#define DEBUFG_ON 0

#if DEBUFG_ON
	#define DEBUG(fmt,args...) printf(fmt ,##args)
#else
	#define DEBUG(fmt,args...) 
#endif

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Memory Manage exception.
 * @retval  None
 ************************************************************************************************************/
void MemManage_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Bus Fault exception.
 * @retval  None
 ************************************************************************************************************/
void BusFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Usage Fault exception.
 * @retval  None
 ************************************************************************************************************/
void UsageFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Debug Monitor exception.
 * @retval  None
 ************************************************************************************************************/
void DebugMon_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
	extern void TimingDelay(void);
	TimingDelay();
}


/*********************************************************************************************************//**
 * @brief   This function handles RTC interrupt.
 * @retval  None
 * @details In RTC interrupt service routine:
 *  - Reset RTC init time when Time is 23:59:59.
 *    - Set RTC Compare register as 86400.
 *    - Reset PWRCU_BAKREG_1 to 0.
 *  - Toggle LED1 each 1s.
 *  - Set gwTimeDisplay to 1 to enable time update.
 *
 ************************************************************************************************************/
//void RTC_IRQHandler(void)
//{
//  extern vu32 gwTimeDisplay;
//  u8 bFlags;

//  bFlags = RTC_GetFlagStatus();
//  if((bFlags & 0x2) != 0x0) /* Match flag */
//  {
//    /* Reset RTC init time when Time is 23:59:59 */
//    RTC_SetCompare(86400);
//    PWRCU_WriteBackupRegister(PWRCU_BAKREG_1, 0);
//  }

//  /* Enable time update */
//  gwTimeDisplay = 1;
//}

void EXTI1_IRQHandler(void)
{
	EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
}


void EVWUP_IRQHandler(void)
{
  if (EXTI_GetWakeupFlagStatus(KEY1_BUTTON_EXTI_CHANNEL) )
  {
    /* Disable button EXTI Channel wakeup event to avoid re-entry                                           */
		EXTI_WakeupEventConfig(KEY1_BUTTON_EXTI_CHANNEL, EXTI_WAKEUP_LOW_LEVEL, DISABLE);
		/* Clear the Key Button EXTI channel event wakeup flag                                                  */
		EXTI_ClearWakeupFlag(KEY1_BUTTON_EXTI_CHANNEL);

//		GPIO_ClearOutBits(HT_GPIOC,GPIO_PIN_11);
		delay_ms(500);
		//printf("WAKEUPBOTTON...\n");
  }
}



/*********************************************************************************************************//** 
 * @brief   This function handles EXTI7 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI7_IRQHandler(void)
{
	extern volatile uint8_t Ov7725_vsync;
	
	/* 检查 EXTI_Line7线路上的中断请求是否发送到了NVIC */
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_7, EXTI_NEGATIVE_EDGE) != RESET)
	{
        if( Ov7725_vsync == 0 )
        {
            FIFO_WRST_L(); 	                      //拉低使FIFO写(数据from摄像头)指针复位
            FIFO_WE_H();	                      //拉高使FIFO写允许
            
            Ov7725_vsync = 1;	   	
            FIFO_WE_H();                          //使FIFO写允许
            FIFO_WRST_H();                        //允许使FIFO写(数据from摄像头)指针运动
		}
        else if( Ov7725_vsync == 1 )
        {
            FIFO_WE_L();                          //拉低使FIFO写暂停
            Ov7725_vsync = 2;
			DEBUG("Picture detected.\r\n");
        }
	}
	
	/* Clear the specified EXTI channel edge flag */
	EXTI_ClearEdgeFlag(EXTI_CHANNEL_7);
}
