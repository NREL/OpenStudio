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

#ifndef _H_dfinsbin
#define _H_dfinsbin

#if DEFINSTANCES_CONSTRUCT && (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE)

#ifndef _H_defins
#include "defins.h"
#endif

#define DFINSBIN_DATA 25

struct definstancesBinaryData
  { 
   DEFINSTANCES *DefinstancesArray;
   long DefinstancesCount;
   long ModuleCount;
   DEFINSTANCES_MODULE *ModuleArray;
  };
  
#define DefinstancesBinaryData(theEnv) ((struct definstancesBinaryData *) GetEnvironmentData(theEnv,DFINSBIN_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _DFINSBIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void SetupDefinstancesBload(void *);
LOCALE void *BloadDefinstancesModuleRef(void *,int);

#ifndef _DFINSBIN_SOURCE_
#endif

#endif

#endif



