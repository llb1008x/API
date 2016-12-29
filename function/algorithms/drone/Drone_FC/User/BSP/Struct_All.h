#ifndef _STRUCT_ALL_H_
#define _STRUCT_ALL_H_
#include "stm32f10x.h"

#include "Led.h"
#include "USART3.h"
#include "US_100.h"
#include "Uart.h" 
#include "Motor.h"
#include "math.h"
#include "Timer3.h"
#include "DMA_ADC.h"
#include "Protocol.h"
#include "I2C_MPU6050.h"
#include "SPI_NRF24L01.h"

/******************************************************************************
							宏定义
*******************************************************************************/
#define Lock_Mode (1<<0)//锁尾
#define Led_Mode  (1<<1)//夜间模式
#define PI 3.141592653f

extern uint8_t high_mode;//定高模式:0失能，1使能
extern uint8_t Rc_Lock,start;//1上锁；0解锁
#define FLY_type 1	
// 3.7V小四轴 FLY_type == 1;
// 7.4V大四轴 FLY_type == 2;
// 无刷大四轴 FLY_type == 3;
//(警告：无刷大四轴版的程序绝对不允许在另外两个版的飞机上运行！！！)

#if FLY_type == 1	//3.7V小四轴

	#define MAX_THROTTLE 999  //油门最大值
	#define MIN_THROTTLE 0    //油门最小值
	#define Battery_NUM  1.0f //电池数量1S
	#define PWM_MAX      999  //PWM计数上限
	#define	PWM_HZ       3000 //PWM频率
	#define BASIC				 1000
	
#elif FLY_type == 2 //7.4V大四轴

	#define MAX_THROTTLE 999  //油门最大值
	#define MIN_THROTTLE 0    //油门最小值
	#define Battery_NUM  2.0f //电池数量2S
	#define PWM_MAX      999  //PWM计数上限	
	#define	PWM_HZ       3000 //PWM频率
	#define BASIC				 1000
	
#elif FLY_type == 3 //无刷大四轴

	#define MAX_THROTTLE 1999 //油门最大值
	#define MIN_THROTTLE 1000 //油门最小值
	#define Battery_NUM  2.0f //电池数量3S
	#define PWM_MAX      1999 //PWM计数上限	
	#define	PWM_HZ       500	//PWM频率‘
	#define BASIC				 0
	
#endif


/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void Nvic_Init(void);

void EEPROM_INIT(void);
void EEPROM_SAVE_ACC_OFFSET(void);
void EEPROM_READ_ACC_OFFSET(void);
void EEPROM_SAVE_GYRO_OFFSET(void);
void EEPROM_READ_GYRO_OFFSET(void);
void EEPROM_SAVE_PID(void);
void EEPROM_READ_PID(void);
void PID_Reset(void);
/******************************************************************************
							结构体声明
*******************************************************************************/ 
extern uint16_t offset_high;;//高度信息
extern int16_t THROTTLE;
extern float high,Filter_high,old_high,d_high;
extern uint16_t RUN_fre,Debug;//测试运行频率
extern float PID[19][3];

/* MPU6050--加速度计结构体 */
struct _acc
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _acc acc;
extern struct _acc filter_acc;
extern struct _acc offset_acc;

/* MPU6050--陀螺仪结构体 */
struct _gyro
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _gyro gyro;
extern struct _gyro filter_gyro;
extern struct _gyro offset_gyro;
extern struct _out_angle control_angle;
extern struct _out_angle control_gyro;

/* float结构体 */
struct _SI_float
{
	float x;
	float y;
	float z;
};
extern struct _SI_float SI_acc;	
extern struct _SI_float SI_gyro;

/* 姿态解算--角度值 */
struct _out_angle
{
	float yaw;
	float roll;
	float pitch;
};
extern struct _out_angle out_angle;

/* 遥控数据 */
struct _Rc
{
	uint16_t THROTTLE;
	uint16_t YAW;
	uint16_t PITCH;
	uint16_t ROLL;
	
	uint16_t AUX1;
	uint16_t AUX2;
	uint16_t AUX3;
	uint16_t AUX4;
	uint16_t AUX5;
	uint16_t AUX6;
};
extern struct _Rc Rc;

/* pid变量 */
struct _pid
{
	float kp;
	float ki;
	float kd;
	float integral;
	
	float output;
};
extern struct _pid pitch;
extern struct _pid roll;
extern struct _pid gyro_pitch;
extern struct _pid gyro_roll;
extern struct _pid gyro_yaw;
extern struct _pid control_high;
extern float target_high;

#endif

