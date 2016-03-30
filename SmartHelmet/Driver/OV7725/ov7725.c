/***************************************************************************************
 *  FileName                    :   ov7725.c
 *  CopyRight                   :	Zoisite
 *  ModuleName                  :   ov7725
 *
 *  CPU                         :	Intel core i5-4210U
 *  RTOS                        :	Windows 7 pro
 *
 *  Create Data                 :   2016/03/28
 *  Author/Corportation         :   Gaunthan
 *
 *  Abstract Description        :   封装OV7725摄像头的初始化、数据采集等功能
 *
 *--------------------------------Revision History--------------------------------------
 *  No      version     Data        Revised By      Item        Description
 *  1       v1.0        2016/3/28 	Gaunthan                    Create this file
 *
 ***************************************************************************************/


/**************************************************************
*   Debug switch Section
**************************************************************/


/**************************************************************
*   Include File Section
**************************************************************/
#include "ov7725.h"


/**************************************************************
*   Macro Define Section
**************************************************************/


/* debug调试宏定义，根据表达式a的真假执行has_bug或no_bug */
#define BUG_DETECT_PRINT(a,has_bug,no_bug) {\
	if(a) 						\
		printf("%s",has_bug); 	\
	else 						\
		printf("%s",no_bug);}


/**
 * @brief	调试信息输出宏定义
 */
#if OV7725_DEBUG_ON
	#define OV7725_DEBUG(fmt,args...) printf(fmt ,##args)
#else
	#define OV7725_DEBUG(fmt,args...)
#endif
		

/* OV7725寄存器宏定义 */
#define GAIN      0x00
#define BLUE      0x01
#define RED       0x02
#define GREEN     0x03
#define BAVG      0x05
#define GAVG      0x06
#define RAVG      0x07
#define AECH      0x08
#define COM2      0x09
#define PID       0x0A
#define VER       0x0B
#define COM3      0x0C
#define COM4      0x0D
#define COM5      0x0E
#define COM6      0x0F
#define AEC       0x10
#define CLKRC     0x11
#define COM7      0x12
#define COM8      0x13
#define COM9      0x14
#define COM10     0x15
#define REG16     0x16
#define HSTART    0x17
#define HSIZE     0x18
#define VSTRT     0x19
#define VSIZE     0x1A
#define PSHFT     0x1B
#define MIDH      0x1C
#define MIDL      0x1D
#define LAEC      0x1F
#define COM11     0x20
#define BDBase    0x22
#define BDMStep   0x23
#define AEW       0x24
#define AEB       0x25
#define VPT       0x26
#define REG28     0x28
#define HOutSize  0x29
#define EXHCH     0x2A
#define EXHCL     0x2B
#define VOutSize  0x2C
#define ADVFL     0x2D
#define ADVFH     0x2E
#define YAVE      0x2F
#define LumHTh    0x30
#define LumLTh    0x31
#define HREF      0x32
#define DM_LNL    0x33
#define DM_LNH    0x34
#define ADoff_B   0x35
#define ADoff_R   0x36
#define ADoff_Gb  0x37
#define ADoff_Gr  0x38
#define Off_B     0x39
#define Off_R     0x3A
#define Off_Gb    0x3B
#define Off_Gr    0x3C
#define COM12     0x3D
#define COM13     0x3E
#define COM14     0x3F
#define COM16     0x41
#define TGT_B     0x42
#define TGT_R     0x43
#define TGT_Gb    0x44
#define TGT_Gr    0x45
#define LC_CTR    0x46
#define LC_XC     0x47
#define LC_YC     0x48
#define LC_COEF   0x49
#define LC_RADI   0x4A
#define LC_COEFB  0x4B 
#define LC_COEFR  0x4C
#define FixGain   0x4D
#define AREF1     0x4F
#define AREF6     0x54
#define UFix      0x60
#define VFix      0x61
#define AWBb_blk  0x62
#define AWB_Ctrl0 0x63
#define DSP_Ctrl1 0x64
#define DSP_Ctrl2 0x65
#define DSP_Ctrl3 0x66
#define DSP_Ctrl4 0x67
#define AWB_bias  0x68
#define AWBCtrl1  0x69
#define AWBCtrl2  0x6A
#define AWBCtrl3  0x6B
#define AWBCtrl4  0x6C
#define AWBCtrl5  0x6D
#define AWBCtrl6  0x6E
#define AWBCtrl7  0x6F
#define AWBCtrl8  0x70
#define AWBCtrl9  0x71
#define AWBCtrl10 0x72
#define AWBCtrl11 0x73
#define AWBCtrl12 0x74
#define AWBCtrl13 0x75
#define AWBCtrl14 0x76
#define AWBCtrl15 0x77
#define AWBCtrl16 0x78
#define AWBCtrl17 0x79
#define AWBCtrl18 0x7A
#define AWBCtrl19 0x7B
#define AWBCtrl20 0x7C
#define AWBCtrl21 0x7D 
#define GAM1      0x7E
#define GAM2      0x7F
#define GAM3      0x80
#define GAM4      0x81
#define GAM5      0x82
#define GAM6      0x83
#define GAM7      0x84
#define GAM8      0x85
#define GAM9      0x86
#define GAM10     0x87
#define GAM11     0x88
#define GAM12     0x89
#define GAM13     0x8A
#define GAM14     0x8B
#define GAM15     0x8C
#define SLOP      0x8D
#define DNSTh     0x8E
#define EDGE0     0x8F
#define EDGE1     0x90
#define DNSOff    0x91
#define EDGE2     0x92
#define EDGE3     0x93
#define MTX1      0x94
#define MTX2      0x95
#define MTX3      0x96
#define MTX4      0x97
#define MTX5      0x98
#define MTX6      0x99
#define MTX_Ctrl  0x9A
#define BRIGHT    0x9B
#define CNST      0x9C
#define UVADJ0    0x9E
#define UVADJ1    0x9F
#define SCAL0     0xA0
#define SCAL1     0xA1
#define SCAL2     0xA2
#define SDE       0xA6
#define USAT      0xA7
#define VSAT      0xA8
#define HUECOS    0xA9
#define HUESIN    0xAA
#define SIGN      0xAB
#define DSPAuto   0xAC

