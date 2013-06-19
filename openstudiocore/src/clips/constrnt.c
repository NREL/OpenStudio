   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  07/01/05            */
   /*                                                     */
   /*                 CONSTRAINT MODULE                   */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides functions for creating and removing     */
/*   constraint records, adding them to the contraint hash   */
/*   table, and enabling and disabling static and dynamic    */
/*   constraint checking.                                    */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian Dantes                                         */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Added allowed-classes slot facet.              */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#define _CONSTRNT_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>

#include "setup.h"

#include "argacces.h"
#include "constant.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "memalloc.h"
#include "multifld.h"
#include "router.h"
#include "scanner.h"

#include "constrnt.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if (! RUN_TIME) && (! BLOAD_ONLY)
   static void                     InstallConstraintRecord(void *,CONSTRAINT_RECORD *);
   static int                      ConstraintCompare(struct constraintRecord *,struct constraintRecord *);
#endif
#if (! RUN_TIME)
   static void                     ReturnConstraintRecord(void *,CONSTRAINT_RECORD *);
   static void                     DeinstallConstraintRecord(void *,CONSTRAINT_RECORD *);
#endif
   static void                     DeallocateConstraintData(void *);

/*****************************************************/
/* InitializeConstraints: Initializes the constraint */
/*   hash table to NULL and defines the static and   */
/*   dynamic constraint access functions.            */
/*****************************************************/
globle void InitializeConstraints(
  void *theEnv)
  {
#if (! RUN_TIME) && (! BLOAD_ONLY)
   int i;
#endif

   AllocateEnvironmentData(theEnv,CONSTRAINT_DATA,sizeof(struct constraintData),DeallocateConstraintData);
   
   ConstraintData(theEnv)->StaticConstraintChecking = TRUE;
   
#if (! RUN_TIME) && (! BLOAD_ONLY)

    ConstraintData(theEnv)->ConstraintHashtable = (struct constraintRecord **)
                          gm2(theEnv,(int) sizeof (struct constraintRecord *) *
                                    SIZE_CONSTRAINT_HASH);

    if (ConstraintData(theEnv)->ConstraintHashtable == NULL) EnvExitRouter(theEnv,EXIT_FAILURE);

    for (i = 0; i < SIZE_CONSTRAINT_HASH; i++) ConstraintData(theEnv)->ConstraintHashtable[i] = NULL;
#endif

#if (! RUN_TIME)
   EnvDefineFunction2(theEnv,"get-dynamic-constraint-checking",'b',GDCCommand,"GDCCommand", "00");
   EnvDefineFunction2(theEnv,"set-dynamic-constraint-checking",'b',SDCCommand,"SDCCommand", "11");

   EnvDefineFunction2(theEnv,"get-static-constraint-checking",'b',GSCCommand,"GSCCommand", "00");
   EnvDefineFunction2(theEnv,"set-static-constraint-checking",'b',SSCCommand,"SSCCommand", "11");
#endif
  }
  
/*****************************************************/
/* DeallocateConstraintData: Deallocates environment */
/*    data for constraints.                          */
/*****************************************************/
static void DeallocateConstraintData(
  void *theEnv)
  {
#if ! RUN_TIME   
   struct constraintRecord *tmpPtr, *nextPtr;
   int i;

   for (i = 0; i < SIZE_CONSTRAINT_HASH; i++)
     {
      tmpPtr = ConstraintData(theEnv)->ConstraintHashtable[i];
      while (tmpPtr != NULL)
        {
         nextPtr = tmpPtr->next;
         ReturnConstraintRecord(theEnv,tmpPtr);
         tmpPtr = nextPtr;
        }
     }

   rm(theEnv,ConstraintData(theEnv)->ConstraintHashtable,
      (int) sizeof (struct constraintRecord *) * SIZE_CONSTRAINT_HASH);
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
#endif
      
#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME)
   if (ConstraintData(theEnv)->NumberOfConstraints != 0)
     {
      genfree(theEnv,(void *) ConstraintData(theEnv)->ConstraintArray,
              (sizeof(CONSTRAINT_RECORD) * ConstraintData(theEnv)->NumberOfConstraints));
     }
#endif
  }

