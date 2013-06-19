   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                    REORDER MODULE                   */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines necessary for converting the   */
/*   the LHS of a rule into an appropriate form suitable for */
/*   the KB Rete topology. This includes transforming the    */
/*   LHS so there is at most one "or" CE (and this is the    */
/*   first CE of the LHS if it is used), adding initial      */
/*   patterns to the LHS (if no LHS is specified or a "test" */
/*   or "not" CE is the first pattern within an "and" CE),   */
/*   removing redundant CEs, and determining appropriate     */
/*   information on nesting for implementing joins from the  */
/*   right.                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*************************************************************/

#define _REORDER_SOURCE_

#include "setup.h"

#if (! RUN_TIME) && (! BLOAD_ONLY) && DEFRULE_CONSTRUCT

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "cstrnutl.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "memalloc.h"
#include "pattern.h"
#include "prntutil.h"
#include "router.h"
#include "rulelhs.h"

#include "reorder.h"

struct variableReference
   { 
    struct symbolHashNode *name;
    int depth;
    struct variableReference *next;
   };
 
struct groupReference
   { 
    struct lhsParseNode *theGroup;
    int depth;
    struct groupReference *next;
   };
     
/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static struct lhsParseNode    *ReverseAndOr(void *,struct lhsParseNode *,struct lhsParseNode *,int);
   static struct lhsParseNode    *PerformReorder1(void *,struct lhsParseNode *,int *);
   static struct lhsParseNode    *PerformReorder2(void *,struct lhsParseNode *,int *);
   static struct lhsParseNode    *CompressCEs(void *,struct lhsParseNode *,int *);
   static void                    IncrementNandDepth(void *,struct lhsParseNode *,int);
   static struct lhsParseNode    *CreateInitialPattern(void *);
   static struct lhsParseNode    *ReorderDriver(void *,struct lhsParseNode *,int *,int);
   static struct lhsParseNode    *AddRemainingInitialPatterns(void *,struct lhsParseNode *);
   static void                    PrintNodes(void *,char *,struct lhsParseNode *);
   static struct lhsParseNode    *AssignPatternIndices(struct lhsParseNode *,short,int);
   static void                    PropagateIndexSlotPatternValues(struct lhsParseNode *,
                                                                  short,short,
                                                                  struct symbolHashNode *,
                                                                  short);
   static void                    PropagateJoinDepth(struct lhsParseNode *,short);
   static void                    PropagateNandDepth(struct lhsParseNode *,int,int);
   static void                    PropagateNandDepth2( struct lhsParseNode *,int,int);
   static intBool                 AddNandPatterns(void *,int,struct lhsParseNode *,struct lhsParseNode *,
                                                  struct lhsParseNode *,struct variableReference *,
                                                  struct groupReference *);
   static int                     VariableDepth(void *,struct variableReference *);
   static void                    InsertNandPatterns(void *,struct lhsParseNode *,struct lhsParseNode *,
                                                     struct lhsParseNode *,int);
   static void                    MarkExistsNands(struct lhsParseNode *);
   static intBool                 AddNandPatternsForTestCE(int,struct lhsParseNode *,struct variableReference *);
   static intBool                 AddNandPatternsForSubconstraints(int,struct lhsParseNode *,struct variableReference *);

/********************************************/
/* ReorderPatterns: Reorders a group of CEs */     
/*   to accommodate KB Rete topology.       */
/********************************************/
globle struct lhsParseNode *ReorderPatterns(
  void *theEnv,
  struct lhsParseNode *theLHS,
  int *anyChange)
  {
   struct lhsParseNode *newLHS, *patternPtr, *tempLHS, *lastLHS;
   unsigned int whichCE;

   /*=============================================*/
   /* If the LHS of the rule was left unspecified */
   /* (e.g., (defrule x => ...)), then nothing    */
   /* more needs to be done.                      */
   /*=============================================*/

   if (theLHS == NULL) return(theLHS);
   
   /*===========================================================*/
   /* The LHS of a rule is enclosed within an implied "and" CE. */
   /*===========================================================*/

   newLHS = GetLHSParseNode(theEnv);
   newLHS->type = AND_CE;
   newLHS->right = theLHS;

   /*=======================================================*/
   /* Reorder the patterns to support the KB Rete topology. */
   /*=======================================================*/

   newLHS = ReorderDriver(theEnv,newLHS,anyChange,1);
   newLHS = ReorderDriver(theEnv,newLHS,anyChange,2);

   /*===========================================*/
   /* The top level and CE may have disappeared */
   /* as a result of pattern compression.       */
   /*===========================================*/

   if (newLHS->type == OR_CE)
     {
      for (tempLHS = newLHS->right, lastLHS = NULL;
           tempLHS != NULL;
           lastLHS = tempLHS, tempLHS = tempLHS->bottom)
        {
         if (tempLHS->type != AND_CE)
           {
            theLHS = GetLHSParseNode(theEnv);
            theLHS->type = AND_CE;
            theLHS->right = tempLHS;
            theLHS->bottom = tempLHS->bottom;
            tempLHS->bottom = NULL;
            if (lastLHS == NULL)
              { newLHS->right = theLHS; }
            else
              { lastLHS->bottom = theLHS; }
            tempLHS = theLHS;
           }
        }
     }
   else if (newLHS->type != AND_CE)
     {
      theLHS = newLHS;
      newLHS = GetLHSParseNode(theEnv);
      newLHS->type = AND_CE;
      newLHS->right = theLHS;
     }

   /*==============================================*/
   /* Add patterns if necessary to not/and groups. */
   /*==============================================*/

   if (newLHS->type == OR_CE)
     {
      for (theLHS = newLHS->right;
           theLHS != NULL;
           theLHS = theLHS->bottom)
        { 
         MarkExistsNands(theLHS->right);
         AddNandPatterns(theEnv,1,NULL,theLHS->right,theLHS->right,NULL,NULL); 
        }
     }
   else
     {
      MarkExistsNands(newLHS->right); 
      AddNandPatterns(theEnv,1,NULL,newLHS->right,newLHS->right,NULL,NULL);
     }

   /*=====================================================*/
   /* Add initial patterns where needed (such as before a */
   /* "test" CE or "not" CE which is the first CE within  */
   /* an "and" CE).                                       */
   /*=====================================================*/

   AddInitialPatterns(theEnv,newLHS);

   /*===========================================================*/
   /* Number the user specified patterns. Patterns added while  */
   /* analyzing the rule (such as placing initial-fact patterns */
   /* before not CEs) are not numbered so that there is no      */
   /* confusion when an error message refers to a CE. Also      */
   /* propagate field and slot values throughout each pattern.  */
   /*===========================================================*/

   if (newLHS->type == OR_CE) theLHS = newLHS->right;
   else theLHS = newLHS;

   for (;
        theLHS != NULL;
        theLHS = theLHS->bottom)
     {
      whichCE = 1;

      for (patternPtr = theLHS->right;
           patternPtr != NULL;
           patternPtr = patternPtr->bottom)
        { if (patternPtr->userCE) patternPtr->whichCE = whichCE++; }

      AssignPatternIndices(theLHS->right,1,1);
     }

   /*===========================*/
   /* Return the processed LHS. */
   /*===========================*/

   return(newLHS);
  }

