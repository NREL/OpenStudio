   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*              RULE CONSTRAINTS MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for detecting constraint       */
/*   conflicts in the LHS and RHS of rules.                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#define _RULECSTR_SOURCE_

#include "setup.h"

#if (! RUN_TIME) && (! BLOAD_ONLY) && DEFRULE_CONSTRUCT

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "analysis.h"
#include "cstrnchk.h"
#include "cstrnops.h"
#include "cstrnutl.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "prcdrpsr.h"
#include "reorder.h"
#include "router.h"
#include "rulepsr.h"

#include "rulecstr.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static intBool                 CheckForUnmatchableConstraints(void *,struct lhsParseNode *,int);
   static intBool                 MultifieldCardinalityViolation(void *,struct lhsParseNode *);
   static struct lhsParseNode    *UnionVariableConstraints(void *,struct lhsParseNode *,
                                                     struct lhsParseNode *);
   static struct lhsParseNode    *AddToVariableConstraints(void *,struct lhsParseNode *,
                                                    struct lhsParseNode *);
   static void                    ConstraintConflictMessage(void *,struct symbolHashNode *,
                                                            int,int,struct symbolHashNode *);
   static intBool                 CheckArgumentForConstraintError(void *,struct expr *,struct expr*,
                                                                  int,struct FunctionDefinition *,
                                                                  struct lhsParseNode *);

/***********************************************************/
/* CheckForUnmatchableConstraints: Determines if a LHS CE  */
/*   node contains unmatchable constraints. Return TRUE if */
/*   there are unmatchable constraints, otherwise FALSE.   */
/***********************************************************/
static intBool CheckForUnmatchableConstraints(
  void *theEnv,
  struct lhsParseNode *theNode,
  int whichCE)
  {
   if (EnvGetStaticConstraintChecking(theEnv) == FALSE) return(FALSE);

   if (UnmatchableConstraint(theNode->constraints))
     {
      ConstraintConflictMessage(theEnv,(SYMBOL_HN *) theNode->value,whichCE,
                                theNode->index,theNode->slot);
      return(TRUE);
     }

   return(FALSE);
  }

/******************************************************/
/* ConstraintConflictMessage: Error message used when */
/*   a constraint restriction for a slot prevents any */
/*   value from matching the pattern constraint.      */
/******************************************************/
static void ConstraintConflictMessage(
  void *theEnv,
  struct symbolHashNode *variableName,
  int thePattern,
  int theField,
  struct symbolHashNode *theSlot)
  {
   /*=========================*/
   /* Print the error header. */
   /*=========================*/

   PrintErrorID(theEnv,"RULECSTR",1,TRUE);

   /*======================================================*/
   /* Print the variable name (if available) and CE number */
   /* for which the constraint violation occurred.         */
   /*======================================================*/

   if (variableName != NULL)
     {
      EnvPrintRouter(theEnv,WERROR,"Variable ?");
      EnvPrintRouter(theEnv,WERROR,ValueToString(variableName));
      EnvPrintRouter(theEnv,WERROR," in CE #");
      PrintLongInteger(theEnv,WERROR,(long int) thePattern);
     }
   else
     {
      EnvPrintRouter(theEnv,WERROR,"Pattern #");
      PrintLongInteger(theEnv,WERROR,(long int) thePattern);
     }

   /*=======================================*/
   /* Print the slot name or field position */
   /* in which the violation occurred.      */
   /*=======================================*/

   if (theSlot == NULL)
     {
      EnvPrintRouter(theEnv,WERROR," field #");
      PrintLongInteger(theEnv,WERROR,(long int) theField);
     }
   else
     {
      EnvPrintRouter(theEnv,WERROR," slot ");
      EnvPrintRouter(theEnv,WERROR,ValueToString(theSlot));
     }

   /*======================================*/
   /* Print the rest of the error message. */
   /*======================================*/

   EnvPrintRouter(theEnv,WERROR,"\nhas constraint conflicts which make the pattern unmatchable.\n");
  }