#if (! RUN_TIME)

/*************************************************************/
/* ReturnConstraintRecord: Frees the data structures used by */
/*   a constraint record. If the returnOnlyFields argument   */
/*   is FALSE, then the constraint record is also freed.     */
/*************************************************************/
static void ReturnConstraintRecord(
  void *theEnv,
  CONSTRAINT_RECORD *constraints)
  {
   if (constraints == NULL) return;

   if (constraints->bucket < 0)
     {
      ReturnExpression(theEnv,constraints->classList);
      ReturnExpression(theEnv,constraints->restrictionList);
      ReturnExpression(theEnv,constraints->maxValue);
      ReturnExpression(theEnv,constraints->minValue);
      ReturnExpression(theEnv,constraints->minFields);
      ReturnExpression(theEnv,constraints->maxFields);
     }

   ReturnConstraintRecord(theEnv,constraints->multifield);

   rtn_struct(theEnv,constraintRecord,constraints);
  }

/***************************************************/
/* DeinstallConstraintRecord: Decrements the count */
/*   values of all occurrences of primitive data   */
/*   types found in a constraint record.           */
/***************************************************/
static void DeinstallConstraintRecord(
  void *theEnv,
  CONSTRAINT_RECORD *constraints)
  {
   if (constraints->bucket >= 0)
     {
      RemoveHashedExpression(theEnv,constraints->classList);
      RemoveHashedExpression(theEnv,constraints->restrictionList);
      RemoveHashedExpression(theEnv,constraints->maxValue);
      RemoveHashedExpression(theEnv,constraints->minValue);
      RemoveHashedExpression(theEnv,constraints->minFields);
      RemoveHashedExpression(theEnv,constraints->maxFields);
     }
   else
     {
      ExpressionDeinstall(theEnv,constraints->classList);
      ExpressionDeinstall(theEnv,constraints->restrictionList);
      ExpressionDeinstall(theEnv,constraints->maxValue);
      ExpressionDeinstall(theEnv,constraints->minValue);
      ExpressionDeinstall(theEnv,constraints->minFields);
      ExpressionDeinstall(theEnv,constraints->maxFields);
     }

   if (constraints->multifield != NULL)
     { DeinstallConstraintRecord(theEnv,constraints->multifield); }
  }

/******************************************/
/* RemoveConstraint: Removes a constraint */
/*   from the constraint hash table.      */
/******************************************/
globle void RemoveConstraint(
  void *theEnv,
  struct constraintRecord *theConstraint)
  {
   struct constraintRecord *tmpPtr, *prevPtr = NULL;

   if (theConstraint == NULL) return;

   /*========================================*/
   /* If the bucket value is less than zero, */
   /* then the constraint wasn't stored in   */
   /* the hash table.                        */
   /*========================================*/

   if (theConstraint->bucket < 0)
     {
      ReturnConstraintRecord(theEnv,theConstraint);
      return;
     }

   /*================================*/
   /* Find and remove the constraint */
   /* from the contraint hash table. */
   /*================================*/

   tmpPtr = ConstraintData(theEnv)->ConstraintHashtable[theConstraint->bucket];
   while (tmpPtr != NULL)
     {
      if (tmpPtr == theConstraint)
        {
         theConstraint->count--;
         if (theConstraint->count == 0)
           {
            if (prevPtr == NULL)
              { ConstraintData(theEnv)->ConstraintHashtable[theConstraint->bucket] = theConstraint->next; }
            else
              { prevPtr->next = theConstraint->next; }
            DeinstallConstraintRecord(theEnv,theConstraint);
            ReturnConstraintRecord(theEnv,theConstraint);
           }
         return;
        }

      prevPtr = tmpPtr;
      tmpPtr = tmpPtr->next;
     }

   return;
  }

