   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  07/01/05            */
   /*                                                     */
   /*            CONSTRAINT OPERATIONS MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides functions for performing operations on  */
/*   constraint records including computing the intersection */
/*   and union of constraint records.                        */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Added allowed-classes slot facet.              */
/*                                                           */
/*************************************************************/

#define _CSTRNOPS_SOURCE_

#include "setup.h"

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>

#if (! RUN_TIME)

#include "constant.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "router.h"
#include "extnfunc.h"
#include "scanner.h"
#include "multifld.h"
#include "constrnt.h"
#include "cstrnchk.h"
#include "cstrnutl.h"

#include "cstrnops.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                     IntersectNumericExpressions(void *,
                                                               CONSTRAINT_RECORD *,
                                                               CONSTRAINT_RECORD *,
                                                               CONSTRAINT_RECORD *,int);
   static void                     IntersectAllowedValueExpressions(void *,
                                                                    CONSTRAINT_RECORD *,
                                                                    CONSTRAINT_RECORD *,
                                                                    CONSTRAINT_RECORD *);
   static void                     IntersectAllowedClassExpressions(void *,
                                                                    CONSTRAINT_RECORD *,
                                                                    CONSTRAINT_RECORD *,
                                                                    CONSTRAINT_RECORD *);
   static int                      FindItemInExpression(int,void *,int,struct expr *);
   static void                     UpdateRestrictionFlags(CONSTRAINT_RECORD *);
#if (! BLOAD_ONLY)
   static void                     UnionRangeMinMaxValueWithList(void *,
                                                                 struct expr *,
                                                                 struct expr *,
                                                                 struct expr **,
                                                                 struct expr **);
   static void                     UnionNumericExpressions(void *,
                                                         CONSTRAINT_RECORD *,
                                                         CONSTRAINT_RECORD *,
                                                         CONSTRAINT_RECORD *,int);
   static struct expr             *AddToUnionList(void *,
                                                  struct expr *,struct expr *,
                                                  CONSTRAINT_RECORD *);
   static void                     UnionAllowedValueExpressions(void *,
                                                                CONSTRAINT_RECORD *,
                                                                CONSTRAINT_RECORD *,
                                                                CONSTRAINT_RECORD *);
   static void                     UnionAllowedClassExpressions(void *,
                                                                CONSTRAINT_RECORD *,
                                                                CONSTRAINT_RECORD *,
                                                                CONSTRAINT_RECORD *);
   static int                      RestrictionOnType(int,CONSTRAINT_RECORD *);
#endif

