   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.20  01/31/02          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Deffunction Construct Compiler Code              */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_dffnxcmp
#define _H_dffnxcmp

#if DEFFUNCTION_CONSTRUCT && CONSTRUCT_COMPILER && (! RUN_TIME)

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

#include "dffnxfun.h"

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _DFFNXCMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void SetupDeffunctionCompiler(void *);
LOCALE void PrintDeffunctionReference(void *,FILE *,DEFFUNCTION *,int,int);
LOCALE void DeffunctionCModuleReference(void *,FILE *,int,int,int);

#endif

#endif


