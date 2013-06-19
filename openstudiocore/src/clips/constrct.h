   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*                  CONSTRUCT MODULE                   */
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
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_constrct

#define _H_constrct

struct constructHeader;
struct construct;

#ifndef _H_moduldef
#include "moduldef.h"
#endif
#ifndef _H_symbol
#include "symbol.h"
#endif

#include "userdata.h"

struct constructHeader
  {
   struct symbolHashNode *name;
   char *ppForm;
   struct defmoduleItemHeader *whichModule;
   long bsaveID;
   struct constructHeader *next;
   struct userData *usrData;
  };

#define CHS (struct constructHeader *)

struct construct
  {
   char *constructName;
   char *pluralName;
   int (*parseFunction)(void *,char *);
   void *(*findFunction)(void *,char *);
   struct symbolHashNode *(*getConstructNameFunction)(struct constructHeader *);
   char *(*getPPFormFunction)(void *,struct constructHeader *);
   struct defmoduleItemHeader *(*getModuleItemFunction)(struct constructHeader *);
   void *(*getNextItemFunction)(void *,void *);
   void (*setNextItemFunction)(struct constructHeader *,struct constructHeader *);
   intBool (*isConstructDeletableFunction)(void *,void *);
   int (*deleteFunction)(void *,void *);
   void (*freeFunction)(void *,void *);
   struct construct *next;
  };

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_scanner
#include "scanner.h"
#endif

#define CONSTRUCT_DATA 42

struct constructData
  { 
   int ClearReadyInProgress;
   int ClearInProgress;
   int ResetReadyInProgress;
   int ResetInProgress;
#if (! RUN_TIME) && (! BLOAD_ONLY)
   struct callFunctionItem   *ListOfSaveFunctions;
   intBool PrintWhileLoading;
   unsigned WatchCompilations;
#endif
   struct construct *ListOfConstructs;
   struct callFunctionItem *ListOfResetFunctions;
   struct callFunctionItem *ListOfClearFunctions;
   struct callFunctionItem *ListOfClearReadyFunctions;
   int Executing;
   int (*BeforeResetFunction)(void *);
   int CheckSyntaxMode;
   int ParsingConstruct;
  };

#define ConstructData(theEnv) ((struct constructData *) GetEnvironmentData(theEnv,CONSTRUCT_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _CONSTRCT_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define RemoveClearFunction(a) EnvRemoveClearFunction(GetCurrentEnvironment(),a)
#define RemoveResetFunction(a) EnvRemoveResetFunction(GetCurrentEnvironment(),a)

#if ALLOW_ENVIRONMENT_GLOBALS
   LOCALE void                           Clear(void);
   LOCALE void                           Reset(void);
   LOCALE int                            Save(char *);
#endif

   LOCALE void                           EnvClear(void *);
   LOCALE void                           EnvReset(void *);
   LOCALE int                            EnvSave(void *,char *);

   LOCALE void                           InitializeConstructData(void *);
   LOCALE intBool                        AddSaveFunction(void *,char *,void (*)(void *,void *,char *),int);
   LOCALE intBool                        RemoveSaveFunction(void *,char *);
   LOCALE intBool                        EnvAddResetFunction(void *,char *,void (*)(void *),int);
   LOCALE intBool                        AddResetFunction(char *,void (*)(void),int);
   LOCALE intBool                        EnvRemoveResetFunction(void *,char *);
   LOCALE intBool                        AddClearReadyFunction(void *,char *,int (*)(void *),int);
   LOCALE intBool                        RemoveClearReadyFunction(void *,char *);
   LOCALE intBool                        EnvAddClearFunction(void *,char *,void (*)(void *),int);
   LOCALE intBool                        AddClearFunction(char *,void (*)(void),int);
   LOCALE intBool                        EnvRemoveClearFunction(void *,char *);
   LOCALE struct construct              *AddConstruct(void *,char *,char *,
                                                      int (*)(void *,char *),
                                                      void *(*)(void *,char *),
                                                      SYMBOL_HN *(*)(struct constructHeader *),
                                                      char *(*)(void *,struct constructHeader *),
                                                      struct defmoduleItemHeader *(*)(struct constructHeader *),
                                                      void *(*)(void *,void *),
                                                      void (*)(struct constructHeader *,struct constructHeader *),
                                                      intBool (*)(void *,void *),
                                                      int (*)(void *,void *),
                                                      void (*)(void *,void *));
   LOCALE int                            RemoveConstruct(void *,char *);
   LOCALE void                           SetCompilationsWatch(void *,unsigned);
   LOCALE unsigned                       GetCompilationsWatch(void *);
   LOCALE void                           SetPrintWhileLoading(void *,intBool);
   LOCALE intBool                        GetPrintWhileLoading(void *);
   LOCALE int                            ExecutingConstruct(void *);
   LOCALE void                           SetExecutingConstruct(void *,int);
   LOCALE void                           InitializeConstructs(void *);
   LOCALE int                          (*SetBeforeResetFunction(void *,int (*)(void *)))(void *);
   LOCALE void                           OldGetConstructList(void *,DATA_OBJECT *,
                                                          void *(*)(void *,void *),
                                                          char *(*)(void *,void *));
   LOCALE void                           ResetCommand(void *);
   LOCALE void                           ClearCommand(void *);
   LOCALE intBool                        ClearReady(void *);
   LOCALE struct construct              *FindConstruct(void *,char *);
   LOCALE void                           DeinstallConstructHeader(void *,struct constructHeader *);
   LOCALE void                           DestroyConstructHeader(void *,struct constructHeader *);

#endif







