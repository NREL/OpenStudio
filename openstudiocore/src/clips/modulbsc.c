   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.22  06/15/04            */
   /*                                                     */
   /*         DEFMODULE BASIC COMMANDS HEADER FILE        */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements core commands for the defmodule       */
/*   construct such as clear, reset, save, ppdefmodule       */
/*   list-defmodules, and get-defmodule-list.                */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#define _MODULBSC_SOURCE_

#include "setup.h"

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <string.h>

#include "constrct.h"
#include "extnfunc.h"
#include "modulbin.h"
#include "prntutil.h"
#include "modulcmp.h"
#include "router.h"
#include "argacces.h"
#include "bload.h"
#include "envrnmnt.h"

#include "modulbsc.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                    ClearDefmodules(void *);
#if DEFMODULE_CONSTRUCT
   static void                    SaveDefmodules(void *,void *,char *);
#endif

/*****************************************************************/
/* DefmoduleBasicCommands: Initializes basic defmodule commands. */
/*****************************************************************/
globle void DefmoduleBasicCommands(
  void *theEnv)
  {
   EnvAddClearFunction(theEnv,"defmodule",ClearDefmodules,2000);

#if DEFMODULE_CONSTRUCT
   AddSaveFunction(theEnv,"defmodule",SaveDefmodules,1100);

#if ! RUN_TIME
   EnvDefineFunction2(theEnv,"get-defmodule-list",'m',PTIEF EnvGetDefmoduleList,"EnvGetDefmoduleList","00");

#if DEBUGGING_FUNCTIONS
   EnvDefineFunction2(theEnv,"list-defmodules",'v', PTIEF ListDefmodulesCommand,"ListDefmodulesCommand","00");
   EnvDefineFunction2(theEnv,"ppdefmodule",'v',PTIEF PPDefmoduleCommand,"PPDefmoduleCommand","11w");
#endif
#endif
#endif

#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE)
   DefmoduleBinarySetup(theEnv);
#endif

#if CONSTRUCT_COMPILER && (! RUN_TIME)
   DefmoduleCompilerSetup(theEnv);
#endif
  }

/*********************************************************/
/* ClearDefmodules: Defmodule clear routine for use with */
/*   the clear command. Creates the MAIN module.         */
/*********************************************************/
static void ClearDefmodules(
  void *theEnv)
  {
#if (BLOAD || BLOAD_AND_BSAVE || BLOAD_ONLY) && (! RUN_TIME)
   if (Bloaded(theEnv) == TRUE) return;
#endif
#if (! RUN_TIME)
   RemoveAllDefmodules(theEnv);

   CreateMainModule(theEnv);
   DefmoduleData(theEnv)->MainModuleRedefinable = TRUE;
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
#endif
  }

#if DEFMODULE_CONSTRUCT

/******************************************/
/* SaveDefmodules: Defmodule save routine */
/*   for use with the save command.       */
/******************************************/
static void SaveDefmodules(
  void *theEnv,
  void *theModule,
  char *logicalName)
  {
   char *ppform;

   ppform = EnvGetDefmodulePPForm(theEnv,theModule);
   if (ppform != NULL)
     {
      PrintInChunks(theEnv,logicalName,ppform);
      EnvPrintRouter(theEnv,logicalName,"\n");
     }
  }

/*************************************************/
/* EnvGetDefmoduleList: H/L and C access routine */
/*   for the get-defmodule-list function.        */
/*************************************************/
globle void EnvGetDefmoduleList(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   OldGetConstructList(theEnv,returnValue,EnvGetNextDefmodule,EnvGetDefmoduleName); 
  }

#if DEBUGGING_FUNCTIONS

/********************************************/
/* PPDefmoduleCommand: H/L access routine   */
/*   for the ppdefmodule command.           */
/********************************************/
globle void PPDefmoduleCommand(
  void *theEnv)
  {
   char *defmoduleName;

   defmoduleName = GetConstructName(theEnv,"ppdefmodule","defmodule name");
   if (defmoduleName == NULL) return;

   PPDefmodule(theEnv,defmoduleName,WDISPLAY);

   return;
  }

/*************************************/
/* PPDefmodule: C access routine for */
/*   the ppdefmodule command.        */
/*************************************/
globle int PPDefmodule(
  void *theEnv,
  char *defmoduleName,
  char *logicalName)
  {
   void *defmodulePtr;

   defmodulePtr = EnvFindDefmodule(theEnv,defmoduleName);
   if (defmodulePtr == NULL)
     {
      CantFindItemErrorMessage(theEnv,"defmodule",defmoduleName);
      return(FALSE);
     }

   if (EnvGetDefmodulePPForm(theEnv,defmodulePtr) == NULL) return(TRUE);
   PrintInChunks(theEnv,logicalName,EnvGetDefmodulePPForm(theEnv,defmodulePtr));
   return(TRUE);
  }

/***********************************************/
/* ListDefmodulesCommand: H/L access routine   */
/*   for the list-defmodules command.          */
/***********************************************/
globle void ListDefmodulesCommand(
  void *theEnv)
  {
   if (EnvArgCountCheck(theEnv,"list-defmodules",EXACTLY,0) == -1) return;

   EnvListDefmodules(theEnv,WDISPLAY);
  }

/***************************************/
/* EnvListDefmodules: C access routine */
/*   for the list-defmodules command.  */
/***************************************/
globle void EnvListDefmodules(
  void *theEnv,
  char *logicalName)
  {
   void *theModule;
   int count = 0;

   for (theModule = EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = EnvGetNextDefmodule(theEnv,theModule))
    {
     EnvPrintRouter(theEnv,logicalName,EnvGetDefmoduleName(theEnv,theModule));
     EnvPrintRouter(theEnv,logicalName,"\n");
     count++;
    }

   PrintTally(theEnv,logicalName,count,"defmodule","defmodules");
  }

#endif /* DEBUGGING_FUNCTIONS */

#endif /* DEFMODULE_CONSTRUCT */


