   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*                 UTILITY HEADER FILE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides a set of utility functions useful to    */
/*   other modules. Primarily these are the functions for    */
/*   handling periodic garbage collection and appending      */
/*   string data.                                            */
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

#ifndef _H_utility
#define _H_utility

#ifdef LOCALE
#undef LOCALE
#endif

struct callFunctionItem
  {
   char *name;
   void (*func)(void *);
   int priority;
   struct callFunctionItem *next;
   short int environmentAware;
   void *context;
  };
  
struct trackedMemory
  {
   void *theMemory;
   struct trackedMemory *next;
   struct trackedMemory *prev;
   size_t memSize;
  };
  
#define UTILITY_DATA 55

struct utilityData
  { 
   struct callFunctionItem *ListOfCleanupFunctions;
   struct callFunctionItem *ListOfPeriodicFunctions;
   short GarbageCollectionLocks;
   short GarbageCollectionHeuristicsEnabled;
   short PeriodicFunctionsEnabled;
   short YieldFunctionEnabled;
   long EphemeralItemCount;
   long EphemeralItemSize;
   long CurrentEphemeralCountMax;
   long CurrentEphemeralSizeMax;
   void (*YieldTimeFunction)(void);
   int LastEvaluationDepth ;
   struct trackedMemory *trackList;
  };

#define UtilityData(theEnv) ((struct utilityData *) GetEnvironmentData(theEnv,UTILITY_DATA))

  /* Is c the start of a utf8 sequence? */
#define IsUTF8Start(ch) (((ch) & 0xC0) != 0x80)
#define IsUTF8MultiByteStart(ch) ((((unsigned char) ch) >= 0xC0) && (((unsigned char) ch) <= 0xF7))
#define IsUTF8MultiByteContinuation(ch) ((((unsigned char) ch) >= 0x80) && (((unsigned char) ch) <= 0xBF))

#ifdef _UTILITY_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define DecrementGCLocks() EnvDecrementGCLocks(GetCurrentEnvironment())
#define IncrementGCLocks() EnvIncrementGCLocks(GetCurrentEnvironment())
#define RemovePeriodicFunction(a) EnvRemovePeriodicFunction(GetCurrentEnvironment(),a)

   LOCALE void                           InitializeUtilityData(void *);
   LOCALE void                           PeriodicCleanup(void *,intBool,intBool);
   LOCALE intBool                        AddCleanupFunction(void *,char *,void (*)(void *),int);
   LOCALE intBool                        EnvAddPeriodicFunction(void *,char *,void (*)(void *),int);
   LOCALE intBool                        AddPeriodicFunction(char *,void (*)(void),int);
   LOCALE intBool                        RemoveCleanupFunction(void *,char *);
   LOCALE intBool                        EnvRemovePeriodicFunction(void *,char *);
   LOCALE char                          *AppendStrings(void *,char *,char *);
   LOCALE char                          *StringPrintForm(void *,char *);
   LOCALE char                          *AppendToString(void *,char *,char *,size_t *,size_t *);
   LOCALE char                          *AppendNToString(void *,char *,char *,size_t,size_t *,size_t *);
   LOCALE char                          *ExpandStringWithChar(void *,int,char *,size_t *,size_t *,size_t);
   LOCALE struct callFunctionItem       *AddFunctionToCallList(void *,char *,int,void (*)(void *),
                                                               struct callFunctionItem *,intBool);
   LOCALE struct callFunctionItem       *AddFunctionToCallListWithContext(void *,char *,int,void (*)(void *),
                                                                          struct callFunctionItem *,intBool,void *);
   LOCALE struct callFunctionItem       *RemoveFunctionFromCallList(void *,char *,
                                                             struct callFunctionItem *,
                                                             int *);
   LOCALE void                           DeallocateCallList(void *,struct callFunctionItem *);
   LOCALE unsigned long                  ItemHashValue(void *,unsigned short,void *,unsigned long);
   LOCALE void                           YieldTime(void *);
   LOCALE short                          SetGarbageCollectionHeuristics(void *,short);
   LOCALE void                           EnvIncrementGCLocks(void *);
   LOCALE void                           EnvDecrementGCLocks(void *);
   LOCALE short                          EnablePeriodicFunctions(void *,short);
   LOCALE short                          EnableYieldFunction(void *,short);
   LOCALE struct trackedMemory          *AddTrackedMemory(void *,void *,size_t);
   LOCALE void                           RemoveTrackedMemory(void *,struct trackedMemory *);
   LOCALE void                           UTF8Increment(char *,size_t *);
   LOCALE size_t                         UTF8Offset(char *,size_t);
   LOCALE size_t                         UTF8Length(char *);
   LOCALE size_t                         UTF8CharNum(char *,size_t);
   
#endif




