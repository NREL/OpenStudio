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

#ifndef _H_immthpsr
#define _H_immthpsr

#if DEFGENERIC_CONSTRUCT && (! BLOAD_ONLY) && (! RUN_TIME)

#include "genrcfun.h"

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _IMMTHPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void AddImplicitMethods(void *,DEFGENERIC *);

#ifndef _IMMTHPSR_SOURCE_
#endif

#endif

#endif




