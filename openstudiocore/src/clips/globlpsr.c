   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*              DEFGLOBAL PARSER MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Parses the defglobal construct.                  */
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
/*            Made the construct redefinition message more   */
/*            prominent.                                     */
/*                                                           */
/*************************************************************/

#define _GLOBLPSR_SOURCE_

#include "setup.h"

#if DEFGLOBAL_CONSTRUCT

#include <string.h>

#include "pprint.h"
#include "router.h"
#include "memalloc.h"
#include "scanner.h"
#include "evaluatn.h"
#include "exprnpsr.h"
#include "constrct.h"
#include "multifld.h"
#include "watch.h"
#include "modulutl.h"
#include "modulpsr.h"
#include "cstrcpsr.h"
#include "globldef.h"
#include "globlbsc.h"
#include "envrnmnt.h"

#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
#include "bload.h"
#endif

#include "globlpsr.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if (! RUN_TIME) && (! BLOAD_ONLY)
   static intBool                 GetVariableDefinition(void *,char *,int *,int,struct token *);
   static void                    AddDefglobal(void *,SYMBOL_HN *,DATA_OBJECT_PTR,struct expr *);
#endif

/*********************************************************************/
/* ParseDefglobal: Coordinates all actions necessary for the parsing */
/*   and creation of a defglobal into the current environment.       */
/*********************************************************************/
globle intBool ParseDefglobal(
  void *theEnv,
  char *readSource)
  {
   int defglobalError = FALSE;
#if (MAC_MCW || WIN_MCW) && (RUN_TIME || BLOAD_ONLY)
#pragma unused(theEnv,readSource)
#endif

#if (! RUN_TIME) && (! BLOAD_ONLY)

   struct token theToken;
   int tokenRead = TRUE;
   struct defmodule *theModule;

   /*=====================================*/
   /* Pretty print buffer initialization. */
   /*=====================================*/

   SetPPBufferStatus(theEnv,ON);
   FlushPPBuffer(theEnv);
   SetIndentDepth(theEnv,3);
   SavePPBuffer(theEnv,"(defglobal ");

   /*=================================================*/
   /* Individual defglobal constructs can't be parsed */
   /* while a binary load is in effect.               */
   /*=================================================*/

#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
   if ((Bloaded(theEnv) == TRUE) && (! ConstructData(theEnv)->CheckSyntaxMode))
     {
      CannotLoadWithBloadMessage(theEnv,"defglobal");
      return(TRUE);
     }
#endif

   /*===========================*/
   /* Look for the module name. */
   /*===========================*/

   GetToken(theEnv,readSource,&theToken);
   if (theToken.type == SYMBOL)
     {
      /*=================================================*/
      /* The optional module name can't contain a module */
      /* separator like other constructs. For example,   */
      /* (defrule X::foo is OK for rules, but the right  */
      /* syntax for defglobals is (defglobal X ?*foo*.   */
      /*=================================================*/

      tokenRead = FALSE;
      if (FindModuleSeparator(ValueToString(theToken.value)))
        {
         SyntaxErrorMessage(theEnv,"defglobal");
         return(TRUE);
        }

      /*=================================*/
      /* Determine if the module exists. */
      /*=================================*/

      theModule = (struct defmodule *) EnvFindDefmodule(theEnv,ValueToString(theToken.value));
      if (theModule == NULL)
        {
         CantFindItemErrorMessage(theEnv,"defmodule",ValueToString(theToken.value));
         return(TRUE);
        }

      /*=========================================*/
      /* If the module name was OK, then set the */
      /* current module to the specified module. */
      /*=========================================*/

      SavePPBuffer(theEnv," ");
      EnvSetCurrentModule(theEnv,(void *) theModule);
     }

   /*===========================================*/
   /* If the module name wasn't specified, then */
   /* use the current module's name in the      */
   /* defglobal's pretty print representation.  */
   /*===========================================*/

   else
     {
      PPBackup(theEnv);
      SavePPBuffer(theEnv,EnvGetDefmoduleName(theEnv,((struct defmodule *) EnvGetCurrentModule(theEnv))));
      SavePPBuffer(theEnv," ");
      SavePPBuffer(theEnv,theToken.printForm);
     }

   /*======================*/
   /* Parse the variables. */
   /*======================*/

   while (GetVariableDefinition(theEnv,readSource,&defglobalError,tokenRead,&theToken))
     {
      tokenRead = FALSE;

      FlushPPBuffer(theEnv);
      SavePPBuffer(theEnv,"(defglobal ");
      SavePPBuffer(theEnv,EnvGetDefmoduleName(theEnv,((struct defmodule *) EnvGetCurrentModule(theEnv))));
      SavePPBuffer(theEnv," ");
     }

#endif

   /*==================================*/
   /* Return the parsing error status. */
   /*==================================*/

   return(defglobalError);
  }

#if (! RUN_TIME) && (! BLOAD_ONLY)

