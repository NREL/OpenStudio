   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*              CONSTRUCT PARSER MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Parsing routines and utilities for parsing       */
/*   constructs.                                             */
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
/*            Made the construct redefinition message more   */
/*            prominent.                                     */
/*                                                           */
/*            Added pragmas to remove compilation warnings.  */
/*                                                           */
/*************************************************************/

#define _CSTRCPSR_SOURCE_

#include "setup.h"

#if (! RUN_TIME) && (! BLOAD_ONLY)

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>

#include "envrnmnt.h"
#include "router.h"
#include "watch.h"
#include "constrct.h"
#include "prcdrpsr.h"
#include "exprnpsr.h"
#include "modulutl.h"
#include "modulpsr.h"
#include "sysdep.h"
#include "utility.h"

#include "cstrcpsr.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static int                     FindConstructBeginning(void *,char *,struct token *,int,int *);

/************************************************/
/* Load: C access routine for the load command. */
/************************************************/
#if ALLOW_ENVIRONMENT_GLOBALS
globle int Load(
  char *fileName)
  {
   return EnvLoad(GetCurrentEnvironment(),fileName);
  }
#endif
  
/************************************************************/
/* EnvLoad: C access routine for the load command. Returns  */
/*   0 if the file couldn't be opened, -1 if the file was   */
/*   opened but an error occurred while loading constructs, */
/*   and 1 if the file was opened and no errors occured     */
/*   while loading.                                         */
/************************************************************/
globle int EnvLoad(
  void *theEnv,
  char *fileName)
  {
   FILE *theFile;
   int noErrorsDetected;

   /*=======================================*/
   /* Open the file specified by file name. */
   /*=======================================*/

   if ((theFile = GenOpen(theEnv,fileName,"r")) == NULL) return(0);

   /*===================================================*/
   /* Read in the constructs. Enabling fast load allows */
   /* the router system to be bypassed for quicker load */
   /* times.                                            */
   /*===================================================*/

   SetFastLoad(theEnv,theFile);
   noErrorsDetected = LoadConstructsFromLogicalName(theEnv,(char *) theFile);
   SetFastLoad(theEnv,NULL);

   /*=================*/
   /* Close the file. */
   /*=================*/

   GenClose(theEnv,theFile);

   /*========================================*/
   /* If no errors occurred during the load, */
   /* return 1, otherwise return -1.         */
   /*========================================*/

   if (noErrorsDetected) return(1);

   return(-1);
  }

