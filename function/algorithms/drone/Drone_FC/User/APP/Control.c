#include "Maths.h"
#include "Control.h"
#include "struct_all.h"

uint8_t Rc_Lock=1,start=0;;//1上锁；0解锁
uint8_t high_mode = 0;//定高模式:0失能，1使能

#define control_gyro_max 	 	50.0f	
#define gyro_integral_max 	5000.0f
#define   DIP  50.0f

#define angle_max 	 		10.0f	
#define angle_integral_max 	1000.0f	

struct _out_angle control_angle;
struct _out_angle control_gyro;
/******************************************************************************
函数原型：	void Control_Angle(struct _out_angle *angle,struct _Rc *rc)
功    能：	PID控制器(外环)
*******************************************************************************/ 
void Control_Angle(struct _out_angle *angle,struct _Rc *rc)
{
	static struct _out_angle last_angle;

//////////////////////////////////////////////////////////////////
//			以下为角度环
//////////////////////////////////////////////////////////////////
	control_angle.roll  = angle->roll  + (rc->PITCH  -1500)/DIP;// + (rc->AUX2 -1500)/100.0f;
	control_angle.pitch = angle->pitch - (rc->ROLL -1500)/DIP;// - (rc->AUX1 -1500)/100.0f;
//////////////////////////////////////////////////////////////////
	if(control_angle.roll >  angle_max)	//ROLL
		roll.integral +=  angle_max;
	else if(control_angle.roll < -angle_max)
		roll.integral += -angle_max;
	else
		roll.integral += control_angle.roll;

	if(control_angle.pitch >  angle_max)//PITCH
		 pitch.integral +=  angle_max;
	else if(control_angle.pitch < -angle_max)
		 pitch.integral += -angle_max;
	else
		pitch.integral += control_angle.pitch;
	
	if(roll.integral >  angle_integral_max)
	   roll.integral =  angle_integral_max;
	if(roll.integral < -angle_integral_max)
	   roll.integral = -angle_integral_max;

	if(pitch.integral >  angle_integral_max)
	   pitch.integral =  angle_integral_max;
	if(pitch.integral < -angle_integral_max)
	   pitch.integral = -angle_integral_max;
//////////////////////////////////////////////////////////////////
	if(start==0)//停机时积分清零
	{
		roll.integral  = 0;
		pitch.integral = 0;
	}
//////////////////////////////////////////////////////////////////
	roll.output  = roll.kp *control_angle.roll  + roll.ki *roll.integral  + roll.kd *(control_angle.roll -last_angle.roll );
	pitch.output = pitch.kp*control_angle.pitch + pitch.ki*pitch.integral + pitch.kd*(control_angle.pitch-last_angle.pitch);
//////////////////////////////////////////////////////////////////
	last_angle.roll =control_angle.roll;
	last_angle.pitch=control_angle.pitch;
}

