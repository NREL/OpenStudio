   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*                  DEFGLOBAL MODULE                   */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides core routines for the creation and      */
/*   maintenance of the defglobal construct.                 */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Corrected code to remove run-time program      */
/*            compiler warning.                              */
/*                                                           */
/*************************************************************/

#define _GLOBLDEF_SOURCE_

#include "setup.h"

#if DEFGLOBAL_CONSTRUCT

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "memalloc.h"
#include "modulpsr.h"
#include "multifld.h"
#include "router.h"
#include "strngrtr.h"
#include "modulutl.h"
#include "globlbsc.h"
#include "globlpsr.h"
#include "globlcom.h"
#include "utility.h"
#include "commline.h"
#include "envrnmnt.h"

#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
#include "bload.h"
#include "globlbin.h"
#endif

#if CONSTRUCT_COMPILER && (! RUN_TIME)
#include "globlcmp.h"
#endif

#include "globldef.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                   *AllocateModule(void *);
   static void                    ReturnModule(void *,void *);
   static void                    ReturnDefglobal(void *,void *);
   static void                    InitializeDefglobalModules(void *);
   static intBool                 GetDefglobalValue2(void *,void *,DATA_OBJECT_PTR);
   static void                    IncrementDefglobalBusyCount(void *,void *);
   static void                    DecrementDefglobalBusyCount(void *,void *);
   static void                    DeallocateDefglobalData(void *);
   static void                    DestroyDefglobalAction(void *,struct constructHeader *,void *);
   static void                    DestroyDefglobal(void *,void *);

/**************************************************************/
/* InitializeDefglobals: Initializes the defglobal construct. */
/**************************************************************/
globle void InitializeDefglobals(
  void *theEnv)
  {  
   struct entityRecord globalInfo = { "GBL_VARIABLE", GBL_VARIABLE,0,0,0,
                                                       NULL,
                                                       NULL,
                                                       NULL,
                                                       GetDefglobalValue2,
                                                       NULL,NULL,
                                                       NULL,NULL,NULL,NULL,NULL,NULL };

   struct entityRecord defglobalPtrRecord = { "DEFGLOBAL_PTR", DEFGLOBAL_PTR,0,0,0,
                                                       NULL,NULL,NULL,
                                                       QGetDefglobalValue,
                                                       NULL,
                                                       DecrementDefglobalBusyCount,
                                                       IncrementDefglobalBusyCount,
                                                       NULL,NULL,NULL,NULL,NULL };
   
   AllocateEnvironmentData(theEnv,DEFGLOBAL_DATA,sizeof(struct defglobalData),DeallocateDefglobalData);
   
   memcpy(&DefglobalData(theEnv)->GlobalInfo,&globalInfo,sizeof(struct entityRecord));   
   memcpy(&DefglobalData(theEnv)->DefglobalPtrRecord,&defglobalPtrRecord,sizeof(struct entityRecord));   

   DefglobalData(theEnv)->ResetGlobals = TRUE;
   DefglobalData(theEnv)->LastModuleIndex = -1;
   
   InstallPrimitive(theEnv,&DefglobalData(theEnv)->GlobalInfo,GBL_VARIABLE);
   InstallPrimitive(theEnv,&DefglobalData(theEnv)->DefglobalPtrRecord,DEFGLOBAL_PTR);

   InitializeDefglobalModules(theEnv);

   DefglobalBasicCommands(theEnv);
   DefglobalCommandDefinitions(theEnv);

   DefglobalData(theEnv)->DefglobalConstruct =
      AddConstruct(theEnv,"defglobal","defglobals",ParseDefglobal,EnvFindDefglobal,
                   GetConstructNamePointer,GetConstructPPForm,
                   GetConstructModuleItem,EnvGetNextDefglobal,SetNextConstruct,
                   EnvIsDefglobalDeletable,EnvUndefglobal,ReturnDefglobal);
  }