/*****************************************************************/
/* LoadConstructsFromLogicalName: Loads a set of constructs into */
/*   the current environment from a specified logical name.      */
/*****************************************************************/
globle int LoadConstructsFromLogicalName(
  void *theEnv,
  char *readSource)
  {
   int constructFlag;
   struct token theToken;
   int noErrors = TRUE;
   int foundConstruct;

   /*=========================================*/
   /* Reset the halt execution and evaluation */
   /* error flags in preparation for parsing. */
   /*=========================================*/

   if (EvaluationData(theEnv)->CurrentEvaluationDepth == 0) SetHaltExecution(theEnv,FALSE);
   SetEvaluationError(theEnv,FALSE);

   /*========================================================*/
   /* Find the beginning of the first construct in the file. */
   /*========================================================*/

   EvaluationData(theEnv)->CurrentEvaluationDepth++;
   GetToken(theEnv,readSource,&theToken);
   foundConstruct = FindConstructBeginning(theEnv,readSource,&theToken,FALSE,&noErrors);

   /*==================================================*/
   /* Parse the file until the end of file is reached. */
   /*==================================================*/

   while ((foundConstruct == TRUE) && (GetHaltExecution(theEnv) == FALSE))
     {
      /*===========================================================*/
      /* Clear the pretty print buffer in preparation for parsing. */
      /*===========================================================*/

      FlushPPBuffer(theEnv);

      /*======================*/
      /* Parse the construct. */
      /*======================*/

      constructFlag = ParseConstruct(theEnv,ValueToString(theToken.value),readSource);

      /*==============================================================*/
      /* If an error occurred while parsing, then find the beginning  */
      /* of the next construct (but don't generate any more error     */
      /* messages--in effect, skip everything until another construct */
      /* is found).                                                   */
      /*==============================================================*/

      if (constructFlag == 1)
        {
         EnvPrintRouter(theEnv,WERROR,"\nERROR:\n");
         PrintInChunks(theEnv,WERROR,GetPPBuffer(theEnv));
         EnvPrintRouter(theEnv,WERROR,"\n");
         noErrors = FALSE;
         GetToken(theEnv,readSource,&theToken);
         foundConstruct = FindConstructBeginning(theEnv,readSource,&theToken,TRUE,&noErrors);
        }

      /*======================================================*/
      /* Otherwise, find the beginning of the next construct. */
      /*======================================================*/

      else
        {
         GetToken(theEnv,readSource,&theToken);
         foundConstruct = FindConstructBeginning(theEnv,readSource,&theToken,FALSE,&noErrors);
        }

      /*=====================================================*/
      /* Yield time if necessary to foreground applications. */
      /*=====================================================*/

       if (foundConstruct)
         { IncrementSymbolCount(theToken.value); }
       EvaluationData(theEnv)->CurrentEvaluationDepth--;
       PeriodicCleanup(theEnv,FALSE,TRUE);
       YieldTime(theEnv);
       EvaluationData(theEnv)->CurrentEvaluationDepth++;
       if (foundConstruct)
         { DecrementSymbolCount(theEnv,(SYMBOL_HN *) theToken.value); }
     }

   EvaluationData(theEnv)->CurrentEvaluationDepth--;

   /*========================================================*/
   /* Print a carriage return if a single character is being */
   /* printed to indicate constructs are being processed.    */
   /*========================================================*/

#if DEBUGGING_FUNCTIONS
   if ((EnvGetWatchItem(theEnv,"compilations") != TRUE) && GetPrintWhileLoading(theEnv))
#else
   if (GetPrintWhileLoading(theEnv))
#endif
     { EnvPrintRouter(theEnv,WDIALOG,"\n"); }

   /*=============================================================*/
   /* Once the load is complete, destroy the pretty print buffer. */
   /* This frees up any memory that was used to create the pretty */
   /* print forms for constructs during parsing. Thus calls to    */
   /* the mem-used function will accurately reflect the amount of */
   /* memory being used after a load command.                     */
   /*=============================================================*/

   DestroyPPBuffer(theEnv);

   /*==========================================================*/
   /* Return a boolean flag which indicates whether any errors */
   /* were encountered while loading the constructs.           */
   /*==========================================================*/

   return(noErrors);
  }

/********************************************************************/
/* FindConstructBeginning: Searches for a left parenthesis followed */
/*   by the name of a valid construct. Used by the load command to  */
/*   find the next construct to be parsed. Returns TRUE is the      */
/*   beginning of a construct was found, otherwise FALSE.           */
/********************************************************************/
static int FindConstructBeginning(
  void *theEnv,
  char *readSource,
  struct token *theToken,
  int errorCorrection,
  int *noErrors)
  {
   int leftParenthesisFound = FALSE;
   int firstAttempt = TRUE;

   /*===================================================*/
   /* Process tokens until the beginning of a construct */
   /* is found or there are no more tokens.             */
   /*===================================================*/

   while (theToken->type != STOP)
     {
      /*=====================================================*/
      /* Constructs begin with a left parenthesis. Make note */
      /* that the opening parenthesis has been found.        */
      /*=====================================================*/

      if (theToken->type == LPAREN)
        { leftParenthesisFound = TRUE; }

      /*=================================================================*/
      /* The name of the construct follows the opening left parenthesis. */
      /* If it is the name of a valid construct, then return TRUE.       */
      /* Otherwise, reset the flags to look for the beginning of a       */
      /* construct. If error correction is being performed (i.e. the     */
      /* last construct parsed had an error in it), then don't bother to */
      /* print an error message, otherwise, print an error message.      */
      /*=================================================================*/

      else if ((theToken->type == SYMBOL) && (leftParenthesisFound == TRUE))
        {
         /*===========================================================*/
         /* Is this a valid construct name (e.g., defrule, deffacts). */
         /*===========================================================*/

         if (FindConstruct(theEnv,ValueToString(theToken->value)) != NULL) return(TRUE);

         /*===============================================*/
         /* The construct name is invalid. Print an error */
         /* message if one hasn't already been printed.   */
         /*===============================================*/

         if (firstAttempt && (! errorCorrection))
           {
            errorCorrection = TRUE;
            *noErrors = FALSE;
            PrintErrorID(theEnv,"CSTRCPSR",1,TRUE);
            EnvPrintRouter(theEnv,WERROR,"Expected the beginning of a construct.\n");
           }

         /*======================================================*/
         /* Indicate that an error has been found and that we're */
         /* looking for a left parenthesis again.                */
         /*======================================================*/

         firstAttempt = FALSE;
         leftParenthesisFound = FALSE;
        }

      /*====================================================================*/
      /* Any token encountered other than a left parenthesis or a construct */
      /* name following a left parenthesis is illegal. Again, if error      */
      /* correction is in progress, no error message is printed, otherwise, */
      /*  an error message is printed.                                      */
      /*====================================================================*/

      else
        {
         if (firstAttempt && (! errorCorrection))
           {
            errorCorrection = TRUE;
            *noErrors = FALSE;
            PrintErrorID(theEnv,"CSTRCPSR",1,TRUE);
            EnvPrintRouter(theEnv,WERROR,"Expected the beginning of a construct.\n");
           }

         firstAttempt = FALSE;
         leftParenthesisFound = FALSE;
        }

      /*============================================*/
      /* Move on to the next token to be processed. */
      /*============================================*/

      GetToken(theEnv,readSource,theToken);
     }

   /*===================================================================*/
   /* Couldn't find the beginning of a construct, so FALSE is returned. */
   /*===================================================================*/

   return(FALSE);
  }

