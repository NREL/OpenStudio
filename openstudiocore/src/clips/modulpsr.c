   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*              DEFMODULE PARSER MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Parses a defmodule construct.                    */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#define _MODULPSR_SOURCE_

#include "setup.h"

#if DEFMODULE_CONSTRUCT && (! RUN_TIME) && (! BLOAD_ONLY)

#include <stdio.h>
#include <string.h>
#define _STDIO_INCLUDED_

#include "memalloc.h"
#include "constant.h"
#include "router.h"
#include "extnfunc.h"
#include "argacces.h"
#include "cstrcpsr.h"
#include "constrct.h"
#include "modulutl.h"
#include "utility.h"
#include "envrnmnt.h"

#if BLOAD || BLOAD_AND_BSAVE
#include "bload.h"
#endif

#include "modulpsr.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static int                        ParsePortSpecifications(void *,
                                                             char *,struct token *,
                                                             struct defmodule *);
   static int                        ParseImportSpec(void *,char *,struct token *,
                                                     struct defmodule *);
   static int                        ParseExportSpec(void *,char *,struct token *,
                                                     struct defmodule *,
                                                     struct defmodule *);
   static intBool                    DeleteDefmodule(void *,void *);
   static int                        FindMultiImportConflict(void *,struct defmodule *);
   static void                       NotExportedErrorMessage(void *,char *,char *,char *);

/*********************************************/
/* GetNumberOfDefmodules: Returns the number */
/*   of defmodules currently defined.        */
/*********************************************/
globle long GetNumberOfDefmodules(
  void *theEnv)
  {
   return(DefmoduleData(theEnv)->NumberOfDefmodules);
  }

/******************************************/
/* SetNumberOfDefmodules: Sets the number */
/*   of defmodules currently defined.     */
/******************************************/
globle void SetNumberOfDefmodules(
  void *theEnv,
  long value)
  {
   DefmoduleData(theEnv)->NumberOfDefmodules = value;
  }

/****************************************************/
/* AddAfterModuleChangeFunction: Adds a function to */
/*   the list of functions that are to be called    */
/*   after a module change occurs.                  */
/****************************************************/
globle void AddAfterModuleDefinedFunction(
  void *theEnv,
  char *name,
  void (*func)(void *),
  int priority)
  {
   DefmoduleData(theEnv)->AfterModuleDefinedFunctions =
     AddFunctionToCallList(theEnv,name,priority,func,DefmoduleData(theEnv)->AfterModuleDefinedFunctions,TRUE);
  }

/******************************************************/
/* AddPortConstructItem: Adds an item to the list of  */
/*   items that can be imported/exported by a module. */
/******************************************************/
globle void AddPortConstructItem(
  void *theEnv,
  char *theName,
  int theType)
  {
   struct portConstructItem *newItem;

   newItem = get_struct(theEnv,portConstructItem);
   newItem->constructName = theName;
   newItem->typeExpected = theType;
   newItem->next = DefmoduleData(theEnv)->ListOfPortConstructItems;
   DefmoduleData(theEnv)->ListOfPortConstructItems = newItem;
  }