/**************************************************************
*   Struct Define Section
**************************************************************/
/**
 * @brief	OV7725寄存器结构定义
 */
typedef struct tagReg
{
	uint8_t Address;			 	/*寄存器地址*/
	uint8_t Value;		           	/*寄存器值*/
}Reg_Info;


/**************************************************************
*   Prototype Declare Section
**************************************************************/


/**************************************************************
*   Global Variable Declare Section
**************************************************************/

/* 寄存器参数配置 */
Reg_Info Sensor_Config[] =
{
	{CLKRC,     0x00}, 		/*clock config*/
	{COM7,      0x46}, 		/*QVGA RGB565 */
	{HSTART,    0x3f},
	{HSIZE,     0x50},
	{VSTRT,     0x03},
	{VSIZE,     0x78},
	{HREF,      0x00},
	{HOutSize,  0x50},
	{VOutSize,  0x78},
	{EXHCH,     0x00},

	/*DSP control*/
	{TGT_B,     0x7f},
	{FixGain,   0x09},
	{AWB_Ctrl0, 0xe0},
	{DSP_Ctrl1, 0xff},
	{DSP_Ctrl2, 0x20},
	{DSP_Ctrl3,	0x00},
	{DSP_Ctrl4, 0x00},

	/*AGC AEC AWB*/
	{COM8,		  0xf0},
	{COM4,		  0x81}, 	/*Pll AEC CONFIG*/
	{COM6,		  0xc5},
	{COM9,		  0x21},
	{BDBase,	  0xFF},
	{BDMStep,	  0x01},
	{AEW,		  0x34},
	{AEB,		  0x3c},
	{VPT,		  0xa1},
	{EXHCL,		  0x00},
	{AWBCtrl3,    0xaa},
	{COM8,		  0xff},
	{AWBCtrl1,    0x5d},

	{EDGE1,		  0x0a},
	{DNSOff,	  0x01},
	{EDGE2,		  0x01},
	{EDGE3,		  0x01},

	{MTX1,		  0x5f},
	{MTX2,		  0x53},
	{MTX3,		  0x11},
	{MTX4,		  0x1a},
	{MTX5,		  0x3d},
	{MTX6,		  0x5a},
	{MTX_Ctrl,    0x1e},

	{BRIGHT,	  0x00},
	{CNST,		  0x25},
	{USAT,		  0x65},
	{VSAT,		  0x65},
	{UVADJ0,	  0x81},
	{SDE,		  0x06},
	
    /*GAMMA config*/
	{GAM1,		  0x0c},
	{GAM2,		  0x16},
	{GAM3,		  0x2a},
	{GAM4,		  0x4e},
	{GAM5,		  0x61},
	{GAM6,		  0x6f},
	{GAM7,		  0x7b},
	{GAM8,		  0x86},
	{GAM9,		  0x8e},
	{GAM10,		  0x97},
	{GAM11,		  0xa4},
	{GAM12,		  0xaf},
	{GAM13,		  0xc5},
	{GAM14,		  0xd7},
	{GAM15,		  0xe8},
	{SLOP,		  0x20},

	{HUECOS,	  0x80},
	{HUESIN,	  0x80},
	{DSPAuto,	  0xff},
	{DM_LNL,	  0x00},
	{BDBase,	  0x99},
	{BDMStep,	  0x03},
	{LC_RADI,	  0x00},
	{LC_COEF,	  0x13},
	{LC_XC,		  0x08},
	{LC_COEFB,    0x14},
	{LC_COEFR,    0x17},
	{LC_CTR,	  0x05},
	
	{COM3,		  0xd0},	/*Horizontal mirror image*/

	/* night mode auto frame rate control */
	{COM5,		  0xf5},	/*在夜视环境下，自动降低帧率，保证低照度画面质量*/
	//{COM5,	  0x31},	/*夜视环境帧率不变*/
};


