   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*        FACT RHS PATTERN PARSER HEADER MODULE        */
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
/*************************************************************/

#ifndef _H_factrhs
#define _H_factrhs

#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_scanner
#include "scanner.h"
#endif
#ifndef _H_factmngr
#include "factmngr.h"
#endif
#ifndef _H_symbol
#include "symbol.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FACTRHS_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE struct expr                   *BuildRHSAssert(void *,char *,struct token *,int *,int,int,char *);
   LOCALE struct expr                   *GetAssertArgument(void *,char *,struct token *,int *,int,int,int *);
   LOCALE struct expr                   *GetRHSPattern(void *,char *,struct token *,int *,int,
                                                       int,int,int);
   LOCALE struct fact                   *StringToFact(void *,char *);

#endif

