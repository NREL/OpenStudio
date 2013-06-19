   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  07/01/05            */
   /*                                                     */
   /*              CONSTRUCT COMMANDS MODULE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains generic routines for deleting, pretty   */
/*   printing, finding, obtaining module information,        */
/*   obtaining lists of constructs, listing constructs, and  */
/*   manipulation routines.                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Modified GetConstructList to remove buffer     */
/*            overflow problem with large construct/module   */
/*            names. DR0858                                  */
/*                                                           */
/*            Changed name of variable log to logName        */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*            Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Corrected an error when compiling as a C++     */
/*            file. DR0868                                   */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Added ConstructsDeletable function.            */
/*                                                           */
/*************************************************************/

#define _CSTRCCOM_SOURCE_

#include <string.h>

#include "setup.h"

#include "constant.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "moduldef.h"
#include "argacces.h"
#include "multifld.h"
#include "modulutl.h"
#include "router.h"
#include "utility.h"
#include "commline.h"
#include "sysdep.h"

#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
#include "bload.h"
#endif

#if (! BLOAD_ONLY) && (! RUN_TIME)
#include "cstrcpsr.h"
#endif

#include "cstrccom.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if DEBUGGING_FUNCTIONS
   static void                    ConstructPrintWatch(void *,char *,struct construct *,void *,
                                                      unsigned (*)(void *,void *));
   static unsigned                ConstructWatchSupport(void *,struct construct *,char *,
                                                        char *,EXPRESSION *,intBool,
                                                        unsigned,unsigned (*)(void *,void *),
                                                        void (*)(void *,unsigned,void *));
#endif

#if (! RUN_TIME)

/************************************/
/* AddConstructToModule: Adds a     */
/* construct to the current module. */
/************************************/
globle void AddConstructToModule(
  struct constructHeader *theConstruct)
  {
   if (theConstruct->whichModule->lastItem == NULL)
     { theConstruct->whichModule->firstItem = theConstruct; }
   else
     { theConstruct->whichModule->lastItem->next = theConstruct; }

   theConstruct->whichModule->lastItem = theConstruct;
   theConstruct->next = NULL;
  }

#endif /* (! RUN_TIME) */

/****************************************************/
/* DeleteNamedConstruct: Generic driver routine for */
/*   deleting a specific construct from a module.   */
/****************************************************/
globle intBool DeleteNamedConstruct(
  void *theEnv,
  char *constructName,
  struct construct *constructClass)
  {
#if (! BLOAD_ONLY)
   void *constructPtr;

   /*=============================*/
   /* Constructs can't be deleted */
   /* while a bload is in effect. */
   /*=============================*/

#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
   if (Bloaded(theEnv) == TRUE) return(FALSE);
#endif

   /*===============================*/
   /* Look for the named construct. */
   /*===============================*/

   constructPtr = (*constructClass->findFunction)(theEnv,constructName);

   /*========================================*/
   /* If the construct was found, delete it. */
   /*========================================*/

   if (constructPtr != NULL)
     { return((*constructClass->deleteFunction)(theEnv,constructPtr)); }

   /*========================================*/
   /* If the construct wasn't found, but the */
   /* special symbol * was used, then delete */
   /* all constructs of the specified type.  */
   /*========================================*/

   if (strcmp("*",constructName) == 0)
     {
      (*constructClass->deleteFunction)(theEnv,NULL);
      return(TRUE);
     }

   /*===============================*/
   /* Otherwise, return FALSE to    */
   /* indicate no deletion occured. */
   /*===============================*/

   return(FALSE);
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv,constructName,constructClass)
#endif
   return(FALSE);
#endif
  }

/*******************************************/
/* FindNamedConstruct: Generic routine for */
/*   searching for a specified construct.  */
/*******************************************/
globle void *FindNamedConstruct(
  void *theEnv,
  char *constructName,
  struct construct *constructClass)
  {
   void *theConstruct;
   SYMBOL_HN *findValue;

   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*=========================================================*/
   /* Extract the construct name. If a module was specified,  */
   /* then ExtractModuleAndConstructName will set the current */
   /* module to the module specified in the name.             */
   /*=========================================================*/

   constructName = ExtractModuleAndConstructName(theEnv,constructName);

   /*=================================================*/
   /* If a valid construct name couldn't be extracted */
   /* or the construct name isn't in the symbol table */
   /* (which means the construct doesn't exist), then */
   /* return NULL to indicate the specified construct */
   /* couldn't be found.                              */
   /*=================================================*/

   if ((constructName == NULL) ?
       TRUE :
       ((findValue = (SYMBOL_HN *) FindSymbolHN(theEnv,constructName)) == NULL))
     {
      RestoreCurrentModule(theEnv);
      return(NULL);
     }

   /*===============================================*/
   /* Loop through every construct of the specified */
   /* class in the current module checking to see   */
   /* if the construct's name matches the construct */
   /* being sought. If found, restore the current   */
   /* module and return a pointer to the construct. */
   /*===============================================*/

   for (theConstruct = (*constructClass->getNextItemFunction)(theEnv,NULL);
        theConstruct != NULL;
        theConstruct = (*constructClass->getNextItemFunction)(theEnv,theConstruct))
     {
      if (findValue == (*constructClass->getConstructNameFunction)((struct constructHeader *) theConstruct))
        {
         RestoreCurrentModule(theEnv);
         return (theConstruct);
        }
     }

   /*=============================*/
   /* Restore the current module. */
   /*=============================*/
   
   RestoreCurrentModule(theEnv);

   /*====================================*/
   /* Return NULL to indicated the named */
   /* construct was not found.           */
   /*====================================*/

   return(NULL);
  }

