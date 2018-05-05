#ifndef _UART_H_
#define _UART_H_

#include "s5pv210.h"
#include <stdarg.h>//包含有va_list的定义
#include <stdlib.h>

extern int isdigit(int c);
extern char _getc_int(void);
extern void _puts(const char *s);
extern int printf(const char *fmt, ...);

#endif /*__UART_H__*/

