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
File        : fs_api.h 
Purpose     : Define global functions and types to be used by an
              application using the file system.

              This file needs to be included by any modul using the
              file system.
----------------------------------------------------------------------
Version-Date-----Author-Explanation
----------------------------------------------------------------------
1.00.00 20020815        First release
1.00.01 20020821        Internal modifications
1.10.00 20020927        MMC & SD card driver added
1.20.00 20021010        IDE & CF driver added
1.21.00 20021025        FAT32 added / POSIX like directory support added
1.22.00 20021028        Internal modifications
1.22.01 20021031        Experimental DSP support added (internal version)
1.23.00 20021202        Intermediate release with switches FS_FAT_NOFAT32
                        and FS_FAT_NOFORMAT. Old manual V1.20 is provided,
                        FS_RmDir is not supported.
1.24.00 20021206        Official successor of 1.20.00
1.24.01 20030110        Internal test version
1.26.00 20030113        Support for trial version added.
                        FS__fat_malloc returns cleared buffer.
                        MMC driver deactivates CS whenever allowed by specification.
----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
 - FS_RmDir is currently not supported
 - FAT32 medias cannot be formatted
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FS_API_H_
#define _FS_API_H_

/*********************************************************************
*
*             #include Section
*
**********************************************************************
*/

#include "fs_port.h"
#include "fs_conf.h"

/*********************************************************************
*
*             #define constants
*
**********************************************************************
*/

/* File system version */
#define FS_VERSION          0x00012600UL   /* 1.26.00 */

/* Maximum size of a directory name */
#define FS_DIRNAME_MAX      255

/* Global error codes */
#define FS_ERR_OK           (FS_i16) 0x0000
#define FS_ERR_EOF          (FS_i16) 0xfff0
#define FS_ERR_DISKFULL     (FS_i16) 0xffe0
#define FS_ERR_INVALIDPAR   (FS_i16) 0xffd0
#define FS_ERR_WRITEONLY    (FS_i16) 0xffc0
#define FS_ERR_READONLY     (FS_i16) 0xffb0
#define FS_ERR_READERROR    (FS_i16) 0xffa0
#define FS_ERR_WRITEERROR   (FS_i16) 0xff90
#define FS_ERR_DISKCHANGED  (FS_i16) 0xff80
#define FS_ERR_CLOSE        (FS_i16) 0xff70

/* Global constants*/
#define FS_SEEK_CUR         1
#define FS_SEEK_END         2
#define FS_SEEK_SET         0

/* I/O commands */
#define FS_CMD_FLUSH_CACHE    1000L
#define FS_CMD_CHK_DSKCHANGE  1010L
#define FS_CMD_FORMAT_MEDIA   2222L
#define FS_CMD_INC_BUSYCNT    3001L
#define FS_CMD_DEC_BUSYCNT    3002L

/* known medias */
#define FS_MEDIA_SMC_1MB    1010L
#define FS_MEDIA_SMC_2MB    1020L
#define FS_MEDIA_SMC_4MB    1030L
#define FS_MEDIA_SMC_8MB    1040L
#define FS_MEDIA_SMC_16MB   1050L
#define FS_MEDIA_SMC_32MB   1060L
#define FS_MEDIA_SMC_64MB   1070L
#define FS_MEDIA_SMC_128MB  1080L

#define FS_MEDIA_RAM_16KB   2010L
#define FS_MEDIA_RAM_64KB   2020L
#define FS_MEDIA_RAM_128KB  2030L
#define FS_MEDIA_RAM_256KB  2040L
#define FS_MEDIA_RAM_512KB  2050L

#define FS_MEDIA_FD_144MB   3040L

#define FS_MEDIA_MMC_32MB   4060L
#define FS_MEDIA_MMC_64MB   4070L
#define FS_MEDIA_MMC_128MB  4080L

