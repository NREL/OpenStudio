   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*              INCREMENTAL RESET MODULE               */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides functionality for the incremental       */
/*   reset of the pattern and join networks when a new       */
/*   rule is added.                                          */
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
/*************************************************************/

#define _INCRRSET_SOURCE_

#include "setup.h"

#include <stdio.h>
#define _STDIO_INCLUDED_

#if DEFRULE_CONSTRUCT

#include "agenda.h"
#include "argacces.h"
#include "constant.h"
#include "drive.h"
#include "engine.h"
#include "envrnmnt.h"
#include "evaluatn.h"
#include "pattern.h"
#include "router.h"
#include "reteutil.h"

#include "incrrset.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if (! RUN_TIME) && (! BLOAD_ONLY)
   static void                    MarkNetworkForIncrementalReset(void *,struct defrule *,int);
   static void                    MarkJoinsForIncrementalReset(void *,struct joinNode *,int);
   static void                    CheckForPrimableJoins(void *,struct defrule *,struct joinNode *);
   static void                    PrimeJoinFromLeftMemory(void *,struct joinNode *);
   static void                    PrimeJoinFromRightMemory(void *,struct joinNode *);
   static void                    MarkPatternForIncrementalReset(void *,int,struct patternNodeHeader *,int);
#endif

/**************************************************************/
/* IncrementalReset: Incrementally resets the specified rule. */
/**************************************************************/
globle void IncrementalReset(
  void *theEnv,
  struct defrule *tempRule)
  {
#if (MAC_MCW || WIN_MCW) && (RUN_TIME || BLOAD_ONLY)
#pragma unused(theEnv,tempRule)
#endif

#if (! RUN_TIME) && (! BLOAD_ONLY)
   struct defrule *tempPtr;
   struct patternParser *theParser;

   /*================================================*/
   /* If incremental reset is disabled, then return. */
   /*================================================*/

   if (! EnvGetIncrementalReset(theEnv)) return;

   /*=====================================================*/
   /* Mark the pattern and join network data structures   */
   /* associated with the rule being incrementally reset. */
   /*=====================================================*/

   MarkNetworkForIncrementalReset(theEnv,tempRule,TRUE);

   /*==========================*/
   /* Begin incremental reset. */
   /*==========================*/

   EngineData(theEnv)->IncrementalResetInProgress = TRUE;

   /*============================================================*/
   /* If the new rule shares patterns or joins with other rules, */
   /* then it is necessary to update its join network based on   */
   /* existing partial matches it shares with other rules.       */
   /*============================================================*/

   for (tempPtr = tempRule;
        tempPtr != NULL;
        tempPtr = tempPtr->disjunct)
     { CheckForPrimableJoins(theEnv,tempPtr,tempPtr->lastJoin); }

   /*===============================================*/
   /* Filter existing data entities through the new */
   /* portions of the pattern and join networks.    */
   /*===============================================*/

   for (theParser = PatternData(theEnv)->ListOfPatternParsers;
        theParser != NULL;
        theParser = theParser->next)
     {
      if (theParser->incrementalResetFunction != NULL)
        { (*theParser->incrementalResetFunction)(theEnv); }
     }

   /*========================*/
   /* End incremental reset. */
   /*========================*/

   EngineData(theEnv)->IncrementalResetInProgress = FALSE;

   /*====================================================*/
   /* Remove the marks in the pattern and join networks. */
   /*====================================================*/

   MarkNetworkForIncrementalReset(theEnv,tempRule,FALSE);
#endif
  }

#if (! RUN_TIME) && (! BLOAD_ONLY)

/**********************************************************************/
/* MarkNetworkForIncrementalReset: Coordinates marking the initialize */
/*   flags in the pattern and join networks both before and after an  */
/*   incremental reset.                                               */
/**********************************************************************/
static void MarkNetworkForIncrementalReset(
  void *theEnv,
  struct defrule *tempRule,
  int value)
  {
   /*============================================*/
   /* Loop through each of the rule's disjuncts. */
   /*============================================*/

   for (;
        tempRule != NULL;
        tempRule = tempRule->disjunct)
     { MarkJoinsForIncrementalReset(theEnv,tempRule->lastJoin,value); }
  }