/*****************************************/
/* UndefconstructCommand: Driver routine */
/*   for the undef<construct> commands.  */
/*****************************************/
globle void UndefconstructCommand(
  void *theEnv,
  char *command,
  struct construct *constructClass)
  {
   char *constructName;
   char buffer[80];

   /*==============================================*/
   /* Get the name of the construct to be deleted. */
   /*==============================================*/

   gensprintf(buffer,"%s name",constructClass->constructName);

   constructName = GetConstructName(theEnv,command,buffer);
   if (constructName == NULL) return;

#if (! RUN_TIME) && (! BLOAD_ONLY)

   /*=============================================*/
   /* Check to see if the named construct exists. */
   /*=============================================*/

   if (((*constructClass->findFunction)(theEnv,constructName) == NULL) &&
       (strcmp("*",constructName) != 0))
     {
      CantFindItemErrorMessage(theEnv,constructClass->constructName,constructName);
      return;
     }

   /*===============================================*/
   /* If the construct does exist, try deleting it. */
   /*===============================================*/

   else if (DeleteNamedConstruct(theEnv,constructName,constructClass) == FALSE)
     {
      CantDeleteItemErrorMessage(theEnv,constructClass->constructName,constructName);
      return;
     }

   return;
#else
   /*=====================================*/
   /* Constructs can't be deleted in a    */
   /* run-time or bload only environment. */
   /*=====================================*/

   CantDeleteItemErrorMessage(theEnv,constructClass->constructName,constructName);
   return;
#endif
  }

/******************************************/
/* PPConstructCommand: Driver routine for */
/*   the ppdef<construct> commands.       */
/******************************************/
globle void PPConstructCommand(
  void *theEnv,
  char *command,
  struct construct *constructClass)
  {
   char *constructName;
   char buffer[80];

   /*===============================*/
   /* Get the name of the construct */
   /* to be "pretty printed."       */
   /*===============================*/

   gensprintf(buffer,"%s name",constructClass->constructName);

   constructName = GetConstructName(theEnv,command,buffer);
   if (constructName == NULL) return;

   /*================================*/
   /* Call the driver routine for    */
   /* pretty printing the construct. */
   /*================================*/

   if (PPConstruct(theEnv,constructName,WDISPLAY,constructClass) == FALSE)
     { CantFindItemErrorMessage(theEnv,constructClass->constructName,constructName); }
  }

/***********************************/
/* PPConstruct: Driver routine for */
/*   pretty printing a construct.  */
/***********************************/
globle int PPConstruct(
  void *theEnv,
  char *constructName,
  char *logicalName,
  struct construct *constructClass)
  {
   void *constructPtr;

   /*==================================*/
   /* Use the construct's name to find */
   /* a pointer to actual construct.   */
   /*==================================*/

   constructPtr = (*constructClass->findFunction)(theEnv,constructName);
   if (constructPtr == NULL) return(FALSE);

   /*==============================================*/
   /* If the pretty print form is NULL (because of */
   /* conserve-mem), return TRUE (which indicates  */
   /* the construct was found).                    */
   /*==============================================*/

   if ((*constructClass->getPPFormFunction)(theEnv,(struct constructHeader *) constructPtr) == NULL)
     { return(TRUE); }

   /*============================================*/
   /* Print the pretty print string in smaller   */
   /* chunks. (VMS had a bug that didn't allow   */
   /* printing a string greater than 512 bytes.) */
   /*============================================*/

   PrintInChunks(theEnv,logicalName,(*constructClass->getPPFormFunction)(theEnv,(struct constructHeader *) constructPtr));

   /*=======================================*/
   /* Return TRUE to indicate the construct */
   /* was found and pretty printed.         */
   /*=======================================*/

   return(TRUE);
  }

/*********************************************/
/* GetConstructModuleCommand: Driver routine */
/*   for def<construct>-module routines      */
/*********************************************/
globle SYMBOL_HN *GetConstructModuleCommand(
  void *theEnv,
  char *command,
  struct construct *constructClass)
  {
   char *constructName;
   char buffer[80];
   struct defmodule *constructModule;

   /*=========================================*/
   /* Get the name of the construct for which */
   /* we want to determine its module.        */
   /*=========================================*/

   gensprintf(buffer,"%s name",constructClass->constructName);

   constructName = GetConstructName(theEnv,command,buffer);
   if (constructName == NULL) return((SYMBOL_HN *) EnvFalseSymbol(theEnv));

   /*==========================================*/
   /* Get a pointer to the construct's module. */
   /*==========================================*/

   constructModule = GetConstructModule(theEnv,constructName,constructClass);
   if (constructModule == NULL)
     {
      CantFindItemErrorMessage(theEnv,constructClass->constructName,constructName);
      return((SYMBOL_HN *) EnvFalseSymbol(theEnv));
     }

   /*============================================*/
   /* Return the name of the construct's module. */
   /*============================================*/

   return(constructModule->name);
  }

/******************************************/
/* GetConstructModule: Driver routine for */
/*   getting the module for a construct   */
/******************************************/
globle struct defmodule *GetConstructModule(
  void *theEnv,
  char *constructName,
  struct construct *constructClass)
  {
   struct constructHeader *constructPtr;
   int count;
   unsigned position;
   SYMBOL_HN *theName;

   /*====================================================*/
   /* If the construct name contains a module specifier, */
   /* then get a pointer to the defmodule associated     */
   /* with the specified name.                           */
   /*====================================================*/

   if ((position = FindModuleSeparator(constructName)) != FALSE)
     {
      theName = ExtractModuleName(theEnv,position,constructName);
      if (theName != NULL)
        { return((struct defmodule *) EnvFindDefmodule(theEnv,ValueToString(theName))); }
     }

   /*============================================*/
   /* No module was specified, so search for the */
   /* named construct in the current module and  */
   /* modules from which it imports.             */
   /*============================================*/

   constructPtr = (struct constructHeader *)
                  FindImportedConstruct(theEnv,constructClass->constructName,NULL,constructName,
                                        &count,TRUE,NULL);
   if (constructPtr == NULL) return(NULL);

   return(constructPtr->whichModule->theModule);
  }

