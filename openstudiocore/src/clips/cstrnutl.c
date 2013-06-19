   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  07/01/05            */
   /*                                                     */
   /*             CONSTRAINT UTILITY MODULE               */
   /*******************************************************/

/*************************************************************/
/* Purpose: Utility routines for manipulating, initializing, */
/*   creating, copying, and comparing constraint records.    */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian Dantes                                         */
/*                                                           */
/* Revision History:                                         */
/*      6.24: Added allowed-classes slot facet.              */
/*                                                           */
/*************************************************************/

#define _CSTRNUTL_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>

#include "setup.h"

#include "constant.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "router.h"
#include "extnfunc.h"
#include "scanner.h"
#include "multifld.h"
#include "argacces.h"

#include "cstrnutl.h"

/************************************************/
/* GetConstraintRecord: Creates and initializes */
/*   the values of a constraint record.         */
/************************************************/
globle struct constraintRecord *GetConstraintRecord(
  void *theEnv)
  {
   CONSTRAINT_RECORD *constraints;
   unsigned i;

   constraints = get_struct(theEnv,constraintRecord);

   for (i = 0 ; i < sizeof(CONSTRAINT_RECORD) ; i++)
     { ((char *) constraints)[i] = '\0'; }

   SetAnyAllowedFlags(constraints,TRUE);

   constraints->multifieldsAllowed = FALSE;
   constraints->singlefieldsAllowed = TRUE;

   constraints->anyRestriction = FALSE;
   constraints->symbolRestriction = FALSE;
   constraints->stringRestriction = FALSE;
   constraints->floatRestriction = FALSE;
   constraints->integerRestriction = FALSE;
   constraints->classRestriction = FALSE;
   constraints->instanceNameRestriction = FALSE;
   constraints->classList = NULL;
   constraints->restrictionList = NULL;
   constraints->minValue = GenConstant(theEnv,SYMBOL,SymbolData(theEnv)->NegativeInfinity);
   constraints->maxValue = GenConstant(theEnv,SYMBOL,SymbolData(theEnv)->PositiveInfinity);
   constraints->minFields = GenConstant(theEnv,INTEGER,SymbolData(theEnv)->Zero);
   constraints->maxFields = GenConstant(theEnv,SYMBOL,SymbolData(theEnv)->PositiveInfinity);
   constraints->bucket = -1;
   constraints->count = 0;
   constraints->multifield = NULL;
   constraints->next = NULL;

   return(constraints);
  }

/********************************************************/
/* SetAnyAllowedFlags: Sets the allowed type flags of a */
/*   constraint record to allow all types. If passed an */
/*   argument of TRUE, just the "any allowed" flag is   */
/*   set to TRUE. If passed an argument of FALSE, then  */
/*   all of the individual type flags are set to TRUE.  */
/********************************************************/
globle void SetAnyAllowedFlags(
  CONSTRAINT_RECORD *theConstraint,
  int justOne)
  {
   int flag1, flag2;

   if (justOne)
     {
      flag1 = TRUE;
      flag2 = FALSE;
     }
   else
     {
      flag1 = FALSE;
      flag2 = TRUE;
     }

   theConstraint->anyAllowed = flag1;
   theConstraint->symbolsAllowed = flag2;
   theConstraint->stringsAllowed = flag2;
   theConstraint->floatsAllowed = flag2;
   theConstraint->integersAllowed = flag2;
   theConstraint->instanceNamesAllowed = flag2;
   theConstraint->instanceAddressesAllowed = flag2;
   theConstraint->externalAddressesAllowed = flag2;
   theConstraint->voidAllowed = flag2;
   theConstraint->factAddressesAllowed = flag2;
  }

