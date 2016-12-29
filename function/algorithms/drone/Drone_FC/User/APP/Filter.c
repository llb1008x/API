#include "math.h"
#include "Maths.h"
#include "Filter.h"
#include "struct_all.h"

static float ACC_IIR_FACTOR;
/******************************************************************************
函数原型：	void Calculate_FilteringCoefficient(float Time, float Cut_Off)
功    能：	iir低通滤波参数计算
*******************************************************************************/ 
void Calculate_FilteringCoefficient(float Time, float Cut_Off)
{
	ACC_IIR_FACTOR = Time /( Time + 1/(2.0f*Pi*Cut_Off) );
}
/******************************************************************************
函数原型：	void ACC_IIR_Filter(struct _acc *Acc_in,struct _acc *Acc_out)
功    能：	iir低通滤波
*******************************************************************************/ 
void ACC_IIR_Filter(struct _acc *Acc_in,struct _acc *Acc_out)
{
	Acc_out->x = Acc_out->x + ACC_IIR_FACTOR*(Acc_in->x - Acc_out->x); 
	Acc_out->y = Acc_out->y + ACC_IIR_FACTOR*(Acc_in->y - Acc_out->y); 
	Acc_out->z = Acc_out->z + ACC_IIR_FACTOR*(Acc_in->z - Acc_out->z); 
}

#define Filter_Num 20
/******************************************************************************
函数原型：	void Gyro_Filter(struct _gyro *Gyro_in,struct _gyro *Gyro_out)
功    能：	gyro窗口滑动滤波
*******************************************************************************/ 
void Gyro_Filter(struct _gyro *Gyro_in,struct _gyro *Gyro_out)
{
	static int16_t Filter_x[Filter_Num],Filter_y[Filter_Num],Filter_z[Filter_Num];
	static uint8_t Filter_count;
	static int32_t Filter_sum_x=0,Filter_sum_y=0,Filter_sum_z=0;
	
	Filter_sum_x -= Filter_x[Filter_count];//丢数据
	Filter_sum_y -= Filter_y[Filter_count];
	Filter_sum_z -= Filter_z[Filter_count];
	
	Filter_x[Filter_count] = Gyro_in->x;
	Filter_y[Filter_count] = Gyro_in->y;
	Filter_z[Filter_count] = Gyro_in->z;

	Filter_sum_x += Filter_x[Filter_count];//更新数据
	Filter_sum_y += Filter_y[Filter_count];
	Filter_sum_z += Filter_z[Filter_count];
	
	Gyro_out->x = Filter_sum_x / Filter_Num;
	Gyro_out->y = Filter_sum_y / Filter_Num;
	Gyro_out->z = Filter_sum_z / Filter_Num;
	
	Filter_count++;
	if(Filter_count == Filter_Num)	Filter_count=0;
}
#define Num 5
/******************************************************************************
函数原型：	void US_100_Filter(struct _gyro *Gyro_in,struct _gyro *Gyro_out)
功    能：	超声波窗口滑动滤波
*******************************************************************************/ 
void US_100_Filter(float *in,float *out)
{
	static int16_t Filter[Num];
	static uint8_t Filter_count;
	static int32_t Filter_sum=0;
	
	Filter_sum -= Filter[Filter_count];//丢数据
	
	Filter[Filter_count] = *in;
	
	Filter_sum += Filter[Filter_count];//更新数据
	
	*out = Filter_sum / Num;
	
	Filter_count++;
	if(Filter_count == Num)	Filter_count=0;
}