/*************************************/
/* Undefconstruct: Generic C routine */
/*   for deleting a construct.       */
/*************************************/
globle intBool Undefconstruct(
  void *theEnv,
  void *theConstruct,
  struct construct *constructClass)
  {
#if BLOAD_ONLY || RUN_TIME
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theConstruct)
#pragma unused(constructClass)
#pragma unused(theEnv)
#endif
   return(FALSE);
#else
   void *currentConstruct,*nextConstruct;
   intBool success;

   /*================================================*/
   /* Delete all constructs of the specified type if */
   /* the construct pointer is the NULL pointer.     */
   /*================================================*/

   if (theConstruct == NULL)
     {
      success = TRUE;

      /*===================================================*/
      /* Loop through all of the constructs in the module. */
      /*===================================================*/

      currentConstruct = (*constructClass->getNextItemFunction)(theEnv,NULL);
      while (currentConstruct != NULL)
        {
         /*==============================*/
         /* Remember the next construct. */
         /*==============================*/

         nextConstruct = (*constructClass->getNextItemFunction)(theEnv,currentConstruct);

         /*=============================*/
         /* Try deleting the construct. */
         /*=============================*/

         if ((*constructClass->isConstructDeletableFunction)(theEnv,currentConstruct))
           {
            RemoveConstructFromModule(theEnv,(struct constructHeader *) currentConstruct);
            (*constructClass->freeFunction)(theEnv,currentConstruct);
           }
         else
           {
            CantDeleteItemErrorMessage(theEnv,constructClass->constructName,
                        ValueToString((*constructClass->getConstructNameFunction)((struct constructHeader *) currentConstruct)));
            success = FALSE;
           }

         /*================================*/
         /* Move on to the next construct. */
         /*================================*/

         currentConstruct = nextConstruct;
        }

      /*=======================================*/
      /* Perform periodic cleanup if embedded. */
      /*=======================================*/

      if ((EvaluationData(theEnv)->CurrentEvaluationDepth == 0) && (! CommandLineData(theEnv)->EvaluatingTopLevelCommand) &&
          (EvaluationData(theEnv)->CurrentExpression == NULL))
        { PeriodicCleanup(theEnv,TRUE,FALSE); }

      /*============================================*/
      /* Return TRUE if all constructs successfully */
      /* deleted, otherwise FALSE.                  */
      /*============================================*/

      return(success);
     }

   /*==================================================*/
   /* Return FALSE if the construct cannot be deleted. */
   /*==================================================*/

   if ((*constructClass->isConstructDeletableFunction)(theEnv,theConstruct) == FALSE)
     { return(FALSE); }

   /*===========================*/
   /* Remove the construct from */
   /* the list in its module.   */
   /*===========================*/

   RemoveConstructFromModule(theEnv,(struct constructHeader *) theConstruct);

   /*=======================*/
   /* Delete the construct. */
   /*=======================*/

   (*constructClass->freeFunction)(theEnv,theConstruct);

   /*=======================================*/
   /* Perform periodic cleanup if embedded. */
   /*=======================================*/

   if ((EvaluationData(theEnv)->CurrentEvaluationDepth == 0) && (! CommandLineData(theEnv)->EvaluatingTopLevelCommand) &&
       (EvaluationData(theEnv)->CurrentExpression == NULL))
     { PeriodicCleanup(theEnv,TRUE,FALSE); }

   /*=============================*/
   /* Return TRUE to indicate the */
   /* construct was deleted.      */
   /*=============================*/

   return(TRUE);
#endif
  }

/***********************************/
/* SaveConstruct: Generic routine  */
/*   for saving a construct class. */
/***********************************/
globle void SaveConstruct(
  void *theEnv,
  void *theModule,
  char *logicalName,
  struct construct *constructClass)
  {
   char *ppform;
   struct constructHeader *theConstruct;

   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*===========================*/
   /* Set the current module to */
   /* the one we're examining.  */
   /*===========================*/

   EnvSetCurrentModule(theEnv,theModule);

   /*==============================================*/
   /* Loop through each construct of the specified */
   /* construct class in the module.               */
   /*==============================================*/

   for (theConstruct = (struct constructHeader *)
                       (*constructClass->getNextItemFunction)(theEnv,NULL);
        theConstruct != NULL;
        theConstruct = (struct constructHeader *)
                       (*constructClass->getNextItemFunction)(theEnv,theConstruct))
     {
      /*==========================================*/
      /* Print the construct's pretty print form. */
      /*==========================================*/

      ppform = (*constructClass->getPPFormFunction)(theEnv,theConstruct);
      if (ppform != NULL)
        {
         PrintInChunks(theEnv,logicalName,ppform);
         EnvPrintRouter(theEnv,logicalName,"\n");
        }
      }

   /*=============================*/
   /* Restore the current module. */
   /*=============================*/

   RestoreCurrentModule(theEnv);
  }

/*********************************************************/
/* GetConstructModuleName: Generic routine for returning */
/*   the name of the module to which a construct belongs */
/*********************************************************/
globle char *GetConstructModuleName(
  struct constructHeader *theConstruct)
  { return(EnvGetDefmoduleName(NULL,(void *) theConstruct->whichModule->theModule)); }

