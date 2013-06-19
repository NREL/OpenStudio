   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*              DEFGLOBAL COMMANDS MODULE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides the show-defglobals, set-reset-globals, */
/*   and get-reset-globals commands.                         */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#define _GLOBLCOM_SOURCE_

#include "setup.h"

#if DEFGLOBAL_CONSTRUCT

#include "extnfunc.h"
#include "argacces.h"
#include "prntutil.h"
#include "router.h"
#include "envrnmnt.h"

#include "globldef.h"

#include "globlcom.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if DEBUGGING_FUNCTIONS
   static void                       PrintDefglobalValueForm(void *,char *,void *);
#endif

/************************************************************/
/* DefglobalCommandDefinitions: Defines defglobal commands. */
/************************************************************/
globle void DefglobalCommandDefinitions(
  void *theEnv)
  {
#if ! RUN_TIME
   EnvDefineFunction2(theEnv,"set-reset-globals",'b',
                  SetResetGlobalsCommand,"SetResetGlobalsCommand", "11");
   EnvDefineFunction2(theEnv,"get-reset-globals",'b',
                   GetResetGlobalsCommand,"GetResetGlobalsCommand", "00");

#if DEBUGGING_FUNCTIONS
   EnvDefineFunction2(theEnv,"show-defglobals",'v',
                   PTIEF ShowDefglobalsCommand,"ShowDefglobalsCommand", "01w");
#endif

#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
#endif
  }

/************************************************/
/* SetResetGlobalsCommand: H/L access routine   */
/*   for the get-reset-globals command.         */
/************************************************/
globle int SetResetGlobalsCommand(
  void *theEnv)
  {
   int oldValue;
   DATA_OBJECT arg_ptr;

   /*===========================================*/
   /* Remember the old value of this attribute. */
   /*===========================================*/

   oldValue = EnvGetResetGlobals(theEnv);

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if (EnvArgCountCheck(theEnv,"set-reset-globals",EXACTLY,1) == -1)
     { return(oldValue); }

   /*===========================================*/
   /* Determine the new value of the attribute. */
   /*===========================================*/

   EnvRtnUnknown(theEnv,1,&arg_ptr);

   if ((arg_ptr.value == EnvFalseSymbol(theEnv)) && (arg_ptr.type == SYMBOL))
     { EnvSetResetGlobals(theEnv,FALSE); }
   else
     { EnvSetResetGlobals(theEnv,TRUE); }

   /*========================================*/
   /* Return the old value of the attribute. */
   /*========================================*/

   return(oldValue);
  }

/****************************************/
/* EnvSetResetGlobals: C access routine */
/*   for the set-reset-globals command. */
/****************************************/
globle intBool EnvSetResetGlobals(
  void *theEnv,
  int value)
  {
   int ov;

   ov = DefglobalData(theEnv)->ResetGlobals;
   DefglobalData(theEnv)->ResetGlobals = value;
   return(ov);
  }

/************************************************/
/* GetResetGlobalsCommand: H/L access routine   */
/*   for the get-reset-globals command.         */
/************************************************/
globle int GetResetGlobalsCommand(
  void *theEnv)
  {
   int oldValue;

   oldValue = EnvGetResetGlobals(theEnv);

   if (EnvArgCountCheck(theEnv,"get-reset-globals",EXACTLY,0) == -1)
     { return(oldValue); }

   return(oldValue);
  }

/****************************************/
/* EnvGetResetGlobals: C access routine */
/*   for the get-reset-globals command. */
/****************************************/
globle intBool EnvGetResetGlobals(
  void *theEnv)
  {   
   return(DefglobalData(theEnv)->ResetGlobals); 
  }

#if DEBUGGING_FUNCTIONS

/***********************************************/
/* ShowDefglobalsCommand: H/L access routine   */
/*   for the show-defglobals command.          */
/***********************************************/
globle void ShowDefglobalsCommand(
  void *theEnv)
  {
   struct defmodule *theModule;
   int numArgs, error;

   if ((numArgs = EnvArgCountCheck(theEnv,"show-defglobals",NO_MORE_THAN,1)) == -1) return;

   if (numArgs == 1)
     {
      theModule = GetModuleName(theEnv,"show-defglobals",1,&error);
      if (error) return;
     }
   else
     { theModule = ((struct defmodule *) EnvGetCurrentModule(theEnv)); }

   EnvShowDefglobals(theEnv,WDISPLAY,theModule);
  }

/***************************************/
/* EnvShowDefglobals: C access routine */
/*   for the show-defglobals command.  */
/***************************************/
globle void EnvShowDefglobals(
  void *theEnv,
  char *logicalName,
  void *vTheModule)
  {
   struct defmodule *theModule = (struct defmodule *) vTheModule;
   struct constructHeader *constructPtr;
   int allModules = FALSE;
   struct defmoduleItemHeader *theModuleItem;

   /*=======================================*/
   /* If the module specified is NULL, then */
   /* list all constructs in all modules.   */
   /*=======================================*/

   if (theModule == NULL)
     {
      theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
      allModules = TRUE;
     }

   /*======================================================*/
   /* Print out the constructs in the specified module(s). */
   /*======================================================*/

   for (;
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      /*===========================================*/
      /* Print the module name before every group  */
      /* of defglobals listed if we're listing the */
      /* defglobals from every module.             */
      /*===========================================*/

      if (allModules)
        {
         EnvPrintRouter(theEnv,logicalName,EnvGetDefmoduleName(theEnv,theModule));
         EnvPrintRouter(theEnv,logicalName,":\n");
        }

      /*=====================================*/
      /* Print every defglobal in the module */
      /* currently being examined.           */
      /*=====================================*/

      theModuleItem = (struct defmoduleItemHeader *) GetModuleItem(theEnv,theModule,DefglobalData(theEnv)->DefglobalModuleIndex);

      for (constructPtr = theModuleItem->firstItem;
           constructPtr != NULL;
           constructPtr = constructPtr->next)
        {
         if (EvaluationData(theEnv)->HaltExecution == TRUE) return;

         if (allModules) EnvPrintRouter(theEnv,logicalName,"   ");
         PrintDefglobalValueForm(theEnv,logicalName,(void *) constructPtr);
         EnvPrintRouter(theEnv,logicalName,"\n");
        }

      /*===================================*/
      /* If we're only listing the globals */
      /* for one module, then return.      */
      /*===================================*/

      if (! allModules) return;
     }
  }

/*****************************************************/
/* PrintDefglobalValueForm: Prints the value form of */
/*   a defglobal (the current value). For example,   */
/*   ?*x* = 3                                        */
/*****************************************************/
static void PrintDefglobalValueForm(
  void *theEnv,
  char *logicalName,
  void *vTheGlobal)
  {
   struct defglobal *theGlobal = (struct defglobal *) vTheGlobal;

   EnvPrintRouter(theEnv,logicalName,"?*");
   EnvPrintRouter(theEnv,logicalName,ValueToString(theGlobal->header.name));
   EnvPrintRouter(theEnv,logicalName,"* = ");
   PrintDataObject(theEnv,logicalName,&theGlobal->current);
  }

#endif /* DEBUGGING_FUNCTIONS */

#endif /* DEFGLOBAL_CONSTRUCT */

