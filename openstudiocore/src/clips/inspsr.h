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

#ifndef _H_inspsr
#define _H_inspsr

#ifndef _H_expressn
#include "expressn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _INSPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#if ! RUN_TIME
LOCALE EXPRESSION *ParseInitializeInstance(void *,EXPRESSION *,char *);
LOCALE EXPRESSION *ParseSlotOverrides(void *,char *,int *);
#endif

LOCALE EXPRESSION *ParseSimpleInstance(void *,EXPRESSION *,char *);

#ifndef _INSCOM_SOURCE_
#endif

#endif