#endif /* (! RUN_TIME) */

#if (! RUN_TIME) && (! BLOAD_ONLY)

/***********************************/
/* HashConstraint: Returns a hash  */
/*   value for a given constraint. */
/***********************************/
globle unsigned long HashConstraint(
  struct constraintRecord *theConstraint)
  {
   int i = 0;
   unsigned long count = 0;
   unsigned long hashValue;
   struct expr *tmpPtr;

   count += (unsigned long)
      (theConstraint->anyAllowed * 17) +
      (theConstraint->symbolsAllowed * 5) +
      (theConstraint->stringsAllowed * 23) +
      (theConstraint->floatsAllowed * 19) +
      (theConstraint->integersAllowed * 29) +
      (theConstraint->instanceNamesAllowed * 31) +
      (theConstraint->instanceAddressesAllowed * 17);

   count += (unsigned long)
      (theConstraint->externalAddressesAllowed * 29) +
      (theConstraint->voidAllowed * 29) +
      (theConstraint->multifieldsAllowed * 29) +
      (theConstraint->factAddressesAllowed * 79) +
      (theConstraint->anyRestriction * 59) +
      (theConstraint->symbolRestriction * 61);
      
   count += (unsigned long)
      (theConstraint->stringRestriction * 3) +
      (theConstraint->floatRestriction * 37) +
      (theConstraint->integerRestriction * 9) +
      (theConstraint->classRestriction * 11) +
      (theConstraint->instanceNameRestriction * 7);

   for (tmpPtr = theConstraint->classList; tmpPtr != NULL; tmpPtr = tmpPtr->nextArg)
     { count += GetAtomicHashValue(tmpPtr->type,tmpPtr->value,i++); }

   for (tmpPtr = theConstraint->restrictionList; tmpPtr != NULL; tmpPtr = tmpPtr->nextArg)
     { count += GetAtomicHashValue(tmpPtr->type,tmpPtr->value,i++); }

   for (tmpPtr = theConstraint->minValue; tmpPtr != NULL; tmpPtr = tmpPtr->nextArg)
     { count += GetAtomicHashValue(tmpPtr->type,tmpPtr->value,i++); }

   for (tmpPtr = theConstraint->maxValue; tmpPtr != NULL; tmpPtr = tmpPtr->nextArg)
     { count += GetAtomicHashValue(tmpPtr->type,tmpPtr->value,i++); }

   for (tmpPtr = theConstraint->minFields; tmpPtr != NULL; tmpPtr = tmpPtr->nextArg)
     { count += GetAtomicHashValue(tmpPtr->type,tmpPtr->value,i++); }

   for (tmpPtr = theConstraint->maxFields; tmpPtr != NULL; tmpPtr = tmpPtr->nextArg)
     { count += GetAtomicHashValue(tmpPtr->type,tmpPtr->value,i++); }

   if (theConstraint->multifield != NULL)
     { count += HashConstraint(theConstraint->multifield); }

   hashValue = (unsigned long) (count % SIZE_CONSTRAINT_HASH);

   return(hashValue);
  }

