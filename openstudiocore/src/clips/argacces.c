   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*               ARGUMENT ACCESS MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides access routines for accessing arguments */
/*   passed to user or system functions defined using the    */
/*   DefineFunction protocol.                                */
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
/*            Added IllegalLogicalNameMessage function.      */
/*                                                           */
/*************************************************************/

#define _ARGACCES_SOURCE_

#include "setup.h"

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "envrnmnt.h"
#include "extnfunc.h"
#include "router.h"
#include "cstrnchk.h"
#include "insfun.h"
#include "factmngr.h"
#include "prntutil.h"
#include "sysdep.h"

#include "argacces.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                    NonexistantError(void *,char *,char *,int);
   static void                    ExpectedTypeError3(void *,char *,char *,int,char *);

/*******************************************************************/
/* EnvRtnLexeme: Access function to retrieve the nth argument from */
/*   a user or system function defined using the DefineFunction    */
/*   protocol. The argument retrieved must be a symbol, string, or */
/*   instance name, otherwise an error is generated. Only the      */
/*   value of the argument is returned (i.e. the string "a" would  */
/*   be returned for a, "a", and [a]).                             */
/*******************************************************************/
globle char *EnvRtnLexeme(
  void *theEnv,
  int argumentPosition)
  {
   int count = 1;
   DATA_OBJECT result;
   struct expr *argPtr;

   /*=====================================================*/
   /* Find the appropriate argument in the argument list. */
   /*=====================================================*/

   for (argPtr = EvaluationData(theEnv)->CurrentExpression->argList;
        (argPtr != NULL) && (count < argumentPosition);
        argPtr = argPtr->nextArg)
     { count++; }

   if (argPtr == NULL)
     {
      NonexistantError(theEnv,"RtnLexeme",
                       ValueToString(ExpressionFunctionCallName(EvaluationData(theEnv)->CurrentExpression)),
                       argumentPosition);
      SetHaltExecution(theEnv,TRUE);
      SetEvaluationError(theEnv,TRUE);
      return(NULL);
     }

   /*============================================*/
   /* Return the value of the nth argument if it */
   /* is a symbol, string, or instance name.     */
   /*============================================*/

   EvaluateExpression(theEnv,argPtr,&result);

   if ((result.type == SYMBOL) ||
#if OBJECT_SYSTEM
       (result.type == INSTANCE_NAME) ||
#endif
       (result.type == STRING))
     { return(ValueToString(result.value));}

   /*======================================================*/
   /* Generate an error if the argument is the wrong type. */
   /*======================================================*/

   ExpectedTypeError3(theEnv,"RtnLexeme",
                  ValueToString(ExpressionFunctionCallName(EvaluationData(theEnv)->CurrentExpression)),
                  argumentPosition,"symbol, string, or instance name");
   SetHaltExecution(theEnv,TRUE);
   SetEvaluationError(theEnv,TRUE);
   return(NULL);
  }

/*******************************************************************/
/* EnvRtnDouble: Access function to retrieve the nth argument from */
/*   a user or system function defined using the DefineFunction    */
/*   protocol. The argument retrieved must be a either a float or  */
/*   an integer (type conversion to a float is performed for       */
/*   integers), otherwise an error is generated. Only the value of */
/*   the argument is returned (i.e. the float 3.0 would be         */
/*   returned for 3.0 and 3).                                      */
/*******************************************************************/
globle double EnvRtnDouble(
  void *theEnv,
  int argumentPosition)
  {
   int count = 1;
   DATA_OBJECT result;
   struct expr *argPtr;

   /*=====================================================*/
   /* Find the appropriate argument in the argument list. */
   /*=====================================================*/

   for (argPtr = EvaluationData(theEnv)->CurrentExpression->argList;
        (argPtr != NULL) && (count < argumentPosition);
        argPtr = argPtr->nextArg)
     { count++; }

   if (argPtr == NULL)
     {
      NonexistantError(theEnv,"RtnDouble",
                       ValueToString(ExpressionFunctionCallName(EvaluationData(theEnv)->CurrentExpression)),
                       argumentPosition);
      SetHaltExecution(theEnv,TRUE);
      SetEvaluationError(theEnv,TRUE);
      return(1.0);
     }

   /*======================================*/
   /* Return the value of the nth argument */
   /* if it is a float or integer.         */
   /*======================================*/

   EvaluateExpression(theEnv,argPtr,&result);

   if (result.type == FLOAT)
     { return(ValueToDouble(result.value)); }
   else if (result.type == INTEGER)
     { return((double) ValueToLong(result.value)); }

   /*======================================================*/
   /* Generate an error if the argument is the wrong type. */
   /*======================================================*/

   ExpectedTypeError3(theEnv,"RtnDouble",
                  ValueToString(ExpressionFunctionCallName(EvaluationData(theEnv)->CurrentExpression)),
                  argumentPosition,"number");
   SetHaltExecution(theEnv,TRUE);
   SetEvaluationError(theEnv,TRUE);
   return(1.0);
  }