/***********************************************************/
/* ParseConstruct: Parses a construct. Returns an integer. */
/*   -1 if the construct name has no parsing function, 0   */
/*   if the construct was parsed successfully, and 1 if    */
/*   the construct was parsed unsuccessfully.              */
/***********************************************************/
globle int ParseConstruct(
  void *theEnv,
  char *name,
  char *logicalName)
  {
   struct construct *currentPtr;
   int rv, ov;

   /*=================================*/
   /* Look for a valid construct name */
   /* (e.g. defrule, deffacts).       */
   /*=================================*/

   currentPtr = FindConstruct(theEnv,name);
   if (currentPtr == NULL) return(-1);

   /*==================================*/
   /* Prepare the parsing environment. */
   /*==================================*/

   ov = GetHaltExecution(theEnv);
   SetEvaluationError(theEnv,FALSE);
   SetHaltExecution(theEnv,FALSE);
   ClearParsedBindNames(theEnv);
   PushRtnBrkContexts(theEnv);
   ExpressionData(theEnv)->ReturnContext = FALSE;
   ExpressionData(theEnv)->BreakContext = FALSE;
   EvaluationData(theEnv)->CurrentEvaluationDepth++;

   /*=======================================*/
   /* Call the construct's parsing routine. */
   /*=======================================*/

   ConstructData(theEnv)->ParsingConstruct = TRUE;
   rv = (*currentPtr->parseFunction)(theEnv,logicalName);
   ConstructData(theEnv)->ParsingConstruct = FALSE;

   /*===============================*/
   /* Restore environment settings. */
   /*===============================*/

   EvaluationData(theEnv)->CurrentEvaluationDepth--;
   PopRtnBrkContexts(theEnv);

   ClearParsedBindNames(theEnv);
   SetPPBufferStatus(theEnv,OFF);
   SetHaltExecution(theEnv,ov);

   /*==============================*/
   /* Return the status of parsing */
   /* the construct.               */
   /*==============================*/

   return(rv);
  }