/***************************************************************/
/* MultifieldCardinalityViolation: Determines if a cardinality */
/*   violation has occurred for a LHS CE node.                 */
/***************************************************************/
static intBool MultifieldCardinalityViolation(
  void *theEnv,
  struct lhsParseNode *theNode)
  {
   struct lhsParseNode *tmpNode;
   struct expr *tmpMax;
   long minFields = 0;
   long maxFields = 0;
   int posInfinity = FALSE;
   CONSTRAINT_RECORD *newConstraint, *tempConstraint;

   /*================================*/
   /* A single field slot can't have */
   /* a cardinality violation.       */
   /*================================*/

   if (theNode->multifieldSlot == FALSE) return(FALSE);

   /*=============================================*/
   /* Determine the minimum and maximum number of */
   /* fields the slot could contain based on the  */
   /* slot constraints found in the pattern.      */
   /*=============================================*/

   for (tmpNode = theNode->bottom;
        tmpNode != NULL;
        tmpNode = tmpNode->right)
     {
      /*====================================================*/
      /* A single field variable increases both the minimum */
      /* and maximum number of fields by one.               */
      /*====================================================*/

      if ((tmpNode->type == SF_VARIABLE) ||
          (tmpNode->type == SF_WILDCARD))
        {
         minFields++;
         maxFields++;
        }

      /*=================================================*/
      /* Otherwise a multifield wildcard or variable has */
      /* been encountered. If it is constrained then use */
      /* minimum and maximum number of fields constraint */
      /* associated with this LHS node.                  */
      /*=================================================*/

      else if (tmpNode->constraints != NULL)
        {
         /*=======================================*/
         /* The lowest minimum of all the min/max */
         /* pairs will be the first in the list.  */
         /*=======================================*/

         if (tmpNode->constraints->minFields->value != SymbolData(theEnv)->NegativeInfinity)
           { minFields += (long) ValueToLong(tmpNode->constraints->minFields->value); }

         /*=========================================*/
         /* The greatest maximum of all the min/max */
         /* pairs will be the last in the list.     */
         /*=========================================*/

         tmpMax = tmpNode->constraints->maxFields;
         while (tmpMax->nextArg != NULL) tmpMax = tmpMax->nextArg;
         if (tmpMax->value == SymbolData(theEnv)->PositiveInfinity)
           { posInfinity = TRUE; }
         else
           { maxFields += (long) ValueToLong(tmpMax->value); }
        }

      /*================================================*/
      /* Otherwise an unconstrained multifield wildcard */
      /* or variable increases the maximum number of    */
      /* fields to positive infinity.                   */
      /*================================================*/

      else
        { posInfinity = TRUE; }
     }

   /*==================================================================*/
   /* Create a constraint record for the cardinality of the sum of the */
   /* cardinalities of the restrictions inside the multifield slot.    */
   /*==================================================================*/

   if (theNode->constraints == NULL) tempConstraint = GetConstraintRecord(theEnv);
   else tempConstraint = CopyConstraintRecord(theEnv,theNode->constraints);
   ReturnExpression(theEnv,tempConstraint->minFields);
   ReturnExpression(theEnv,tempConstraint->maxFields);
   tempConstraint->minFields = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,(long long) minFields));
   if (posInfinity) tempConstraint->maxFields = GenConstant(theEnv,SYMBOL,SymbolData(theEnv)->PositiveInfinity);
   else tempConstraint->maxFields = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,(long long) maxFields));

   /*================================================================*/
   /* Determine the final cardinality for the multifield slot by     */
   /* intersecting the cardinality sum of the restrictions within    */
   /* the multifield slot with the original cardinality of the slot. */
   /*================================================================*/

   newConstraint = IntersectConstraints(theEnv,theNode->constraints,tempConstraint);
   if (theNode->derivedConstraints) RemoveConstraint(theEnv,theNode->constraints);
   RemoveConstraint(theEnv,tempConstraint);
   theNode->constraints = newConstraint;
   theNode->derivedConstraints = TRUE;

   /*===================================================================*/
   /* Determine if the final cardinality for the slot can be satisfied. */
   /*===================================================================*/

   if (EnvGetStaticConstraintChecking(theEnv) == FALSE) return(FALSE);
   if (UnmatchableConstraint(newConstraint)) return(TRUE);

   return(FALSE);
  }