/**************************************************************/
/* IntersectConstraints: Creates a new constraint record that */
/*   is the intersection of two other constraint records.     */
/**************************************************************/
globle struct constraintRecord *IntersectConstraints(
  void *theEnv,
  CONSTRAINT_RECORD *c1,
  CONSTRAINT_RECORD *c2)
  {
   struct constraintRecord *rv;
   int c1Changed = FALSE, c2Changed = FALSE;

   /*=================================================*/
   /* If both constraint records are NULL,then create */
   /* a constraint record that allows any value.      */
   /*=================================================*/

   if ((c1 == NULL) && (c2 == NULL))
     {
      rv = GetConstraintRecord(theEnv);
      rv->multifieldsAllowed = TRUE;
      return(rv);
     }

   /*=================================================*/
   /* If one of the constraint records is NULL, then  */
   /* the intersection is the other constraint record */
   /* (a NULL value means no constraints).            */
   /*=================================================*/

   if (c1 == NULL) return(CopyConstraintRecord(theEnv,c2));

   if (c2 == NULL) return(CopyConstraintRecord(theEnv,c1));

   /*=================================*/
   /* Create a new constraint record. */
   /*=================================*/

   rv = GetConstraintRecord(theEnv);

   /*==============================*/
   /* Intersect the allowed types. */
   /*==============================*/

   if ((c1->multifieldsAllowed != c2->multifieldsAllowed) &&
       (c1->singlefieldsAllowed != c2->singlefieldsAllowed))
     {
      rv->anyAllowed = FALSE;
      return(rv);
     }

   if (c1->multifieldsAllowed && c2->multifieldsAllowed)
     { rv->multifieldsAllowed = TRUE; }
   else
     { rv->multifieldsAllowed = FALSE; }

   if (c1->singlefieldsAllowed && c2->singlefieldsAllowed)
     { rv->singlefieldsAllowed = TRUE; }
   else
     { rv->singlefieldsAllowed = FALSE; }

   if (c1->anyAllowed && c2->anyAllowed) rv->anyAllowed = TRUE;
   else
     {
      if (c1->anyAllowed)
        {
         c1Changed = TRUE;
         SetAnyAllowedFlags(c1,FALSE);
        }
      else if (c2->anyAllowed)
        {
         c2Changed = TRUE;
         SetAnyAllowedFlags(c2,FALSE);
        }

      rv->anyAllowed = FALSE;
      rv->symbolsAllowed = (c1->symbolsAllowed && c2->symbolsAllowed);
      rv->stringsAllowed = (c1->stringsAllowed && c2->stringsAllowed);
      rv->floatsAllowed = (c1->floatsAllowed && c2->floatsAllowed);
      rv->integersAllowed = (c1->integersAllowed && c2->integersAllowed);
      rv->instanceNamesAllowed = (c1->instanceNamesAllowed && c2->instanceNamesAllowed);
      rv->instanceAddressesAllowed = (c1->instanceAddressesAllowed && c2->instanceAddressesAllowed);
      rv->externalAddressesAllowed = (c1->externalAddressesAllowed && c2->externalAddressesAllowed);
      rv->voidAllowed = (c1->voidAllowed && c2->voidAllowed);
      rv->multifieldsAllowed = (c1->multifieldsAllowed && c2->multifieldsAllowed);
      rv->factAddressesAllowed = (c1->factAddressesAllowed && c2->factAddressesAllowed);

      if (c1Changed) SetAnyAllowedFlags(c1,TRUE);
      if (c2Changed) SetAnyAllowedFlags(c2,TRUE);
     }

   /*=====================================*/
   /* Intersect the allowed-values flags. */
   /*=====================================*/

   if (c1->anyRestriction || c2->anyRestriction) rv->anyRestriction = TRUE;
   else
     {
      rv->anyRestriction = FALSE;
      rv->symbolRestriction = (c1->symbolRestriction || c2->symbolRestriction);
      rv->stringRestriction = (c1->stringRestriction || c2->stringRestriction);
      rv->floatRestriction = (c1->floatRestriction || c2->floatRestriction);
      rv->integerRestriction = (c1->integerRestriction || c2->integerRestriction);
      rv->classRestriction = (c1->classRestriction || c2->classRestriction);
      rv->instanceNameRestriction = (c1->instanceNameRestriction || c2->instanceNameRestriction);
     }

   /*==================================================*/
   /* Intersect the allowed values list, allowed class */
   /* list, min and max values, and the range values.  */
   /*==================================================*/

   IntersectAllowedValueExpressions(theEnv,c1,c2,rv);
   IntersectAllowedClassExpressions(theEnv,c1,c2,rv);
   IntersectNumericExpressions(theEnv,c1,c2,rv,TRUE);
   IntersectNumericExpressions(theEnv,c1,c2,rv,FALSE);

   /*==========================================*/
   /* Update the allowed-values flags based on */
   /* the previous intersection for allowed,   */
   /* min and max, and range values.           */
   /*==========================================*/

   UpdateRestrictionFlags(rv);

   /*============================================*/
   /* If multifields are allowed, then intersect */
   /* the constraint record for them.            */
   /*============================================*/

   if (rv->multifieldsAllowed)
     {
      rv->multifield = IntersectConstraints(theEnv,c1->multifield,c2->multifield);
      if (UnmatchableConstraint(rv->multifield))
        { rv->multifieldsAllowed = FALSE; }
     }

   /*========================*/
   /* Return the intersected */
   /* constraint record.     */
   /*========================*/

   return(rv);
  }

/*************************************************/
/* IntersectAllowedValueExpressions: Creates the */
/*   intersection of two allowed-values lists.   */
/*************************************************/
static void IntersectAllowedValueExpressions(
  void *theEnv,
  CONSTRAINT_RECORD *constraint1,
  CONSTRAINT_RECORD *constraint2,
  CONSTRAINT_RECORD *newConstraint)
  {
   struct expr *theList1, *theList2;
   struct expr *theHead = NULL, *tmpExpr;

   /*===========================================*/
   /* Loop through each value in allowed-values */
   /* list of the first constraint record. Add  */
   /* each value to a list if it satisfies the  */
   /* restrictions for both constraint records. */
   /*===========================================*/

   for (theList1 = constraint1->restrictionList;
        theList1 != NULL;
        theList1 = theList1->nextArg)
     {
      if (CheckAllowedValuesConstraint(theList1->type,theList1->value,constraint1) &&
          CheckAllowedValuesConstraint(theList1->type,theList1->value,constraint2))
        {
         tmpExpr = GenConstant(theEnv,theList1->type,theList1->value);
         tmpExpr->nextArg = theHead;
         theHead = tmpExpr;
        }
     }

   /*===========================================*/
   /* Loop through each value in allowed-values */
   /* list of the second constraint record. Add */
   /* each value to a list if it satisfies the  */
   /* restrictions for both constraint records. */
   /*===========================================*/

   for (theList2 = constraint2->restrictionList;
        theList2 != NULL;
        theList2 = theList2->nextArg)
     {
      if (FindItemInExpression(theList2->type,theList2->value,TRUE,theHead))
        { /* The value is already in the list--Do nothing */ }
      else if (CheckAllowedValuesConstraint(theList2->type,theList2->value,constraint1) &&
               CheckAllowedValuesConstraint(theList2->type,theList2->value,constraint2))
        {
         tmpExpr = GenConstant(theEnv,theList2->type,theList2->value);
         tmpExpr->nextArg = theHead;
         theHead = tmpExpr;
        }
     }

   /*================================================*/
   /* Set the allowed values list for the constraint */
   /* record to the intersected values of the two    */
   /* other constraint records.                      */
   /*================================================*/

   newConstraint->restrictionList = theHead;
  }
  
