   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                ENVRNMNT HEADER FILE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Routines for supporting multiple environments.   */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Added CreateRuntimeEnvironment function.       */
/*                                                           */
/*            Added support for context information when an  */
/*            environment is created (i.e a pointer from the */
/*            CLIPS environment to its parent environment).  */
/*                                                           */
/*      6.30: Added support for passing context information  */ 
/*            to user defined functions and callback         */
/*            functions.                                     */
/*                                                           */
/*************************************************************/

#ifndef _H_envrnmnt
#define _H_envrnmnt

#ifndef _H_symbol
#include "symbol.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _ENVRNMNT_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define USER_ENVIRONMENT_DATA 70
#define MAXIMUM_ENVIRONMENT_POSITIONS 100

struct environmentCleanupFunction
  {
   char *name;
   void (*func)(void *);
   int priority;
   struct environmentCleanupFunction *next;
  };

struct environmentData
  {   
   unsigned int initialized : 1;
   unsigned long environmentIndex;
   void *context;
   void *routerContext;
   void *functionContext;
   void *callbackContext;
   void **theData;
   void (**cleanupFunctions)(void *);
   struct environmentCleanupFunction *listOfCleanupEnvironmentFunctions;
   struct environmentData *next;
  };

typedef struct environmentData ENVIRONMENT_DATA;
typedef struct environmentData * ENVIRONMENT_DATA_PTR;

#define GetEnvironmentData(theEnv,position) (((struct environmentData *) theEnv)->theData[position])
#define SetEnvironmentData(theEnv,position,value) (((struct environmentData *) theEnv)->theData[position] = value)

   LOCALE intBool                        AllocateEnvironmentData(void *,unsigned int,unsigned long,void (*)(void *));
   LOCALE intBool                        DeallocateEnvironmentData(void);
#if ALLOW_ENVIRONMENT_GLOBALS
   LOCALE void                           SetCurrentEnvironment(void *);
   LOCALE intBool                        SetCurrentEnvironmentByIndex(unsigned long);
   LOCALE void                          *GetEnvironmentByIndex(unsigned long);
   LOCALE void                          *GetCurrentEnvironment(void);
   LOCALE unsigned long                  GetEnvironmentIndex(void *);
#endif
   LOCALE void                          *CreateEnvironment(void);
   LOCALE void                          *CreateRuntimeEnvironment(struct symbolHashNode **,struct floatHashNode **,
                                                                  struct integerHashNode **,struct bitMapHashNode **);
   LOCALE intBool                        DestroyEnvironment(void *);
   LOCALE intBool                        AddEnvironmentCleanupFunction(void *,char *,void (*)(void *),int);
   LOCALE void                          *GetEnvironmentContext(void *);
   LOCALE void                          *SetEnvironmentContext(void *,void *);
   LOCALE void                          *GetEnvironmentRouterContext(void *);
   LOCALE void                          *SetEnvironmentRouterContext(void *,void *);
   LOCALE void                          *GetEnvironmentFunctionContext(void *);
   LOCALE void                          *SetEnvironmentFunctionContext(void *,void *);
   LOCALE void                          *GetEnvironmentCallbackContext(void *);
   LOCALE void                          *SetEnvironmentCallbackContext(void *,void *);

#endif