/*****************************************************************/
/* EnvRtnLong: Access function to retrieve the nth argument from */
/*   a user or system function defined using the DefineFunction  */
/*   protocol. The argument retrieved must be a either a float   */
/*   or an integer (type conversion to an integer is performed   */
/*   for floats), otherwise an error is generated. Only the      */
/*   value of the argument is returned (i.e. the integer 4       */
/*   would be returned for 4.3 and 4).                           */
/*****************************************************************/
globle long long EnvRtnLong(
  void *theEnv,
  int argumentPosition)
  {
   int count = 1;
   DATA_OBJECT result;
   struct expr *argPtr;

   /*=====================================================*/
   /* Find the appropriate argument in the argument list. */
   /*=====================================================*/

   for (argPtr = EvaluationData(theEnv)->CurrentExpression->argList;
        (argPtr != NULL) && (count < argumentPosition);
        argPtr = argPtr->nextArg)
     { count++; }

   if (argPtr == NULL)
     {
      NonexistantError(theEnv,"RtnLong",
                       ValueToString(ExpressionFunctionCallName(EvaluationData(theEnv)->CurrentExpression)),
                       argumentPosition);
      SetHaltExecution(theEnv,TRUE);
      SetEvaluationError(theEnv,TRUE);
      return(1L);
     }

   /*======================================*/
   /* Return the value of the nth argument */
   /* if it is a float or integer.         */
   /*======================================*/

   EvaluateExpression(theEnv,argPtr,&result);

   if (result.type == FLOAT)
     { return((long) ValueToDouble(result.value)); }
   else if (result.type == INTEGER)
     { return(ValueToLong(result.value)); }

   /*======================================================*/
   /* Generate an error if the argument is the wrong type. */
   /*======================================================*/

   ExpectedTypeError3(theEnv,"RtnLong",
                  ValueToString(ExpressionFunctionCallName(EvaluationData(theEnv)->CurrentExpression)),
                  argumentPosition,"number");
   SetHaltExecution(theEnv,TRUE);
   SetEvaluationError(theEnv,TRUE);
   return(1L);
  }

/********************************************************************/
/* EnvRtnUnknown: Access function to retrieve the nth argument from */
/*   a user or system function defined using the DefineFunction     */
/*   protocol. The argument retrieved can be of any type. The value */
/*   and type of the argument are returned in a DATA_OBJECT         */
/*   structure provided by the calling function.                    */
/********************************************************************/
globle DATA_OBJECT_PTR EnvRtnUnknown(
  void *theEnv,
  int argumentPosition,
  DATA_OBJECT_PTR returnValue)
  {
   int count = 1;
   struct expr *argPtr;

   /*=====================================================*/
   /* Find the appropriate argument in the argument list. */
   /*=====================================================*/

   for (argPtr = EvaluationData(theEnv)->CurrentExpression->argList;
        (argPtr != NULL) && (count < argumentPosition);
        argPtr = argPtr->nextArg)
     { count++; }

   if (argPtr == NULL)
     {
      NonexistantError(theEnv,"RtnUnknown",
                       ValueToString(ExpressionFunctionCallName(EvaluationData(theEnv)->CurrentExpression)),
                       argumentPosition);
      SetHaltExecution(theEnv,TRUE);
      SetEvaluationError(theEnv,TRUE);
      return(NULL);
     }

   /*=======================================*/
   /* Return the value of the nth argument. */
   /*=======================================*/

   EvaluateExpression(theEnv,argPtr,returnValue);
   return(returnValue);
  }

