   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*             DEFGLOBAL PARSER HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_globlpsr

#define _H_globlpsr

#ifdef _DEFGLOBL_SOURCE_
struct defglobal;
#endif

#ifndef _H_expressn
#include "expressn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _GLOBLPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE intBool                 ParseDefglobal(void *,char *);
   LOCALE intBool                 ReplaceGlobalVariable(void *,struct expr *);
   LOCALE void                    GlobalReferenceErrorMessage(void *,char *);

#endif



