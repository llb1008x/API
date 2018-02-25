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
File        : fs_lbl.h 
Purpose     : Header file for the file system's Logical Block Layer
----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
None.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FS_LBL_H_
#define _FS_LBL_H_

/*********************************************************************
*
*             #define constants
*
**********************************************************************
*/

#define FS_LBL_MEDIACHANGED     0x0001


/*********************************************************************
*
*             Global function prototypes
*
**********************************************************************
*/

int FS__lb_status(const FS__device_type *driver, FS_u32 id);
int FS__lb_read(const FS__device_type *driver, FS_u32 id, FS_u32 block, void *buffer);
int FS__lb_write(const FS__device_type *driver, FS_u32 id, FS_u32 block, void *buffer);
int FS__lb_ioctl(const FS__device_type *driver, FS_u32 id, FS_i32 cmd, FS_i32 aux, void *buffer);

#endif  /* _FS_LBL_H_  */


