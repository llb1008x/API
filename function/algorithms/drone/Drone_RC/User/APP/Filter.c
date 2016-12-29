#include "Filter.h"
#include "struct_all.h"

/******************************************************************************
函数原型：	void RC_Limit(struct _Rc *rc)
功    能：	限制遥控指令范围
*******************************************************************************/ 
void RC_Limit(struct _Rc *rc)
{
	rc->THROTTLE = (rc->THROTTLE<=1000)?1000:rc->THROTTLE; 
	rc->THROTTLE = (rc->THROTTLE>=2000)?2000:rc->THROTTLE; 
	rc->PITCH = (rc->PITCH<=1000)?1000:rc->PITCH; 
	rc->PITCH = (rc->PITCH>=2000)?2000:rc->PITCH; 
	rc->ROLL = (rc->ROLL<=1000)?1000:rc->ROLL; 
	rc->ROLL = (rc->ROLL>=2000)?2000:rc->ROLL; 
	rc->YAW  = (rc->YAW<=1000)?1000:rc->YAW; 
	rc->YAW  = (rc->YAW>=2000)?2000:rc->YAW; 
	rc->AUX1 = (rc->AUX1<=1000)?1000:rc->AUX1; 
	rc->AUX1 = (rc->AUX1>=2000)?2000:rc->AUX1; 
	rc->AUX2 = (rc->AUX2<=1000)?1000:rc->AUX2; 
	rc->AUX2 = (rc->AUX2>=2000)?2000:rc->AUX2; 
	rc->AUX3 = (rc->AUX3<=1000)?1000:rc->AUX3; 
	rc->AUX3 = (rc->AUX3>=2000)?2000:rc->AUX3; 
}

#define Filter_Num 5
#define d_MAX 100
/******************************************************************************
函数原型：	void Rc_Filter( struct _Rc *Rc_in,struct _Rc *Rc_out)
功    能：	遥控指令滤波
*******************************************************************************/ 
void Rc_Filter( struct _Rc *Rc_in,struct _Rc *Rc_out)
{
	static uint16_t Filter_A[Filter_Num],Filter_B[Filter_Num],Filter_C[Filter_Num],Filter_D[Filter_Num],
									Filter_E[Filter_Num],Filter_F[Filter_Num],Filter_G[Filter_Num];
	static uint8_t 	Filter_count;
	static uint32_t 	Filter_sum_A=0,Filter_sum_B=0,Filter_sum_C=0,Filter_sum_D=0,
									Filter_sum_E=0,Filter_sum_F=0,Filter_sum_G=0;
//////////////////////////////////////////////////////////////////
//			由AD值计算遥控数据
//////////////////////////////////////////////////////////////////
#ifdef American_Mode//美国手
	Rc_in->THROTTLE = 1000 + (uint16_t)(0.25f*ADC_Value[4]) - offset0;//油门
	Rc_in->PITCH 	  = 2000 - (uint16_t)(0.25f*ADC_Value[7]) - offset2;//升降舵
#else //日本手
	Rc_in->THROTTLE = 2000 - (uint16_t)(0.25f*ADC_Value[7]) - offset0;//油门
	Rc_in->PITCH 	  = 1000 + (uint16_t)(0.25f*ADC_Value[4]) - offset2;//升降舵
#endif
	
	Rc_in->YAW 		  = 1000 + (uint16_t)(0.25f*ADC_Value[5]) - offset1;//方向
	Rc_in->ROLL 	  =	2000 - (uint16_t)(0.25f*ADC_Value[6]) - offset3;//副翼
	
	Rc_in->AUX3 	  = 1000 + (uint16_t)(0.25f*ADC_Value[2]) - offset6;//
	Rc_in->AUX2	 	  = 1000 + (uint16_t)(0.25f*ADC_Value[0]) - offset5;//油门微调
	Rc_in->AUX1 	  = 1000 + (uint16_t)(0.25f*ADC_Value[1]) - offset4;//
//////////////////////////////////////////////////////////////////
	RC_Limit(Rc_in);	//将遥控数据限制在1000~2000
	Do_Offset();			//遥控上电后自动对中，响一声后即对中完成	
//////////////////////////////////////////////////////////////////
//	遥控指令滤波
//////////////////////////////////////////////////////////////////
//丢数据
	Filter_sum_A -= Filter_A[Filter_count];
	Filter_sum_B -= Filter_B[Filter_count];
	Filter_sum_C -= Filter_C[Filter_count];
	Filter_sum_D -= Filter_D[Filter_count];
	Filter_sum_E -= Filter_E[Filter_count];
	Filter_sum_F -= Filter_F[Filter_count];
	Filter_sum_G -= Filter_G[Filter_count];
		
	Filter_A[Filter_count] = Rc_in->PITCH;
	Filter_B[Filter_count] = Rc_in->ROLL;
	Filter_C[Filter_count] = Rc_in->THROTTLE;
	Filter_D[Filter_count] = Rc_in->YAW;
	Filter_E[Filter_count] = Rc_in->AUX1;
	Filter_F[Filter_count] = Rc_in->AUX2;
	Filter_G[Filter_count] = Rc_in->AUX3;
//更新数据	
	Filter_sum_A += Filter_A[Filter_count];
	Filter_sum_B += Filter_B[Filter_count];
	Filter_sum_C += Filter_C[Filter_count];
	Filter_sum_D += Filter_D[Filter_count];
	Filter_sum_E += Filter_E[Filter_count];
	Filter_sum_F += Filter_F[Filter_count];
	Filter_sum_G += Filter_G[Filter_count];
	
	Rc_out->PITCH 		= Filter_sum_A / Filter_Num;
	Rc_out->ROLL  		= Filter_sum_B / Filter_Num;
	Rc_out->THROTTLE 	= Filter_sum_C / Filter_Num;
	Rc_out->YAW 	 		= Filter_sum_D / Filter_Num;
	Rc_out->AUX1 	 		= Filter_sum_E / Filter_Num;
	Rc_out->AUX2 	 		= Filter_sum_F / Filter_Num;
	Rc_out->AUX3 	 		= Filter_sum_G / Filter_Num;

	Filter_count++;
	if(Filter_count == Filter_Num)	Filter_count=0;
//////////////////////////////////////////////////////////////////	
//遥杆中点
	if( sqrt((Rc_out->THROTTLE-1500)*(Rc_out->THROTTLE-1500) + (Rc_out->YAW-1500)*(Rc_out->YAW-1500)) <= 25 )
	{
		Rc_out->THROTTLE=1500;
		Rc_out->YAW=1500;
	}
	if( sqrt((Rc_out->ROLL-1500)*(Rc_out->ROLL-1500) + (Rc_out->PITCH-1500)*(Rc_out->PITCH-1500)) <= 25 )
	{
		Rc_out->ROLL=1500;
		Rc_out->PITCH=1500;
	}
	
	if(Rc_out->AUX1>1495 && Rc_out->AUX1<1505)	
		Rc_out->AUX1=1500;
	
//	if(Rc_out->AUX2>1495 && Rc_out->AUX2<1505)	
//		Rc_out->AUX2=1500;
	
	if(Rc_out->AUX3>1495 && Rc_out->AUX3<1505)	
		Rc_out->AUX3=1500;
}


