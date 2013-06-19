   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                 FACT MATCH MODULE                   */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the algorithm for pattern matching in */
/*   the fact pattern network.                               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Removed INCREMENTAL_RESET compilation flag.    */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*            Fix for DR0880. 2008-01-24                     */
/*                                                           */
/*************************************************************/

#define _FACTMCH_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT && DEFRULE_CONSTRUCT

#include "drive.h"
#include "engine.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "factgen.h"
#include "factrete.h"
#include "incrrset.h"
#include "memalloc.h"
#include "reteutil.h"
#include "router.h"
#include "sysdep.h"
#include "tmpltdef.h"

#include "factmch.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static intBool                  EvaluatePatternExpression(void *,struct factPatternNode *,struct expr *);
   static void                     TraceErrorToJoin(void *,struct factPatternNode *,int);
   static void                     ProcessFactAlphaMatch(void *,struct fact *,struct multifieldMarker *,struct factPatternNode *);
   static struct factPatternNode  *GetNextFactPatternNode(void *,int,struct factPatternNode *);
   static int                      SkipFactPatternNode(void *,struct factPatternNode *);
   static void                     ProcessMultifieldNode(void *,
                                                         struct factPatternNode *,
                                                         struct multifieldMarker *,
                                                         struct multifieldMarker *,int);
   static void                     PatternNetErrorMessage(void *,struct factPatternNode *);