/******************************************************/
/* ParseDefmodule: Coordinates all actions necessary  */
/*   for the parsing and creation of a defmodule into */
/*   the current environment.                         */
/******************************************************/
globle int ParseDefmodule(
  void *theEnv,
  char *readSource)
  {
   SYMBOL_HN *defmoduleName;
   struct defmodule *newDefmodule;
   struct token inputToken;
   int i;
   struct moduleItem *theItem;
   struct portItem *portSpecs, *nextSpec;
   struct defmoduleItemHeader *theHeader;
   struct callFunctionItem *defineFunctions;
   struct defmodule *redefiningMainModule = NULL;
   int parseError;
   struct portItem *oldImportList = NULL, *oldExportList = NULL;
   short overwrite = FALSE;

   /*================================================*/
   /* Flush the buffer which stores the pretty print */
   /* representation for a module.  Add the already  */
   /* parsed keyword defmodule to this buffer.       */
   /*================================================*/

   SetPPBufferStatus(theEnv,ON);
   FlushPPBuffer(theEnv);
   SetIndentDepth(theEnv,3);
   SavePPBuffer(theEnv,"(defmodule ");

   /*===============================*/
   /* Modules cannot be loaded when */
   /* a binary load is in effect.   */
   /*===============================*/

#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
   if ((Bloaded(theEnv) == TRUE) && (! ConstructData(theEnv)->CheckSyntaxMode))
     {
      CannotLoadWithBloadMessage(theEnv,"defmodule");
      return(TRUE);
     }
#endif

   /*=====================================================*/
   /* Parse the name and comment fields of the defmodule. */
   /* Remove the defmodule if it already exists.          */
   /*=====================================================*/

   defmoduleName = GetConstructNameAndComment(theEnv,readSource,&inputToken,"defmodule",
                                              EnvFindDefmodule,DeleteDefmodule,"+",
                                              TRUE,TRUE,FALSE);
   if (defmoduleName == NULL) { return(TRUE); }

   if (strcmp(ValueToString(defmoduleName),"MAIN") == 0)
     { redefiningMainModule = (struct defmodule *) EnvFindDefmodule(theEnv,"MAIN"); }

   /*==============================================*/
   /* Create the defmodule structure if necessary. */
   /*==============================================*/

   if (redefiningMainModule == NULL)
     {
      newDefmodule = (struct defmodule *) EnvFindDefmodule(theEnv,ValueToString(defmoduleName));
      if (newDefmodule)
        { overwrite = TRUE; }
      else
        {
         newDefmodule = get_struct(theEnv,defmodule);
         newDefmodule->name = defmoduleName;
         newDefmodule->usrData = NULL;
         newDefmodule->next = NULL;
        }
     }
   else
     {
      overwrite = TRUE;
      newDefmodule = redefiningMainModule;
     }

   if (overwrite)
     {
      oldImportList = newDefmodule->importList;
      oldExportList = newDefmodule->exportList;
     }

   newDefmodule->importList = NULL;
   newDefmodule->exportList = NULL;

   /*===================================*/
   /* Finish parsing the defmodule (its */
   /* import/export specifications).    */
   /*===================================*/

   parseError = ParsePortSpecifications(theEnv,readSource,&inputToken,newDefmodule);

   /*====================================*/
   /* Check for import/export conflicts. */
   /*====================================*/

   if (! parseError) parseError = FindMultiImportConflict(theEnv,newDefmodule);

   /*======================================================*/
   /* If an error occured in parsing or an import conflict */
   /* was detected, abort the definition of the defmodule. */
   /* If we're only checking syntax, then we want to exit  */
   /* at this point as well.                               */
   /*======================================================*/

   if (parseError || ConstructData(theEnv)->CheckSyntaxMode)
     {
      while (newDefmodule->importList != NULL)
        {
         nextSpec = newDefmodule->importList->next;
         rtn_struct(theEnv,portItem,newDefmodule->importList);
         newDefmodule->importList = nextSpec;
        }

      while (newDefmodule->exportList != NULL)
        {
         nextSpec = newDefmodule->exportList->next;
         rtn_struct(theEnv,portItem,newDefmodule->exportList);
         newDefmodule->exportList = nextSpec;
        }

      if ((redefiningMainModule == NULL) && (! overwrite))
        { rtn_struct(theEnv,defmodule,newDefmodule); }

      if (overwrite)
        {
         newDefmodule->importList = oldImportList;
         newDefmodule->exportList = oldExportList;
        }

      if (parseError) return(TRUE);
      return(FALSE);
     }

   /*===============================================*/
   /* Increment the symbol table counts for symbols */
   /* used in the defmodule data structures.        */
   /*===============================================*/

   if (redefiningMainModule == NULL)
     { IncrementSymbolCount(newDefmodule->name); }
   else
     {
      if ((newDefmodule->importList != NULL) ||
          (newDefmodule->exportList != NULL))
        { DefmoduleData(theEnv)->MainModuleRedefinable = FALSE; }
     }

   for (portSpecs = newDefmodule->importList; portSpecs != NULL; portSpecs = portSpecs->next)
     {
      if (portSpecs->moduleName != NULL) IncrementSymbolCount(portSpecs->moduleName);
      if (portSpecs->constructType != NULL) IncrementSymbolCount(portSpecs->constructType);
      if (portSpecs->constructName != NULL) IncrementSymbolCount(portSpecs->constructName);
     }

   for (portSpecs = newDefmodule->exportList; portSpecs != NULL; portSpecs = portSpecs->next)
     {
      if (portSpecs->moduleName != NULL) IncrementSymbolCount(portSpecs->moduleName);
      if (portSpecs->constructType != NULL) IncrementSymbolCount(portSpecs->constructType);
      if (portSpecs->constructName != NULL) IncrementSymbolCount(portSpecs->constructName);
     }

   /*====================================================*/
   /* Allocate storage for the module's construct lists. */
   /*====================================================*/

   if (redefiningMainModule != NULL) { /* Do nothing */ }
   else if (DefmoduleData(theEnv)->NumberOfModuleItems == 0) newDefmodule->itemsArray = NULL;
   else
     {
      newDefmodule->itemsArray = (struct defmoduleItemHeader **) gm2(theEnv,sizeof(void *) * DefmoduleData(theEnv)->NumberOfModuleItems);
      for (i = 0, theItem = DefmoduleData(theEnv)->ListOfModuleItems;
           (i < DefmoduleData(theEnv)->NumberOfModuleItems) && (theItem != NULL);
           i++, theItem = theItem->next)
        {
         if (theItem->allocateFunction == NULL)
           { newDefmodule->itemsArray[i] = NULL; }
         else
           {
            newDefmodule->itemsArray[i] = (struct defmoduleItemHeader *)
                                          (*theItem->allocateFunction)(theEnv);
            theHeader = (struct defmoduleItemHeader *) newDefmodule->itemsArray[i];
            theHeader->theModule = newDefmodule;
            theHeader->firstItem = NULL;
            theHeader->lastItem = NULL;
           }
        }
     }

   /*=======================================*/
   /* Save the pretty print representation. */
   /*=======================================*/

   SavePPBuffer(theEnv,"\n");

   if (EnvGetConserveMemory(theEnv) == TRUE)
     { newDefmodule->ppForm = NULL; }
   else
     { newDefmodule->ppForm = CopyPPBuffer(theEnv); }

   /*==============================================*/
   /* Add the defmodule to the list of defmodules. */
   /*==============================================*/

   if (redefiningMainModule == NULL)
     {
      if (DefmoduleData(theEnv)->LastDefmodule == NULL) DefmoduleData(theEnv)->ListOfDefmodules = newDefmodule;
      else DefmoduleData(theEnv)->LastDefmodule->next = newDefmodule;
      DefmoduleData(theEnv)->LastDefmodule = newDefmodule;
      newDefmodule->bsaveID = DefmoduleData(theEnv)->NumberOfDefmodules++;
     }

   EnvSetCurrentModule(theEnv,(void *) newDefmodule);

   /*=========================================*/
   /* Call any functions required by other    */
   /* constructs when a new module is defined */
   /*=========================================*/

   for (defineFunctions = DefmoduleData(theEnv)->AfterModuleDefinedFunctions;
        defineFunctions != NULL;
        defineFunctions = defineFunctions->next)
     { (* (void (*)(void *)) defineFunctions->func)(theEnv); }

   /*===============================================*/
   /* Defmodule successfully parsed with no errors. */
   /*===============================================*/

   return(FALSE);
  }

