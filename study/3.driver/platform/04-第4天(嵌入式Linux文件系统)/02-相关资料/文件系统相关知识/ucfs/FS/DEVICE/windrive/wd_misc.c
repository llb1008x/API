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
File        : wd_misc.c
Purpose     : Device Driver using Windows I/O function for logical sector
              access.
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
#include "fs_api.h"
#include <string.h>

#if FS_USE_WINDRIVE_DRIVER

#ifndef _WIN32
#error Driver requires Windows API
#endif /* _WIN32 */

#include <windows.h>


/*********************************************************************
*
*             Local data types
*
**********************************************************************
*/

typedef struct {
  FS_u32 block;
  char buffer[FS_WD_BLOCKSIZE];
} _FS_wd_cachetype;


/*********************************************************************
*
*             Local variables        
*
**********************************************************************
*/

static HANDLE _hdrive[FS_WD_MAXUNIT];
static _FS_wd_cachetype  _FS_wd_cache[FS_WD_MAXUNIT][FS_WD_CACHENUM];
static int _FS_wd_cache_index[FS_WD_MAXUNIT];
static _FS_wd_cachetype  _FS_wd_wbuffer[FS_WD_MAXUNIT][FS_WD_WBUFFNUM];
static int _online[FS_WD_MAXUNIT];


/*********************************************************************
*
*             Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*             _FS_WD_DevStatus
*/

static int _FS_WD_DevStatus(FS_u32 id) {
  int i;

  if (!_online[id]) {
    _online[id] = 1;
    if (id==0) {
      _hdrive[id] = CreateFile (FS_WD_DEV0NAME, 
                     GENERIC_READ | GENERIC_WRITE,
                     FILE_SHARE_READ|FILE_SHARE_WRITE,
                     NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                     NULL);
    }
    else {
      _hdrive[id] = CreateFile (FS_WD_DEV1NAME, 
                     GENERIC_READ | GENERIC_WRITE,
                     FILE_SHARE_READ|FILE_SHARE_WRITE,
                     NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                     NULL);
    }

    for (i=0;i<FS_WD_CACHENUM;i++) {
      _FS_wd_cache[id][i].block=-1;
    }
    _FS_wd_cache_index[id] = 0;

    for (i=0;i<FS_WD_WBUFFNUM;i++) {
      _FS_wd_wbuffer[id][i].block=-1;
    }

    return FS_LBL_MEDIACHANGED;
  }
  return 0;
}


/*********************************************************************
*
*             _FS_WD_DevRead
*/

static int _FS_WD_DevRead(FS_u32 id, FS_u32 block, void *buffer) {
  DWORD bytenum;
  int i;

  /* check if buffer in wbuffer */
  i = 0;
  while (i<FS_WD_WBUFFNUM) {
    if (_FS_wd_wbuffer[id][i].block==block) {
      memcpy(buffer,_FS_wd_wbuffer[id][i].buffer,512);
      return 0;
    }
    i++;
  }
  
  for (i=0;i<FS_WD_CACHENUM;i++) {
    if (_FS_wd_cache[id][i].block==block) {
      memcpy(buffer,_FS_wd_cache[id][i].buffer,512);
      return 0;
    }
  }

  SetFilePointer(_hdrive[id],block*512,0,FILE_BEGIN);
  ReadFile(_hdrive[id], buffer, 512, &bytenum, NULL);
  
  _FS_wd_cache[id][_FS_wd_cache_index[id]].block = block;
  memcpy(_FS_wd_cache[id][_FS_wd_cache_index[id]].buffer,buffer,512);
  _FS_wd_cache_index[id]++;
  if (_FS_wd_cache_index[id]>=FS_WD_CACHENUM) {
    _FS_wd_cache_index[id] = 0;
  }

  return 0;
}


/*********************************************************************
*
*             _FS_WD_DevWrite
*/

static int _FS_WD_DevWrite(FS_u32 id, FS_u32 block, void *buffer) {
  DWORD bytenum;
  int i,j;

  /* clear in read cache */
  for (i=0;i<FS_WD_CACHENUM;i++) {
    if (_FS_wd_cache[id][i].block==block) {
      _FS_wd_cache[id][i].block = -1;
    }
  }

  /* check if buffer in wbuffer */
  i = 0;
  while (i<FS_WD_WBUFFNUM) {
    if (_FS_wd_wbuffer[id][i].block==block) {
      memcpy(_FS_wd_wbuffer[id][i].buffer,buffer,512);
      return 0;
    }
    i++;
  }

  /* check for free wbuffer */
  i = 0;
  while (i<FS_WD_WBUFFNUM) {
    if (_FS_wd_wbuffer[id][i].block==-1) {
      memcpy(_FS_wd_wbuffer[id][i].buffer,buffer,512);
      _FS_wd_wbuffer[id][i].block = block;
      return 0;
    }
    i++;
  }

  /* write back complete cache  */
  i = 0;
  while ((_FS_wd_wbuffer[id][i].block==-1) && (i<FS_WD_WBUFFNUM)) {
    i++;
  }

  while (i<FS_WD_WBUFFNUM) {

    for (j=0;j<FS_WD_WBUFFNUM;j++) {
      if ((_FS_wd_wbuffer[id][j].block!=-1) &&
        (_FS_wd_wbuffer[id][j].block<_FS_wd_wbuffer[id][i].block)) {
        i = j;
      }
    }

    if (_FS_wd_wbuffer[id][i].block!=-1) {
      SetFilePointer(_hdrive[id],_FS_wd_wbuffer[id][i].block*512,0,FILE_BEGIN);
      WriteFile(_hdrive[id], _FS_wd_wbuffer[id][i].buffer,512, &bytenum, NULL);
      _FS_wd_wbuffer[id][i].block = -1;
    }

    i = 0;
    while ((_FS_wd_wbuffer[id][i].block==-1) && (i<FS_WD_WBUFFNUM)) {
      i++;
    }
  } 

  memcpy(_FS_wd_wbuffer[id][0].buffer,buffer,512);
  _FS_wd_wbuffer[id][0].block = block;


  return 0;
}


/*********************************************************************
*
*             _FS_WD_DevIoCtl
*/

static int _FS_WD_DevIoCtl(FS_u32 id, FS_i32 cmd, FS_i32 aux, void *buffer) {
  DWORD bytenum;
  int i;

  if (cmd==FS_CMD_FLUSH_CACHE) {
    if (_online[id]) {
      i = 0;
      while (i<FS_WD_WBUFFNUM) {
        if (_FS_wd_wbuffer[id][i].block!=-1) {
          SetFilePointer(_hdrive[id],_FS_wd_wbuffer[id][i].block*512,0,FILE_BEGIN);
          WriteFile(_hdrive[id], _FS_wd_wbuffer[id][i].buffer,512, &bytenum, NULL);
          _FS_wd_wbuffer[id][i].block = -1;
        }
        i++;
      }
      CloseHandle(_hdrive[id]);
      _online[id] = 0;
    }
  }
  return 0;
}


/*********************************************************************
*
*             Global variables        
*
**********************************************************************
*/

const FS__device_type FS__windrive_driver = {
  "Windrive driver",
  _FS_WD_DevStatus,
  _FS_WD_DevRead,
  _FS_WD_DevWrite,
  _FS_WD_DevIoCtl
};

#endif /* FS_USE_WINDRIVE_DRIVER */