/****************************************************/
/* DeallocateDefglobalData: Deallocates environment */
/*    data for the defglobal construct.             */
/****************************************************/
static void DeallocateDefglobalData(
  void *theEnv)
  {
#if ! RUN_TIME
   struct defglobalModule *theModuleItem;
   void *theModule;
   
#if BLOAD || BLOAD_AND_BSAVE
   if (Bloaded(theEnv)) return;
#endif

   DoForAllConstructs(theEnv,DestroyDefglobalAction,DefglobalData(theEnv)->DefglobalModuleIndex,FALSE,NULL); 

   for (theModule = EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = EnvGetNextDefmodule(theEnv,theModule))
     {
      theModuleItem = (struct defglobalModule *)
                      GetModuleItem(theEnv,(struct defmodule *) theModule,
                                    DefglobalData(theEnv)->DefglobalModuleIndex);
      rtn_struct(theEnv,defglobalModule,theModuleItem);
     }
#else
   DoForAllConstructs(theEnv,DestroyDefglobalAction,DefglobalData(theEnv)->DefglobalModuleIndex,FALSE,NULL); 
#endif
  }
  
/***************************************************/
/* DestroyDefglobalAction: Action used to remove   */
/*   defglobals as a result of DestroyEnvironment. */
/***************************************************/
#if WIN_BTC
#pragma argsused
#endif
static void DestroyDefglobalAction(
  void *theEnv,
  struct constructHeader *theConstruct,
  void *buffer)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(buffer)
#endif
#if (! BLOAD_ONLY)
   struct defglobal *theDefglobal = (struct defglobal *) theConstruct;
   
   if (theDefglobal == NULL) return;

   DestroyDefglobal(theEnv,theDefglobal);
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv,theConstruct)
#endif
#endif
  }

/*********************************************************/
/* InitializeDefglobalModules: Initializes the defglobal */
/*   construct for use with the defmodule construct.     */
/*********************************************************/
static void InitializeDefglobalModules(
  void *theEnv)
  {
   DefglobalData(theEnv)->DefglobalModuleIndex = RegisterModuleItem(theEnv,"defglobal",
                                    AllocateModule,
                                    ReturnModule,
#if BLOAD_AND_BSAVE || BLOAD || BLOAD_ONLY
                                    BloadDefglobalModuleReference,
#else
                                    NULL,
#endif
#if CONSTRUCT_COMPILER && (! RUN_TIME)
                                    DefglobalCModuleReference,
#else
                                    NULL,
#endif
                                    EnvFindDefglobal);

#if (! BLOAD_ONLY) && (! RUN_TIME) && DEFMODULE_CONSTRUCT
   AddPortConstructItem(theEnv,"defglobal",SYMBOL);
#endif
  }

/*************************************************/
/* AllocateModule: Allocates a defglobal module. */
/*************************************************/
static void *AllocateModule(
  void *theEnv)
  {   
   return((void *) get_struct(theEnv,defglobalModule)); 
  }

/***********************************************/
/* ReturnModule: Deallocates a defglobal module. */
/***********************************************/
static void ReturnModule(
  void *theEnv,
  void *theItem)
  {
   FreeConstructHeaderModule(theEnv,(struct defmoduleItemHeader *) theItem,DefglobalData(theEnv)->DefglobalConstruct);
   rtn_struct(theEnv,defglobalModule,theItem);
  }

/**************************************************************/
/* GetDefglobalModuleItem: Returns a pointer to the defmodule */
/*  item for the specified defglobal or defmodule.            */
/**************************************************************/
globle struct defglobalModule *GetDefglobalModuleItem(
  void *theEnv,
  struct defmodule *theModule)
  {
   return((struct defglobalModule *) GetConstructModuleItemByIndex(theEnv,theModule,DefglobalData(theEnv)->DefglobalModuleIndex));
  }

