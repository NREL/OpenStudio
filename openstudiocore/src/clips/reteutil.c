   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                 RETE UTILITY MODULE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides a set of utility functions useful to    */
/*   other modules.                                          */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Removed INCREMENTAL_RESET compilation flag.    */
/*                                                           */
/*            Rule with exists CE has incorrect activation.  */
/*            DR0867                                         */
/*                                                           */
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*            Removed pseudo-facts used in not CEs.          */
/*                                                           */
/*************************************************************/

#define _RETEUTIL_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "setup.h"

#if DEFRULE_CONSTRUCT

#include "drive.h"
#include "engine.h"
#include "envrnmnt.h"
#include "incrrset.h"
#include "match.h"
#include "memalloc.h"
#include "moduldef.h"
#include "pattern.h"
#include "retract.h"
#include "router.h"

#include "reteutil.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                        TraceErrorToRuleDriver(void *,struct joinNode *,char *,int,int);
   static struct alphaMemoryHash     *FindAlphaMemory(void *,struct patternNodeHeader *,unsigned long);
   static unsigned long               AlphaMemoryHashValue(struct patternNodeHeader *,unsigned long);
   static void                        UnlinkAlphaMemory(void *,struct patternNodeHeader *,struct alphaMemoryHash *);
   static void                        UnlinkAlphaMemoryBucketSiblings(void *,struct alphaMemoryHash *);
   static void                        InitializePMLinks(struct partialMatch *);
   static void                        UnlinkBetaPartialMatchfromAlphaAndBetaLineage(struct partialMatch *);
   static int                         CountPriorPatterns(struct joinNode *);
   static void                        ResizeBetaMemory(void *,struct betaMemory *);
   static void                        ResetBetaMemory(void *,struct betaMemory *);
#if (CONSTRUCT_COMPILER || BLOAD_AND_BSAVE) && (! RUN_TIME)
   static void                        TagNetworkTraverseJoins(void *,long int *,long int *,struct joinNode *);
#endif
   
/***********************************************************/
/* PrintPartialMatch: Prints out the list of fact indices  */
/*   and/or instance names associated with a partial match */
/*   or rule instantiation.                                */
/***********************************************************/
globle void PrintPartialMatch(
  void *theEnv,
  char *logicalName,
  struct partialMatch *list)
  {
   struct patternEntity *matchingItem;
   unsigned short i;

   for (i = 0; i < list->bcount;)
     {
      if ((get_nth_pm_match(list,i) != NULL) &&
          (get_nth_pm_match(list,i)->matchingItem != NULL))
        {
         matchingItem = get_nth_pm_match(list,i)->matchingItem;
         (*matchingItem->theInfo->base.shortPrintFunction)(theEnv,logicalName,matchingItem);
        }
      else
        { EnvPrintRouter(theEnv,logicalName,"*"); }
      i++;
      if (i < list->bcount) EnvPrintRouter(theEnv,logicalName,",");
     }
  }

/**********************************************/
/* CopyPartialMatch:  Copies a partial match. */
/**********************************************/
globle struct partialMatch *CopyPartialMatch(
  void *theEnv,
  struct partialMatch *list)
  {
   struct partialMatch *linker;
   unsigned short i;

   linker = get_var_struct(theEnv,partialMatch,sizeof(struct genericMatch) *
                                        (list->bcount - 1));

   InitializePMLinks(linker);
   linker->betaMemory = TRUE;
   linker->busy = FALSE;
   linker->rhsMemory = FALSE;
   linker->bcount = list->bcount;
   linker->hashValue = 0;

   for (i = 0; i < linker->bcount; i++) linker->binds[i] = list->binds[i];

   return(linker);
  }

/**********************************************/
/* CreateEmptyPartialMatch:  */
/**********************************************/
globle struct partialMatch *CreateEmptyPartialMatch(
  void *theEnv)
  {
   struct partialMatch *linker;

   linker = get_struct(theEnv,partialMatch);

   InitializePMLinks(linker);
   linker->betaMemory = TRUE;
   linker->busy = FALSE;
   linker->rhsMemory = FALSE;
   linker->bcount = 1;
   linker->hashValue = 0;
   linker->binds[0].gm.theValue = NULL;

   return(linker);
  }

/****************************************************/
/* InitializePMLinks: */
/****************************************************/
static void InitializePMLinks(
  struct partialMatch *theMatch)
  {
   theMatch->nextInMemory = NULL;
   theMatch->prevInMemory = NULL;
   theMatch->nextRightChild = NULL;
   theMatch->prevRightChild = NULL;
   theMatch->nextLeftChild = NULL;
   theMatch->prevLeftChild = NULL;
   theMatch->children = NULL;
   theMatch->rightParent = NULL;
   theMatch->leftParent = NULL;
   theMatch->blockList = NULL;
   theMatch->nextBlocked = NULL;
   theMatch->prevBlocked = NULL;
   theMatch->marker = NULL;
   theMatch->dependents = NULL;
  }
  
/***********************************************************/
/* UpdateBetaPMLinks: . */
/***********************************************************/
globle void UpdateBetaPMLinks(
  void *theEnv,
  struct partialMatch *thePM,
  struct partialMatch *lhsBinds,
  struct partialMatch *rhsBinds,
  struct joinNode *join,
  unsigned long hashValue,
  int side)
  {
   unsigned long betaLocation;
   struct betaMemory *theMemory;
   
   if (side == LHS)
     { 
      theMemory = join->leftMemory; 
      thePM->rhsMemory = FALSE;
     }
   else
     {
      theMemory = join->rightMemory;
      thePM->rhsMemory = TRUE;
     }
   
   thePM->hashValue = hashValue;
     
   /*================================*/
   /* Update the node's linked list. */
   /*================================*/

   betaLocation = hashValue % theMemory->size;
   
   if (side == LHS)
     {
      thePM->nextInMemory = theMemory->beta[betaLocation];
      if (theMemory->beta[betaLocation] != NULL)
        { theMemory->beta[betaLocation]->prevInMemory = thePM; }
      theMemory->beta[betaLocation] = thePM;
     }
   else
     {
      if (theMemory->last[betaLocation] != NULL)
        {
         theMemory->last[betaLocation]->nextInMemory = thePM;
         thePM->prevInMemory = theMemory->last[betaLocation];
        }
      else
        { theMemory->beta[betaLocation] = thePM; }

      theMemory->last[betaLocation] = thePM;
     }
     
   theMemory->count++;
   join->memoryAdds++;
   
   thePM->owner = join;

   /*======================================*/
   /* Update the alpha memory linked list. */
   /*======================================*/
      
   if (rhsBinds != NULL)
     {
      thePM->nextRightChild = rhsBinds->children;
      if (rhsBinds->children != NULL)
        { rhsBinds->children->prevRightChild = thePM; }
      rhsBinds->children = thePM;
      thePM->rightParent = rhsBinds;
    }
      
   /*=====================================*/
   /* Update the beta memory linked list. */
   /*=====================================*/

   if (lhsBinds != NULL)
     {
      thePM->nextLeftChild = lhsBinds->children;
      if (lhsBinds->children != NULL)
        { lhsBinds->children->prevLeftChild = thePM; }
      lhsBinds->children = thePM;
      thePM->leftParent = lhsBinds;
     }

   if (! DefruleData(theEnv)->BetaMemoryResizingFlag)
     { return; }

   if ((theMemory->size > 1) &&
       (theMemory->count > (theMemory->size * 11)))
     { ResizeBetaMemory(theEnv,theMemory); }
  }