/**********************************************/
/* ConstraintCompare: Compares two constraint */
/*   records and returns TRUE if they are     */
/*   identical, otherwise FALSE.              */
/**********************************************/
static int ConstraintCompare(
  struct constraintRecord *constraint1,
  struct constraintRecord *constraint2)
  {
   struct expr *tmpPtr1, *tmpPtr2;

   if ((constraint1->anyAllowed != constraint2->anyAllowed) ||
       (constraint1->symbolsAllowed != constraint2->symbolsAllowed) ||
       (constraint1->stringsAllowed != constraint2->stringsAllowed) ||
       (constraint1->floatsAllowed != constraint2->floatsAllowed) ||
       (constraint1->integersAllowed != constraint2->integersAllowed) ||
       (constraint1->instanceNamesAllowed != constraint2->instanceNamesAllowed) ||
       (constraint1->instanceAddressesAllowed != constraint2->instanceAddressesAllowed) ||
       (constraint1->externalAddressesAllowed != constraint2->externalAddressesAllowed) ||
       (constraint1->voidAllowed != constraint2->voidAllowed) ||
       (constraint1->multifieldsAllowed != constraint2->multifieldsAllowed) ||
       (constraint1->singlefieldsAllowed != constraint2->singlefieldsAllowed) ||
       (constraint1->factAddressesAllowed != constraint2->factAddressesAllowed) ||
       (constraint1->anyRestriction != constraint2->anyRestriction) ||
       (constraint1->symbolRestriction != constraint2->symbolRestriction) ||
       (constraint1->stringRestriction != constraint2->stringRestriction) ||
       (constraint1->floatRestriction != constraint2->floatRestriction) ||
       (constraint1->integerRestriction != constraint2->integerRestriction) ||
       (constraint1->classRestriction != constraint2->classRestriction) ||
       (constraint1->instanceNameRestriction != constraint2->instanceNameRestriction))
     { return(FALSE); }

   for (tmpPtr1 = constraint1->classList, tmpPtr2 = constraint2->classList;
        (tmpPtr1 != NULL) && (tmpPtr2 != NULL);
        tmpPtr1 = tmpPtr1->nextArg, tmpPtr2 = tmpPtr2->nextArg)
     {
      if ((tmpPtr1->type != tmpPtr2->type) || (tmpPtr1->value != tmpPtr2->value))
        { return(FALSE); }
     }
   if (tmpPtr1 != tmpPtr2) return(FALSE);

   for (tmpPtr1 = constraint1->restrictionList, tmpPtr2 = constraint2->restrictionList;
        (tmpPtr1 != NULL) && (tmpPtr2 != NULL);
        tmpPtr1 = tmpPtr1->nextArg, tmpPtr2 = tmpPtr2->nextArg)
     {
      if ((tmpPtr1->type != tmpPtr2->type) || (tmpPtr1->value != tmpPtr2->value))
        { return(FALSE); }
     }
   if (tmpPtr1 != tmpPtr2) return(FALSE);

   for (tmpPtr1 = constraint1->minValue, tmpPtr2 = constraint2->minValue;
        (tmpPtr1 != NULL) && (tmpPtr2 != NULL);
        tmpPtr1 = tmpPtr1->nextArg, tmpPtr2 = tmpPtr2->nextArg)
     {
      if ((tmpPtr1->type != tmpPtr2->type) || (tmpPtr1->value != tmpPtr2->value))
        { return(FALSE); }
     }
   if (tmpPtr1 != tmpPtr2) return(FALSE);

   for (tmpPtr1 = constraint1->maxValue, tmpPtr2 = constraint2->maxValue;
        (tmpPtr1 != NULL) && (tmpPtr2 != NULL);
        tmpPtr1 = tmpPtr1->nextArg, tmpPtr2 = tmpPtr2->nextArg)
     {
      if ((tmpPtr1->type != tmpPtr2->type) || (tmpPtr1->value != tmpPtr2->value))
        { return(FALSE); }
     }
   if (tmpPtr1 != tmpPtr2) return(FALSE);

   for (tmpPtr1 = constraint1->minFields, tmpPtr2 = constraint2->minFields;
        (tmpPtr1 != NULL) && (tmpPtr2 != NULL);
        tmpPtr1 = tmpPtr1->nextArg, tmpPtr2 = tmpPtr2->nextArg)
     {
      if ((tmpPtr1->type != tmpPtr2->type) || (tmpPtr1->value != tmpPtr2->value))
        { return(FALSE); }
     }
   if (tmpPtr1 != tmpPtr2) return(FALSE);

   for (tmpPtr1 = constraint1->maxFields, tmpPtr2 = constraint2->maxFields;
        (tmpPtr1 != NULL) && (tmpPtr2 != NULL);
        tmpPtr1 = tmpPtr1->nextArg, tmpPtr2 = tmpPtr2->nextArg)
     {
      if ((tmpPtr1->type != tmpPtr2->type) || (tmpPtr1->value != tmpPtr2->value))
        { return(FALSE); }
     }
   if (tmpPtr1 != tmpPtr2) return(FALSE);

   if (((constraint1->multifield == NULL) && (constraint2->multifield != NULL)) ||
       ((constraint1->multifield != NULL) && (constraint2->multifield == NULL)))
     { return(FALSE); }
   else if (constraint1->multifield == constraint2->multifield)
     { return(TRUE); }

   return(ConstraintCompare(constraint1->multifield,constraint2->multifield));
  }