/*************************************************************/
/* DeleteDefmodule: Used by the parsing routine to determine */
/*   if a module can be redefined. Only the MAIN module can  */
/*   be redefined (and it can only be redefined once).       */
/*************************************************************/
static intBool DeleteDefmodule(
  void *theEnv,
  void *theConstruct)
  {
   if (strcmp(EnvGetDefmoduleName(theEnv,theConstruct),"MAIN") == 0)
     { return(DefmoduleData(theEnv)->MainModuleRedefinable); }

   return(FALSE);
  }

/*********************************************************/
/* ParsePortSpecifications: Parses the import and export */
/*   specifications found in a defmodule construct.      */
/*********************************************************/
static int ParsePortSpecifications(
  void *theEnv,
  char *readSource,
  struct token *theToken,
  struct defmodule *theDefmodule)
  {
   int error;

   /*=============================*/
   /* The import and export lists */
   /* are initially empty.        */
   /*=============================*/

   theDefmodule->importList = NULL;
   theDefmodule->exportList = NULL;

   /*==========================================*/
   /* Parse import/export specifications until */
   /* a right parenthesis is encountered.      */
   /*==========================================*/

   while (theToken->type != RPAREN)
     {
      /*========================================*/
      /* Look for the opening left parenthesis. */
      /*========================================*/

      if (theToken->type != LPAREN)
        {
         SyntaxErrorMessage(theEnv,"defmodule");
         return(TRUE);
        }

      /*====================================*/
      /* Look for the import/export keyword */
      /* and call the appropriate functions */
      /* for parsing the specification.     */
      /*====================================*/

      GetToken(theEnv,readSource,theToken);

      if (theToken->type != SYMBOL)
        {
         SyntaxErrorMessage(theEnv,"defmodule");
         return(TRUE);
        }

      if (strcmp(ValueToString(theToken->value),"import") == 0)
        {
         error = ParseImportSpec(theEnv,readSource,theToken,theDefmodule);
        }
      else if (strcmp(ValueToString(theToken->value),"export") == 0)
        {
         error = ParseExportSpec(theEnv,readSource,theToken,theDefmodule,NULL);
        }
      else
        {
         SyntaxErrorMessage(theEnv,"defmodule");
         return(TRUE);
        }

      if (error) return(TRUE);

      /*============================================*/
      /* Begin parsing the next port specification. */
      /*============================================*/

      PPCRAndIndent(theEnv);
      GetToken(theEnv,readSource,theToken);

      if (theToken->type == RPAREN)
        {
         PPBackup(theEnv);
         PPBackup(theEnv);
         SavePPBuffer(theEnv,")");
        }
     }

   /*===================================*/
   /* Return FALSE to indicate no error */
   /* occurred while parsing the        */
   /* import/export specifications.     */
   /*===================================*/

   return(FALSE);
  }

