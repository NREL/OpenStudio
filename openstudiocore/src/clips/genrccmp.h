   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.20  01/31/02          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Generic Function Construct Compiler Code         */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_genrccmp
#define _H_genrccmp

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _GENRCCMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

#include "genrcfun.h"

LOCALE void SetupGenericsCompiler(void *);
LOCALE void PrintGenericFunctionReference(void *,FILE *,DEFGENERIC *,int,int);
LOCALE void DefgenericCModuleReference(void *,FILE *,int,int,int);

#endif