/*************************************************/
/* IntersectAllowedClassExpressions: Creates the */
/*   intersection of two allowed-classes lists.  */
/*************************************************/
static void IntersectAllowedClassExpressions(
  void *theEnv,
  CONSTRAINT_RECORD *constraint1,
  CONSTRAINT_RECORD *constraint2,
  CONSTRAINT_RECORD *newConstraint)
  {
   struct expr *theList1, *theList2;
   struct expr *theHead = NULL, *tmpExpr;

   /*============================================*/
   /* Loop through each value in allowed-classes */
   /* list of the first constraint record. Add   */
   /* each value to a list if it satisfies the   */
   /* restrictions for both constraint records.  */
   /*============================================*/
   
   for (theList1 = constraint1->classList;
        theList1 != NULL;
        theList1 = theList1->nextArg)
     {
      if (CheckAllowedClassesConstraint(theEnv,theList1->type,theList1->value,constraint1) &&
          CheckAllowedClassesConstraint(theEnv,theList1->type,theList1->value,constraint2))
        {
         tmpExpr = GenConstant(theEnv,theList1->type,theList1->value);
         tmpExpr->nextArg = theHead;
         theHead = tmpExpr;
        }
     }

   /*============================================*/
   /* Loop through each value in allowed-classes */
   /* list of the second constraint record. Add  */
   /* each value to a list if it satisfies the   */
   /* restrictions for both constraint records.  */
   /*============================================*/

   for (theList2 = constraint2->classList;
        theList2 != NULL;
        theList2 = theList2->nextArg)
     {
      if (FindItemInExpression(theList2->type,theList2->value,TRUE,theHead))
        { /* The value is already in the list--Do nothing */ }
      else if (CheckAllowedClassesConstraint(theEnv,theList2->type,theList2->value,constraint1) &&
               CheckAllowedClassesConstraint(theEnv,theList2->type,theList2->value,constraint2))
        {
         tmpExpr = GenConstant(theEnv,theList2->type,theList2->value);
         tmpExpr->nextArg = theHead;
         theHead = tmpExpr;
        }
     }

   /*=================================================*/
   /* Set the allowed classes list for the constraint */
   /* record to the intersected values of the two     */
   /* other constraint records.                       */
   /*=================================================*/

   newConstraint->classList = theHead;
  }
  