/**********************************************************************/
/* MarkJoinsForIncrementalReset: Coordinates marking the initialize */
/*   flags in the pattern and join networks both before and after an  */
/*   incremental reset.                                               */
/**********************************************************************/
static void MarkJoinsForIncrementalReset(
  void *theEnv,
  struct joinNode *joinPtr,
  int value)
  {
   struct patternNodeHeader *patternPtr;

   for (;
        joinPtr != NULL;
        joinPtr = joinPtr->lastLevel)
     {
      if (joinPtr->ruleToActivate != NULL)
        { 
         joinPtr->marked = FALSE;
         joinPtr->initialize = value;
         continue; 
        }
        
      if (joinPtr->joinFromTheRight)
        { MarkJoinsForIncrementalReset(theEnv,(struct joinNode *) joinPtr->rightSideEntryStructure,value); }

      /*================*/
      /* Mark the join. */
      /*================*/

      joinPtr->marked = FALSE; /* GDR 6.05 */
        
      if (joinPtr->initialize) 
        {
         joinPtr->initialize = value;
         if (joinPtr->joinFromTheRight == FALSE)
           {
            patternPtr = (struct patternNodeHeader *) GetPatternForJoin(joinPtr);
            if (patternPtr != NULL)
              { MarkPatternForIncrementalReset(theEnv,(int) joinPtr->rhsType,patternPtr,value); }
           }
        }
     }
  }
  
/*******************************************************************************/
/* CheckForPrimableJoins: Updates the joins of a rule for an incremental reset */
/*   if portions of that rule are shared with other rules that have already    */
/*   been incrementally reset. A join for a new rule will be updated if it is  */
/*   marked for initialization and either its parent join or its associated    */
/*   entry pattern node has not been marked for initialization. The function   */
/*   PrimeJoin is used to update joins which meet these criteria.              */
/*******************************************************************************/
static void CheckForPrimableJoins(
  void *theEnv,
  struct defrule *tempRule,
  struct joinNode *joinPtr)
  {   
   /*========================================*/
   /* Loop through each of the rule's joins. */
   /*========================================*/

   for (;
        joinPtr != NULL;
        joinPtr = joinPtr->lastLevel)
     {
      /*===============================*/
      /* Update the join if necessary. */
      /*===============================*/

      if ((joinPtr->initialize) && (! joinPtr->marked))
        {
         if (joinPtr->firstJoin == TRUE)
           {
			if (joinPtr->joinFromTheRight == FALSE)
              {
               if ((joinPtr->rightSideEntryStructure == NULL) ||
                   (joinPtr->patternIsNegated) ||
                   (((struct patternNodeHeader *) joinPtr->rightSideEntryStructure)->initialize == FALSE))
                 {
                  PrimeJoinFromLeftMemory(theEnv,joinPtr);
                  joinPtr->marked = TRUE;
                 }
              }
            else
              {
               PrimeJoinFromRightMemory(theEnv,joinPtr);
               joinPtr->marked = TRUE;
              }
           }
         else if (joinPtr->lastLevel->initialize == FALSE)
           { 
            PrimeJoinFromLeftMemory(theEnv,joinPtr); 
            joinPtr->marked = TRUE;
           }
         else if ((joinPtr->joinFromTheRight) &&
                (((struct joinNode *) joinPtr->rightSideEntryStructure)->initialize == FALSE))
           {
            PrimeJoinFromRightMemory(theEnv,joinPtr);
            joinPtr->marked = TRUE;
           }
        }
        
      if (joinPtr->joinFromTheRight)
        { CheckForPrimableJoins(theEnv,tempRule,(struct joinNode *) joinPtr->rightSideEntryStructure); }
     }
  }

