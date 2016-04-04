/***************************************************************************************
 *  FileName                    :   ov7670.c
 *  CopyRight                   :	Zoisite
 *  ModuleName                  :   ov7670
 *
 *  CPU                         :	Intel core i5-4210U
 *  RTOS                        :	Windows 7 pro
 *
 *  Create Data                 :   2016/03/28
 *  Author/Corportation         :   Gaunthan
 *
 *  Abstract Description        :   封装ov7670摄像头的初始化、数据采集等功能
 *
 *--------------------------------Revision History--------------------------------------
 *  No      version     Data        Revised By      Item        Description
 *  1       v1.0        2016/04/02 	Gaunthan                    Create this file
 *
 ***************************************************************************************/


/**************************************************************
*   Debug switch Section
**************************************************************/


/**************************************************************
*   Include File Section
**************************************************************/
#include "ov7670.h"


/**************************************************************
*   Macro Define Section
**************************************************************/

/**
 * @brief	调试信息输出宏定义
 */
#if OV7670_DEBUG_ON
	#define OV7670_DEBUG(fmt,args...) printf(fmt ,##args)
#else
	#define OV7670_DEBUG(fmt,args...)
#endif	

/* Registers */
#define REG_GAIN        0x00    /* Gain lower 8 bits (rest in vref) */
#define REG_BLUE        0x01    /* blue gain */
#define REG_RED         0x02    /* red gain */
#define REG_VREF        0x03    /* Pieces of GAIN, VSTART, VSTOP */
#define REG_COM1        0x04    /* Control 1 */
#define COM1_CCIR656    0x40     /* CCIR656 enable */
#define REG_BAVE        0x05    /* U/B Average level */
#define REG_GbAVE       0x06    /* Y/Gb Average level */
#define REG_AECHH       0x07    /* AEC MS 5 bits */
#define REG_RAVE        0x08    /* V/R Average level */
#define REG_COM2        0x09    /* Control 2 */
#define COM2_SSLEEP      0x10    /* Soft sleep mode */
#define REG_PID         0x0a    /* Product ID MSB */
#define REG_VER         0x0b    /* Product ID LSB */
#define REG_COM3        0x0c    /* Control 3 */
#define COM3_SWAP        0x40    /* Byte swap */
#define COM3_SCALEEN     0x08    /* Enable scaling */
#define COM3_DCWEN       0x04    /* Enable downsamp/crop/window */
#define REG_COM4        0x0d    /* Control 4 */
#define REG_COM5        0x0e    /* All "reserved" */
#define REG_COM6        0x0f    /* Control 6 */
#define REG_AECH        0x10    /* More bits of AEC value */
#define REG_CLKRC       0x11    /* Clocl control */
#define CLK_EXT         0x40    /* Use external clock directly */
#define CLK_SCALE       0x3f    /* Mask for internal clock scale */
#define REG_COM7        0x12
    /* Control 7 */
#define COM7_RESET      0x80    /* Register reset */
#define COM7_FMT_MASK   0x38
#define COM7_FMT_VGA    0x00
#define COM7_FMT_CIF    0x20    /* CIF format */
#define COM7_FMT_QVGA   0x10    /* QVGA format */
#define COM7_FMT_QCIF   0x08    /* QCIF format */
#define COM7_RGB        0x04    /* bits 0 and 2 - RGB format */
#define COM7_YUV        0x00    /* YUV */
#define COM7_BAYER      0x01    /* Bayer format */
#define COM7_PBAYER     0x05    /* "Processed bayer" */
#define REG_COM8        0x13    /* Control 8 */
#define COM8_FASTAEC    0x80    /* Enable fast AGC/AEC */
#define COM8_AECSTEP    0x40    /* Unlimited AEC step size */
#define COM8_BFILT      0x20    /* Band filter enable */
#define COM8_AGC        0x04    /* Auto gain enable */
#define COM8_AWB        0x02    /* White balance enable */
#define COM8_AEC        0x01    /* Auto exposure enable */
#define REG_COM9        0x14    /* Control 9  - gain ceiling */
#define REG_COM10       0x15    /* Control 10 */
#define COM10_HSYNC     0x40    /* HSYNC instead of HREF */
#define COM10_PCLK_HB   0x20    /* Suppress PCLK on horiz blank */
#define COM10_HREF_REV  0x08    /* Reverse HREF */
#define COM10_VS_LEAD   0x04    /* VSYNC on clock leading edge */
#define COM10_VS_NEG    0x02    /* VSYNC negative */
#define COM10_HS_NEG    0x01    /* HSYNC negative */
#define REG_HSTART      0x17    /* Horiz start high bits */
#define REG_HSTOP       0x18    /* Horiz stop high bits */
#define REG_VSTART      0x19    /* Vert start high bits */
#define REG_VSTOP       0x1a    /* Vert stop high bits */
#define REG_PSHFT       0x1b    /* Pixel delay after HREF */
#define REG_MIDH        0x1c    /* Manuf. ID high */
#define REG_MIDL        0x1d    /* Manuf. ID low */
#define REG_MVFP        0x1e    /* Mirror / vflip */
#define MVFP_MIRROR     0x20    /* Mirror image */
#define MVFP_FLIP       0x10    /* Vertical flip */
 