int16_t THROTTLE;
/******************************************************************************
函数原型：	void Control_Gyro(struct _SI_float *gyro,struct _Rc *rc,uint8_t Lock)
功    能：	PID控制器(内环)
*******************************************************************************/ 
void Control_Gyro(struct _SI_float *gyro,struct _Rc *rc,uint8_t Lock)
{
	static struct _out_angle last_gyro;
	static uint8_t i;
	uint16_t THROTTLE_BASIC;
	int16_t throttle1,throttle2,throttle3,throttle4;
//////////////////////////////////////////////////////////////////
//			以下为角速度环
//////////////////////////////////////////////////////////////////
	control_gyro.roll  = -roll.output - gyro->y*Radian_to_Angle;
	control_gyro.pitch = pitch.output - gyro->x*Radian_to_Angle;
//////////////////////////////////////////////////////////////////	
	if(rc->AUX4 & Lock_Mode)
		control_gyro.yaw  = - gyro->z*Radian_to_Angle;// - (rc->AUX3 -1500)/100.0f;//锁尾模式
	else
		control_gyro.yaw  = -(rc->YAW-1500)/DIP - gyro->z*Radian_to_Angle;// + (rc->AUX3 -1500)/50.0f;//非锁尾模式
//////////////////////////////////////////////////////////////////
	//积分
	if(control_gyro.roll >  control_gyro_max)	//GYRO_ROLL
		gyro_roll.integral +=  control_gyro_max;
	else if(control_gyro.roll < -control_gyro_max)
		gyro_roll.integral += -control_gyro_max;
	else
		gyro_roll.integral += control_gyro.roll;
	
	if(control_gyro.pitch >  control_gyro_max)//GYRO_PITCH
		gyro_pitch.integral +=  control_gyro_max;
	else if(control_gyro.pitch < -control_gyro_max)
		gyro_pitch.integral += -control_gyro_max;
	else
		gyro_pitch.integral += control_gyro.pitch;
	
	if(control_gyro.yaw >  control_gyro_max)//GYRO_YAW
		gyro_yaw.integral +=  control_gyro_max;
	if(control_gyro.yaw < -control_gyro_max)
		gyro_yaw.integral += -control_gyro_max;
	else
		gyro_yaw.integral += control_gyro.yaw;
	
	if(gyro_roll.integral >  gyro_integral_max)
	   gyro_roll.integral =  gyro_integral_max;
	if(gyro_roll.integral < -gyro_integral_max)
	   gyro_roll.integral = -gyro_integral_max;
	
	if(gyro_pitch.integral >  gyro_integral_max)
	   gyro_pitch.integral =  gyro_integral_max;
	if(gyro_pitch.integral < -gyro_integral_max)
	   gyro_pitch.integral = -gyro_integral_max;
	
	if(gyro_yaw.integral >  gyro_integral_max)
	   gyro_yaw.integral =  gyro_integral_max;
	if(gyro_yaw.integral < -gyro_integral_max)
	   gyro_yaw.integral = -gyro_integral_max;
//////////////////////////////////////////////////////////////////
	if(start==0)//停机时积分清零
	{
		gyro_roll.integral  = 0;
		gyro_pitch.integral = 0;
		gyro_yaw.integral  	= 0;
	}
//////////////////////////////////////////////////////////////////
	gyro_roll.output  = gyro_roll.kp *control_gyro.roll  + gyro_roll.ki *gyro_roll.integral  + gyro_roll.kd *(control_gyro.roll -last_gyro.roll );
	gyro_pitch.output = gyro_pitch.kp*control_gyro.pitch + gyro_pitch.ki*gyro_pitch.integral + gyro_pitch.kd*(control_gyro.pitch-last_gyro.pitch);
	gyro_yaw.output   = gyro_yaw.kp  *control_gyro.yaw   + gyro_yaw.ki  *gyro_yaw.integral   + gyro_yaw.kd  *(control_gyro.yaw  -last_gyro.yaw  );
//////////////////////////////////////////////////////////////////
	last_gyro.roll =control_gyro.roll;
	last_gyro.pitch=control_gyro.pitch;
	last_gyro.yaw  =control_gyro.yaw;
//////////////////////////////////////////////////////////////////
	//油门的控制
	if(high_mode)	//定高模式
	{
		THROTTLE = Control_high(rc->THROTTLE);//超声波定高
		
		if(THROTTLE>100)	start = 1;
		else start = 0;
		
		if(FLY_type == 3) THROTTLE = THROTTLE + 1000;//电调pwm信号范围（1000~2000）
	
		//停机时，用手拿飞机离开地面自动上锁，避免伤人。
		if(high>40 && start==0)	
		{
			i++;
			if(i>100) {Rc_Lock=1;i=0;}//上锁
		}
		else i=0;
	}
	else	//手动控制模式
	{
		THROTTLE_BASIC = (rc->AUX2 -1500)*0.7f;	//控制油门基数大小
		
		if(rc->THROTTLE>1200)	start = 1;
		else start = 0;
		
		THROTTLE = rc->THROTTLE - THROTTLE_BASIC - BASIC;
	}
	if(THROTTLE < MIN_THROTTLE ) THROTTLE = MIN_THROTTLE;
	if(THROTTLE > (MAX_THROTTLE - 200) ) THROTTLE = MAX_THROTTLE - 200;//油门不允许达到最大值，否则会失控
//////////////////////////////////////////////////////////////////
	if(start && Lock==0)
	{
		throttle1 = THROTTLE + gyro_pitch.output + gyro_roll.output - gyro_yaw.output ;
		throttle2 = THROTTLE + gyro_pitch.output - gyro_roll.output + gyro_yaw.output ;
		throttle3 = THROTTLE - gyro_pitch.output + gyro_roll.output + gyro_yaw.output ;
		throttle4 = THROTTLE - gyro_pitch.output - gyro_roll.output - gyro_yaw.output ;
		
		if(throttle1>MAX_THROTTLE)throttle1=MAX_THROTTLE;
		if(throttle1<MIN_THROTTLE)throttle1=MIN_THROTTLE;
		
		if(throttle2>MAX_THROTTLE)throttle2=MAX_THROTTLE;
		if(throttle2<MIN_THROTTLE)throttle2=MIN_THROTTLE;
		
		if(throttle3>MAX_THROTTLE)throttle3=MAX_THROTTLE;
		if(throttle3<MIN_THROTTLE)throttle3=MIN_THROTTLE;
		
		if(throttle4>MAX_THROTTLE)throttle4=MAX_THROTTLE;
		if(throttle4<MIN_THROTTLE)throttle4=MIN_THROTTLE;
	}
	else
	{
		throttle1=MIN_THROTTLE;
		throttle2=MIN_THROTTLE;
		throttle3=MIN_THROTTLE;
		throttle4=MIN_THROTTLE;
	}
	Motor_Out(throttle1,throttle2,throttle3,throttle4); // M1 M2 M0 M3
}