/****************************************************************************/
/* PrimeJoinFromLeftMemory: Updates a join in a rule for an incremental     */
/*   reset. Joins are updated by "priming" them only if the join (or its    */
/*   associated pattern) is shared with other rules that have already been  */
/*   incrementally reset. A join for a new rule will be updated if it is    */
/*   marked for initialization and either its parent join or its associated */
/*   entry pattern node has not been marked for initialization.             */
/****************************************************************************/
static void PrimeJoinFromLeftMemory(
  void *theEnv,
  struct joinNode *joinPtr)
  {
   struct partialMatch *theList, *linker;
   struct alphaMemoryHash *listOfHashNodes;
   unsigned long b;
   unsigned long hashValue;
   struct betaMemory *theMemory;
   struct partialMatch *notParent;
   struct joinLink *tempLink;

   /*===========================================================*/
   /* If the join is the first join of a rule, then send all of */
   /* the partial matches from the alpha memory of the pattern  */
   /* associated with this join to the join for processing and  */
   /* the priming process is then complete.                     */
   /*===========================================================*/

   if (joinPtr->firstJoin == TRUE)
     {
      if (joinPtr->rightSideEntryStructure == NULL)
        { NetworkAssert(theEnv,joinPtr->rightMemory->beta[0],joinPtr); }
      else if (joinPtr->patternIsNegated)
        { 
         notParent = joinPtr->leftMemory->beta[0];
         
         if (joinPtr->secondaryNetworkTest != NULL)
           {
            if (EvaluateSecondaryNetworkTest(theEnv,notParent,joinPtr) == FALSE)
              { return; }
           }

         for (listOfHashNodes = ((struct patternNodeHeader *) joinPtr->rightSideEntryStructure)->firstHash;
              listOfHashNodes != NULL;
              listOfHashNodes = listOfHashNodes->nextHash)
           {
            if (listOfHashNodes->alphaMemory != NULL)
              { 
               AddBlockedLink(notParent,listOfHashNodes->alphaMemory);
               return; 
              }
           }

         EPMDrive(theEnv,notParent,joinPtr);
        }
      else
        {  
         for (listOfHashNodes = ((struct patternNodeHeader *) joinPtr->rightSideEntryStructure)->firstHash;
              listOfHashNodes != NULL;
              listOfHashNodes = listOfHashNodes->nextHash)
           {
            for (theList = listOfHashNodes->alphaMemory;
                 theList != NULL;
                 theList = theList->nextInMemory)
              { NetworkAssert(theEnv,theList,joinPtr); }
           }
        }  
      return;
     }

   /*========================================*/
   /* Find another beta memory from which we */
   /* can retrieve the partial matches.      */
   /*========================================*/
   
   tempLink = joinPtr->lastLevel->nextLinks;
      
   while (tempLink != NULL)
     {
      if ((tempLink->join != joinPtr) &&
          (tempLink->join->initialize == FALSE))
        { break; }
      
      tempLink = tempLink->next;
     }
     
   if (tempLink == NULL) return;
   
   if (tempLink->enterDirection == LHS)
     { theMemory = tempLink->join->leftMemory; }
   else
     { theMemory = tempLink->join->rightMemory; }
   
   /*============================================*/
   /* Send all partial matches from the selected */
   /* beta memory to the new join.               */
   /*============================================*/

   for (b = 0; b < theMemory->size; b++)
     {
      for (theList = theMemory->beta[b];
           theList != NULL;
           theList = theList->nextInMemory)
        {
         linker = CopyPartialMatch(theEnv,theList);
                                   
         if (joinPtr->leftHash != NULL)
           { hashValue = BetaMemoryHashValue(theEnv,joinPtr->leftHash,linker,NULL,joinPtr); }
         else
           { hashValue = 0; }
         
         UpdateBetaPMLinks(theEnv,linker,theList->leftParent,theList->rightParent,joinPtr,hashValue,LHS);
         
         NetworkAssertLeft(theEnv,linker,joinPtr);
        }
     }
  }
  
/****************************************************************************/
/* PrimeJoinFromRightMemory: Updates a join in a rule for an incremental    */
/*   reset. Joins are updated by "priming" them only if the join (or its    */
/*   associated pattern) is shared with other rules that have already been  */
/*   incrementally reset. A join for a new rule will be updated if it is    */
/*   marked for initialization and either its parent join or its associated */
/*   entry pattern node has not been marked for initialization.             */
/****************************************************************************/
static void PrimeJoinFromRightMemory(
  void *theEnv,
  struct joinNode *joinPtr)
  {
   struct partialMatch *theList, *linker;
   unsigned long b;
   struct betaMemory *theMemory;
   unsigned long hashValue;
   struct joinLink *tempLink;
   struct partialMatch *notParent;

   /*=======================================*/
   /* This should be a join from the right. */
   /*=======================================*/
   
   if (joinPtr->joinFromTheRight == FALSE)
     { return; }
     
   /*========================================*/
   /* Find another beta memory from which we */
   /* can retrieve the partial matches.      */
   /*========================================*/
      
   tempLink = ((struct joinNode *) joinPtr->rightSideEntryStructure)->nextLinks;
   while (tempLink != NULL)
     {
      if ((tempLink->join != joinPtr) && 
          (tempLink->join->initialize == FALSE))
        { break; }
      
      tempLink = tempLink->next;
     }
     
   if (tempLink == NULL) 
     {
      if (joinPtr->firstJoin &&
          (joinPtr->rightMemory->beta[0] == NULL) &&
          (! joinPtr->patternIsExists))
        {
         notParent = joinPtr->leftMemory->beta[0];

         if (joinPtr->secondaryNetworkTest != NULL)
           {
            if (EvaluateSecondaryNetworkTest(theEnv,notParent,joinPtr) == FALSE)
              { return; }
           }

         EPMDrive(theEnv,notParent,joinPtr);
        }

      return;
     }
   
   if (tempLink->enterDirection == LHS)
     { theMemory = tempLink->join->leftMemory; }
   else
     { theMemory = tempLink->join->rightMemory; }

   /*============================================*/
   /* Send all partial matches from the selected */
   /* beta memory to the new join.               */
   /*============================================*/

   for (b = 0; b < theMemory->size; b++)
     {
      for (theList = theMemory->beta[b];
           theList != NULL;
           theList = theList->nextInMemory)
        {
         linker = CopyPartialMatch(theEnv,theList);
                                   
         if (joinPtr->rightHash != NULL)
           { hashValue = BetaMemoryHashValue(theEnv,joinPtr->rightHash,linker,NULL,joinPtr); }
         else
           { hashValue = 0; }
         
         UpdateBetaPMLinks(theEnv,linker,theList->leftParent,theList->rightParent,joinPtr,hashValue,RHS);
         NetworkAssert(theEnv,linker,joinPtr); 
        }
     }
           
   if (joinPtr->firstJoin &&
       (joinPtr->rightMemory->beta[0] == NULL) &&
       (! joinPtr->patternIsExists))
     {
      notParent = joinPtr->leftMemory->beta[0];

      if (joinPtr->secondaryNetworkTest != NULL)
        {
         if (EvaluateSecondaryNetworkTest(theEnv,notParent,joinPtr) == FALSE)
           { return; }
        }

      EPMDrive(theEnv,notParent,joinPtr);
     }
  }
  
