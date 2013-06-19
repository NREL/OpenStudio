   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.30  03/04/08          */
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
/*      6.30: Added support to allow CreateClassScopeMap to   */
/*            be used by other functions.                     */
/*                                                            */
/*************************************************************/

#ifndef _H_classpsr
#define _H_classpsr

#if OBJECT_SYSTEM && (! BLOAD_ONLY) && (! RUN_TIME)

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _CLASSPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE int ParseDefclass(void *,char *);

#if DEFMODULE_CONSTRUCT
LOCALE void *CreateClassScopeMap(void *,DEFCLASS *);
#endif

#endif

#endif