/***********************************************************/
/* EnvRtnArgCount: Returns the length of the argument list */
/*   for the function call currently being evaluated.      */
/***********************************************************/
globle int EnvRtnArgCount(
  void *theEnv)
  {
   int count = 0;
   struct expr *argPtr;

   for (argPtr = EvaluationData(theEnv)->CurrentExpression->argList;
        argPtr != NULL;
        argPtr = argPtr->nextArg)
     { count++; }

   return(count);
  }
  
/************************************************************************/
/* EnvArgCountCheck: Given the expected number of arguments, determines */
/*   if the function currently being evaluated has the correct number   */
/*   of arguments. Three types of argument checking are provided by     */
/*   this function: 1) The function has exactly the expected number of  */
/*   arguments; 2) The function has at least the expected number of     */
/*   arguments; 3) The function has at most the expected number of      */
/*   arguments. The number of arguments is returned if no error occurs, */
/*   otherwise -1 is returned.                                          */
/************************************************************************/
globle int EnvArgCountCheck(
  void *theEnv,
  char *functionName,
  int countRelation,
  int expectedNumber)
  {
   int numberOfArguments;

   /*==============================================*/
   /* Get the number of arguments for the function */
   /* currently being evaluated.                   */
   /*==============================================*/

   numberOfArguments = EnvRtnArgCount(theEnv);

   /*=========================================================*/
   /* If the function satisfies expected number of arguments, */
   /* constraint, then return the number of arguments found.  */
   /*=========================================================*/

   if (countRelation == EXACTLY)
     { if (numberOfArguments == expectedNumber) return(numberOfArguments); }
   else if (countRelation == AT_LEAST)
     { if (numberOfArguments >= expectedNumber) return(numberOfArguments); }
   else if (countRelation == NO_MORE_THAN)
     { if (numberOfArguments <= expectedNumber) return(numberOfArguments); }

   /*================================================*/
   /* The correct number of arguments was not found. */
   /* Generate an error message and return -1.       */
   /*================================================*/

   ExpectedCountError(theEnv,functionName,countRelation,expectedNumber);

   SetHaltExecution(theEnv,TRUE);
   SetEvaluationError(theEnv,TRUE);

   return(-1);
  }

