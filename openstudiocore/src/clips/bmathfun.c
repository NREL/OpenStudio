   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*             BASIC MATH FUNCTIONS MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for numerous basic math        */
/*   functions including +, *, -, /, integer, float, div,    */
/*   abs,set-auto-float-dividend, get-auto-float-dividend,   */
/*   min, and max.                                           */
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

#define _BMATHFUN_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "setup.h"

#include "argacces.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "router.h"

#include "bmathfun.h"

#define BMATHFUN_DATA 6

struct basicMathFunctionData
  { 
   intBool AutoFloatDividend;
  };

#define BasicMathFunctionData(theEnv) ((struct basicMathFunctionData *) GetEnvironmentData(theEnv,BMATHFUN_DATA))

/***************************************************************/
/* BasicMathFunctionDefinitions: Defines basic math functions. */
/***************************************************************/
globle void BasicMathFunctionDefinitions(
  void *theEnv)
  {   
   AllocateEnvironmentData(theEnv,BMATHFUN_DATA,sizeof(struct basicMathFunctionData),NULL);
   
   BasicMathFunctionData(theEnv)->AutoFloatDividend = TRUE;

#if ! RUN_TIME
   EnvDefineFunction2(theEnv,"+", 'n',PTIEF AdditionFunction, "AdditionFunction", "2*n");
   EnvDefineFunction2(theEnv,"*", 'n', PTIEF MultiplicationFunction, "MultiplicationFunction", "2*n");
   EnvDefineFunction2(theEnv,"-", 'n', PTIEF SubtractionFunction, "SubtractionFunction", "2*n");
    
   EnvDefineFunction2(theEnv,"/", 'n', PTIEF DivisionFunction, "DivisionFunction", "2*n");
   EnvDefineFunction2(theEnv,"div", 'g', PTIEF DivFunction, "DivFunction", "2*n");
   EnvDefineFunction2(theEnv,"set-auto-float-dividend", 'b',
                   SetAutoFloatDividendCommand, "SetAutoFloatDividendCommand", "11");
   EnvDefineFunction2(theEnv,"get-auto-float-dividend", 'b',
                  GetAutoFloatDividendCommand, "GetAutoFloatDividendCommand", "00");

   EnvDefineFunction2(theEnv,"integer", 'g', PTIEF IntegerFunction, "IntegerFunction", "11n");
   EnvDefineFunction2(theEnv,"float", 'd', PTIEF FloatFunction, "FloatFunction", "11n");
   EnvDefineFunction2(theEnv,"abs", 'n', PTIEF AbsFunction, "AbsFunction", "11n");
   EnvDefineFunction2(theEnv,"min", 'n', PTIEF MinFunction, "MinFunction", "2*n");
   EnvDefineFunction2(theEnv,"max", 'n', PTIEF MaxFunction, "MaxFunction", "2*n");
#endif
  }

/**********************************/
/* AdditionFunction: H/L access   */
/*   routine for the + function.  */
/**********************************/
globle void AdditionFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   double ftotal = 0.0;
   long long ltotal = 0LL;
   intBool useFloatTotal = FALSE;
   EXPRESSION *theExpression;
   DATA_OBJECT theArgument;
   int pos = 1;

   /*=================================================*/
   /* Loop through each of the arguments adding it to */
   /* a running total. If a floating point number is  */
   /* encountered, then do all subsequent operations  */
   /* using floating point values.                    */
   /*=================================================*/

   theExpression = GetFirstArgument();

   while (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"+",&theArgument,useFloatTotal,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (useFloatTotal)
        { ftotal += ValueToDouble(theArgument.value); }
      else
        {
         if (theArgument.type == INTEGER)
           { ltotal += ValueToLong(theArgument.value); }
         else
           {
            ftotal = (double) ltotal + ValueToDouble(theArgument.value);
            useFloatTotal = TRUE;
           }
        }

      pos++;
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   if (useFloatTotal)
     {
      returnValue->type = FLOAT;
      returnValue->value = (void *) EnvAddDouble(theEnv,ftotal);
     }
   else
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,ltotal);
     }
  }