/**********************************************************/
/* ParseImportSpec: Parses import specifications found in */
/*   a defmodule construct.                               */
/*                                                        */
/* <import-spec> ::= (import <module-name> <port-item>)   */
/*                                                        */
/* <port-item>   ::= ?ALL |                               */
/*                   ?NONE |                              */
/*                   <construct-name> ?ALL |              */
/*                   <construct-name> ?NONE |             */
/*                   <construct-name> <names>*            */
/**********************************************************/
static int ParseImportSpec(
  void *theEnv,
  char *readSource,
  struct token *theToken,
  struct defmodule *newModule)
  {
   struct defmodule *theModule;
   struct portItem *thePort, *oldImportSpec;
   int found, count;

   /*===========================*/
   /* Look for the module name. */
   /*===========================*/

   SavePPBuffer(theEnv," ");

   GetToken(theEnv,readSource,theToken);

   if (theToken->type != SYMBOL)
     {
      SyntaxErrorMessage(theEnv,"defmodule import specification");
      return(TRUE);
     }

   /*=====================================*/
   /* Verify the existence of the module. */
   /*=====================================*/

   if ((theModule = (struct defmodule *)
                    EnvFindDefmodule(theEnv,ValueToString(theToken->value))) == NULL)
     {
      CantFindItemErrorMessage(theEnv,"defmodule",ValueToString(theToken->value));
      return(TRUE);
     }

   /*========================================*/
   /* If the specified module doesn't export */
   /* any constructs, then the import        */
   /* specification is meaningless.          */
   /*========================================*/

   if (theModule->exportList == NULL)
     {
      NotExportedErrorMessage(theEnv,EnvGetDefmoduleName(theEnv,theModule),NULL,NULL);
      return(TRUE);
     }

   /*==============================================*/
   /* Parse the remaining portion of the import    */
   /* specification and return if an error occurs. */
   /*==============================================*/

   oldImportSpec = newModule->importList;
   if (ParseExportSpec(theEnv,readSource,theToken,newModule,theModule)) return(TRUE);

   /*========================================================*/
   /* If the ?NONE keyword was used with the import spec,    */
   /* then no constructs were actually imported and the      */
   /* import spec does not need to be checked for conflicts. */
   /*========================================================*/

   if (newModule->importList == oldImportSpec) return(FALSE);

   /*======================================================*/
   /* Check to see if the construct being imported can be  */
   /* by the specified module. This check exported doesn't */
   /* guarantee that a specific named construct actually   */
   /* exists. It just checks that it could be exported if  */
   /* it does exists.                                      */
   /*======================================================*/

   if (newModule->importList->constructType != NULL)
     {
      /*=============================*/
      /* Look for the construct in   */
      /* the module that exports it. */
      /*=============================*/

      found = FALSE;
      for (thePort = theModule->exportList;
           (thePort != NULL) && (! found);
           thePort = thePort->next)
        {
         if (thePort->constructType == NULL) found = TRUE;
         else if (thePort->constructType == newModule->importList->constructType)
           {
            if (newModule->importList->constructName == NULL) found = TRUE;
            else if (thePort->constructName == NULL) found = TRUE;
            else if (thePort->constructName == newModule->importList->constructName)
              { found = TRUE; }
           }
        }

      /*=======================================*/
      /* If it's not exported by the specified */
      /* module, print an error message.       */
      /*=======================================*/

      if (! found)
        {
         if (newModule->importList->constructName == NULL)
           {
            NotExportedErrorMessage(theEnv,EnvGetDefmoduleName(theEnv,theModule),
                                    ValueToString(newModule->importList->constructType),
                                    NULL);
           }
         else
           {
            NotExportedErrorMessage(theEnv,EnvGetDefmoduleName(theEnv,theModule),
                                    ValueToString(newModule->importList->constructType),
                                    ValueToString(newModule->importList->constructName));
           }
         return(TRUE);
        }
     }

   /*======================================================*/
   /* Verify that specific named constructs actually exist */
   /* and can be seen from the module importing them.      */
   /*======================================================*/

   SaveCurrentModule(theEnv);
   EnvSetCurrentModule(theEnv,(void *) newModule);

   for (thePort = newModule->importList;
        thePort != NULL;
        thePort = thePort->next)
     {
      if ((thePort->constructType == NULL) || (thePort->constructName == NULL))
        { continue; }

      theModule = (struct defmodule *)
                  EnvFindDefmodule(theEnv,ValueToString(thePort->moduleName));
      EnvSetCurrentModule(theEnv,theModule);
      if (FindImportedConstruct(theEnv,ValueToString(thePort->constructType),NULL,
                                ValueToString(thePort->constructName),&count,
                                TRUE,FALSE) == NULL)
        {
         NotExportedErrorMessage(theEnv,EnvGetDefmoduleName(theEnv,theModule),
                                 ValueToString(thePort->constructType),
                                 ValueToString(thePort->constructName));
         RestoreCurrentModule(theEnv);
         return(TRUE);
        }
     }

   RestoreCurrentModule(theEnv);

   /*===============================================*/
   /* The import list has been successfully parsed. */
   /*===============================================*/

   return(FALSE);
  }