/*****************************************************/
/* EnvFindDefglobal: Searches for a defglobal in the */
/*   list of defglobals. Returns a pointer to the    */
/*   defglobal if found, otherwise NULL.             */
/*****************************************************/
globle void *EnvFindDefglobal(
  void *theEnv,
  char *defglobalName)
  { 
   return(FindNamedConstruct(theEnv,defglobalName,DefglobalData(theEnv)->DefglobalConstruct)); 
  }

/********************************************************************/
/* EnvGetNextDefglobal: If passed a NULL pointer, returns the first */
/*   defglobal in the defglobal list. Otherwise returns the next    */
/*   defglobal following the defglobal passed as an argument.       */
/********************************************************************/
globle void *EnvGetNextDefglobal(
  void *theEnv,
  void *defglobalPtr)
  { 
   return((void *) GetNextConstructItem(theEnv,(struct constructHeader *) defglobalPtr,DefglobalData(theEnv)->DefglobalModuleIndex)); 
  }

/*********************************************************/
/* EnvIsDefglobalDeletable: Returns TRUE if a particular */
/*   defglobal can be deleted, otherwise returns FALSE.  */
/*********************************************************/
globle intBool EnvIsDefglobalDeletable(
  void *theEnv,
  void *ptr)
  {
   if (! ConstructsDeletable(theEnv))
     { return FALSE; }

   if (((struct defglobal *) ptr)->busyCount) return(FALSE);

   return(TRUE);
  }

/************************************************************/
/* ReturnDefglobal: Returns the data structures associated  */
/*   with a defglobal construct to the pool of free memory. */
/************************************************************/
static void ReturnDefglobal(
  void *theEnv,
  void *vTheDefglobal)
  {
#if (MAC_MCW || WIN_MCW) && (RUN_TIME || BLOAD_ONLY)
#pragma unused(theEnv,vTheDefglobal)
#endif
   
#if (! BLOAD_ONLY) && (! RUN_TIME)
   struct defglobal *theDefglobal = (struct defglobal *) vTheDefglobal;
   
   if (theDefglobal == NULL) return;

   /*====================================*/
   /* Return the global's current value. */
   /*====================================*/

   ValueDeinstall(theEnv,&theDefglobal->current);
   if (theDefglobal->current.type == MULTIFIELD)
     { ReturnMultifield(theEnv,(struct multifield *) theDefglobal->current.value); }

   /*================================================*/
   /* Return the expression representing the initial */
   /* value of the defglobal when it was defined.    */
   /*================================================*/

   RemoveHashedExpression(theEnv,theDefglobal->initial);

   /*===============================*/
   /* Release items stored in the   */
   /* defglobal's construct header. */
   /*===============================*/

   DeinstallConstructHeader(theEnv,&theDefglobal->header);

   /*======================================*/
   /* Return the defglobal data structure. */
   /*======================================*/

   rtn_struct(theEnv,defglobal,theDefglobal);

   /*===========================================*/
   /* Set the variable indicating that a change */
   /* has been made to a global variable.       */
   /*===========================================*/

   DefglobalData(theEnv)->ChangeToGlobals = TRUE;
#endif
  }
  
/************************************************************/
/* DestroyDefglobal: Returns the data structures associated  */
/*   with a defglobal construct to the pool of free memory. */
/************************************************************/
static void DestroyDefglobal(
  void *theEnv,
  void *vTheDefglobal)
  {
#if (MAC_MCW || WIN_MCW) && BLOAD_ONLY
#pragma unused(theEnv,vTheDefglobal)
#endif
   
#if (! BLOAD_ONLY)
   struct defglobal *theDefglobal = (struct defglobal *) vTheDefglobal;
   
   if (theDefglobal == NULL) return;

   /*====================================*/
   /* Return the global's current value. */
   /*====================================*/

   if (theDefglobal->current.type == MULTIFIELD)
     { ReturnMultifield(theEnv,(struct multifield *) theDefglobal->current.value); }
     
#if (! RUN_TIME)

   /*===============================*/
   /* Release items stored in the   */
   /* defglobal's construct header. */
   /*===============================*/

   DeinstallConstructHeader(theEnv,&theDefglobal->header);

   /*======================================*/
   /* Return the defglobal data structure. */
   /*======================================*/

   rtn_struct(theEnv,defglobal,theDefglobal);
#endif
#endif
  }
  