#define REG_AEW         0x24    /* AGC upper limit */
#define REG_AEB         0x25    /* AGC lower limit */
#define REG_VPT         0x26    /* AGC/AEC fast mode op region */
#define REG_HSYST       0x30    /* HSYNC rising edge delay */
#define REG_HSYEN       0x31    /* HSYNC falling edge delay */
#define REG_HREF        0x32    /* HREF pieces */
#define REG_TSLB        0x3a    /* lots of stuff */
#define TSLB_YLAST      0x04    /* UYVY or VYUY - see com13 */
#define REG_COM11       0x3b    /* Control 11 */
#define COM11_NIGHT     0x80    /* NIght mode enable */
#define COM11_NMFR      0x60    /* Two bit NM frame rate */
#define COM11_HZAUTO    0x10    /* Auto detect 50/60 Hz */
#define COM11_50HZ      0x08    /* Manual 50Hz select */
#define COM11_EXP       0x02
#define REG_COM12       0x3c    /* Control 12 */
#define COM12_HREF      0x80    /* HREF always */
#define REG_COM13       0x3d    /* Control 13 */
#define COM13_GAMMA     0x80    /* Gamma enable */
#define COM13_UVSAT     0x40    /* UV saturation auto adjustment */
#define COM13_UVSWAP    0x01    /* V before U - w/TSLB */
#define REG_COM14       0x3e    /* Control 14 */
#define COM14_DCWEN     0x10    /* DCW/PCLK-scale enable */
#define REG_EDGE        0x3f    /* Edge enhancement factor */
#define REG_COM15       0x40    /* Control 15 */
#define COM15_R10F0     0x00    /* Data range 10 to F0 */
#define COM15_R01FE     0x80    /*            01 to FE */
#define COM15_R00FF     0xc0    /*            00 to FF */
#define COM15_RGB565    0x10    /* RGB565 output */
#define COM15_RGB555    0x30    /* RGB555 output */
#define REG_COM16       0x41    /* Control 16 */
#define COM16_AWBGAIN   0x08    /* AWB gain enable */
#define REG_COM17       0x42    /* Control 17 */
#define COM17_AECWIN    0xc0    /* AEC window - must match COM4 */
#define COM17_CBAR      0x08    /* DSP Color bar */
 
 /*
  * This matrix defines how the colors are generated, must be
  * tweaked to adjust hue and saturation.
  *
  * Order: v-red, v-green, v-blue, u-red, u-green, u-blue
  *
  * They are nine-bit signed quantities, with the sign bit
  * stored in 0x58.  Sign for v-red is bit 0, and up from there.
  */
#define REG_CMATRIX_BASE 0x4f
#define CMATRIX_LEN 6
#define REG_CMATRIX_SIGN 0x58
 
 
#define REG_BRIGHT      0x55    /* Brightness */
#define REG_CONTRAS     0x56    /* Contrast control */
 
#define REG_GFIX        0x69    /* Fix gain control */
 
#define REG_REG76       0x76    /* OV's name */
#define R76_BLKPCOR     0x80    /* Black pixel correction enable */
#define R76_WHTPCOR     0x40    /* White pixel correction enable */
 
#define REG_RGB444      0x8c    /* RGB 444 control */
#define R444_ENABLE     0x02    /* Turn on RGB444, overrides 5x5 */
#define R444_RGBX       0x01    /* Empty nibble at end */
 
#define REG_HAECC1      0x9f    /* Hist AEC/AGC control 1 */
#define REG_HAECC2      0xa0    /* Hist AEC/AGC control 2 */
 
#define REG_BD50MAX     0xa5    /* 50hz banding step limit */
#define REG_HAECC3      0xa6    /* Hist AEC/AGC control 3 */
#define REG_HAECC4      0xa7    /* Hist AEC/AGC control 4 */
#define REG_HAECC5      0xa8    /* Hist AEC/AGC control 5 */
#define REG_HAECC6      0xa9    /* Hist AEC/AGC control 6 */
#define REG_HAECC7      0xaa    /* Hist AEC/AGC control 7 */
#define REG_BD60MAX     0xab    /* 60hz banding step limit */

 
/**************************************************************
*   Struct Define Section
**************************************************************/
/**
 * @brief	OV7670寄存器结构定义
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

/* 由于内存大小限制，缓冲区不能过大。因此采用行缓冲 */
u16 LineBuf[FRAME_WIDTH];