/****************************************/
/* MultiplicationFunction: CLIPS access */
/*   routine for the * function.        */
/****************************************/
globle void MultiplicationFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   double ftotal = 1.0;
   long long ltotal = 1LL;
   intBool useFloatTotal = FALSE;
   EXPRESSION *theExpression;
   DATA_OBJECT theArgument;
   int pos = 1;

   /*===================================================*/
   /* Loop through each of the arguments multiplying it */
   /* by a running product. If a floating point number  */
   /* is encountered, then do all subsequent operations */
   /* using floating point values.                      */
   /*===================================================*/

   theExpression = GetFirstArgument();

   while (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"*",&theArgument,useFloatTotal,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (useFloatTotal)
        { ftotal *= ValueToDouble(theArgument.value); }
      else
        {
         if (theArgument.type == INTEGER)
           { ltotal *= ValueToLong(theArgument.value); }
         else
           {
            ftotal = (double) ltotal * ValueToDouble(theArgument.value);
            useFloatTotal = TRUE;
           }
        }
      pos++;
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   if (useFloatTotal)
     {
      returnValue->type = FLOAT;
      returnValue->value = (void *) EnvAddDouble(theEnv,ftotal);
     }
   else
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,ltotal);
     }
  }

/*************************************/
/* SubtractionFunction: CLIPS access */
/*   routine for the - function.     */
/*************************************/
globle void SubtractionFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   double ftotal = 0.0;
   long long ltotal = 0LL;
   intBool useFloatTotal = FALSE;
   EXPRESSION *theExpression;
   DATA_OBJECT theArgument;
   int pos = 1;

   /*=================================================*/
   /* Get the first argument. This number which will  */
   /* be the starting total from which all subsequent */
   /* arguments will subtracted.                      */
   /*=================================================*/

   theExpression = GetFirstArgument();
   if (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"-",&theArgument,useFloatTotal,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (theArgument.type == INTEGER)
        { ltotal = ValueToLong(theArgument.value); }
      else
        {
         ftotal = ValueToDouble(theArgument.value);
         useFloatTotal = TRUE;
        }
      pos++;
     }

   /*===================================================*/
   /* Loop through each of the arguments subtracting it */
   /* from a running total. If a floating point number  */
   /* is encountered, then do all subsequent operations */
   /* using floating point values.                      */
   /*===================================================*/

   while (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"-",&theArgument,useFloatTotal,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (useFloatTotal)
        { ftotal -= ValueToDouble(theArgument.value); }
      else
        {
         if (theArgument.type == INTEGER)
           { ltotal -= ValueToLong(theArgument.value); }
         else
           {
            ftotal = (double) ltotal - ValueToDouble(theArgument.value);
            useFloatTotal = TRUE;
           }
        }
      pos++;
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   if (useFloatTotal)
     {
      returnValue->type = FLOAT;
      returnValue->value = (void *) EnvAddDouble(theEnv,ftotal);
     }
   else
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,ltotal);
     }
  }