/***************************************************/
/* ProcessConnectedConstraints: Examines a single  */
/*   connected constraint searching for constraint */
/*   violations.                                   */
/***************************************************/
globle intBool ProcessConnectedConstraints(
  void *theEnv,
  struct lhsParseNode *theNode,
  struct lhsParseNode *multifieldHeader,
  struct lhsParseNode *patternHead)
  {
   struct constraintRecord *orConstraints = NULL, *andConstraints;
   struct constraintRecord *tmpConstraints, *rvConstraints;
   struct lhsParseNode *orNode, *andNode;
   struct expr *tmpExpr;

   /*============================================*/
   /* Loop through all of the or (|) constraints */
   /* found in the connected constraint.         */
   /*============================================*/

   for (orNode = theNode->bottom; orNode != NULL; orNode = orNode->bottom)
     {
      /*=================================================*/
      /* Intersect all of the &'ed constraints together. */
      /*=================================================*/

      andConstraints = NULL;
      for (andNode = orNode; andNode != NULL; andNode = andNode->right)
        {
         if (! andNode->negated)
           {
            if (andNode->type == RETURN_VALUE_CONSTRAINT)
              {
               if (andNode->expression->type == FCALL)
                 {
                  rvConstraints = FunctionCallToConstraintRecord(theEnv,andNode->expression->value);
                  tmpConstraints = andConstraints;
                  andConstraints = IntersectConstraints(theEnv,andConstraints,rvConstraints);
                  RemoveConstraint(theEnv,tmpConstraints);
                  RemoveConstraint(theEnv,rvConstraints);
                 }
              }
            else if (ConstantType(andNode->type))
              {
               tmpExpr = GenConstant(theEnv,andNode->type,andNode->value);
               rvConstraints = ExpressionToConstraintRecord(theEnv,tmpExpr);
               tmpConstraints = andConstraints;
               andConstraints = IntersectConstraints(theEnv,andConstraints,rvConstraints);
               RemoveConstraint(theEnv,tmpConstraints);
               RemoveConstraint(theEnv,rvConstraints);
               ReturnExpression(theEnv,tmpExpr);
              }
            else if (andNode->constraints != NULL)
              {
               tmpConstraints = andConstraints;
               andConstraints = IntersectConstraints(theEnv,andConstraints,andNode->constraints);
               RemoveConstraint(theEnv,tmpConstraints);
              }
           }
        }

      /*===========================================================*/
      /* Intersect the &'ed constraints with the slot constraints. */
      /*===========================================================*/

      tmpConstraints = andConstraints;
      andConstraints = IntersectConstraints(theEnv,andConstraints,theNode->constraints);
      RemoveConstraint(theEnv,tmpConstraints);

      /*===============================================================*/
      /* Remove any negated constants from the list of allowed values. */
      /*===============================================================*/

      for (andNode = orNode; andNode != NULL; andNode = andNode->right)
        {
         if ((andNode->negated) && ConstantType(andNode->type))
             { RemoveConstantFromConstraint(theEnv,andNode->type,andNode->value,andConstraints); }
        }

      /*=======================================================*/
      /* Union the &'ed constraints with the |'ed constraints. */
      /*=======================================================*/

      tmpConstraints = orConstraints;
      orConstraints = UnionConstraints(theEnv,orConstraints,andConstraints);
      RemoveConstraint(theEnv,tmpConstraints);
      RemoveConstraint(theEnv,andConstraints);
     }

   /*===============================================*/
   /* Replace the constraints for the slot with the */
   /* constraints derived from the connected        */
   /* constraints (which should be a subset.        */
   /*===============================================*/

   if (orConstraints != NULL)
     {
      if (theNode->derivedConstraints) RemoveConstraint(theEnv,theNode->constraints);
      theNode->constraints = orConstraints;
      theNode->derivedConstraints = TRUE;
     }

   /*==================================*/
   /* Check for constraint violations. */
   /*==================================*/

   if (CheckForUnmatchableConstraints(theEnv,theNode,(int) patternHead->whichCE))
     { return(TRUE); }

   /*=========================================*/
   /* If the constraints are for a multifield */
   /* slot, check for cardinality violations. */
   /*=========================================*/

   if ((multifieldHeader != NULL) && (theNode->right == NULL))
     {
      if (MultifieldCardinalityViolation(theEnv,multifieldHeader))
        {
         ConstraintViolationErrorMessage(theEnv,"The group of restrictions",
                                                  NULL,FALSE,
                                                  (int) patternHead->whichCE,
                                                  multifieldHeader->slot,
                                                  multifieldHeader->index,
                                                  CARDINALITY_VIOLATION,
                                                  multifieldHeader->constraints,TRUE);
          return(TRUE);
         }
      }

   /*=======================================*/
   /* Return FALSE indicating no constraint */
   /* violations were detected.             */
   /*=======================================*/

   return(FALSE);
  }

