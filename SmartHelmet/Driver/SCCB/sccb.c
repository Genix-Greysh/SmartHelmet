#include "sccb.h"

/**
 * @brief	SCCB引脚配置。选用PD7(SCCB_SCL)与PD8(SCCB_SDA)
 * @param	
 * @return	None
 */
void SCCB_GPIO_Config(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
	/* Enable PD */
	CKCUClock.Bit.PD         = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_7 | GPIO_PIN_8, GPIO_DIR_OUT);
	
}