/**********************************************************/
/* ParseExportSpec: Parses export specifications found in */
/*   a defmodule construct. This includes parsing the     */
/*   remaining specification found in an import           */
/*   specification after the module name.                 */
/**********************************************************/
static int ParseExportSpec(
  void *theEnv,
  char *readSource,
  struct token *theToken,
  struct defmodule *newModule,
  struct defmodule *importModule)
  {
   struct portItem *newPort;
   SYMBOL_HN *theConstruct, *moduleName;
   struct portConstructItem *thePortConstruct;
   char *errorMessage;

   /*===========================================*/
   /* Set up some variables for error messages. */
   /*===========================================*/

   if (importModule != NULL)
     {
      errorMessage = "defmodule import specification";
      moduleName = importModule->name;
     }
   else
     {
      errorMessage = "defmodule export specification";
      moduleName = NULL;
     }

   /*=============================================*/
   /* Handle the special variables ?ALL and ?NONE */
   /* in the import/export specification.         */
   /*=============================================*/

   SavePPBuffer(theEnv," ");
   GetToken(theEnv,readSource,theToken);

   if (theToken->type == SF_VARIABLE)
     {
      /*==============================*/
      /* Check to see if the variable */
      /* is either ?ALL or ?NONE.     */
      /*==============================*/

      if (strcmp(ValueToString(theToken->value),"ALL") == 0)
        {
         newPort = (struct portItem *) get_struct(theEnv,portItem);
         newPort->moduleName = moduleName;
         newPort->constructType = NULL;
         newPort->constructName = NULL;
         newPort->next = NULL;
        }
      else if (strcmp(ValueToString(theToken->value),"NONE") == 0)
        { newPort = NULL; }
      else
        {
         SyntaxErrorMessage(theEnv,errorMessage);
         return(TRUE);
        }

      /*=======================================================*/
      /* The export/import specification must end with a right */
      /* parenthesis after ?ALL or ?NONE at this point.        */
      /*=======================================================*/

      GetToken(theEnv,readSource,theToken);

      if (theToken->type != RPAREN)
        {
         if (newPort != NULL) rtn_struct(theEnv,portItem,newPort);
         PPBackup(theEnv);
         SavePPBuffer(theEnv," ");
         SavePPBuffer(theEnv,theToken->printForm);
         SyntaxErrorMessage(theEnv,errorMessage);
         return(TRUE);
        }

      /*=====================================*/
      /* Add the new specification to either */
      /* the import or export list.          */
      /*=====================================*/

      if (newPort != NULL)
        {
         if (importModule != NULL)
           {
            newPort->next = newModule->importList;
            newModule->importList = newPort;
           }
         else
           {
            newPort->next = newModule->exportList;
            newModule->exportList = newPort;
           }
        }

      /*============================================*/
      /* Return FALSE to indicate the import/export */
      /* specification was successfully parsed.     */
      /*============================================*/

      return(FALSE);
     }

   /*========================================================*/
   /* If the ?ALL and ?NONE keywords were not used, then the */
   /* token must be the name of an importable construct.     */
   /*========================================================*/

   if (theToken->type != SYMBOL)
     {
      SyntaxErrorMessage(theEnv,errorMessage);
      return(TRUE);
     }

   theConstruct = (SYMBOL_HN *) theToken->value;

   if ((thePortConstruct = ValidPortConstructItem(theEnv,ValueToString(theConstruct))) == NULL)
     {
      SyntaxErrorMessage(theEnv,errorMessage);
      return(TRUE);
     }

   /*=============================================================*/
   /* If the next token is the special variable ?ALL, then all    */
   /* constructs of the specified type are imported/exported. If  */
   /* the next token is the special variable ?NONE, then no       */
   /* constructs of the specified type will be imported/exported. */
   /*=============================================================*/

   SavePPBuffer(theEnv," ");
   GetToken(theEnv,readSource,theToken);

   if (theToken->type == SF_VARIABLE)
     {
      /*==============================*/
      /* Check to see if the variable */
      /* is either ?ALL or ?NONE.     */
      /*==============================*/

      if (strcmp(ValueToString(theToken->value),"ALL") == 0)
        {
         newPort = (struct portItem *) get_struct(theEnv,portItem);
         newPort->moduleName = moduleName;
         newPort->constructType = theConstruct;
         newPort->constructName = NULL;
         newPort->next = NULL;
        }
      else if (strcmp(ValueToString(theToken->value),"NONE") == 0)
        { newPort = NULL; }
      else
        {
         SyntaxErrorMessage(theEnv,errorMessage);
         return(TRUE);
        }

      /*=======================================================*/
      /* The export/import specification must end with a right */
      /* parenthesis after ?ALL or ?NONE at this point.        */
      /*=======================================================*/

      GetToken(theEnv,readSource,theToken);

      if (theToken->type != RPAREN)
        {
         if (newPort != NULL) rtn_struct(theEnv,portItem,newPort);
         PPBackup(theEnv);
         SavePPBuffer(theEnv," ");
         SavePPBuffer(theEnv,theToken->printForm);
         SyntaxErrorMessage(theEnv,errorMessage);
         return(TRUE);
        }

      /*=====================================*/
      /* Add the new specification to either */
      /* the import or export list.          */
      /*=====================================*/

      if (newPort != NULL)
        {
         if (importModule != NULL)
           {
            newPort->next = newModule->importList;
            newModule->importList = newPort;
           }
         else
           {
            newPort->next = newModule->exportList;
            newModule->exportList = newPort;
           }
        }

      /*============================================*/
      /* Return FALSE to indicate the import/export */
      /* specification was successfully parsed.     */
      /*============================================*/

      return(FALSE);
     }

   /*============================================*/
   /* There must be at least one named construct */
   /* in the import/export list at this point.   */
   /*============================================*/

   if (theToken->type == RPAREN)
     {
      SyntaxErrorMessage(theEnv,errorMessage);
      return(TRUE);
     }

   /*=====================================*/
   /* Read in the list of imported items. */
   /*=====================================*/

   while (theToken->type != RPAREN)
     {
      if (theToken->type != thePortConstruct->typeExpected)
        {
         SyntaxErrorMessage(theEnv,errorMessage);
         return(TRUE);
        }

      /*========================================*/
      /* Create the data structure to represent */
      /* the import/export specification for    */
      /* the named construct.                   */
      /*========================================*/

      newPort = (struct portItem *) get_struct(theEnv,portItem);
      newPort->moduleName = moduleName;
      newPort->constructType = theConstruct;
      newPort->constructName = (SYMBOL_HN *) theToken->value;

      /*=====================================*/
      /* Add the new specification to either */
      /* the import or export list.          */
      /*=====================================*/

      if (importModule != NULL)
        {
         newPort->next = newModule->importList;
         newModule->importList = newPort;
        }
      else
        {
         newPort->next = newModule->exportList;
         newModule->exportList = newPort;
        }

      /*===================================*/
      /* Move on to the next import/export */
      /* specification.                    */
      /*===================================*/

      SavePPBuffer(theEnv," ");
      GetToken(theEnv,readSource,theToken);
     }

   /*=============================*/
   /* Fix up pretty print buffer. */
   /*=============================*/

   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,")");

   /*============================================*/
   /* Return FALSE to indicate the import/export */
   /* specification was successfully parsed.     */
   /*============================================*/

   return(FALSE);
  }