/*************************************************************************/
/* FactPatternMatch: Implements the core loop for fact pattern matching. */
/*************************************************************************/
globle void FactPatternMatch(
  void *theEnv,
  struct fact *theFact,
  struct factPatternNode *patternPtr,
  int offset,
  struct multifieldMarker *markers,
  struct multifieldMarker *endMark)
  {
   int theSlotField;
   int offsetSlot;
   DATA_OBJECT theResult;
   struct factPatternNode *tempPtr;
   
   /*=========================================================*/
   /* If there's nothing left in the pattern network to match */
   /* against, then the current traversal of the pattern      */
   /* network needs to back up.                               */
   /*=========================================================*/

   if (patternPtr == NULL) return;

   /*=======================================================*/
   /* The offsetSlot variable indicates the current offset  */
   /* within the multifield slot being pattern matched.     */
   /* (Recall that a multifield wildcard or variable        */
   /* recursively iterates through all possible  bindings.) */
   /* Once a new slot starts being pattern matched, the     */
   /* offset is reset to zero.                              */
   /*=======================================================*/

   offsetSlot = patternPtr->whichSlot;

   /*================================================*/
   /* Set up some global parameters for use by the   */
   /* Rete access functions and general convenience. */
   /*================================================*/

   FactData(theEnv)->CurrentPatternFact = theFact;
   FactData(theEnv)->CurrentPatternMarks = markers;

   /*============================================*/
   /* Loop through each node in pattern network. */
   /*============================================*/

   while (patternPtr != NULL)
     {
      /*=============================================================*/
      /* Determine the position of the field we're going to pattern  */
      /* match. If this routine has been entered recursively because */
      /* of multifield wildcards or variables, then add in the       */
      /* additional offset caused by the values which match these    */
      /* multifields. This offset may be negative (if for example a  */
      /* a multifield matched a zero length value).                  */
      /*=============================================================*/

      theSlotField = patternPtr->whichField;
      if (offsetSlot == patternPtr->whichSlot)
        { theSlotField += offset; }

      /*===================================*/
      /* Determine if we want to skip this */
      /* node during an incremental reset. */
      /*===================================*/

      if (SkipFactPatternNode(theEnv,patternPtr))
        { patternPtr = GetNextFactPatternNode(theEnv,TRUE,patternPtr); }

      /*=========================================================*/
      /* If this is a single field pattern node, then determine  */
      /* if the constraints for the node have been satisfied for */
      /* the current field in the slot being examined.           */
      /*=========================================================*/

      else if (patternPtr->header.singlefieldNode)
        {
         /*==================================================*/
         /* If we're at the last slot in the pattern, make   */
         /* sure the number of fields in the fact correspond */
         /* to the number of fields required by the pattern  */
         /* based on the binding of multifield variables.    */
         /*==================================================*/

         int skipit = FALSE;
         if (patternPtr->header.endSlot &&
             ((FactData(theEnv)->CurrentPatternMarks == NULL) ?
              FALSE :
              (FactData(theEnv)->CurrentPatternMarks->where.whichSlotNumber == patternPtr->whichSlot)) &&
             (FactData(theEnv)->CurrentPatternFact->theProposition.theFields
                  [patternPtr->whichSlot].type == MULTIFIELD))
           {
            if ((patternPtr->leaveFields + theSlotField) != (int)
               ((struct multifield *) FactData(theEnv)->CurrentPatternFact->theProposition.theFields
                                      [patternPtr->whichSlot].value)->multifieldLength)
              { skipit = TRUE; }
           }

         if (skipit)
           { patternPtr = GetNextFactPatternNode(theEnv,TRUE,patternPtr); }
         else

         if (patternPtr->header.selector)
           {
            if (EvaluatePatternExpression(theEnv,patternPtr,patternPtr->networkTest->nextArg))
              {
               EvaluateExpression(theEnv,patternPtr->networkTest,&theResult);
            
               tempPtr = (struct factPatternNode *) FindHashedPatternNode(theEnv,patternPtr,theResult.type,theResult.value);
              }
            else
              { tempPtr = NULL; }
              
            if (tempPtr != NULL)
              {
               if (tempPtr->header.stopNode)
                 { ProcessFactAlphaMatch(theEnv,theFact,markers,tempPtr); }
               
               patternPtr = GetNextFactPatternNode(theEnv,FALSE,tempPtr);
              }
            else
              { patternPtr = GetNextFactPatternNode(theEnv,TRUE,patternPtr); }
           }
         
         /*=============================================*/
         /* If the constraints are satisified, then ... */
         /*=============================================*/

         else if (EvaluatePatternExpression(theEnv,patternPtr,patternPtr->networkTest))
           {
            /*=======================================================*/
            /* If a leaf pattern node has been successfully reached, */
            /* then the pattern has been satisified. Generate an     */
            /* alpha match to store in the pattern node.             */
            /*=======================================================*/

            if (patternPtr->header.stopNode)
              { ProcessFactAlphaMatch(theEnv,theFact,markers,patternPtr); }

            /*===================================*/
            /* Move on to the next pattern node. */
            /*===================================*/

            patternPtr = GetNextFactPatternNode(theEnv,FALSE,patternPtr);
           }

         /*==============================================*/
         /* Otherwise, move on to the next pattern node. */
         /*==============================================*/

         else
           { patternPtr = GetNextFactPatternNode(theEnv,TRUE,patternPtr); }
        }

      /*======================================================*/
      /* If this is a multifield pattern node, then determine */
      /* if the constraints for the node have been satisfied  */
      /* for the current field in the slot being examined.    */
      /*======================================================*/

      else if (patternPtr->header.multifieldNode)
        {
         /*========================================================*/
         /* Determine if the multifield pattern node's constraints */
         /* are satisfied. If we've traversed to a different slot  */
         /* than the one we started this routine with, then the    */
         /* offset into the slot is reset to zero.                 */
         /*========================================================*/

         if (offsetSlot == patternPtr->whichSlot)
           { ProcessMultifieldNode(theEnv,patternPtr,markers,endMark,offset); }
         else
           { ProcessMultifieldNode(theEnv,patternPtr,markers,endMark,0); }

         /*===================================================*/
         /* Move on to the next pattern node. Since the lower */
         /* branches of the pattern network have already been */
         /* recursively processed by ProcessMultifieldNode,   */
         /* we get the next pattern node by treating this     */
         /* multifield pattern node as if it were a single    */
         /* field pattern node that failed its constraint.    */
         /*===================================================*/

         patternPtr = GetNextFactPatternNode(theEnv,TRUE,patternPtr);
        }
     }
  }

