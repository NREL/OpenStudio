   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                 DEFRULE HEADER FILE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Defines basic defrule primitive functions such   */
/*   as allocating and deallocating, traversing, and finding */
/*   defrule data structures.                                */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Removed DYNAMIC_SALIENCE and                   */
/*            LOGICAL_DEPENDENCIES compilation flags.        */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*            Added additional developer statistics to help  */
/*            analyze join network performance.              */
/*                                                           */
/*            Added salience groups to improve performance   */
/*            with large numbers of activations of different */
/*            saliences.                                     */
/*                                                           */
/*************************************************************/

#ifndef _H_ruledef
#define _H_ruledef

#define GetDisjunctIndex(r) ((struct constructHeader *) r)->bsaveID

struct defrule;
struct defruleModule;

#ifndef _H_conscomp
#include "conscomp.h"
#endif
#ifndef _H_symbol
#include "symbol.h"
#endif
#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_constrct
#include "constrct.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif
#ifndef _H_constrnt
#include "constrnt.h"
#endif
#ifndef _H_cstrccom
#include "cstrccom.h"
#endif
#ifndef _H_agenda
#include "agenda.h"
#endif
#ifndef _H_network
#include "network.h"
#endif

struct defrule
  {
   struct constructHeader header;
   int salience;
   int localVarCnt;
   unsigned int complexity      : 11;
   unsigned int afterBreakpoint :  1;
   unsigned int watchActivation :  1;
   unsigned int watchFiring     :  1;
   unsigned int autoFocus       :  1;
   unsigned int executing       :  1;
   struct expr *dynamicSalience;
   struct expr *actions;
   struct joinNode *logicalJoin;
   struct joinNode *lastJoin;
   struct defrule *disjunct;
  };

struct defruleModule
  {
   struct defmoduleItemHeader header;
   struct salienceGroup *groupings;
   struct activation *agenda;
  };

#ifndef ALPHA_MEMORY_HASH_SIZE
#define ALPHA_MEMORY_HASH_SIZE       63559L
#endif

#define DEFRULE_DATA 16

struct defruleData
  { 
   struct construct *DefruleConstruct;
   int DefruleModuleIndex;
   long long CurrentEntityTimeTag;
   struct alphaMemoryHash **AlphaMemoryTable;
   intBool BetaMemoryResizingFlag;
   struct joinLink *RightPrimeJoins;
   struct joinLink *LeftPrimeJoins;

#if DEBUGGING_FUNCTIONS
    unsigned WatchRules;
    int DeletedRuleDebugFlags;
#endif
#if DEVELOPER && (! RUN_TIME) && (! BLOAD_ONLY)
    unsigned WatchRuleAnalysis;
#endif
#if CONSTRUCT_COMPILER && (! RUN_TIME)
   struct CodeGeneratorItem *DefruleCodeItem;
#endif
  };

#define EnvGetDefruleName(theEnv,x) GetConstructNameString((struct constructHeader *) x)
#define EnvGetDefrulePPForm(theEnv,x) GetConstructPPForm(theEnv,(struct constructHeader *) x)
#define EnvDefruleModule(theEnv,x) GetConstructModuleName((struct constructHeader *) x)

#define DefruleData(theEnv) ((struct defruleData *) GetEnvironmentData(theEnv,DEFRULE_DATA))

#define GetPreviousJoin(theJoin) \
   (((theJoin)->joinFromTheRight) ? \
    ((struct joinNode *) (theJoin)->rightSideEntryStructure) : \
    ((theJoin)->lastLevel))
#define GetPatternForJoin(theJoin) \
   (((theJoin)->joinFromTheRight) ? \
    NULL : \
    ((theJoin)->rightSideEntryStructure))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _RULEDEF_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define DefruleModule(x) GetConstructModuleName((struct constructHeader *) x)
#define FindDefrule(a) EnvFindDefrule(GetCurrentEnvironment(),a)
#define GetDefruleName(x) GetConstructNameString((struct constructHeader *) x)
#define GetDefrulePPForm(x) GetConstructPPForm(GetCurrentEnvironment(),(struct constructHeader *) x)
#define GetNextDefrule(a) EnvGetNextDefrule(GetCurrentEnvironment(),a)
#define IsDefruleDeletable(a) EnvIsDefruleDeletable(GetCurrentEnvironment(),a)

   LOCALE void                           InitializeDefrules(void *);
   LOCALE void                          *EnvFindDefrule(void *,char *);
   LOCALE void                          *EnvGetNextDefrule(void *,void *);
   LOCALE struct defruleModule          *GetDefruleModuleItem(void *,struct defmodule *);
   LOCALE intBool                        EnvIsDefruleDeletable(void *,void *);
#if RUN_TIME
   LOCALE void                           DefruleRunTimeInitialize(void *,struct joinLink *,struct joinLink *);
#endif
#if RUN_TIME || BLOAD_ONLY || BLOAD || BLOAD_AND_BSAVE
   LOCALE void                           AddBetaMemoriesToJoin(void *,struct joinNode *);
#endif

#endif