/**************************************************/
/* ConstraintReferenceErrorMessage: Generic error */
/*   message for LHS constraint violation errors  */
/*   that occur within an expression.             */
/**************************************************/
globle void ConstraintReferenceErrorMessage(
  void *theEnv,
  struct symbolHashNode *theVariable,
  struct lhsParseNode *theExpression,
  int whichArgument,
  int whichCE,
  struct symbolHashNode *slotName,
  int theField)
  {
   struct expr *temprv;

   PrintErrorID(theEnv,"RULECSTR",2,TRUE);

   /*==========================*/
   /* Print the variable name. */
   /*==========================*/

   EnvPrintRouter(theEnv,WERROR,"Previous variable bindings of ?");
   EnvPrintRouter(theEnv,WERROR,ValueToString(theVariable));
   EnvPrintRouter(theEnv,WERROR," caused the type restrictions");

   /*============================*/
   /* Print the argument number. */
   /*============================*/

   EnvPrintRouter(theEnv,WERROR,"\nfor argument #");
   PrintLongInteger(theEnv,WERROR,(long int) whichArgument);

   /*=======================*/
   /* Print the expression. */
   /*=======================*/

   EnvPrintRouter(theEnv,WERROR," of the expression ");
   temprv = LHSParseNodesToExpression(theEnv,theExpression);
   ReturnExpression(theEnv,temprv->nextArg);
   temprv->nextArg = NULL;
   PrintExpression(theEnv,WERROR,temprv);
   EnvPrintRouter(theEnv,WERROR,"\n");
   ReturnExpression(theEnv,temprv);

   /*========================================*/
   /* Print out the index of the conditional */
   /* element and the slot name or field     */
   /* index where the violation occured.     */
   /*========================================*/

   EnvPrintRouter(theEnv,WERROR,"found in CE #");
   PrintLongInteger(theEnv,WERROR,(long int) whichCE);
   if (slotName == NULL)
     {
      if (theField > 0)
        {
         EnvPrintRouter(theEnv,WERROR," field #");
         PrintLongInteger(theEnv,WERROR,(long int) theField);
        }
     }
   else
     {
      EnvPrintRouter(theEnv,WERROR," slot ");
      EnvPrintRouter(theEnv,WERROR,ValueToString(slotName));
     }

   EnvPrintRouter(theEnv,WERROR," to be violated.\n");
  }