/************************************************/
/* QSetDefglobalValue: Lowest level routine for */
/*   setting a defglobal's value.               */
/************************************************/
globle void QSetDefglobalValue(
  void *theEnv,
  struct defglobal *theGlobal,
  DATA_OBJECT_PTR vPtr,
  int resetVar)
  {
   /*====================================================*/
   /* If the new value passed for the defglobal is NULL, */
   /* then reset the defglobal to the initial value it   */
   /* had when it was defined.                           */
   /*====================================================*/

   if (resetVar)
     {
      EvaluateExpression(theEnv,theGlobal->initial,vPtr);
      if (EvaluationData(theEnv)->EvaluationError)
        {
         vPtr->type = SYMBOL;
         vPtr->value = EnvFalseSymbol(theEnv);
        }
     }

   /*==========================================*/
   /* If globals are being watch, then display */
   /* the change to the global variable.       */
   /*==========================================*/

#if DEBUGGING_FUNCTIONS
   if (theGlobal->watch)
     {
      EnvPrintRouter(theEnv,WTRACE,":== ?*");
      EnvPrintRouter(theEnv,WTRACE,ValueToString(theGlobal->header.name));
      EnvPrintRouter(theEnv,WTRACE,"* ==> ");
      PrintDataObject(theEnv,WTRACE,vPtr);
      EnvPrintRouter(theEnv,WTRACE," <== ");
      PrintDataObject(theEnv,WTRACE,&theGlobal->current);
      EnvPrintRouter(theEnv,WTRACE,"\n");
     }
#endif

   /*==============================================*/
   /* Remove the old value of the global variable. */
   /*==============================================*/

   ValueDeinstall(theEnv,&theGlobal->current);
   if (theGlobal->current.type == MULTIFIELD)
     { ReturnMultifield(theEnv,(struct multifield *) theGlobal->current.value); }

   /*===========================================*/
   /* Set the new value of the global variable. */
   /*===========================================*/

   theGlobal->current.type = vPtr->type;
   if (vPtr->type != MULTIFIELD) theGlobal->current.value = vPtr->value;
   else DuplicateMultifield(theEnv,&theGlobal->current,vPtr);
   ValueInstall(theEnv,&theGlobal->current);

   /*===========================================*/
   /* Set the variable indicating that a change */
   /* has been made to a global variable.       */
   /*===========================================*/

   DefglobalData(theEnv)->ChangeToGlobals = TRUE;

   if ((EvaluationData(theEnv)->CurrentEvaluationDepth == 0) && (! CommandLineData(theEnv)->EvaluatingTopLevelCommand) &&
       (EvaluationData(theEnv)->CurrentExpression == NULL))
     { PeriodicCleanup(theEnv,TRUE,FALSE); }
  }

/**************************************************************/
/* QFindDefglobal: Searches for a defglobal in the list of    */
/*   defglobals. Returns a pointer to the defglobal if found, */
/*   otherwise NULL.                                          */
/**************************************************************/
globle struct defglobal *QFindDefglobal(
  void *theEnv,
  SYMBOL_HN *defglobalName)
  {
   struct defglobal *theDefglobal;

   for (theDefglobal = (struct defglobal *) EnvGetNextDefglobal(theEnv,NULL);
        theDefglobal != NULL;
        theDefglobal = (struct defglobal *) EnvGetNextDefglobal(theEnv,theDefglobal))
     { if (defglobalName == theDefglobal->header.name) return (theDefglobal); }

   return(NULL);
  }