/*********************************************************/
/* GetConstructNameAndComment: Get the name and comment  */
/*   field of a construct. Returns name of the construct */
/*   if no errors are detected, otherwise returns NULL.  */
/*********************************************************/
#if WIN_BTC && (! DEBUGGING_FUNCTIONS)
#pragma argsused
#endif
globle SYMBOL_HN *GetConstructNameAndComment(
  void *theEnv,
  char *readSource,
  struct token *inputToken,
  char *constructName,
  void *(*findFunction)(void *,char *),
  int (*deleteFunction)(void *,void *),
  char *constructSymbol,
  int fullMessageCR,
  int getComment,
  int moduleNameAllowed)
  {
#if (MAC_MCW || WIN_MCW || MAC_XCD) && (! DEBUGGING_FUNCTIONS)
#pragma unused(fullMessageCR)
#endif
   SYMBOL_HN *name, *moduleName;
   int redefining = FALSE;
   void *theConstruct;
   unsigned separatorPosition;
   struct defmodule *theModule;

   /*==========================*/
   /* Next token should be the */
   /* name of the construct.   */
   /*==========================*/

   GetToken(theEnv,readSource,inputToken);
   if (inputToken->type != SYMBOL)
     {
      PrintErrorID(theEnv,"CSTRCPSR",2,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Missing name for ");
      EnvPrintRouter(theEnv,WERROR,constructName);
      EnvPrintRouter(theEnv,WERROR," construct\n");
      return(NULL);
     }

   name = (SYMBOL_HN *) inputToken->value;

   /*===============================*/
   /* Determine the current module. */
   /*===============================*/

   separatorPosition = FindModuleSeparator(ValueToString(name));
   if (separatorPosition)
     {
      if (moduleNameAllowed == FALSE)
        {
         SyntaxErrorMessage(theEnv,"module specifier");
         return(NULL);
        }

      moduleName = ExtractModuleName(theEnv,separatorPosition,ValueToString(name));
      if (moduleName == NULL)
        {
         SyntaxErrorMessage(theEnv,"construct name");
         return(NULL);
        }

      theModule = (struct defmodule *) EnvFindDefmodule(theEnv,ValueToString(moduleName));
      if (theModule == NULL)
        {
         CantFindItemErrorMessage(theEnv,"defmodule",ValueToString(moduleName));
         return(NULL);
        }

      EnvSetCurrentModule(theEnv,(void *) theModule);
      name = ExtractConstructName(theEnv,separatorPosition,ValueToString(name));
      if (name == NULL)
        {
         SyntaxErrorMessage(theEnv,"construct name");
         return(NULL);
        }
     }

   /*=====================================================*/
   /* If the module was not specified, record the current */
   /* module name as part of the pretty-print form.       */
   /*=====================================================*/

   else
     {
      theModule = ((struct defmodule *) EnvGetCurrentModule(theEnv));
      if (moduleNameAllowed)
        {
         PPBackup(theEnv);
         SavePPBuffer(theEnv,EnvGetDefmoduleName(theEnv,theModule));
         SavePPBuffer(theEnv,"::");
         SavePPBuffer(theEnv,ValueToString(name));
        }
     }

   /*==================================================================*/
   /* Check for import/export conflicts from the construct definition. */
   /*==================================================================*/

#if DEFMODULE_CONSTRUCT
   if (FindImportExportConflict(theEnv,constructName,theModule,ValueToString(name)))
     {
      ImportExportConflictMessage(theEnv,constructName,ValueToString(name),NULL,NULL);
      return(NULL);
     }
#endif

   /*========================================================*/
   /* Remove the construct if it is already in the knowledge */
   /* base and we're not just checking syntax.               */
   /*========================================================*/

   if ((findFunction != NULL) && (! ConstructData(theEnv)->CheckSyntaxMode))
     {
      theConstruct = (*findFunction)(theEnv,ValueToString(name));
      if (theConstruct != NULL)
        {
         redefining = TRUE;
         if (deleteFunction != NULL)
           {
            if ((*deleteFunction)(theEnv,theConstruct) == FALSE)
              {
               PrintErrorID(theEnv,"CSTRCPSR",4,TRUE);
               EnvPrintRouter(theEnv,WERROR,"Cannot redefine ");
               EnvPrintRouter(theEnv,WERROR,constructName);
               EnvPrintRouter(theEnv,WERROR," ");
               EnvPrintRouter(theEnv,WERROR,ValueToString(name));
               EnvPrintRouter(theEnv,WERROR," while it is in use.\n");
               return(NULL);
              }
           }
        }
     }

   /*=============================================*/
   /* If compilations are being watched, indicate */
   /* that a construct is being compiled.         */
   /*=============================================*/

#if DEBUGGING_FUNCTIONS
   if ((EnvGetWatchItem(theEnv,"compilations") == TRUE) &&
       GetPrintWhileLoading(theEnv) && (! ConstructData(theEnv)->CheckSyntaxMode))
     {
      if (redefining) 
        {
         PrintWarningID(theEnv,"CSTRCPSR",1,TRUE);
         EnvPrintRouter(theEnv,WDIALOG,"Redefining ");
        }
      else EnvPrintRouter(theEnv,WDIALOG,"Defining ");

      EnvPrintRouter(theEnv,WDIALOG,constructName);
      EnvPrintRouter(theEnv,WDIALOG,": ");
      EnvPrintRouter(theEnv,WDIALOG,ValueToString(name));

      if (fullMessageCR) EnvPrintRouter(theEnv,WDIALOG,"\n");
      else EnvPrintRouter(theEnv,WDIALOG," ");
     }
   else
#endif
     {
      if (GetPrintWhileLoading(theEnv) && (! ConstructData(theEnv)->CheckSyntaxMode))
        { EnvPrintRouter(theEnv,WDIALOG,constructSymbol); }
     }

   /*===============================*/
   /* Get the comment if it exists. */
   /*===============================*/

   GetToken(theEnv,readSource,inputToken);
   if ((inputToken->type == STRING) && getComment)
     {
      PPBackup(theEnv);
      SavePPBuffer(theEnv," ");
      SavePPBuffer(theEnv,inputToken->printForm);
      GetToken(theEnv,readSource,inputToken);
      if (inputToken->type != RPAREN)
        {
         PPBackup(theEnv);
         SavePPBuffer(theEnv,"\n   ");
         SavePPBuffer(theEnv,inputToken->printForm);
        }
     }
   else if (inputToken->type != RPAREN)
     {
      PPBackup(theEnv);
      SavePPBuffer(theEnv,"\n   ");
      SavePPBuffer(theEnv,inputToken->printForm);
     }

   /*===================================*/
   /* Return the name of the construct. */
   /*===================================*/

   return(name);
  }

/****************************************/
/* RemoveConstructFromModule: Removes a */
/*   construct from its module's list   */
/****************************************/
globle void RemoveConstructFromModule(
  void *theEnv,
  struct constructHeader *theConstruct)
  {
   struct constructHeader *lastConstruct,*currentConstruct;

   /*==============================*/
   /* Find the specified construct */
   /* in the module's list.        */
   /*==============================*/

   lastConstruct = NULL;
   currentConstruct = theConstruct->whichModule->firstItem;
   while (currentConstruct != theConstruct)
     {
      lastConstruct = currentConstruct;
      currentConstruct = currentConstruct->next;
     }

   /*========================================*/
   /* If it wasn't there, something's wrong. */
   /*========================================*/

   if (currentConstruct == NULL)
     {
      SystemError(theEnv,"CSTRCPSR",1);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   /*==========================*/
   /* Remove it from the list. */
   /*==========================*/

   if (lastConstruct == NULL)
     { theConstruct->whichModule->firstItem = theConstruct->next; }
   else
     { lastConstruct->next = theConstruct->next; }

   /*=================================================*/
   /* Update the pointer to the last item in the list */
   /* if the construct just deleted was at the end.   */
   /*=================================================*/

   if (theConstruct == theConstruct->whichModule->lastItem)
     { theConstruct->whichModule->lastItem = lastConstruct; }
  }

/******************************************************/
/* ImportExportConflictMessage: Generic error message */
/*   for an import/export module conflict detected    */
/*   when a construct is being defined.               */
/******************************************************/
globle void ImportExportConflictMessage(
  void *theEnv,
  char *constructName,
  char *itemName,
  char *causedByConstruct,
  char *causedByName)
  {
   PrintErrorID(theEnv,"CSTRCPSR",3,TRUE);
   EnvPrintRouter(theEnv,WERROR,"Cannot define ");
   EnvPrintRouter(theEnv,WERROR,constructName);
   EnvPrintRouter(theEnv,WERROR," ");
   EnvPrintRouter(theEnv,WERROR,itemName);
   EnvPrintRouter(theEnv,WERROR," because of an import/export conflict");

   if (causedByConstruct == NULL) EnvPrintRouter(theEnv,WERROR,".\n");
   else
     {
      EnvPrintRouter(theEnv,WERROR," caused by the ");
      EnvPrintRouter(theEnv,WERROR,causedByConstruct);
      EnvPrintRouter(theEnv,WERROR," ");
      EnvPrintRouter(theEnv,WERROR,causedByName);
      EnvPrintRouter(theEnv,WERROR,".\n");
     }
  }

#endif /* (! RUN_TIME) && (! BLOAD_ONLY) */


