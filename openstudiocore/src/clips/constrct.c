   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*                  CONSTRUCT MODULE                   */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides basic functionality for creating new    */
/*   types of constructs, saving constructs to a file, and   */
/*   adding new functionality to the clear and reset         */
/*   commands.                                               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Added environment parameter to GenClose.       */
/*            Added environment parameter to GenOpen.        */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#define _CONSTRCT_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <string.h>

#include "setup.h"

#include "constant.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "router.h"
#include "scanner.h"
#include "watch.h"
#include "prcdrfun.h"
#include "prcdrpsr.h"
#include "argacces.h"
#include "exprnpsr.h"
#include "multifld.h"
#include "moduldef.h"
#include "sysdep.h"
#include "utility.h"
#include "commline.h"

#include "ruledef.h" /* TBD Remove */
#include "constrct.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                        DeallocateConstructData(void *);

/**************************************************/
/* InitializeConstructData: Allocates environment */
/*    data for constructs.                        */
/**************************************************/
globle void InitializeConstructData(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,CONSTRUCT_DATA,sizeof(struct constructData),DeallocateConstructData);

#if (! RUN_TIME) && (! BLOAD_ONLY)   
   ConstructData(theEnv)->WatchCompilations = ON;
#endif
  }
  
/****************************************************/
/* DeallocateConstructData: Deallocates environment */
/*    data for constructs.                          */
/****************************************************/
static void DeallocateConstructData(
  void *theEnv)
  {
   struct construct *tmpPtr, *nextPtr;

#if (! RUN_TIME) && (! BLOAD_ONLY)
   DeallocateCallList(theEnv,ConstructData(theEnv)->ListOfSaveFunctions);
#endif
   DeallocateCallList(theEnv,ConstructData(theEnv)->ListOfResetFunctions);
   DeallocateCallList(theEnv,ConstructData(theEnv)->ListOfClearFunctions);
   DeallocateCallList(theEnv,ConstructData(theEnv)->ListOfClearReadyFunctions);
   
   tmpPtr = ConstructData(theEnv)->ListOfConstructs;
   while (tmpPtr != NULL)
     {
      nextPtr = tmpPtr->next;
      rtn_struct(theEnv,construct,tmpPtr);
      tmpPtr = nextPtr;
     }
  }

#if (! RUN_TIME) && (! BLOAD_ONLY)

/*************************************************/
/* FindConstruct: Determines whether a construct */
/*   type is in the ListOfConstructs.            */
/*************************************************/
globle struct construct *FindConstruct(
  void *theEnv,
  char *name)
  {
   struct construct *currentPtr;

   for (currentPtr = ConstructData(theEnv)->ListOfConstructs;
        currentPtr != NULL;
        currentPtr = currentPtr->next)
     {
      if (strcmp(name,currentPtr->constructName) == 0)
        { return(currentPtr); }
     }

   return(NULL);
  }

/***********************************************************/
/* RemoveConstruct: Removes a construct and its associated */
/*   parsing function from the ListOfConstructs. Returns   */
/*   TRUE if the construct type was removed, otherwise     */
/*   FALSE.                                                */
/***********************************************************/
globle int RemoveConstruct(
  void *theEnv,
  char *name)
  {
   struct construct *currentPtr, *lastPtr = NULL;

   for (currentPtr = ConstructData(theEnv)->ListOfConstructs;
        currentPtr != NULL;
        currentPtr = currentPtr->next)
     {
      if (strcmp(name,currentPtr->constructName) == 0)
        {
         if (lastPtr == NULL)
           { ConstructData(theEnv)->ListOfConstructs = currentPtr->next; }
         else
           { lastPtr->next = currentPtr->next; }
         rtn_struct(theEnv,construct,currentPtr);
         return(TRUE);
        }

      lastPtr = currentPtr;
     }

   return(FALSE);
  }

