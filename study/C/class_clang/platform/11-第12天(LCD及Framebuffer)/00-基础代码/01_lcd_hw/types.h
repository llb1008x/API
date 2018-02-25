#ifndef _LINUX_TYPES_H
#define _LINUX_TYPES_H

typedef unsigned char		uchar;
typedef volatile unsigned long	vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char	vu_char;

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;

/* sysv */
typedef unsigned char		unchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;

typedef unsigned char		__u8;
typedef unsigned short		__u16;
typedef unsigned int		__u32;
typedef unsigned long long	uint64_t;
typedef unsigned long long __u64;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;

typedef		__u8		uint8_t;
typedef		__u16		uint16_t;
typedef		__u32		uint32_t;

typedef		__u8		u_int8_t;
typedef		__u16		u_int16_t;
typedef		__u32		u_int32_t;

typedef long long		loff_t;


typedef __u16  __le16;
typedef __u16  __be16;
typedef __u32  __le32;
typedef __u32  __be32;

typedef unsigned int		__kernel_size_t;
typedef __kernel_size_t		size_t;


#define readw(addr) (*(volatile u16 *) (addr))
#define readl(addr) (*(volatile u32 *) (addr))
#define writew(b,addr) ((*(volatile u16 *) (addr)) = (b))
#define writel(b,addr) ((*(volatile u32 *) (addr)) = (b))

#endif /* _LINUX_TYPES_H */