/**********************************************************/
/* AddBlockedLink: Adds a link between a partial match in */
/*   the beta memory of a join (with a negated RHS) and a */
/*   partial match in its right memory that prevents the  */
/*   partial match from being satisfied and propagated to */
/*   the next join in the rule.                           */
/**********************************************************/
globle void AddBlockedLink(
  struct partialMatch *thePM,
  struct partialMatch *rhsBinds)
  {
   thePM->marker = rhsBinds;
   thePM->nextBlocked = rhsBinds->blockList;
   if (rhsBinds->blockList != NULL)
     { rhsBinds->blockList->prevBlocked = thePM; }
   rhsBinds->blockList = thePM;
  }

/*************************************************************/
/* RemoveBlockedLink: Removes a link between a partial match */
/*   in the beta memory of a join (with a negated RHS) and a */
/*   partial match in its right memory that prevents the     */
/*   partial match from being satisfied and propagated to    */
/*   the next join in the rule.                              */
/*************************************************************/
globle void RemoveBlockedLink(
  struct partialMatch *thePM)
  {
   struct partialMatch *blocker;
   
   if (thePM->prevBlocked == NULL)
     { 
      blocker = (struct partialMatch *) thePM->marker;
      blocker->blockList = thePM->nextBlocked; 
     } 
   else
     { thePM->prevBlocked->nextBlocked = thePM->nextBlocked; }

   if (thePM->nextBlocked != NULL)
     { thePM->nextBlocked->prevBlocked = thePM->prevBlocked; }

   thePM->nextBlocked = NULL;
   thePM->prevBlocked = NULL;
   thePM->marker = NULL;
  }
         
/***********************************************************/
/* UnlinkBetaPMFromNodeAndLineage: . */
/***********************************************************/
globle void UnlinkBetaPMFromNodeAndLineage(
  void *theEnv,
  struct joinNode *join,
  struct partialMatch *thePM,
  int side)
  {
   unsigned long betaLocation;
   struct betaMemory *theMemory;

   if (side == LHS)
     { theMemory = join->leftMemory; }
   else
     { theMemory = join->rightMemory; }
   
   /*=============================================*/
   /* Update the nextInMemory/prevInMemory links. */
   /*=============================================*/
   
   theMemory->count--;
   join->memoryDeletes++;

   betaLocation = thePM->hashValue % theMemory->size;
   
   if ((side == RHS) &&
       (theMemory->last[betaLocation] == thePM))
     { theMemory->last[betaLocation] = thePM->prevInMemory; }
     
   if (thePM->prevInMemory == NULL)
     { 
      betaLocation = thePM->hashValue % theMemory->size;
      theMemory->beta[betaLocation] = thePM->nextInMemory; 
     }
   else
     { thePM->prevInMemory->nextInMemory = thePM->nextInMemory; }

   if (thePM->nextInMemory != NULL)
     { thePM->nextInMemory->prevInMemory = thePM->prevInMemory; }
     
   thePM->nextInMemory = NULL;
   thePM->prevInMemory = NULL;
   
   UnlinkBetaPartialMatchfromAlphaAndBetaLineage(thePM);

   if (! DefruleData(theEnv)->BetaMemoryResizingFlag)
     { return; }

   if ((theMemory->count == 0) && (theMemory->size > 1))
     { ResetBetaMemory(theEnv,theMemory); }
  } 

/***********************************************************/
/* UnlinkNonLeftLineage: . */
/***********************************************************/
globle void UnlinkNonLeftLineage(
  void *theEnv,
  struct joinNode *join,
  struct partialMatch *thePM,
  int side)
  {
   unsigned long betaLocation;
   struct betaMemory *theMemory;
   struct partialMatch *tempPM;

   if (side == LHS)
     { theMemory = join->leftMemory; }
   else
     { theMemory = join->rightMemory; }
   
   /*=============================================*/
   /* Update the nextInMemory/prevInMemory links. */
   /*=============================================*/
   
   theMemory->count--;
   join->memoryDeletes++;

   betaLocation = thePM->hashValue % theMemory->size;
   
   if ((side == RHS) &&
       (theMemory->last[betaLocation] == thePM))
     { theMemory->last[betaLocation] = thePM->prevInMemory; }
     
   if (thePM->prevInMemory == NULL)
     { 
      betaLocation = thePM->hashValue % theMemory->size;
      theMemory->beta[betaLocation] = thePM->nextInMemory; 
     }
   else
     { thePM->prevInMemory->nextInMemory = thePM->nextInMemory; }

   if (thePM->nextInMemory != NULL)
     { thePM->nextInMemory->prevInMemory = thePM->prevInMemory; }

   /*=========================*/
   /* Update the alpha lists. */
   /*=========================*/

   if (thePM->prevRightChild == NULL)
     { 
      if (thePM->rightParent != NULL)
        { 
         thePM->rightParent->children = thePM->nextRightChild; 
         if (thePM->nextRightChild != NULL)
           {
            thePM->rightParent->children = thePM->nextRightChild;
            thePM->nextRightChild->rightParent = thePM->rightParent;
           }
        } 
     }
   else
     { thePM->prevRightChild->nextRightChild = thePM->nextRightChild; }

   if (thePM->nextRightChild != NULL)
     { thePM->nextRightChild->prevRightChild = thePM->prevRightChild; }
   
   /*===========================*/
   /* Update the blocked lists. */
   /*===========================*/

   if (thePM->prevBlocked == NULL)
     { 
      tempPM = (struct partialMatch *) thePM->marker;
      
      if (tempPM != NULL)
        { tempPM->blockList = thePM->nextBlocked; } 
     }
   else
     { thePM->prevBlocked->nextBlocked = thePM->nextBlocked; }

   if (thePM->nextBlocked != NULL)
     { thePM->nextBlocked->prevBlocked = thePM->prevBlocked; }

   if (! DefruleData(theEnv)->BetaMemoryResizingFlag)
     { return; }

   if ((theMemory->count == 0) && (theMemory->size > 1))
     { ResetBetaMemory(theEnv,theMemory); }
  } 