/************************************/
/* AddConstraint: Adds a constraint */
/*   to the constraint hash table.  */
/************************************/
globle struct constraintRecord *AddConstraint(
  void *theEnv,
  struct constraintRecord *theConstraint)
  {
   struct constraintRecord *tmpPtr;
   unsigned long hashValue;

   if (theConstraint == NULL) return(NULL);

   hashValue = HashConstraint(theConstraint);

   for (tmpPtr = ConstraintData(theEnv)->ConstraintHashtable[hashValue];
        tmpPtr != NULL;
        tmpPtr = tmpPtr->next)
     {
      if (ConstraintCompare(theConstraint,tmpPtr))
        {
         tmpPtr->count++;
         ReturnConstraintRecord(theEnv,theConstraint);
         return(tmpPtr);
        }
     }

   InstallConstraintRecord(theEnv,theConstraint);
   theConstraint->count = 1;
   theConstraint->bucket = hashValue;
   theConstraint->next = ConstraintData(theEnv)->ConstraintHashtable[hashValue];
   ConstraintData(theEnv)->ConstraintHashtable[hashValue] = theConstraint;
   return(theConstraint);
  }

/*************************************************/
/* InstallConstraintRecord: Increments the count */
/*   values of all occurrences of primitive data */
/*   types found in a constraint record.         */
/*************************************************/
static void InstallConstraintRecord(
  void *theEnv,
  CONSTRAINT_RECORD *constraints)
  {
   struct expr *tempExpr;

   tempExpr = AddHashedExpression(theEnv,constraints->classList);
   ReturnExpression(theEnv,constraints->classList);
   constraints->classList = tempExpr;

   tempExpr = AddHashedExpression(theEnv,constraints->restrictionList);
   ReturnExpression(theEnv,constraints->restrictionList);
   constraints->restrictionList = tempExpr;

   tempExpr = AddHashedExpression(theEnv,constraints->maxValue);
   ReturnExpression(theEnv,constraints->maxValue);
   constraints->maxValue = tempExpr;

   tempExpr = AddHashedExpression(theEnv,constraints->minValue);
   ReturnExpression(theEnv,constraints->minValue);
   constraints->minValue = tempExpr;

   tempExpr = AddHashedExpression(theEnv,constraints->minFields);
   ReturnExpression(theEnv,constraints->minFields);
   constraints->minFields = tempExpr;

   tempExpr = AddHashedExpression(theEnv,constraints->maxFields);
   ReturnExpression(theEnv,constraints->maxFields);
   constraints->maxFields = tempExpr;

   if (constraints->multifield != NULL)
     { InstallConstraintRecord(theEnv,constraints->multifield); }
  }

#endif /* (! RUN_TIME) && (! BLOAD_ONLY) */