/*****************************************************/
/* CopyConstraintRecord: Copies a constraint record. */
/*****************************************************/
globle struct constraintRecord *CopyConstraintRecord(
  void *theEnv,
  CONSTRAINT_RECORD *sourceConstraint)
  {
   CONSTRAINT_RECORD *theConstraint;

   if (sourceConstraint == NULL) return(NULL);

   theConstraint = get_struct(theEnv,constraintRecord);

   theConstraint->anyAllowed = sourceConstraint->anyAllowed;
   theConstraint->symbolsAllowed = sourceConstraint->symbolsAllowed;
   theConstraint->stringsAllowed = sourceConstraint->stringsAllowed;
   theConstraint->floatsAllowed = sourceConstraint->floatsAllowed;
   theConstraint->integersAllowed = sourceConstraint->integersAllowed;
   theConstraint->instanceNamesAllowed = sourceConstraint->instanceNamesAllowed;
   theConstraint->instanceAddressesAllowed = sourceConstraint->instanceAddressesAllowed;
   theConstraint->externalAddressesAllowed = sourceConstraint->externalAddressesAllowed;
   theConstraint->voidAllowed = sourceConstraint->voidAllowed;
   theConstraint->multifieldsAllowed = sourceConstraint->multifieldsAllowed;
   theConstraint->singlefieldsAllowed = sourceConstraint->singlefieldsAllowed;
   theConstraint->factAddressesAllowed = sourceConstraint->factAddressesAllowed;
   theConstraint->anyRestriction = sourceConstraint->anyRestriction;
   theConstraint->symbolRestriction = sourceConstraint->symbolRestriction;
   theConstraint->stringRestriction = sourceConstraint->stringRestriction;
   theConstraint->floatRestriction = sourceConstraint->floatRestriction;
   theConstraint->integerRestriction = sourceConstraint->integerRestriction;
   theConstraint->classRestriction = sourceConstraint->classRestriction;
   theConstraint->instanceNameRestriction = sourceConstraint->instanceNameRestriction;
   theConstraint->classList = CopyExpression(theEnv,sourceConstraint->classList);
   theConstraint->restrictionList = CopyExpression(theEnv,sourceConstraint->restrictionList);
   theConstraint->minValue = CopyExpression(theEnv,sourceConstraint->minValue);
   theConstraint->maxValue = CopyExpression(theEnv,sourceConstraint->maxValue);
   theConstraint->minFields = CopyExpression(theEnv,sourceConstraint->minFields);
   theConstraint->maxFields = CopyExpression(theEnv,sourceConstraint->maxFields);
   theConstraint->bucket = -1;
   theConstraint->count = 0;
   theConstraint->multifield = CopyConstraintRecord(theEnv,sourceConstraint->multifield);
   theConstraint->next = NULL;

   return(theConstraint);
  }

#if (! RUN_TIME) && (! BLOAD_ONLY)

/**************************************************************/
/* SetAnyRestrictionFlags: Sets the restriction type flags of */
/*   a constraint record to indicate there are restriction on */
/*   all types. If passed an argument of TRUE, just the       */
/*   "any restriction" flag is set to TRUE. If passed an      */
/*   argument of FALSE, then all of the individual type       */
/*   restriction flags are set to TRUE.                       */
/**************************************************************/
globle void SetAnyRestrictionFlags(
  CONSTRAINT_RECORD *theConstraint,
  int justOne)
  {
   int flag1, flag2;

   if (justOne)
     {
      flag1 = TRUE;
      flag2 = FALSE;
     }
   else
     {
      flag1 = FALSE;
      flag2 = TRUE;
     }

   theConstraint->anyRestriction = flag1;
   theConstraint->symbolRestriction = flag2;
   theConstraint->stringRestriction = flag2;
   theConstraint->floatRestriction = flag2;
   theConstraint->integerRestriction = flag2;
   theConstraint->instanceNameRestriction = flag2;
  }