/*************************************************************/
/* ValidPortConstructItem: Returns TRUE if a given construct */
/*   name is in the list of constructs which can be exported */
/*   and imported, otherwise FALSE is returned.              */
/*************************************************************/
globle struct portConstructItem *ValidPortConstructItem(
  void *theEnv,
  char *theName)
  {
   struct portConstructItem *theItem;

   for (theItem = DefmoduleData(theEnv)->ListOfPortConstructItems;
        theItem != NULL;
        theItem = theItem->next)
     { if (strcmp(theName,theItem->constructName) == 0) return(theItem); }

   return(NULL);
  }

/***********************************************************/
/* FindMultiImportConflict: Determines if a module imports */
/*   the same named construct from more than one module    */
/*   (i.e. an ambiguous reference which is not allowed).   */
/***********************************************************/
static int FindMultiImportConflict(
  void *theEnv,
  struct defmodule *theModule)
  {
   struct defmodule *testModule;
   int count;
   struct portConstructItem *thePCItem;
   struct construct *theConstruct;
   void *theCItem;

   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*============================*/
   /* Loop through every module. */
   /*============================*/

   for (testModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        testModule != NULL;
        testModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,testModule))
     {
      /*========================================*/
      /* Loop through every construct type that */
      /* can be imported/exported by a module.  */
      /*========================================*/

      for (thePCItem = DefmoduleData(theEnv)->ListOfPortConstructItems;
           thePCItem != NULL;
           thePCItem = thePCItem->next)
        {
         EnvSetCurrentModule(theEnv,(void *) testModule);

         /*=====================================================*/
         /* Loop through every construct of the specified type. */
         /*=====================================================*/

         theConstruct = FindConstruct(theEnv,thePCItem->constructName);

         for (theCItem = (*theConstruct->getNextItemFunction)(theEnv,NULL);
              theCItem != NULL;
              theCItem = (*theConstruct->getNextItemFunction)(theEnv,theCItem))
            {
             /*===============================================*/
             /* Check to see if the specific construct in the */
             /* module can be imported with more than one     */
             /* reference into the module we're examining for */
             /* ambiguous import  specifications.             */
             /*===============================================*/

             EnvSetCurrentModule(theEnv,(void *) theModule);
             FindImportedConstruct(theEnv,thePCItem->constructName,NULL,
                                   ValueToString((*theConstruct->getConstructNameFunction)
                                                 ((struct constructHeader *) theCItem)),
                                   &count,FALSE,NULL);
             if (count > 1)
               {
                ImportExportConflictMessage(theEnv,"defmodule",EnvGetDefmoduleName(theEnv,theModule),
                                            thePCItem->constructName,
                                            ValueToString((*theConstruct->getConstructNameFunction)
                                                          ((struct constructHeader *) theCItem)));
                RestoreCurrentModule(theEnv);
                return(TRUE);
               }

             EnvSetCurrentModule(theEnv,(void *) testModule);
            }
        }
     }

   /*=============================*/
   /* Restore the current module. */
   /*=============================*/

   RestoreCurrentModule(theEnv);

   /*=======================================*/
   /* Return FALSE to indicate no ambiguous */
   /* references were found.                */
   /*=======================================*/

   return(FALSE);
  }

