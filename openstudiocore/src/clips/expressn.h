   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*               EXPRESSION HEADER FILE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains routines for creating, deleting,        */
/*   compacting, installing, and hashing expressions.        */
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

#ifndef _H_expressn

#define _H_expressn

struct expr;
struct exprHashNode;

#ifndef _H_exprnops
#include "exprnops.h"
#endif

/******************************/
/* Expression Data Structures */
/******************************/

struct expr
   {
    unsigned short type;
    void *value;
    struct expr *argList;
    struct expr *nextArg;
   };

#define arg_list argList
#define next_arg nextArg

typedef struct expr EXPRESSION;

typedef struct exprHashNode
  {
   unsigned hashval;
   unsigned count;
   struct expr *exp;
   struct exprHashNode *next;
   long bsaveID;
  } EXPRESSION_HN;

#define EXPRESSION_HASH_SIZE 503

/*************************/
/* Type and Value Macros */
/*************************/

#define GetType(target)         ((target).type)
#define GetpType(target)        ((target)->type)
#define SetType(target,val)     ((target).type = (unsigned short) (val))
#define SetpType(target,val)    ((target)->type = (unsigned short) (val))
#define GetValue(target)        ((target).value)
#define GetpValue(target)       ((target)->value)
#define SetValue(target,val)    ((target).value = (void *) (val))
#define SetpValue(target,val)   ((target)->value = (void *) (val))

#define EnvGetType(theEnv,target)         ((target).type)
#define EnvGetpType(theEnv,target)        ((target)->type)
#define EnvSetType(theEnv,target,val)     ((target).type = (unsigned short) (val))
#define EnvSetpType(theEnv,target,val)    ((target)->type = (unsigned short) (val))
#define EnvGetValue(theEnv,target)        ((target).value)
#define EnvGetpValue(theEnv,target)       ((target)->value)
#define EnvSetValue(theEnv,target,val)    ((target).value = (void *) (val))
#define EnvSetpValue(theEnv,target,val)   ((target)->value = (void *) (val))

/********************/
/* ENVIRONMENT DATA */
/********************/

#ifndef _H_exprnpsr
#include "exprnpsr.h"
#endif

#define EXPRESSION_DATA 45

struct expressionData
  { 
   void *PTR_AND;
   void *PTR_OR;
   void *PTR_EQ;
   void *PTR_NEQ;
   void *PTR_NOT;
   EXPRESSION_HN **ExpressionHashTable;
#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE)
   long NumberOfExpressions;
   struct expr *ExpressionArray;
   long int ExpressionCount;
#endif
#if (! RUN_TIME)
   SAVED_CONTEXTS *svContexts;
   int ReturnContext;
   int BreakContext;
#endif
   intBool SequenceOpMode;
  };

#define ExpressionData(theEnv) ((struct expressionData *) GetEnvironmentData(theEnv,EXPRESSION_DATA))

/********************/
/* Global Functions */
/********************/

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _EXPRESSN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           ReturnExpression(void *,struct expr *);
   LOCALE void                           ExpressionInstall(void *,struct expr *);
   LOCALE void                           ExpressionDeinstall(void *,struct expr *);
   LOCALE struct expr                   *PackExpression(void *,struct expr *);
   LOCALE void                           ReturnPackedExpression(void *,struct expr *);
   LOCALE void                           InitExpressionData(void *);
   LOCALE void                           InitExpressionPointers(void *);
#if (! BLOAD_ONLY) && (! RUN_TIME)
   LOCALE EXPRESSION                    *AddHashedExpression(void *,EXPRESSION *);
#endif
#if (! RUN_TIME)
   LOCALE void                           RemoveHashedExpression(void *,EXPRESSION *);
#endif
#if BLOAD_AND_BSAVE || BLOAD_ONLY || BLOAD || CONSTRUCT_COMPILER
   LOCALE long                           HashedExpressionIndex(void *,EXPRESSION *);
#endif

#endif