/********************************************************/
/* AddToVariableConstraints: Adds the constraints for a */
/*   variable to a list of constraints. If the variable */
/*   is already in the list, the constraints for the    */
/*   variable are intersected with the new constraints. */
/********************************************************/
static struct lhsParseNode *AddToVariableConstraints(
  void *theEnv,
  struct lhsParseNode *oldList,
  struct lhsParseNode *newItems)
  {
   CONSTRAINT_RECORD *newConstraints;
   struct lhsParseNode *temp, *trace;

   /*=================================================*/
   /* Loop through each of the new constraints adding */
   /* it to the list if it's not already present or   */
   /* modifying the constraint if it is.              */
   /*=================================================*/

   while (newItems != NULL)
     {
      /*==========================================*/
      /* Get the next item since the next pointer */
      /* value (right) needs to be set to NULL.   */
      /*==========================================*/

      temp = newItems->right;
      newItems->right = NULL;

      /*===================================*/
      /* Search the list for the variable. */
      /*===================================*/

      for (trace = oldList; trace != NULL; trace = trace->right)
        {
         /*=========================================*/
         /* If the variable is already in the list, */
         /* modify the constraint already there to  */
         /* include the new constraint.             */
         /*=========================================*/

         if (trace->value == newItems->value)
           {
            newConstraints = IntersectConstraints(theEnv,trace->constraints,
                                                  newItems->constraints);
            RemoveConstraint(theEnv,trace->constraints);
            trace->constraints = newConstraints;
            ReturnLHSParseNodes(theEnv,newItems);
            break;
           }
        }

      /*=================================*/
      /* Add the variable constraints to */
      /* the list if it wasn't found.    */
      /*=================================*/

      if (trace == NULL)
        {
         newItems->right = oldList;
         oldList = newItems;
        }

      /*===========================*/
      /* Move on to the next item. */
      /*===========================*/

      newItems = temp;
     }

   return(oldList);
  }

/***********************************************************/
/* UnionVariableConstraints: Unions two lists of variable  */
/*   constraints. If a variable appears in one list  but   */
/*   not the other, then the variable is unconstrained and */
/*   thus not included in the unioned list.                */
/***********************************************************/
static struct lhsParseNode *UnionVariableConstraints(
  void *theEnv,
  struct lhsParseNode *list1,
  struct lhsParseNode *list2)
  {
   struct lhsParseNode *list3 = NULL, *trace, *temp;

   /*===================================*/
   /* Loop through all of the variables */
   /* in the first list.                */
   /*===================================*/

   while (list1 != NULL)
     {
      /*=============================================*/
      /* Search for the variable in the second list. */
      /*=============================================*/

      for (trace = list2; trace != NULL; trace = trace->right)
        {
         /*============================================*/
         /* If the variable is found in both lists,    */
         /* union the constraints and add the variable */
         /* to the new list being constructed.         */
         /*============================================*/

         if (list1->value == trace->value)
           {
            temp = GetLHSParseNode(theEnv);
            temp->derivedConstraints = TRUE;
            temp->value = list1->value;
            temp->constraints = UnionConstraints(theEnv,list1->constraints,trace->constraints);
            temp->right = list3;
            list3 = temp;
            break;
           }
        }

      /*==============================*/
      /* Move on to the next variable */
      /* in the first list.           */
      /*==============================*/

      temp = list1->right;
      list1->right = NULL;
      ReturnLHSParseNodes(theEnv,list1);
      list1 = temp;
     }

   /*====================================*/
   /* Free the items in the second list. */
   /*====================================*/

   ReturnLHSParseNodes(theEnv,list2);

   /*======================*/
   /* Return the new list. */
   /*======================*/

   return(list3);
  }

