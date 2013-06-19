   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/02/06            */
   /*                                                     */
   /*               PARSING FUNCTIONS MODULE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for several parsing related    */
/*   functions including...                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Corrected code to remove run-time program      */
/*            compiler warnings.                             */
/*                                                           */
/*************************************************************/

#define _PARSEFUN_SOURCE_

#include "setup.h"

#include <string.h>

#include "argacces.h"
#include "cstrcpsr.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "extnfunc.h"
#include "memalloc.h"
#include "multifld.h"
#include "prcdrpsr.h"
#include "router.h"
#include "strngrtr.h"
#include "utility.h"

#include "parsefun.h"

#define PARSEFUN_DATA 11

struct parseFunctionData
  { 
   char *ErrorString;
   size_t ErrorCurrentPosition;
   size_t ErrorMaximumPosition;
   char *WarningString;
   size_t WarningCurrentPosition;
   size_t WarningMaximumPosition;
  };

#define ParseFunctionData(theEnv) ((struct parseFunctionData *) GetEnvironmentData(theEnv,PARSEFUN_DATA))

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if (! RUN_TIME) && (! BLOAD_ONLY)
   static int                     FindErrorCapture(void *,char *);
   static int                     PrintErrorCapture(void *,char *,char *);
   static void                    DeactivateErrorCapture(void *);
   static void                    SetErrorCaptureValues(void *,DATA_OBJECT_PTR);
#endif

/*****************************************/
/* ParseFunctionDefinitions: Initializes */
/*   the parsing related functions.      */
/*****************************************/
globle void ParseFunctionDefinitions(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,PARSEFUN_DATA,sizeof(struct parseFunctionData),NULL);

#if ! RUN_TIME
   EnvDefineFunction2(theEnv,"check-syntax",'u',PTIEF CheckSyntaxFunction,"CheckSyntaxFunction","11s");
#endif
  }

#if (! RUN_TIME) && (! BLOAD_ONLY)
/*******************************************/
/* CheckSyntaxFunction: H/L access routine */
/*   for the check-syntax function.        */
/*******************************************/
globle void CheckSyntaxFunction(
  void *theEnv,
  DATA_OBJECT *returnValue)
  {
   DATA_OBJECT theArg;

   /*===============================*/
   /* Set up a default return value */
   /* (TRUE for problems found).    */
   /*===============================*/

   SetpType(returnValue,SYMBOL);
   SetpValue(returnValue,EnvTrueSymbol(theEnv));

   /*=====================================================*/
   /* Function check-syntax expects exactly one argument. */
   /*=====================================================*/

   if (EnvArgCountCheck(theEnv,"check-syntax",EXACTLY,1) == -1) return;

   /*========================================*/
   /* The argument should be of type STRING. */
   /*========================================*/

   if (EnvArgTypeCheck(theEnv,"check-syntax",1,STRING,&theArg) == FALSE)
     { return; }

   /*===================*/
   /* Check the syntax. */
   /*===================*/

   CheckSyntax(theEnv,DOToString(theArg),returnValue);
  }