/*结构体数组成员数目*/
uint8_t OV7725_REG_NUM = sizeof(Sensor_Config)/sizeof(Sensor_Config[0]);

/* 帧同步信号标志，在中断函数和main函数里面使用 */
volatile uint8_t Ov7725_vsync;	 


/**************************************************************
*   File Static Variable Define Section
**************************************************************/


/**************************************************************
*   Function Define Section
**************************************************************/
/**
 *  @name	FIFO_GPIO_Config
 *  @brief	配置OV7725的FIFO芯片引脚
 *  @param  None        
 *  @return None
 *  @notice
 */
static void FIFO_GPIO_Config(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};

	/* Enable PB */
	CKCUClock.Bit.PB         = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	/* 配置为输出引脚 */
	GPIO_DirectionConfig(HT_GPIOB, 
							GPIO_PIN_0	| 	/* PB0，STROBE，闪光灯控制 		*/ 
							GPIO_PIN_2	|	/* PB2，FIFO_WRST，FIFO写复位 	*/
							GPIO_PIN_3	|	/* PB3, FIFO_RRST，FIFO读复位 	*/
							GPIO_PIN_4	|	/* PB4，FIFO_OE，FIFO输出使能 	*/
							GPIO_PIN_5	|	/* PB5，FIFO_RCK，FIFO读时钟 	*/
							GPIO_PIN_6		/* PB6，FIFO_WE，FIFO写使能 	*/
		, GPIO_DIR_OUT);
	
	/* 配置PB8~PB15为输入引脚，用于FIFO数据采集输入 */
	GPIO_DirectionConfig(HT_GPIOB,       
							GPIO_PIN_8	| GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11	|
							GPIO_PIN_12	| GPIO_PIN_13| GPIO_PIN_14 | GPIO_PIN_15
			, GPIO_DIR_IN);
	
	FIFO_CS_L();	  					/*拉低使FIFO输出使能*/
    FIFO_WE_H();   						/*拉高使FIFO写允许*/
}

