#include "MPU6050.h"

/**************************************************************
*        Global Value Define Section
**************************************************************/
u8 MPU_Data [33];
float Axis[3];
float Angle[3];
float Angular[3];
float FinalBaseAxis[3];

/**
 * @brief  PDMA的初始化。
 * @param  
 * @retval None
 */
void PDMA_Configuration(void)
{
	PDMACH_InitTypeDef PDMACH_InitStructure;	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
	//Config Clock
	CKCUClock.Bit.PDMA       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	//Config USART
	USART_RxPDMACmd(HT_USART0, ENABLE);

	//PDMA
	PDMACH_InitStructure.PDMACH_SrcAddr   = (u32) HT_USART0_BASE;
	PDMACH_InitStructure.PDMACH_DstAddr   = (u32) &MPU_Data;

	PDMACH_InitStructure.PDMACH_AdrMod    = SRC_ADR_FIX | DST_ADR_LIN_INC | AUTO_RELOAD;
	PDMACH_InitStructure.PDMACH_Priority  = H_PRIO;
	PDMACH_InitStructure.PDMACH_BlkCnt    = 33;
	PDMACH_InitStructure.PDMACH_BlkLen    = 1;
	PDMACH_InitStructure.PDMACH_DataSize  = WIDTH_8BIT;

	PDMA_Config(PDMA_CH2, &PDMACH_InitStructure);  
	PDMA_IntConig(PDMA_CH2, (PDMA_INT_GE | PDMA_INT_TC), ENABLE);
	PDMA_EnaCmd(PDMA_CH2, ENABLE);
	PDMA_SwTrigCmd(PDMA_CH2, ENABLE);
}

/**
 * @brief  得出三个方向轴的加速度
 * @param  
 * @retval None
 */
void Axis_DataTransfrom(void)
{
	int i;
	for(i = 0 ; i < 3; i++)
	{			
		if(MPU_Data[i * 11 + 1] == 0x51)
		{
			Axis[0] = ((short)(MPU_Data[3 + i * 11] << 8 | MPU_Data[2 + i * 11])) / 32768.0 * 16;		//X轴加速度	 
			Axis[1] = ((short)(MPU_Data[5 + i * 11] << 8 | MPU_Data[4 + i * 11])) / 32768.0 * 16;    	//Y轴加速度
			Axis[2] = ((short)(MPU_Data[7 + i * 11] << 8 | MPU_Data[6 + i * 11])) / 32768.0 * 16;    	//Z轴加速度		
		}
	}
}

/**
 * @brief  得出三个方向轴的角速度
 * @param  
 * @retval None
 */
void Angular_DataTransFrom(void)
{
	int i;
	for(i = 0 ; i < 3; i++)
	{			
		if(MPU_Data[i * 11 + 1] == 0x52)
		{
			Angular[0] = ((short)(MPU_Data[3 + i * 11] << 8 | MPU_Data[2 + i * 11])) / 32768.0 * 2000;	//X轴角速度	 
			Angular[1] = ((short)(MPU_Data[5 + i * 11] << 8 | MPU_Data[4 + i * 11])) / 32768.0 * 2000;    //Y轴角速度
			Angular[2] = ((short)(MPU_Data[7 + i * 11] << 8 | MPU_Data[6 + i * 11])) / 32768.0 * 2000;    //Z轴角速度		
		}
	}
}

/**
 * @brief  获得三个方向轴的角度
 * @param  
 * @retval None
 */
void Angle_DataTransfrom(void)
{
	int i;
	for(i = 0; i < 3; i++)
	{
		if(MPU_Data[i * 11 + 1] == 0x53)
		{
			Angle[0] = ((short)(MPU_Data[3 + i * 11] << 8) | MPU_Data[2 + i * 11]) / 32768.0 * 180;	//X轴角度
			Angle[1] = ((short)(MPU_Data[5 + i * 11] << 8) | MPU_Data[4 + i * 11]) / 32768.0 * 180;	//Y轴角度
			Angle[2] = ((short)(MPU_Data[7 + i * 11] << 8) | MPU_Data[6 + i * 11]) / 32768.0 * 180;	//Z轴角度
		}
	}
}