/*******************************************************************/
/* UnlinkBetaPartialMatchfromAlphaAndBetaLineage: Removes the      */
/*   lineage links from a beta memory partial match. This removes  */
/*   the links between this partial match and its left and right   */
/*   memory parents. It also removes the links between this        */
/*   partial match and any of its children in other beta memories. */
/*******************************************************************/
static void UnlinkBetaPartialMatchfromAlphaAndBetaLineage(
  struct partialMatch *thePM)
  {
   struct partialMatch *tempPM;
   
   /*=========================*/
   /* Update the alpha lists. */
   /*=========================*/

   if (thePM->prevRightChild == NULL)
     { 
      if (thePM->rightParent != NULL)
        { thePM->rightParent->children = thePM->nextRightChild; } 
     }
   else
     { thePM->prevRightChild->nextRightChild = thePM->nextRightChild; }

   if (thePM->nextRightChild != NULL)
     { thePM->nextRightChild->prevRightChild = thePM->prevRightChild; }

   thePM->rightParent = NULL;
   thePM->nextRightChild = NULL;
   thePM->prevRightChild = NULL;
   
   /*========================*/
   /* Update the beta lists. */
   /*========================*/

   if (thePM->prevLeftChild == NULL)
     { 
      if (thePM->leftParent != NULL)
        { thePM->leftParent->children = thePM->nextLeftChild; } 
     }
   else
     { thePM->prevLeftChild->nextLeftChild = thePM->nextLeftChild; }

   if (thePM->nextLeftChild != NULL)
     { thePM->nextLeftChild->prevLeftChild = thePM->prevLeftChild; }

   thePM->leftParent = NULL;
   thePM->nextLeftChild = NULL;
   thePM->prevLeftChild = NULL;

   /*===========================*/
   /* Update the blocked lists. */
   /*===========================*/

   if (thePM->prevBlocked == NULL)
     { 
      tempPM = (struct partialMatch *) thePM->marker;
      
      if (tempPM != NULL)
        { tempPM->blockList = thePM->nextBlocked; } 
     }
   else
     { thePM->prevBlocked->nextBlocked = thePM->nextBlocked; }

   if (thePM->nextBlocked != NULL)
     { thePM->nextBlocked->prevBlocked = thePM->prevBlocked; }

   thePM->marker = NULL;
   thePM->nextBlocked = NULL;
   thePM->prevBlocked = NULL;
      
   /*===============================================*/
   /* Remove parent reference from the child links. */
   /*===============================================*/
   
   if (thePM->children != NULL)
     {
      if (thePM->betaMemory)
        {
         for (tempPM = thePM->children; tempPM != NULL; tempPM = tempPM->nextLeftChild)
           { tempPM->leftParent = NULL; }
        }
      else
        {
         for (tempPM = thePM->children; tempPM != NULL; tempPM = tempPM->nextRightChild)
           { tempPM->rightParent = NULL; }
        }
        
      thePM->children = NULL;
     }
  } 
  
/********************************************************/
/* MergePartialMatches: Merges two partial matches. The */
/*   second match should either be NULL (indicating a   */
/*   negated CE) or contain a single match.             */
/********************************************************/
globle struct partialMatch *MergePartialMatches(
  void *theEnv,
  struct partialMatch *lhsBind,
  struct partialMatch *rhsBind)
  {
   struct partialMatch *linker;
   static struct partialMatch mergeTemplate = { 1 }; /* betaMemory is TRUE, remainder are 0 or NULL */
  
   /*=================================*/
   /* Allocate the new partial match. */
   /*=================================*/
   
   linker = get_var_struct(theEnv,partialMatch,sizeof(struct genericMatch) * lhsBind->bcount);

   /*============================================*/
   /* Set the flags to their appropriate values. */
   /*============================================*/
   
   memcpy(linker,&mergeTemplate,sizeof(struct partialMatch) - sizeof(struct genericMatch));
   
   linker->bcount = (unsigned short) (lhsBind->bcount + 1);
   
   /*========================================================*/
   /* Copy the bindings of the partial match being extended. */
   /*========================================================*/
      
   memcpy(linker->binds,lhsBind->binds,sizeof(struct genericMatch) * lhsBind->bcount);

   /*===================================*/
   /* Add the binding of the rhs match. */
   /*===================================*/
 
   if (rhsBind == NULL)
     { linker->binds[lhsBind->bcount].gm.theValue = NULL; }
   else
     { linker->binds[lhsBind->bcount].gm.theValue = rhsBind->binds[0].gm.theValue; }

   return(linker);
  }

/*******************************************************************/
/* InitializePatternHeader: Initializes a pattern header structure */
/*   (used by the fact and instance pattern matchers).             */
/*******************************************************************/
globle void InitializePatternHeader(
  void *theEnv,
  struct patternNodeHeader *theHeader)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
   theHeader->firstHash = NULL;
   theHeader->lastHash = NULL;
   theHeader->entryJoin = NULL;
   theHeader->rightHash = NULL;
   theHeader->singlefieldNode = FALSE;
   theHeader->multifieldNode = FALSE;
   theHeader->stopNode = FALSE;
#if (! RUN_TIME)
   theHeader->initialize = EnvGetIncrementalReset(theEnv);
#else
   theHeader->initialize = FALSE;
#endif
   theHeader->marked = FALSE;
   theHeader->beginSlot = FALSE;
   theHeader->endSlot = FALSE;
   theHeader->selector = FALSE;
  }