/*****************************************************/
/* SetConstraintType: Given a constraint type and a  */
/*   constraint, sets the allowed type flags for the */
/*   specified type in the constraint to TRUE.       */
/*****************************************************/
globle int SetConstraintType(
  int theType,
  CONSTRAINT_RECORD *constraints)
  {
   int rv = TRUE;

   switch(theType)
     {
      case UNKNOWN_VALUE:
         rv = constraints->anyAllowed;
         constraints->anyAllowed = TRUE;
         break;

      case SYMBOL:
         rv = constraints->symbolsAllowed;
         constraints->symbolsAllowed = TRUE;
         break;

      case STRING:
         rv = constraints->stringsAllowed;
         constraints->stringsAllowed = TRUE;
         break;

      case SYMBOL_OR_STRING:
         rv = (constraints->stringsAllowed | constraints->symbolsAllowed);
         constraints->symbolsAllowed = TRUE;
         constraints->stringsAllowed = TRUE;
         break;

      case INTEGER:
         rv = constraints->integersAllowed;
         constraints->integersAllowed = TRUE;
         break;

      case FLOAT:
         rv = constraints->floatsAllowed;
         constraints->floatsAllowed = TRUE;
         break;

      case INTEGER_OR_FLOAT:
         rv = (constraints->integersAllowed | constraints->floatsAllowed);
         constraints->integersAllowed = TRUE;
         constraints->floatsAllowed = TRUE;
         break;

      case INSTANCE_ADDRESS:
         rv = constraints->instanceAddressesAllowed;
         constraints->instanceAddressesAllowed = TRUE;
         break;

      case INSTANCE_NAME:
         rv = constraints->instanceNamesAllowed;
         constraints->instanceNamesAllowed = TRUE;
         break;

      case INSTANCE_OR_INSTANCE_NAME:
         rv = (constraints->instanceNamesAllowed | constraints->instanceAddressesAllowed);
         constraints->instanceNamesAllowed = TRUE;
         constraints->instanceAddressesAllowed = TRUE;
         break;

      case EXTERNAL_ADDRESS:
         rv = constraints->externalAddressesAllowed;
         constraints->externalAddressesAllowed = TRUE;
         break;

      case RVOID:
         rv = constraints->voidAllowed;
         constraints->voidAllowed = TRUE;
         break;

      case FACT_ADDRESS:
         rv = constraints->factAddressesAllowed;
         constraints->factAddressesAllowed = TRUE;
         break;

      case MULTIFIELD:
         rv = constraints->multifieldsAllowed;
         constraints->multifieldsAllowed = TRUE;
         break;
     }

   if (theType != UNKNOWN_VALUE) constraints->anyAllowed = FALSE;
   return(rv);
  }

#endif /* (! RUN_TIME) && (! BLOAD_ONLY) */

/*************************************************************/
/* CompareNumbers: Given two numbers (which can be integers, */
/*   floats, or the symbols for positive/negative infinity)  */
/*   returns the relationship between the numbers (greater   */
/*   than, less than or equal).                              */
/*************************************************************/
globle int CompareNumbers(
  void *theEnv,
  int type1,
  void *vptr1,
  int type2,
  void *vptr2)
  {
   /*============================================*/
   /* Handle the situation in which the values   */
   /* are exactly equal (same type, same value). */
   /*============================================*/

   if (vptr1 == vptr2) return(EQUAL);

   /*=======================================*/
   /* Handle the special cases for positive */
   /* and negative infinity.                */
   /*=======================================*/

   if (vptr1 == SymbolData(theEnv)->PositiveInfinity) return(GREATER_THAN);

   if (vptr1 == SymbolData(theEnv)->NegativeInfinity) return(LESS_THAN);

   if (vptr2 == SymbolData(theEnv)->PositiveInfinity) return(LESS_THAN);

   if (vptr2 == SymbolData(theEnv)->NegativeInfinity) return(GREATER_THAN);

   /*=======================*/
   /* Compare two integers. */
   /*=======================*/

   if ((type1 == INTEGER) && (type2 == INTEGER))
     {
      if (ValueToLong(vptr1) < ValueToLong(vptr2))
        { return(LESS_THAN); }
      else if (ValueToLong(vptr1) > ValueToLong(vptr2))
        { return(GREATER_THAN); }

      return(EQUAL);
     }

   /*=====================*/
   /* Compare two floats. */
   /*=====================*/

   if ((type1 == FLOAT) && (type2 == FLOAT))
     {
      if (ValueToDouble(vptr1) < ValueToDouble(vptr2))
        { return(LESS_THAN); }
      else if (ValueToDouble(vptr1) > ValueToDouble(vptr2))
        { return(GREATER_THAN); }

      return(EQUAL);
     }

   /*================================*/
   /* Compare an integer to a float. */
   /*================================*/

   if ((type1 == INTEGER) && (type2 == FLOAT))
     {
      if (((double) ValueToLong(vptr1)) < ValueToDouble(vptr2))
        { return(LESS_THAN); }
      else if (((double) ValueToLong(vptr1)) > ValueToDouble(vptr2))
        { return(GREATER_THAN); }

      return(EQUAL);
     }

   /*================================*/
   /* Compare a float to an integer. */
   /*================================*/

   if ((type1 == FLOAT) && (type2 == INTEGER))
     {
      if (ValueToDouble(vptr1) < ((double) ValueToLong(vptr2)))
        { return(LESS_THAN); }
      else if (ValueToDouble(vptr1) > ((double) ValueToLong(vptr2)))
        { return(GREATER_THAN); }

      return(EQUAL);
     }

   /*===================================*/
   /* One of the arguments was invalid. */
   /* Return -1 to indicate an error.   */
   /*===================================*/

   return(-1);
  }