/**************************************************************/
/* ProcessMultifieldNode: Handles recursive pattern matching  */
/*  when a multifield wildcard or variable is encountered as  */
/*  a slot constraint. The pattern matching routine is called */
/*  iteratively for each possible binding of the multifield   */
/*  wildcard or variable.                                     */
/**************************************************************/
static void ProcessMultifieldNode(
  void *theEnv,
  struct factPatternNode *thePattern,
  struct multifieldMarker *markers,
  struct multifieldMarker *endMark,
  int offset)
  {
   struct multifieldMarker *newMark, *oldMark;
   int repeatCount;
   struct multifield *theSlotValue;
   DATA_OBJECT theResult;
   struct factPatternNode *tempPtr;
   intBool success;

   /*========================================*/
   /* Get a pointer to the slot value of the */
   /* multifield slot being pattern matched. */
   /*========================================*/

   theSlotValue = (struct multifield *)
     FactData(theEnv)->CurrentPatternFact->theProposition.theFields[thePattern->whichSlot].value;

   /*===============================================*/
   /* Save the value of the markers already stored. */
   /*===============================================*/

   oldMark = markers;

   /*===========================================*/
   /* Create a new multifield marker and append */
   /* it to the end of the current list.        */
   /*===========================================*/

   newMark = get_struct(theEnv,multifieldMarker);
   newMark->whichField = thePattern->whichField - 1;
   newMark->where.whichSlotNumber = (short) thePattern->whichSlot;
   newMark->startPosition = (thePattern->whichField - 1) + offset;
   newMark->next = NULL;

   if (endMark == NULL)
     {
      markers = newMark;
      FactData(theEnv)->CurrentPatternMarks = markers;
     }
   else
     { endMark->next = newMark; }

   /*============================================*/
   /* Handle a multifield constraint as the last */
   /* constraint of a slot as a special case.    */
   /*============================================*/

   if (thePattern->header.endSlot)
     {
      newMark->endPosition = (long) theSlotValue->multifieldLength -
                                    (thePattern->leaveFields + 1);

      /*=======================================================*/
      /* Make sure the endPosition is never more than less one */
      /* less of the startPosition (a multifield containing no */
      /* values.                                               */
      /*=======================================================*/

      if (newMark->endPosition < newMark->startPosition)
        { newMark->endPosition = newMark->startPosition - 1; }

      /*===========================================*/
      /* Determine if the constraint is satisfied. */
      /*===========================================*/

      if (thePattern->header.selector)
        {
         if (EvaluatePatternExpression(theEnv,thePattern,thePattern->networkTest->nextArg))
           {
            EvaluateExpression(theEnv,thePattern->networkTest,&theResult);
         
            thePattern = (struct factPatternNode *) FindHashedPatternNode(theEnv,thePattern,theResult.type,theResult.value);
            if (thePattern != NULL)
              { success = TRUE; }
            else
              { success = FALSE; }
           }
         else
           { success = FALSE; }
        }
      else if ((thePattern->networkTest == NULL) ?
          TRUE :
          (EvaluatePatternExpression(theEnv,thePattern,thePattern->networkTest)))
        { success = TRUE; }
      else
        { success = FALSE; }
    
      if (success)
        {
         /*=======================================================*/
         /* If a leaf pattern node has been successfully reached, */
         /* then the pattern has been satisified. Generate an     */
         /* alpha match to store in the pattern node.             */
         /*=======================================================*/

         if (thePattern->header.stopNode)
           { ProcessFactAlphaMatch(theEnv,FactData(theEnv)->CurrentPatternFact,FactData(theEnv)->CurrentPatternMarks,thePattern); }

         /*=============================================*/
         /* Recursively continue pattern matching based */
         /* on the multifield binding just generated.   */
         /*=============================================*/

         FactPatternMatch(theEnv,FactData(theEnv)->CurrentPatternFact,
                          thePattern->nextLevel,0,FactData(theEnv)->CurrentPatternMarks,newMark);
        }

      /*================================================*/
      /* Discard the multifield marker since we've done */
      /* all the pattern matching for this binding of   */
      /* the multifield slot constraint.                */
      /*================================================*/

      rtn_struct(theEnv,multifieldMarker,newMark);
      if (endMark != NULL) endMark->next = NULL;
      FactData(theEnv)->CurrentPatternMarks = oldMark;
      return;
     }

   /*==============================================*/
   /* Perform matching for nodes beneath this one. */
   /*==============================================*/

   for (repeatCount = (long) (theSlotValue->multifieldLength -
                      (newMark->startPosition + thePattern->leaveFields));
        repeatCount >= 0;
        repeatCount--)
     {
      newMark->endPosition = newMark->startPosition + (repeatCount - 1);

      if (thePattern->header.selector)
        {
         if (EvaluatePatternExpression(theEnv,thePattern,thePattern->networkTest->nextArg))
           {
            EvaluateExpression(theEnv,thePattern->networkTest,&theResult);
         
            tempPtr = (struct factPatternNode *) FindHashedPatternNode(theEnv,thePattern,theResult.type,theResult.value);
            if (tempPtr != NULL)
              {
               FactPatternMatch(theEnv,FactData(theEnv)->CurrentPatternFact,
                                tempPtr->nextLevel,offset + repeatCount - 1,
                                FactData(theEnv)->CurrentPatternMarks,newMark);
              }
           }
        }
      else if ((thePattern->networkTest == NULL) ?
               TRUE :
               (EvaluatePatternExpression(theEnv,thePattern,thePattern->networkTest)))
        {
         FactPatternMatch(theEnv,FactData(theEnv)->CurrentPatternFact,
                          thePattern->nextLevel,offset + repeatCount - 1,
                          FactData(theEnv)->CurrentPatternMarks,newMark);
        }
     }

    /*======================================================*/
    /* Get rid of the marker created for a multifield node. */
    /*======================================================*/

    rtn_struct(theEnv,multifieldMarker,newMark);
    if (endMark != NULL) endMark->next = NULL;
    FactData(theEnv)->CurrentPatternMarks = oldMark;
   }

