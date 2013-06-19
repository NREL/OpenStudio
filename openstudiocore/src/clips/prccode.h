   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
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
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_prccode
#define _H_prccode

#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif
#ifndef _H_scanner
#include "scanner.h"
#endif
#ifndef _H_symbol
#include "symbol.h"
#endif

typedef struct ProcParamStack
  {
   DATA_OBJECT *ParamArray;

#if DEFGENERIC_CONSTRUCT
   EXPRESSION *ParamExpressions;
#endif

   int ParamArraySize;
   DATA_OBJECT *WildcardValue;
   void (*UnboundErrFunc)(void *);
   struct ProcParamStack *nxt;
  } PROC_PARAM_STACK;

#define PROCEDURAL_PRIMITIVE_DATA 37

struct proceduralPrimitiveData
  { 
   void *NoParamValue;
   DATA_OBJECT *ProcParamArray;
   int ProcParamArraySize;
   EXPRESSION *CurrentProcActions;
#if DEFGENERIC_CONSTRUCT
   EXPRESSION *ProcParamExpressions;
#endif
   PROC_PARAM_STACK *pstack;
   DATA_OBJECT *WildcardValue;
   DATA_OBJECT *LocalVarArray;
   void (*ProcUnboundErrFunc)(void *);
   ENTITY_RECORD ProcParameterInfo; 
   ENTITY_RECORD ProcWildInfo;
   ENTITY_RECORD ProcGetInfo;     
   ENTITY_RECORD ProcBindInfo;      
#if ! DEFFUNCTION_CONSTRUCT
   ENTITY_RECORD DeffunctionEntityRecord;
#endif
#if ! DEFGENERIC_CONSTRUCT
   ENTITY_RECORD GenericEntityRecord;
#endif
   int Oldindex;
  };

#define ProceduralPrimitiveData(theEnv) ((struct proceduralPrimitiveData *) GetEnvironmentData(theEnv,PROCEDURAL_PRIMITIVE_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _PRCCODE_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void InstallProcedurePrimitives(void *);

#if (! BLOAD_ONLY) && (! RUN_TIME)

#if DEFFUNCTION_CONSTRUCT || OBJECT_SYSTEM
LOCALE EXPRESSION *ParseProcParameters(void *,char *,struct token *,EXPRESSION *,
                                       SYMBOL_HN **,int *,int *,int *,
                                       int (*)(void *,char *));
#endif
LOCALE EXPRESSION *ParseProcActions(void *,char *,char *,struct token *,EXPRESSION *,SYMBOL_HN *,
                                    int (*)(void *,EXPRESSION *,void *),
                                    int (*)(void *,EXPRESSION *,void *),
                                    int *,void *);
LOCALE intBool ReplaceProcVars(void *,char *,EXPRESSION *,EXPRESSION *,SYMBOL_HN *,
                                     int (*)(void *,EXPRESSION *,void *),void *);
#if DEFGENERIC_CONSTRUCT
LOCALE EXPRESSION *GenProcWildcardReference(void *,int);
#endif
#endif

LOCALE void PushProcParameters(void *,EXPRESSION *,int,char *,char *,void (*)(void *));
LOCALE void PopProcParameters(void *);

#if DEFGENERIC_CONSTRUCT
LOCALE EXPRESSION *GetProcParamExpressions(void *);
#endif

LOCALE void EvaluateProcActions(void *,struct defmodule *,EXPRESSION *,int,
                                DATA_OBJECT *,void (*)(void *));
LOCALE void PrintProcParamArray(void *,char *);
LOCALE void GrabProcWildargs(void *,DATA_OBJECT *,int);

#endif