/***************************/
/* Save: C access routine  */
/*   for the save command. */
/***************************/
#if ALLOW_ENVIRONMENT_GLOBALS
globle int Save(
  char *fileName)  
  {
   return EnvSave(GetCurrentEnvironment(),fileName);
  }  
#endif

/************************************************/
/* Save: C access routine for the save command. */
/************************************************/
globle int EnvSave(
  void *theEnv,
  char *fileName)
  {
   struct callFunctionItem *saveFunction;
   FILE *filePtr;
   void *defmodulePtr;

   /*=====================*/
   /* Open the save file. */
   /*=====================*/

   if ((filePtr = GenOpen(theEnv,fileName,"w")) == NULL)
     { return(FALSE); }

   /*===========================*/
   /* Bypass the router system. */
   /*===========================*/

   SetFastSave(theEnv,filePtr);

   /*======================*/
   /* Save the constructs. */
   /*======================*/
   
   for (defmodulePtr = EnvGetNextDefmodule(theEnv,NULL);
        defmodulePtr != NULL;
        defmodulePtr = EnvGetNextDefmodule(theEnv,defmodulePtr))
     {
      for (saveFunction = ConstructData(theEnv)->ListOfSaveFunctions;
           saveFunction != NULL;
           saveFunction = saveFunction->next)
        {
         ((* (void (*)(void *,void *,char *)) saveFunction->func))(theEnv,defmodulePtr,(char *) filePtr);
        }
     }

   /*======================*/
   /* Close the save file. */
   /*======================*/

   GenClose(theEnv,filePtr);

   /*===========================*/
   /* Remove the router bypass. */
   /*===========================*/

   SetFastSave(theEnv,NULL);

   /*=========================*/
   /* Return TRUE to indicate */
   /* successful completion.  */
   /*=========================*/

   return(TRUE);
  }

/*******************************************************/
/* RemoveSaveFunction: Removes a function from the     */
/*   ListOfSaveFunctions. Returns TRUE if the function */
/*   was successfully removed, otherwise FALSE.        */
/*******************************************************/
globle intBool RemoveSaveFunction(
  void *theEnv,
  char *name)
  {
   int found;

   ConstructData(theEnv)->ListOfSaveFunctions =
     RemoveFunctionFromCallList(theEnv,name,ConstructData(theEnv)->ListOfSaveFunctions,&found);

   if (found) return(TRUE);

   return(FALSE);
  }

/**********************************/
/* SetCompilationsWatch: Sets the */
/*   value of WatchCompilations.  */
/**********************************/
globle void SetCompilationsWatch(
  void *theEnv,
  unsigned value)
  {
   ConstructData(theEnv)->WatchCompilations = value;
  }

/*************************************/
/* GetCompilationsWatch: Returns the */
/*   value of WatchCompilations.     */
/*************************************/
globle unsigned GetCompilationsWatch(
  void *theEnv)
  {   
   return(ConstructData(theEnv)->WatchCompilations); 
  }

/**********************************/
/* SetPrintWhileLoading: Sets the */
/*   value of PrintWhileLoading.  */
/**********************************/
globle void SetPrintWhileLoading(
  void *theEnv,
  intBool value)
  {
   ConstructData(theEnv)->PrintWhileLoading = value;
  }

/*************************************/
/* GetPrintWhileLoading: Returns the */
/*   value of PrintWhileLoading.     */
/*************************************/
globle intBool GetPrintWhileLoading(
  void *theEnv)
  {
   return(ConstructData(theEnv)->PrintWhileLoading);
  }
#endif

/*************************************/
/* InitializeConstructs: Initializes */
/*   the Construct Manager.          */
/*************************************/
globle void InitializeConstructs(
  void *theEnv)
  {
#if (! RUN_TIME)
   EnvDefineFunction2(theEnv,"clear",   'v', PTIEF ClearCommand,   "ClearCommand", "00");
   EnvDefineFunction2(theEnv,"reset",   'v', PTIEF ResetCommand,   "ResetCommand", "00");

#if DEBUGGING_FUNCTIONS && (! BLOAD_ONLY)
   AddWatchItem(theEnv,"compilations",0,&ConstructData(theEnv)->WatchCompilations,30,NULL,NULL);
#endif
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
#endif
  }

