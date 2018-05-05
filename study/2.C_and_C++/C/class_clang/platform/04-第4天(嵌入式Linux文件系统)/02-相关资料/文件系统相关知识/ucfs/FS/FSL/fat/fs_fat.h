/*
**********************************************************************
*                          Micrium, Inc.
*                      949 Crestview Circle
*                     Weston,  FL 33327-1848
*
*                            uC/FS
*
*                (c) Copyright 2002, Micrium, Inc.
*                      All rights reserved.
*
***********************************************************************

----------------------------------------------------------------------
File        : fs_fat.h
Purpose     : FAT File System Layer header
----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
None.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FS_FAT_H_
#define _FS_FAT_H_

/*********************************************************************
*
*             #define constants
*
**********************************************************************
*/

#define FS_FAT_ATTR_READ_ONLY   0x01
#define FS_FAT_ATTR_ARCHIVE     0x20
#define FS_FAT_ATTR_DIRECTORY   0x10

#define FS_FAT_DENTRY_SIZE      0x20


/*********************************************************************
*
*             Global data types
*
**********************************************************************
*/

/* BIOS parameter block (FAT12/FAT16)*/
typedef struct {
  char            jmpboot[3];       /* 0xE9 0x0000                    */
  char            oemname[8];       /* '        '                     */
  unsigned char   bytespersec[2];   /* _512_,1024,2048,4096           */
  char            secperclus;       /* sec in allocation unit         */
  FS_u16          rsvdseccnt;       /* 1 for FAT12 & FAT16            */
  char            numfats;          /* 2                              */
  unsigned char   rootentcnt[2];    /* number of root dir entries     */
  unsigned char   totsec16[2];      /* RSVD + FAT + ROOT + FATA (<64k) */
  char            media;            /* 0xf0 floppy, else 0xF8 same in fat[0] */
  FS_u16          fatsz16;          /* number of FAT sectors          */
  FS_u16          secpertrk;        /* sectors per track              */
  FS_u16          numheads;         /* number of heads                */
  FS_u32          hiddsec;          /* number of hidden sec prec.     */
  FS_u32          totsec32;         /* RSVD + FAT + ROOT + FATA (>=64k) */
  char            offset36[0x1da];
  FS_u16          signature;        /* 0xAA55 Signature               */
} FS__fat_BPB_fat_type;


/* FAT directory entry */
typedef struct {
  unsigned char   data[32];
} FS__fat_dentry_type;


/*********************************************************************
*
*             Externals
*
**********************************************************************
*/

extern FS__fat_BPB_fat_type FS__fat_BPB_unit[FS_MAXDEV][FS_FAT_MAXUNIT];


/*********************************************************************
*
*             Global function prototypes
*
**********************************************************************
*/

/* fat_misc */
void FS__fat_block_init(void);
char *FS__fat_malloc(unsigned int Size);
void FS__fat_free(void *Block);
FS_i32 FS__fat_diskclust(int idx, FS_u32 unit, FS_i32 first, FS_i32 fclust);
FS_i32 FS__fat_FAT_alloc(int idx, FS_u32 unit, FS_i32 last);
FS_i32 FS__fat_FAT_find_eof(int idx, FS_u32 unit, FS_i32 start, FS_u32 *clstsize);
int FS__fat_which_type(int idx, FS_u32 unit);
int FS__fat_checkunit(int idx, FS_u32 unit);

/* fat_in */
FS_size_t FS__fat_fread(void *ptr, FS_size_t size, FS_size_t n, FS_FILE *fp);

/* fat_out */
FS_size_t FS__fat_fwrite(const void *ptr, FS_size_t size, FS_size_t n,FS_FILE *fp);

/* fat_open */
FS_FILE *FS__fat_fopen(const char *name, const char *mode, FS_FILE *fp);
FS_u32 FS__fat_dir_size(int dev_idx, FS_u32 unit, FS_u32 curdir);
FS_u32 FS__fat_dir_realsec(int idx, FS_u32 unit, FS_u32 curdir, FS_u32 dsec);
void FS__fat_make_realname(char *dst, const char *src);
FS_u32 FS__fat_find_dir(int idx, FS_u32 unit, char *dirname, FS_u32 curdir, FS_u32 curdir_size);
FS_u32 FS__fat_findpath(int idx, const char *name, char *realname, FS_FARCHARPTR *fname, FS_u32 *unit, FS_u32 *dstart);

/* fat_ioctl */
int FS__fat_ioctl(const FS__device_type *driver, FS_u32 id, FS_i32 cmd, FS_i32 aux, void *buffer);

/* fat_dir */
#if FS_POSIX_DIR_SUPPORT
FS_DIR *FS__fat_opendir(const char *dirname, FS_DIR *dirp);
int FS__fat_closedir(FS_DIR *dirp);
struct FS_DIRENT *FS__fat_readdir(FS_DIR *dirp);
int  FS__fat_mkdir(const char *dirname, FS_DIR *dirp);
#endif /* FS_POSIX_DIR_SUPPORT */


#endif  /* _FS_FAT_H_ */


