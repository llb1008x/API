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
File        : fs_fsl.h
Purpose     : Define structures for File-System-Layer
----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
None.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FS_FSL_H_
#define _FS_FSL_H_

/*********************************************************************
*
*             Global data types
*
**********************************************************************
*/

typedef struct {
    FS_FARCHARPTR name;
    FS_FILE *   (*fsl_fopen)(const char *name, const char *mode, FS_FILE *fp);
    void        (*fsl_fclose)(FS_FILE *fp);
    FS_size_t   (*fsl_fread)(void *ptr, FS_size_t size, FS_size_t n, FS_FILE *fp);
    FS_size_t   (*fsl_fwrite)(const void *ptr, FS_size_t size, FS_size_t n, FS_FILE *fp);
    long        (*fsl_ftell)(FS_FILE *fp);
    int         (*fsl_fseek)(FS_FILE *fp, long int offset, int whence);
    int         (*fsl_ioctl)(const FS__device_type *driver, FS_u32 id, FS_i32 cmd, FS_i32 aux, void *buffer);
#if FS_POSIX_DIR_SUPPORT
    FS_DIR *            (*fsl_opendir)(const char *dirname, FS_DIR *dirp);
    int                 (*fsl_closedir)(FS_DIR *dirp);
    struct FS_DIRENT *  (*fsl_readdir)(FS_DIR *dirp);
    void                (*fsl_rewinddir)(FS_DIR *dirp);
    int                 (*fsl_mkdir)(const char *dirname, FS_DIR *dirp);
    int                 (*fsl_rmdir)(const char *path);
#endif  /* FS_POSIX_DIR_SUPPORT */
} FS__fsl_type;


#endif  /* _FS_FSL_H_ */