/******************************************************/
/* GetNextFactPatternNode: Returns the next node in a */
/*   pattern network tree to be traversed. The next   */
/*   node is computed using a depth first traversal.  */
/******************************************************/
static struct factPatternNode *GetNextFactPatternNode(
  void *theEnv,
  int finishedMatching,
  struct factPatternNode *thePattern)
  {
   EvaluationData(theEnv)->EvaluationError = FALSE;

   /*===================================================*/
   /* If pattern matching was successful at the current */
   /* node in the tree and it's possible to go deeper   */
   /* into the tree, then move down to the next level.  */
   /*===================================================*/

   if (finishedMatching == FALSE)
     { if (thePattern->nextLevel != NULL) return(thePattern->nextLevel); }

   /*================================================*/
   /* Keep backing up toward the root of the pattern */
   /* network until a side branch can be taken.      */
   /*================================================*/

   while ((thePattern->rightNode == NULL) ||
          ((thePattern->lastLevel != NULL) &&
           (thePattern->lastLevel->header.selector)))
     {
      /*========================================*/
      /* Back up to check the next side branch. */
      /*========================================*/

      thePattern = thePattern->lastLevel;

      /*======================================*/
      /* If we branched up from the root, the */
      /* entire tree has been traversed.      */
      /*======================================*/

      if (thePattern == NULL) return(NULL);
      
      /*======================================*/
      /* Skip selector constants and pop back */
      /* back to the selector node.           */
      /*======================================*/

      if ((thePattern->lastLevel != NULL) &&
          (thePattern->lastLevel->header.selector))
        { thePattern = thePattern->lastLevel; }

      /*===================================================*/
      /* If we branched up to a multifield node, then stop */
      /* since these nodes are handled recursively. The    */
      /* previous call to the pattern matching algorithm   */
      /* on the stack will handle backing up to the nodes  */
      /* above the multifield node in the pattern network. */
      /*===================================================*/

      if (thePattern->header.multifieldNode) return(NULL);
     }

   /*==================================*/
   /* Move on to the next side branch. */
   /*==================================*/

   return(thePattern->rightNode);
  }