/***************************************************************/
/* GetVariableDefinition: Parses and evaluates a single global */
/*   variable in a defglobal construct. Returns TRUE if the    */
/*   variable was successfully parsed and FALSE if a right     */
/*   parenthesis is encountered (signifying the end of the     */
/*   defglobal construct) or an error occurs. The error status */
/*   flag is also set if an error occurs.                      */
/***************************************************************/
static intBool GetVariableDefinition(
  void *theEnv,
  char *readSource,
  int *defglobalError,
  int tokenRead,
  struct token *theToken)
  {
   SYMBOL_HN *variableName;
   struct expr *assignPtr;
   DATA_OBJECT assignValue;

   /*========================================*/
   /* Get next token, which should either be */
   /* a closing parenthesis or a variable.   */
   /*========================================*/

   if (! tokenRead) GetToken(theEnv,readSource,theToken);
   if (theToken->type == RPAREN) return(FALSE);

   if (theToken->type == SF_VARIABLE)
     {
      SyntaxErrorMessage(theEnv,"defglobal");
      *defglobalError = TRUE;
      return(FALSE);
     }
   else if (theToken->type != GBL_VARIABLE)
     {
      SyntaxErrorMessage(theEnv,"defglobal");
      *defglobalError = TRUE;
      return(FALSE);
     }

   variableName = (SYMBOL_HN *) theToken->value;

   SavePPBuffer(theEnv," ");

   /*================================*/
   /* Print out compilation message. */
   /*================================*/

#if DEBUGGING_FUNCTIONS
   if ((EnvGetWatchItem(theEnv,"compilations") == ON) && GetPrintWhileLoading(theEnv))
     {
      if (QFindDefglobal(theEnv,variableName) != NULL) 
        {
         PrintWarningID(theEnv,"CSTRCPSR",1,TRUE);
         EnvPrintRouter(theEnv,WDIALOG,"Redefining defglobal: ");
        }
      else EnvPrintRouter(theEnv,WDIALOG,"Defining defglobal: ");
      EnvPrintRouter(theEnv,WDIALOG,ValueToString(variableName));
      EnvPrintRouter(theEnv,WDIALOG,"\n");
     }
   else
#endif
     { if (GetPrintWhileLoading(theEnv)) EnvPrintRouter(theEnv,WDIALOG,":"); }

   /*==================================================================*/
   /* Check for import/export conflicts from the construct definition. */
   /*==================================================================*/

#if DEFMODULE_CONSTRUCT
   if (FindImportExportConflict(theEnv,"defglobal",((struct defmodule *) EnvGetCurrentModule(theEnv)),ValueToString(variableName)))
     {
      ImportExportConflictMessage(theEnv,"defglobal",ValueToString(variableName),NULL,NULL);
      *defglobalError = TRUE;
      return(FALSE);
     }
#endif

   /*==============================*/
   /* The next token must be an =. */
   /*==============================*/

   GetToken(theEnv,readSource,theToken);
   if (strcmp(theToken->printForm,"=") != 0)
     {
      SyntaxErrorMessage(theEnv,"defglobal");
      *defglobalError = TRUE;
      return(FALSE);
     }

   SavePPBuffer(theEnv," ");

   /*======================================================*/
   /* Parse the expression to be assigned to the variable. */
   /*======================================================*/

   assignPtr = ParseAtomOrExpression(theEnv,readSource,NULL);
   if (assignPtr == NULL)
     {
      *defglobalError = TRUE;
      return(FALSE);
     }

   /*==========================*/
   /* Evaluate the expression. */
   /*==========================*/

   if (! ConstructData(theEnv)->CheckSyntaxMode)
     {
      SetEvaluationError(theEnv,FALSE);
      if (EvaluateExpression(theEnv,assignPtr,&assignValue))
        {
         ReturnExpression(theEnv,assignPtr);
         *defglobalError = TRUE;
         return(FALSE);
        }
     }
   else
     { ReturnExpression(theEnv,assignPtr); }

   SavePPBuffer(theEnv,")");

   /*======================================*/
   /* Add the variable to the global list. */
   /*======================================*/

   if (! ConstructData(theEnv)->CheckSyntaxMode)
     { AddDefglobal(theEnv,variableName,&assignValue,assignPtr); }

   /*==================================================*/
   /* Return TRUE to indicate that the global variable */
   /* definition was successfully parsed.              */
   /*==================================================*/

   return(TRUE);
  }