/******************************************************************/
/* CreateAlphaMatch: Given a pointer to an entity (such as a fact */
/*   or instance) which matched a pattern, this function creates  */
/*   a partial match suitable for storing in the alpha memory of  */
/*   the pattern network. Note that the multifield markers which  */
/*   are passed as a calling argument are copied (thus the caller */
/*   is still responsible for freeing these data structures).     */
/******************************************************************/
globle struct partialMatch *CreateAlphaMatch(
  void *theEnv,
  void *theEntity,
  struct multifieldMarker *markers,
  struct patternNodeHeader *theHeader,
  unsigned long hashOffset)
  {
   struct partialMatch *theMatch;
   struct alphaMatch *afbtemp;
   unsigned long hashValue;
   struct alphaMemoryHash *theAlphaMemory;

   /*==================================================*/
   /* Create the alpha match and intialize its values. */
   /*==================================================*/

   theMatch = get_struct(theEnv,partialMatch);
   InitializePMLinks(theMatch);
   theMatch->betaMemory = FALSE;
   theMatch->busy = FALSE;
   theMatch->bcount = 1;
   theMatch->hashValue = hashOffset;

   afbtemp = get_struct(theEnv,alphaMatch);
   afbtemp->next = NULL;
   afbtemp->matchingItem = (struct patternEntity *) theEntity;

   if (markers != NULL)
     { afbtemp->markers = CopyMultifieldMarkers(theEnv,markers); }
   else
     { afbtemp->markers = NULL; }

   theMatch->binds[0].gm.theMatch = afbtemp;

   /*============================================*/
   /* Find the alpha memory of the pattern node. */
   /*============================================*/

   hashValue = AlphaMemoryHashValue(theHeader,hashOffset);
   theAlphaMemory = FindAlphaMemory(theEnv,theHeader,hashValue);
   afbtemp->bucket = hashValue;

   /*============================================*/
   /* Create an alpha memory if it wasn't found. */
   /*============================================*/

   if (theAlphaMemory == NULL)
     {
      theAlphaMemory = get_struct(theEnv,alphaMemoryHash);
      theAlphaMemory->bucket = hashValue;
      theAlphaMemory->owner = theHeader;
      theAlphaMemory->alphaMemory = NULL;
      theAlphaMemory->endOfQueue = NULL;
      theAlphaMemory->nextHash = NULL;

      theAlphaMemory->next = DefruleData(theEnv)->AlphaMemoryTable[hashValue];
      if (theAlphaMemory->next != NULL)
        { theAlphaMemory->next->prev = theAlphaMemory; }

      theAlphaMemory->prev = NULL; 
      DefruleData(theEnv)->AlphaMemoryTable[hashValue] = theAlphaMemory;
      
      if (theHeader->firstHash == NULL)
        {
         theHeader->firstHash = theAlphaMemory;
         theHeader->lastHash = theAlphaMemory;
         theAlphaMemory->prevHash = NULL;
        }
      else
        {
         theHeader->lastHash->nextHash = theAlphaMemory;
         theAlphaMemory->prevHash = theHeader->lastHash;
         theHeader->lastHash = theAlphaMemory;
        }
     }

   /*====================================*/
   /* Store the alpha match in the alpha */
   /* memory of the pattern node.        */
   /*====================================*/
 
    theMatch->prevInMemory = theAlphaMemory->endOfQueue;
    if (theAlphaMemory->endOfQueue == NULL)
     {
      theAlphaMemory->alphaMemory = theMatch;
      theAlphaMemory->endOfQueue = theMatch;
     }
   else
     {
      theAlphaMemory->endOfQueue->nextInMemory = theMatch;
      theAlphaMemory->endOfQueue = theMatch;
     }

   /*===================================================*/
   /* Return a pointer to the newly create alpha match. */
   /*===================================================*/

   return(theMatch);
  }
  
/*******************************************/
/* CopyMultifieldMarkers: Copies a list of */
/*   multifieldMarker data structures.     */
/*******************************************/
struct multifieldMarker *CopyMultifieldMarkers(
  void *theEnv,
  struct multifieldMarker *theMarkers)
  {
   struct multifieldMarker *head = NULL, *lastMark = NULL, *newMark;

   while (theMarkers != NULL)
     {
      newMark = get_struct(theEnv,multifieldMarker);
      newMark->next = NULL;
      newMark->whichField = theMarkers->whichField;
      newMark->where = theMarkers->where;
      newMark->startPosition = theMarkers->startPosition;
      newMark->endPosition = theMarkers->endPosition;

      if (lastMark == NULL)
        { head = newMark; }
      else
        { lastMark->next = newMark; }
      lastMark = newMark;

      theMarkers = theMarkers->next;
     }

   return(head);
  }

/***************************************************************/
/* FlushAlphaBetaMemory: Returns all partial matches in a list */
/*   of partial matches either directly to the pool of free    */
/*   memory or to the list of GarbagePartialMatches. Partial   */
/*   matches stored in alpha memories must be placed on the    */
/*   list of GarbagePartialMatches.                            */
/***************************************************************/
globle void FlushAlphaBetaMemory(
  void *theEnv,
  struct partialMatch *pfl)
  {
   struct partialMatch *pfltemp;

   while (pfl != NULL)
     {
      pfltemp = pfl->nextInMemory;

      UnlinkBetaPartialMatchfromAlphaAndBetaLineage(pfl);
      ReturnPartialMatch(theEnv,pfl);

      pfl = pfltemp;
     }
  }

/*****************************************************************/
/* DestroyAlphaBetaMemory: Returns all partial matches in a list */
/*   of partial matches directly to the pool of free memory.     */
/*****************************************************************/
globle void DestroyAlphaBetaMemory(
  void *theEnv,
  struct partialMatch *pfl)
  {
   struct partialMatch *pfltemp;

   while (pfl != NULL)
     {
      pfltemp = pfl->nextInMemory;
      DestroyPartialMatch(theEnv,pfl); 
      pfl = pfltemp;
     }
  }

/******************************************************/
/* FindEntityInPartialMatch: Searches for a specified */
/*   data entity in a partial match.                  */
/******************************************************/
globle int FindEntityInPartialMatch(
  struct patternEntity *theEntity,
  struct partialMatch *thePartialMatch)
  {
   unsigned short i;

   for (i = 0 ; i < thePartialMatch->bcount; i++)
     {
      if (thePartialMatch->binds[i].gm.theMatch == NULL) continue;
      if (thePartialMatch->binds[i].gm.theMatch->matchingItem == theEntity)
        { return(TRUE); }
     }

   return(FALSE);
  }
  
