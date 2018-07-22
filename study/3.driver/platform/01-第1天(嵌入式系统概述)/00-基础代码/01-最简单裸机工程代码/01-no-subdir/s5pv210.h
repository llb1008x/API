#ifndef __S5PV210_H__
#define __S5PV210_H__


typedef unsigned char			uchar;
typedef volatile unsigned long	vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char	vu_char;

typedef vu_char		S5PC11X_REG8;
typedef vu_short	S5PC11X_REG16;
typedef vu_long		S5PC11X_REG32;

#define UData(Data)	((unsigned long) (Data))

#define __REG(x)	(*(vu_long *)(x))
#define __REGl(x)	(*(vu_long *)(x))
#define __REGw(x)	(*(vu_short *)(x))
#define __REGb(x)	(*(vu_char *)(x))
#define __REG2(x,y)	(*(vu_long *)((x) + (y)))

/*GPIO*/
#define ELFIN_GPIO_BASE		0xE0200000

#define GPA0CON				0xE0200000	 
#define GPA0DAT				0xE0200004		 
#define GPA0PUD				0xE0200008		 
#define GPA0DRV				0xE020000C
#define GPA0CONPDN			0xE0200010	 
#define GPA0PUDPDN			0xE0200014

#define GPA1CON				0xE0200020
#define GPA1DAT				0xE0200024	 
#define GPA1PUD				0xE0200028	 
#define GPA1DRV				0xE020002C	 
#define GPA1CONPDN			0xE0200030 	 
#define GPA1PUDPDN			0xE0200034

#define GPD0CON				0xE02000A0
#define GPD0DAT				0xE02000A4	 
#define GPD0PUD				0xE02000A8	 
#define GPD0DRV				0xE02000AC
#define GPD0CONPDN			0xE02000B0	 
#define GPD0PUDPDN			0xE02000B4	

#define GPD1CON				0xE02000C0
#define GPD1DAT				0xE02000C4 
#define GPD1PUD				0xE02000C8 
#define GPD1DRV				0xE02000CC
#define GPD1CONPDN			0xE02000D0	 
#define GPD1PUDPDN			0xE02000D4	

#define GPH0CON				0xE0200C00
#define GPH0DAT				0xE0200C04 	 
#define GPH0PUD				0xE0200C08 	 
#define GPH0DRV				0xE0200C0C

#define GPH1CON				0xE0200C20
#define GPH1DAT				0xE0200C24	 
#define GPH1PUD				0xE0200C28	 
#define GPH1DRV				0xE0200C2C

#define GPH2CON				0xE0200C40
#define GPH2DAT				0xE0200C44	 
#define GPH2PUD				0xE0200C48	 
#define GPH2DRV				0xE0200C4C

#define GPH3CON				0xE0200C60
#define GPH3DAT				0xE0200C64 
#define GPH3PUD				0xE0200C68 
#define GPH3DRV				0xE0200C6C

/*UART*/
#define UART_CONSOLE 		0XE2900000

typedef struct {
	S5PC11X_REG32	ULCON;
	S5PC11X_REG32	UCON;
	S5PC11X_REG32	UFCON;
	S5PC11X_REG32	UMCON;
	S5PC11X_REG32	UTRSTAT;
	S5PC11X_REG32	UERSTAT;
	S5PC11X_REG32	UFSTAT;
	S5PC11X_REG32	UMSTAT;
	S5PC11X_REG8	UTXH;
	S5PC11X_REG8	res1[3];
	S5PC11X_REG8	URXH;
	S5PC11X_REG8	res2[3];
	S5PC11X_REG32	UBRDIV;
	S5PC11X_REG32	UDIVSLOT;
}S5PC11X_UART;


#endif /*_S5PV210_H_*/