/**************************************/
/* ClearCommand: H/L access routine   */
/*   for the clear command.           */
/**************************************/
globle void ClearCommand(
  void *theEnv)
  {
   if (EnvArgCountCheck(theEnv,"clear",EXACTLY,0) == -1) return;
   EnvClear(theEnv);
   return;
  }

/**************************************/
/* ResetCommand: H/L access routine   */
/*   for the reset command.           */
/**************************************/
globle void ResetCommand(
  void *theEnv)
  {
   if (EnvArgCountCheck(theEnv,"reset",EXACTLY,0) == -1) return;
   EnvReset(theEnv);
   return;
  }

/****************************/
/* Reset: C access routine  */
/*   for the reset command. */
/****************************/
#if ALLOW_ENVIRONMENT_GLOBALS
globle void Reset()
  {
   EnvReset(GetCurrentEnvironment());
  }  
#endif

/******************************/
/* EnvReset: C access routine */
/*   for the reset command.   */
/******************************/
globle void EnvReset(
  void *theEnv)
  {
   struct callFunctionItem *resetPtr;

   /*=====================================*/
   /* The reset command can't be executed */
   /* while a reset is in progress.       */
   /*=====================================*/

   if (ConstructData(theEnv)->ResetInProgress) return;

   ConstructData(theEnv)->ResetInProgress = TRUE;
   ConstructData(theEnv)->ResetReadyInProgress = TRUE;

   /*================================================*/
   /* If the reset is performed from the top level   */
   /* command prompt, reset the halt execution flag. */
   /*================================================*/

   if (EvaluationData(theEnv)->CurrentEvaluationDepth == 0) SetHaltExecution(theEnv,FALSE);

   /*=======================================================*/
   /* Call the before reset function to determine if the    */
   /* reset should continue. [Used by the some of the       */
   /* windowed interfaces to query the user whether a       */
   /* reset should proceed with activations on the agenda.] */
   /*=======================================================*/

   if ((ConstructData(theEnv)->BeforeResetFunction != NULL) ? 
       ((*ConstructData(theEnv)->BeforeResetFunction)(theEnv) == FALSE) :
                                       FALSE)
     {
      ConstructData(theEnv)->ResetReadyInProgress = FALSE;
      ConstructData(theEnv)->ResetInProgress = FALSE;
      return;
     }
   ConstructData(theEnv)->ResetReadyInProgress = FALSE;

   /*===========================*/
   /* Call each reset function. */
   /*===========================*/

   for (resetPtr = ConstructData(theEnv)->ListOfResetFunctions;
        (resetPtr != NULL) && (GetHaltExecution(theEnv) == FALSE);
        resetPtr = resetPtr->next)
     { 
      if (resetPtr->environmentAware)
        { (*resetPtr->func)(theEnv); }
      else            
        { (* (void (*)(void)) resetPtr->func)(); }
     }

   /*============================================*/
   /* Set the current module to the MAIN module. */
   /*============================================*/

   EnvSetCurrentModule(theEnv,(void *) EnvFindDefmodule(theEnv,"MAIN"));

   /*===========================================*/
   /* Perform periodic cleanup if the reset was */
   /* issued from an embedded controller.       */
   /*===========================================*/

   if ((EvaluationData(theEnv)->CurrentEvaluationDepth == 0) && (! CommandLineData(theEnv)->EvaluatingTopLevelCommand) &&
       (EvaluationData(theEnv)->CurrentExpression == NULL))
     { PeriodicCleanup(theEnv,TRUE,FALSE); }

   /*===================================*/
   /* A reset is no longer in progress. */
   /*===================================*/

   ConstructData(theEnv)->ResetInProgress = FALSE;
  }