/*********************************************************/
/* GetConstructNameString: Generic routine for returning */
/*   the name string of a construct.                     */
/*********************************************************/
globle char *GetConstructNameString(
  struct constructHeader *theConstruct)
  { return(ValueToString(theConstruct->name)); }

/**************************************************/
/* EnvGetConstructNameString: Generic routine for */
/*   returning the name string of a construct.    */
/**************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle char *EnvGetConstructNameString(
  void *theEnv,
  struct constructHeader *theConstruct)
  { 
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   return(ValueToString(theConstruct->name)); 
  }

/**********************************************************/
/* GetConstructNamePointer: Generic routine for returning */
/*   the name pointer of a construct.                     */
/**********************************************************/
globle SYMBOL_HN *GetConstructNamePointer(
  struct constructHeader *theConstruct)
  { return(theConstruct->name); }

/************************************************/
/* GetConstructListFunction: Generic Routine    */
/*   for retrieving the constructs in a module. */
/************************************************/
globle void GetConstructListFunction(
  void *theEnv,
  char *functionName,
  DATA_OBJECT_PTR returnValue,
  struct construct *constructClass)
  {
   struct defmodule *theModule;
   DATA_OBJECT result;
   int numArgs;

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if ((numArgs = EnvArgCountCheck(theEnv,functionName,NO_MORE_THAN,1)) == -1)
     {
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*====================================*/
   /* If an argument was given, check to */
   /* see that it's a valid module name. */
   /*====================================*/

   if (numArgs == 1)
     {
      /*======================================*/
      /* Only symbols are valid module names. */
      /*======================================*/

      EnvRtnUnknown(theEnv,1,&result);
      if (GetType(result) != SYMBOL)
        {
         EnvSetMultifieldErrorValue(theEnv,returnValue);
         ExpectedTypeError1(theEnv,functionName,1,"defmodule name");
         return;
        }

      /*===========================================*/
      /* Verify that the named module exists or is */
      /* the symbol * (for obtaining the construct */
      /* list for all modules).                    */
      /*===========================================*/

      if ((theModule = (struct defmodule *) EnvFindDefmodule(theEnv,DOToString(result))) == NULL)
        {
         if (strcmp("*",DOToString(result)) != 0)
           {
            EnvSetMultifieldErrorValue(theEnv,returnValue);
            ExpectedTypeError1(theEnv,functionName,1,"defmodule name");
            return;
           }

         theModule = NULL;
        }
     }

   /*=====================================*/
   /* Otherwise use the current module to */
   /* generate the construct list.        */
   /*=====================================*/

   else
     { theModule = ((struct defmodule *) EnvGetCurrentModule(theEnv)); }

   /*=============================*/
   /* Call the driver routine to  */
   /* get the list of constructs. */
   /*=============================*/

   GetConstructList(theEnv,returnValue,constructClass,theModule);
  }

/********************************************/
/* GetConstructList: Generic C Routine for  */
/*   retrieving the constructs in a module. */
/********************************************/
globle void GetConstructList(
  void *theEnv,
  DATA_OBJECT_PTR returnValue,
  struct construct *constructClass,
  struct defmodule *theModule)
  {
   void *theConstruct;
   unsigned long count = 0;
   struct multifield *theList;
   SYMBOL_HN *theName;
   struct defmodule *loopModule;
   int allModules = FALSE;
#if WIN_BTC
   size_t largestConstructNameSize, bufferSize = 80;      /* prevents warning */
#else
   size_t largestConstructNameSize = 0, bufferSize = 80;  /* prevents warning */
#endif
   char *buffer;
   
   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*=======================================*/
   /* If the module specified is NULL, then */
   /* get all constructs in all modules.    */
   /*=======================================*/

   if (theModule == NULL)
     {
      theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
      allModules = TRUE;
     }

   /*======================================================*/
   /* Count the number of constructs to  be retrieved and  */
   /* determine the buffer size needed to store the        */
   /* module-name::construct-names that will be generated. */
   /*======================================================*/

   loopModule = theModule;
   while (loopModule != NULL)
     {
      size_t tempSize;

      /*======================================================*/
      /* Set the current module to the module being examined. */
      /*======================================================*/
            
      EnvSetCurrentModule(theEnv,(void *) loopModule);

      /*===========================================*/
      /* Loop over every construct in the  module. */
      /*===========================================*/
                  
      theConstruct = NULL;
      largestConstructNameSize = 0;
      
      while ((theConstruct = (*constructClass->getNextItemFunction)(theEnv,theConstruct)) != NULL)
        { 
         /*================================*/
         /* Increment the construct count. */
         /*================================*/
         
         count++; 

         /*=================================================*/
         /* Is this the largest construct name encountered? */
         /*=================================================*/
         
         tempSize = strlen(ValueToString((*constructClass->getConstructNameFunction)((struct constructHeader *) theConstruct)));
         if (tempSize > largestConstructNameSize)
           { largestConstructNameSize = tempSize; }
        }
        
      /*========================================*/
      /* Determine the size of the module name. */
      /*========================================*/
      
      tempSize = strlen(EnvGetDefmoduleName(theEnv,loopModule));

      /*======================================================*/
      /* The buffer must be large enough for the module name, */
      /* the largest name of all the constructs, and the ::.  */
      /*======================================================*/
            
      if ((tempSize + largestConstructNameSize + 5) > bufferSize)
        { bufferSize = tempSize + largestConstructNameSize + 5; }

      /*=============================*/
      /* Move on to the next module. */
      /*=============================*/
      
      if (allModules) loopModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,loopModule);
      else loopModule = NULL;
     }

   /*===========================*/
   /* Allocate the name buffer. */
   /*===========================*/
   
   buffer = (char *) genalloc(theEnv,bufferSize);
   
   /*================================*/
   /* Create the multifield value to */
   /* store the construct names.     */
   /*================================*/

   SetpType(returnValue,MULTIFIELD);
   SetpDOBegin(returnValue,1);
   SetpDOEnd(returnValue,(long) count);
   theList = (struct multifield *) EnvCreateMultifield(theEnv,count);
   SetpValue(returnValue,(void *) theList);

   /*===========================*/
   /* Store the construct names */
   /* in the multifield value.  */
   /*===========================*/

   loopModule = theModule;
   count = 1;
   while (loopModule != NULL)
     {
      /*============================*/
      /* Set the current module to  */
      /* the module being examined. */
      /*============================*/

      EnvSetCurrentModule(theEnv,(void *) loopModule);

      /*===============================*/
      /* Add each construct name found */
      /* in the module to the list.    */
      /*===============================*/

      theConstruct = NULL;
      while ((theConstruct = (*constructClass->getNextItemFunction)(theEnv,theConstruct)) != NULL)
        {
         theName = (*constructClass->getConstructNameFunction)((struct constructHeader *) theConstruct);
         SetMFType(theList,count,SYMBOL);
         if (allModules)
           {
            genstrcpy(buffer,EnvGetDefmoduleName(theEnv,loopModule));
            genstrcat(buffer,"::");
            genstrcat(buffer,ValueToString(theName));
            SetMFValue(theList,count,EnvAddSymbol(theEnv,buffer));
           }
         else
           { SetMFValue(theList,count,EnvAddSymbol(theEnv,ValueToString(theName))); }
         count++;
        }

      /*==================================*/
      /* Move on to the next module (if   */
      /* the list is to contain the names */
      /* of constructs from all modules). */
      /*==================================*/

      if (allModules) loopModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,loopModule);
      else loopModule = NULL;
     }

   /*=========================*/
   /* Return the name buffer. */
   /*=========================*/
   
   genfree(theEnv,buffer,bufferSize);
   
   /*=============================*/
   /* Restore the current module. */
   /*=============================*/

   RestoreCurrentModule(theEnv);
  }