/* 寄存器参数配置 */
Reg_Info Sensor_Config[] =
{
     /*以下为OV7670 QVGA RGB565参数  */
  	{0x3a, 0x04},//
	{0x40, 0x10},
	{0x12, 0x14},//QVGA,RGB输出

	//输出窗口设置
	{0x32, 0x80},
	{0x17, 0x16},         
	{0x18, 0x04},//5
	{0x19, 0x02},
	{0x1a, 0x7a},//0x7a,
 	{0x03, 0x0a},//0x0a,


	{0x0c, 0x0c},
	{0x15, 0x00},
	{0x3e, 0x00},//10
	{0x70, 0x00},
	{0x71, 0x01},
	{0x72, 0x11},
	{0x73, 0x09},//
        
	{0xa2, 0x02},//15
	{0x11, 0x00},//时钟分频设置,0,不分频.
	{0x7a, 0x20},
	{0x7b, 0x1c},
	{0x7c, 0x28},
        
	{0x7d, 0x3c},//20
	{0x7e, 0x55},
	{0x7f, 0x68},
	{0x80, 0x76},
	{0x81, 0x80},
        
	{0x82, 0x88},
	{0x83, 0x8f},
	{0x84, 0x96},
	{0x85, 0xa3},
	{0x86, 0xaf},
        
	{0x87, 0xc4},//30
	{0x88, 0xd7},
	{0x89, 0xe8},
	{0x13, 0xe0},
	{0x00, 0x00},//AGC
        
	{0x10, 0x00},
	{0x0d, 0x00}, 
	{0x14, 0x20},//0x38, limit the max gain
	{0xa5, 0x05},
	{0xab, 0x07},
        
	{0x24, 0x75},//40
	{0x25, 0x63},
	{0x26, 0xA5},
	{0x9f, 0x78},
	{0xa0, 0x68},
        
	{0xa1, 0x03},//0x0b,
	{0xa6, 0xdf},//0xd8,
	{0xa7, 0xdf},//0xd8,
	{0xa8, 0xf0},
	{0xa9, 0x90},
        
	{0xaa, 0x94},//50
	{0x13, 0xe5},
	{0x0e, 0x61},
	{0x0f, 0x4b},
	{0x16, 0x02},
        
	{0x1e, 0x27},//图像输出镜像控制.0x07,
	{0x21, 0x02},
	{0x22, 0x91},
	{0x29, 0x07},
	{0x33, 0x0b},
        
	{0x35, 0x0b},//60
	{0x37, 0x1d},
	{0x38, 0x71},
	{0x39, 0x2a},
	{0x3c, 0x78},
        
	{0x4d, 0x40},
	{0x4e, 0x20},
	{0x69, 0x5d},
	{0x6b, 0x40},//PLL*4=48Mhz
	{0x74, 0x19},
	{0x8d, 0x4f},
        
	{0x8e, 0x00},//70
	{0x8f, 0x00},
	{0x90, 0x00},
	{0x91, 0x00},
	{0x92, 0x00},//0x19,//0x66
        
	{0x96, 0x00},
	{0x9a, 0x80},
	{0xb0, 0x84},
	{0xb1, 0x0c},
	{0xb2, 0x0e},
        
	{0xb3, 0x82},//80
	{0xb8, 0x0a},
	{0x43, 0x14},
	{0x44, 0xf0},
	{0x45, 0x34},
        
	{0x46, 0x58},
	{0x47, 0x28},
	{0x48, 0x3a},
	{0x59, 0x88},
	{0x5a, 0x88},
        
	{0x5b, 0x44},//90
	{0x5c, 0x67},
	{0x5d, 0x49},
	{0x5e, 0x0e},
	{0x64, 0x04},
	{0x65, 0x20},
        
	{0x66, 0x05},
	{0x94, 0x04},
	{0x95, 0x08},
	{0x6c, 0x0a},
	{0x6d, 0x55},
        
        
	{0x4f, 0x80},
	{0x50, 0x80},
	{0x51, 0x00},
	{0x52, 0x22},
	{0x53, 0x5e},
	{0x54, 0x80},
        
	//{0x54, 0x40},//110
        
        
	{0x09, 0x03},//驱动能力最大

	{0x6e, 0x11},//100
	{0x6f, 0x9f},//0x9e for advance AWB
    {0x55, 0x00},//亮度
    {0x56, 0x40},//对比度
    {0x57, 0x80},//0x40,  change according to Jim's request
};

/* 帧同步信号标志，在中断函数和main函数里面使用 */
volatile uint8_t Ov7670_vsync;	 


/**************************************************************
*   File Static Variable Define Section
**************************************************************/


/**************************************************************
*   Function Define Section
**************************************************************/

