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
File        : fs_conf.c
Purpose     : File system configurable global data
              Unless you are going to add own device drivers, or you
              would like to modify e.g. order in the device table, you
              do not have to modify this file. Usually all configuration
              can be made in fs_conf.h.
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
#include "fs_api.h"
#include "fs_fsl.h"
#include "fs_int.h"
#include "fs_conf.h"


/*********************************************************************
*
*             Externals
*
**********************************************************************
*/

/*********************************************************************
*
*             File System Layer Function Tables
*/

/* FAT function table */
#ifdef FS_USE_FAT_FSL
 extern const FS__fsl_type FS__fat_functable;
#endif /* FS_USE_FAT_FSL */


/*********************************************************************
*
*             Device Driver Function Tables
*
  If you add an own device driver, you will have to add an external
  for its function table here.
*/

/* RAMDISK_DRIVER function table */
#if FS_USE_RAMDISK_DRIVER
  extern const FS__device_type    FS__ramdevice_driver;
#endif  /* FS_USE_RAMDISK_DRIVER */

/* WINDRIVE_DRIVER function table */
#if FS_USE_WINDRIVE_DRIVER
  extern const FS__device_type    FS__windrive_driver;
#endif  /* FS_USE_WINDRIVE_DRIVER */

/* SMC_DRIVER function table */
#if FS_USE_SMC_DRIVER
  extern const FS__device_type    FS__smcdevice_driver;
#endif /* FS_USE_SMC_DRIVER */

/* MMC_DRIVER function table */
#if FS_USE_MMC_DRIVER
  extern const FS__device_type    FS__mmcdevice_driver;
#endif /* FS_USE_SMC_DRIVER */

#if FS_USE_IDE_DRIVER
  extern const FS__device_type    FS__idedevice_driver;
#endif /* FS_USE_IDE_DRIVER */


/*********************************************************************
*
*             Global variables        
*
**********************************************************************
*/

/*********************************************************************
*
*             _FS_devinfo
*
  This data structure does tell the file system, which file system layer
  (currently FAT only) and device driver (e.g. SmartMedia or RAM disk) is
  used for which device name.
  If you do not specify the device name in an file system API call, the
  first entry in this table will be used.
*/

#ifndef FS_DEVINFO
  #if FS_USE_SMC_DRIVER
    #define FS_DEVINFO_DEVSMC     { "smc",    &FS__fat_functable, &FS__smcdevice_driver, 0 },
  #else
    #define FS_DEVINFO_DEVSMC
  #endif
  #if FS_USE_MMC_DRIVER
    #define FS_DEVINFO_DEVMMC     { "mmc",    &FS__fat_functable, &FS__mmcdevice_driver, 0 },
  #else
    #define FS_DEVINFO_DEVMMC
  #endif
  #if FS_USE_IDE_DRIVER
    #define FS_DEVINFO_DEVIDE     { "ide",    &FS__fat_functable, &FS__idedevice_driver, 0 },
  #else
    #define FS_DEVINFO_DEVIDE
  #endif /* FS_USE_IDE_DRIVER */
  #if FS_USE_WINDRIVE_DRIVER
    #define FS_DEVINFO_DEVWINDRV  { "windrv", &FS__fat_functable, &FS__windrive_driver,  0 },
  #else
    #define FS_DEVINFO_DEVWINDRV
  #endif
  #if FS_USE_RAMDISK_DRIVER
    #define FS_DEVINFO_DEVRAM     { "ram",    &FS__fat_functable, &FS__ramdevice_driver, 0 },
  #else
    #define FS_DEVINFO_DEVRAM
  #endif
  #define FS_DEVINFO FS_DEVINFO_DEVSMC FS_DEVINFO_DEVMMC FS_DEVINFO_DEVIDE FS_DEVINFO_DEVWINDRV FS_DEVINFO_DEVRAM
#endif  /* FS_DEVINFO */

const FS__devinfo_type _FS__devinfo[] = { FS_DEVINFO };

const FS__devinfo_type *FS__pDevInfo = _FS__devinfo;

const unsigned int FS__maxdev=sizeof(_FS__devinfo)/sizeof(FS__devinfo_type);

const unsigned int FS__fat_maxunit=FS_FAT_MAXUNIT;


