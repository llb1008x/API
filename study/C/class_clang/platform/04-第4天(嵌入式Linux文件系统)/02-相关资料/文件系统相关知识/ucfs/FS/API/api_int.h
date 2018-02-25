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
File        : api_int.h 
Purpose     : Internals used accross different modules of API layer
----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
None.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FS_API_INT_H_
#define _FS_API_INT_H_

/*********************************************************************
*
*             Global function prototypes
*
**********************************************************************
*/

int FS__find_fsl(const char *name, FS_FARCHARPTR *fname);


#endif  /* _FS_API_INT_H_ */