/***********************************************************************/
/* GetPatternNumberFromJoin: Given a pointer to a join associated with */
/*   a pattern CE, returns an integer representing the position of the */
/*   pattern CE in the rule (e.g. first, second, third).               */
/***********************************************************************/
globle int GetPatternNumberFromJoin(
  struct joinNode *joinPtr)
  {
   int whichOne = 0;

   while (joinPtr != NULL)
     {
      if (joinPtr->joinFromTheRight)
        { joinPtr = (struct joinNode *) joinPtr->rightSideEntryStructure; }
      else
        {
         whichOne++;
         joinPtr = joinPtr->lastLevel;
        }
     }

   return(whichOne);
  }

/************************************************************************/
/* TraceErrorToRule: Prints an error message when a error occurs as the */
/*   result of evaluating an expression in the pattern network. Used to */
/*   indicate which rule caused the problem.                            */
/************************************************************************/
globle void TraceErrorToRule(
  void *theEnv,
  struct joinNode *joinPtr,
  char *indentSpaces)
  {
   int patternCount;
   
   MarkRuleNetwork(theEnv,0);

   patternCount = CountPriorPatterns(joinPtr->lastLevel) + 1;

   TraceErrorToRuleDriver(theEnv,joinPtr,indentSpaces,patternCount,FALSE);

   MarkRuleNetwork(theEnv,0);
  }

/**************************************************************/
/* TraceErrorToRuleDriver: Driver code for printing out which */
/*   rule caused a pattern or join network error.             */
/**************************************************************/
static void TraceErrorToRuleDriver(
  void *theEnv,
  struct joinNode *joinPtr,
  char *indentSpaces,
  int priorRightJoinPatterns,
  int enteredJoinFromRight)
  {
   char *name;
   int priorPatternCount;
   struct joinLink *theLinks;

   if ((joinPtr->joinFromTheRight) && enteredJoinFromRight)
     { priorPatternCount = CountPriorPatterns(joinPtr->lastLevel); }
   else
     { priorPatternCount = 0; }
        
   if (joinPtr->marked)
     { /* Do Nothing */ }
   else if (joinPtr->ruleToActivate != NULL)
     {
      joinPtr->marked = 1;
      name = EnvGetDefruleName(theEnv,joinPtr->ruleToActivate);
      EnvPrintRouter(theEnv,WERROR,indentSpaces);

      EnvPrintRouter(theEnv,WERROR,"Of pattern #");
      PrintLongInteger(theEnv,WERROR,priorRightJoinPatterns+priorPatternCount);
      EnvPrintRouter(theEnv,WERROR," in rule ");
      EnvPrintRouter(theEnv,WERROR,name);
      EnvPrintRouter(theEnv,WERROR,"\n");
     }
   else
     {
      joinPtr->marked = 1;
        
      theLinks = joinPtr->nextLinks;
      while (theLinks != NULL)
        {
         TraceErrorToRuleDriver(theEnv,theLinks->join,indentSpaces,
                                priorRightJoinPatterns+priorPatternCount,
                                (theLinks->enterDirection == RHS));
         theLinks = theLinks->next;
        }
     }
  }

/**************************************************************/
/* CountPriorPatterns:            */
/**************************************************************/
static int CountPriorPatterns(
  struct joinNode *joinPtr)
  {
   int count = 0;
   
   while (joinPtr != NULL)
     {
      if (joinPtr->joinFromTheRight)
        { count += CountPriorPatterns((struct joinNode *) joinPtr->rightSideEntryStructure); }
      else
        { count++; }
        
      joinPtr = joinPtr->lastLevel;
     }
     
   return(count);
  }

/********************************************************/
/* MarkRuleNetwork: Sets the marked flag in each of the */
/*   joins in the join network to the specified value.  */
/********************************************************/
globle void MarkRuleNetwork(
  void *theEnv,
  int value)
  {
   struct defrule *rulePtr;
   struct joinNode *joinPtr;
   struct defmodule *modulePtr;

   /*===========================*/
   /* Loop through each module. */
   /*===========================*/

   SaveCurrentModule(theEnv);
   for (modulePtr = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        modulePtr != NULL;
        modulePtr = (struct defmodule *) EnvGetNextDefmodule(theEnv,modulePtr))
     {
      EnvSetCurrentModule(theEnv,(void *) modulePtr);

      /*=========================*/
      /* Loop through each rule. */
      /*=========================*/

      rulePtr = (struct defrule *) EnvGetNextDefrule(theEnv,NULL);
      while (rulePtr != NULL)
        {
         /*=============================*/
         /* Mark each join for the rule */
         /* with the specified value.   */
         /*=============================*/

         joinPtr = rulePtr->lastJoin;
         MarkRuleJoins(joinPtr,value);

         /*=================================*/
         /* Move on to the next rule or the */
         /* next disjunct for this rule.    */
         /*=================================*/

         if (rulePtr->disjunct != NULL) rulePtr = rulePtr->disjunct;
         else rulePtr = (struct defrule *) EnvGetNextDefrule(theEnv,rulePtr);
        }

     }

   RestoreCurrentModule(theEnv);
  }

/*****************************************/
/* MarkRuleJoins:      */
/*****************************************/ 
globle void MarkRuleJoins(
  struct joinNode *joinPtr,
  int value)
  {
   while (joinPtr != NULL)
     {
      if (joinPtr->joinFromTheRight)
        { MarkRuleJoins((struct joinNode *) joinPtr->rightSideEntryStructure,value); }
        
      joinPtr->marked = value;
      joinPtr = joinPtr->lastLevel;
     }
  }

/*****************************************/
/* GetAlphaMemory: Retrieves the list of */
/*   matches from an alpha memory.       */
/*****************************************/ 
globle struct partialMatch *GetAlphaMemory(
  void *theEnv,
  struct patternNodeHeader *theHeader,
  unsigned long hashOffset)
  {
   struct alphaMemoryHash *theAlphaMemory;
   unsigned long hashValue;

   hashValue = AlphaMemoryHashValue(theHeader,hashOffset);
   theAlphaMemory = FindAlphaMemory(theEnv,theHeader,hashValue);

   if (theAlphaMemory == NULL)
     { return NULL; }

   return theAlphaMemory->alphaMemory;
  }