/*********************************************************/
/* IntersectNumericExpressions: Creates the intersection */
/*   of two range or two min/max-fields constraints.     */
/*********************************************************/
static void IntersectNumericExpressions(
  void *theEnv,
  CONSTRAINT_RECORD *constraint1,
  CONSTRAINT_RECORD *constraint2,
  CONSTRAINT_RECORD *newConstraint,
  int range)
  {
   struct expr *tmpmin1, *tmpmax1, *tmpmin2, *tmpmax2, *theMin, *theMax;
   struct expr *theMinList, *theMaxList, *lastMin = NULL, *lastMax = NULL;
   int cmaxmax, cminmin, cmaxmin, cminmax;

   /*==========================================*/
   /* Initialize the new range/min/max values  */
   /* for the intersection of the constraints. */
   /*==========================================*/

   theMinList = NULL;
   theMaxList = NULL;

   /*=================================*/
   /* Determine the min/max values of */
   /* the first constraint record.    */
   /*=================================*/

   if (range)
     {
      tmpmin1 = constraint1->minValue;
      tmpmax1 = constraint1->maxValue;
     }
   else
     {
      tmpmin1 = constraint1->minFields;
      tmpmax1 = constraint1->maxFields;
     }

   /*===========================================*/
   /* Loop through each of range/min/max values */
   /* from the first constraint record.         */
   /*===========================================*/

   for (;
        tmpmin1 != NULL;
        tmpmin1 = tmpmin1->nextArg, tmpmax1 = tmpmax1->nextArg)
     {
      /*============================================*/
      /* Get the appropriate values from the second */
      /* constraint record for comparison.          */
      /*============================================*/

      if (range)
        {
         tmpmin2 = constraint2->minValue;
         tmpmax2 = constraint2->maxValue;
        }
      else
        {
         tmpmin2 = constraint2->minFields;
         tmpmax2 = constraint2->maxFields;
        }

      /*================================================*/
      /* Loop through each of range/min/max values from */
      /* the second constraint record comparing it to   */
      /* the values from the first constraint record.   */
      /*================================================*/

      for (;
           tmpmin2 != NULL;
           tmpmin2 = tmpmin2->nextArg, tmpmax2 = tmpmax2->nextArg)
        {
         /*==============================================*/
         /* Determine the relationship between the four  */
         /* combinations of min/max values (>, <, or =). */
         /*==============================================*/

         cmaxmax = CompareNumbers(theEnv,tmpmax1->type,tmpmax1->value,
                                  tmpmax2->type,tmpmax2->value);

         cminmin = CompareNumbers(theEnv,tmpmin1->type,tmpmin1->value,
                                  tmpmin2->type,tmpmin2->value);

         cmaxmin = CompareNumbers(theEnv,tmpmax1->type,tmpmax1->value,
                                  tmpmin2->type,tmpmin2->value);

         cminmax = CompareNumbers(theEnv,tmpmin1->type,tmpmin1->value,
                                  tmpmax2->type,tmpmax2->value);

         /*============================================*/
         /* If the range/min/max values don't overlap, */
         /* then proceed to the next pair of numbers   */
         /* to see if they overlap.                    */
         /*============================================*/

         if ((cmaxmin == LESS_THAN) || (cminmax == GREATER_THAN))
           { continue; }

         /*=======================================*/
         /* Compute the new minimum value for the */
         /* intersected range/min/max values.     */
         /*=======================================*/

         if (cminmin == GREATER_THAN)
           { theMin = GenConstant(theEnv,tmpmin1->type,tmpmin1->value); }
         else
           { theMin = GenConstant(theEnv,tmpmin2->type,tmpmin2->value); }

         /*=======================================*/
         /* Compute the new maximum value for the */
         /* intersected range/min/max values.     */
         /*=======================================*/

         if (cmaxmax == LESS_THAN)
           { theMax = GenConstant(theEnv,tmpmax1->type,tmpmax1->value); }
         else
           { theMax = GenConstant(theEnv,tmpmax2->type,tmpmax2->value); }

         /*==================================*/
         /* Add the new range/min/max values */
         /* to the intersection list.        */
         /*==================================*/

         if (lastMin == NULL)
           {
            theMinList = theMin;
            theMaxList = theMax;
           }
         else
           {
            lastMin->nextArg = theMin;
            lastMax->nextArg = theMax;
           }

         lastMin = theMin;
         lastMax = theMax;
        }
     }

   /*============================================================*/
   /* If the intersection produced a pair of valid range/min/max */
   /* values, then replace the previous values of the constraint */
   /* record to the new intersected values.                      */
   /*============================================================*/

   if (theMinList != NULL)
     {
      if (range)
        {
         ReturnExpression(theEnv,newConstraint->minValue);
         ReturnExpression(theEnv,newConstraint->maxValue);
         newConstraint->minValue = theMinList;
         newConstraint->maxValue = theMaxList;
        }
      else
        {
         ReturnExpression(theEnv,newConstraint->minFields);
         ReturnExpression(theEnv,newConstraint->maxFields);
         newConstraint->minFields = theMinList;
         newConstraint->maxFields = theMaxList;
        }
     }

   /*===============================================================*/
   /* Otherwise, the intersection produced no valid range/min/max   */
   /* values. For the range attribute, this means that no numbers   */
   /* can satisfy the constraint. For the min/max fields attribute, */
   /* it means that no value can satisfy the constraint.            */
   /*===============================================================*/

   else
     {
      if (range)
        {
         if (newConstraint->anyAllowed) SetAnyAllowedFlags(newConstraint,FALSE);
         newConstraint->integersAllowed = FALSE;
         newConstraint->floatsAllowed = FALSE;
        }
      else
        {
         SetAnyAllowedFlags(newConstraint,TRUE);
         newConstraint->singlefieldsAllowed = FALSE;
         newConstraint->multifieldsAllowed = FALSE;
         newConstraint->anyAllowed = FALSE;
        }
     }
  }

/************************************************************/
/* UpdateRestrictionFlags: Updates the types allowed flags  */
/*   based on the allowed values in a constraint record.    */
/*   Intended to be called after the allowed values list    */
/*   has been changed (for example after intersecting the   */
/*   allowed-values list there may no be any values of a    */
/*   particular type left even though the type is allowed). */
/************************************************************/
static void UpdateRestrictionFlags(
  CONSTRAINT_RECORD *rv)
  {
   if ((rv->anyRestriction) && (rv->restrictionList == NULL))
     {
      SetAnyAllowedFlags(rv,TRUE);
      rv->anyAllowed = FALSE;
     }

   if ((rv->symbolRestriction) && (rv->symbolsAllowed))
     { rv->symbolsAllowed = FindItemInExpression(SYMBOL,NULL,FALSE,rv->restrictionList); }

   if ((rv->stringRestriction)  && (rv->stringsAllowed))
     { rv->stringsAllowed = FindItemInExpression(STRING,NULL,FALSE,rv->restrictionList); }

   if ((rv->floatRestriction) && (rv->floatsAllowed))
     { rv->floatsAllowed = FindItemInExpression(FLOAT,NULL,FALSE,rv->restrictionList); }

   if ((rv->integerRestriction) && (rv->integersAllowed))
     { rv->integersAllowed = FindItemInExpression(INTEGER,NULL,FALSE,rv->restrictionList); }

   if ((rv->instanceNameRestriction) && (rv->instanceNamesAllowed))
     { rv->instanceNamesAllowed = FindItemInExpression(INSTANCE_NAME,NULL,FALSE,rv->restrictionList); }
  }