/************************************/
/* SetBeforeResetFunction: Sets the */
/*  value of BeforeResetFunction.   */
/************************************/
globle int (*SetBeforeResetFunction(void *theEnv,
                                    int (*theFunction)(void *)))(void *)
  {
   int (*tempFunction)(void *);

   tempFunction = ConstructData(theEnv)->BeforeResetFunction;
   ConstructData(theEnv)->BeforeResetFunction = theFunction;
   return(tempFunction);
  }

#if ALLOW_ENVIRONMENT_GLOBALS
/*************************************/
/* AddResetFunction: Adds a function */
/*   to ListOfResetFunctions.        */
/*************************************/
globle intBool AddResetFunction(
  char *name,
  void (*functionPtr)(void),
  int priority)
  {
   void *theEnv;
   
   theEnv = GetCurrentEnvironment();
   
   ConstructData(theEnv)->ListOfResetFunctions = 
      AddFunctionToCallList(theEnv,name,priority,(void (*)(void *)) functionPtr,
                            ConstructData(theEnv)->ListOfResetFunctions,FALSE);
   return(TRUE);
  }
#endif

/****************************************/
/* EnvAddResetFunction: Adds a function */
/*   to ListOfResetFunctions.           */
/****************************************/
globle intBool EnvAddResetFunction(
  void *theEnv,
  char *name,
  void (*functionPtr)(void *),
  int priority)
  {
   ConstructData(theEnv)->ListOfResetFunctions = AddFunctionToCallList(theEnv,name,priority,
                                                functionPtr,
                                                ConstructData(theEnv)->ListOfResetFunctions,TRUE);
   return(TRUE);
  }

/**********************************************/
/* EnvRemoveResetFunction: Removes a function */
/*   from the ListOfResetFunctions.           */
/**********************************************/
globle intBool EnvRemoveResetFunction(
  void *theEnv,
  char *name)
  {
   int found;

   ConstructData(theEnv)->ListOfResetFunctions =
      RemoveFunctionFromCallList(theEnv,name,ConstructData(theEnv)->ListOfResetFunctions,&found);

   if (found) return(TRUE);

   return(FALSE);
  }

/****************************/
/* Clear: C access routine  */
/*   for the clear command. */
/****************************/
#if ALLOW_ENVIRONMENT_GLOBALS
globle void Clear()
  {
   EnvClear(GetCurrentEnvironment());
  }  
#endif
    
