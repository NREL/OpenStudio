   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*             BASIC MATH FUNCTIONS MODULE             */
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

#ifndef _H_bmathfun

#define _H_bmathfun

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _BMATHFUN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define GetAutoFloatDividend() EnvGetAutoFloatDividend(GetCurrentEnvironment())
#define SetAutoFloatDividend(a) EnvSetAutoFloatDividend(GetCurrentEnvironment(),a)

   LOCALE void                    BasicMathFunctionDefinitions(void *);
   LOCALE void                    AdditionFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                    MultiplicationFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                    SubtractionFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                    DivisionFunction(void *,DATA_OBJECT_PTR);
   LOCALE long long               DivFunction(void *);
   LOCALE intBool                 SetAutoFloatDividendCommand(void *);
   LOCALE intBool                 GetAutoFloatDividendCommand(void *);
   LOCALE intBool                 EnvGetAutoFloatDividend(void *);
   LOCALE intBool                 EnvSetAutoFloatDividend(void *,int);
   LOCALE long long               IntegerFunction(void *);
   LOCALE double                  FloatFunction(void *);
   LOCALE void                    AbsFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                    MinFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                    MaxFunction(void *,DATA_OBJECT_PTR);

#endif




