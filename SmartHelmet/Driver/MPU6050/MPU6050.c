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
 * @brief  PDMA�ĳ�ʼ����
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
 * @brief  �ó�����������ļ��ٶ�
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
			Axis[0] = ((short)(MPU_Data[3 + i * 11] << 8 | MPU_Data[2 + i * 11])) / 32768.0 * 16;		//X����ٶ�	 
			Axis[1] = ((short)(MPU_Data[5 + i * 11] << 8 | MPU_Data[4 + i * 11])) / 32768.0 * 16;    	//Y����ٶ�
			Axis[2] = ((short)(MPU_Data[7 + i * 11] << 8 | MPU_Data[6 + i * 11])) / 32768.0 * 16;    	//Z����ٶ�		
		}
	}
}

/**
 * @brief  �ó�����������Ľ��ٶ�
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
			Angular[0] = ((short)(MPU_Data[3 + i * 11] << 8 | MPU_Data[2 + i * 11])) / 32768.0 * 2000;	//X����ٶ�	 
			Angular[1] = ((short)(MPU_Data[5 + i * 11] << 8 | MPU_Data[4 + i * 11])) / 32768.0 * 2000;    //Y����ٶ�
			Angular[2] = ((short)(MPU_Data[7 + i * 11] << 8 | MPU_Data[6 + i * 11])) / 32768.0 * 2000;    //Z����ٶ�		
		}
	}
}

/**
 * @brief  �������������ĽǶ�
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
			Angle[0] = ((short)(MPU_Data[3 + i * 11] << 8) | MPU_Data[2 + i * 11]) / 32768.0 * 180;	//X��Ƕ�
			Angle[1] = ((short)(MPU_Data[5 + i * 11] << 8) | MPU_Data[4 + i * 11]) / 32768.0 * 180;	//Y��Ƕ�
			Angle[2] = ((short)(MPU_Data[7 + i * 11] << 8) | MPU_Data[6 + i * 11]) / 32768.0 * 180;	//Z��Ƕ�
		}
	}
}

/**
 * @brief  �����õļ��ٶȣ�ȥ����׼ֵ
 * @param  
 * @retval None
 */
void Axis_GetFinalData(void)
{
	/*�ֱ��ý��ٶȺͼ��ٶȻ�׼ֵ*/
	Axis_DataTransfrom();
	Angle_DataTransfrom();
	Angular_DataTransFrom();
	
	/*������ٶȵĻ�׼ֵ*/
	DealWithRotation();
}

/**
 * @brief  		ͨ�������ٶȵľ��ұ仯���ж��Ƿ�������
 * @param  		void
 * @retval 		TRUE����ʾ���������ˣ�FALSE����ʾ��û�з�������
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
 * @brief  		����X��Y��Z����ת��õ���ת����
 * @param  		matrix����ת���󣬴洢��ת�������
 * 				angleIndegree���Ƕȣ�����֪�Ķ�����ʽ��ʾ����Ҫת��Ϊ�� / n
 *				axis��	��ʾ�����ĸ���ת
 */
#define MATRIX( row, col ) matrix[ row * 3 + col ]
static void Rotate(float matrix[], float angleInDegree, float x, float y, float z)
{
	float alpha = angleInDegree / 180.0 * PI;	//���Ƕ�ת��Ϊ�� / n�ĸ�ʽ
	float Sin = sin(alpha);						
	float Cos = cos(alpha);						
	float translateValue = 1.0 - Cos;			//����ϵ��ֵ

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
 * @brief  		���վ���˷����о����ҳ˻�ø�������ת���λ��
 * @param  		matrix����ת���󣬴洢��ת�������
 *				coord��������󣬳�ʼʱ��λ������
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
 * @brief  		������ת����ٶȵ�����ƽ������
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
		GetAcceleration(matrix, originCoord, FinalBaseAxis);	//���ν���õľ����е�ֵ�����ҳ�
		Rotate(matrix, Angle[1], 0.0, 1.0, 0.0);
		GetAcceleration(matrix, FinalBaseAxis, originCoord);	//������ԭ���Ƿ���洢���޸ģ���Ϊÿһ�ο��Խ���õ���Ϊ��һ�εĲ���
		Rotate(matrix, Angle[2], 0.0, 0.0, 1.0);
		GetAcceleration(matrix, originCoord, FinalBaseAxis);
		
		Axis[i] -= FinalBaseAxis[2];	//��ȥZ���(��Ϊ����������ļ��ٶ�)
	}
}