/***********************************/
/* DivisionFunction:  CLIPS access */
/*   routine for the / function.   */
/***********************************/
globle void DivisionFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   double ftotal = 1.0;
   long long ltotal = 1LL;
   intBool useFloatTotal;
   EXPRESSION *theExpression;
   DATA_OBJECT theArgument;
   int pos = 1;

   useFloatTotal = BasicMathFunctionData(theEnv)->AutoFloatDividend;
   
   /*===================================================*/
   /* Get the first argument. This number which will be */
   /* the starting product from which all subsequent    */
   /* arguments will divide. If the auto float dividend */
   /* feature is enable, then this number is converted  */
   /* to a float if it is an integer.                   */
   /*===================================================*/

   theExpression = GetFirstArgument();
   if (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"/",&theArgument,useFloatTotal,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (theArgument.type == INTEGER)
        { ltotal = ValueToLong(theArgument.value); }
      else
        {
         ftotal = ValueToDouble(theArgument.value);
         useFloatTotal = TRUE;
        }
      pos++;
     }

   /*====================================================*/
   /* Loop through each of the arguments dividing it     */
   /* into a running product. If a floating point number */
   /* is encountered, then do all subsequent operations  */
   /* using floating point values. Each argument is      */
   /* checked to prevent a divide by zero error.         */
   /*====================================================*/

   while (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"/",&theArgument,useFloatTotal,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if ((theArgument.type == INTEGER) ? (ValueToLong(theArgument.value) == 0L) :
                                 ((theArgument.type == FLOAT) ? ValueToDouble(theArgument.value) == 0.0 : FALSE))
        {
         DivideByZeroErrorMessage(theEnv,"/");
         SetHaltExecution(theEnv,TRUE);
         SetEvaluationError(theEnv,TRUE);
         returnValue->type = FLOAT;
         returnValue->value = (void *) EnvAddDouble(theEnv,1.0);
         return;
        }

      if (useFloatTotal)
        { ftotal /= ValueToDouble(theArgument.value); }
      else
        {
         if (theArgument.type == INTEGER)
           { ltotal /= ValueToLong(theArgument.value); }
         else
           {
            ftotal = (double) ltotal / ValueToDouble(theArgument.value);
            useFloatTotal = TRUE;
           }
        }
      pos++;
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   if (useFloatTotal)
     {
      returnValue->type = FLOAT;
      returnValue->value = (void *) EnvAddDouble(theEnv,ftotal);
     }
   else
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,ltotal);
     }
  }

/*************************************/
/* DivFunction: H/L access routine   */
/*   for the div function.           */
/*************************************/
globle long long DivFunction(
  void *theEnv)
  {
   long long total = 1LL;
   EXPRESSION *theExpression;
   DATA_OBJECT theArgument;
   int pos = 1;
   long long theNumber;

   /*===================================================*/
   /* Get the first argument. This number which will be */
   /* the starting product from which all subsequent    */
   /* arguments will divide.                            */
   /*===================================================*/

   theExpression = GetFirstArgument();
   if (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"div",&theArgument,FALSE,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (theArgument.type == INTEGER)
        { total = ValueToLong(theArgument.value); }
      else
        { total = (long long) ValueToDouble(theArgument.value); }
      pos++;
     }

   /*=====================================================*/
   /* Loop through each of the arguments dividing it into */
   /* a running product. Floats are converted to integers */
   /* and each argument is checked to prevent a divide by */
   /* zero error.                                         */
   /*=====================================================*/

   while (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"div",&theArgument,FALSE,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (theArgument.type == INTEGER) theNumber = ValueToLong(theArgument.value);
      else if (theArgument.type == FLOAT) theNumber = (long long) ValueToDouble(theArgument.value);
      else theNumber = 1;

      if (theNumber == 0LL)
        {
         DivideByZeroErrorMessage(theEnv,"div");
         SetHaltExecution(theEnv,TRUE);
         SetEvaluationError(theEnv,TRUE);
         return(1L);
        }

      if (theArgument.type == INTEGER)
        { total /= ValueToLong(theArgument.value); }
      else
        { total = total / (long long) ValueToDouble(theArgument.value); }

      pos++;
     }

   /*======================================================*/
   /* The result of the div function is always an integer. */
   /*======================================================*/

   return(total);
  }

/*****************************************************/
/* SetAutoFloatDividendCommand: H/L access routine   */
/*   for the set-auto-float-dividend command.        */
/*****************************************************/
globle int SetAutoFloatDividendCommand(
  void *theEnv)
  {
   int oldValue;
   DATA_OBJECT theArgument;

   /*===============================*/
   /* Remember the present setting. */
   /*===============================*/

   oldValue = BasicMathFunctionData(theEnv)->AutoFloatDividend;

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if (EnvArgCountCheck(theEnv,"set-auto-float-dividend",EXACTLY,1) == -1)
     { return(oldValue); }

   EnvRtnUnknown(theEnv,1,&theArgument);

   /*============================================================*/
   /* The symbol FALSE disables the auto float dividend feature. */
   /*============================================================*/

   if ((theArgument.value == EnvFalseSymbol(theEnv)) && (theArgument.type == SYMBOL))
     { BasicMathFunctionData(theEnv)->AutoFloatDividend = FALSE; }
   else
     { BasicMathFunctionData(theEnv)->AutoFloatDividend = TRUE; }

   /*======================================*/
   /* Return the old value of the feature. */
   /*======================================*/

   return(oldValue);
  }

