#ifndef __LRC_H__
#define __LRC_H__


typedef int s32_t;
typedef unsigned int u32_t;
typedef unsigned short u16_t;
typedef unsigned char u8_t;

typedef struct _LRC_INFO 
{
	u32_t Time;
	char LrcText[100];
	struct _LRC_INFO *next;
} LRC_INFO;

extern LRC_INFO *InsertList( LRC_INFO * head, LRC_INFO *pi);

extern LRC_INFO *GetLrcInfo(char *LrcFile, u32_t FileSize);

extern void print( LRC_INFO * head);

extern void Message_Song(char *Lrc_Buffer);

#endif
