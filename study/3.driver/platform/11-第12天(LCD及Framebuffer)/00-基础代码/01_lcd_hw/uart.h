#ifndef _UART_H_
#define _UART_H_

#include "s5pv210.h"
#include <stdarg.h>//������va_list�Ķ���
#include <stdlib.h>

extern char _getc_int(void);
extern void _puts(const char *s);
extern int printf(const char *fmt, ...);

#endif /*_UART_H_*/