/*****************************************************/
/* GetAutoFloatDividendCommand: H/L access routine   */
/*   for the get-auto-float-dividend command.        */
/*****************************************************/
globle int GetAutoFloatDividendCommand(
  void *theEnv)
  {
   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   EnvArgCountCheck(theEnv,"get-auto-float-dividend",EXACTLY,0);

   /*=============================*/
   /* Return the current setting. */
   /*=============================*/

   return(BasicMathFunctionData(theEnv)->AutoFloatDividend);
  }

/*************************************************/
/* EnvGetAutoFloatDividend: C access routine for */
/*   the get-auto-float-dividend command.        */
/*************************************************/
globle intBool EnvGetAutoFloatDividend(
  void *theEnv)
  {
   return(BasicMathFunctionData(theEnv)->AutoFloatDividend);
  }

/*************************************************/
/* EnvSetAutoFloatDividend: C access routine for */
/*   the set-auto-float-dividend command.        */
/*************************************************/
globle intBool EnvSetAutoFloatDividend(
  void *theEnv,
  int value)
  {
   int ov;

   ov = BasicMathFunctionData(theEnv)->AutoFloatDividend;
   BasicMathFunctionData(theEnv)->AutoFloatDividend = value;
   return(ov);
  }

/*****************************************/
/* IntegerFunction: H/L access routine   */
/*   for the integer function.           */
/*****************************************/
globle long long IntegerFunction(
  void *theEnv)
  {
   DATA_OBJECT valstruct;

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if (EnvArgCountCheck(theEnv,"integer",EXACTLY,1) == -1) return(0LL);

   /*================================================================*/
   /* Check for the correct type of argument. Note that ArgTypeCheck */
   /* will convert floats to integers when an integer is requested   */
   /* (which is the purpose of the integer function).                */
   /*================================================================*/

   if (EnvArgTypeCheck(theEnv,"integer",1,INTEGER,&valstruct) == FALSE) return(0LL);

   /*===================================================*/
   /* Return the numeric value converted to an integer. */
   /*===================================================*/

   return(ValueToLong(valstruct.value));
  }

/***************************************/
/* FloatFunction: H/L access routine   */
/*   for the float function.           */
/***************************************/
globle double FloatFunction(
  void *theEnv)
  {
   DATA_OBJECT valstruct;

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if (EnvArgCountCheck(theEnv,"float",EXACTLY,1) == -1) return(0.0);

   /*================================================================*/
   /* Check for the correct type of argument. Note that ArgTypeCheck */
   /* will convert integers to floats when a float is requested      */
   /* (which is the purpose of the float function).                  */
   /*================================================================*/

   if (EnvArgTypeCheck(theEnv,"float",1,FLOAT,&valstruct) == FALSE) return(0.0);

   /*================================================*/
   /* Return the numeric value converted to a float. */
   /*================================================*/

   return(ValueToDouble(valstruct.value));
  }

/*************************************/
/* AbsFunction: H/L access routine   */
/*   for the abs function.           */
/*************************************/
globle void AbsFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if (EnvArgCountCheck(theEnv,"abs",EXACTLY,1) == -1)
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,0L);
      return;
     }

   /*======================================*/
   /* Check that the argument is a number. */
   /*======================================*/

   if (EnvArgTypeCheck(theEnv,"abs",1,INTEGER_OR_FLOAT,returnValue) == FALSE)
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,0L);
      return;
     }

   /*==========================================*/
   /* Return the absolute value of the number. */
   /*==========================================*/

   if (returnValue->type == INTEGER)
     {
      if (ValueToLong(returnValue->value) < 0L)
        { returnValue->value = (void *) EnvAddLong(theEnv,- ValueToLong(returnValue->value)); }
     }
   else if (ValueToDouble(returnValue->value) < 0.0)
     { returnValue->value = (void *) EnvAddDouble(theEnv,- ValueToDouble(returnValue->value)); }
  }

