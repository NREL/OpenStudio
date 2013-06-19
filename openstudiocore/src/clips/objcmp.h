   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.20  01/31/02          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Object System Construct Compiler Code            */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_objcmp
#define _H_objcmp

#ifndef _STDIO_INCLUDED_
#include <stdio.h>
#define _STDIO_INCLUDED_
#endif

#ifndef _H_conscomp
#include "conscomp.h"
#endif
#ifndef _H_object
#include "object.h"
#endif

#define OBJECT_COMPILER_DATA 36

struct objectCompilerData
  { 
#if CONSTRUCT_COMPILER && (! RUN_TIME)
   struct CodeGeneratorItem *ObjectCodeItem;
#endif
  };

#define ObjectCompilerData(theEnv) ((struct objectCompilerData *) GetEnvironmentData(theEnv,OBJECT_COMPILER_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _OBJCMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void SetupObjectsCompiler(void *);
LOCALE void PrintClassReference(void *,FILE *,DEFCLASS *,int,int);
LOCALE void DefclassCModuleReference(void *,FILE *,int,int,int);

#endif