/******************************************/
/* ReorderDriver: Reorders a group of CEs */
/*   to accommodate KB Rete topology.     */
/******************************************/
static struct lhsParseNode *ReorderDriver(
  void *theEnv,
  struct lhsParseNode *theLHS,
  int *anyChange,
  int pass)
  {
   struct lhsParseNode *argPtr;
   struct lhsParseNode *before, *save;
   int change, newChange;
   *anyChange = FALSE;

   /*===================================*/
   /* Continue processing the LHS until */
   /* no more changes have been made.   */
   /*===================================*/

   change = TRUE;
   while (change)
     {
      /*==================================*/
      /* No change yet on this iteration. */
      /*==================================*/

      change = FALSE;

      /*=======================================*/
      /* Reorder the current level of the LHS. */
      /*=======================================*/

      if ((theLHS->type == AND_CE) ||
          (theLHS->type == NOT_CE) ||
          (theLHS->type == OR_CE))
        {
         if (pass == 1) theLHS = PerformReorder1(theEnv,theLHS,&newChange);
         else theLHS = PerformReorder2(theEnv,theLHS,&newChange);

         if (newChange)
           {
            *anyChange = TRUE;
            change = TRUE;
           }

         theLHS = CompressCEs(theEnv,theLHS,&newChange);

         if (newChange)
           {
            *anyChange = TRUE;
            change = TRUE;
           }
        }

      /*=====================================================*/
      /* Recursively reorder CEs at lower levels in the LHS. */
      /*=====================================================*/

      before = NULL;
      argPtr = theLHS->right;

      while (argPtr != NULL)
        {
         /*==================================*/
         /* Remember the next CE to reorder. */
         /*==================================*/

         save = argPtr->bottom;

         /*============================================*/
         /* Reorder the current CE at the lower level. */
         /*============================================*/

         if ((argPtr->type == AND_CE) ||
             (argPtr->type == NOT_CE) ||
             (argPtr->type == OR_CE))
           {
            if (before == NULL)
              {
               argPtr->bottom = NULL;
               theLHS->right = ReorderDriver(theEnv,argPtr,&newChange,pass);
               theLHS->right->bottom = save;
               before = theLHS->right;
              }
            else
              {
               argPtr->bottom = NULL;
               before->bottom = ReorderDriver(theEnv,argPtr,&newChange,pass);
               before->bottom->bottom = save;
               before = before->bottom;
              }

            if (newChange)
              {
               *anyChange = TRUE;
               change = TRUE;
              }
           }
         else
           { before = argPtr; }

         /*====================================*/
         /* Move on to the next CE to reorder. */
         /*====================================*/

         argPtr = save;
        }
     }

   /*===========================*/
   /* Return the reordered LHS. */
   /*===========================*/

   return(theLHS);
  }

/*************************************************************/
/* MarkExistsNands:                */
/*************************************************************/
static void MarkExistsNands(
  struct lhsParseNode *theLHS)
  {      
   int currentDepth = 1;
   struct lhsParseNode *tmpLHS;
    
   while (theLHS != NULL)
     {
      if (IsExistsSubjoin(theLHS,currentDepth))
        {
         theLHS->existsNand = TRUE;
         
         for (tmpLHS = theLHS;
              tmpLHS != NULL;
              tmpLHS = tmpLHS->bottom)
           {
            tmpLHS->beginNandDepth--;
            if (tmpLHS->endNandDepth <= currentDepth)
              { break; }
            else
              { tmpLHS->endNandDepth--; }
           }
        }
      
      currentDepth = theLHS->endNandDepth;
      theLHS = theLHS->bottom;
     }
  }
      
/*************************************************************/
/* AddNandPatterns: Adds patterns to the beginning of a nand */
/*   CE group when a referenced variable falls is detected   */
/*   outside the prior enclosing group.                      */
/*************************************************************/
static intBool AddNandPatterns(
  void *theEnv,
  int depth,
  struct lhsParseNode *parent,
  struct lhsParseNode *firstPattern,
  struct lhsParseNode *theLHS,
  struct variableReference *variables,
  struct groupReference *groups)
  {      
   struct lhsParseNode *nandStart, *theField, *newParent, *subField;
   struct variableReference *startVariables, *tmpVar;
   intBool addPatterns = FALSE, returnAddPatterns = FALSE;
   struct groupReference *tmpGroup;
   int vdepth;
   
   nandStart = theLHS;
   startVariables = variables;
   newParent = parent;
   
   while (theLHS != NULL)
     {
      if (theLHS->beginNandDepth > depth)
        {
         /*==============================*/
         /* Process the next nand group. */
         /*==============================*/

         tmpGroup = get_struct(theEnv,groupReference);
         tmpGroup->depth = depth;
         tmpGroup->theGroup = theLHS;
         tmpGroup->next = groups;
         groups = tmpGroup;
         
         if (AddNandPatterns(theEnv,depth+1,newParent,firstPattern,theLHS,variables,groups))
           { addPatterns = TRUE; }

         groups = tmpGroup->next;
         rtn_struct(theEnv,variableReference,tmpGroup);
         
         /*===================================================*/
         /* Skip to the end of the nand group just processed. */
         /*===================================================*/
         
         while (theLHS->endNandDepth > depth)
           { theLHS = theLHS->bottom; }
        }
      else
        {
         /*===========================================*/
         /* Search the pattern for binding variables. */
         /*===========================================*/
        
         if (theLHS->type == PATTERN_CE)
           {            
            for (theField = theLHS->right; theField != NULL; theField = theField->right)
              {
               if (theField->multifieldSlot)
                 { subField = theField->bottom; }
               else
                 { subField = theField; }
                 
               while (subField != NULL)
                 {
                  if ((subField->type == SF_VARIABLE) || (subField->type == MF_VARIABLE))
                    {
                     vdepth = VariableDepth(subField->value,variables);
                  
                     if (vdepth != -1)
                       {
                        if (theLHS->negated)
                          {
                           if ((depth - vdepth) > 0)
                             { addPatterns = TRUE; }
                          }
                        else
                          {
                           if ((depth - vdepth) > 1)
                             { returnAddPatterns = TRUE; }
                          }
                       }
                  
                     tmpVar = get_struct(theEnv,variableReference);
                     tmpVar->depth = depth;
                     tmpVar->name = (struct symbolHashNode *) subField->value;
                     tmpVar->next = variables;
                     variables = tmpVar;
                    }

                  if (theLHS->negated)
                    {
                     if (AddNandPatternsForSubconstraints(depth+1,subField,variables))
                       { addPatterns = TRUE; }
                    }
                  else 
                    {
                     if (AddNandPatternsForSubconstraints(depth,subField,variables))
                       { returnAddPatterns = TRUE; }
                    }
                    
                  if (theField->multifieldSlot)
                    { subField = subField->right; }
                  else
                    { subField = NULL; }
                 }
              }
           }
         else if (theLHS->type == TEST_CE)
           {
            if (AddNandPatternsForTestCE(depth,theLHS->expression,variables))
              { returnAddPatterns = TRUE; }
           }
           
         /*=====================================================*/
         /* Return if the end of a nand group has been reached. */
         /*=====================================================*/
         
         if (theLHS->endNandDepth < depth)
           { 
            /*===================================================*/
            /* Deallocate variables detected in this nand group. */
            /*===================================================*/
            
            while (variables != startVariables)
              {
               tmpVar = variables->next;
               rtn_struct(theEnv,variableReference,variables);
               variables = tmpVar;
              }
              
            if (addPatterns)
              { InsertNandPatterns(theEnv,parent,firstPattern,nandStart,depth); }
              
            return(addPatterns || returnAddPatterns); 
           }
        }
      
      /*=============================================*/
      /* Move on the next pattern in the nand group. */
      /*=============================================*/
    
      newParent = theLHS;
      theLHS = theLHS->bottom;
     }

   if (addPatterns)
     { InsertNandPatterns(theEnv,parent,firstPattern,nandStart,depth); }

   /*===================================================*/
   /* Deallocate variables detected in this nand group. */
   /*===================================================*/

   while (variables != startVariables)
     {
      tmpVar = variables->next;
      rtn_struct(theEnv,variableReference,variables);
      variables = tmpVar;
     }
     
   return(addPatterns || returnAddPatterns);
  }

/******************************/
/* AddNandPatternsForTestCE:  */
/******************************/
static intBool AddNandPatternsForTestCE(
  int depth,
  struct lhsParseNode *testCE,
  struct variableReference *variables)
  {
   int vdepth;
   
   while (testCE != NULL)
     {
      if ((testCE->type == SF_VARIABLE) || (testCE->type == MF_VARIABLE))
        {
         vdepth = VariableDepth(testCE->value,variables);
                  
         if (vdepth != -1)
           {
            if ((depth - vdepth) > 1)
              { return(TRUE); }
           }
        }

      if ((testCE->bottom != NULL) &&
          (AddNandPatternsForTestCE(depth,testCE->bottom,variables)))
        { return(TRUE); }
        
      testCE = testCE->right;
     }
  
   return(FALSE);
  }
  