/*************************************************************/
/* FindItemInExpression: Determines if a particular constant */
/*   (such as 27) or a class of constants (such as integers) */
/*   can be found in a list of constants. Returns TRUE if    */
/*   such a constant can be found, otherwise FALSE.          */
/*************************************************************/
static int FindItemInExpression(
  int theType,
  void *theValue,
  int useValue,
  struct expr *theList)
  {
   while (theList != NULL)
     {
      if (theList->type == theType)
        {
         if (! useValue) return(TRUE);
         else if (theList->value == theValue) return(TRUE);
        }

      theList = theList->nextArg;
     }

   return(FALSE);
  }

#if (! BLOAD_ONLY)

/**************************************************/
/* RestrictionOnType: Determines if a restriction */
/*   is present for a specific type. Returns TRUE */
/*   if there is, otherwise FALSE.                */
/**************************************************/
static int RestrictionOnType(
  int theType,
  CONSTRAINT_RECORD *theConstraint)
  {
   if (theConstraint == NULL) return(FALSE);

   if ((theConstraint->anyRestriction) ||
       (theConstraint->symbolRestriction && (theType == SYMBOL)) ||
       (theConstraint->stringRestriction && (theType == STRING)) ||
       (theConstraint->floatRestriction && (theType == FLOAT)) ||
       (theConstraint->integerRestriction && (theType == INTEGER)) ||
       (theConstraint->classRestriction && ((theType == INSTANCE_ADDRESS) ||
                                            (theType == INSTANCE_NAME))) ||
       (theConstraint->instanceNameRestriction && (theType == INSTANCE_NAME)))
     { return(TRUE); }

   return(FALSE);
  }

/**********************************************************/
/* UnionConstraints: Creates a new constraint record that */
/*   is the union of two other constraint records.        */
/**********************************************************/
globle struct constraintRecord *UnionConstraints(
  void *theEnv,
  CONSTRAINT_RECORD *c1,
  CONSTRAINT_RECORD *c2)
  {
   struct constraintRecord *rv;
   int c1Changed = FALSE, c2Changed = FALSE;

   /*=================================================*/
   /* If both constraint records are NULL,then create */
   /* a constraint record that allows any value.      */
   /*=================================================*/

   if ((c1 == NULL) && (c2 == NULL)) return(GetConstraintRecord(theEnv));

   /*=====================================================*/
   /* If one of the constraint records is NULL, then the  */
   /* union is the other constraint record. Note that     */
   /* this is different from the way that intersections   */
   /* were handled (a NULL constraint record implied that */
   /*  any value was legal which in turn would imply that */
   /* the union would allow any value as well).           */
   /*=====================================================*/

   if (c1 == NULL) return(CopyConstraintRecord(theEnv,c2));

   if (c2 == NULL) return(CopyConstraintRecord(theEnv,c1));

   /*=================================*/
   /* Create a new constraint record. */
   /*=================================*/

   rv = GetConstraintRecord(theEnv);

   /*==========================*/
   /* Union the allowed types. */
   /*==========================*/

   if (c1->multifieldsAllowed || c2->multifieldsAllowed)
     { rv->multifieldsAllowed = TRUE; }

   if (c1->singlefieldsAllowed || c2->singlefieldsAllowed)
     { rv->singlefieldsAllowed = TRUE; }

   if (c1->anyAllowed || c2->anyAllowed) rv->anyAllowed = TRUE;
   else
     {
      rv->anyAllowed = FALSE;
      rv->symbolsAllowed = (c1->symbolsAllowed || c2->symbolsAllowed);
      rv->stringsAllowed = (c1->stringsAllowed || c2->stringsAllowed);
      rv->floatsAllowed = (c1->floatsAllowed || c2->floatsAllowed);
      rv->integersAllowed = (c1->integersAllowed || c2->integersAllowed);
      rv->instanceNamesAllowed = (c1->instanceNamesAllowed || c2->instanceNamesAllowed);
      rv->instanceAddressesAllowed = (c1->instanceAddressesAllowed || c2->instanceAddressesAllowed);
      rv->externalAddressesAllowed = (c1->externalAddressesAllowed || c2->externalAddressesAllowed);
      rv->voidAllowed = (c1->voidAllowed || c2->voidAllowed);
      rv->factAddressesAllowed = (c1->factAddressesAllowed || c2->factAddressesAllowed);
     }

   /*=================================*/
   /* Union the allowed-values flags. */
   /*=================================*/

   if (c1->anyRestriction && c2->anyRestriction) rv->anyRestriction = TRUE;
   else
     {
      if (c1->anyRestriction)
        {
         c1Changed = TRUE;
         SetAnyRestrictionFlags(c1,FALSE);
        }
      else if (c2->anyRestriction)
        {
         c2Changed = TRUE;
         SetAnyRestrictionFlags(c2,FALSE);
        }

      rv->anyRestriction = FALSE;
      rv->symbolRestriction = (c1->symbolRestriction && c2->symbolRestriction);
      rv->stringRestriction = (c1->stringRestriction && c2->stringRestriction);
      rv->floatRestriction = (c1->floatRestriction && c2->floatRestriction);
      rv->integerRestriction = (c1->integerRestriction && c2->integerRestriction);
      rv->classRestriction = (c1->classRestriction && c2->classRestriction);
      rv->instanceNameRestriction = (c1->instanceNameRestriction && c2->instanceNameRestriction);

      if (c1Changed) SetAnyRestrictionFlags(c1,FALSE);
      else if (c2Changed) SetAnyRestrictionFlags(c2,FALSE);
     }

   /*========================================*/
   /* Union the allowed values list, the min */
   /* and max values, and the range values.  */
   /*========================================*/

   UnionAllowedValueExpressions(theEnv,c1,c2,rv);
   UnionAllowedClassExpressions(theEnv,c1,c2,rv);
   UnionNumericExpressions(theEnv,c1,c2,rv,TRUE);
   UnionNumericExpressions(theEnv,c1,c2,rv,FALSE);

   /*========================================*/
   /* If multifields are allowed, then union */
   /* the constraint record for them.        */
   /*========================================*/

   if (rv->multifieldsAllowed)
     { rv->multifield = UnionConstraints(theEnv,c1->multifield,c2->multifield); }

   /*====================*/
   /* Return the unioned */
   /* constraint record. */
   /*====================*/

   return(rv);
  }