/*****************************************************/
/* EnvClear: C access routine for the clear command. */
/*****************************************************/
globle void EnvClear(
  void *theEnv)
  {
   struct callFunctionItem *theFunction;
   
   /*==========================================*/
   /* Activate the watch router which captures */
   /* trace output so that it is not displayed */
   /* during a clear.                          */
   /*==========================================*/

#if DEBUGGING_FUNCTIONS
   EnvActivateRouter(theEnv,WTRACE);
#endif

   /*===================================*/
   /* Determine if a clear is possible. */
   /*===================================*/

   ConstructData(theEnv)->ClearReadyInProgress = TRUE;
   if (ClearReady(theEnv) == FALSE)
     {
      PrintErrorID(theEnv,"CONSTRCT",1,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Some constructs are still in use. Clear cannot continue.\n");
#if DEBUGGING_FUNCTIONS
      EnvDeactivateRouter(theEnv,WTRACE);
#endif
      ConstructData(theEnv)->ClearReadyInProgress = FALSE;
      return;
     }
   ConstructData(theEnv)->ClearReadyInProgress = FALSE;

   /*===========================*/
   /* Call all clear functions. */
   /*===========================*/

   ConstructData(theEnv)->ClearInProgress = TRUE;

   for (theFunction = ConstructData(theEnv)->ListOfClearFunctions;
        theFunction != NULL;
        theFunction = theFunction->next)
     { 
      if (theFunction->environmentAware)
        { (*theFunction->func)(theEnv); }
      else            
        { (* (void (*)(void)) theFunction->func)(); }
     }

   /*=============================*/
   /* Deactivate the watch router */
   /* for capturing output.       */
   /*=============================*/

#if DEBUGGING_FUNCTIONS
   EnvDeactivateRouter(theEnv,WTRACE);
#endif

   /*===========================================*/
   /* Perform periodic cleanup if the clear was */
   /* issued from an embedded controller.       */
   /*===========================================*/

   if ((EvaluationData(theEnv)->CurrentEvaluationDepth == 0) && (! CommandLineData(theEnv)->EvaluatingTopLevelCommand) &&
       (EvaluationData(theEnv)->CurrentExpression == NULL))
     { PeriodicCleanup(theEnv,TRUE,FALSE); }

   /*===========================*/
   /* Clear has been completed. */
   /*===========================*/

   ConstructData(theEnv)->ClearInProgress = FALSE;
   
   if ((DefruleData(theEnv)->RightPrimeJoins != NULL) ||
       (DefruleData(theEnv)->LeftPrimeJoins != NULL))
     { SystemError(theEnv,"CONSTRCT",1); }
       
   /*============================*/
   /* Perform reset after clear. */
   /*============================*/
   
   EnvReset(theEnv);
  }

/*********************************************************/
/* ClearReady: Returns TRUE if a clear can be performed, */
/*   otherwise FALSE. Note that this is destructively    */
/*   determined (e.g. facts will be deleted as part of   */
/*   the determination).                                 */
/*********************************************************/
globle intBool ClearReady(
  void *theEnv)
  {
   struct callFunctionItem *theFunction;
   int (*tempFunction)(void *);

   for (theFunction = ConstructData(theEnv)->ListOfClearReadyFunctions;
        theFunction != NULL;
        theFunction = theFunction->next)
     {
      tempFunction = (int (*)(void *)) theFunction->func;
      if ((*tempFunction)(theEnv) == FALSE)
        { return(FALSE); }
     }

   return(TRUE);
  }

/******************************************/
/* AddClearReadyFunction: Adds a function */
/*   to ListOfClearReadyFunctions.        */
/******************************************/
globle intBool AddClearReadyFunction(
  void *theEnv,
  char *name,
  int (*functionPtr)(void *),
  int priority)
  {
   ConstructData(theEnv)->ListOfClearReadyFunctions =
     AddFunctionToCallList(theEnv,name,priority,
                           (void (*)(void *)) functionPtr,
                           ConstructData(theEnv)->ListOfClearReadyFunctions,TRUE);
   return(1);
  }

/************************************************/
/* RemoveClearReadyFunction: Removes a function */
/*   from the ListOfClearReadyFunctions.        */
/************************************************/
globle intBool RemoveClearReadyFunction(
  void *theEnv,
  char *name)
  {
   int found;

   ConstructData(theEnv)->ListOfClearReadyFunctions =
      RemoveFunctionFromCallList(theEnv,name,ConstructData(theEnv)->ListOfClearReadyFunctions,&found);

   if (found) return(TRUE);

   return(FALSE);
  }

#if ALLOW_ENVIRONMENT_GLOBALS
/*************************************/
/* AddClearFunction: Adds a function */
/*   to ListOfClearFunctions.        */
/*************************************/
globle intBool AddClearFunction(
  char *name,
  void (*functionPtr)(void),
  int priority)
  {
   void *theEnv;
   
   theEnv = GetCurrentEnvironment();
   
   ConstructData(theEnv)->ListOfClearFunctions =
      AddFunctionToCallList(theEnv,name,priority,
                            (void (*)(void *)) functionPtr,
                            ConstructData(theEnv)->ListOfClearFunctions,FALSE);
   return(1);
  }
#endif
    
/****************************************/
/* EnvAddClearFunction: Adds a function */
/*   to ListOfClearFunctions.           */
/****************************************/
globle intBool EnvAddClearFunction(
  void *theEnv,
  char *name,
  void (*functionPtr)(void *),
  int priority)
  {
   ConstructData(theEnv)->ListOfClearFunctions =
      AddFunctionToCallList(theEnv,name,priority,
                            (void (*)(void *)) functionPtr,
                            ConstructData(theEnv)->ListOfClearFunctions,TRUE);
   return(1);
  }

/**********************************************/
/* EnvRemoveClearFunction: Removes a function */
/*    from the ListOfClearFunctions.          */
/**********************************************/
globle intBool EnvRemoveClearFunction(
  void *theEnv,
  char *name)
  {
   int found;

   ConstructData(theEnv)->ListOfClearFunctions =
     RemoveFunctionFromCallList(theEnv,name,ConstructData(theEnv)->ListOfClearFunctions,&found);

   if (found) return(TRUE);

   return(FALSE);
  }

/***********************************************/
/* ExecutingConstruct: Returns TRUE if a */
/*   construct is currently being executed,    */
/*   otherwise FALSE.                    */
/***********************************************/
globle int ExecutingConstruct(
  void *theEnv)
  {
   return(ConstructData(theEnv)->Executing); 
  }

/********************************************/
/* SetExecutingConstruct: Sets the value of */
/*   the executing variable indicating that */
/*   actions such as reset, clear, etc      */
/*   should not be performed.               */
/********************************************/
globle void SetExecutingConstruct(
  void *theEnv,
  int value)
  {
   ConstructData(theEnv)->Executing = value;
  }

/************************************************************/
/* OldGetConstructList: Returns a list of all the construct */
/*   names in a multifield value. It doesn't check the      */
/*   number of arguments. It assumes that the restriction   */
/*   string in DefineFunction2 call was "00".               */
/************************************************************/
globle void OldGetConstructList(
  void *theEnv,
  DATA_OBJECT_PTR returnValue,
  void *(*nextFunction)(void *,void *),
  char *(*nameFunction)(void *,void *))
  {
   void *theConstruct;
   unsigned long count = 0;
   struct multifield *theList;

   /*====================================*/
   /* Determine the number of constructs */
   /* of the specified type.             */
   /*====================================*/

   for (theConstruct = (*nextFunction)(theEnv,NULL);
        theConstruct != NULL;
        theConstruct = (*nextFunction)(theEnv,theConstruct))
     { count++; }

   /*===========================*/
   /* Create a multifield large */
   /* enough to store the list. */
   /*===========================*/

   SetpType(returnValue,MULTIFIELD);
   SetpDOBegin(returnValue,1);
   SetpDOEnd(returnValue,(long) count);
   theList = (struct multifield *) EnvCreateMultifield(theEnv,count);
   SetpValue(returnValue,(void *) theList);

   /*====================================*/
   /* Store the names in the multifield. */
   /*====================================*/

   for (theConstruct = (*nextFunction)(theEnv,NULL), count = 1;
        theConstruct != NULL;
        theConstruct = (*nextFunction)(theEnv,theConstruct), count++)
     {
      if (EvaluationData(theEnv)->HaltExecution == TRUE)
        {
         EnvSetMultifieldErrorValue(theEnv,returnValue);
         return;
        }
      SetMFType(theList,count,SYMBOL);
      SetMFValue(theList,count,EnvAddSymbol(theEnv,(*nameFunction)(theEnv,theConstruct)));
     }
  }

/*******************************************************/
/* DeinstallConstructHeader: Decrements the busy count */
/*   of a construct name and frees its pretty print    */
/*   representation string (both of which are stored   */
/*   in the generic construct header).                 */
/*******************************************************/
globle void DeinstallConstructHeader(
  void *theEnv,
  struct constructHeader *theHeader)
  {
   DecrementSymbolCount(theEnv,theHeader->name);
   if (theHeader->ppForm != NULL)
     {
      rm(theEnv,theHeader->ppForm,
         sizeof(char) * (strlen(theHeader->ppForm) + 1));
      theHeader->ppForm = NULL;
     }

   if (theHeader->usrData != NULL)
     {
      ClearUserDataList(theEnv,theHeader->usrData);
      theHeader->usrData = NULL;
     }
  }

/**************************************************/
/* DestroyConstructHeader: Frees the pretty print */
/*   representation string and user data (both of */
/*   which are stored in the generic construct    */
/*   header).                                     */
/**************************************************/
globle void DestroyConstructHeader(
  void *theEnv,
  struct constructHeader *theHeader)
  {
   if (theHeader->ppForm != NULL)
     {
      rm(theEnv,theHeader->ppForm,
         sizeof(char) * (strlen(theHeader->ppForm) + 1));
      theHeader->ppForm = NULL;
     }

   if (theHeader->usrData != NULL)
     {
      ClearUserDataList(theEnv,theHeader->usrData);
      theHeader->usrData = NULL;
     }
  }

/*****************************************************/
/* AddConstruct: Adds a construct and its associated */
/*   parsing function to the ListOfConstructs.       */
/*****************************************************/
globle struct construct *AddConstruct(
  void *theEnv,
  char *name,
  char *pluralName,
  int (*parseFunction)(void *,char *),
  void *(*findFunction)(void *,char *),
  SYMBOL_HN *(*getConstructNameFunction)(struct constructHeader *),
  char *(*getPPFormFunction)(void *,struct constructHeader *),
  struct defmoduleItemHeader *(*getModuleItemFunction)(struct constructHeader *),
  void *(*getNextItemFunction)(void *,void *),
  void (*setNextItemFunction)(struct constructHeader *,struct constructHeader *),
  intBool (*isConstructDeletableFunction)(void *,void *),
  int (*deleteFunction)(void *,void *),
  void (*freeFunction)(void *,void *))
  {
   struct construct *newPtr;

   /*=============================*/
   /* Allocate and initialize the */
   /* construct data structure.   */
   /*=============================*/

   newPtr = get_struct(theEnv,construct);

   newPtr->constructName = name;
   newPtr->pluralName = pluralName;
   newPtr->parseFunction = parseFunction;
   newPtr->findFunction = findFunction;
   newPtr->getConstructNameFunction = getConstructNameFunction;
   newPtr->getPPFormFunction = getPPFormFunction;
   newPtr->getModuleItemFunction = getModuleItemFunction;
   newPtr->getNextItemFunction = getNextItemFunction;
   newPtr->setNextItemFunction = setNextItemFunction;
   newPtr->isConstructDeletableFunction = isConstructDeletableFunction;
   newPtr->deleteFunction = deleteFunction;
   newPtr->freeFunction = freeFunction;

   /*===============================*/
   /* Add the construct to the list */
   /* of constructs and return it.  */
   /*===============================*/

   newPtr->next = ConstructData(theEnv)->ListOfConstructs;
   ConstructData(theEnv)->ListOfConstructs = newPtr;
   return(newPtr);
  }

/************************************/
/* AddSaveFunction: Adds a function */
/*   to the ListOfSaveFunctions.    */
/************************************/
globle intBool AddSaveFunction(
  void *theEnv,
  char *name,
  void (*functionPtr)(void *,void *,char *),
  int priority)
  {
#if (MAC_MCW || WIN_MCW) && (RUN_TIME || BLOAD_ONLY)
#pragma unused(name)
#pragma unused(functionPtr)
#pragma unused(priority)
#endif

#if (! RUN_TIME) && (! BLOAD_ONLY)
   ConstructData(theEnv)->ListOfSaveFunctions =
     AddFunctionToCallList(theEnv,name,priority,
                           (void (*)(void *)) functionPtr,
                           ConstructData(theEnv)->ListOfSaveFunctions,TRUE);
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
#endif

   return(1);
  }