/**************************************/
/* AddNandPatternsForSubconstraints:  */
/**************************************/
static intBool AddNandPatternsForSubconstraints(
  int depth,
  struct lhsParseNode *theField,
  struct variableReference *variables)
  {
   struct lhsParseNode *andField;
   struct lhsParseNode *orField;
   int vdepth;

   /*=========================================*/
   /* Loop through each of the | constraints. */
   /*=========================================*/

   orField = theField->bottom;
   for (;
        orField != NULL;
        orField = orField->bottom)
     {
      /*=========================================*/
      /* Loop through each of the & constraints. */
      /*=========================================*/

      for (andField = orField;
           andField != NULL;
           andField = andField->right)
        {
         /*==========================================*/
         /* If a variable is found, determine if the */
         /* reference requires adding nand patterns. */
         /*==========================================*/

         if ((andField->type == SF_VARIABLE) || (andField->type == MF_VARIABLE))
           { 
            vdepth = VariableDepth(andField->value,variables);

            if (vdepth != -1)
              {
               if ((depth - vdepth) > 1)
                  { return(TRUE); }
              }
           }

         /*========================================================*/
         /* Check predicate and return value constraints to see if */
         /* the reference variables require adding nand patterns   */
         /*========================================================*/

         else if ((andField->type == PREDICATE_CONSTRAINT) ||
                  (andField->type == RETURN_VALUE_CONSTRAINT))
           {
            if (AddNandPatternsForTestCE(depth,andField->expression,variables))
              { return(TRUE); }
           }
        }
     }

   /*===========================================*/
   /* It is not necessary to add nand patterns. */
   /*===========================================*/

   return(FALSE);
  }

/*************************************************************/
/* InsertNandPatterns:                     */
/*************************************************************/
static void InsertNandPatterns(
  void *theEnv,
  struct lhsParseNode *parent,
  struct lhsParseNode *firstPattern,
  struct lhsParseNode *endPattern,
  int depth)
  {  
   struct lhsParseNode *theLHS, *newNode, *first = NULL, *last = NULL;
   struct lhsParseNode *theFinalGroup = NULL;
   int lastEnd = 1;
   intBool insideFinalGroup = FALSE;
   int minDepth, maxDepth;

   if (parent == NULL) return;
   
   /*=========================================================================*/
   /* Determine the highest level not/and group to which the pattern belongs. */
   /*=========================================================================*/

   for (theLHS = firstPattern; theLHS != endPattern; theLHS = theLHS->bottom)
     {
      if ((lastEnd == 1) && (theLHS->beginNandDepth > 1))
        { theFinalGroup = theLHS; }
        
      lastEnd = theLHS->endNandDepth;
     }

   if (lastEnd == 1)
     { theFinalGroup = endPattern; }
     
   minDepth = theFinalGroup->endNandDepth;
   maxDepth = theFinalGroup->beginNandDepth;
        
   for (theLHS = theFinalGroup; theLHS != endPattern; theLHS = theLHS->bottom)
     {
      if (theLHS->endNandDepth < minDepth)
        { minDepth = theLHS->endNandDepth; }
        
      if (theLHS->beginNandDepth > maxDepth)
        { maxDepth = theLHS->beginNandDepth; }
     }
     
   /* TBD Add code here to determine the block start. */
   
   for (theLHS = firstPattern; theLHS != endPattern; theLHS = theLHS->bottom)
     {
      if (theLHS == theFinalGroup)
        { insideFinalGroup = TRUE; }
        
      newNode = GetLHSParseNode(theEnv);
      CopyLHSParseNode(theEnv,newNode,theLHS,TRUE);
      newNode->right = CopyLHSParseNodes(theEnv,theLHS->right);
      newNode->value = NULL;

      if (insideFinalGroup)
        {
         newNode->beginNandDepth = depth + (theLHS->beginNandDepth - minDepth);
         newNode->endNandDepth = depth + (theLHS->endNandDepth - minDepth);
        }
      else
        {
         newNode->beginNandDepth = depth + (theLHS->beginNandDepth - firstPattern->beginNandDepth);
         newNode->endNandDepth = depth + (theLHS->endNandDepth - firstPattern->beginNandDepth);
        }
        
      if (first == NULL)
        { first = newNode; }
      else
        { last->bottom = newNode; }
        
      last = newNode;
     }
     
   first->existsNand = endPattern->existsNand;
   endPattern->existsNand = FALSE;
   
   parent->bottom = first;
   last->bottom = endPattern;
  }

/**************************************************************/
/* VariableDepth: Returns the most recent depth of a variable */
/*   or -1 if the variable has not yet been bound.            */
/**************************************************************/
static int VariableDepth(
  void *theName,
  struct variableReference *variables)
  {
   while (variables != NULL)
     {
      if (variables->name == theName)
        { return(variables->depth); }
        
      variables = variables->next;
     }
     
   return -1;
  }

/****************************************************************/
/* AddInitialPatterns: Add initial patterns to CEs where needed */
/*   (such as before a "test" CE or "not" CE which is the first */
/*   CE within an "and" CE).                                    */
/****************************************************************/
globle void AddInitialPatterns(
  void *theEnv,
  struct lhsParseNode *theLHS)
  {
   struct lhsParseNode *thePattern;

   /*====================================================*/
   /* If there are multiple disjuncts for the rule, then */
   /* add initial patterns to each disjunct separately.  */
   /*====================================================*/

   if (theLHS->type == OR_CE)
     {
      for (thePattern = theLHS->right;
           thePattern != NULL;
           thePattern = thePattern->bottom)
        { AddInitialPatterns(theEnv,thePattern); }

      return;
     }

   /*================================*/
   /* Handle the remaining patterns. */
   /*================================*/

   theLHS->right = AddRemainingInitialPatterns(theEnv,theLHS->right);
  }

/***********************************************************/
/* PerformReorder1: Reorders a group of CEs to accommodate */
/*   KB Rete topology. The first pass of this function     */
/*   transforms or CEs into equivalent forms.              */
/***********************************************************/
static struct lhsParseNode *PerformReorder1(
  void *theEnv,
  struct lhsParseNode *theLHS,
  int *newChange)
  {
   struct lhsParseNode *argPtr, *lastArg, *nextArg;
   struct lhsParseNode *tempArg, *newNode;
   int count;
   int change;

   /*======================================================*/
   /* Loop through the CEs as long as changes can be made. */
   /*======================================================*/

   change = TRUE;
   *newChange = FALSE;

   while (change)
     {
      change = FALSE;
      count = 1;
      lastArg = NULL;

      for (argPtr = theLHS->right;
           argPtr != NULL;)
        {
         /*=============================================================*/
         /* Convert and/or CE combinations into or/and CE combinations. */
         /*=============================================================*/

         if ((theLHS->type == AND_CE) && (argPtr->type == OR_CE))
           {
            theLHS = ReverseAndOr(theEnv,theLHS,argPtr->right,count);

            change = TRUE;
            *newChange = TRUE;
            break;
           }

         /*==============================================================*/
         /* Convert not/or CE combinations into and/not CE combinations. */
         /*==============================================================*/

         else if ((theLHS->type == NOT_CE) && (argPtr->type == OR_CE))
           {
            change = TRUE;
            *newChange = TRUE;

            tempArg = argPtr->right;

            argPtr->right = NULL;
            argPtr->bottom = NULL;
            ReturnLHSParseNodes(theEnv,argPtr);
            theLHS->type = AND_CE;
            theLHS->right = tempArg;

            while (tempArg != NULL)
              {
               newNode = GetLHSParseNode(theEnv);
               CopyLHSParseNode(theEnv,newNode,tempArg,FALSE);
               newNode->right = tempArg->right;
               newNode->bottom = NULL;

               tempArg->type = NOT_CE;
               tempArg->negated = FALSE;
               tempArg->exists = FALSE;
               tempArg->existsNand = FALSE;
               tempArg->logical = FALSE;
               tempArg->value = NULL;
               tempArg->expression = NULL;
               tempArg->right = newNode;

               tempArg = tempArg->bottom;
              }

            break;
           }

         /*=====================================*/
         /* Remove duplication of or CEs within */
         /* or CEs and and CEs within and CEs.  */
         /*=====================================*/

         else if (((theLHS->type == OR_CE) && (argPtr->type == OR_CE)) ||
                  ((theLHS->type == AND_CE) && (argPtr->type == AND_CE)))
           {
            if (argPtr->logical) theLHS->logical = TRUE;

            change = TRUE;
            *newChange = TRUE;
            tempArg = argPtr->right;
            nextArg = argPtr->bottom;
            argPtr->right = NULL;
            argPtr->bottom = NULL;
            ReturnLHSParseNodes(theEnv,argPtr);

            if (lastArg == NULL)
              { theLHS->right = tempArg; }
            else
              { lastArg->bottom = tempArg; }

            argPtr = tempArg;
            while (tempArg->bottom != NULL) tempArg = tempArg->bottom;
            tempArg->bottom = nextArg;
           }

         /*===================================================*/
         /* If no changes are needed, move on to the next CE. */
         /*===================================================*/

         else
           {
            count++;
            lastArg = argPtr;
            argPtr = argPtr->bottom;
           }
        }
     }

   /*===========================*/
   /* Return the reordered LHS. */
   /*===========================*/

   return(theLHS);
  }

