   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  05/17/06          */
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
/*      6.24: Converted INSTANCE_PATTERN_MATCHING to          */
/*            DEFRULE_CONSTRUCT.                              */
/*                                                            */
/*************************************************************/

#ifndef _H_objrtcmp
#define _H_objrtcmp

#if DEFRULE_CONSTRUCT && OBJECT_SYSTEM && (! RUN_TIME) && CONSTRUCT_COMPILER

#ifndef _STDIO_INCLUDED_
#include <stdio.h>
#define _STDIO_INCLUDED_
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _OBJRTCMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void ObjectPatternsCompilerSetup(void *);
LOCALE void ObjectPatternNodeReference(void *,void *,FILE *,int,int);

#endif

#endif