/**
 *  @name	FIFO_GPIO_Config
 *  @brief	配置OV7670的FIFO芯片引脚
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
	
	GPIO_InputConfig(HT_GPIOB,       
							GPIO_PIN_8	| GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11	|
							GPIO_PIN_12	| GPIO_PIN_13| GPIO_PIN_14 | GPIO_PIN_15
			, ENABLE);
	
	FIFO_CS_L();	  					/*拉低使FIFO输出使能*/
    FIFO_WE_H();   						/*拉高使FIFO写允许*/
}

/**
 *  @name	Ov7670_GPIO_Config
 *  @brief	配置OV7670所用到的引脚
 *  @param  None        
 *  @return None
 *  @notice
 */
void Ov7670_GPIO_Config(void)
{
	SCCB_GPIO_Config();
	FIFO_GPIO_Config();
}


/**
 *  @name	VSYNC_GPIO_Configuration
 *  @brief	配置PB7作为OV7670 VSYNC场引脚
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
	
	/* Enable GPIO Input Function  */
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_7, ENABLE);

	/* 由于要配置为下降沿触发的中断模式，因此上拉该引脚 */
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_7, GPIO_PR_UP);
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
	EXTI_InitTypeDef EXTI_InitStruct;
	
	/* Enable PB & AFIO & EXTI */
	CKCUClock.Bit.PB         = 1;
	CKCUClock.Bit.AFIO 		 = 1;
	CKCUClock.Bit.EXTI 		 = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	/* Config EXTI channel 7 */
	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_7;
	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
	EXTI_InitStruct.EXTI_DebounceCnt = 0;
	EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;
	EXTI_Init(&EXTI_InitStruct);
	
	/* Select Port as EXTI Trigger Source */
	AFIO_EXTISourceConfig(AFIO_EXTI_CH_7, AFIO_ESS_PB);
	
	/* Enable EXTI line 7 Interrupt                                                                          */
	EXTI_IntConfig(EXTI_CHANNEL_7, ENABLE);
}


/**
 *  @name	VSYNC_Init
 *  @brief	初始化OV7670 VSYNC引脚
 *  @param  None        
 *  @return None
 *  @notice
 */
void VSYNC_Init(void)
{
    VSYNC_GPIO_Configuration();
	VSYNC_NVIC_Configuration();
    VSYNC_EXTI_Configuration();
}


/**
 *  @name	Ov7670_Init
 *  @brief	初始化OV7670
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7670_Init(void)
{
	uint16_t i = 0;
	uint8_t Sensor_IDCode = 0;	
	
	/*结构体数组成员数目*/
	uint8_t OV7670_REG_NUM = sizeof(Sensor_Config)/sizeof(Sensor_Config[0]);

	OV7670_DEBUG("ov7670 Register Config Start......\r\n");
	
	if( 0 == SCCB_WriteByte ( 0x12, 0x80 ) ) /*复位sensor */
	{
		OV7670_DEBUG("sccb write data error\r\n");		
		return ERROR ;
	}	

	if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, 0x0b ) )	 /* 读取sensor ID号*/
	{
		OV7670_DEBUG("read id faild\r\n");		
		return ERROR;
	}
	OV7670_DEBUG("Sensor ID is 0x%x\r\n", Sensor_IDCode);	
	
	if(Sensor_IDCode == OV7670_ID)
	{
		for( i = 0 ; i < OV7670_REG_NUM ; i++ )
		{
			if( 0 == SCCB_WriteByte(Sensor_Config[i].Address, Sensor_Config[i].Value) )
			{                
				OV7670_DEBUG("write reg %x faild\r\n", Sensor_Config[i].Address);
				return ERROR;
			}
		}
	}
	else
	{
		return ERROR;
	}
	OV7670_DEBUG("ov7670 Register Config Success\r\n");
	
	return SUCCESS;
}



/**
 *  @name	Ov7670_TryInit
 *  @brief	带超时计数地初始化OV7670
 *  @param  None        
 *  @return None
 *  @notice
 */
ErrStatus Ov7670_TryInit(void)
{
	int retry = 3;
	
	while(retry--)
	{
		if(Ov7670_Init() == SUCCESS)
		{
			OV7670_DEBUG("Init ov7670 success.\r\n");	
			return SUCCESS;
		}
		else
			OV7670_DEBUG("Init ov7670 error.\r\n");
	}
	return ERROR;	//超时返回错误
}



/**
 *	@name	ImageLineDetect
 *	@brief	采集一帧图像的某一行
 *	@param	None
 *	@return	None
 *	@notice	由于函数栈过小，本函数使用了全局数组来存放采集到的数据
 */		
void ImageLineDetect(void)
{
	int i;
	
	/* 从FIFO读出一行像素保存到Line_Buf */
	for(i = 0; i < FRAME_WIDTH; ++i)
		READ_FIFO_PIXEL(LineBuf[i]);
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