/****************************************************************/
/* ExpressionToConstraintRecord: Converts an expression into a  */
/*   constraint record. For example, an expression representing */
/*   the symbol BLUE would be converted to a  record with       */
/*   allowed types SYMBOL and allow-values BLUE.                */
/****************************************************************/
globle CONSTRAINT_RECORD *ExpressionToConstraintRecord(
  void *theEnv,
  struct expr *theExpression)
  {
   CONSTRAINT_RECORD *rv;

   /*================================================*/
   /* A NULL expression is converted to a constraint */
   /* record with no values allowed.                 */
   /*================================================*/

   if (theExpression == NULL)
     {
      rv = GetConstraintRecord(theEnv);
      rv->anyAllowed = FALSE;
      return(rv);
     }

   /*=============================================================*/
   /* Convert variables and function calls to constraint records. */
   /*=============================================================*/

   if ((theExpression->type == SF_VARIABLE) ||
       (theExpression->type == MF_VARIABLE) ||
#if DEFGENERIC_CONSTRUCT
       (theExpression->type == GCALL) ||
#endif
#if DEFFUNCTION_CONSTRUCT
       (theExpression->type == PCALL) ||
#endif
       (theExpression->type == GBL_VARIABLE) ||
       (theExpression->type == MF_GBL_VARIABLE))
     {
      rv = GetConstraintRecord(theEnv);
      rv->multifieldsAllowed = TRUE;
      return(rv);
     }
   else if (theExpression->type == FCALL)
     { return(FunctionCallToConstraintRecord(theEnv,theExpression->value)); }

   /*============================================*/
   /* Convert a constant to a constraint record. */
   /*============================================*/

   rv = GetConstraintRecord(theEnv);
   rv->anyAllowed = FALSE;

   if (theExpression->type == FLOAT)
     {
      rv->floatRestriction = TRUE;
      rv->floatsAllowed = TRUE;
     }
   else if (theExpression->type == INTEGER)
     {
      rv->integerRestriction = TRUE;
      rv->integersAllowed = TRUE;
     }
   else if (theExpression->type == SYMBOL)
     {
      rv->symbolRestriction = TRUE;
      rv->symbolsAllowed = TRUE;
     }
   else if (theExpression->type == STRING)
     {
      rv->stringRestriction = TRUE;
      rv->stringsAllowed = TRUE;
     }
   else if (theExpression->type == INSTANCE_NAME)
     {
      rv->instanceNameRestriction = TRUE;
      rv->instanceNamesAllowed = TRUE;
     }
   else if (theExpression->type == INSTANCE_ADDRESS)
     { rv->instanceAddressesAllowed = TRUE; }

   if (rv->floatsAllowed || rv->integersAllowed || rv->symbolsAllowed ||
       rv->stringsAllowed || rv->instanceNamesAllowed)
     { rv->restrictionList = GenConstant(theEnv,theExpression->type,theExpression->value); }

   return(rv);
  }