/**
 *  @name	Ov7725_GPIO_Config
 *  @brief	配置OV7725所用到的引脚
 *  @param  None        
 *  @return None
 *  @notice
 */
void Ov7725_GPIO_Config(void)
{
	SCCB_GPIO_Config();
	FIFO_GPIO_Config();
}


/**
 *  @name	VSYNC_GPIO_Configuration
 *  @brief	配置PB7作为OV7725 VSYNC场引脚
 *  @param  None        
 *  @return None
 *  @notice
 */
static void VSYNC_GPIO_Configuration(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
	/* Enable PB */
	CKCUClock.Bit.PB         = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	/* 配置PB7作为VSYNC引脚 */
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_7, GPIO_DIR_IN);
}


	
/**
 *  @name	VSYNC_NVIC_Configuration
 *  @brief	使用中断通道7
 *  @param  None        
 *  @return None
 *  @notice
 */
static void VSYNC_NVIC_Configuration(void)
{
	/* Enable IRQ */
	NVIC_EnableIRQ(EXTI7_IRQn);
}


/**
 *  @name	VSYNC_EXTI_Configuration
 *  @brief	配置PB7为外部中断引脚
 *  @param  None        
 *  @return None
 *  @notice
	                ___                            ___
	  VSYNC:     __|   |__________________________|   |__     
	 
 */
static void VSYNC_EXTI_Configuration(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
	/* Enable PB & AFIO & EXTI */
	CKCUClock.Bit.PB         = 1;
	CKCUClock.Bit.AFIO 		 = 1;
	CKCUClock.Bit.EXTI 		 = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	/* Select Port as EXTI Trigger Source                                                                     */
	AFIO_EXTISourceConfig(AFIO_EXTI_CH_7, AFIO_ESS_PB);

	/* Config EXTI line 7 Interrupt */
	EXTI_WakeupEventConfig(EXTI_CHANNEL_7, EXTI_WAKEUP_HIGH_LEVEL, ENABLE);
	
	/* Enable EXTI line 7 Interrupt                                                                          */
	EXTI_IntConfig(EXTI_CHANNEL_7, ENABLE);

	/* Activate the corresponding EXTI interrupt                                                              */
	EXTI_SWIntCmd(EXTI_CHANNEL_7, ENABLE);
}


/**
 *  @name	VSYNC_Init
 *  @brief	初始化OV7725 VSYNC引脚
 *  @param  None        
 *  @return None
 *  @notice
 */
void VSYNC_Init(void)
{
    VSYNC_GPIO_Configuration();
    VSYNC_EXTI_Configuration();
    VSYNC_NVIC_Configuration();
}


/**
 *  @name	Ov7725_Init
 *  @brief	初始化OV7725
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7725_Init(void)
{
	uint16_t i = 0;
	uint8_t Sensor_IDCode = 0;	
	
	//DEBUG("ov7725 Register Config Start......");
	
	if( 0 == SCCB_WriteByte ( 0x12, 0x80 ) ) /*复位sensor */
	{
		//DEBUG("sccb write data error");		
		return ERROR ;
	}	

	if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, 0x0b ) )	 /* 读取sensor ID号*/
	{
		//DEBUG("read id faild");		
		return ERROR;
	}
	//DEBUG("Sensor ID is 0x%x", Sensor_IDCode);	
	
	if(Sensor_IDCode == OV7725_ID)
	{
		for( i = 0 ; i < OV7725_REG_NUM ; i++ )
		{
			if( 0 == SCCB_WriteByte(Sensor_Config[i].Address, Sensor_Config[i].Value) )
			{                
				//DEBUG("write reg faild", Sensor_Config[i].Address);
				return ERROR;
			}
		}
	}
	else
	{
		return ERROR;
	}
	//DEBUG("ov7725 Register Config Success");
	
	return SUCCESS;
}



 #ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
          /* User can add his own implementation to report the file name and line number,
             ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
          while (1)
          {}
}
#endif