/*******************************************************/
/* ProcessFactAlphaMatch: When a fact pattern has been */
/*   satisfied, this routine creates an alpha match to */
/*   store in the pattern network and then sends the   */
/*   new alpha match through the join network.         */
/*******************************************************/
static void ProcessFactAlphaMatch(
  void *theEnv,
  struct fact *theFact,
  struct multifieldMarker *theMarks,
  struct factPatternNode *thePattern)
  {
   struct partialMatch *theMatch;
   struct patternMatch *listOfMatches;
   struct joinNode *listOfJoins;
   unsigned long hashValue;

  /*============================================*/
  /* Create the hash value for the alpha match. */
  /*============================================*/

  hashValue = ComputeRightHashValue(theEnv,&thePattern->header);

  /*===========================================*/
  /* Create the partial match for the pattern. */
  /*===========================================*/

  theMatch = CreateAlphaMatch(theEnv,theFact,theMarks,(struct patternNodeHeader *) &thePattern->header,hashValue);
  theMatch->owner = &thePattern->header;

  /*=======================================================*/
  /* Add the pattern to the list of matches for this fact. */
  /*=======================================================*/

  listOfMatches = (struct patternMatch *) theFact->list;
  theFact->list = (void *) get_struct(theEnv,patternMatch);
  ((struct patternMatch *) theFact->list)->next = listOfMatches;
  ((struct patternMatch *) theFact->list)->matchingPattern = (struct patternNodeHeader *) thePattern;
  ((struct patternMatch *) theFact->list)->theMatch = theMatch;

  /*================================================================*/
  /* Send the partial match to the joins connected to this pattern. */
  /*================================================================*/

  for (listOfJoins = thePattern->header.entryJoin;
       listOfJoins != NULL;
       listOfJoins = listOfJoins->rightMatchNode)
     { NetworkAssert(theEnv,theMatch,listOfJoins); }
  }

/*****************************************************************/
/* EvaluatePatternExpression: Performs a faster evaluation for   */
/*   fact pattern network expressions than if EvaluateExpression */
/*   were used directly.                                         */
/*****************************************************************/
static int EvaluatePatternExpression(
  void *theEnv,
  struct factPatternNode *patternPtr,
  struct expr *theTest)
  {
   DATA_OBJECT theResult;
   struct expr *oldArgument;
   int rv;

   /*=====================================*/
   /* A pattern node without a constraint */
   /* is always satisfied.                */
   /*=====================================*/

   if (theTest == NULL) return(TRUE);

   /*======================================*/
   /* Evaluate pattern network primitives. */
   /*======================================*/

   switch(theTest->type)
     {
      /*==============================================*/
      /* This primitive compares the value stored in  */
      /* a single field slot to a constant for either */
      /* equality or inequality.                      */
      /*==============================================*/

      case FACT_PN_CONSTANT1:
        oldArgument = EvaluationData(theEnv)->CurrentExpression;
        EvaluationData(theEnv)->CurrentExpression = theTest;
        rv = FactPNConstant1(theEnv,theTest->value,&theResult);
        EvaluationData(theEnv)->CurrentExpression = oldArgument;
        return(rv);

      /*=============================================*/
      /* This primitive compares the value stored in */
      /* a multifield slot to a constant for either  */
      /* equality or inequality.                     */
      /*=============================================*/

      case FACT_PN_CONSTANT2:
        oldArgument = EvaluationData(theEnv)->CurrentExpression;
        EvaluationData(theEnv)->CurrentExpression = theTest;
        rv = FactPNConstant2(theEnv,theTest->value,&theResult);
        EvaluationData(theEnv)->CurrentExpression = oldArgument;
        return(rv);

      /*================================================*/
      /* This primitive determines if a multifield slot */
      /* contains at least a certain number of fields.  */
      /*================================================*/

      case FACT_SLOT_LENGTH:
        oldArgument = EvaluationData(theEnv)->CurrentExpression;
        EvaluationData(theEnv)->CurrentExpression = theTest;
        rv = FactSlotLength(theEnv,theTest->value,&theResult);
        EvaluationData(theEnv)->CurrentExpression = oldArgument;
        return(rv);
     }

   /*==============================================*/
   /* Evaluate "or" expressions by evaluating each */
   /* argument and return TRUE if any of them      */
   /* evaluated to TRUE, otherwise return FALSE.   */
   /*==============================================*/

   if (theTest->value == ExpressionData(theEnv)->PTR_OR)
     {
      for (theTest = theTest->argList;
           theTest != NULL;
           theTest = theTest->nextArg)
        {
         if (EvaluatePatternExpression(theEnv,patternPtr,theTest) == TRUE)
           {
            if (EvaluationData(theEnv)->EvaluationError) return(FALSE);
            return(TRUE);
           }
         if (EvaluationData(theEnv)->EvaluationError) return(FALSE);
        }

      return(FALSE);
     }

   /*===============================================*/
   /* Evaluate "and" expressions by evaluating each */
   /* argument and return FALSE if any of them      */
   /* evaluated to FALSE, otherwise return TRUE.    */
   /*===============================================*/

   else if (theTest->value == ExpressionData(theEnv)->PTR_AND)
     {
      for (theTest = theTest->argList;
           theTest != NULL;
           theTest = theTest->nextArg)
        {
         if (EvaluatePatternExpression(theEnv,patternPtr,theTest) == FALSE)
           { return(FALSE); }
         if (EvaluationData(theEnv)->EvaluationError) return(FALSE);
        }

      return(TRUE);
     }

   /*==========================================================*/
   /* Evaluate all other expressions using EvaluateExpression. */
   /*==========================================================*/

   if (EvaluateExpression(theEnv,theTest,&theResult))
     {
      PatternNetErrorMessage(theEnv,patternPtr);
      return(FALSE);
     }

   if ((theResult.value == EnvFalseSymbol(theEnv)) && (theResult.type == SYMBOL))
     { return(FALSE); }

   return(TRUE);
  }