/*********************************/
/* CheckSyntax: C access routine */
/*   for the build function.     */
/*********************************/
globle int CheckSyntax(
  void *theEnv,
  char *theString,
  DATA_OBJECT_PTR returnValue)
  {
   char *name;
   struct token theToken;
   struct expr *top;
   short rv;

   /*==============================*/
   /* Set the default return value */
   /* (TRUE for problems found).   */
   /*==============================*/

   SetpType(returnValue,SYMBOL);
   SetpValue(returnValue,EnvTrueSymbol(theEnv));

   /*===========================================*/
   /* Create a string source router so that the */
   /* string can be used as an input source.    */
   /*===========================================*/

   if (OpenStringSource(theEnv,"check-syntax",theString,0) == 0)
     { return(TRUE); }

   /*=================================*/
   /* Only expressions and constructs */
   /* can have their syntax checked.  */
   /*=================================*/

   GetToken(theEnv,"check-syntax",&theToken);

   if (theToken.type != LPAREN)
     {
      CloseStringSource(theEnv,"check-syntax");
      SetpValue(returnValue,EnvAddSymbol(theEnv,"MISSING-LEFT-PARENTHESIS"));
      return(TRUE);
     }

   /*========================================*/
   /* The next token should be the construct */
   /* type or function name.                 */
   /*========================================*/

   GetToken(theEnv,"check-syntax",&theToken);
   if (theToken.type != SYMBOL)
     {
      CloseStringSource(theEnv,"check-syntax");
      SetpValue(returnValue,EnvAddSymbol(theEnv,"EXPECTED-SYMBOL-AFTER-LEFT-PARENTHESIS"));
      return(TRUE);
     }

   name = ValueToString(theToken.value);

   /*==============================================*/
   /* Set up a router to capture the error output. */
   /*==============================================*/

   EnvAddRouter(theEnv,"error-capture",40,
              FindErrorCapture, PrintErrorCapture,
              NULL, NULL, NULL);

   /*================================*/
   /* Determine if it's a construct. */
   /*================================*/

   if (FindConstruct(theEnv,name))
     {
      ConstructData(theEnv)->CheckSyntaxMode = TRUE;
      rv = (short) ParseConstruct(theEnv,name,"check-syntax");
      GetToken(theEnv,"check-syntax",&theToken);
      ConstructData(theEnv)->CheckSyntaxMode = FALSE;

      if (rv)
        {
         EnvPrintRouter(theEnv,WERROR,"\nERROR:\n");
         PrintInChunks(theEnv,WERROR,GetPPBuffer(theEnv));
         EnvPrintRouter(theEnv,WERROR,"\n");
        }

      DestroyPPBuffer(theEnv);

      CloseStringSource(theEnv,"check-syntax");

      if ((rv != FALSE) || (ParseFunctionData(theEnv)->WarningString != NULL))
        {
         SetErrorCaptureValues(theEnv,returnValue);
         DeactivateErrorCapture(theEnv);
         return(TRUE);
        }

      if (theToken.type != STOP)
        {
         SetpValue(returnValue,EnvAddSymbol(theEnv,"EXTRANEOUS-INPUT-AFTER-LAST-PARENTHESIS"));
         DeactivateErrorCapture(theEnv);
         return(TRUE);
        }

      SetpType(returnValue,SYMBOL);
      SetpValue(returnValue,EnvFalseSymbol(theEnv));
      DeactivateErrorCapture(theEnv);
      return(FALSE);
     }

   /*=======================*/
   /* Parse the expression. */
   /*=======================*/

   top = Function2Parse(theEnv,"check-syntax",name);
   GetToken(theEnv,"check-syntax",&theToken);
   ClearParsedBindNames(theEnv);
   CloseStringSource(theEnv,"check-syntax");

   if (top == NULL)
     {
      SetErrorCaptureValues(theEnv,returnValue);
      DeactivateErrorCapture(theEnv);
      return(TRUE);
     }

   if (theToken.type != STOP)
     {
      SetpValue(returnValue,EnvAddSymbol(theEnv,"EXTRANEOUS-INPUT-AFTER-LAST-PARENTHESIS"));
      DeactivateErrorCapture(theEnv);
      ReturnExpression(theEnv,top);
      return(TRUE);
     }

   DeactivateErrorCapture(theEnv);

   ReturnExpression(theEnv,top);
   SetpType(returnValue,SYMBOL);
   SetpValue(returnValue,EnvFalseSymbol(theEnv));
   return(FALSE);
  }

/**************************************************/
/* DeactivateErrorCapture: Deactivates the error  */
/*   capture router and the strings used to store */
/*   the captured information.                    */
/**************************************************/
static void DeactivateErrorCapture(
  void *theEnv)
  {   
   if (ParseFunctionData(theEnv)->ErrorString != NULL)
     {
      rm(theEnv,ParseFunctionData(theEnv)->ErrorString,ParseFunctionData(theEnv)->ErrorMaximumPosition);
      ParseFunctionData(theEnv)->ErrorString = NULL;
     }

   if (ParseFunctionData(theEnv)->WarningString != NULL)
     {
      rm(theEnv,ParseFunctionData(theEnv)->WarningString,ParseFunctionData(theEnv)->WarningMaximumPosition);
      ParseFunctionData(theEnv)->WarningString = NULL;
     }

   ParseFunctionData(theEnv)->ErrorCurrentPosition = 0;
   ParseFunctionData(theEnv)->ErrorMaximumPosition = 0;
   ParseFunctionData(theEnv)->WarningCurrentPosition = 0;
   ParseFunctionData(theEnv)->WarningMaximumPosition = 0;

   EnvDeleteRouter(theEnv,"error-capture");
  }

