   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*             ARGUMENT ACCESS HEADER FILE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides access routines for accessing arguments */
/*   passed to user or system functions defined using the    */
/*   DefineFunction protocol.                                */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Added IllegalLogicalNameMessage function.      */
/*                                                           */
/*************************************************************/

#ifndef _H_argacces

#define _H_argacces

#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _ARGACCES_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define RtnArgCount() EnvRtnArgCount(GetCurrentEnvironment())
#define ArgCountCheck(a,b,c) EnvArgCountCheck(GetCurrentEnvironment(),a,b,c)
#define ArgRangeCheck(a,b,c) EnvArgRangeCheck(GetCurrentEnvironment(),a,b,c)
#define RtnLexeme(a) EnvRtnLexeme(GetCurrentEnvironment(),a)
#define RtnDouble(a) EnvRtnDouble(GetCurrentEnvironment(),a)
#define RtnLong(a) EnvRtnLong(GetCurrentEnvironment(),a)
#define RtnUnknown(a,b) EnvRtnUnknown(GetCurrentEnvironment(),a,b)
#define ArgTypeCheck(a,b,c,d) EnvArgTypeCheck(GetCurrentEnvironment(),a,b,c,d)

   LOCALE int                            EnvRtnArgCount(void *);
   LOCALE int                            EnvArgCountCheck(void *,char *,int,int);
   LOCALE int                            EnvArgRangeCheck(void *,char *,int,int);
   LOCALE char                          *EnvRtnLexeme(void *,int);
   LOCALE double                         EnvRtnDouble(void *,int);
   LOCALE long long                      EnvRtnLong(void *,int);
   LOCALE struct dataObject             *EnvRtnUnknown(void *,int,struct dataObject *);
   LOCALE int                            EnvArgTypeCheck(void *,char *,int,int,struct dataObject *);
   LOCALE intBool                        GetNumericArgument(void *,struct expr *,char *,struct dataObject *,int,int);
   LOCALE char                          *GetLogicalName(void *,int,char *);
   LOCALE char                          *GetFileName(void *,char *,int);
   LOCALE char                          *GetConstructName(void *,char *,char *);
   LOCALE void                           ExpectedCountError(void *,char *,int,int);
   LOCALE void                           OpenErrorMessage(void *,char *,char *);
   LOCALE intBool                        CheckFunctionArgCount(void *,char *,char *,int);
   LOCALE void                           ExpectedReturnTypeError(char *,char *);
   LOCALE void                           ExpectedTypeError1(void *,char *,int,char *);
   LOCALE void                           ExpectedTypeError2(void *,char *,int);
   LOCALE struct defmodule              *GetModuleName(void *,char *,int,int *);
   LOCALE void                          *GetFactOrInstanceArgument(void *,int,DATA_OBJECT *,char *);
   LOCALE void                           IllegalLogicalNameMessage(void *,char *);

#endif