/*****************************************/
/* GetLeftBetaMemory: Retrieves the list */
/*   of matches from a beta memory.      */
/*****************************************/ 
globle struct partialMatch *GetLeftBetaMemory(
  struct joinNode *theJoin,
  unsigned long hashValue)
  {
   unsigned long betaLocation;
   
   betaLocation = hashValue % theJoin->leftMemory->size;

   return theJoin->leftMemory->beta[betaLocation];
  }

/******************************************/
/* GetRightBetaMemory: Retrieves the list */
/*   of matches from a beta memory.       */
/******************************************/ 
globle struct partialMatch *GetRightBetaMemory(
  struct joinNode *theJoin,
  unsigned long hashValue)
  {
   unsigned long betaLocation;
   
   betaLocation = hashValue % theJoin->rightMemory->size;

   return theJoin->rightMemory->beta[betaLocation];
  }
    
/***************************************/
/* ReturnLeftMemory: Sets the contents */
/*   of a beta memory to NULL.         */
/***************************************/ 
globle void ReturnLeftMemory(
  void *theEnv,
  struct joinNode *theJoin)
  {
   if (theJoin->leftMemory == NULL) return;
   genfree(theEnv,theJoin->leftMemory->beta,sizeof(struct partialMatch *) * theJoin->leftMemory->size);
   rtn_struct(theEnv,betaMemory,theJoin->leftMemory);
   theJoin->leftMemory = NULL;
  }

/***************************************/
/* ReturnRightMemory: Sets the contents */
/*   of a beta memory to NULL.         */
/***************************************/ 
globle void ReturnRightMemory(
  void *theEnv,
  struct joinNode *theJoin)
  {
   if (theJoin->rightMemory == NULL) return;
   genfree(theEnv,theJoin->rightMemory->beta,sizeof(struct partialMatch *) * theJoin->rightMemory->size);
   genfree(theEnv,theJoin->rightMemory->last,sizeof(struct partialMatch *) * theJoin->rightMemory->size);
   rtn_struct(theEnv,betaMemory,theJoin->rightMemory);
   theJoin->rightMemory = NULL;
  }
  
/****************************************************************/
/* DestroyBetaMemory: Destroys the contents of a beta memory in */
/*   preperation for the deallocation of a join. Destroying is  */
/*   performed when the environment is being deallocated and it */
/*   is not necessary to leave the environment in a consistent  */
/*   state (as it would be if just a single rule were being     */
/*   deleted).                                                  */
/****************************************************************/
globle void DestroyBetaMemory(
  void *theEnv,
  struct joinNode *theJoin,
  int side)
  {  
   unsigned long i;
       
   if (side == LHS)
     {
      if (theJoin->leftMemory == NULL) return;
   
      for (i = 0; i < theJoin->leftMemory->size; i++)
        { DestroyAlphaBetaMemory(theEnv,theJoin->leftMemory->beta[i]); }
     }
   else
     {
      if (theJoin->rightMemory == NULL) return;
   
      for (i = 0; i < theJoin->rightMemory->size; i++)
        { DestroyAlphaBetaMemory(theEnv,theJoin->rightMemory->beta[i]); }
     }
  }
    
/*************************************************************/
/* FlushBetaMemory: Flushes the contents of a beta memory in */
/*   preperation for the deallocation of a join. Flushing    */
/*   is performed when the partial matches in the beta       */
/*   memory may still be in use because the environment will */
/*   remain active.                                          */
/*************************************************************/
globle void FlushBetaMemory(
  void *theEnv,
  struct joinNode *theJoin,
  int side)
  {
   unsigned long i;
   
   if (side == LHS)
     {
      if (theJoin->leftMemory == NULL) return;

      for (i = 0; i < theJoin->leftMemory->size; i++)
        { FlushAlphaBetaMemory(theEnv,theJoin->leftMemory->beta[i]); }
     }
   else
     {
      if (theJoin->rightMemory == NULL) return;

      for (i = 0; i < theJoin->rightMemory->size; i++)
        { FlushAlphaBetaMemory(theEnv,theJoin->rightMemory->beta[i]); }
     }
 }
  
/*****************************************************************/
/* BetaMemoryNotEmpty:  */
/*****************************************************************/
globle intBool BetaMemoryNotEmpty(
  struct joinNode *theJoin)
  {
   if (theJoin->leftMemory != NULL)
     {
      if (theJoin->leftMemory->count > 0)
        { return(TRUE); }
     }
     
   if (theJoin->rightMemory != NULL)
     {
      if (theJoin->rightMemory->count > 0)
        { return(TRUE); }
     }
     
   return(FALSE);
  }
    
/*********************************************/
/* RemoveAlphaMemoryMatches: Removes matches */
/*   from an alpha memory.                   */
/*********************************************/ 
globle void RemoveAlphaMemoryMatches(
  void *theEnv,
  struct patternNodeHeader *theHeader,
  struct partialMatch *theMatch,
  struct alphaMatch *theAlphaMatch)
  {
   struct alphaMemoryHash *theAlphaMemory = NULL;
   unsigned long hashValue;

   if ((theMatch->prevInMemory == NULL) || (theMatch->nextInMemory == NULL))
     {
      hashValue = theAlphaMatch->bucket;
      theAlphaMemory = FindAlphaMemory(theEnv,theHeader,hashValue);
     }
     
   if (theMatch->prevInMemory != NULL)
     { theMatch->prevInMemory->nextInMemory = theMatch->nextInMemory; }
   else
     { theAlphaMemory->alphaMemory = theMatch->nextInMemory; }

   if (theMatch->nextInMemory != NULL)
     { theMatch->nextInMemory->prevInMemory = theMatch->prevInMemory; }
   else
     { theAlphaMemory->endOfQueue = theMatch->prevInMemory; }

   /*====================================*/
   /* Add the match to the garbage list. */
   /*====================================*/

   theMatch->nextInMemory = EngineData(theEnv)->GarbagePartialMatches;
   EngineData(theEnv)->GarbagePartialMatches = theMatch;

   if ((theAlphaMemory != NULL) && (theAlphaMemory->alphaMemory == NULL))
     { UnlinkAlphaMemory(theEnv,theHeader,theAlphaMemory); }
  }