/*********************************************/
/* ListConstructCommand: Generic Routine for */
/*   listing the constructs in a module.     */
/*********************************************/
globle void ListConstructCommand(
  void *theEnv,
  char *functionName,
  struct construct *constructClass)
  {
   struct defmodule *theModule;
   DATA_OBJECT result;
   int numArgs;

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if ((numArgs = EnvArgCountCheck(theEnv,functionName,NO_MORE_THAN,1)) == -1) return;

   /*====================================*/
   /* If an argument was given, check to */
   /* see that it's a valid module name. */
   /*====================================*/

   if (numArgs == 1)
     {
      /*======================================*/
      /* Only symbols are valid module names. */
      /*======================================*/

      EnvRtnUnknown(theEnv,1,&result);
      if (GetType(result) != SYMBOL)
        {
         ExpectedTypeError1(theEnv,functionName,1,"defmodule name");
         return;
        }

      /*===========================================*/
      /* Verify that the named module exists or is */
      /* the symbol * (for obtaining the construct */
      /* list for all modules).                    */
      /*===========================================*/

      if ((theModule = (struct defmodule *) EnvFindDefmodule(theEnv,DOToString(result))) == NULL)
        {
         if (strcmp("*",DOToString(result)) != 0)
           {
            ExpectedTypeError1(theEnv,functionName,1,"defmodule name");
            return;
           }

         theModule = NULL;
        }
     }

   /*=====================================*/
   /* Otherwise use the current module to */
   /* generate the construct list.        */
   /*=====================================*/

   else
     { theModule = ((struct defmodule *) EnvGetCurrentModule(theEnv)); }

   /*=========================*/
   /* Call the driver routine */
   /* to list the constructs. */
   /*=========================*/

   ListConstruct(theEnv,constructClass,WDISPLAY,theModule);
  }

/*****************************************/
/* ListConstruct: Generic C Routine for  */
/*   listing the constructs in a module. */
/*****************************************/
globle void ListConstruct(
  void *theEnv,
  struct construct *constructClass,
  char *logicalName,
  struct defmodule *theModule)
  {
   void *constructPtr;
   SYMBOL_HN *constructName;
   long count = 0;
   int allModules = FALSE;

   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*=======================================*/
   /* If the module specified is NULL, then */
   /* list all constructs in all modules.   */
   /*=======================================*/

   if (theModule == NULL)
     {
      theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
      allModules = TRUE;
     }

   /*==================================*/
   /* Loop through all of the modules. */
   /*==================================*/

   while (theModule != NULL)
     {
      /*========================================*/
      /* If we're printing the construct in all */
      /* modules, then preface each module      */
      /* listing with the name of the module.   */
      /*========================================*/

      if (allModules)
        {
         EnvPrintRouter(theEnv,logicalName,EnvGetDefmoduleName(theEnv,theModule));
         EnvPrintRouter(theEnv,logicalName,":\n");
        }

      /*===============================*/
      /* Set the current module to the */
      /* module we're examining.       */
      /*===============================*/

      EnvSetCurrentModule(theEnv,(void *) theModule);

      /*===========================================*/
      /* List all of the constructs in the module. */
      /*===========================================*/

      for (constructPtr = (*constructClass->getNextItemFunction)(theEnv,NULL);
           constructPtr != NULL;
           constructPtr = (*constructClass->getNextItemFunction)(theEnv,constructPtr))
        {
         if (EvaluationData(theEnv)->HaltExecution == TRUE) return;

         constructName = (*constructClass->getConstructNameFunction)((struct constructHeader *) constructPtr);

         if (constructName != NULL)
           {
            if (allModules) EnvPrintRouter(theEnv,WDISPLAY,"   ");
            EnvPrintRouter(theEnv,logicalName,ValueToString(constructName));
            EnvPrintRouter(theEnv,logicalName,"\n");
           }

         count++;
        }

      /*====================================*/
      /* Move on to the next module (if the */
      /* listing is to contain the names of */
      /* constructs from all modules).      */
      /*====================================*/

      if (allModules) theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule);
      else theModule = NULL;
     }

   /*=================================================*/
   /* Print the tally and restore the current module. */
   /*=================================================*/

   PrintTally(theEnv,WDISPLAY,count,constructClass->constructName,
                             constructClass->pluralName);

   RestoreCurrentModule(theEnv);
  }

