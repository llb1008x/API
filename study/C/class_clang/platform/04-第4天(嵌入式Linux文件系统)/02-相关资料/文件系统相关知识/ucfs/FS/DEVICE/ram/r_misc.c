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
File        : r_misc.c 
Purpose     : Device Driver for simple array in RAM
----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
None.
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*             #include Section
*
**********************************************************************
*/

#include "fs_port.h"
#include "fs_dev.h" 
#include "fs_lbl.h" 
#include "fs_conf.h"
#include <string.h>

#if FS_USE_RAMDISK_DRIVER

/*********************************************************************
*
*             Local Variables        
*
**********************************************************************
*/

static char _array[(long)FS_RR_BLOCKNUM * FS_RR_BLOCKSIZE];


/*********************************************************************
*
*             Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*             _FS_RAM_DevStatus
*/

static int _FS_RAM_DevStatus(FS_u32 id) {
  static int online[1];

  if (id!=0) {
    return -1;
  }
  if (!online[id]) {
    online[id] = 1;
    return FS_LBL_MEDIACHANGED;
  }
  return 0;
}


/*********************************************************************
*
*             _FS_RAM_DevRead
*/

static int _FS_RAM_DevRead(FS_u32 id, FS_u32 block, void *buffer) {
  if (id!=0) {
    return -1;
  }
  memcpy(buffer,((char*)&_array[0])+block*FS_RR_BLOCKSIZE,
        (size_t) FS_RR_BLOCKSIZE);
  return 0;
}


/*********************************************************************
*
*             _FS_RAM_DevWrite
*/

static int _FS_RAM_DevWrite(FS_u32 id, FS_u32 block, void *buffer) {
  if (id!=0) {
    return -1;
  }
  memcpy(((char*)&_array[0])+block*FS_RR_BLOCKSIZE,buffer,
        (size_t) FS_RR_BLOCKSIZE);
  return 0;
}


/*********************************************************************
*
*             _FS_RAM_DevIoCtl
*/

static int _FS_RAM_DevIoCtl(FS_u32 id, FS_i32 cmd, FS_i32 aux, void *buffer) {
  /* make some compilers happy */
  cmd = cmd;
  aux = aux;
  buffer = buffer;
  if (id!=0) {
    return -1;
  }
  return 0;
}


/*********************************************************************
*
*             Global variables
*
**********************************************************************
*/

const FS__device_type FS__ramdevice_driver = {
  "RAMDISK device",
  _FS_RAM_DevStatus,
  _FS_RAM_DevRead,
  _FS_RAM_DevWrite,
  _FS_RAM_DevIoCtl
};

#endif /* FS_USE_RAMDISK_DRIVER */