/***********************************************************/
/* PerformReorder2: Reorders a group of CEs to accommodate */
/*   KB Rete topology. The second pass performs all other  */
/*   transformations not associated with the or CE.        */
/***********************************************************/
static struct lhsParseNode *PerformReorder2(
  void *theEnv,
  struct lhsParseNode *theLHS,
  int *newChange)
  {
   struct lhsParseNode *argPtr;
   int change;

   /*======================================================*/
   /* Loop through the CEs as long as changes can be made. */
   /*======================================================*/

   change = TRUE;
   *newChange = FALSE;

   while (change)
     {
      change = FALSE;

      for (argPtr = theLHS->right;
           argPtr != NULL;)
        {
         /*=======================================================*/
         /* A sequence of three not CEs grouped within each other */
         /* can be replaced with a single not CE. For example,    */
         /* (not (not (not (a)))) can be replaced with (not (a)). */
         /*=======================================================*/
         
         if ((theLHS->type == NOT_CE) &&
             (argPtr->type == NOT_CE) &&
             (argPtr->right != NULL) &&
             (argPtr->right->type == NOT_CE))
           {
            change = TRUE;
            *newChange = TRUE; 
            
            theLHS->right = argPtr->right->right;

            argPtr->right->right = NULL;
            ReturnLHSParseNodes(theEnv,argPtr);

            break;
           }

         /*==========================================*/
         /* Replace two not CEs containing a pattern */
         /* CE with an exists pattern CE.            */
         /*==========================================*/

         else if ((theLHS->type == NOT_CE) && 
                  (argPtr->type == NOT_CE) &&
                  (argPtr->right != NULL) &&
                  (argPtr->right->type == PATTERN_CE))
           {
            change = TRUE;
            *newChange = TRUE;

            CopyLHSParseNode(theEnv,theLHS,argPtr->right,FALSE);

            theLHS->negated = TRUE;
            theLHS->exists = TRUE;
            theLHS->existsNand = FALSE;
            theLHS->right = argPtr->right->right;

            argPtr->right->networkTest = NULL; 
            argPtr->right->externalNetworkTest = NULL;
            argPtr->right->secondaryNetworkTest = NULL;
            argPtr->right->secondaryExternalNetworkTest = NULL;
            argPtr->right->externalRightHash = NULL;
            argPtr->right->externalLeftHash = NULL;
            argPtr->right->leftHash = NULL;
            argPtr->right->rightHash = NULL;
            argPtr->right->betaHash = NULL;
            argPtr->right->expression = NULL;
            argPtr->right->userData = NULL;
            argPtr->right->right = NULL;
            argPtr->right->bottom = NULL;
            ReturnLHSParseNodes(theEnv,argPtr);
            break;
           }
         
         /*======================================*/
         /* Replace not CEs containing a pattern */
         /* CE with a negated pattern CE.        */
         /*======================================*/

         else if ((theLHS->type == NOT_CE) && (argPtr->type == PATTERN_CE))
           {
            change = TRUE;
            *newChange = TRUE;

            CopyLHSParseNode(theEnv,theLHS,argPtr,FALSE);

            theLHS->negated = TRUE;
            theLHS->exists = FALSE;
            theLHS->existsNand = FALSE;
            theLHS->right = argPtr->right;

            argPtr->networkTest = NULL; 
            argPtr->externalNetworkTest = NULL;
            argPtr->secondaryNetworkTest = NULL;
            argPtr->secondaryExternalNetworkTest = NULL;
            argPtr->externalRightHash = NULL;
            argPtr->externalLeftHash = NULL;
            argPtr->constantSelector = NULL;
            argPtr->constantValue = NULL;
            argPtr->leftHash = NULL;
            argPtr->rightHash = NULL;
            argPtr->betaHash = NULL;
            argPtr->expression = NULL;
            argPtr->userData = NULL;
            argPtr->right = NULL;
            argPtr->bottom = NULL;
            ReturnLHSParseNodes(theEnv,argPtr);
            break;
           }

         /*============================================================*/
         /* Replace "and" and "not" CEs contained within a not CE with */
         /* just the and CE, but increment the nand depths of the      */
         /* pattern contained within.                                  */
         /*============================================================*/

         else if ((theLHS->type == NOT_CE) &&
                  ((argPtr->type == AND_CE) ||  (argPtr->type == NOT_CE)))
           {
            change = TRUE;
            *newChange = TRUE;

            theLHS->type = argPtr->type;

            theLHS->negated = argPtr->negated;
            theLHS->exists = argPtr->exists;
            theLHS->existsNand = argPtr->existsNand;
            theLHS->value = argPtr->value;
            theLHS->logical = argPtr->logical;
            theLHS->right = argPtr->right;
            argPtr->right = NULL;
            argPtr->bottom = NULL;
            ReturnLHSParseNodes(theEnv,argPtr);

            IncrementNandDepth(theEnv,theLHS->right,TRUE);
            break;
           }

         /*===================================================*/
         /* If no changes are needed, move on to the next CE. */
         /*===================================================*/

         else
           {
            argPtr = argPtr->bottom;
           }
        }
     }

   /*===========================*/
   /* Return the reordered LHS. */
   /*===========================*/

   return(theLHS);
  }

