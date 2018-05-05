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
File        : fs_int.h 
Purpose     : Internals used accross different layers of the file system
----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
None.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FS_INT_H_
#define _FS_INT_H_

/*********************************************************************
*
*             Global data types
*
**********************************************************************
*/

typedef struct {
  const char             *devname;
  const FS__fsl_type     *fs_ptr;
  const FS__device_type  *devdriver;
  const void             *data;
} FS__devinfo_type;


/*********************************************************************
*
*             Externals
*
**********************************************************************
*/

/* fs_info.c */
extern const FS__devinfo_type   *FS__pDevInfo;
extern const unsigned int       FS__maxdev;
extern const unsigned int       FS__fat_maxunit;

#endif  /* _FS_INT_H_ */