/******************************************************/
/* NotExportedErrorMessage: Generalized error message */
/*  for indicating that a construct type or specific  */
/*  named construct is not exported.                  */
/******************************************************/
static void NotExportedErrorMessage(
  void *theEnv,
  char *theModule,
  char *theConstruct,
  char *theName)
  {
   PrintErrorID(theEnv,"MODULPSR",1,TRUE);
   EnvPrintRouter(theEnv,WERROR,"Module ");
   EnvPrintRouter(theEnv,WERROR,theModule);
   EnvPrintRouter(theEnv,WERROR," does not export ");

   if (theConstruct == NULL) EnvPrintRouter(theEnv,WERROR,"any constructs");
   else if (theName == NULL)
     {
      EnvPrintRouter(theEnv,WERROR,"any ");
      EnvPrintRouter(theEnv,WERROR,theConstruct);
      EnvPrintRouter(theEnv,WERROR," constructs");
     }
   else
     {
      EnvPrintRouter(theEnv,WERROR,"the ");
      EnvPrintRouter(theEnv,WERROR,theConstruct);
      EnvPrintRouter(theEnv,WERROR," ");
      EnvPrintRouter(theEnv,WERROR,theName);
     }

   EnvPrintRouter(theEnv,WERROR,".\n");
  }