/*****************************************************************/
/* GetExpressionVarConstraints: Given an expression stored using */
/*   the LHS parse node data structures, determines and returns  */
/*   the constraints on variables caused by that expression. For */
/*   example, the expression (+ ?x 1) would imply a numeric type */
/*   constraint for the variable ?x since the addition function  */
/*   expects numeric arguments.                                  */
/*****************************************************************/
globle struct lhsParseNode *GetExpressionVarConstraints(
  void *theEnv,
  struct lhsParseNode *theExpression)
  {
   struct lhsParseNode *list1 = NULL, *list2;

   for (; theExpression != NULL; theExpression = theExpression->bottom)
     {
      if (theExpression->right != NULL)
        {
         list2 = GetExpressionVarConstraints(theEnv,theExpression->right);
         list1 = AddToVariableConstraints(theEnv,list2,list1);
        }

      if (theExpression->type == SF_VARIABLE)
        {
         list2 = GetLHSParseNode(theEnv);
         if (theExpression->referringNode != NULL)
           { list2->type = theExpression->referringNode->type; }
         else
           { list2->type = SF_VARIABLE; }
         list2->value = theExpression->value;
         list2->derivedConstraints = TRUE;
         list2->constraints = CopyConstraintRecord(theEnv,theExpression->constraints);
         list1 = AddToVariableConstraints(theEnv,list2,list1);
        }
     }

   return(list1);
  }

/***********************************************/
/* DeriveVariableConstraints: Derives the list */
/*   of variable constraints associated with a */
/*   single connected constraint.              */
/***********************************************/
globle struct lhsParseNode *DeriveVariableConstraints(
  void *theEnv,
  struct lhsParseNode *theNode)
  {
   struct lhsParseNode *orNode, *andNode;
   struct lhsParseNode *list1, *list2, *list3 = NULL;
   int first = TRUE;

   /*===============================*/
   /* Process the constraints for a */
   /* single connected constraint.  */
   /*===============================*/

   for (orNode = theNode->bottom; orNode != NULL; orNode = orNode->bottom)
     {
      /*=================================================*/
      /* Intersect all of the &'ed constraints together. */
      /*=================================================*/

      list2 = NULL;
      for (andNode = orNode; andNode != NULL; andNode = andNode->right)
        {
         if ((andNode->type == RETURN_VALUE_CONSTRAINT) ||
             (andNode->type == PREDICATE_CONSTRAINT))
           {
            list1 = GetExpressionVarConstraints(theEnv,andNode->expression);
            list2 = AddToVariableConstraints(theEnv,list2,list1);
           }
        }

      if (first)
        {
         list3 = list2;
         first = FALSE;
        }
      else
        { list3 = UnionVariableConstraints(theEnv,list3,list2); }
     }

   return(list3);
  }

/*******************************************/
/* CheckRHSForConstraintErrors: Checks the */
/*   RHS of a rule for constraint errors.  */
/*******************************************/
globle intBool CheckRHSForConstraintErrors(
  void *theEnv,
  struct expr *expressionList,
  struct lhsParseNode *theLHS)
  {
   struct FunctionDefinition *theFunction;
   int i;
   struct expr *lastOne = NULL, *checkList, *tmpPtr;

   if (expressionList == NULL) return(FALSE);

   for (checkList = expressionList;
        checkList != NULL;
        checkList = checkList->nextArg)
      {
       expressionList = checkList->argList;
       i = 1;
       if (checkList->type == FCALL)
         {
          lastOne = checkList;
          theFunction = (struct FunctionDefinition *) checkList->value;
         }
       else
         { theFunction = NULL; }

       while (expressionList != NULL)
         {
          if (CheckArgumentForConstraintError(theEnv,expressionList,lastOne,i,
                                              theFunction,theLHS))
            { return(TRUE); }

          i++;
          tmpPtr = expressionList->nextArg;
          expressionList->nextArg = NULL;
          if (CheckRHSForConstraintErrors(theEnv,expressionList,theLHS)) return(TRUE);
          expressionList->nextArg = tmpPtr;
          expressionList = expressionList->nextArg;
         }
      }

   return(FALSE);
  }