/************************************************************************/
/* PatternNetErrorMessage: Prints the informational header to the error */
/*   message that occurs when a error occurs as the  result of          */
/*   evaluating an expression in the fact pattern network. Prints the   */
/*   fact currently being pattern matched and the field number or slot  */
/*   name in the pattern from which the error originated. The error is  */
/*   then trace to the point where the pattern enters the join network  */
/*   so that the names of the rule which utilize the pattern can also   */
/*   be printed.                                                        */
/************************************************************************/
static void PatternNetErrorMessage(
  void *theEnv,
  struct factPatternNode *patternPtr)
  {
   char buffer[60];
   struct templateSlot *theSlots;
   int i;

   /*=======================================*/
   /* Print the fact being pattern matched. */
   /*=======================================*/

   PrintErrorID(theEnv,"FACTMCH",1,TRUE);
   EnvPrintRouter(theEnv,WERROR,"This error occurred in the fact pattern network\n");
   EnvPrintRouter(theEnv,WERROR,"   Currently active fact: ");
   PrintFact(theEnv,WERROR,FactData(theEnv)->CurrentPatternFact,FALSE,FALSE);
   EnvPrintRouter(theEnv,WERROR,"\n");

   /*==============================================*/
   /* Print the field position or slot name of the */
   /* pattern from which the error originated.     */
   /*==============================================*/

   if (FactData(theEnv)->CurrentPatternFact->whichDeftemplate->implied)
     { gensprintf(buffer,"   Problem resides in field #%d\n",patternPtr->whichField); }
   else
     {
      theSlots = FactData(theEnv)->CurrentPatternFact->whichDeftemplate->slotList;
      for (i = 0; i < (int) patternPtr->whichSlot; i++) theSlots = theSlots->next;
      gensprintf(buffer,"   Problem resides in slot %s\n",ValueToString(theSlots->slotName));
     }

   EnvPrintRouter(theEnv,WERROR,buffer);

   /*==========================================================*/
   /* Trace the pattern to its entry point to the join network */
   /* (which then traces to the defrule data structure so that */
   /* the name(s) of the rule(s) utilizing the patterns can be */
   /* printed).                                                */
   /*==========================================================*/

   TraceErrorToJoin(theEnv,patternPtr,FALSE);
   EnvPrintRouter(theEnv,WERROR,"\n");
  }

