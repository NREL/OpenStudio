   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.20  01/31/02          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_genrcbin
#define _H_genrcbin

#include "genrcfun.h"

#define GENRCBIN_DATA 28

struct defgenericBinaryData
  { 
   DEFGENERIC *DefgenericArray;
   long ModuleCount;
   long GenericCount;
   long MethodCount;
   long RestrictionCount;
   long TypeCount;
   DEFGENERIC_MODULE *ModuleArray;
   DEFMETHOD *MethodArray;
   RESTRICTION *RestrictionArray;
   void **TypeArray;
  };
  
#define DefgenericBinaryData(theEnv) ((struct defgenericBinaryData *) GetEnvironmentData(theEnv,GENRCBIN_DATA))

#define GenericPointer(i) (((i) == -1L) ? NULL : (DEFGENERIC *) &DefgenericBinaryData(theEnv)->DefgenericArray[i])

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _GENRCBIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void SetupGenericsBload(void *);
LOCALE void *BloadDefgenericModuleReference(void *,int);

#endif