/*************************************************************/
/* FindImportExportConflict: Determines if the definition of */
/*   a construct would cause an import/export conflict. The  */
/*   construct is not yet defined when this function is      */
/*   called. TRUE is returned if an import/export conflicts  */
/*   is found, otherwise FALSE is returned.                  */
/*************************************************************/
globle int FindImportExportConflict(
  void *theEnv,
  char *constructName,
  struct defmodule *matchModule,
  char *findName)
  {
   struct defmodule *theModule;
   struct moduleItem *theModuleItem;
   int count;

   /*===========================================================*/
   /* If the construct type can't be imported or exported, then */
   /* it's not possible to have an import/export conflict.      */
   /*===========================================================*/

   if (ValidPortConstructItem(theEnv,constructName) == NULL) return(FALSE);

   /*============================================*/
   /* There module name should already have been */
   /* separated fromthe construct's name.        */
   /*============================================*/

   if (FindModuleSeparator(findName)) return(FALSE);

   /*===============================================================*/
   /* The construct must be capable of being stored within a module */
   /* (this test should never fail). The construct must also have   */
   /* a find function associated with it so we can actually look    */
   /* for import/export conflicts.                                  */
   /*===============================================================*/

   if ((theModuleItem = FindModuleItem(theEnv,constructName)) == NULL) return(FALSE);

   if (theModuleItem->findFunction == NULL) return(FALSE);

   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*================================================================*/
   /* Look at each module and count each definition of the specified */
   /* construct which is visible to the module. If more than one     */
   /* definition is visible, then an import/export conflict exists   */
   /* and TRUE is returned.                                          */
   /*================================================================*/

   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      FindImportedConstruct(theEnv,constructName,NULL,findName,&count,TRUE,matchModule);
      if (count > 1)
        {
         RestoreCurrentModule(theEnv);
         return(TRUE);
        }
     }

   /*==========================================*/
   /* Restore the current module. No conflicts */
   /* were detected so FALSE is returned.      */
   /*==========================================*/

   RestoreCurrentModule(theEnv);
   return(FALSE);
  }

#endif /* DEFMODULE_CONSTRUCT && (! RUN_TIME) && (! BLOAD_ONLY) */


