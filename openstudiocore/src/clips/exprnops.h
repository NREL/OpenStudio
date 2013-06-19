   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*          EXPRESSION OPERATIONS HEADER FILE          */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides utility routines for manipulating and   */
/*   examining expressions.                                  */
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
/*************************************************************/

#ifndef _H_exprnops

#define _H_exprnops

#ifndef _H_expressn
#include "expressn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _EXPRNOPS_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE intBool                        ConstantExpression(struct expr *);
   LOCALE void                           PrintExpression(void *,char *,struct expr *);
   LOCALE long                           ExpressionSize(struct expr *);
   LOCALE int                            CountArguments(struct expr *);
   LOCALE struct expr                   *CopyExpression(void *,struct expr *);
   LOCALE intBool                        ExpressionContainsVariables(struct expr *,int);
   LOCALE intBool                        IdenticalExpression(struct expr *,struct expr *);
   LOCALE struct expr                   *GenConstant(void *,unsigned short,void *);
#if ! RUN_TIME
   LOCALE int                            CheckArgumentAgainstRestriction(void *,struct expr *,int);
#endif
   LOCALE intBool                        ConstantType(int);
   LOCALE struct expr                   *CombineExpressions(void *,struct expr *,struct expr *);
   LOCALE struct expr                   *AppendExpressions(struct expr *,struct expr *);

#endif


