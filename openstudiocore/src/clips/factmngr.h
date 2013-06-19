   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*              FACTS MANAGER HEADER FILE              */
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
/*      6.23: Added support for templates maintaining their  */
/*            own list of facts.                             */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_factmngr

#define _H_factmngr

struct fact;

#ifndef _H_facthsh
#include "facthsh.h"
#endif
#ifndef _H_conscomp
#include "conscomp.h"
#endif
#ifndef _H_pattern
#include "pattern.h"
#endif
#include "multifld.h"
#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_tmpltdef
#include "tmpltdef.h"
#endif

struct fact
  {
   struct patternEntity factHeader;
   struct deftemplate *whichDeftemplate;
   void *list;
   long long factIndex;
   unsigned long hashValue;
   unsigned int depth : 15;
   unsigned int garbage : 1;
   struct fact *previousFact;
   struct fact *nextFact;
   struct fact *previousTemplateFact;
   struct fact *nextTemplateFact;
   struct multifield theProposition;
  };
  
#define FACTS_DATA 3

struct factsData
  {
   int ChangeToFactList;
#if DEBUGGING_FUNCTIONS
   unsigned WatchFacts;
#endif
   struct fact DummyFact;
   struct fact *GarbageFacts;
   struct fact *LastFact;
   struct fact *FactList;
   long long NextFactIndex;
   unsigned long NumberOfFacts;
   struct patternEntityRecord  FactInfo;
#if (! RUN_TIME) && (! BLOAD_ONLY)
   struct deftemplate *CurrentDeftemplate;
#endif
#if DEFRULE_CONSTRUCT && (! RUN_TIME) && DEFTEMPLATE_CONSTRUCT && CONSTRUCT_COMPILER
   struct CodeGeneratorItem *FactCodeItem;
#endif
   struct factHashEntry **FactHashTable;
   unsigned long FactHashTableSize;
   intBool FactDuplication;
#if DEFRULE_CONSTRUCT
   struct fact             *CurrentPatternFact;
   struct multifieldMarker *CurrentPatternMarks;
#endif
   long LastModuleIndex;
  };
  
#define FactData(theEnv) ((struct factsData *) GetEnvironmentData(theEnv,FACTS_DATA))

#ifdef LOCALE
#undef LOCALE
#endif
#ifdef _FACTMNGR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define Assert(a) EnvAssert(GetCurrentEnvironment(),a)
#define AssertString(a) EnvAssertString(GetCurrentEnvironment(),a)
#define AssignFactSlotDefaults(a) EnvAssignFactSlotDefaults(GetCurrentEnvironment(),a)
#define CreateFact(a) EnvCreateFact(GetCurrentEnvironment(),a)
#define DecrementFactCount(a) EnvDecrementFactCount(GetCurrentEnvironment(),a)
#define GetFactListChanged() EnvGetFactListChanged(GetCurrentEnvironment())
#define GetFactPPForm(a,b,c) EnvGetFactPPForm(GetCurrentEnvironment(),a,b,c)
#define GetNextFact(a) EnvGetNextFact(GetCurrentEnvironment(),a)
#define IncrementFactCount(a) EnvIncrementFactCount(GetCurrentEnvironment(),a)
#define PutFactSlot(a,b,c) EnvPutFactSlot(GetCurrentEnvironment(),a,b,c)
#define Retract(a) EnvRetract(GetCurrentEnvironment(),a)
#define SetFactListChanged(a) EnvSetFactListChanged(GetCurrentEnvironment(),a)

#if ALLOW_ENVIRONMENT_GLOBALS
   LOCALE intBool                        GetFactSlot(void *,char *,DATA_OBJECT *);
   LOCALE long long                      FactIndex(void *);
#endif

   LOCALE void                          *EnvAssert(void *,void *);
   LOCALE void                          *EnvAssertString(void *,char *);
   LOCALE struct fact                   *EnvCreateFact(void *,void *);
   LOCALE void                           EnvDecrementFactCount(void *,void *);
   LOCALE long long                      EnvFactIndex(void *,void *);
   LOCALE intBool                        EnvGetFactSlot(void *,void *,char *,DATA_OBJECT *);
   LOCALE void                           PrintFactWithIdentifier(void *,char *,struct fact *);
   LOCALE void                           PrintFact(void *,char *,struct fact *,int,int);
   LOCALE void                           PrintFactIdentifierInLongForm(void *,char *,void *);
   LOCALE intBool                        EnvRetract(void *,void *);
   LOCALE void                           RemoveAllFacts(void *);
   LOCALE struct fact                   *CreateFactBySize(void *,unsigned);
   LOCALE void                           FactInstall(void *,struct fact *);
   LOCALE void                           FactDeinstall(void *,struct fact *);
   LOCALE void                          *EnvGetNextFact(void *,void *);
   LOCALE void                          *GetNextFactInScope(void *theEnv,void *);
   LOCALE void                           EnvGetFactPPForm(void *,char *,unsigned,void *);
   LOCALE int                            EnvGetFactListChanged(void *);
   LOCALE void                           EnvSetFactListChanged(void *,int);
   LOCALE unsigned long                  GetNumberOfFacts(void *);
   LOCALE void                           InitializeFacts(void *);
   LOCALE struct fact                   *FindIndexedFact(void *,long long);
   LOCALE void                           EnvIncrementFactCount(void *,void *);
   LOCALE void                           PrintFactIdentifier(void *,char *,void *);
   LOCALE void                           DecrementFactBasisCount(void *,void *);
   LOCALE void                           IncrementFactBasisCount(void *,void *);
   LOCALE void                           ReturnFact(void *,struct fact *);
   LOCALE void                           MatchFactFunction(void *,void *);
   LOCALE intBool                        EnvPutFactSlot(void *,void *,char *,DATA_OBJECT *);
   LOCALE intBool                        EnvAssignFactSlotDefaults(void *,void *);
   LOCALE intBool                        CopyFactSlotValues(void *,void *,void *);
   LOCALE intBool                        DeftemplateSlotDefault(void *,struct deftemplate *,
                                                                struct templateSlot *,DATA_OBJECT *,int);

#ifndef _FACTMNGR_SOURCE_
   extern int                            ChangeToFactList;
   extern struct fact                    DummyFact;
#if DEBUGGING_FUNCTIONS
   extern unsigned                       WatchFacts;
#endif
#endif

#endif