/**************************************************/
/* UnionNumericExpressions: Creates the union of  */
/*   two range or two min/max-fields constraints. */
/**************************************************/
static void UnionNumericExpressions(
  void *theEnv,
  CONSTRAINT_RECORD *constraint1,
  CONSTRAINT_RECORD *constraint2,
  CONSTRAINT_RECORD *newConstraint,
  int range)
  {
   struct expr *tmpmin, *tmpmax;
   struct expr *theMinList, *theMaxList;

   /*=========================================*/
   /* Initialize the new range/min/max values */
   /* for the union of the constraints.       */
   /*=========================================*/

   theMinList = NULL;
   theMaxList = NULL;

   /*=================================*/
   /* Determine the min/max values of */
   /* the first constraint record.    */
   /*=================================*/

   if (range)
     {
      tmpmin = constraint1->minValue;
      tmpmax = constraint1->maxValue;
     }
   else
     {
      tmpmin = constraint1->minFields;
      tmpmax = constraint1->maxFields;
     }

   /*============================================*/
   /* Add each range/min/max pair from the first */
   /* constraint record to the union list.       */
   /*============================================*/

   for (;
        tmpmin != NULL;
        tmpmin = tmpmin->nextArg,tmpmax = tmpmax->nextArg)
     { UnionRangeMinMaxValueWithList(theEnv,tmpmin,tmpmax,&theMinList,&theMaxList); }

   /*=================================*/
   /* Determine the min/max values of */
   /* the second constraint record.   */
   /*=================================*/

   if (range)
     {
      tmpmin = constraint2->minValue;
      tmpmax = constraint2->maxValue;
     }
   else
     {
      tmpmin = constraint2->minFields;
      tmpmax = constraint2->maxFields;
     }

   /*=============================================*/
   /* Add each range/min/max pair from the second */
   /* constraint record to the union list.        */
   /*=============================================*/

   for (;
        tmpmin != NULL;
        tmpmin = tmpmin->nextArg,tmpmax = tmpmax->nextArg)
     { UnionRangeMinMaxValueWithList(theEnv,tmpmin,tmpmax,&theMinList,&theMaxList); }

   /*=====================================================*/
   /* If the union produced a pair of valid range/min/max */
   /* values, then replace the previous values of the     */
   /* constraint record to the new unioned values.        */
   /*=====================================================*/

   if (theMinList != NULL)
     {
      if (range)
        {
         ReturnExpression(theEnv,newConstraint->minValue);
         ReturnExpression(theEnv,newConstraint->maxValue);
         newConstraint->minValue = theMinList;
         newConstraint->maxValue = theMaxList;
        }
      else
        {
         ReturnExpression(theEnv,newConstraint->minFields);
         ReturnExpression(theEnv,newConstraint->maxFields);
         newConstraint->minFields = theMinList;
         newConstraint->maxFields = theMaxList;
        }
     }

   /*==============================================================*/
   /* Otherwise, the union produced no valid range/min/max values. */
   /* For the range attribute, this means that no numbers can      */
   /* satisfy the constraint. For the min/max fields attribute, it */
   /* means that no value can satisfy the constraint.              */
   /*==============================================================*/

   else
     {
      if (range)
        {
         if (newConstraint->anyAllowed) SetAnyAllowedFlags(newConstraint,FALSE);
         newConstraint->integersAllowed = FALSE;
         newConstraint->floatsAllowed = FALSE;
        }
      else
        {
         SetAnyAllowedFlags(newConstraint,TRUE);
         newConstraint->anyAllowed = TRUE;
        }
     }
  }

