   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*            EXPRESSION PARSER HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for parsing expressions.       */
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

#ifndef _H_exprnpsr

#define _H_exprnpsr

#if (! RUN_TIME)

typedef struct saved_contexts
  {
   int rtn;
   int brk;
   struct saved_contexts *nxt;
  } SAVED_CONTEXTS;

#endif

#ifndef _H_extnfunc
#include "extnfunc.h"
#endif
#ifndef _H_scanner
#include "scanner.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _EXPRNPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define GetSequenceOperatorRecognition() EnvGetSequenceOperatorRecognition(GetCurrentEnvironment())
#define SetSequenceOperatorRecognition(a) EnvSetSequenceOperatorRecognition(GetCurrentEnvironment(),a)

   LOCALE struct expr                   *Function0Parse(void *,char *);
   LOCALE struct expr                   *Function1Parse(void *,char *);
   LOCALE struct expr                   *Function2Parse(void *,char *,char *);
   LOCALE void                           PushRtnBrkContexts(void *);
   LOCALE void                           PopRtnBrkContexts(void *);
   LOCALE intBool                        ReplaceSequenceExpansionOps(void *,struct expr *,struct expr *,
                                                                     void *,void *);
   LOCALE struct expr                   *CollectArguments(void *,struct expr *,char *);
   LOCALE struct expr                   *ArgumentParse(void *,char *,int *);
   LOCALE struct expr                   *ParseAtomOrExpression(void *,char *,struct token *);
   LOCALE EXPRESSION                    *ParseConstantArguments(void *,char *,int *);
   LOCALE intBool                        EnvSetSequenceOperatorRecognition(void *,int);
   LOCALE intBool                        EnvGetSequenceOperatorRecognition(void *);
   LOCALE struct expr                   *GroupActions(void *,char *,struct token *,int,char *,int);
   LOCALE struct expr                   *RemoveUnneededProgn(void *,struct expr *);
#if (! RUN_TIME)
   LOCALE int                     CheckExpressionAgainstRestrictions(void *,struct expr *,char *,char *);
#endif

#endif