/*********************************************************/
/* AddDefglobal: Adds a defglobal to the current module. */
/*********************************************************/
static void AddDefglobal(
  void *theEnv,
  SYMBOL_HN *name,
  DATA_OBJECT_PTR vPtr,
  struct expr *ePtr)
  {
   struct defglobal *defglobalPtr;
   intBool newGlobal = FALSE;
#if DEBUGGING_FUNCTIONS
   int GlobalHadWatch = FALSE;
#endif

   /*========================================================*/
   /* If the defglobal is already defined, then use the old  */
   /* data structure and substitute new values. If it hasn't */
   /* been defined, then create a new data structure.        */
   /*========================================================*/

   defglobalPtr = QFindDefglobal(theEnv,name);
   if (defglobalPtr == NULL)
     {
      newGlobal = TRUE;
      defglobalPtr = get_struct(theEnv,defglobal);
     }
   else
     {
      DeinstallConstructHeader(theEnv,&defglobalPtr->header);
#if DEBUGGING_FUNCTIONS
      GlobalHadWatch = defglobalPtr->watch;
#endif
     }

   /*===========================================*/
   /* Remove the old values from the defglobal. */
   /*===========================================*/

   if (newGlobal == FALSE)
     {
      ValueDeinstall(theEnv,&defglobalPtr->current);
      if (defglobalPtr->current.type == MULTIFIELD)
        { ReturnMultifield(theEnv,(struct multifield *) defglobalPtr->current.value); }

      RemoveHashedExpression(theEnv,defglobalPtr->initial);
     }

   /*=======================================*/
   /* Copy the new values to the defglobal. */
   /*=======================================*/

   defglobalPtr->current.type = vPtr->type;
   if (vPtr->type != MULTIFIELD) defglobalPtr->current.value = vPtr->value;
   else DuplicateMultifield(theEnv,&defglobalPtr->current,vPtr);
   ValueInstall(theEnv,&defglobalPtr->current);

   defglobalPtr->initial = AddHashedExpression(theEnv,ePtr);
   ReturnExpression(theEnv,ePtr);
   DefglobalData(theEnv)->ChangeToGlobals = TRUE;

   /*=================================*/
   /* Restore the old watch value to  */
   /* the defglobal if redefined.     */
   /*=================================*/

#if DEBUGGING_FUNCTIONS
   defglobalPtr->watch = GlobalHadWatch ? TRUE : WatchGlobals;
#endif

   /*======================================*/
   /* Save the name and pretty print form. */
   /*======================================*/

   defglobalPtr->header.name = name;
   defglobalPtr->header.usrData = NULL;
   IncrementSymbolCount(name);

   SavePPBuffer(theEnv,"\n");
   if (EnvGetConserveMemory(theEnv) == TRUE)
     { defglobalPtr->header.ppForm = NULL; }
   else
     { defglobalPtr->header.ppForm = CopyPPBuffer(theEnv); }

   defglobalPtr->inScope = TRUE;

   /*=============================================*/
   /* If the defglobal was redefined, we're done. */
   /*=============================================*/

   if (newGlobal == FALSE) return;

   /*===================================*/
   /* Copy the defglobal variable name. */
   /*===================================*/

   defglobalPtr->busyCount = 0;
   defglobalPtr->header.whichModule = (struct defmoduleItemHeader *)
                               GetModuleItem(theEnv,NULL,FindModuleItem(theEnv,"defglobal")->moduleIndex);

   /*=============================================*/
   /* Add the defglobal to the list of defglobals */
   /* for the current module.                     */
   /*=============================================*/

   AddConstructToModule(&defglobalPtr->header);
  }

/*****************************************************************/
/* ReplaceGlobalVariable: Replaces a global variable found in an */
/*   expression with the appropriate primitive data type which   */
/*   can later be used to retrieve the global variable's value.  */
/*****************************************************************/
globle intBool ReplaceGlobalVariable(
  void *theEnv,
  struct expr *ePtr)
  {
   struct defglobal *theGlobal;
   int count;

   /*=================================*/
   /* Search for the global variable. */
   /*=================================*/

   theGlobal = (struct defglobal *)
               FindImportedConstruct(theEnv,"defglobal",NULL,ValueToString(ePtr->value),
                                     &count,TRUE,NULL);

   /*=============================================*/
   /* If it wasn't found, print an error message. */
   /*=============================================*/

   if (theGlobal == NULL)
     {
      GlobalReferenceErrorMessage(theEnv,ValueToString(ePtr->value));
      return(FALSE);
     }

   /*========================================================*/
   /* The current implementation of the defmodules shouldn't */
   /* allow a construct to be defined which would cause an   */
   /* ambiguous reference, but we'll check for it anyway.    */
   /*========================================================*/

   if (count > 1)
     {
      AmbiguousReferenceErrorMessage(theEnv,"defglobal",ValueToString(ePtr->value));
      return(FALSE);
     }

   /*==============================================*/
   /* Replace the symbolic reference of the global */
   /* variable with a direct pointer reference.    */
   /*==============================================*/

   ePtr->type = DEFGLOBAL_PTR;
   ePtr->value = (void *) theGlobal;

   return(TRUE);
  }

/*****************************************************************/
/* GlobalReferenceErrorMessage: Prints an error message when a   */
/*   symbolic reference to a global variable cannot be resolved. */
/*****************************************************************/
globle void GlobalReferenceErrorMessage(
  void *theEnv,
  char *variableName)
  {
   PrintErrorID(theEnv,"GLOBLPSR",1,TRUE);
   EnvPrintRouter(theEnv,WERROR,"\nGlobal variable ?*");
   EnvPrintRouter(theEnv,WERROR,variableName);
   EnvPrintRouter(theEnv,WERROR,"* was referenced, but is not defined.\n");
  }

#endif /* (! RUN_TIME) && (! BLOAD_ONLY) */

#endif /* DEFGLOBAL_CONSTRUCT */



