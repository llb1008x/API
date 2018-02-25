#ifndef _S5PV210_H_
#define _S5PV210_H_

#include "uart.h"

typedef unsigned char		uchar;
typedef volatile unsigned long	vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char	vu_char;

typedef vu_char		S5PC11X_REG8;
typedef vu_short	S5PC11X_REG16;
typedef vu_long		S5PC11X_REG32;

#ifndef __ASSEMBLY__
#define UData(Data)	((unsigned long) (Data))

#define __REG(x)	(*(vu_long *)(x))
#define __REGl(x)	(*(vu_long *)(x))
#define __REGw(x)	(*(vu_short *)(x))
#define __REGb(x)	(*(vu_char *)(x))
#define __REG2(x,y)	(*(vu_long *)((x) + (y)))
#else
#define UData(Data)	(Data)

#define __REG(x)	(x)
#define __REGl(x)	(x)
#define __REGw(x)	(x)
#define __REGb(x)	(x)
#define __REG2(x,y)	((x) + (y))
#endif

#define GPA0_CON						__REG(ELFIN_GPIO_BASE + GPA0CON_OFFSET)	 
#define GPA1_CON						__REG(ELFIN_GPIO_BASE + GPA1CON_OFFSET)

/*
 * UART
 */
#define ELFIN_UART_BASE			0XE2900000

#define ELFIN_UART0_OFFSET		0x0000
#define ELFIN_UART1_OFFSET		0x0400
#define ELFIN_UART2_OFFSET		0x0800
#define ELFIN_UART3_OFFSET		0x0c00

#if defined(CONFIG_SERIAL1)
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + ELFIN_UART0_OFFSET)
#elif defined(CONFIG_SERIAL2)
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + ELFIN_UART1_OFFSET)
#elif defined(CONFIG_SERIAL3)
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + ELFIN_UART2_OFFSET)
#elif defined(CONFIG_SERIAL4)
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + ELFIN_UART3_OFFSET)
#else
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + ELFIN_UART0_OFFSET)
#endif

#define ULCON_OFFSET			0x00
#define UCON_OFFSET			0x04
#define UFCON_OFFSET			0x08
#define UMCON_OFFSET			0x0C
#define UTRSTAT_OFFSET			0x10
#define UERSTAT_OFFSET			0x14
#define UFSTAT_OFFSET			0x18
#define UMSTAT_OFFSET			0x1C
#define UTXH_OFFSET			0x20
#define URXH_OFFSET			0x24
#define UBRDIV_OFFSET			0x28
#define UDIVSLOT_OFFSET			0x2C
#define UINTP_OFFSET			0x30
#define UINTSP_OFFSET			0x34
#define UINTM_OFFSET			0x38

#define UTRSTAT_TX_EMPTY		BIT2
#define UTRSTAT_RX_READY		BIT0
#define UART_ERR_MASK			0xF

#define S5PC11X_UART_CHANNELS		3
#define S5PC11X_SPI_CHANNELS		2

#ifndef __ASSEMBLY__
typedef enum {
	S5PC11X_UART0,
	S5PC11X_UART1,
	S5PC11X_UART2,
	S5PC11X_UART3,
} S5PC11X_UARTS_NR;
#endif 

#if defined(CONFIG_CLK_533_133_100_100)
#define UART_UBRDIV_VAL		26
#define UART_UDIVSLOT_VAL	0x0808
#else
#define UART_UBRDIV_VAL		34
#define UART_UDIVSLOT_VAL	0xDDDD
#endif

/* UART (see manual chapter 11) */
typedef struct {
	S5PC11X_REG32	ULCON;
	S5PC11X_REG32	UCON;
	S5PC11X_REG32	UFCON;
	S5PC11X_REG32	UMCON;
	S5PC11X_REG32	UTRSTAT;
	S5PC11X_REG32	UERSTAT;
	S5PC11X_REG32	UFSTAT;
	S5PC11X_REG32	UMSTAT;
#ifdef __BIG_ENDIAN
	S5PC11X_REG8	res1[3];
	S5PC11X_REG8	UTXH;
	S5PC11X_REG8	res2[3];
	S5PC11X_REG8	URXH;
#else /* Little Endian */
	S5PC11X_REG8	UTXH;
	S5PC11X_REG8	res1[3];
	S5PC11X_REG8	URXH;
	S5PC11X_REG8	res2[3];
#endif
	S5PC11X_REG32	UBRDIV;
	S5PC11X_REG32	UDIVSLOT;//xiuhai
} /*__attribute__((__packed__))*/ S5PC11X_UART;

static inline S5PC11X_UART * S5PC11X_GetBase_UART(S5PC11X_UARTS_NR nr)
{
	return (S5PC11X_UART *)(ELFIN_UART_BASE + (nr*0x400));
}

#endif /*_S5PV210_H_*/