/*************************************/
/* MinFunction: H/L access routine   */
/*   for the min function.           */
/*************************************/
globle void MinFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT argValue;
   int numberOfArguments, i;

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if ((numberOfArguments = EnvArgCountCheck(theEnv,"min",AT_LEAST,1)) == -1)
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,0L);
      return;
     }

   /*============================================*/
   /* Check that the first argument is a number. */
   /*============================================*/

   if (EnvArgTypeCheck(theEnv,"min",1,INTEGER_OR_FLOAT,returnValue) == FALSE)
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,0L);
      return;
     }

   /*===========================================================*/
   /* Loop through the remaining arguments, first checking each */
   /* argument to see that it is a number, and then determining */
   /* if the argument is less than the previous arguments and   */
   /* is thus the minimum value.                                */
   /*===========================================================*/

   for (i = 2 ; i <= numberOfArguments ; i++)
     {
      if (EnvArgTypeCheck(theEnv,"min",i,INTEGER_OR_FLOAT,&argValue) == FALSE) return;

      if (returnValue->type == INTEGER)
        {
         if (argValue.type == INTEGER)
           {
            if (ValueToLong(returnValue->value) > ValueToLong(argValue.value))
              {
               returnValue->type = argValue.type;
               returnValue->value = argValue.value;
              }
           }
         else
           {
            if ((double) ValueToLong(returnValue->value) >
                         ValueToDouble(argValue.value))
              {
               returnValue->type = argValue.type;
               returnValue->value = argValue.value;
              }
           }
        }
      else
        {
         if (argValue.type == INTEGER)
           {
            if (ValueToDouble(returnValue->value) >
                (double) ValueToLong(argValue.value))
              {
               returnValue->type = argValue.type;
               returnValue->value = argValue.value;
              }
           }
         else
           {
            if (ValueToDouble(returnValue->value) > ValueToDouble(argValue.value))
              {
               returnValue->type = argValue.type;
               returnValue->value = argValue.value;
              }
           }
        }
     }

   return;
  }

/*************************************/
/* MaxFunction: H/L access routine   */
/*   for the max function.           */
/*************************************/
globle void MaxFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT argValue;
   int numberOfArguments, i;

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if ((numberOfArguments = EnvArgCountCheck(theEnv,"max",AT_LEAST,1)) == -1)
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,0L);
      return;
     }

   /*============================================*/
   /* Check that the first argument is a number. */
   /*============================================*/

   if (EnvArgTypeCheck(theEnv,"max",1,INTEGER_OR_FLOAT,returnValue) == FALSE)
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,0L);
      return;
     }

   /*===========================================================*/
   /* Loop through the remaining arguments, first checking each */
   /* argument to see that it is a number, and then determining */
   /* if the argument is greater than the previous arguments    */
   /* and is thus the maximum value.                            */
   /*===========================================================*/

   for (i = 2 ; i <= numberOfArguments ; i++)
     {
      if (EnvArgTypeCheck(theEnv,"max",i,INTEGER_OR_FLOAT,&argValue) == FALSE) return;

      if (returnValue->type == INTEGER)
        {
         if (argValue.type == INTEGER)
           {
            if (ValueToLong(returnValue->value) < ValueToLong(argValue.value))
              {
               returnValue->type = argValue.type;
               returnValue->value = argValue.value;
              }
           }
         else
           {
            if ((double) ValueToLong(returnValue->value) <
                         ValueToDouble(argValue.value))
              {
               returnValue->type = argValue.type;
               returnValue->value = argValue.value;
              }
           }
        }
      else
        {
         if (argValue.type == INTEGER)
           {
            if (ValueToDouble(returnValue->value) <
                (double) ValueToLong(argValue.value))
              {
               returnValue->type = argValue.type;
               returnValue->value = argValue.value;
              }
           }
         else
           {
            if (ValueToDouble(returnValue->value) < ValueToDouble(argValue.value))
              {
               returnValue->type = argValue.type;
               returnValue->value = argValue.value;
              }
           }
        }
     }

   return;
  }

