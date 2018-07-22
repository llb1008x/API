.DEFINE	P_IOA_Data   		0x7000         // Write Data into data register and read from IOA pad
.DEFINE P_IOA_Buffer        0x7001         // Write Data into buffer register and read from buffer register
.DEFINE P_IOA_Dir           0x7002         // Direction vector for IOA
.DEFINE P_IOA_Attrib        0x7003         // Attribute vector for IOA
.DEFINE P_IOA_Latch         0x7004         // Latch PortA data for key change wake-up

.DEFINE P_IOB_Data         	0x7005         // Write Data into the data register and read from IOB pad
.DEFINE P_IOB_Buffer        0x7006         // Write Data into buffer register and read from buffer register
.DEFINE P_IOB_Dir           0x7007         // Direction vector for IOB
.DEFINE P_IOB_Attrib        0x7008         // Attribute vector for IOB
//端口需要改动时需要修改以下定义

.define	DATA_PORT_HIGHT	0x00		//数据口输出高八位定义，若数据接口为IO端口的高八位，则不屏蔽
.define	M_IO_DATA_MASK	0xff00			//若数据口为高八位，则定义值为0xff00

.define M_IO_EP			0x4000			//EP与SPCE061A的接口定义
.define	M_IO_RW			0x2000			//RW与SPCE061A的接口定义
.define	M_IO_AO			0x1000			//AO与SPCE061A的接口定义
.define	M_IO_CS			0x8000		    //CS与SPCE061A的接口定义
//数据端口定义
.define	P_IO_Data_D		P_IOA_Data
.define	P_IO_Data_A		P_IOA_Attrib
.define	P_IO_Data_Di	P_IOA_Dir
.define P_IO_Data_B		P_IOA_Buffer
//控制端口定义
.define	P_IO_Ctrl_D		P_IOB_Data
.define	P_IO_Ctrl_A		P_IOB_Attrib
.define	P_IO_Ctrl_Di	P_IOB_Dir
.define P_IO_Ctrl_B		P_IOB_Buffer
//.....end端口修改即修订到前面即可
.define	P_IO_Data		P_IO_Data_D
.define	P_IO_Ctrl		P_IO_Ctrl_D