/**************************************************/
/* ReverseAndOr: Switches and/or CEs into         */
/*   equivalent or/and CEs. For example:          */
/*                                                */
/*     (and (or a b) (or c d))                    */
/*                                                */
/*   would be converted to                        */
/*                                                */
/*     (or (and a (or c d)) (and b (or c d))),    */
/*                                                */
/*   if the "or" CE being expanded was (or a b).  */
/**************************************************/
static struct lhsParseNode *ReverseAndOr(
  void *theEnv,
  struct lhsParseNode *listOfCEs,
  struct lhsParseNode *orCE,
  int orPosition)
  {
   int count;
   struct lhsParseNode *listOfExpandedOrCEs = NULL;
   struct lhsParseNode *lastExpandedOrCE = NULL;
   struct lhsParseNode *copyOfCEs, *replaceCE;

   /*========================================================*/
   /* Loop through each of the CEs contained within the "or" */
   /* CE that is being expanded into the enclosing "and" CE. */
   /*========================================================*/

   while (orCE != NULL)
     {
      /*===============================*/
      /* Make a copy of the and/or CE. */
      /*===============================*/

      copyOfCEs = CopyLHSParseNodes(theEnv,listOfCEs);

      /*====================================================*/
      /* Get a pointer to the "or" CE being expanded in the */
      /* copy just made based on the position of the "or"   */
      /* CE in the original and/or CE (e.g., 1st, 2nd).     */
      /*====================================================*/

      for (count = 1, replaceCE = copyOfCEs->right;
           count != orPosition;
           count++, replaceCE = replaceCE->bottom)
        { /* Do Nothing*/ }

      /*===================================================*/
      /* Delete the contents of the "or" CE being expanded */
      /* in the copy of the and/or CE. From the example    */
      /* above, (and (or a b) (or c d)) would be replaced  */
      /* with (and (or) (or c d)). Note that the "or" CE   */
      /* is still left as a placeholder.                   */
      /*===================================================*/

      ReturnLHSParseNodes(theEnv,replaceCE->right);

      /*======================================================*/
      /* Copy the current CE being examined in the "or" CE to */
      /* the placeholder left in the and/or CE. From the      */
      /* example above, (and (or) (or c d)) would be replaced */
      /* with (and a (or c d)) if the "a" pattern from the    */
      /* "or" CE was being examined or (and b (or c d)) if    */
      /* the "b" pattern from the "or" CE was being examined. */
      /*======================================================*/

      CopyLHSParseNode(theEnv,replaceCE,orCE,TRUE);
      replaceCE->right = CopyLHSParseNodes(theEnv,orCE->right);

      /*====================================*/
      /* Add the newly expanded "and" CE to */
      /* the list of CEs already expanded.  */
      /*====================================*/

      if (lastExpandedOrCE == NULL)
        {
         listOfExpandedOrCEs = copyOfCEs;
         copyOfCEs->bottom = NULL;
         lastExpandedOrCE = copyOfCEs;
        }
      else
        {
         lastExpandedOrCE->bottom = copyOfCEs;
         copyOfCEs->bottom = NULL;
         lastExpandedOrCE = copyOfCEs;
        }

      /*=======================================================*/
      /* Move on to the next CE in the "or" CE being expanded. */
      /*=======================================================*/

      orCE = orCE->bottom;
     }

   /*=====================================================*/
   /* Release the original and/or CE list to free memory. */
   /*=====================================================*/

   ReturnLHSParseNodes(theEnv,listOfCEs);

   /*================================================*/
   /* Wrap an or CE around the list of expanded CEs. */
   /*================================================*/

   copyOfCEs = GetLHSParseNode(theEnv);
   copyOfCEs->type = OR_CE;
   copyOfCEs->right = listOfExpandedOrCEs;

   /*================================*/
   /* Return the newly expanded CEs. */
   /*================================*/

   return(copyOfCEs);
  }

/***********************************************************/
/* CompressCEs:              */
/***********************************************************/
static struct lhsParseNode *CompressCEs(
  void *theEnv,
  struct lhsParseNode *theLHS,
  int *newChange)
  {
   struct lhsParseNode *argPtr, *lastArg, *nextArg;
   struct lhsParseNode *tempArg;
   int change;
   struct expr *e1, *e2;

   /*======================================================*/
   /* Loop through the CEs as long as changes can be made. */
   /*======================================================*/

   change = TRUE;
   *newChange = FALSE;

   while (change)
     {
      change = FALSE;
      lastArg = NULL;

      for (argPtr = theLHS->right;
           argPtr != NULL;)
        {
         /*=====================================*/
         /* Remove duplication of or CEs within */
         /* or CEs and and CEs within and CEs.  */
         /*=====================================*/

         if (((theLHS->type == OR_CE) && (argPtr->type == OR_CE)) ||
             ((theLHS->type == AND_CE) && (argPtr->type == AND_CE)))
           {
            if (argPtr->logical) theLHS->logical = TRUE;

            change = TRUE;
            *newChange = TRUE;
            tempArg = argPtr->right;
            nextArg = argPtr->bottom;
            argPtr->right = NULL;
            argPtr->bottom = NULL;
            ReturnLHSParseNodes(theEnv,argPtr);

            if (lastArg == NULL)
              { theLHS->right = tempArg; }
            else
              { lastArg->bottom = tempArg; }

            argPtr = tempArg;
            while (tempArg->bottom != NULL) tempArg = tempArg->bottom;
            tempArg->bottom = nextArg;
           }

         /*=======================================================*/
         /* Replace not CEs containing a test CE with just a test */
         /* CE with the original test CE condition negated.       */
         /*=======================================================*/

         else if ((theLHS->type == NOT_CE) && (argPtr->type == TEST_CE))
           {
            change = TRUE;
            *newChange = TRUE;

            e1 = GenConstant(theEnv,FCALL,ExpressionData(theEnv)->PTR_NOT);
            e2 = LHSParseNodesToExpression(theEnv,argPtr->expression);
            e1->arg_list = e2;

            CopyLHSParseNode(theEnv,theLHS,argPtr,TRUE);

            ReturnLHSParseNodes(theEnv,argPtr);
            ReturnLHSParseNodes(theEnv,theLHS->expression);

            theLHS->expression = ExpressionToLHSParseNodes(theEnv,e1);
            theLHS->right = NULL;
            ReturnExpression(theEnv,e1);

            break;
           }

         /*==============================*/
         /* Two adjacent test CEs within */
         /* an and CE can be combined.   */
         /*==============================*/

         else if ((theLHS->type == AND_CE) && (argPtr->type == TEST_CE) &&
                  ((argPtr->bottom != NULL) ? argPtr->bottom->type == TEST_CE :
                                              FALSE) &&
                   (argPtr->beginNandDepth == argPtr->endNandDepth) &&
                   (argPtr->endNandDepth == argPtr->bottom->beginNandDepth))
           {
            change = TRUE;
            *newChange = TRUE;

            e1 = LHSParseNodesToExpression(theEnv,argPtr->expression);
            e2 = LHSParseNodesToExpression(theEnv,argPtr->bottom->expression);
            e1 = CombineExpressions(theEnv,e1,e2);

            ReturnLHSParseNodes(theEnv,argPtr->expression);
            argPtr->expression = ExpressionToLHSParseNodes(theEnv,e1);
            ReturnExpression(theEnv,e1);

            tempArg = argPtr->bottom;
            argPtr->bottom = tempArg->bottom;
            tempArg->bottom = NULL;

            ReturnLHSParseNodes(theEnv,tempArg);
           }

         /*=====================================*/
         /* Replace and CEs containing a single */
         /* test CE with just a test CE.        */
         /*=====================================*/

         else if ((theLHS->type == AND_CE) && (argPtr->type == TEST_CE) &&
                  (theLHS->right == argPtr) && (argPtr->bottom == NULL))
           {
            change = TRUE;
            *newChange = TRUE;

            CopyLHSParseNode(theEnv,theLHS,argPtr,TRUE);
            theLHS->right = NULL;
            ReturnLHSParseNodes(theEnv,argPtr);
            break;
           }

         /*===================================================*/
         /* If no changes are needed, move on to the next CE. */
         /*===================================================*/

         else
           {
            lastArg = argPtr;
            argPtr = argPtr->bottom;
           }
        }
     }

   /*===========================*/
   /* Return the reordered LHS. */
   /*===========================*/

   return(theLHS);
  }

/*********************************************************************/
/* CopyLHSParseNodes: Copies a linked group of conditional elements. */
/*********************************************************************/
globle struct lhsParseNode *CopyLHSParseNodes(
  void *theEnv,
  struct lhsParseNode *listOfCEs)
  {
   struct lhsParseNode *newList;

   if (listOfCEs == NULL)
     { return(NULL); }

   newList = get_struct(theEnv,lhsParseNode);
   CopyLHSParseNode(theEnv,newList,listOfCEs,TRUE);

   newList->right = CopyLHSParseNodes(theEnv,listOfCEs->right);
   newList->bottom = CopyLHSParseNodes(theEnv,listOfCEs->bottom);

   return(newList);
  }