/****************************************************************/
/* EnvArgRangeCheck: Checks that the number of arguments passed */
/*   to a function falls within a specified minimum and maximum */
/*   range. The number of arguments passed to the function is   */
/*   returned if no error occurs, otherwise -1 is returned.     */
/****************************************************************/
globle int EnvArgRangeCheck(
  void *theEnv,
  char *functionName,
  int min,
  int max)
  {
   int numberOfArguments;

   numberOfArguments = EnvRtnArgCount(theEnv);
   if ((numberOfArguments < min) || (numberOfArguments > max))
     {
      PrintErrorID(theEnv,"ARGACCES",1,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Function ");
      EnvPrintRouter(theEnv,WERROR,functionName);
      EnvPrintRouter(theEnv,WERROR," expected at least ");
      PrintLongInteger(theEnv,WERROR,(long) min);
      EnvPrintRouter(theEnv,WERROR," and no more than ");
      PrintLongInteger(theEnv,WERROR,(long) max);
      EnvPrintRouter(theEnv,WERROR," arguments.\n");
      SetHaltExecution(theEnv,TRUE);
      SetEvaluationError(theEnv,TRUE);
      return(-1);
     }

   return(numberOfArguments);
  }

/*************************************************************/
/* EnvArgTypeCheck: Retrieves the nth argument passed to the */
/*   function call currently being evaluated and determines  */
/*   if it matches a specified type. Returns TRUE if the     */
/*   argument was successfully retrieved and is of the       */
/*   appropriate type, otherwise returns FALSE.              */
/*************************************************************/
globle int EnvArgTypeCheck(
  void *theEnv,
  char *functionName,
  int argumentPosition,
  int expectedType,
  DATA_OBJECT_PTR returnValue)
  {
   /*========================*/
   /* Retrieve the argument. */
   /*========================*/

   EnvRtnUnknown(theEnv,argumentPosition,returnValue);
   if (EvaluationData(theEnv)->EvaluationError) return(FALSE);

   /*========================================*/
   /* If the argument's type exactly matches */
   /* the expected type, then return TRUE.   */
   /*========================================*/

   if (returnValue->type == expectedType) return (TRUE);

   /*=============================================================*/
   /* Some expected types encompass more than one primitive type. */
   /* If the argument's type matches one of the primitive types   */
   /* encompassed by the expected type, then return TRUE.         */
   /*=============================================================*/

   if ((expectedType == INTEGER_OR_FLOAT) &&
       ((returnValue->type == INTEGER) || (returnValue->type == FLOAT)))
     { return(TRUE); }

   if ((expectedType == SYMBOL_OR_STRING) &&
       ((returnValue->type == SYMBOL) || (returnValue->type == STRING)))
     { return(TRUE); }

#if OBJECT_SYSTEM
   if (((expectedType == SYMBOL_OR_STRING) || (expectedType == SYMBOL)) &&
       (returnValue->type == INSTANCE_NAME))
     { return(TRUE); }

   if ((expectedType == INSTANCE_NAME) &&
       ((returnValue->type == INSTANCE_NAME) || (returnValue->type == SYMBOL)))
     { return(TRUE); }

   if ((expectedType == INSTANCE_OR_INSTANCE_NAME) &&
       ((returnValue->type == INSTANCE_ADDRESS) ||
        (returnValue->type == INSTANCE_NAME) ||
        (returnValue->type == SYMBOL)))
     { return(TRUE); }
#endif

   /*===========================================================*/
   /* If the expected type is float and the argument's type is  */
   /* integer (or vice versa), then convert the argument's type */
   /* to match the expected type and then return TRUE.          */
   /*===========================================================*/

   if ((returnValue->type == INTEGER) && (expectedType == FLOAT))
     {
      returnValue->type = FLOAT;
      returnValue->value = (void *) EnvAddDouble(theEnv,(double) ValueToLong(returnValue->value));
      return(TRUE);
     }

   if ((returnValue->type == FLOAT) && (expectedType == INTEGER))
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,(long long) ValueToDouble(returnValue->value));
      return(TRUE);
     }

   /*=====================================================*/
   /* The argument's type didn't match the expected type. */
   /* Print an error message and return FALSE.            */
   /*=====================================================*/

   if (expectedType == FLOAT) ExpectedTypeError1(theEnv,functionName,argumentPosition,"float");
   else if (expectedType == INTEGER) ExpectedTypeError1(theEnv,functionName,argumentPosition,"integer");
   else if (expectedType == SYMBOL) ExpectedTypeError1(theEnv,functionName,argumentPosition,"symbol");
   else if (expectedType == STRING) ExpectedTypeError1(theEnv,functionName,argumentPosition,"string");
   else if (expectedType == MULTIFIELD) ExpectedTypeError1(theEnv,functionName,argumentPosition,"multifield");
   else if (expectedType == INTEGER_OR_FLOAT)  ExpectedTypeError1(theEnv,functionName,argumentPosition,"integer or float");
   else if (expectedType == SYMBOL_OR_STRING) ExpectedTypeError1(theEnv,functionName,argumentPosition,"symbol or string");
#if OBJECT_SYSTEM
   else if (expectedType == INSTANCE_NAME) ExpectedTypeError1(theEnv,functionName,argumentPosition,"instance name");
   else if (expectedType == INSTANCE_ADDRESS) ExpectedTypeError1(theEnv,functionName,argumentPosition,"instance address");
   else if (expectedType == INSTANCE_OR_INSTANCE_NAME) ExpectedTypeError1(theEnv,functionName,argumentPosition,"instance address or instance name");
#endif

   SetHaltExecution(theEnv,TRUE);
   SetEvaluationError(theEnv,TRUE);

   return(FALSE);
  }