/**********************************************************/
/* SetNextConstruct: Sets the next field of one construct */
/*   to point to another construct of the same type.      */
/**********************************************************/
globle void SetNextConstruct(
  struct constructHeader *theConstruct,
  struct constructHeader *targetConstruct)
  { theConstruct->next = targetConstruct; }

/********************************************************************/
/* GetConstructModuleItem: Returns the construct module for a given */
/*   construct (note that this is a pointer to a data structure     */
/*   like the deffactsModule, not a pointer to an environment       */
/*   module which contains a number of types of constructs.         */
/********************************************************************/
globle struct defmoduleItemHeader *GetConstructModuleItem(
  struct constructHeader *theConstruct)
  { return(theConstruct->whichModule); }

/*************************************************/
/* GetConstructPPForm: Returns the pretty print  */
/*   representation for the specified construct. */
/*************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle char *GetConstructPPForm(
  void *theEnv,
  struct constructHeader *theConstruct)
  { 
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   return(theConstruct->ppForm); 
  }

/****************************************************/
/* GetNextConstructItem: Returns the next construct */
/*   items from a list of constructs.               */
/****************************************************/
globle struct constructHeader *GetNextConstructItem(
  void *theEnv,
  struct constructHeader *theConstruct,
  int moduleIndex)
  {
   struct defmoduleItemHeader *theModuleItem;

   if (theConstruct == NULL)
     {
      theModuleItem = (struct defmoduleItemHeader *)
                      GetModuleItem(theEnv,NULL,moduleIndex);
      if (theModuleItem == NULL) return(NULL);
      return(theModuleItem->firstItem);
     }

   return(theConstruct->next);
  }

/*******************************************************/
/* GetConstructModuleItemByIndex: Returns a pointer to */
/*  the defmodule item for the specified construct. If */
/*  theModule is NULL, then the construct module item  */
/*  for the current module is returned, otherwise the  */
/*  construct module item for the specified construct  */
/*  is returned.                                       */
/*******************************************************/
globle struct defmoduleItemHeader *GetConstructModuleItemByIndex(
  void *theEnv,
  struct defmodule *theModule,
  int moduleIndex)
  {
   if (theModule != NULL)
     {
      return((struct defmoduleItemHeader *)
             GetModuleItem(theEnv,theModule,moduleIndex));
     }

   return((struct defmoduleItemHeader *)
          GetModuleItem(theEnv,((struct defmodule *) EnvGetCurrentModule(theEnv)),moduleIndex));
  }

/******************************************/
/* FreeConstructHeaderModule: Deallocates */
/*   the data structures associated with  */
/*   the construct module item header.    */
/******************************************/
globle void FreeConstructHeaderModule(
  void *theEnv,
  struct defmoduleItemHeader *theModuleItem,
  struct construct *constructClass)
  {
   struct constructHeader *thisOne, *nextOne;

   thisOne = theModuleItem->firstItem;

   while (thisOne != NULL)
     {
      nextOne = thisOne->next;
      (*constructClass->freeFunction)(theEnv,thisOne);
      thisOne = nextOne;
     }
  }

/**********************************************/
/* DoForAllConstructs: Executes an action for */
/*   all constructs of a specified type.      */
/**********************************************/
globle long DoForAllConstructs(
  void *theEnv,
  void (*actionFunction)(void *,struct constructHeader *,void *),
  int moduleItemIndex,
  int interruptable,
  void *userBuffer)
  {
   struct constructHeader *theConstruct;
   struct defmoduleItemHeader *theModuleItem;
   void *theModule;
   long moduleCount = 0L;
   
   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*==================================*/
   /* Loop through all of the modules. */
   /*==================================*/

   for (theModule = EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = EnvGetNextDefmodule(theEnv,theModule), moduleCount++)
     {
      /*=============================*/
      /* Set the current module to   */
      /* the module we're examining. */
      /*=============================*/

      EnvSetCurrentModule(theEnv,(void *) theModule);

      /*================================================*/
      /* Perform the action for each of the constructs. */
      /*================================================*/

      theModuleItem = (struct defmoduleItemHeader *)
                      GetModuleItem(theEnv,(struct defmodule *) theModule,moduleItemIndex);

      for (theConstruct = theModuleItem->firstItem;
           theConstruct != NULL;
           theConstruct = theConstruct->next)
        {
         if (interruptable)
           {
            if (GetHaltExecution(theEnv) == TRUE)
              {
               RestoreCurrentModule(theEnv);
               return(-1L);
              }
           }

         (*actionFunction)(theEnv,theConstruct,userBuffer);
        }
     }

   /*=============================*/
   /* Restore the current module. */
   /*=============================*/

   RestoreCurrentModule(theEnv);

   /*=========================================*/
   /* Return the number of modules traversed. */
   /*=========================================*/

   return(moduleCount);
  }