/**********************************************************/
/* CopyLHSParseNode: Copies a single conditional element. */
/**********************************************************/
globle void CopyLHSParseNode(
  void *theEnv,
  struct lhsParseNode *dest,
  struct lhsParseNode *src,
  int duplicate)
  {
   dest->type = src->type;
   dest->value = src->value;
   dest->negated = src->negated;
   dest->exists = src->exists;
   dest->existsNand = src->existsNand;
   dest->bindingVariable = src->bindingVariable;
   dest->withinMultifieldSlot = src->withinMultifieldSlot;
   dest->multifieldSlot = src->multifieldSlot;
   dest->multiFieldsBefore = src->multiFieldsBefore;
   dest->multiFieldsAfter = src->multiFieldsAfter;
   dest->singleFieldsBefore = src->singleFieldsBefore;
   dest->singleFieldsAfter = src->singleFieldsAfter;
   dest->logical = src->logical;
   dest->userCE = src->userCE;
   dest->referringNode = src->referringNode;
   dest->patternType = src->patternType;
   dest->pattern = src->pattern;
   dest->index = src->index;
   dest->slot = src->slot;
   dest->slotNumber = src->slotNumber;
   dest->beginNandDepth = src->beginNandDepth;
   dest->endNandDepth = src->endNandDepth;
   dest->joinDepth = src->joinDepth;

   /*==========================================================*/
   /* The duplicate flag controls whether pointers to existing */
   /* data structures are used when copying some slots or if   */
   /* new copies of the data structures are made.              */
   /*==========================================================*/

   if (duplicate)
     {
      dest->networkTest = CopyExpression(theEnv,src->networkTest);
      dest->externalNetworkTest = CopyExpression(theEnv,src->externalNetworkTest);
      dest->secondaryNetworkTest = CopyExpression(theEnv,src->secondaryNetworkTest);
      dest->secondaryExternalNetworkTest = CopyExpression(theEnv,src->secondaryExternalNetworkTest);
      dest->externalRightHash = CopyExpression(theEnv,src->externalRightHash);
      dest->externalLeftHash = CopyExpression(theEnv,src->externalLeftHash);
      dest->constantSelector = CopyExpression(theEnv,src->constantSelector);
      dest->constantValue = CopyExpression(theEnv,src->constantValue);
      dest->leftHash = CopyExpression(theEnv,src->leftHash);
      dest->betaHash = CopyExpression(theEnv,src->betaHash);
      dest->rightHash = CopyExpression(theEnv,src->rightHash);
      if (src->userData == NULL)
        { dest->userData = NULL; }
      else if (src->patternType->copyUserDataFunction == NULL)
        { dest->userData = src->userData; }
      else
        { dest->userData = (*src->patternType->copyUserDataFunction)(theEnv,src->userData); }
      dest->expression = CopyLHSParseNodes(theEnv,src->expression);
      dest->constraints = CopyConstraintRecord(theEnv,src->constraints);
      if (dest->constraints != NULL) dest->derivedConstraints = TRUE;
      else dest->derivedConstraints = FALSE;
     }
   else
     {
      dest->networkTest = src->networkTest;
      dest->externalNetworkTest = src->externalNetworkTest;
      dest->secondaryNetworkTest = src->secondaryNetworkTest;
      dest->secondaryExternalNetworkTest = src->secondaryExternalNetworkTest;
      dest->externalRightHash = src->externalRightHash;
      dest->externalLeftHash = src->externalLeftHash;
      dest->constantSelector = src->constantSelector;
      dest->constantValue = src->constantValue;
      dest->leftHash = src->leftHash;
      dest->betaHash = src->betaHash;
      dest->rightHash = src->rightHash;
      dest->userData = src->userData;
      dest->expression = src->expression;
      dest->derivedConstraints = FALSE;
      dest->constraints = src->constraints;
     }
  }

/****************************************************/
/* GetLHSParseNode: Creates an empty node structure */
/*   used for building conditional elements.        */
/****************************************************/
globle struct lhsParseNode *GetLHSParseNode(
  void *theEnv)
  {
   struct lhsParseNode *newNode;

   newNode = get_struct(theEnv,lhsParseNode);
   newNode->type = UNKNOWN_VALUE;
   newNode->value = NULL;
   newNode->negated = FALSE;
   newNode->exists = FALSE;
   newNode->existsNand = FALSE;
   newNode->bindingVariable = FALSE;
   newNode->withinMultifieldSlot = FALSE;
   newNode->multifieldSlot = FALSE;
   newNode->multiFieldsBefore = 0;
   newNode->multiFieldsAfter = 0;
   newNode->singleFieldsBefore = 0;
   newNode->singleFieldsAfter = 0;
   newNode->logical = FALSE;
   newNode->derivedConstraints = FALSE;
   newNode->userCE = TRUE;
   newNode->constraints = NULL;
   newNode->referringNode = NULL;
   newNode->patternType = NULL;
   newNode->pattern = -1;
   newNode->index = -1;
   newNode->slot = NULL;
   newNode->slotNumber = -1;
   newNode->beginNandDepth = 1;
   newNode->endNandDepth = 1;
   newNode->joinDepth = 0;
   newNode->userData = NULL;
   newNode->networkTest = NULL;
   newNode->externalNetworkTest = NULL;
   newNode->secondaryNetworkTest = NULL;
   newNode->secondaryExternalNetworkTest = NULL;
   newNode->externalRightHash = NULL;
   newNode->externalLeftHash = NULL;
   newNode->constantSelector = NULL;
   newNode->constantValue = NULL;
   newNode->leftHash = NULL;
   newNode->betaHash = NULL;
   newNode->rightHash = NULL;
   newNode->expression = NULL;
   newNode->right = NULL;
   newNode->bottom = NULL;

   return(newNode);
  }

/********************************************************/
/* ReturnLHSParseNodes:  Returns a multiply linked list */
/*   of lhsParseNode structures to the memory manager.  */
/********************************************************/
globle void ReturnLHSParseNodes(
  void *theEnv,
  struct lhsParseNode *waste)
  {
   if (waste != NULL)
     {
      ReturnExpression(theEnv,waste->networkTest);
      ReturnExpression(theEnv,waste->externalNetworkTest);
      ReturnExpression(theEnv,waste->secondaryNetworkTest);
      ReturnExpression(theEnv,waste->secondaryExternalNetworkTest);
      ReturnExpression(theEnv,waste->externalRightHash);
      ReturnExpression(theEnv,waste->externalLeftHash);
      ReturnExpression(theEnv,waste->constantSelector);
      ReturnExpression(theEnv,waste->constantValue);
      ReturnExpression(theEnv,waste->leftHash);
      ReturnExpression(theEnv,waste->betaHash);
      ReturnExpression(theEnv,waste->rightHash);
      ReturnLHSParseNodes(theEnv,waste->right);
      ReturnLHSParseNodes(theEnv,waste->bottom);
      ReturnLHSParseNodes(theEnv,waste->expression);
      if (waste->derivedConstraints) RemoveConstraint(theEnv,waste->constraints);
      if ((waste->userData != NULL) &&
          (waste->patternType->returnUserDataFunction != NULL))
        { (*waste->patternType->returnUserDataFunction)(theEnv,waste->userData); }
      rtn_struct(theEnv,lhsParseNode,waste);
     }
  }

/********************************************************/
/* ExpressionToLHSParseNodes: Copies an expression into */
/*   the equivalent lhsParseNode data structures.       */
/********************************************************/
globle struct lhsParseNode *ExpressionToLHSParseNodes(
  void *theEnv,
  struct expr *expressionList)
  {
   struct lhsParseNode *newList, *theList;
   struct FunctionDefinition *theFunction;
   int i, theRestriction;

   /*===========================================*/
   /* A NULL expression requires no conversion. */
   /*===========================================*/

   if (expressionList == NULL) return(NULL);

   /*====================================*/
   /* Recursively convert the expression */
   /* to lhsParseNode data structures.   */
   /*====================================*/

   newList = GetLHSParseNode(theEnv);
   newList->type = expressionList->type;
   newList->value = expressionList->value;
   newList->right = ExpressionToLHSParseNodes(theEnv,expressionList->nextArg);
   newList->bottom = ExpressionToLHSParseNodes(theEnv,expressionList->argList);

   /*==================================================*/
   /* If the expression is a function call, then store */
   /* the constraint information for the functions     */
   /* arguments in the lshParseNode data structures.   */
   /*==================================================*/

   if (newList->type != FCALL) return(newList);

   theFunction = (struct FunctionDefinition *) newList->value;
   for (theList = newList->bottom, i = 1;
        theList != NULL;
        theList = theList->right, i++)
     {
      if (theList->type == SF_VARIABLE)
        {
         theRestriction = GetNthRestriction(theFunction,i);
         theList->constraints = ArgumentTypeToConstraintRecord(theEnv,theRestriction);
         theList->derivedConstraints = TRUE;
        }
     }

   /*==================================*/
   /* Return the converted expression. */
   /*==================================*/

   return(newList);
  }