/*********************************************************************/
/* MarkPatternForIncrementalReset: Given a pattern node and its type */
/*   (fact, instance, etc.), calls the appropriate function to mark  */
/*   the pattern for an incremental reset. Used to mark the pattern  */
/*   nodes both before and after an incremental reset.               */
/*********************************************************************/
static void MarkPatternForIncrementalReset(
  void *theEnv,
  int rhsType,
  struct patternNodeHeader *theHeader,
  int value)
  {
   struct patternParser *tempParser;
   
   tempParser = GetPatternParser(theEnv,rhsType);

   if (tempParser != NULL)
     {
      if (tempParser->markIRPatternFunction != NULL)
        { (*tempParser->markIRPatternFunction)(theEnv,theHeader,value); }
     }
  }

#endif

/********************************************/
/* EnvGetIncrementalReset: C access routine */
/*   for the get-incremental-reset command. */
/********************************************/
globle intBool EnvGetIncrementalReset(
  void *theEnv)
  {   
   return(EngineData(theEnv)->IncrementalResetFlag);
  }

/********************************************/
/* EnvSetIncrementalReset: C access routine */
/*   for the set-incremental-reset command. */
/********************************************/
globle intBool EnvSetIncrementalReset(
  void *theEnv,
  int value)
  {
   int ov;
   struct defmodule *theModule;

   SaveCurrentModule(theEnv);

   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);
      if (EnvGetNextDefrule(theEnv,NULL) != NULL)
        {
         RestoreCurrentModule(theEnv);
         return(-1);
        }
     }
     
   RestoreCurrentModule(theEnv);

   ov = EngineData(theEnv)->IncrementalResetFlag;
   EngineData(theEnv)->IncrementalResetFlag = value;
   return(ov);
  }

/****************************************************/
/* SetIncrementalResetCommand: H/L access routine   */
/*   for the set-incremental-reset command.         */
/****************************************************/
globle int SetIncrementalResetCommand(
  void *theEnv)
  {
   int oldValue;
   DATA_OBJECT argPtr;
   struct defmodule *theModule;

   oldValue = EnvGetIncrementalReset(theEnv);

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if (EnvArgCountCheck(theEnv,"set-incremental-reset",EXACTLY,1) == -1)
     { return(oldValue); }

   /*=========================================*/
   /* The incremental reset behavior can't be */
   /* changed when rules are loaded.          */
   /*=========================================*/

   SaveCurrentModule(theEnv);

   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);
      if (EnvGetNextDefrule(theEnv,NULL) != NULL)
        {
         RestoreCurrentModule(theEnv);
         PrintErrorID(theEnv,"INCRRSET",1,FALSE);
         EnvPrintRouter(theEnv,WERROR,"The incremental reset behavior cannot be changed with rules loaded.\n");
         SetEvaluationError(theEnv,TRUE);
         return(oldValue);
        }
     }
     
   RestoreCurrentModule(theEnv);

   /*==================================================*/
   /* The symbol FALSE disables incremental reset. Any */
   /* other value enables incremental reset.           */
   /*==================================================*/

   EnvRtnUnknown(theEnv,1,&argPtr);

   if ((argPtr.value == EnvFalseSymbol(theEnv)) && (argPtr.type == SYMBOL))
     { EnvSetIncrementalReset(theEnv,FALSE); }
   else
     { EnvSetIncrementalReset(theEnv,TRUE); }

   /*=======================*/
   /* Return the old value. */
   /*=======================*/

   return(oldValue);
  }

/****************************************************/
/* GetIncrementalResetCommand: H/L access routine   */
/*   for the get-incremental-reset command.         */
/****************************************************/
globle int GetIncrementalResetCommand(
  void *theEnv)
  {
   int oldValue;

   oldValue = EnvGetIncrementalReset(theEnv);

   if (EnvArgCountCheck(theEnv,"get-incremental-reset",EXACTLY,0) == -1)
     { return(oldValue); }

   return(oldValue);
  }

#endif /* DEFRULE_CONSTRUCT */