#define FS_MEDIA_SD_16MB    5050L
#define FS_MEDIA_SD_64MB    5070L
#define FS_MEDIA_SD_128MB   5080L

#define FS_MEDIA_CF_32MB    6060L
#define FS_MEDIA_CF_64MB    6070L


/*********************************************************************
*
*             Global data types
*
**********************************************************************
*/

typedef struct {
  FS_u32 fileid_lo;          /* unique id for file (lo)      */
  FS_u32 fileid_hi;          /* unique id for file (hi)      */
  FS_u32 fileid_ex;          /* unique id for file (ex)      */
  FS_i32 filepos;            /* current position in file     */
  FS_i32 size;               /* size of file                 */
  int dev_index;             /* index in _FS_devinfo[]       */
  FS_i16 error;              /* error code                   */
  unsigned char inuse;       /* handle in use mark           */
  unsigned char mode_r;      /* mode READ                    */
  unsigned char mode_w;      /* mode WRITE                   */
  unsigned char mode_a;      /* mode APPEND                  */
  unsigned char mode_c;      /* mode CREATE                  */
  unsigned char mode_b;      /* mode BINARY                  */
} FS_FILE;


/*********************************************************************
*
*             directory types
*/

#if FS_POSIX_DIR_SUPPORT

#define FS_ino_t  int

struct FS_DIRENT {
  FS_ino_t  d_ino;                      /* to be POSIX conform */
  char      d_name[FS_DIRNAME_MAX]; 
};

typedef struct {
  struct FS_DIRENT  dirent;  /* cunrrent directory entry     */
  FS_u32 dirid_lo;           /* unique id for file (lo)      */
  FS_u32 dirid_hi;           /* unique id for file (hi)      */
  FS_u32 dirid_ex;           /* unique id for file (ex)      */
  FS_i32 dirpos;             /* current position in file     */
  FS_i32 size;               /* size of file                 */
  int dev_index;             /* index in _FS_devinfo[]       */
  FS_i16 error;              /* error code                   */
  unsigned char inuse;       /* handle in use mark           */
} FS_DIR;

#endif  /* FS_POSIX_DIR_SUPPORT */


/*********************************************************************
*
*             Global function prototypes
*
**********************************************************************
*/

/*********************************************************************
*
*             STD file I/O functions
*/

FS_FILE             *FS_FOpen(const char *name, const char *mode);
void                FS_FClose(FS_FILE *fp);
FS_size_t           FS_FRead(void *ptr, FS_size_t size, FS_size_t n, FS_FILE *fp);
FS_size_t           FS_FWrite(const void *ptr, FS_size_t size, FS_size_t n, FS_FILE *fp);


/*********************************************************************
*
*             file pointer handling
*/

int                 FS_FSeek(FS_FILE *fp, FS_i32 offset, int whence);
FS_i32              FS_FTell(FS_FILE *fp);


/*********************************************************************
*
*             I/O error handling
*/

FS_i16              FS_FError(FS_FILE *fp);
void                FS_ClearErr(FS_FILE *fp);


/*********************************************************************
*
*             file functions
*/

int                 FS_Remove(const char * name);


/*********************************************************************
*
*             IOCTL
*/

int                 FS_IoCtl(const char *device, FS_i32 cmd, FS_i32 aux, void *buffer);



/*********************************************************************
*
*             directory functions
*/

#if FS_POSIX_DIR_SUPPORT

FS_DIR              *FS_OpenDir(const char *dirname);
int                 FS_CloseDir(FS_DIR *dirp);
struct FS_DIRENT    *FS_ReadDir(FS_DIR *dirp);
void                FS_RewindDir(FS_DIR *dirp);
int                 FS_MkDir(const char *dirname);
int                 FS_RmDir(const char *dirname);

#endif  /* FS_POSIX_DIR_SUPPORT */


/*********************************************************************
*
*             file system control functions
*/

int                 FS_Init(void);
int                 FS_Exit(void);


#endif  /* _FS_API_H_ */