/******************************************************************/
/* LHSParseNodesToExpression: Copies lhsParseNode data structures */
/*   into the equivalent expression data structures.              */
/******************************************************************/
globle struct expr *LHSParseNodesToExpression(
  void *theEnv,
  struct lhsParseNode *nodeList)
  {
   struct expr *newList;

   if (nodeList == NULL)
     { return(NULL); }

   newList = get_struct(theEnv,expr);
   newList->type = nodeList->type;
   newList->value = nodeList->value;
   newList->nextArg = LHSParseNodesToExpression(theEnv,nodeList->right);
   newList->argList = LHSParseNodesToExpression(theEnv,nodeList->bottom);

   return(newList);
  }

/************************************************************/
/* IncrementNandDepth: Increments the nand depth of a group */
/*   of CEs. The nand depth is used to indicate the nesting */
/*   of not/and or not/not CEs which are implemented using  */
/*   joins from the right. A single pattern within a "not"  */
/*   CE does not require a join from the right and its nand */
/*   depth is normally not increased (except when it's      */
/*   within a not/and or not/not CE. The begin nand depth   */
/*   indicates the current nesting for a CE. The end nand   */
/*   depth indicates the nand depth in the following CE     */
/*   (assuming that the next CE is not the beginning of a   */
/*   new group of nand CEs). All but the last CE in a nand  */
/*   group should have the same begin and end nand depths.  */
/*   Since a single CE can be the last CE of several nand   */
/*   groups, it is possible to have an end nand depth that  */
/*   is more than 1 less than the begin nand depth of the   */
/*   CE.                                                    */
/************************************************************/
static void IncrementNandDepth(
  void *theEnv,
  struct lhsParseNode *theLHS,
  int lastCE)
  {
   /*======================================*/
   /* Loop through each CE in the group of */
   /* CEs having its nand depth increased. */
   /*======================================*/

   for (;
        theLHS != NULL;
        theLHS = theLHS->bottom)
     {
      /*=========================================================*/
      /* Increment the begin nand depth of pattern and test CEs. */
      /* The last CE in the original list doesn't have its end   */
      /* nand depth incremented. All other last CEs in other CEs */
      /* entered recursively do have their end depth incremented */
      /* (unless the last CE in the recursively entered CE is    */
      /* the same last CE as contained in the original group     */
      /* when this function was first entered).                  */
      /*=========================================================*/

      if ((theLHS->type == PATTERN_CE) || (theLHS->type == TEST_CE))
        {
         theLHS->beginNandDepth++;

         if (lastCE == FALSE) theLHS->endNandDepth++;
         else if (theLHS->bottom != NULL) theLHS->endNandDepth++;
        }

      /*==============================================*/
      /* Recursively increase the nand depth of other */
      /* CEs contained within the CE having its nand  */
      /* depth increased.                             */
      /*==============================================*/

      else if ((theLHS->type == AND_CE) || (theLHS->type == NOT_CE))
        {
         IncrementNandDepth(theEnv,theLHS->right,
                            (lastCE ? (theLHS->bottom == NULL) : FALSE));
        }

      /*=====================================*/
      /* All or CEs should have been removed */
      /* from the LHS at this point.         */
      /*=====================================*/

      else if (theLHS->type == OR_CE)
        { SystemError(theEnv,"REORDER",1); }
     }
  }

/***********************************************************/
/* CreateInitialPattern: Creates a default pattern used in */
/*  the LHS of a rule under certain cirmustances (such as  */
/*  when a "not" or "test" CE is the first CE in an "and"  */
/*  CE or when no CEs are specified in the LHS of a rule.  */
/***********************************************************/
static struct lhsParseNode *CreateInitialPattern(
  void *theEnv)
  {
   struct lhsParseNode *topNode;

   /*==========================================*/
   /* Create the top most node of the pattern. */
   /*==========================================*/

   topNode = GetLHSParseNode(theEnv);
   topNode->type = PATTERN_CE;
   topNode->userCE = FALSE;
   topNode->bottom = NULL;
   
   return(topNode);
  }

/*****************************************************************/
/* AddRemainingInitialPatterns: Finishes adding initial patterns */
/*   where needed on the LHS of a rule. Assumes that an initial  */
/*   pattern has been added to the beginning of the rule if one  */
/*   was needed.                                                 */
/*****************************************************************/
static struct lhsParseNode *AddRemainingInitialPatterns(
  void *theEnv,
  struct lhsParseNode *theLHS)
  {
   struct lhsParseNode *lastNode = NULL, *thePattern, *rv = theLHS;
   int currentDepth = 1;
   
   while (theLHS != NULL)
     {
      if ((theLHS->type == TEST_CE) &&
          (theLHS->beginNandDepth  > currentDepth))
        {
         thePattern = CreateInitialPattern(theEnv);
         thePattern->beginNandDepth = theLHS->beginNandDepth;
         thePattern->endNandDepth = theLHS->beginNandDepth;
         thePattern->logical = theLHS->logical;
         thePattern->existsNand = theLHS->existsNand;
         theLHS->existsNand = FALSE;
     
         thePattern->bottom = theLHS;
               
         if (lastNode == NULL)
           { rv = thePattern; }
         else
           { lastNode->bottom = thePattern; }
        }
        
      lastNode = theLHS;
      currentDepth = theLHS->endNandDepth;
      theLHS = theLHS->bottom;
     }
     
   return(rv);
  }   

/**********************************************/
/* PrintNodes: Debugging routine which prints */
/*   the representation of a CE.              */
/**********************************************/
static void PrintNodes(
  void *theEnv,
  char *fileid,
  struct lhsParseNode *theNode)
  {
   if (theNode == NULL) return;

   while (theNode != NULL)
     {
      switch (theNode->type)
        {
         case PATTERN_CE:
           EnvPrintRouter(theEnv,fileid,"(");
           if (theNode->negated) EnvPrintRouter(theEnv,fileid,"n");
           if (theNode->exists) EnvPrintRouter(theEnv,fileid,"x");
           if (theNode->logical) EnvPrintRouter(theEnv,fileid,"l");
           PrintLongInteger(theEnv,fileid,(long long) theNode->beginNandDepth);
           EnvPrintRouter(theEnv,fileid,"-");
           PrintLongInteger(theEnv,fileid,(long long) theNode->endNandDepth);
           EnvPrintRouter(theEnv,fileid," ");
           EnvPrintRouter(theEnv,fileid,ValueToString(theNode->right->bottom->value));
           EnvPrintRouter(theEnv,fileid,")");
           break;

         case TEST_CE:
           EnvPrintRouter(theEnv,fileid,"(test ");
           PrintLongInteger(theEnv,fileid,(long long) theNode->beginNandDepth);
           EnvPrintRouter(theEnv,fileid,"-");
           PrintLongInteger(theEnv,fileid,(long long) theNode->endNandDepth);
           EnvPrintRouter(theEnv,fileid,")");
           break;

         case NOT_CE:
           if (theNode->logical) EnvPrintRouter(theEnv,fileid,"(lnot ");
           else EnvPrintRouter(theEnv,fileid,"(not ");;
           PrintNodes(theEnv,fileid,theNode->right);
           EnvPrintRouter(theEnv,fileid,")");
           break;

         case OR_CE:
           if (theNode->logical) EnvPrintRouter(theEnv,fileid,"(lor ");
           else EnvPrintRouter(theEnv,fileid,"(or ");
           PrintNodes(theEnv,fileid,theNode->right);
           EnvPrintRouter(theEnv,fileid,")");
           break;

         case AND_CE:
           if (theNode->logical) EnvPrintRouter(theEnv,fileid,"(land ");
           else EnvPrintRouter(theEnv,fileid,"(and ");
           PrintNodes(theEnv,fileid,theNode->right);
           EnvPrintRouter(theEnv,fileid,")");
           break;

         default:
           EnvPrintRouter(theEnv,fileid,"(unknown)");
           break;

        }

      theNode = theNode->bottom;
      if (theNode != NULL) EnvPrintRouter(theEnv,fileid," ");
     }

   return;
  }