/**********************************************/
/* SDCCommand: H/L access routine for the     */
/*   set-dynamic-constraint-checking command. */
/**********************************************/
globle int SDCCommand(
  void *theEnv)
  {
   int oldValue;
   DATA_OBJECT arg_ptr;

   oldValue = EnvGetDynamicConstraintChecking(theEnv);

   if (EnvArgCountCheck(theEnv,"set-dynamic-constraint-checking",EXACTLY,1) == -1)
     { return(oldValue); }

   EnvRtnUnknown(theEnv,1,&arg_ptr);

   if ((arg_ptr.value == EnvFalseSymbol(theEnv)) && (arg_ptr.type == SYMBOL))
     { EnvSetDynamicConstraintChecking(theEnv,FALSE); }
   else
     { EnvSetDynamicConstraintChecking(theEnv,TRUE); }

   return(oldValue);
  }

/**********************************************/
/* GDCCommand: H/L access routine for the     */
/*   get-dynamic-constraint-checking command. */
/**********************************************/
globle int GDCCommand(
  void *theEnv)
  {
   int oldValue;

   oldValue = EnvGetDynamicConstraintChecking(theEnv);

   if (EnvArgCountCheck(theEnv,"get-dynamic-constraint-checking",EXACTLY,0) == -1)
     { return(oldValue); }

   return(oldValue);
  }

/*********************************************/
/* SSCCommand: H/L access routine for the    */
/*   set-static-constraint-checking command. */
/*********************************************/
globle int SSCCommand(
  void *theEnv)
  {
   int oldValue;
   DATA_OBJECT arg_ptr;

   oldValue = EnvGetStaticConstraintChecking(theEnv);

   if (EnvArgCountCheck(theEnv,"set-static-constraint-checking",EXACTLY,1) == -1)
     { return(oldValue); }

   EnvRtnUnknown(theEnv,1,&arg_ptr);

   if ((arg_ptr.value == EnvFalseSymbol(theEnv)) && (arg_ptr.type == SYMBOL))
     { EnvSetStaticConstraintChecking(theEnv,FALSE); }
   else
     { EnvSetStaticConstraintChecking(theEnv,TRUE); }

   return(oldValue);
  }

/*********************************************/
/* GSCCommand: H/L access routine for the    */
/*   get-static-constraint-checking command. */
/*********************************************/
globle int GSCCommand(
  void *theEnv)
  {
   int oldValue;

   oldValue = EnvGetStaticConstraintChecking(theEnv);

   if (EnvArgCountCheck(theEnv,"get-static-constraint-checking",EXACTLY,0) == -1)
     { return(oldValue); }

   return(oldValue);
  }

/******************************************************/
/* EnvSetDynamicConstraintChecking: C access routine  */
/*   for the set-dynamic-constraint-checking command. */
/******************************************************/
globle intBool EnvSetDynamicConstraintChecking(
  void *theEnv,
  int value)
  {
   int ov;
   ov = ConstraintData(theEnv)->DynamicConstraintChecking;
   ConstraintData(theEnv)->DynamicConstraintChecking = value;
   return(ov);
  }

/******************************************************/
/* EnvGetDynamicConstraintChecking: C access routine  */
/*   for the get-dynamic-constraint-checking command. */
/******************************************************/
globle intBool EnvGetDynamicConstraintChecking(
  void *theEnv)
  { 
   return(ConstraintData(theEnv)->DynamicConstraintChecking); 
  }

/*****************************************************/
/* EnvSetStaticConstraintChecking: C access routine  */
/*   for the set-static-constraint-checking command. */
/*****************************************************/
globle intBool EnvSetStaticConstraintChecking(
  void *theEnv,
  int value)
  {
   int ov;

   ov = ConstraintData(theEnv)->StaticConstraintChecking;
   ConstraintData(theEnv)->StaticConstraintChecking = value;
   return(ov);
  }

/*****************************************************/
/* EnvGetStaticConstraintChecking: C access routine  */
/*   for the get-static-constraint-checking command. */
/*****************************************************/
globle intBool EnvGetStaticConstraintChecking(
  void *theEnv)
  {    
   return(ConstraintData(theEnv)->StaticConstraintChecking); 
  }