/*******************************************************/
/* FunctionCallToConstraintRecord: Converts a function */
/*   call to a constraint record. For example, the +   */
/*   function when converted would be a constraint     */
/*   record with allowed types INTEGER and FLOAT.      */
/*******************************************************/
globle CONSTRAINT_RECORD *FunctionCallToConstraintRecord(
  void *theEnv,
  void *theFunction)
  {
   CONSTRAINT_RECORD *rv;

   rv = GetConstraintRecord(theEnv);
   rv->anyAllowed = FALSE;

   switch ((char) ValueFunctionType(theFunction))
     {
      case 'a':
        rv->externalAddressesAllowed = TRUE;
        break;

      case 'f':
      case 'd':
        rv->floatsAllowed = TRUE;
        break;

      case 'i':
      case 'g':
      case 'l':
        rv->integersAllowed = TRUE;
        break;

      case 'j':
        rv->instanceNamesAllowed = TRUE;
        rv->symbolsAllowed = TRUE;
        rv->stringsAllowed = TRUE;
        break;

      case 'k':
        rv->symbolsAllowed = TRUE;
        rv->stringsAllowed = TRUE;
        break;

      case 'm':
        rv->singlefieldsAllowed = FALSE;
        rv->multifieldsAllowed = TRUE;
        break;

      case 'n':
        rv->floatsAllowed = TRUE;
        rv->integersAllowed = TRUE;
        break;

      case 'o':
        rv->instanceNamesAllowed = TRUE;
        break;

      case 's':
        rv->stringsAllowed = TRUE;
        break;

      case 'u':
        rv->anyAllowed = TRUE;
        rv->multifieldsAllowed = TRUE;
        break;

      case 'w':
      case 'c':
      case 'b':
        rv->symbolsAllowed = TRUE;
        break;

      case 'x':
        rv->instanceAddressesAllowed = TRUE;
        break;

      case 'v':
        rv->voidAllowed = TRUE;
        break;
     }

   return(rv);
  }

/*******************************************************/
/* ArgumentTypeToConstraintRecord: Converts one of the */
/*   function argument types (used by DefineFunction2) */
/*   to a constraint record.                           */
/*******************************************************/
globle CONSTRAINT_RECORD *ArgumentTypeToConstraintRecord(
  void *theEnv,
  int theRestriction)
  {
   CONSTRAINT_RECORD *rv;

   rv = GetConstraintRecord(theEnv);
   rv->anyAllowed = FALSE;

   switch (theRestriction)
     {
      case 'a':
        rv->externalAddressesAllowed = TRUE;
        break;

      case 'e':
        rv->symbolsAllowed = TRUE;
        rv->instanceNamesAllowed = TRUE;
        rv->instanceAddressesAllowed = TRUE;
        break;

      case 'd':
      case 'f':
        rv->floatsAllowed = TRUE;
        break;

      case 'g':
        rv->integersAllowed = TRUE;
        rv->floatsAllowed = TRUE;
        rv->symbolsAllowed = TRUE;
        break;

      case 'h':
        rv->factAddressesAllowed = TRUE;
        rv->integersAllowed = TRUE;
        rv->symbolsAllowed = TRUE;
        rv->instanceNamesAllowed = TRUE;
        rv->instanceAddressesAllowed = TRUE;
        break;

      case 'i':
      case 'l':
        rv->integersAllowed = TRUE;
        break;

      case 'j':
        rv->symbolsAllowed = TRUE;
        rv->stringsAllowed = TRUE;
        rv->instanceNamesAllowed = TRUE;
        break;

      case 'k':
        rv->symbolsAllowed = TRUE;
        rv->stringsAllowed = TRUE;
        break;

      case 'm':
        rv->singlefieldsAllowed = FALSE;
        rv->multifieldsAllowed = TRUE;
        break;

      case 'n':
        rv->floatsAllowed = TRUE;
        rv->integersAllowed = TRUE;
        break;

      case 'o':
        rv->instanceNamesAllowed = TRUE;
        break;

      case 'p':
        rv->instanceNamesAllowed = TRUE;
        rv->symbolsAllowed = TRUE;
        break;

      case 'q':
        rv->symbolsAllowed = TRUE;
        rv->stringsAllowed = TRUE;
        rv->multifieldsAllowed = TRUE;
        break;

      case 's':
        rv->stringsAllowed = TRUE;
        break;

      case 'w':
        rv->symbolsAllowed = TRUE;
        break;

      case 'x':
        rv->instanceAddressesAllowed = TRUE;
        break;

      case 'y':
        rv->factAddressesAllowed = TRUE;
        break;

      case 'z':
        rv->symbolsAllowed = TRUE;
        rv->factAddressesAllowed = TRUE;
        rv->integersAllowed = TRUE;
        break;

      case 'u':
        rv->anyAllowed = TRUE;
        rv->multifieldsAllowed = TRUE;
        break;

      case 'v':
        rv->voidAllowed = TRUE;
        break;
     }

   return(rv);
  }