/******************************************************/
/* DoForAllConstructsInModule: Executes an action for */
/*   all constructs of a specified type in a module.  */
/******************************************************/
globle void DoForAllConstructsInModule(
  void *theEnv,
  void *theModule,
  void (*actionFunction)(void *,struct constructHeader *,void *),
  int moduleItemIndex,
  int interruptable,
  void *userBuffer)
  {
   struct constructHeader *theConstruct;
   struct defmoduleItemHeader *theModuleItem;
   
   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*=============================*/
   /* Set the current module to   */
   /* the module we're examining. */
   /*=============================*/

   EnvSetCurrentModule(theEnv,(void *) theModule);

   /*================================================*/
   /* Perform the action for each of the constructs. */
   /*================================================*/

   theModuleItem = (struct defmoduleItemHeader *)
                   GetModuleItem(theEnv,(struct defmodule *) theModule,moduleItemIndex);

   for (theConstruct = theModuleItem->firstItem;
        theConstruct != NULL;
        theConstruct = theConstruct->next)
     {
      if (interruptable)
        {
         if (GetHaltExecution(theEnv) == TRUE)
           {
            RestoreCurrentModule(theEnv);
            return;
           }
        }

      (*actionFunction)(theEnv,theConstruct,userBuffer);
     }

   /*=============================*/
   /* Restore the current module. */
   /*=============================*/

   RestoreCurrentModule(theEnv);
  }

/*****************************************************/
/* InitializeConstructHeader: Initializes construct  */
/*   header info, including to which module item the */
/*   new construct belongs                           */
/*****************************************************/
globle void InitializeConstructHeader(
  void *theEnv,
  char *constructType,
  struct constructHeader *theConstruct,
  SYMBOL_HN *theConstructName)
  {
   struct moduleItem *theModuleItem;
   struct defmoduleItemHeader *theItemHeader;

   theModuleItem = FindModuleItem(theEnv,constructType);
   theItemHeader = (struct defmoduleItemHeader *)
                   GetModuleItem(theEnv,NULL,theModuleItem->moduleIndex);

   theConstruct->whichModule = theItemHeader;
   theConstruct->name = theConstructName;
   theConstruct->ppForm = NULL;
   theConstruct->bsaveID = 0L;
   theConstruct->next = NULL;
   theConstruct->usrData = NULL;
  }

/*************************************************/
/* SetConstructPPForm: Sets a construct's pretty */
/*   print form and deletes the old one.         */
/*************************************************/
globle void SetConstructPPForm(
  void *theEnv,
  struct constructHeader *theConstruct,
  char *ppForm)
  {
   if (theConstruct->ppForm != NULL)
     {
      rm(theEnv,(void *) theConstruct->ppForm,
         ((strlen(theConstruct->ppForm) + 1) * sizeof(char)));
     }
   theConstruct->ppForm = ppForm;
  }

#if DEBUGGING_FUNCTIONS

/******************************************************/
/* ConstructPrintWatchAccess: Provides an interface   */
/*   to the list-watch-items function for a construct */
/******************************************************/
globle unsigned ConstructPrintWatchAccess(
  void *theEnv,
  struct construct *constructClass,
  char *logName,
  EXPRESSION *argExprs,
  unsigned (*getWatchFunc)(void *,void *),
  void (*setWatchFunc)(void *,unsigned,void *))
  {
   return(ConstructWatchSupport(theEnv,constructClass,"list-watch-items",logName,argExprs,
                                FALSE,FALSE,getWatchFunc,setWatchFunc));
  }

/**************************************************/
/* ConstructSetWatchAccess: Provides an interface */
/*   to the watch function for a construct        */
/**************************************************/
globle unsigned ConstructSetWatchAccess(
  void *theEnv,
  struct construct *constructClass,
  unsigned newState,
  EXPRESSION *argExprs,
  unsigned (*getWatchFunc)(void *,void *),
  void (*setWatchFunc)(void *,unsigned,void *))
  {
   return(ConstructWatchSupport(theEnv,constructClass,"watch",WERROR,argExprs,
                                TRUE,newState,getWatchFunc,setWatchFunc));
  }