/*************************************************************/
/* AssignPatternIndices: For each pattern CE in the LHS of a */
/*   rule, determines the pattern index for the CE. A simple */
/*   1 to N numbering can't be used since a join from the    */
/*   right only counts as a single CE to other CEs outside   */
/*   the lexical scope of the join from the right. For       */
/*   example, the patterns in the following LHS              */
/*                                                           */
/*     (a) (not (b) (c) (d)) (e)                             */
/*                                                           */
/*   would be assigned the following numbers: a-1, b-2, c-3, */
/*   d-4, and e-3.                                           */
/*************************************************************/
static struct lhsParseNode *AssignPatternIndices(
  struct lhsParseNode *theLHS,
  short startIndex,
  int depth)
  {
   struct lhsParseNode *theField;
   short joinDepth = 0;

   /*====================================*/
   /* Loop through the CEs at this level */
   /* assigning each CE a pattern index. */
   /*====================================*/

   while (theLHS != NULL)
     {
      /*============================================================*/
      /* If we're entering a group of CEs requiring a join from the */
      /* right, compute the pattern indices for that group and then */
      /* proceed with the next CE in this group. A join from the    */
      /* right only counts as one CE on this level regardless of    */
      /* the number of CEs it contains. If the end of this level is */
      /* encountered while processing the join from right, then     */
      /* return to the previous level.                              */
      /*============================================================*/

      if (theLHS->beginNandDepth > depth)
        {
         theLHS = AssignPatternIndices(theLHS,startIndex,theLHS->beginNandDepth);
         if (theLHS->endNandDepth < depth) return(theLHS);
         startIndex++;
         joinDepth++;
        }

      /*=====================================================*/
      /* A test CE is not assigned a pattern index, however, */
      /* if it is the last CE at the end of this level, then */
      /* return to the previous level.                       */
      /*=====================================================*/

      else if (theLHS->type == TEST_CE)
        { 
         theLHS->joinDepth = joinDepth - 1;
         PropagateJoinDepth(theLHS->expression,(short) (joinDepth - 1));
         PropagateNandDepth2(theLHS->expression,theLHS->beginNandDepth,theLHS->endNandDepth);
         if (theLHS->endNandDepth < depth) return(theLHS); 
        }

      /*==========================================================*/
      /* The fields of a pattern CE need to be assigned a pattern */
      /* index, field index, and/or slot names. If this CE is the */
      /* last CE at the end of this level, then return to the     */
      /* previous level.                                          */
      /*==========================================================*/

      else if (theLHS->type == PATTERN_CE)
        {
         theLHS->pattern = startIndex;
         theLHS->joinDepth = joinDepth;
         PropagateJoinDepth(theLHS->right,joinDepth);
         for (theField = theLHS->right; theField != NULL; theField = theField->right)
           {
            theField->pattern = startIndex;
            PropagateIndexSlotPatternValues(theField,theField->pattern,
                                            theField->index,theField->slot,
                                            theField->slotNumber);
            PropagateNandDepth(theField,theLHS->beginNandDepth,theLHS->endNandDepth);
           }

         if (theLHS->endNandDepth < depth) return(theLHS);
         startIndex++;
         joinDepth++;
        }

      /*=========================*/
      /* Move on to the next CE. */
      /*=========================*/

      theLHS = theLHS->bottom;
     }

   /*========================================*/
   /* There are no more CEs left to process. */
   /* Return to the previous level.          */
   /*========================================*/

   return(NULL);
  }

/***********************************************************/
/* PropagateIndexSlotPatternValues: Assigns pattern, field */
/*   and slot identifiers to a field in a pattern.         */
/***********************************************************/
static void PropagateIndexSlotPatternValues(
  struct lhsParseNode *theField,
  short thePattern,
  short theIndex,
  struct symbolHashNode *theSlot,
  short theSlotNumber)
  {
   struct lhsParseNode *tmpNode, *andField;

   /*=============================================*/
   /* A NULL field does not have to be processed. */
   /*=============================================*/

   if (theField == NULL) return;

   /*=====================================================*/
   /* Assign the appropriate identifiers for a multifield */
   /* slot by calling this routine recursively.           */
   /*=====================================================*/

   if (theField->multifieldSlot)
     {
      theField->pattern = thePattern;
      if (theIndex > 0) theField->index = theIndex;
      theField->slot = theSlot;
      theField->slotNumber = theSlotNumber;

      for (tmpNode = theField->bottom;
           tmpNode != NULL;
           tmpNode = tmpNode->right)
        {
         tmpNode->pattern = thePattern;
         tmpNode->slot = theSlot;
         PropagateIndexSlotPatternValues(tmpNode,thePattern,tmpNode->index,
                                         theSlot,theSlotNumber);
        }

      return;
     }

   /*=======================================================*/
   /* Loop through each of the or'ed constraints (connected */
   /* by a |) in this field of the pattern.                 */
   /*=======================================================*/

   for (theField = theField->bottom;
        theField != NULL;
        theField = theField->bottom)
     {
      /*===========================================================*/
      /* Loop through each of the and'ed constraints (connected by */
      /* a &) in this field of the pattern. Assign the pattern,    */
      /* field, and slot identifiers.                              */
      /*===========================================================*/

      for (andField = theField; andField != NULL; andField = andField->right)
        {
         andField->pattern = thePattern;
         if (theIndex > 0) andField->index = theIndex;
         andField->slot = theSlot;
         andField->slotNumber = theSlotNumber;
        }
     }
   }

/***********************************************************/
/* PropagateJoinDepth:          */
/***********************************************************/
static void PropagateJoinDepth(
  struct lhsParseNode *theField,
  short joinDepth)
  {
   while (theField != NULL)
     {
      theField->joinDepth = joinDepth;
      if (theField->bottom != NULL)
        { PropagateJoinDepth(theField->bottom,joinDepth); }
      if (theField->expression != NULL)
        { PropagateJoinDepth(theField->expression,joinDepth); }
      theField = theField->right;
     }
  }

/***********************************************************/
/* PropagateNandDepth:          */
/***********************************************************/
static void PropagateNandDepth(
  struct lhsParseNode *theField,
  int beginDepth,
  int endDepth)
  { 
   if (theField == NULL) return;
   theField->beginNandDepth = beginDepth;
   theField->endNandDepth = endDepth;
   
   PropagateNandDepth(theField->expression,beginDepth,endDepth);
      
   for (theField = theField->bottom; theField != NULL; theField = theField->right)
      { PropagateNandDepth(theField,beginDepth,endDepth); }
  }

/***********************************************************/
/* PropagateNandDepth2:        */
/***********************************************************/
static void PropagateNandDepth2( 
  struct lhsParseNode *theField,
  int beginDepth,
  int endDepth)
  { 
   if (theField == NULL) return;
   for (; theField != NULL; theField = theField->right)
      { 
       theField->beginNandDepth = beginDepth;
       theField->endNandDepth = endDepth;
       PropagateNandDepth2(theField->bottom,beginDepth,endDepth); 
      }
  }
  
/*************************************************************************/
/* IsExistsSubjoin:  */
/*************************************************************************/
globle int IsExistsSubjoin(
  struct lhsParseNode *theLHS,
  int parentDepth)
  {
   int startDepth = theLHS->beginNandDepth;
   
   if ((startDepth - parentDepth) != 2)
     { return(FALSE); }
     
   while (theLHS->endNandDepth >= startDepth)
     { theLHS = theLHS->bottom; }
   
   /*
   if (theLHS->endNandDepth == parentDepth)
     { return(TRUE); }
   */  

   if (theLHS->endNandDepth <= parentDepth)
     { return(TRUE); }

   return(FALSE);
  }
  
#endif