/**
 * @brief  处理获得的加速度，去除基准值
 * @param  
 * @retval None
 */
void Axis_GetFinalData(void)
{
	/*分别获得角速度和加速度基准值*/
	Axis_DataTransfrom();
	Angle_DataTransfrom();
	Angular_DataTransFrom();
	
	/*处理加速度的基准值*/
	DealWithRotation();
}

/**
 * @brief  		通过检测加速度的剧烈变化来判断是否发生车祸
 * @param  		void
 * @retval 		TRUE：表示发生车祸了；FALSE：表示并没有发生车祸
 */
bool IsAccident(void)
{
	Axis_GetFinalData();
	
	if(Axis[2] > 0.520)
		return TRUE;
	else
		return FALSE;
}

/**
 * @brief  		按照X、Y或Z轴旋转后得到旋转矩阵
 * @param  		matrix：旋转矩阵，存储旋转后的数据
 * 				angleIndegree：角度，以熟知的度数方式表示，需要转换为π / n
 *				axis：	表示绕着哪个轴转
 */
#define MATRIX( row, col ) matrix[ row * 3 + col ]
static void Rotate(float matrix[], float angleInDegree, float x, float y, float z)
{
	float alpha = angleInDegree / 180.0 * PI;	//将角度转换为π / n的格式
	float Sin = sin(alpha);						
	float Cos = cos(alpha);						
	float translateValue = 1.0 - Cos;			//矩阵系数值

	MATRIX(0, 0) = translateValue * x * x + Cos;
	MATRIX(0, 1) = translateValue * x * y + Sin * z;
	MATRIX(0, 2) = translateValue * x * z - Sin * y;
	MATRIX(1, 0) = translateValue * x * y - Sin * z;
	MATRIX(1, 1) = translateValue * y * y + Cos;
	MATRIX(1, 2) = translateValue * y * z + Sin * x;
	MATRIX(2, 0) = translateValue * x * z + Sin * y;
	MATRIX(2, 1) = translateValue * y * z - Sin * x;
	MATRIX(2, 2) = translateValue * z * z + Cos;
}

/**
 * @brief  		按照矩阵乘法进行矩阵右乘获得各个轴旋转后的位置
 * @param  		matrix：旋转矩阵，存储旋转后的数据
 *				coord：坐标矩阵，初始时的位置坐标
 */
static void GetAcceleration(float matrix[9], float originCoord[3], float finalCoord[3])
{
	int i, j;
	for(i = 0; i < 3; ++i)
	{
		finalCoord[i] = 0.0;
		for(j = 0; j < 3; ++j)
		{
			finalCoord[i] += originCoord[j] * MATRIX(j, i);
		}
	}
}

/**
 * @brief  		处理旋转后加速度的消除平衡问题
 * @param  		void
 */
void DealWithRotation(void)
{
	float matrix[9] = {0.0};
	float originCoord[3];
	int i, j;
	
	for(i = 0; i < 3; ++i)
	{
		for(j = 0; j < 3; ++j)
			originCoord[j] = 0.0;
		
		originCoord[i] = 1.0;
		
		Rotate(matrix, Angle[0], 1.0, 0.0, 0.0);
		GetAcceleration(matrix, originCoord, FinalBaseAxis);	//依次将获得的矩阵中的值进行右乘
		Rotate(matrix, Angle[1], 0.0, 1.0, 0.0);
		GetAcceleration(matrix, FinalBaseAxis, originCoord);	//交换的原因是方便存储和修改，因为每一次可以将获得的作为下一次的参数
		Rotate(matrix, Angle[2], 0.0, 0.0, 1.0);
		GetAcceleration(matrix, originCoord, FinalBaseAxis);
		
		Axis[i] -= FinalBaseAxis[2];	//减去Z轴的(其为其重力方向的加速度)
	}
}
