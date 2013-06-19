   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                  MATCH HEADER FILE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
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

#ifndef _H_match

#define _H_match

struct genericMatch;
struct patternMatch;
struct partialMatch;
struct alphaMatch;
struct multifieldMarker;

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_network
#include "network.h"
#endif
#ifndef _H_pattern
#include "pattern.h"
#endif

/************************************************************/
/* PATTERNMATCH STRUCTURE:                                  */
/************************************************************/
struct patternMatch
  {
   struct patternMatch *next;
   struct partialMatch *theMatch;
   struct patternNodeHeader *matchingPattern;
  };

/**************************/
/* genericMatch structure */
/**************************/
struct genericMatch
  {
   union
     {
      void *theValue;
      struct alphaMatch *theMatch;
     } gm;
  };

/************************************************************/
/* PARTIALMATCH STRUCTURE:                                  */
/************************************************************/
struct partialMatch
  {
   unsigned int betaMemory  :  1;
   unsigned int busy        :  1;
   unsigned int rhsMemory   :  1;
   unsigned short bcount; 
   unsigned long hashValue;
   void *owner;
   void *marker;
   void *dependents;
   struct partialMatch *nextInMemory;
   struct partialMatch *prevInMemory;
   struct partialMatch *children;
   struct partialMatch *rightParent;
   struct partialMatch *nextRightChild;
   struct partialMatch *prevRightChild;
   struct partialMatch *leftParent;
   struct partialMatch *nextLeftChild;
   struct partialMatch *prevLeftChild;
   struct partialMatch *blockList;
   struct partialMatch *nextBlocked;
   struct partialMatch *prevBlocked;
   struct genericMatch binds[1];
  };

/************************************************************/
/* ALPHAMATCH STRUCTURE:                                    */
/************************************************************/
struct alphaMatch
  {
   struct patternEntity *matchingItem;
   struct multifieldMarker *markers;
   struct alphaMatch *next;
   unsigned long bucket;
  };

/************************************************************/
/* MULTIFIELDMARKER STRUCTURE: Used in the pattern matching */
/* process to mark the range of fields that the $? and      */
/* $?variables match because a single pattern restriction   */
/* may span zero or more fields..                           */
/************************************************************/
struct multifieldMarker
  {
   int whichField;
   union
     {
      void *whichSlot;
      short int whichSlotNumber;
     } where;
    long startPosition;
    long endPosition;
    struct multifieldMarker *next;
   };

#define get_nth_pm_value(thePM,thePos) (thePM->binds[thePos].gm.theValue)
#define get_nth_pm_match(thePM,thePos) (thePM->binds[thePos].gm.theMatch)

#define set_nth_pm_value(thePM,thePos,theVal) (thePM->binds[thePos].gm.theValue = (void *) theVal)
#define set_nth_pm_match(thePM,thePos,theVal) (thePM->binds[thePos].gm.theMatch = theVal)

#endif