/*********************************************************/
/* UnionRangeMinMaxValueWithList: Unions a range/min/max */
/*   pair of values with a list of such values.          */
/*********************************************************/
static void UnionRangeMinMaxValueWithList(
  void *theEnv,
  struct expr *addmin,
  struct expr *addmax,
  struct expr **theMinList,
  struct expr **theMaxList)
  {
   struct expr *tmpmin, *tmpmax, *lastmin, *lastmax;
   struct expr *themin, *themax, *nextmin, *nextmax;
   int cmaxmin, cmaxmax, cminmin, cminmax;

   /*=========================================================*/
   /* If no values are on the lists, then use the new values. */
   /*=========================================================*/

   if (*theMinList == NULL)
     {
      *theMinList = GenConstant(theEnv,addmin->type,addmin->value);
      *theMaxList = GenConstant(theEnv,addmax->type,addmax->value);
      return;
     }

   lastmin = NULL;
   lastmax = NULL;
   tmpmin = (*theMinList);
   tmpmax = (*theMaxList);

   while (tmpmin != NULL)
     {
      cmaxmax = CompareNumbers(theEnv,addmax->type,addmax->value,
                               tmpmax->type,tmpmax->value);

      cminmin = CompareNumbers(theEnv,addmin->type,addmin->value,
                               tmpmin->type,tmpmin->value);

      cmaxmin = CompareNumbers(theEnv,addmax->type,addmax->value,
                               tmpmin->type,tmpmin->value);

      cminmax = CompareNumbers(theEnv,addmin->type,addmin->value,
                               tmpmax->type,tmpmax->value);

      /*=================================*/
      /* Check to see if the range is    */
      /* contained within another range. */
      /*=================================*/

      if (((cmaxmax == LESS_THAN) || (cmaxmax == EQUAL)) &&
          ((cminmin == GREATER_THAN) || (cminmin == EQUAL)))
        { return; }

      /*================================*/
      /* Extend the greater than range. */
      /*================================*/

      if ((cmaxmax == GREATER_THAN) &&
          ((cminmax == LESS_THAN) || (cminmax == EQUAL)))
        {
         tmpmax->type = addmax->type;
         tmpmax->value = addmax->value;
        }

      /*=============================*/
      /* Extend the less than range. */
      /*=============================*/

      if ((cminmin == LESS_THAN) &&
          ((cmaxmin == GREATER_THAN) || (cmaxmin == EQUAL)))
        {
         tmpmin->type = addmin->type;
         tmpmin->value = addmin->value;
        }

      /*====================*/
      /* Handle insertions. */
      /*====================*/

      if (cmaxmin == LESS_THAN)
        {
         if (lastmax == NULL)
           {
            themin = GenConstant(theEnv,addmin->type,addmin->value);
            themax = GenConstant(theEnv,addmax->type,addmax->value);
            themin->nextArg = *theMinList;
            themax->nextArg = *theMaxList;
            *theMinList = themin;
            *theMaxList = themax;
            return;
           }

         if (CompareNumbers(theEnv,addmin->type,addmin->value,
                            lastmax->type,lastmax->value) == GREATER_THAN)
           {
            themin = GenConstant(theEnv,addmin->type,addmin->value);
            themax = GenConstant(theEnv,addmax->type,addmax->value);

            themin->nextArg = lastmin->nextArg;
            themax->nextArg = lastmax->nextArg;

            lastmin->nextArg = themin;
            lastmax->nextArg = themax;
            return;
           }
        }

      /*==========================*/
      /* Move on to the next one. */
      /*==========================*/

      tmpmin = tmpmin->nextArg;
      tmpmax = tmpmax->nextArg;
     }

   /*===========================*/
   /* Merge overlapping ranges. */
   /*===========================*/

   tmpmin = (*theMinList);
   tmpmax = (*theMaxList);

   while (tmpmin != NULL)
     {
      nextmin = tmpmin->nextArg;
      nextmax = tmpmax->nextArg;
      if (nextmin != NULL)
        {
         cmaxmin = CompareNumbers(theEnv,tmpmax->type,tmpmax->value,
                                  nextmin->type,nextmin->value);
         if ((cmaxmin == GREATER_THAN) || (cmaxmin == EQUAL))
           {
            tmpmax->type = nextmax->type;
            tmpmax->value = nextmax->value;
            tmpmax->nextArg = nextmax->nextArg;
            tmpmin->nextArg = nextmin->nextArg;

            rtn_struct(theEnv,expr,nextmin);
            rtn_struct(theEnv,expr,nextmax);
           }
         else
           {
            tmpmin = tmpmin->nextArg;
            tmpmax = tmpmax->nextArg;
           }
        }
      else
        {
         tmpmin = nextmin;
         tmpmax = nextmax;
        }
     }
  }

