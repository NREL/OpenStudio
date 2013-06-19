   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
   /*                                                     */
   /*                 BLOAD HEADER FILE                   */
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

#ifndef _H_bload
#define _H_bload

#ifndef _H_utility
#include "utility.h"
#endif
#ifndef _H_extnfunc
#include "extnfunc.h"
#endif
#ifndef _H_exprnbin
#include "exprnbin.h"
#endif
#ifndef _H_symbol
#include "symbol.h"
#endif
#ifndef _H_sysdep
#include "sysdep.h"
#endif
#ifndef _H_symblbin
#include "symblbin.h"
#endif

#define BLOAD_DATA 38

struct bloadData
  { 
   char *BinaryPrefixID;
   char *BinaryVersionID;
   struct FunctionDefinition **FunctionArray;
   int BloadActive;
   struct callFunctionItem *BeforeBloadFunctions;
   struct callFunctionItem *AfterBloadFunctions;
   struct callFunctionItem *ClearBloadReadyFunctions;
   struct callFunctionItem *AbortBloadFunctions;
  };

#define BloadData(theEnv) ((struct bloadData *) GetEnvironmentData(theEnv,BLOAD_DATA))

#ifdef LOCALE
#undef LOCALE
#endif
#ifdef _BLOAD_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define FunctionPointer(i) ((struct FunctionDefinition *) (((i) == -1L) ? NULL : BloadData(theEnv)->FunctionArray[i]))

#define Bload(theEnv,a) EnvBload(theEnv,a)

   LOCALE void                    InitializeBloadData(void *);
   LOCALE int                     BloadCommand(void *);
   LOCALE intBool                 EnvBload(void *,char *);
   LOCALE void                    BloadandRefresh(void *,long,size_t,void (*)(void *,void *,long));
   LOCALE intBool                 Bloaded(void *);
   LOCALE void                    AddBeforeBloadFunction(void *,char *,void (*)(void *),int);
   LOCALE void                    AddAfterBloadFunction(void *,char *,void (*)(void *),int);
   LOCALE void                    AddBloadReadyFunction(void *,char *,int (*)(void),int);
   LOCALE void                    AddClearBloadReadyFunction(void *,char *,int (*)(void *),int);
   LOCALE void                    AddAbortBloadFunction(void *,char *,void (*)(void *),int);
   LOCALE void                    CannotLoadWithBloadMessage(void *,char *);

#endif