/******************************************************************/
/* SetErrorCaptureValues: Stores the error/warnings captured when */
/*   parsing an expression or construct into a multifield value.  */
/*   The first field contains the output sent to the WERROR       */
/*   logical name and the second field contains the output sent   */
/*   to the WWARNING logical name. FALSE is stored in either      */
/*   position if no output was sent to those logical names.       */
/******************************************************************/
static void SetErrorCaptureValues(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   struct multifield *theMultifield;

   theMultifield = (struct multifield *) EnvCreateMultifield(theEnv,2L);

   if (ParseFunctionData(theEnv)->ErrorString != NULL)
     {
      SetMFType(theMultifield,1,STRING);
      SetMFValue(theMultifield,1,EnvAddSymbol(theEnv,ParseFunctionData(theEnv)->ErrorString));
     }
   else
     {
      SetMFType(theMultifield,1,SYMBOL);
      SetMFValue(theMultifield,1,EnvFalseSymbol(theEnv));
     }

   if (ParseFunctionData(theEnv)->WarningString != NULL)
     {
      SetMFType(theMultifield,2,STRING);
      SetMFValue(theMultifield,2,EnvAddSymbol(theEnv,ParseFunctionData(theEnv)->WarningString));
     }
   else
     {
      SetMFType(theMultifield,2,SYMBOL);
      SetMFValue(theMultifield,2,EnvFalseSymbol(theEnv));
     }

   SetpType(returnValue,MULTIFIELD);
   SetpDOBegin(returnValue,1);
   SetpDOEnd(returnValue,2);
   SetpValue(returnValue,(void *) theMultifield);
  }

/**********************************/
/* FindErrorCapture: Find routine */
/*   for the check-syntax router. */
/**********************************/
#if WIN_BTC
#pragma argsused
#endif
static int FindErrorCapture(
  void *theEnv,
  char *logicalName)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   if ((strcmp(logicalName,WERROR) == 0) ||
       (strcmp(logicalName,WWARNING) == 0))
     { return(TRUE); }

   return(FALSE);
  }

/************************************/
/* PrintErrorCapture: Print routine */
/*   for the check-syntax router.   */
/************************************/
static int PrintErrorCapture(
  void *theEnv,
  char *logicalName,
  char *str)
  {
   if (strcmp(logicalName,WERROR) == 0)
     {
      ParseFunctionData(theEnv)->ErrorString = AppendToString(theEnv,str,ParseFunctionData(theEnv)->ErrorString,
                                   &ParseFunctionData(theEnv)->ErrorCurrentPosition,
                                   &ParseFunctionData(theEnv)->ErrorMaximumPosition);
     }
   else if (strcmp(logicalName,WWARNING) == 0)
     {
      ParseFunctionData(theEnv)->WarningString = AppendToString(theEnv,str,ParseFunctionData(theEnv)->WarningString,
                                     &ParseFunctionData(theEnv)->WarningCurrentPosition,
                                     &ParseFunctionData(theEnv)->WarningMaximumPosition);
     }

   return(1);
  }

#else
/****************************************************/
/* CheckSyntaxFunction: This is the non-functional  */
/*   stub provided for use with a run-time version. */
/****************************************************/
globle void CheckSyntaxFunction(
  void *theEnv,
  DATA_OBJECT *returnValue)
  {
   PrintErrorID(theEnv,"PARSEFUN",1,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Function check-syntax does not work in run time modules.\n");
   SetpType(returnValue,SYMBOL);
   SetpValue(returnValue,EnvTrueSymbol(theEnv));
  }

/************************************************/
/* CheckSyntax: This is the non-functional stub */
/*   provided for use with a run-time version.  */
/************************************************/
globle int CheckSyntax(
  void *theEnv,
  char *theString,
  DATA_OBJECT_PTR returnValue)
  {
#if (MAC_MCW || WIN_MCW) && (RUN_TIME || BLOAD_ONLY)
#pragma unused(theString)
#pragma unused(returnValue)
#endif

   PrintErrorID(theEnv,"PARSEFUN",1,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Function check-syntax does not work in run time modules.\n");
   SetpType(returnValue,SYMBOL);
   SetpValue(returnValue,EnvTrueSymbol(theEnv));
   return(TRUE);
  }

#endif /* (! RUN_TIME) && (! BLOAD_ONLY) */


