#include "EEPROM.h"
#include "Filter.h"
#include "struct_all.h"

struct _acc  acc;			//原始数据
struct _gyro gyro;
////////////////////////////////////////////
struct _acc  filter_acc;	//滤波后数据
struct _gyro filter_gyro;
////////////////////////////////////////////
struct _acc  offset_acc;	//零偏数据
struct _gyro offset_gyro;
////////////////////////////////////////////
struct _SI_float  SI_acc;	//加速度数据（m/s2）
struct _SI_float  SI_gyro;	//角速度数据（rad）
struct _SI_float  roll_3D_gyro; //翻滚角速度数据
////////////////////////////////////////////
struct _Rc Rc;				//遥控通道
struct _out_angle out_angle;//姿态解算-角度值

/* pid */
struct _pid pitch;
struct _pid roll;
struct _pid gyro_pitch;
struct _pid gyro_roll;
struct _pid gyro_yaw;
struct _pid control_high;


/******************************************************************************
函数原型：	void PID_Reset(void)
功    能：	重置PID参数
*******************************************************************************/ 
void PID_Reset(void)
{
//控制数据初始化
	Rc.PITCH 		= 1500;
	Rc.ROLL 		= 1500;
	Rc.YAW 			= 1500;
	Rc.THROTTLE = 1500;

	Rc.AUX1 	= 1500;
	Rc.AUX2		= 1500;
	Rc.AUX3		= 1500;
//串级PID参数初始化
	
	//角度环
	PID[1][0] = roll.kp = pitch.kp = 2.0f;
	PID[1][1] = roll.ki = pitch.ki = 0.04f;
	PID[1][2] = roll.kd = pitch.kd = 0.0f;
	//角速度环
	PID[2][0] = gyro_roll.kp = gyro_pitch.kp = 2.0f;
	PID[2][1] = gyro_roll.ki = gyro_pitch.ki = 0.0f;
	PID[2][2] = gyro_roll.kd = gyro_pitch.kd = 0.0f;
	
	//偏航角
	PID[3][0] = gyro_yaw.kp = 8.0f;
	PID[3][1] = gyro_yaw.ki = 0.1f;
	PID[3][2] = gyro_yaw.kd = 0.0f;
//超声波定高PID初始化
	if(FLY_type==1)//小四轴定高参数
	{
		PID[4][0] = control_high.kp = 1.0f;
		PID[4][1] = control_high.ki = 0.1f;
		PID[4][2] = control_high.kd = 30.0f;
	}
	if(FLY_type==2)//大四轴定高参数
	{
		PID[4][0] = control_high.kp = 0.4f;
		PID[4][1] = control_high.ki = 0.1f;
		PID[4][2] = control_high.kd = 20.0f;
	}
	EEPROM_SAVE_PID();
}
///////////////////////////////////////////////////////////
#define EEPROM_offset_acc_x		0
#define EEPROM_offset_acc_y		1
#define EEPROM_offset_acc_z		2
#define EEPROM_offset_gyro_x	3
#define EEPROM_offset_gyro_y	4
#define EEPROM_offset_gyro_z	5
#define EEPROM_PID_ROL_P		6
#define EEPROM_PID_ROL_I		7
#define EEPROM_PID_ROL_D		8
#define EEPROM_PID_PIT_P		9
#define EEPROM_PID_PIT_I		10
#define EEPROM_PID_PIT_D		11
#define EEPROM_PID_YAW_P		12
#define EEPROM_PID_YAW_I		13
#define EEPROM_PID_YAW_D		14
#define EEPROM_SUM				15

uint16_t data_x,data_y,data_z;
uint16_t VirtAddVarTab[NumbOfVar] = {
	0x9A00,0x9A01,0x9A02,0x9A03,0x9A04,0x9A05,0x9A06,0x9A07,0x9A08,0x9A09,
	0x9A10,0x9A12,0x9A13,0x9A14,0x9A15
};//虚拟地址

void EEPROM_INIT(void)//初始化模拟的EEPROM，读取参数
{
	EE_Init();	
	EEPROM_READ_ACC_OFFSET();
	EEPROM_READ_GYRO_OFFSET();
	EEPROM_READ_PID();
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_SUM], &data_x);
	if(data_x != EEPROM_SUM)//检查是否首次试飞
	{
		__set_PRIMASK(1);//关全部中断
		EE_WriteVariable(VirtAddVarTab[EEPROM_SUM], EEPROM_SUM);
		 __set_PRIMASK(0);//开中断
		PID_Reset();//首次试飞，设置PID参数为默认值
	}
}

void EEPROM_SAVE_ACC_OFFSET(void)	//保存acc零偏
{
	__set_PRIMASK(1);//关全部中断
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_acc_x], offset_acc.x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_acc_y], offset_acc.y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_acc_z], offset_acc.z);
	 __set_PRIMASK(0);//开中断
}
void EEPROM_READ_ACC_OFFSET(void)	//读取acc零偏
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_acc_x], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_acc_y], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_acc_z], &data_z);
	offset_acc.x = (int16_t) data_x;
	offset_acc.y = (int16_t) data_y;
	offset_acc.z = (int16_t) data_z;
}
void EEPROM_SAVE_GYRO_OFFSET(void)	//保存gyro零偏
{
	 __set_PRIMASK(1);//关全部中断
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_gyro_x], offset_gyro.x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_gyro_y], offset_gyro.y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_gyro_z], offset_gyro.z);
	 __set_PRIMASK(0);//开中断
}
void EEPROM_READ_GYRO_OFFSET(void)	//读取gyro零偏
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_gyro_x], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_gyro_y], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_gyro_z], &data_z);
	offset_gyro.x = (int16_t) data_x;
	offset_gyro.y = (int16_t) data_y;
	offset_gyro.z = (int16_t) data_z;
}
void EEPROM_SAVE_PID(void)	//保存PID参数
{
	 __set_PRIMASK(1);//关全部中断
	data_x = (int16_t) (roll.kp*10);
	data_y = (int16_t) (roll.ki*1000);
	data_z = (int16_t) (roll.kd);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ROL_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ROL_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ROL_D], data_z);

	data_x = (int16_t) (gyro_roll.kp*10);
	data_y = (int16_t) (gyro_roll.ki*1000);
	data_z = (int16_t) (gyro_roll.kd);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PIT_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PIT_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PIT_D], data_z);

	data_x = (int16_t) (gyro_yaw.kp*10);
	data_y = (int16_t) (gyro_yaw.ki*1000);
	data_z = (int16_t) (gyro_yaw.kd);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_YAW_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_YAW_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_YAW_D], data_z);
	__set_PRIMASK(0);//开中断
}
void EEPROM_READ_PID(void)	//读取PID参数
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ROL_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ROL_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ROL_D], &data_z);
	roll.kp = (float) data_x/10.0f;
	roll.ki = (float) data_y/1000.0f;
	roll.kd = (float) data_z;
	pitch.kp = roll.kp;
	pitch.ki = roll.ki;
	pitch.kd = roll.kd;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PIT_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PIT_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PIT_D], &data_z);
	gyro_roll.kp = (float) data_x/10.0f;
	gyro_roll.ki = (float) data_y/1000.0f;
	gyro_roll.kd = (float) data_z;
	gyro_pitch.kp = gyro_roll.kp;
	gyro_pitch.ki = gyro_roll.ki;
	gyro_pitch.kd = gyro_roll.kd;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_YAW_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_YAW_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_YAW_D], &data_z);
	gyro_yaw.kp = (float) data_x/10.0f;
	gyro_yaw.ki = (float) data_y/1000.0f;
	gyro_yaw.kd = (float) data_z;
}