float target_high;
#define high_integral_max 10000
#define high_integral_speed_max 30
/******************************************************************************
函数原型：	uint16_t Control_high(uint16_t THROTTLE)
功    能：	高度 PID 控制
*******************************************************************************/ 
uint16_t Control_high(uint16_t THROTTLE)//高度 PID 控制
{
	static uint16_t THROTTLE_out=0;
	extern uint8_t high_ok;
	
	if(high_ok == 0);//高度信息未更新（50HZ）
	else
	{
		high_ok = 0;
		
		if(start==0 && THROTTLE>=1700)//开始起飞
		{
			control_high.integral = 3000 + (2000-Rc.AUX2);//从3000开始积分，加快起飞速度
			target_high = 100;//开始起飞目标高度10cm
		}
		else
		{
			target_high += (THROTTLE - 1500)/100.0f;
			if(target_high<=0)target_high=0;	//高度限幅
			if(target_high>=2000)target_high=2000;
		////////////////////////////////////////////////////////////////
			//高度误差
			control_high.output = target_high - high;	
			
			if(control_high.output < -100)	control_high.output=-100;//限幅
			if(control_high.output > 100)	control_high.output=100;//限幅
		////////////////////////////////////////////////////////////////	
			//高度误差积分
			if(control_high.output>5 && d_high <= 0)
			{
				if(control_high.output >= high_integral_speed_max)
					control_high.integral += high_integral_speed_max;	
				else	control_high.integral += control_high.output;
			}
			else if(control_high.output<-5 && d_high >= 0)
			{
				if(control_high.output <= -high_integral_speed_max)
					control_high.integral -= high_integral_speed_max;
				else	control_high.integral += control_high.output;
			}

			//加速度限幅
			if(d_high < -3) d_high = -3;
			if(d_high > 3) d_high = 3;

			//目标高度为零并且已经下降到目标高度以下 或者 上锁状态下，积分清零
			if( (target_high ==0 && control_high.output >= -40) || Rc_Lock==1)	
			{
				control_high.integral = 0;
			}
			
			THROTTLE_out = control_high.kp * control_high.output + control_high.ki * control_high.integral - control_high.kd * d_high ;
		}
	}
	
	THROTTLE_out = (THROTTLE_out>=800)?800:THROTTLE_out; //油门输出限幅，不能满油门输出，避免PID失调
	
	return THROTTLE_out;
}

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
	
/******************************************************************************
函数原型：	void RC_LOCK(void)
功    能：	遥控手势指令及解锁处理（以下注释针对美国手，遥控默认的就是美国手）
*******************************************************************************/ 
void RC_LOCK(void)
{
	static uint8_t count0,count1,count2,count3,count4,count5;
////////////////////////////////////////////////
	if(Rc_Lock == 1)//上锁状态下
	{
		if(Rc.THROTTLE<1200 && Rc.YAW<1200)//左手遥杆打到左下角，解锁
		{
			count0++;
			if(count0>100)
			{
				count0 = 0;
				Rc_Lock = 0;
				LED_FLASH();
			}
		}
		else
			count0 = 0;
		////////////////////////////////////////////////
		if(Rc.THROTTLE<1200 && Rc.PITCH<1200)//左右遥杆都拉倒最低，校准仪器
		{
			count2++;
			if(count2>100 && Rc_Lock)
			{
				count2=0;
				Do_GYRO_Offset();//校正陀螺仪
				Do_ACC_Offset(); //校正加速度计
				Do_US_Offset();	//校正超声波
				LED_FLASH();
			}
		}
		else
			count2 = 0;

	}
////////////////////////////////////////////////	
	if(Rc_Lock == 0)//解锁状态下
	{
		if(Rc.THROTTLE<1200 && Rc.YAW>1800)//左手遥杆打到右下角，上锁
		{	
			count1++;
			if(count1>100)
			{
				count1 = 0;
				Rc_Lock = 1;
				LED_FLASH();
			}
		}
		else
			count1 = 0;
	}

////////////////////////////////////////////////	
	if(high_mode == 0)//手动控制模式下
	{
		if(Rc.ROLL<1200 && Rc.PITCH<1200)//右遥杆都拉到左下角，超声波定高模式
		{
			count4++;
			if(count4>100 && Rc_Lock)
			{
				count4=0;
				high_mode = 1;
				LED_FLASH();
			}
		}
		else
			count4 = 0;
	}
////////////////////////////////////////////////	
	if(high_mode == 1)//超声波定高模式下
	{
		if(Rc_Lock == 0)//紧急停机
		{
			if(Rc.YAW>1990)//左手遥杆打到最左边，紧急停机
			{
				count3++;
				if(count3>10 && Rc_Lock==0)
				{
					count3 = 0;
					Rc_Lock = 1;
					control_high.integral = 0;
					LED_FLASH();
				}
			}
			else
				count3 = 0;
		}
	////////////////////////////////////////////////	
		if(Rc.PITCH<1200 && Rc.ROLL>1800)//右遥杆都拉到右下角，手动控制模式
		{
			count5++;
			if(count5>100 && Rc_Lock)
			{
				count5=0;
				high_mode = 0;
				LED_FLASH();
			}
		}
		else
			count5 = 0;
	}
}