/******************************************************************/
/* GetNumericArgument: Evaluates an expression to yield a numeric */
/*  argument. This provides quicker retrieval than using some of  */
/*  the other argument access routines. The numeric argument is   */
/*  returned in a DATA_OBJECT supplied by the calling function.   */
/*  TRUE is returned if a numeric argument was successfully       */
/*  retrieved, otherwise FALSE is returned.                       */
/******************************************************************/
globle intBool GetNumericArgument(
  void *theEnv,
  struct expr *theArgument,
  char *functionName,
  DATA_OBJECT *result,
  intBool convertToFloat,
  int whichArgument)
  {
   unsigned short theType;
   void *theValue;

   /*==================================================================*/
   /* Evaluate the expression (don't bother calling EvaluateExpression */
   /* if the type is float or integer).                                */
   /*==================================================================*/

   switch(theArgument->type)
     {
      case FLOAT:
      case INTEGER:
        theType = theArgument->type;
        theValue = theArgument->value;
        break;

      default:
        EvaluateExpression(theEnv,theArgument,result);
        theType = result->type;
        theValue = result->value;
        break;
     }

   /*==========================================*/
   /* If the argument is not float or integer, */
   /* print an error message and return FALSE. */
   /*==========================================*/

   if ((theType != FLOAT) && (theType != INTEGER))
     {
      ExpectedTypeError1(theEnv,functionName,whichArgument,"integer or float");
      SetHaltExecution(theEnv,TRUE);
      SetEvaluationError(theEnv,TRUE);
      result->type = INTEGER;
      result->value = (void *) EnvAddLong(theEnv,0LL);
      return(FALSE);
     }

   /*==========================================================*/
   /* If the argument is an integer and the "convert to float" */
   /* flag is TRUE, then convert the integer to a float.       */
   /*==========================================================*/

   if ((convertToFloat) && (theType == INTEGER))
     {
      theType = FLOAT;
      theValue = (void *) EnvAddDouble(theEnv,(double) ValueToLong(theValue));
     }

   /*============================================================*/
   /* The numeric argument was successfully retrieved. Store the */
   /* argument in the user supplied DATA_OBJECT and return TRUE. */
   /*============================================================*/

   result->type = theType;
   result->value = theValue;

   return(TRUE);
  }

/*********************************************************************/
/* GetLogicalName: Retrieves the nth argument passed to the function */
/*   call currently being evaluated and determines if it is a valid  */
/*   logical name. If valid, the logical name is returned, otherwise */
/*   NULL is returned.                                               */
/*********************************************************************/
globle char *GetLogicalName(
  void *theEnv,
  int whichArgument,
  char *defaultLogicalName)
  {
   char *logicalName;
   DATA_OBJECT result;

   EnvRtnUnknown(theEnv,whichArgument,&result);

   if ((GetType(result) == SYMBOL) ||
       (GetType(result) == STRING) ||
       (GetType(result) == INSTANCE_NAME))
     {
      logicalName = ValueToString(result.value);
      if ((strcmp(logicalName,"t") == 0) || (strcmp(logicalName,"T") == 0))
        { logicalName = defaultLogicalName; }
     }
   else if (GetType(result) == FLOAT)
     {
      logicalName = ValueToString(EnvAddSymbol(theEnv,FloatToString(theEnv,DOToDouble(result))));
     }
   else if (GetType(result) == INTEGER)
     {
      logicalName = ValueToString(EnvAddSymbol(theEnv,LongIntegerToString(theEnv,DOToLong(result))));
     }
   else
     { logicalName = NULL; }

   return(logicalName);
  }

/************************************************************/
/* GetFileName: Retrieves the nth argument passed to the    */
/*   function call currently being evaluated and determines */
/*   if it is a valid file name. If valid, the file name is */
/*   returned, otherwise NULL is returned.                  */
/************************************************************/
globle char *GetFileName(
  void *theEnv,
  char *functionName,
  int whichArgument)
  {
   DATA_OBJECT result;

   EnvRtnUnknown(theEnv,whichArgument,&result);
   if ((GetType(result) != STRING) && (GetType(result) != SYMBOL))
     {
      ExpectedTypeError1(theEnv,functionName,whichArgument,"file name");
      return(NULL);
     }

   return(DOToString(result));
  }