/*********************************************************************/
/* EnvGetDefglobalValueForm: Returns the pretty print representation */
/*   of the current value of the specified defglobal. For example,   */
/*   if the current value of ?*x* is 5, the string "?*x* = 5" would  */
/*   be returned.                                                    */
/*********************************************************************/
globle void EnvGetDefglobalValueForm(
  void *theEnv,
  char *buffer,
  unsigned bufferLength,
  void *vTheGlobal)
  {
   struct defglobal *theGlobal = (struct defglobal *) vTheGlobal;

   OpenStringDestination(theEnv,"GlobalValueForm",buffer,bufferLength);
   EnvPrintRouter(theEnv,"GlobalValueForm","?*");
   EnvPrintRouter(theEnv,"GlobalValueForm",ValueToString(theGlobal->header.name));
   EnvPrintRouter(theEnv,"GlobalValueForm","* = ");
   PrintDataObject(theEnv,"GlobalValueForm",&theGlobal->current);
   CloseStringDestination(theEnv,"GlobalValueForm");
  }

/************************************************************/
/* EnvGetGlobalsChanged: Returns the defglobal change flag. */
/************************************************************/
globle int EnvGetGlobalsChanged(
  void *theEnv)
  {    
   return(DefglobalData(theEnv)->ChangeToGlobals); 
  }

/*********************************************************/
/* EnvSetGlobalsChanged: Sets the defglobal change flag. */
/*********************************************************/
globle void EnvSetGlobalsChanged(
  void *theEnv,
  int value)
  {
   DefglobalData(theEnv)->ChangeToGlobals = value; 
  }