/*****************************************************************/
/* DestroyAlphaMemory: */
/*****************************************************************/
globle void DestroyAlphaMemory(
  void *theEnv,
  struct patternNodeHeader *theHeader,
  int unlink)
  {
   struct alphaMemoryHash *theAlphaMemory, *tempMemory;

   theAlphaMemory = theHeader->firstHash;

   while (theAlphaMemory != NULL)
     {
      tempMemory = theAlphaMemory->nextHash;
      DestroyAlphaBetaMemory(theEnv,theAlphaMemory->alphaMemory); 
      if (unlink)
        { UnlinkAlphaMemoryBucketSiblings(theEnv,theAlphaMemory); }
      rtn_struct(theEnv,alphaMemoryHash,theAlphaMemory);
      theAlphaMemory = tempMemory;
     }

   theHeader->firstHash = NULL;
   theHeader->lastHash = NULL;
  }

/*****************************************************************/
/* FlushAlphaMemory:  */
/*****************************************************************/
globle void FlushAlphaMemory(
  void *theEnv,
  struct patternNodeHeader *theHeader)
  {
   struct alphaMemoryHash *theAlphaMemory, *tempMemory;

   theAlphaMemory = theHeader->firstHash;

   while (theAlphaMemory != NULL)
     {
      tempMemory = theAlphaMemory->nextHash;
      FlushAlphaBetaMemory(theEnv,theAlphaMemory->alphaMemory); 
      UnlinkAlphaMemoryBucketSiblings(theEnv,theAlphaMemory);
      rtn_struct(theEnv,alphaMemoryHash,theAlphaMemory);
      theAlphaMemory = tempMemory;
     }

   theHeader->firstHash = NULL;
   theHeader->lastHash = NULL;
  }

/*****************************************************************/
/* FindAlphaMemory:  */
/*****************************************************************/
static struct alphaMemoryHash *FindAlphaMemory(
  void *theEnv,
  struct patternNodeHeader *theHeader,
  unsigned long hashValue)
  {
   struct alphaMemoryHash *theAlphaMemory;
      
   theAlphaMemory = DefruleData(theEnv)->AlphaMemoryTable[hashValue];

   if (theAlphaMemory != NULL)
     {
      while ((theAlphaMemory != NULL) && (theAlphaMemory->owner != theHeader))
        { theAlphaMemory = theAlphaMemory->next; }
     }
     
   return theAlphaMemory;
  }   

/*****************************************************************/
/* AlphaMemoryHashValue:  */
/*****************************************************************/
static unsigned long AlphaMemoryHashValue(
  struct patternNodeHeader *theHeader,
  unsigned long hashOffset)
  {
   unsigned long hashValue;
   union
     {
      void *vv;
      unsigned uv;
     } fis;
        
   fis.uv = 0;
   fis.vv = theHeader;
   
   hashValue = fis.uv + hashOffset;
   hashValue = hashValue % ALPHA_MEMORY_HASH_SIZE;
   
   return hashValue;
  }
  
/*****************************************************************/
/* UnlinkAlphaMemory:  */
/*****************************************************************/
static void UnlinkAlphaMemory(
  void *theEnv,
  struct patternNodeHeader *theHeader,
  struct alphaMemoryHash *theAlphaMemory)
  {
   /*======================*/
   /* Unlink the siblings. */
   /*======================*/
    
   UnlinkAlphaMemoryBucketSiblings(theEnv,theAlphaMemory);
      
   /*================================*/
   /* Update firstHash and lastHash. */
   /*================================*/
   
   if (theAlphaMemory == theHeader->firstHash)
     { theHeader->firstHash = theAlphaMemory->nextHash; }
     
   if (theAlphaMemory == theHeader->lastHash)
     { theHeader->lastHash = theAlphaMemory->prevHash; }
        
   /*===============================*/
   /* Update nextHash and prevHash. */
   /*===============================*/

   if (theAlphaMemory->prevHash != NULL)
     { theAlphaMemory->prevHash->nextHash = theAlphaMemory->nextHash; }
     
   if (theAlphaMemory->nextHash != NULL)
     { theAlphaMemory->nextHash->prevHash = theAlphaMemory->prevHash; }
              
   rtn_struct(theEnv,alphaMemoryHash,theAlphaMemory);
  }   

/*****************************************************************/
/* UnlinkAlphaMemoryBucketSiblings:  */
/*****************************************************************/
static void UnlinkAlphaMemoryBucketSiblings(
  void *theEnv,
  struct alphaMemoryHash *theAlphaMemory)
  {
   if (theAlphaMemory->prev == NULL)
     { DefruleData(theEnv)->AlphaMemoryTable[theAlphaMemory->bucket] = theAlphaMemory->next; }
   else
     { theAlphaMemory->prev->next = theAlphaMemory->next; }
        
   if (theAlphaMemory->next != NULL)
     { theAlphaMemory->next->prev = theAlphaMemory->prev; }
  }   

/********************************************/
/* ComputeRightHashValue:       */
/********************************************/ 
unsigned long ComputeRightHashValue(
  void *theEnv,
  struct patternNodeHeader *theHeader)
  {
   struct expr *tempExpr;
   unsigned long hashValue = 0;
   unsigned long multiplier = 1;
      
   if (theHeader->rightHash == NULL)
     { return hashValue; }
     
   for (tempExpr = theHeader->rightHash; 
        tempExpr != NULL; 
        tempExpr = tempExpr->nextArg, multiplier = multiplier * 509)
      {
       DATA_OBJECT theResult;
       struct expr *oldArgument;
        
       oldArgument = EvaluationData(theEnv)->CurrentExpression;
       EvaluationData(theEnv)->CurrentExpression = tempExpr;
       (*EvaluationData(theEnv)->PrimitivesArray[tempExpr->type]->evaluateFunction)(theEnv,tempExpr->value,&theResult);
       EvaluationData(theEnv)->CurrentExpression = oldArgument;
        
       switch (theResult.type)
         {
          case STRING:
          case SYMBOL:
          case INSTANCE_NAME:
            hashValue += (((SYMBOL_HN *) theResult.value)->bucket * multiplier);
            break;
             
          case INTEGER:
            hashValue += (((INTEGER_HN *) theResult.value)->bucket * multiplier);
            break;
             
          case FLOAT:
            hashValue += (((FLOAT_HN *) theResult.value)->bucket * multiplier);
            break;
          }
       }
       
     return hashValue;
    }