/******************************************************/
/* ConstructWatchSupport: Generic construct interface */
/*   into watch and list-watch-items.                 */
/******************************************************/
static unsigned ConstructWatchSupport(
  void *theEnv,
  struct construct *constructClass,
  char *funcName,
  char *logName,
  EXPRESSION *argExprs,
  intBool setFlag,
  unsigned newState,
  unsigned (*getWatchFunc)(void *,void *),
  void (*setWatchFunc)(void *,unsigned,void *))
  {
   struct defmodule *theModule;
   void *theConstruct;
   DATA_OBJECT constructName;
   int argIndex = 2;

   /*========================================*/
   /* If no constructs are specified, then   */
   /* show/set the trace for all constructs. */
   /*========================================*/

   if (argExprs == NULL)
     {
      /*==========================*/
      /* Save the current module. */
      /*==========================*/

      SaveCurrentModule(theEnv);

      /*===========================*/
      /* Loop through each module. */
      /*===========================*/

      for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
           theModule != NULL;
           theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,(void *) theModule))
        {
         /*============================*/
         /* Set the current module to  */
         /* the module being examined. */
         /*============================*/

         EnvSetCurrentModule(theEnv,(void *) theModule);

         /*====================================================*/
         /* If we're displaying the names of constructs with   */
         /* watch flags enabled, then preface each module      */
         /* listing of constructs with the name of the module. */
         /*====================================================*/

         if (setFlag == FALSE)
           {
            EnvPrintRouter(theEnv,logName,EnvGetDefmoduleName(theEnv,(void *) theModule));
            EnvPrintRouter(theEnv,logName,":\n");
           }

         /*============================================*/
         /* Loop through each construct in the module. */
         /*============================================*/

         for (theConstruct = (*constructClass->getNextItemFunction)(theEnv,NULL);
              theConstruct != NULL;
              theConstruct = (*constructClass->getNextItemFunction)(theEnv,theConstruct))
           {
            /*=============================================*/
            /* Either set the watch flag for the construct */
            /* or display its current state.               */
            /*=============================================*/

            if (setFlag)
              { (*setWatchFunc)(theEnv,newState,theConstruct); }
            else
              {
               EnvPrintRouter(theEnv,logName,"   ");
               ConstructPrintWatch(theEnv,logName,constructClass,theConstruct,getWatchFunc);
              }
           }
        }

      /*=============================*/
      /* Restore the current module. */
      /*=============================*/

      RestoreCurrentModule(theEnv);

      /*====================================*/
      /* Return TRUE to indicate successful */
      /* completion of the command.         */
      /*====================================*/

      return(TRUE);
     }

   /*==================================================*/
   /* Show/set the trace for each specified construct. */
   /*==================================================*/

   while (argExprs != NULL)
     {
      /*==========================================*/
      /* Evaluate the argument that should be a   */
      /* construct name. Return FALSE is an error */
      /* occurs when evaluating the argument.     */
      /*==========================================*/

      if (EvaluateExpression(theEnv,argExprs,&constructName))
        { return(FALSE); }

      /*================================================*/
      /* Check to see that it's a valid construct name. */
      /*================================================*/

      if ((constructName.type != SYMBOL) ? TRUE :
          ((theConstruct = LookupConstruct(theEnv,constructClass,
                                           DOToString(constructName),TRUE)) == NULL))
        {
         ExpectedTypeError1(theEnv,funcName,argIndex,constructClass->constructName);
         return(FALSE);
        }

      /*=============================================*/
      /* Either set the watch flag for the construct */
      /* or display its current state.               */
      /*=============================================*/

      if (setFlag)
        { (*setWatchFunc)(theEnv,newState,theConstruct); }
      else
        { ConstructPrintWatch(theEnv,logName,constructClass,theConstruct,getWatchFunc); }

      /*===============================*/
      /* Move on to the next argument. */
      /*===============================*/

      argIndex++;
      argExprs = GetNextArgument(argExprs);
     }

   /*====================================*/
   /* Return TRUE to indicate successful */
   /* completion of the command.         */
   /*====================================*/

   return(TRUE);
  }

/*************************************************/
/* ConstructPrintWatch: Displays the trace value */
/*   of a construct for list-watch-items         */
/*************************************************/
static void ConstructPrintWatch(
  void *theEnv,
  char *logName,
  struct construct *constructClass,
  void *theConstruct,
  unsigned (*getWatchFunc)(void *,void *))
  {
   EnvPrintRouter(theEnv,logName,ValueToString((*constructClass->getConstructNameFunction)((struct constructHeader *) theConstruct)));
   if ((*getWatchFunc)(theEnv,theConstruct))
     EnvPrintRouter(theEnv,logName," = on\n");
   else
     EnvPrintRouter(theEnv,logName," = off\n");
  }

#endif /* DEBUGGING_FUNCTIONS */

/*****************************************************/
/* LookupConstruct: Finds a construct in the current */
/*   or imported modules. If specified, will also    */
/*   look for construct in a non-imported module.    */
/*****************************************************/
globle void *LookupConstruct(
  void *theEnv,
  struct construct *constructClass,
  char *constructName,
  intBool moduleNameAllowed)
  {
   void *theConstruct;
   char *constructType;
   int moduleCount;

   /*============================================*/
   /* Look for the specified construct in the    */
   /* current module or in any imported modules. */
   /*============================================*/

   constructType = constructClass->constructName;
   theConstruct = FindImportedConstruct(theEnv,constructType,NULL,constructName,
                                        &moduleCount,TRUE,NULL);

   /*===========================================*/
   /* Return NULL if the reference is ambiguous */
   /* (it was found in more than one module).   */
   /*===========================================*/

   if (theConstruct != NULL)
     {
      if (moduleCount > 1)
        {
         AmbiguousReferenceErrorMessage(theEnv,constructType,constructName);
         return(NULL);
        }
      return(theConstruct);
     }

   /*=============================================*/
   /* If specified, check to see if the construct */
   /* is in a non-imported module.                */
   /*=============================================*/

   if (moduleNameAllowed && FindModuleSeparator(constructName))
     { theConstruct = (*constructClass->findFunction)(theEnv,constructName); }

   /*====================================*/
   /* Return a pointer to the construct. */
   /*====================================*/

   return(theConstruct);
  }

/***********************************************************/
/* ConstructsDeletable: Returns a boolean value indicating */
/*   whether constructs in general can be deleted.         */
/***********************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle intBool ConstructsDeletable(
  void *theEnv)
  {
#if BLOAD_ONLY || RUN_TIME || ((! BLOAD) && (! BLOAD_AND_BSAVE))
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
#endif

#if BLOAD_ONLY || RUN_TIME
   return(FALSE);
#elif BLOAD || BLOAD_AND_BSAVE
   if (Bloaded(theEnv))
     return(FALSE);
   return TRUE;
#else
   return(TRUE);
#endif
  }