/******************************************************************/
/* OpenErrorMessage: Generalized error message for opening files. */
/******************************************************************/
globle void OpenErrorMessage(
  void *theEnv,
  char *functionName,
  char *fileName)
  {
   PrintErrorID(theEnv,"ARGACCES",2,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Function ");
   EnvPrintRouter(theEnv,WERROR,functionName);
   EnvPrintRouter(theEnv,WERROR," was unable to open file ");
   EnvPrintRouter(theEnv,WERROR,fileName);
   EnvPrintRouter(theEnv,WERROR,".\n");
  }

/************************************************************/
/* GetModuleName: Retrieves the nth argument passed to the  */
/*   function call currently being evaluated and determines */
/*   if it is a valid module name. If valid, the module     */
/*   name is returned or NULL is returned to indicate all   */
/*   modules.                                               */
/************************************************************/
globle struct defmodule *GetModuleName(
  void *theEnv,
  char *functionName,
  int whichArgument,
  int *error)
  {
   DATA_OBJECT result;
   struct defmodule *theModule;

   *error = FALSE;

   /*========================*/
   /* Retrieve the argument. */
   /*========================*/

   EnvRtnUnknown(theEnv,whichArgument,&result);

   /*=================================*/
   /* A module name must be a symbol. */
   /*=================================*/

   if (GetType(result) != SYMBOL)
     {
      ExpectedTypeError1(theEnv,functionName,whichArgument,"defmodule name");
      *error = TRUE;
      return(NULL);
     }

   /*=======================================*/
   /* Check to see that the symbol actually */
   /* corresponds to a defined module.      */
   /*=======================================*/

   if ((theModule = (struct defmodule *) EnvFindDefmodule(theEnv,DOToString(result))) == NULL)
     {
      if (strcmp("*",DOToString(result)) != 0)
        {
         ExpectedTypeError1(theEnv,functionName,whichArgument,"defmodule name");
         *error = TRUE;
        }
      return(NULL);
     }

   /*=================================*/
   /* Return a pointer to the module. */
   /*=================================*/

   return(theModule);
  }

/****************************************************************/
/* GetConstructName: Retrieves the 1st argument passed to the   */
/*   function call currently being evaluated and determines if  */
/*   it is a valid name for a construct. Also checks that the   */
/*   function is only passed a single argument. This routine    */
/*   is used by functions such as ppdeftemplate, undefrule,     */
/*   etc... to retrieve the construct name on which to operate. */
/****************************************************************/
globle char *GetConstructName(
  void *theEnv,
  char *functionName,
  char *constructType)
  {
   DATA_OBJECT result;

   if (EnvRtnArgCount(theEnv) != 1)
     {
      ExpectedCountError(theEnv,functionName,EXACTLY,1);
      return(NULL);
     }

   EnvRtnUnknown(theEnv,1,&result);

   if (GetType(result) != SYMBOL)
     {
      ExpectedTypeError1(theEnv,functionName,1,constructType);
      return(NULL);
     }

   return(DOToString(result));
  }

/**************************************************************************/
/* NonexistantError: Prints the error message for a nonexistant argument. */
/**************************************************************************/
static void NonexistantError(
  void *theEnv,
  char *accessFunction,
  char *functionName,
  int argumentPosition)
  {
   PrintErrorID(theEnv,"ARGACCES",3,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Function ");
   EnvPrintRouter(theEnv,WERROR,accessFunction);
   EnvPrintRouter(theEnv,WERROR," received a request from function ");
   EnvPrintRouter(theEnv,WERROR,functionName);
   EnvPrintRouter(theEnv,WERROR," for argument #");
   PrintLongInteger(theEnv,WERROR,(long int) argumentPosition);
   EnvPrintRouter(theEnv,WERROR," which is non-existent\n");
  }

/*********************************************************/
/* ExpectedCountError: Prints the error message for an   */
/*   incorrect number of arguments passed to a function. */
/*********************************************************/
globle void ExpectedCountError(
  void *theEnv,
  char *functionName,
  int countRelation,
  int expectedNumber)
  {
   PrintErrorID(theEnv,"ARGACCES",4,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Function ");
   EnvPrintRouter(theEnv,WERROR,functionName);

   if (countRelation == EXACTLY)
     { EnvPrintRouter(theEnv,WERROR," expected exactly "); }
   else if (countRelation == AT_LEAST)
     { EnvPrintRouter(theEnv,WERROR," expected at least "); }
   else if (countRelation == NO_MORE_THAN)
     { EnvPrintRouter(theEnv,WERROR," expected no more than "); }
   else
     { EnvPrintRouter(theEnv,WERROR," generated an illegal argument check for "); }

   PrintLongInteger(theEnv,WERROR,(long int) expectedNumber);
   EnvPrintRouter(theEnv,WERROR," argument(s)\n");
  }

/*************************************************************/
/*  NAME         : CheckFunctionArgCount                     */
/*  DESCRIPTION  : Checks the number of arguments against    */
/*                 the system function restriction list      */
/*  INPUTS       : 1) Name of the calling function           */
/*                 2) The restriction list can be NULL       */
/*                 3) The number of arguments                */
/*  RETURNS      : TRUE if OK, FALSE otherwise               */
/*  SIDE EFFECTS : EvaluationError set on errrors            */
/*  NOTES        : Used to check generic function implicit   */
/*                 method (system function) calls and system */
/*                 function calls which have the sequence    */
/*                 expansion operator in their argument list */
/*************************************************************/
globle intBool CheckFunctionArgCount(
  void *theEnv,
  char *functionName,
  char *restrictions,
  int argumentCount)
  {
   register int minArguments, maxArguments;
   char theChar[2];

   theChar[0] = '0';
   theChar[1] = EOS;

   /*=====================================================*/
   /* If there are no restrictions, then there is no need */
   /* to check for the correct number of arguments.       */
   /*=====================================================*/

   if (restrictions == NULL) return(TRUE);

   /*===========================================*/
   /* Determine the minimum number of arguments */
   /* required by the function.                 */
   /*===========================================*/

   if (isdigit(restrictions[0]))
     {
      theChar[0] = restrictions[0];
      minArguments = atoi(theChar);
     }
   else
     { minArguments = -1; }

   /*===========================================*/
   /* Determine the maximum number of arguments */
   /* required by the function.                 */
   /*===========================================*/

   if (isdigit(restrictions[1]))
     {
      theChar[0] = restrictions[1];
      maxArguments = atoi(theChar);
     }
   else
     { maxArguments = 10000; }

   /*==============================================*/
   /* If the function expects exactly N arguments, */
   /* then check to see if there are N arguments.  */
   /*==============================================*/

   if (minArguments == maxArguments)
     {
      if (argumentCount != minArguments)
        {
         ExpectedCountError(theEnv,functionName,EXACTLY,minArguments);
         SetEvaluationError(theEnv,TRUE);
         return(FALSE);
        }
      return(TRUE);
     }

   /*==================================*/
   /* Check to see if there were fewer */
   /* arguments passed than expected.  */
   /*==================================*/

   if (argumentCount < minArguments)
     {
      ExpectedCountError(theEnv,functionName,AT_LEAST,minArguments);
      SetEvaluationError(theEnv,TRUE);
      return(FALSE);
     }

   /*=================================*/
   /* Check to see if there were more */
   /* arguments passed than expected. */
   /*=================================*/

   if (argumentCount > maxArguments)
     {
      ExpectedCountError(theEnv,functionName,NO_MORE_THAN,maxArguments);
      SetEvaluationError(theEnv,TRUE);
      return(FALSE);
     }

   /*===============================*/
   /* The number of arguments falls */
   /* within the expected range.    */
   /*===============================*/

   return(TRUE);
  }

/*******************************************************************/
/* ExpectedTypeError1: Prints the error message for the wrong type */
/*   of argument passed to a user or system defined function. The  */
/*   expected type is passed as a string to this function.         */
/*******************************************************************/
globle void ExpectedTypeError1(
  void *theEnv,
  char *functionName,
  int whichArg,
  char *expectedType)
  {
   PrintErrorID(theEnv,"ARGACCES",5,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Function ");
   EnvPrintRouter(theEnv,WERROR,functionName);
   EnvPrintRouter(theEnv,WERROR," expected argument #");
   PrintLongInteger(theEnv,WERROR,(long int) whichArg);
   EnvPrintRouter(theEnv,WERROR," to be of type ");
   EnvPrintRouter(theEnv,WERROR,expectedType);
   EnvPrintRouter(theEnv,WERROR,"\n");
  }

/**************************************************************/
/* ExpectedTypeError2: Prints the error message for the wrong */
/*   type of argument passed to a user or system defined      */
/*   function. The expected type is derived by examining the  */
/*   function's argument restriction list.                    */
/**************************************************************/
globle void ExpectedTypeError2(
  void *theEnv,
  char *functionName,
  int whichArg)
  {
   struct FunctionDefinition *theFunction;
   char *theType;

   theFunction = FindFunction(theEnv,functionName);

   if (theFunction == NULL) return;

   theType = GetArgumentTypeName(GetNthRestriction(theFunction,whichArg));

   ExpectedTypeError1(theEnv,functionName,whichArg,theType);
  }

/*******************************************************************/
/* ExpectedTypeError3: Prints the error message for the wrong type */
/*   of argument passed to a user or system defined function when  */
/*   the argument was requested by calling RtnLexeme, RtnLong, or  */
/*   RtnDouble.                                                    */
/*******************************************************************/
static void ExpectedTypeError3(
  void *theEnv,
  char *accessFunction,
  char *functionName,
  int argumentPosition,
  char *type)
  {
   PrintErrorID(theEnv,"ARGACCES",6,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Function ");
   EnvPrintRouter(theEnv,WERROR,accessFunction);
   EnvPrintRouter(theEnv,WERROR," received a request from function ");
   EnvPrintRouter(theEnv,WERROR,functionName);
   EnvPrintRouter(theEnv,WERROR," for argument #");
   PrintLongInteger(theEnv,WERROR,(long int) argumentPosition);
   EnvPrintRouter(theEnv,WERROR," which is not of type ");
   EnvPrintRouter(theEnv,WERROR,type);
   EnvPrintRouter(theEnv,WERROR,"\n");
  }

/***************************************************/
/* GetFactOrInstanceArgument: Utility routine for  */
/*   retrieving a fact or instance argument        */
/***************************************************/
void *GetFactOrInstanceArgument(
  void *theEnv,
  int thePosition,
  DATA_OBJECT *item,
  char *functionName)
  {
#if DEFTEMPLATE_CONSTRUCT || OBJECT_SYSTEM
   void *ptr;
#endif

   /*==============================*/
   /* Retrieve the first argument. */
   /*==============================*/

   EnvRtnUnknown(theEnv,thePosition,item);

   /*==================================================*/
   /* Fact and instance addresses are valid arguments. */
   /*==================================================*/

   if ((GetpType(item) == FACT_ADDRESS) ||
       (GetpType(item) == INSTANCE_ADDRESS))
     { return(GetpValue(item)); }

   /*==================================================*/
   /* An integer is a valid argument if it corresponds */
   /* to the fact index of an existing fact.           */
   /*==================================================*/

#if DEFTEMPLATE_CONSTRUCT
   else if (GetpType(item) == INTEGER)
     {
      if ((ptr = (void *) FindIndexedFact(theEnv,DOPToLong(item))) == NULL)
        {
         char tempBuffer[20];
         gensprintf(tempBuffer,"f-%lld",DOPToLong(item));
         CantFindItemErrorMessage(theEnv,"fact",tempBuffer);
        }
      return(ptr);
     }
#endif

   /*================================================*/
   /* Instance names and symbols are valid arguments */
   /* if they correspond to an existing instance.    */
   /*================================================*/

#if OBJECT_SYSTEM
   else if ((GetpType(item) == INSTANCE_NAME) || (GetpType(item) == SYMBOL))
     {
      if ((ptr = (void *) FindInstanceBySymbol(theEnv,(SYMBOL_HN *) GetpValue(item))) == NULL)
        {
         CantFindItemErrorMessage(theEnv,"instance",ValueToString(GetpValue(item)));
        }
      return(ptr);
     }
#endif

   /*========================================*/
   /* Any other type is an invalid argument. */
   /*========================================*/

   ExpectedTypeError2(theEnv,functionName,thePosition);
   return(NULL);
  }

/****************************************************/
/* IllegalLogicalNameMessage: Generic error message */
/*   for illegal logical names.                     */
/****************************************************/
void IllegalLogicalNameMessage(
  void *theEnv,
  char *theFunction)
  {
   PrintErrorID(theEnv,"IOFUN",1,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Illegal logical name used for ");
   EnvPrintRouter(theEnv,WERROR,theFunction);
   EnvPrintRouter(theEnv,WERROR," function.\n");
  }
