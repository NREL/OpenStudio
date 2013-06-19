   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*          PROCEDURAL FUNCTIONS HEADER FILE           */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_prcdrfun

#define _H_prcdrfun

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _PRCDRFUN_SOURCE
#define LOCALE
#else
#define LOCALE extern
#endif

typedef struct loopCounterStack
  {
   long long loopCounter;
   struct loopCounterStack *nxt;
  } LOOP_COUNTER_STACK;

#define PRCDRFUN_DATA 13

struct procedureFunctionData
  { 
   int ReturnFlag;
   int BreakFlag;
   LOOP_COUNTER_STACK *LoopCounterStack;
   struct dataObject *BindList;
  };

#define ProcedureFunctionData(theEnv) ((struct procedureFunctionData *) GetEnvironmentData(theEnv,PRCDRFUN_DATA))

   LOCALE void                           ProceduralFunctionDefinitions(void *);
   LOCALE void                           WhileFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                           LoopForCountFunction(void *,DATA_OBJECT_PTR);
   LOCALE long long                      GetLoopCount(void *);
   LOCALE void                           IfFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                           BindFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                           PrognFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                           ReturnFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                           BreakFunction(void *);
   LOCALE void                           SwitchFunction(void *,DATA_OBJECT_PTR);
   LOCALE intBool                        GetBoundVariable(void *,struct dataObject *,struct symbolHashNode *);
   LOCALE void                           FlushBindList(void *);

#endif