/***************************************************/
/* UnionAllowedClassExpressions: Creates the union */
/*   of two sets of allowed-classes expressions.   */
/***************************************************/
static void UnionAllowedClassExpressions(
  void *theEnv,
  CONSTRAINT_RECORD *constraint1,
  CONSTRAINT_RECORD *constraint2,
  CONSTRAINT_RECORD *newConstraint)
  {
   struct expr *theHead = NULL;

   theHead = AddToUnionList(theEnv,constraint1->classList,theHead,newConstraint);
   theHead = AddToUnionList(theEnv,constraint2->classList,theHead,newConstraint);

   newConstraint->classList = theHead;
  }

/***************************************************/
/* UnionAllowedValueExpressions: Creates the union */
/*   of two sets of allowed value expressions.     */
/***************************************************/
static void UnionAllowedValueExpressions(
  void *theEnv,
  CONSTRAINT_RECORD *constraint1,
  CONSTRAINT_RECORD *constraint2,
  CONSTRAINT_RECORD *newConstraint)
  {
   struct expr *theHead = NULL;

   theHead = AddToUnionList(theEnv,constraint1->restrictionList,theHead,newConstraint);
   theHead = AddToUnionList(theEnv,constraint2->restrictionList,theHead,newConstraint);

   newConstraint->restrictionList = theHead;
  }

/************************************************************/
/* AddToUnionList: Adds a list of values to a unioned list  */
/*   making sure that duplicates are not added and that any */
/*   value added satisfies the constraints for the list.    */
/************************************************************/
static struct expr *AddToUnionList(
  void *theEnv,
  struct expr *theList1,
  struct expr *theHead,
  CONSTRAINT_RECORD *theConstraint)
  {
   struct expr *theList2;
   int flag;

   /*======================================*/
   /* Loop through each value in the list  */
   /* being added to the unioned set.      */
   /*======================================*/

   for (;theList1 != NULL; theList1 = theList1->nextArg)
     {
      /*===================================*/
      /* Determine if the value is already */
      /* in the unioned list.              */
      /*===================================*/

      flag = TRUE;
      for (theList2 = theHead;
           theList2 != NULL;
           theList2 = theList2->nextArg)
        {
         if ((theList1->type == theList2->type) &&
             (theList1->value == theList2->value))
           {
            flag = FALSE;
            break;
           }
        }

      /*=====================================================*/
      /* If the value wasn't in the unioned list and doesn't */
      /* violate any of the unioned list's constraints, then */
      /* add it to the list.                                 */
      /*=====================================================*/

      if (flag)
        {
         if (RestrictionOnType(theList1->type,theConstraint))
           {
            theList2 = GenConstant(theEnv,theList1->type,theList1->value);
            theList2->nextArg = theHead;
            theHead = theList2;
           }
        }
     }

   /*==============================*/
   /* Return the new unioned list. */
   /*==============================*/

   return(theHead);
  }

/****************************************************/
/* RemoveConstantFromConstraint: Removes a constant */
/*   value (including any duplicates) from the      */
/*   restriction list of a constraint record.       */
/****************************************************/
globle void RemoveConstantFromConstraint(
  void *theEnv,
  int theType,
  void *theValue,
  CONSTRAINT_RECORD *theConstraint)
  {
   struct expr *theList, *lastOne = NULL, *tmpList;

   if (theConstraint == NULL) return;

   theList = theConstraint->restrictionList;
   theConstraint->restrictionList = NULL;

   while (theList != NULL)
     {
      if ((theList->type != theType) || (theList->value != theValue))
        {
         if (lastOne == NULL)
           { theConstraint->restrictionList = theList; }
         else
           { lastOne->nextArg = theList; }
         lastOne = theList;
         theList = theList->nextArg;
         lastOne->nextArg = NULL;
        }
      else
        {
         tmpList = theList;
         theList = theList->nextArg;
         tmpList->nextArg = NULL;
         ReturnExpression(theEnv,tmpList);
        }
     }

   UpdateRestrictionFlags(theConstraint);
  }

#endif /* (! BLOAD_ONLY) */

#endif /* (! RUN_TIME) */