/***************************************************************************/
/* TraceErrorToJoin: Traces the cause of an evaluation error which occured */
/*   in the fact pattern network to the entry join in the join network for */
/*   the pattern from which the error originated. Once the entry join is   */
/*   reached, the error is then traced to the defrule data structures so   */
/*   that the name of the rule(s) containing the pattern can be printed.   */
/***************************************************************************/
static void TraceErrorToJoin(
  void *theEnv,
  struct factPatternNode *patternPtr,
  int traceRight)
  {
   struct joinNode *joinPtr;

   while (patternPtr != NULL)
     {
      if (patternPtr->header.stopNode)
        {
         for (joinPtr = patternPtr->header.entryJoin;
              joinPtr != NULL;
              joinPtr = joinPtr->rightMatchNode)
           { TraceErrorToRule(theEnv,joinPtr,"      "); }
        }
      else
        { TraceErrorToJoin(theEnv,patternPtr->nextLevel,TRUE); }

      if (traceRight) patternPtr = patternPtr->rightNode;
      else patternPtr = NULL;
     }
  }

/***********************************************************************/
/* SkipFactPatternNode: During an incremental reset, only fact pattern */
/*   nodes associated with new patterns are traversed. Given a pattern */
/*   node, this routine will return TRUE if the pattern node should be */
/*   traversed during incremental reset pattern matching or FALSE if   */
/*   the node should be skipped.                                       */
/***********************************************************************/
static int SkipFactPatternNode(
  void *theEnv,
  struct factPatternNode *thePattern)
  {
#if (MAC_MCW || WIN_MCW) && (RUN_TIME || BLOAD_ONLY)
#pragma unused(theEnv,thePattern)
#endif

#if (! RUN_TIME) && (! BLOAD_ONLY)
   if (EngineData(theEnv)->IncrementalResetInProgress &&
       (thePattern->header.initialize == FALSE))
     { return(TRUE); }
#endif

   return(FALSE);
  }

/***************************************************************/
/* MarkFactPatternForIncrementalReset: Sets the initialization */
/*  field of a fact pattern for use with incremental reset.    */
/*  This is called before an incremental reset for newly added */
/*  patterns to indicate that the pattern nodes should be      */
/*  traversed and then after an incremental reset to indicate  */
/*  that the nodes were traversed ("initialized") by the       */
/*  incremental reset.                                         */
/***************************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle void MarkFactPatternForIncrementalReset(
  void *theEnv,
  struct patternNodeHeader *thePattern,
  int value)
  {
   struct factPatternNode *patternPtr = (struct factPatternNode *) thePattern;
   struct joinNode *theJoin;
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   /*=====================================*/
   /* We should be passed a valid pointer */
   /* to a fact pattern network node.     */
   /*=====================================*/

   Bogus(patternPtr == NULL);

   /*===============================================================*/
   /* If the pattern was previously initialized,  then don't bother */
   /* with it unless the pattern was subsumed by another pattern    */
   /* and associated with a join that hasn't been initialized.      */
   /* DR0880 2008-01-24                                             */
   /*===============================================================*/

   if (patternPtr->header.initialize == FALSE)
     { 
      for (theJoin = patternPtr->header.entryJoin;
           theJoin != NULL;
           theJoin = theJoin->rightMatchNode)
        {
         if (theJoin->initialize == FALSE)
           { return; }
        }
     }

   /*======================================================*/
   /* Set the initialization field of this pattern network */
   /* node and all pattern network nodes which preceed it. */
   /*======================================================*/

   while (patternPtr != NULL)
     {
      patternPtr->header.initialize = value;
      patternPtr = patternPtr->lastLevel;
     }
  }

/**************************************************************/
/* FactsIncrementalReset: Incremental reset function for the  */
/*   fact pattern network. Asserts all facts in the fact-list */
/*   so that they repeat the pattern matching process. During */
/*   an incremental reset, newly added patterns should be the */
/*   only active patterns in the fact pattern network.        */
/**************************************************************/
globle void FactsIncrementalReset(
  void *theEnv)
  {
   struct fact *factPtr;

   for (factPtr = (struct fact *) EnvGetNextFact(theEnv,NULL);
        factPtr != NULL;
        factPtr = (struct fact *) EnvGetNextFact(theEnv,factPtr))
     {
      EngineData(theEnv)->JoinOperationInProgress = TRUE;
      FactPatternMatch(theEnv,factPtr,
                       factPtr->whichDeftemplate->patternNetwork,
                       0,NULL,NULL);
      EngineData(theEnv)->JoinOperationInProgress = FALSE;
     }
  }

#endif /* DEFTEMPLATE_CONSTRUCT && DEFRULE_CONSTRUCT */