/*************************************************************/
/* CheckArgumentForConstraintError: Checks a single argument */
/*   found in the RHS of a rule for constraint errors.       */
/*   Returns TRUE if an error is detected, otherwise FALSE.  */
/*************************************************************/
static intBool CheckArgumentForConstraintError(
  void *theEnv,
  struct expr *expressionList,
  struct expr *lastOne,
  int i,
  struct FunctionDefinition *theFunction,
  struct lhsParseNode *theLHS)
  {
   int theRestriction;
   CONSTRAINT_RECORD *constraint1, *constraint2, *constraint3, *constraint4;
   struct lhsParseNode *theVariable;
   struct expr *tmpPtr;
   int rv = FALSE;

   /*=============================================================*/
   /* Skip anything that isn't a variable or isn't an argument to */
   /* a user defined function (i.e. deffunctions and generic have */
   /* no constraint information so they aren't checked).          */
   /*=============================================================*/

   if ((expressionList->type != SF_VARIABLE) || (theFunction == NULL))
     { return (rv); }

   /*===========================================*/
   /* Get the restrictions for the argument and */
   /* convert them to a constraint record.      */
   /*===========================================*/

   theRestriction = GetNthRestriction(theFunction,i);
   constraint1 = ArgumentTypeToConstraintRecord(theEnv,theRestriction);

   /*================================================*/
   /* Look for the constraint record associated with */
   /* binding the variable in the LHS of the rule.   */
   /*================================================*/

   theVariable = FindVariable((SYMBOL_HN *) expressionList->value,theLHS);
   if (theVariable != NULL)
     {
      if (theVariable->type == MF_VARIABLE)
        {
         constraint2 = GetConstraintRecord(theEnv);
         SetConstraintType(MULTIFIELD,constraint2);
        }
      else if (theVariable->constraints == NULL)
        { constraint2 = GetConstraintRecord(theEnv); }
      else
        { constraint2 = CopyConstraintRecord(theEnv,theVariable->constraints); }
     }
   else
     { constraint2 = NULL; }

   /*================================================*/
   /* Look for the constraint record associated with */
   /* binding the variable on the RHS of the rule.   */
   /*================================================*/

   constraint3 = FindBindConstraints(theEnv,(SYMBOL_HN *) expressionList->value);

   /*====================================================*/
   /* Union the LHS and RHS variable binding constraints */
   /* (the variable must satisfy one or the other).      */
   /*====================================================*/

   constraint3 = UnionConstraints(theEnv,constraint3,constraint2);

   /*====================================================*/
   /* Intersect the LHS/RHS variable binding constraints */
   /* with the function argument restriction constraints */
   /* (the variable must satisfy both).                  */
   /*====================================================*/

   constraint4 = IntersectConstraints(theEnv,constraint3,constraint1);

   /*====================================*/
   /* Check for unmatchable constraints. */
   /*====================================*/

   if (UnmatchableConstraint(constraint4) && EnvGetStaticConstraintChecking(theEnv))
     {
      PrintErrorID(theEnv,"RULECSTR",3,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Previous variable bindings of ?");
      EnvPrintRouter(theEnv,WERROR,ValueToString((SYMBOL_HN *) expressionList->value));
      EnvPrintRouter(theEnv,WERROR," caused the type restrictions");
      EnvPrintRouter(theEnv,WERROR,"\nfor argument #");
      PrintLongInteger(theEnv,WERROR,(long int) i);
      EnvPrintRouter(theEnv,WERROR," of the expression ");
      tmpPtr = lastOne->nextArg;
      lastOne->nextArg = NULL;
      PrintExpression(theEnv,WERROR,lastOne);
      lastOne->nextArg = tmpPtr;
      EnvPrintRouter(theEnv,WERROR,"\nfound in the rule's RHS to be violated.\n");

      rv = TRUE;
     }

   /*===========================================*/
   /* Free the temporarily created constraints. */
   /*===========================================*/

   RemoveConstraint(theEnv,constraint1);
   RemoveConstraint(theEnv,constraint2);
   RemoveConstraint(theEnv,constraint3);
   RemoveConstraint(theEnv,constraint4);

   /*========================================*/
   /* Return TRUE if unmatchable constraints */
   /* were detected, otherwise FALSE.        */
   /*========================================*/

   return(rv);
  }

#endif /* (! RUN_TIME) && (! BLOAD_ONLY) && DEFRULE_CONSTRUCT */
