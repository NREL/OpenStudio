   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*           EXPRESSION BLOAD/BSAVE HEADER FILE        */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the binary save/load feature for the  */
/*    expression data structure.                             */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_exprnbin
#define _H_exprnbin

#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

#ifdef LOCALE
#undef LOCALE
#endif
#ifdef _EXPRNBIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define ExpressionPointer(i) ((struct expr *) (((i) == -1L) ? NULL : &ExpressionData(theEnv)->ExpressionArray[i]))
#define HashedExpressionPointer(i) ExpressionPointer(i)

   LOCALE void                        AllocateExpressions(void *);
   LOCALE void                        RefreshExpressions(void *);
   LOCALE void                        ClearBloadedExpressions(void *);
   LOCALE void                        FindHashedExpressions(void *);
   LOCALE void                        BsaveHashedExpressions(void *,FILE *);
   LOCALE void                        BsaveConstructExpressions(void *,FILE *);
   LOCALE void                        BsaveExpression(void *,struct expr *,FILE *);

#endif







