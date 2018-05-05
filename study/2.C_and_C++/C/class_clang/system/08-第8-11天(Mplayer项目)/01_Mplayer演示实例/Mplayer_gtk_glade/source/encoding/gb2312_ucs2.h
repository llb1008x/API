#ifndef __GB2312_UCS2_H__
#define __GB2312_UCS2_H__
/*************************************************************
* 文件：gb2312_ucs2.c gb2312_ucs2.h
* 功能：GB2312码与uncode 码相互转换。
**************************************************************/

/*************************************************************
* 语法格式：unsigned short gb2312_to_ucs2(unsigned short ucs2)
* 实现功能：gb2312转换为ucs2
* 参数：	gb2312 待转换的gb2312编码
* 返回值：	ucs2编码
**************************************************************/
unsigned short gb2312_to_ucs2(unsigned short gb2312);

/*************************************************************
* 语法格式：unsigned short ucs2_to_gb2312(unsigned short ucs2)
* 实现功能：ucs2转gb2312
* 参数：	ucs2: 待转换的ucs2编码
* 返回值：	gb2312编码
**************************************************************/
unsigned short ucs2_to_gb2312(unsigned short ucs2);

/*************************************************************
*功能：
* 说明:
* UTF-8以字节为单位对Unicode进行编码。
* 从Unicode到UTF-8的编码方式如下：
* Unicode编码(16进制) --> UTF-8 字节流(二进制) 
* U-00000000 ~ U-0000007F --> 0xxxxxxx  
* U-00000080 ~ U-000007FF --> 110xxxxx 10xxxxxx  
* U-00000800 ~ U-0000FFFF --> 1110xxxx 10xxxxxx 10xxxxxx  
* U-00010000 ~ U-001FFFFF --> 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
* U-00200000 ~ U-03FFFFFF --> 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
* U-04000000 ~ U-7FFFFFFF --> 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
* 故根据每个汉字的第一个字节，便可以知道utf-8汉字由几个字节组成
*************************************************************/
int get_utf8_nbytes(unsigned char first_char);

/*************************************************************
* 语法格式：int utf8_to_ucs2(const char *utf8)
* 实现功能：utf-8字符串转换为unicode码
* 参数：	utf utf-8字符串
* 返回值：	unicode码码值
*************************************************************/
int utf8_to_ucs2(const char *utf8);

/*************************************************************
* 语法格式：char *utf8str_to_ucs2str(char *dest, char *source)
* 功能：	utf8字符串转为ucs2字符串
* 参数：	source：utf8字符串
			dest：ucs2字符串
* 返回值：	ucs2字符串首地址
**************************************************************/
char *utf8str_to_ucs2str(char *dest, char *source);

/*************************************************************
* 语法格式：unsigned char *utf8_to_gb2312(
							const unsigned char *utf, unsigned char *gb2312)
* 实现功能：utf-8字符串转换为gb2312字符串
* 参数：	utf utf-8字符串
* 			gb2312 gb2312字符串
* 返回值：	
**************************************************************/
unsigned char *utf8_to_gb2312(const unsigned char *utf, unsigned char *gb2312);

/*************************************************************
* 语法格式：unsigned char *gb2312_to_utf8(
							const unsigned char *gb2312, unsigned char *utf)
* 实现功能：gb2312字符串转换为utf-8字符串
* 参数：	gb2312 gb2312字符串
* 			utf utf-8字符串
* 返回值：	
**************************************************************/
unsigned char *gb2312_to_utf8(const unsigned char *gb2312, unsigned char *utf);

#endif 