/***********************************************************/
/* ResizeBetaMemory:                                */
/***********************************************************/
globle void ResizeBetaMemory(
  void *theEnv,
  struct betaMemory *theMemory)
  {
   struct partialMatch **oldArray, **lastAdd, *thePM, *nextPM;
   unsigned long i, oldSize, betaLocation;
   
   oldSize = theMemory->size;
   oldArray = theMemory->beta;
   
   theMemory->size = oldSize * 11;
   theMemory->beta = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *) * theMemory->size);
     
   lastAdd = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *) * theMemory->size);
   memset(theMemory->beta,0,sizeof(struct partialMatch *) * theMemory->size);
   memset(lastAdd,0,sizeof(struct partialMatch *) * theMemory->size);
   
   for (i = 0; i < oldSize; i++)
     {
      thePM = oldArray[i];
      while (thePM != NULL)
        {
         nextPM = thePM->nextInMemory;
         
         thePM->nextInMemory = NULL;
         
         betaLocation = thePM->hashValue % theMemory->size;
         thePM->prevInMemory = lastAdd[betaLocation];
         
         if (lastAdd[betaLocation] != NULL)
           { lastAdd[betaLocation]->nextInMemory = thePM; }
         else
           { theMemory->beta[betaLocation] = thePM; }
           
         lastAdd[betaLocation] = thePM;
         
         thePM = nextPM;
        } 
     }
  
   if (theMemory->last != NULL)
     { 
      genfree(theEnv,theMemory->last,sizeof(struct partialMatch *) * oldSize);
      theMemory->last = lastAdd;
     }
   else
     { genfree(theEnv,lastAdd,sizeof(struct partialMatch *) * theMemory->size); }
     
   genfree(theEnv,oldArray,sizeof(struct partialMatch *) * oldSize);
  }

/***********************************************************/
/* ResetBetaMemory:                                */
/***********************************************************/
static void ResetBetaMemory(
  void *theEnv,
  struct betaMemory *theMemory)
  {
   struct partialMatch **oldArray, **lastAdd;
   unsigned long oldSize;

   if ((theMemory->size == 1) ||
       (theMemory->size == INITIAL_BETA_HASH_SIZE))
     { return; }

   oldSize = theMemory->size;
   oldArray = theMemory->beta;
   
   theMemory->size = INITIAL_BETA_HASH_SIZE;
   theMemory->beta = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *) * theMemory->size);
   memset(theMemory->beta,0,sizeof(struct partialMatch *) * theMemory->size);
   genfree(theEnv,oldArray,sizeof(struct partialMatch *) * oldSize);
     
   if (theMemory->last != NULL)
     {
      lastAdd = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *) * theMemory->size);
      memset(lastAdd,0,sizeof(struct partialMatch *) * theMemory->size);
      genfree(theEnv,theMemory->last,sizeof(struct partialMatch *) * oldSize);
      theMemory->last = lastAdd;
     }
  }
  
#if (CONSTRUCT_COMPILER || BLOAD_AND_BSAVE) && (! RUN_TIME)

/*************************************************************/
/* TagRuleNetwork: Assigns each join in the join network and */
/*   each defrule data structure with a unique integer ID.   */
/*   Also counts the number of defrule and joinNode data     */
/*   structures currently in use.                            */
/*************************************************************/
globle void TagRuleNetwork(
  void *theEnv,
  long int *moduleCount,
  long int *ruleCount,
  long int *joinCount,
  long int *linkCount)
  {
   struct defmodule *modulePtr;
   struct defrule *rulePtr;
   struct joinLink *theLink;

   *moduleCount = 0;
   *ruleCount = 0;
   *joinCount = 0;
   *linkCount = 0;

   MarkRuleNetwork(theEnv,0);

   for (theLink = DefruleData(theEnv)->LeftPrimeJoins;
        theLink != NULL;
        theLink = theLink->next)
     { 
      theLink->bsaveID = *linkCount;
      (*linkCount)++; 
     }

   for (theLink = DefruleData(theEnv)->RightPrimeJoins;
        theLink != NULL;
        theLink = theLink->next)
     {
      theLink->bsaveID = *linkCount;
      (*linkCount)++;
     }
     
   /*===========================*/
   /* Loop through each module. */
   /*===========================*/

   for (modulePtr = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        modulePtr != NULL;
        modulePtr = (struct defmodule *) EnvGetNextDefmodule(theEnv,modulePtr))
     {
      (*moduleCount)++;
      EnvSetCurrentModule(theEnv,(void *) modulePtr);

      /*=========================*/
      /* Loop through each rule. */
      /*=========================*/

      rulePtr = (struct defrule *) EnvGetNextDefrule(theEnv,NULL);

      while (rulePtr != NULL)
        {
         rulePtr->header.bsaveID = *ruleCount;
         (*ruleCount)++;

         /*=========================*/
         /* Loop through each join. */
         /*=========================*/
        
         TagNetworkTraverseJoins(theEnv,joinCount,linkCount,rulePtr->lastJoin);

         if (rulePtr->disjunct != NULL) rulePtr = rulePtr->disjunct;
         else rulePtr = (struct defrule *) EnvGetNextDefrule(theEnv,rulePtr);
        }
     }
  }

/*******************************************************************/
/* TagNetworkTraverseJoins: Traverses the join network for a rule. */
/*******************************************************************/
static void TagNetworkTraverseJoins(
  void *theEnv,
  long int *joinCount,
  long int *linkCount,
  struct joinNode *joinPtr)
  {
   struct joinLink *theLink;
   for (;
        joinPtr != NULL;
        joinPtr = joinPtr->lastLevel)
     { 
      if (joinPtr->marked == 0)
        {
         joinPtr->marked = 1;
         joinPtr->bsaveID = *joinCount;
         (*joinCount)++;
         for (theLink = joinPtr->nextLinks;
              theLink != NULL;
              theLink = theLink->next)
           { 
            theLink->bsaveID = *linkCount;
            (*linkCount)++; 
           }
        }
      
      if (joinPtr->joinFromTheRight)
        { TagNetworkTraverseJoins(theEnv,joinCount,linkCount,(struct joinNode *) joinPtr->rightSideEntryStructure); }
     }
  }

#endif /* (CONSTRUCT_COMPILER || BLOAD_AND_BSAVE) && (! RUN_TIME) */

#endif /* DEFRULE_CONSTRUCT */