/**********************************************************/
/* GetDefglobalValue2: Returns the value of the specified */
/*   global variable in the supplied DATA_OBJECT.         */
/**********************************************************/
static intBool GetDefglobalValue2(
  void *theEnv,
  void *theValue,
  DATA_OBJECT_PTR vPtr)
  {
   struct defglobal *theGlobal;
   int count;

   /*===========================================*/
   /* Search for the specified defglobal in the */
   /* modules visible to the current module.    */
   /*===========================================*/

   theGlobal = (struct defglobal *)
               FindImportedConstruct(theEnv,"defglobal",NULL,ValueToString(theValue),
               &count,TRUE,NULL);

   /*=============================================*/
   /* If it wasn't found, print an error message. */
   /*=============================================*/

   if (theGlobal == NULL)
     {
      PrintErrorID(theEnv,"GLOBLDEF",1,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Global variable ?*");
      EnvPrintRouter(theEnv,WERROR,ValueToString(theValue));
      EnvPrintRouter(theEnv,WERROR,"* is unbound.\n");
      vPtr->type = SYMBOL;
      vPtr->value = EnvFalseSymbol(theEnv);
      SetEvaluationError(theEnv,TRUE);
      return(FALSE);
     }

   /*========================================================*/
   /* The current implementation of the defmodules shouldn't */
   /* allow a construct to be defined which would cause an   */
   /* ambiguous reference, but we'll check for it anyway.    */
   /*========================================================*/

   if (count > 1)
     {
      AmbiguousReferenceErrorMessage(theEnv,"defglobal",ValueToString(theValue));
      vPtr->type = SYMBOL;
      vPtr->value = EnvFalseSymbol(theEnv);
      SetEvaluationError(theEnv,TRUE);
      return(FALSE);
     }

   /*=================================*/
   /* Get the value of the defglobal. */
   /*=================================*/

   QGetDefglobalValue(theEnv,theGlobal,vPtr);

   return(TRUE);
  }

/***************************************************************/
/* QGetDefglobalValue: Returns the value of a global variable. */
/***************************************************************/
globle int QGetDefglobalValue(
  void *theEnv,
  void *vTheGlobal,
  DATA_OBJECT_PTR vPtr)
  {
   struct defglobal *theGlobal = (struct defglobal *) vTheGlobal;

   /*===============================================*/
   /* Transfer values which can be copied directly. */
   /*===============================================*/

   vPtr->type = theGlobal->current.type;
   vPtr->value = theGlobal->current.value;
   vPtr->begin = theGlobal->current.begin;
   vPtr->end = theGlobal->current.end;

   /*===========================================================*/
   /* If the global contains a multifield value, return a copy  */
   /* of the value so that routines which use this value are    */
   /* not affected if the value of the global is later changed. */
   /*===========================================================*/

   if (vPtr->type == MULTIFIELD)
     {
      vPtr->value = EnvCreateMultifield(theEnv,(unsigned long) (vPtr->end + 1));
      GenCopyMemory(struct field,vPtr->end + 1,
                                &((struct multifield *) vPtr->value)->theFields[0],
                                &((struct multifield *) theGlobal->current.value)->theFields[theGlobal->current.begin]);
     }

   return(TRUE);
  }

/************************************************************/
/* EnvGetDefglobalValue: Returns the value of the specified */
/*   global variable in the supplied DATA_OBJECT.           */
/************************************************************/
globle intBool EnvGetDefglobalValue(
  void *theEnv,
  char *variableName,
  DATA_OBJECT_PTR vPtr)
  {
   struct defglobal *theDefglobal;

   if ((theDefglobal = (struct defglobal *) EnvFindDefglobal(theEnv,variableName)) == NULL)
     { return(FALSE); }

   QGetDefglobalValue(theEnv,theDefglobal,vPtr);

   return(TRUE);
  }

/****************************************************************/
/* EnvSetDefglobalValue: Sets the value of the specified global */
/*   variable to the value stored in the supplied DATA_OBJECT.  */
/****************************************************************/
globle intBool EnvSetDefglobalValue(
  void *theEnv,
  char *variableName,
  DATA_OBJECT_PTR vPtr)
  {
   struct defglobal *theGlobal;

   if ((theGlobal = QFindDefglobal(theEnv,(SYMBOL_HN *) EnvAddSymbol(theEnv,variableName))) == NULL)
     { return(FALSE); }

   QSetDefglobalValue(theEnv,theGlobal,vPtr,FALSE);

   return(TRUE);
  }

/**********************************************************/
/* DecrementDefglobalBusyCount: Decrements the busy count */
/*   of a defglobal data structure.                       */
/**********************************************************/
static void DecrementDefglobalBusyCount(
  void *theEnv,
  void *vTheGlobal)
  {
   struct defglobal *theGlobal = (struct defglobal *) vTheGlobal;

   if (! ConstructData(theEnv)->ClearInProgress) theGlobal->busyCount--;
  }

/**********************************************************/
/* IncrementDefglobalBusyCount: Increments the busy count */
/*   of a defglobal data structure.                       */
/**********************************************************/
#if WIN_BTC
#pragma argsused
#endif
static void IncrementDefglobalBusyCount(
  void *theEnv,
  void *vTheGlobal)
  {
   struct defglobal *theGlobal = (struct defglobal *) vTheGlobal;
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   theGlobal->busyCount++;
  }

/***********************************************************************/
/* UpdateDefglobalScope: Updates the scope flag of all the defglobals. */
/***********************************************************************/
globle void UpdateDefglobalScope(
  void *theEnv)
  {
   struct defglobal *theDefglobal;
   int moduleCount;
   struct defmodule *theModule;
   struct defmoduleItemHeader *theItem;
   
   /*============================*/
   /* Loop through every module. */
   /*============================*/

   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      /*============================================================*/
      /* Loop through every defglobal in the module being examined. */
      /*============================================================*/

      theItem = (struct defmoduleItemHeader *)
                GetModuleItem(theEnv,theModule,DefglobalData(theEnv)->DefglobalModuleIndex);

      for (theDefglobal = (struct defglobal *) theItem->firstItem;
           theDefglobal != NULL ;
           theDefglobal = (struct defglobal *) EnvGetNextDefglobal(theEnv,theDefglobal))
        {
         /*====================================================*/
         /* If the defglobal is visible to the current module, */
         /* then mark it as being in scope, otherwise mark it  */
         /* as being out of scope.                             */
         /*====================================================*/

         if (FindImportedConstruct(theEnv,"defglobal",theModule,
                                   ValueToString(theDefglobal->header.name),
                                   &moduleCount,TRUE,NULL) != NULL)
           { theDefglobal->inScope = TRUE; }
         else
           { theDefglobal->inScope = FALSE; }
        }
     }
  }

