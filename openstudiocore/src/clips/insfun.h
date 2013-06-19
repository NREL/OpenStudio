   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  05/17/06          */
   /*                                                     */
   /*                INSTANCE FUNCTIONS MODULE            */
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
/*      6.24: Link error occurs for the SlotExistError       */
/*            function when OBJECT_SYSTEM is set to 0 in     */
/*            setup.h. DR0865                                */
/*                                                           */
/*            Converted INSTANCE_PATTERN_MATCHING to         */
/*            DEFRULE_CONSTRUCT.                             */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Moved EvaluateAndStoreInDataObject to          */
/*            evaluatn.c                                     */
/*                                                           */
/*************************************************************/

#ifndef _H_insfun
#define _H_insfun

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif
#ifndef _H_object
#include "object.h"
#endif

#ifndef _H_pattern
#include "pattern.h"
#endif

typedef struct igarbage
  {
   INSTANCE_TYPE *ins;
   struct igarbage *nxt;
  } IGARBAGE;

#define INSTANCE_TABLE_HASH_SIZE 8191
#define InstanceSizeHeuristic(ins)      sizeof(INSTANCE_TYPE)

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _INSFUN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define DecrementInstanceCount(a) EnvDecrementInstanceCount(GetCurrentEnvironment(),a)
#define GetInstancesChanged() EnvGetInstancesChanged(GetCurrentEnvironment())
#define IncrementInstanceCount(a) EnvIncrementInstanceCount(GetCurrentEnvironment(),a)
#define SetInstancesChanged(a) EnvSetInstancesChanged(GetCurrentEnvironment(),a)

LOCALE void EnvIncrementInstanceCount(void *,void *);
LOCALE void EnvDecrementInstanceCount(void *,void *);
LOCALE void InitializeInstanceTable(void *);
LOCALE void CleanupInstances(void *);
LOCALE unsigned HashInstance(SYMBOL_HN *);
LOCALE void DestroyAllInstances(void *);
LOCALE void RemoveInstanceData(void *,INSTANCE_TYPE *);
LOCALE INSTANCE_TYPE *FindInstanceBySymbol(void *,SYMBOL_HN *);
LOCALE INSTANCE_TYPE *FindInstanceInModule(void *,SYMBOL_HN *,struct defmodule *,
                                           struct defmodule *,unsigned);
LOCALE INSTANCE_SLOT *FindInstanceSlot(void *,INSTANCE_TYPE *,SYMBOL_HN *);
LOCALE int FindInstanceTemplateSlot(void *,DEFCLASS *,SYMBOL_HN *);
LOCALE int PutSlotValue(void *,INSTANCE_TYPE *,INSTANCE_SLOT *,DATA_OBJECT *,DATA_OBJECT *,char *);
LOCALE int DirectPutSlotValue(void *,INSTANCE_TYPE *,INSTANCE_SLOT *,DATA_OBJECT *,DATA_OBJECT *);
LOCALE intBool ValidSlotValue(void *,DATA_OBJECT *,SLOT_DESC *,INSTANCE_TYPE *,char *);
LOCALE INSTANCE_TYPE *CheckInstance(void *,char *);
LOCALE void NoInstanceError(void *,char *,char *);
LOCALE void StaleInstanceAddress(void *,char *,int);
LOCALE int EnvGetInstancesChanged(void *);
LOCALE void EnvSetInstancesChanged(void *,int);
LOCALE void PrintSlot(void *,char *,SLOT_DESC *,INSTANCE_TYPE *,char *);
LOCALE void PrintInstanceNameAndClass(void *,char *,INSTANCE_TYPE *,intBool);
LOCALE void PrintInstanceName(void *,char *,void *);
LOCALE void PrintInstanceLongForm(void *,char *,void *);

#if DEFRULE_CONSTRUCT && OBJECT_SYSTEM
LOCALE void DecrementObjectBasisCount(void *,void *);
LOCALE void IncrementObjectBasisCount(void *,void *);
LOCALE void MatchObjectFunction(void *,void *);
LOCALE intBool NetworkSynchronized(void *,void *);
#endif

#endif