/*******************************************************/
/* GetNextDefglobalInScope: Returns the next defglobal */
/*   that is scope of the current module. Works in a   */
/*   similar fashion to GetNextDefglobal, but skips    */
/*   defglobals that are out of scope.                 */
/*******************************************************/
globle void *GetNextDefglobalInScope(
  void *theEnv,
  void *vTheGlobal)
  {
   struct defglobal *theGlobal = (struct defglobal *) vTheGlobal;
   struct defmoduleItemHeader *theItem;

   /*=======================================*/
   /* If we're beginning the search for the */
   /* first defglobal in scope, then ...    */
   /*=======================================*/

   if (theGlobal == NULL)
     {
      /*==============================================*/
      /* If the current module has been changed since */
      /* the last time the scopes were computed, then */
      /* recompute the scopes.                        */
      /*==============================================*/

      if (DefglobalData(theEnv)->LastModuleIndex != DefmoduleData(theEnv)->ModuleChangeIndex)
        {
         UpdateDefglobalScope(theEnv);
         DefglobalData(theEnv)->LastModuleIndex = DefmoduleData(theEnv)->ModuleChangeIndex;
        }

      /*==========================================*/
      /* Get the first module and first defglobal */
      /* to start the search with.                */
      /*==========================================*/

      DefglobalData(theEnv)->TheDefmodule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
      theItem = (struct defmoduleItemHeader *)
                GetModuleItem(theEnv,DefglobalData(theEnv)->TheDefmodule,DefglobalData(theEnv)->DefglobalModuleIndex);
      theGlobal = (struct defglobal *) theItem->firstItem;
     }

   /*==================================================*/
   /* Otherwise, see if the last defglobal returned by */
   /* this function has a defglobal following it.      */
   /*==================================================*/

   else
     { theGlobal = (struct defglobal *) EnvGetNextDefglobal(theEnv,theGlobal); }

   /*======================================*/
   /* Continue looping through the modules */
   /* until a defglobal in scope is found. */
   /*======================================*/

   while (DefglobalData(theEnv)->TheDefmodule != NULL)
     {
      /*=====================================================*/
      /* Loop through the defglobals in the module currently */
      /* being examined to see if one is in scope.           */
      /*=====================================================*/

      for (;
           theGlobal != NULL;
           theGlobal = (struct defglobal *) EnvGetNextDefglobal(theEnv,theGlobal))
        { if (theGlobal->inScope) return((void *) theGlobal); }

      /*================================================*/
      /* If a global in scope couldn't be found in this */
      /* module, then move on to the next module.       */
      /*================================================*/

      DefglobalData(theEnv)->TheDefmodule = (struct defmodule *) EnvGetNextDefmodule(theEnv,DefglobalData(theEnv)->TheDefmodule);
      theItem = (struct defmoduleItemHeader *)
                GetModuleItem(theEnv,DefglobalData(theEnv)->TheDefmodule,DefglobalData(theEnv)->DefglobalModuleIndex);
      theGlobal = (struct defglobal *) theItem->firstItem;
     }

   /*====================================*/
   /* All the globals in scope have been */
   /* traversed and there are none left. */
   /*====================================*/

   return(NULL);
  }

#endif /* DEFGLOBAL_CONSTRUCT */


