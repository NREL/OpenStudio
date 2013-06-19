   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                 RULE PATTERN MODULE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides the mechanism for recognizing and       */
/*   parsing the various types of patterns that can be used  */
/*   in the LHS of a rule. In version 6.0, the only pattern  */
/*   types provided are for deftemplate and instance         */
/*   patterns.                                               */
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
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*************************************************************/

#define _PATTERN_SOURCE_

#include "setup.h"

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>

#if DEFRULE_CONSTRUCT

#include "constant.h"
#include "constrnt.h"
#include "cstrnchk.h"
#include "cstrnutl.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "match.h"
#include "memalloc.h"
#include "reteutil.h"
#include "router.h"
#include "rulecmp.h"

#include "pattern.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if (! RUN_TIME) && (! BLOAD_ONLY)
   static struct lhsParseNode            *ConjuctiveRestrictionParse(void *,char *,struct token *,int *);
   static struct lhsParseNode            *LiteralRestrictionParse(void *,char *,struct token *,int *);
   static int                             CheckForVariableMixing(void *,struct lhsParseNode *);
   static void                            TallyFieldTypes(struct lhsParseNode *);
#endif
   static void                            DeallocatePatternData(void *);
   static struct patternNodeHashEntry   **CreatePatternHashTable(void *,unsigned long);
   
/*****************************************************************************/
/* InitializePatterns: Initializes the global data associated with patterns. */
/*****************************************************************************/
globle void InitializePatterns(
  void *theEnv)
  {   
   AllocateEnvironmentData(theEnv,PATTERN_DATA,sizeof(struct patternData),DeallocatePatternData);
   PatternData(theEnv)->NextPosition = 1;
   PatternData(theEnv)->PatternHashTable = CreatePatternHashTable(theEnv,SIZE_PATTERN_HASH);
   PatternData(theEnv)->PatternHashTableSize = SIZE_PATTERN_HASH;
  }

/*******************************************************************/
/* CreatePatternHashTable: Creates and initializes a fact hash table. */
/*******************************************************************/
static struct patternNodeHashEntry **CreatePatternHashTable(
   void *theEnv,
   unsigned long tableSize)
   {
    unsigned long i;
    struct patternNodeHashEntry **theTable;

    theTable = (struct patternNodeHashEntry **)
               gm3(theEnv,sizeof (struct patternNodeHashEntry *) * tableSize);

    if (theTable == NULL) EnvExitRouter(theEnv,EXIT_FAILURE);
    
    for (i = 0; i < tableSize; i++) theTable[i] = NULL;
    
    return(theTable);
   }
       
/**************************************************/
/* DeallocatePatternData: Deallocates environment */
/*    data for rule pattern registration.         */
/**************************************************/
static void DeallocatePatternData(
  void *theEnv)
  {
   struct reservedSymbol *tmpRSPtr, *nextRSPtr;
   struct patternParser *tmpPPPtr, *nextPPPtr;
   struct patternNodeHashEntry *tmpPNEPtr, *nextPNEPtr;
   unsigned long i;

   tmpRSPtr = PatternData(theEnv)->ListOfReservedPatternSymbols;
   while (tmpRSPtr != NULL)
     {
      nextRSPtr = tmpRSPtr->next;
      rtn_struct(theEnv,reservedSymbol,tmpRSPtr);
      tmpRSPtr = nextRSPtr;
     }
     
   tmpPPPtr = PatternData(theEnv)->ListOfPatternParsers;
   while (tmpPPPtr != NULL)
     {
      nextPPPtr = tmpPPPtr->next;
      rtn_struct(theEnv,patternParser,tmpPPPtr);
      tmpPPPtr = nextPPPtr;
     }
   
   for (i = 0; i < PatternData(theEnv)->PatternHashTableSize; i++) 
     {
      tmpPNEPtr = PatternData(theEnv)->PatternHashTable[i];
      
      while (tmpPNEPtr != NULL)
        {
         nextPNEPtr = tmpPNEPtr->next;
         rtn_struct(theEnv,patternNodeHashEntry,tmpPNEPtr);
         tmpPNEPtr = nextPNEPtr;
        }
     }
  
   rm3(theEnv,PatternData(theEnv)->PatternHashTable,
       sizeof(struct patternNodeHashEntry *) * PatternData(theEnv)->PatternHashTableSize);
  }

/******************************************************************************/
/* AddHashedPatternNode: Adds a pattern node entry to the pattern hash table. */
/******************************************************************************/
globle void AddHashedPatternNode(
  void *theEnv,
  void *parent,
  void *child,
  unsigned short keyType,
  void *keyValue)
  {
   unsigned long hashValue;
   struct patternNodeHashEntry *newhash, *temp;

   hashValue = GetAtomicHashValue(keyType,keyValue,1) + HashExternalAddress(parent,0); /* TBD mult * 30 */

   newhash = get_struct(theEnv,patternNodeHashEntry);
   newhash->parent = parent;
   newhash->child = child;
   newhash->type = keyType;
   newhash->value = keyValue;

   hashValue = (hashValue % PatternData(theEnv)->PatternHashTableSize);
   
   temp = PatternData(theEnv)->PatternHashTable[hashValue];
   PatternData(theEnv)->PatternHashTable[hashValue] = newhash;
   newhash->next = temp;
  }

/***************************************************/
/* RemoveHashedPatternNode: Removes a pattern node */
/*   entry from the pattern node hash table.       */
/***************************************************/
globle intBool RemoveHashedPatternNode(
  void *theEnv,
  void *parent,
  void *child,
  unsigned short keyType,
  void *keyValue)
  {
   unsigned long hashValue;
   struct patternNodeHashEntry *hptr, *prev;

   hashValue = GetAtomicHashValue(keyType,keyValue,1) + HashExternalAddress(parent,0); /* TBD mult * 30 */
   hashValue = (hashValue % PatternData(theEnv)->PatternHashTableSize);

   for (hptr = PatternData(theEnv)->PatternHashTable[hashValue], prev = NULL;
        hptr != NULL;
        hptr = hptr->next)
     {
      if (hptr->child == child)
        {
         if (prev == NULL)
           {
            PatternData(theEnv)->PatternHashTable[hashValue] = hptr->next;
            rtn_struct(theEnv,patternNodeHashEntry,hptr);
            return(1);
           }
         else
           {
            prev->next = hptr->next;
            rtn_struct(theEnv,patternNodeHashEntry,hptr);
            return(1);
           }
        }
      prev = hptr;
     }

   return(0);
  }

/***********************************************/
/* FindHashedPatternNode: Finds a pattern node */
/*   entry in the pattern node hash table.     */
/***********************************************/
globle void *FindHashedPatternNode(
  void *theEnv,
  void *parent,
  unsigned short keyType,
  void *keyValue)
  {
   unsigned long hashValue;
   struct patternNodeHashEntry *hptr;

   hashValue = GetAtomicHashValue(keyType,keyValue,1) + HashExternalAddress(parent,0); /* TBD mult * 30 */
   hashValue = (hashValue % PatternData(theEnv)->PatternHashTableSize);

   for (hptr = PatternData(theEnv)->PatternHashTable[hashValue];
        hptr != NULL;
        hptr = hptr->next)
     {
      if ((hptr->parent == parent) &&
          (keyType == hptr->type) &&
          (keyValue == hptr->value))
        { return(hptr->child); }
     }

   return(NULL);
  }
  
/******************************************************************/
/* AddReservedPatternSymbol: Adds a symbol to the list of symbols */
/*  that are restricted for use in patterns. For example, the     */
/*  deftemplate construct cannot use the symbol "object", since   */
/*  this needs to be reserved for object patterns. Some symbols,  */
/*  such as "exists" are completely reserved and can not be used  */
/*  to start any type of pattern CE.                              */
/******************************************************************/
void AddReservedPatternSymbol(
  void *theEnv,
  char *theSymbol,
  char *reservedBy)
  {
   struct reservedSymbol *newSymbol;

   newSymbol = get_struct(theEnv,reservedSymbol);
   newSymbol->theSymbol = theSymbol;
   newSymbol->reservedBy = reservedBy;
   newSymbol->next = PatternData(theEnv)->ListOfReservedPatternSymbols;
   PatternData(theEnv)->ListOfReservedPatternSymbols = newSymbol;
  }

/******************************************************************/
/* ReservedPatternSymbol: Returns TRUE if the specified symbol is */
/*   a reserved pattern symbol, otherwise FALSE is returned. If   */
/*   the construct which is trying to use the symbol is the same  */
/*   construct that reserved the symbol, then FALSE is returned.  */
/******************************************************************/
intBool ReservedPatternSymbol(
  void *theEnv,
  char *theSymbol,
  char *checkedBy)
  {
   struct reservedSymbol *currentSymbol;

   for (currentSymbol = PatternData(theEnv)->ListOfReservedPatternSymbols;
        currentSymbol != NULL;
        currentSymbol = currentSymbol->next)
     {
      if (strcmp(theSymbol,currentSymbol->theSymbol) == 0)
        {
         if ((currentSymbol->reservedBy == NULL) || (checkedBy ==  NULL))
           { return(TRUE); }

         if (strcmp(checkedBy,currentSymbol->reservedBy) == 0) return(FALSE);

         return(TRUE);
        }
     }

   return(FALSE);
  }

/********************************************************/
/* ReservedPatternSymbolErrorMsg: Generic error message */
/*   for attempting to use a reserved pattern symbol.   */
/********************************************************/
void ReservedPatternSymbolErrorMsg(
  void *theEnv,
  char *theSymbol,
  char *usedFor)
  {
   PrintErrorID(theEnv,"PATTERN",1,TRUE);
   EnvPrintRouter(theEnv,WERROR,"The symbol ");
   EnvPrintRouter(theEnv,WERROR,theSymbol);
   EnvPrintRouter(theEnv,WERROR," has special meaning\n");
   EnvPrintRouter(theEnv,WERROR,"and may not be used as ");
   EnvPrintRouter(theEnv,WERROR,usedFor);
   EnvPrintRouter(theEnv,WERROR,".\n");
  }

/************************************************************/
/* GetNextEntity: Utility routine for accessing all of the  */
/*   data entities that can match patterns. Currently facts */
/*   and instances are the only data entities available.    */
/************************************************************/
globle void GetNextPatternEntity(
  void *theEnv,
  struct patternParser **theParser,
  struct patternEntity **theEntity)
  {

   /*=============================================================*/
   /* If the current parser is NULL, then we want to retrieve the */
   /* very first data entity. The traversal of entities is done   */
   /* by entity type (e.g. all facts are traversed followed by    */
   /* all instances). To get the first entity type to traverse,   */
   /* the current parser is set to the first parser on the list   */
   /* of pattern parsers.                                         */
   /*=============================================================*/

   if (*theParser == NULL)
     {
      *theParser = PatternData(theEnv)->ListOfPatternParsers;
      *theEntity = NULL;
     }

   /*================================================================*/
   /* Otherwise try to retrieve the next entity following the entity */
   /* returned by the last call to GetNextEntity. If that entity was */
   /* the last of its data type, then move on to the next pattern    */
   /* parser, otherwise return that entity as the next one.          */
   /*================================================================*/

   else if (theEntity != NULL)
     {
      *theEntity = (struct patternEntity *)
                   (*(*theParser)->entityType->base.getNextFunction)(theEnv,*theEntity);

      if ((*theEntity) != NULL) return;

      *theParser = (*theParser)->next;
     }

   /*===============================================================*/
   /* Otherwise, we encountered a situation which should not occur. */
   /* Once a NULL entity is returned from GetNextEntity, it should  */
   /* not be passed back to GetNextEntity.                          */
   /*===============================================================*/

   else
     {
      SystemError(theEnv,"PATTERN",1);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   /*================================================*/
   /* Keep looping through the lists of entities and */
   /* pattern parsers until an entity is found.      */
   /*================================================*/

   while ((*theEntity == NULL) && (*theParser != NULL))
     {
      *theEntity = (struct patternEntity *)
                   (*(*theParser)->entityType->base.getNextFunction)(theEnv,*theEntity);

      if (*theEntity != NULL) return;

      *theParser = (*theParser)->next;
     }

   return;
  }

/**************************************************************/
/* DetachPattern: Detaches a pattern from the pattern network */
/*   by calling the appropriate function for the data type    */
/*   associated with the pattern.                             */
/**************************************************************/
void DetachPattern(
  void *theEnv,
  int rhsType,
  struct patternNodeHeader *theHeader)
  {
   if (rhsType == 0) return;
   
   if (PatternData(theEnv)->PatternParserArray[rhsType-1] != NULL)
     {
      FlushAlphaMemory(theEnv,theHeader);
      (*PatternData(theEnv)->PatternParserArray[rhsType-1]->removePatternFunction)(theEnv,theHeader);
     }
  }

/**************************************************/
/* AddPatternParser: Adds a pattern type to the   */
/*   list of pattern parsers used to detect valid */
/*   patterns in the LHS of a rule.               */
/**************************************************/
globle intBool AddPatternParser(
  void *theEnv,
  struct patternParser *newPtr)
  {
   struct patternParser *currentPtr, *lastPtr = NULL;

   /*============================================*/
   /* Check to see that the limit for the number */
   /* of pattern parsers has not been exceeded.  */
   /*============================================*/

   if (PatternData(theEnv)->NextPosition >= MAX_POSITIONS) return(FALSE);

   /*================================*/
   /* Create the new pattern parser. */
   /*================================*/

   newPtr->positionInArray = PatternData(theEnv)->NextPosition;
   PatternData(theEnv)->PatternParserArray[PatternData(theEnv)->NextPosition-1] = newPtr;
   PatternData(theEnv)->NextPosition++;

   /*================================*/
   /* Add the parser to the list of  */
   /* parsers based on its priority. */
   /*================================*/

   if (PatternData(theEnv)->ListOfPatternParsers == NULL)
     {
      newPtr->next = NULL;
      PatternData(theEnv)->ListOfPatternParsers = newPtr;
      return(TRUE);
     }

   currentPtr = PatternData(theEnv)->ListOfPatternParsers;
   while ((currentPtr != NULL) ? (newPtr->priority < currentPtr->priority) : FALSE)
     {
      lastPtr = currentPtr;
      currentPtr = currentPtr->next;
     }

   if (lastPtr == NULL)
     {
      newPtr->next = PatternData(theEnv)->ListOfPatternParsers;
      PatternData(theEnv)->ListOfPatternParsers = newPtr;
     }
   else
     {
      newPtr->next = currentPtr;
      lastPtr->next = newPtr;
     }

   return(TRUE);
  }

/****************************************************/
/* FindPatternParser: Searches for a pattern parser */
/*  that can parse a pattern beginning with the     */
/*  specified keyword (e.g. "object").              */
/****************************************************/
globle struct patternParser *FindPatternParser(
  void *theEnv,
  char *name)
  {
   struct patternParser *tempParser;

   for (tempParser = PatternData(theEnv)->ListOfPatternParsers;
        tempParser != NULL;
        tempParser = tempParser->next)
     { if (strcmp(tempParser->name,name) == 0) return(tempParser); }

   return(NULL);
  }

/******************************************************/
/* GetPatternParser: Returns a pointer to the pattern */
/*    parser for the specified data entity.           */
/******************************************************/
struct patternParser *GetPatternParser(
  void *theEnv,
  int rhsType)
  {
   if (rhsType == 0) return(NULL);
   
   return(PatternData(theEnv)->PatternParserArray[rhsType-1]);
  }

#if CONSTRUCT_COMPILER && (! RUN_TIME)

/*************************************************************/
/* PatternNodeHeaderToCode: Writes the C code representation */
/*   of a patternNodeHeader data structure.                  */
/*************************************************************/
globle void PatternNodeHeaderToCode(
  void *theEnv,
  FILE *fp,
  struct patternNodeHeader *theHeader,
  int imageID,
  int maxIndices)
  {
   fprintf(fp,"{NULL,NULL,");

   if (theHeader->entryJoin == NULL)
     { fprintf(fp,"NULL,"); }
   else
     {
      fprintf(fp,"&%s%d_%d[%d],",
                 JoinPrefix(),imageID,
                 (((int) theHeader->entryJoin->bsaveID) / maxIndices) + 1,
                 ((int) theHeader->entryJoin->bsaveID) % maxIndices);
     }

   PrintHashedExpressionReference(theEnv,fp,theHeader->rightHash,imageID,maxIndices);
  
   fprintf(fp,",%d,%d,%d,0,0,%d,%d,%d}",theHeader->singlefieldNode,
                                     theHeader->multifieldNode,
                                     theHeader->stopNode,
                                     theHeader->beginSlot,
                                     theHeader->endSlot,
                                     theHeader->selector);
  }

#endif /* CONSTRUCT_COMPILER && (! RUN_TIME) */

#if (! RUN_TIME) && (! BLOAD_ONLY)

/****************************************************************/
/* PostPatternAnalysis: Calls the post analysis routines for    */
/*   each of the pattern parsers to allow additional processing */
/*   by the pattern parser after the variable analysis routines */
/*   have analyzed the LHS patterns.                            */
/****************************************************************/
globle intBool PostPatternAnalysis(
  void *theEnv,
  struct lhsParseNode *theLHS)
  {
   struct lhsParseNode *patternPtr;
   struct patternParser *tempParser;

   for (patternPtr = theLHS; patternPtr != NULL; patternPtr = patternPtr->bottom)
     {
      if ((patternPtr->type == PATTERN_CE) && (patternPtr->patternType != NULL))
        {
         tempParser = patternPtr->patternType;
         if (tempParser->postAnalysisFunction != NULL)
           { if ((*tempParser->postAnalysisFunction)(theEnv,patternPtr)) return(TRUE); }
        }
     }

   return(FALSE);
  }

/******************************************************************/
/* RestrictionParse: Parses a single field within a pattern. This */
/*    field may either be a single field wildcard, a multifield   */
/*    wildcard, a single field variable, a multifield variable,   */
/*    or a series of connected constraints.                       */
/*                                                                */
/* <constraint> ::= ? |                                           */
/*                  $? |                                          */
/*                  <connected-constraint>                        */
/******************************************************************/
struct lhsParseNode *RestrictionParse(
  void *theEnv,
  char *readSource,
  struct token *theToken,
  int multifieldSlot,
  struct symbolHashNode *theSlot,
  short slotNumber,
  CONSTRAINT_RECORD *theConstraints,
  short position)
  {
   struct lhsParseNode *topNode = NULL, *lastNode = NULL, *nextNode;
   int numberOfSingleFields = 0;
   int numberOfMultifields = 0;
   short startPosition = position;
   int error = FALSE;
   CONSTRAINT_RECORD *tempConstraints;

   /*==================================================*/
   /* Keep parsing fields until a right parenthesis is */
   /* encountered. This will either indicate the end   */
   /* of an instance or deftemplate slot or the end of */
   /* an ordered fact.                                 */
   /*==================================================*/

   while (theToken->type != RPAREN)
     {
      /*========================================*/
      /* Look for either a single or multifield */
      /* wildcard or a conjuctive restriction.  */
      /*========================================*/

      if ((theToken->type == SF_WILDCARD) ||
          (theToken->type == MF_WILDCARD))
        {
         nextNode = GetLHSParseNode(theEnv);
         nextNode->type = theToken->type;
         nextNode->negated = FALSE;
         nextNode->exists = FALSE;
         GetToken(theEnv,readSource,theToken);
        }
      else
        {
         nextNode = ConjuctiveRestrictionParse(theEnv,readSource,theToken,&error);
         if (nextNode == NULL)
           {
            ReturnLHSParseNodes(theEnv,topNode);
            return(NULL);
           }
        }

      /*========================================================*/
      /* Fix up the pretty print representation of a multifield */
      /* slot so that the fields don't run together.            */
      /*========================================================*/

      if ((theToken->type != RPAREN) && (multifieldSlot == TRUE))
        {
         PPBackup(theEnv);
         SavePPBuffer(theEnv," ");
         SavePPBuffer(theEnv,theToken->printForm);
        }

      /*========================================*/
      /* Keep track of the number of single and */
      /* multifield restrictions encountered.   */
      /*========================================*/

      if ((nextNode->type == SF_WILDCARD) || (nextNode->type == SF_VARIABLE))
        { numberOfSingleFields++; }
      else
        { numberOfMultifields++; }

      /*===================================*/
      /* Assign the slot name and indices. */
      /*===================================*/

      nextNode->slot = theSlot;
      nextNode->slotNumber = slotNumber;
      nextNode->index = position++;

      /*==============================================*/
      /* If we're not dealing with a multifield slot, */
      /* attach the constraints directly to the node  */
      /* and return.                                  */
      /*==============================================*/

      if (! multifieldSlot)
        {
         if (theConstraints == NULL)
           {
            if (nextNode->type == SF_VARIABLE)
              { nextNode->constraints = GetConstraintRecord(theEnv); }
            else nextNode->constraints = NULL;
           }
         else nextNode->constraints = theConstraints;
         return(nextNode);
        }

      /*====================================================*/
      /* Attach the restriction to the list of restrictions */
      /* already parsed for this slot or ordered fact.      */
      /*====================================================*/

      if (lastNode == NULL) topNode = nextNode;
      else lastNode->right = nextNode;

      lastNode = nextNode;
     }

   /*=====================================================*/
   /* Once we're through parsing, check to make sure that */
   /* a single field slot was given a restriction. If the */
   /* following test fails, then we know we're dealing    */
   /* with a multifield slot.                             */
   /*=====================================================*/

   if ((topNode == NULL) && (! multifieldSlot))
     {
      SyntaxErrorMessage(theEnv,"defrule");
      return(NULL);
     }

   /*===============================================*/
   /* Loop through each of the restrictions in the  */
   /* list of restrictions for the multifield slot. */
   /*===============================================*/

   for (nextNode = topNode; nextNode != NULL; nextNode = nextNode->right)
     {
      /*===================================================*/
      /* Assign a constraint record to each constraint. If */
      /* the slot has an explicit constraint, then copy    */
      /* this and store it with the constraint. Otherwise, */
      /* create a constraint record for a single field     */
      /* constraint and skip the constraint modifications  */
      /* for a multifield constraint.                      */
      /*===================================================*/

      if (theConstraints == NULL)
        {
         if (nextNode->type == SF_VARIABLE)
           { nextNode->constraints = GetConstraintRecord(theEnv); }
         else
           { continue; }
        }
      else
        { nextNode->constraints = CopyConstraintRecord(theEnv,theConstraints); }

      /*==========================================*/
      /* Remove the min and max field constraints */
      /* for the entire slot from the constraint  */
      /* record for this single constraint.       */
      /*==========================================*/

      ReturnExpression(theEnv,nextNode->constraints->minFields);
      ReturnExpression(theEnv,nextNode->constraints->maxFields);
      nextNode->constraints->minFields = GenConstant(theEnv,SYMBOL,SymbolData(theEnv)->NegativeInfinity);
      nextNode->constraints->maxFields = GenConstant(theEnv,SYMBOL,SymbolData(theEnv)->PositiveInfinity);
      nextNode->derivedConstraints = TRUE;

      /*====================================================*/
      /* If we're not dealing with a multifield constraint, */
      /* then no further modifications are needed to the    */
      /* min and max constraints for this constraint.       */
      /*====================================================*/

      if ((nextNode->type != MF_WILDCARD) && (nextNode->type != MF_VARIABLE))
        { continue; }

      /*==========================================================*/
      /* Create a separate constraint record to keep track of the */
      /* cardinality information for this multifield constraint.  */
      /*==========================================================*/

      tempConstraints = GetConstraintRecord(theEnv);
      SetConstraintType(MULTIFIELD,tempConstraints);
      tempConstraints->singlefieldsAllowed = FALSE;
      tempConstraints->multifield = nextNode->constraints;
      nextNode->constraints = tempConstraints;

      /*=====================================================*/
      /* Adjust the min and max field values for this single */
      /* multifield constraint based on the min and max      */
      /* fields for the entire slot and the number of single */
      /* field values contained in the slot.                 */
      /*=====================================================*/

      if (theConstraints->maxFields->value != SymbolData(theEnv)->PositiveInfinity)
        {
         ReturnExpression(theEnv,tempConstraints->maxFields);
         tempConstraints->maxFields = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,ValueToLong(theConstraints->maxFields->value) - numberOfSingleFields));
        }

      if ((numberOfMultifields == 1) && (theConstraints->minFields->value != SymbolData(theEnv)->NegativeInfinity))
        {
         ReturnExpression(theEnv,tempConstraints->minFields);
         tempConstraints->minFields = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,ValueToLong(theConstraints->minFields->value) - numberOfSingleFields));
        }
     }

   /*================================================*/
   /* If a multifield slot is being parsed, place a  */
   /* node on top of the list of constraints parsed. */
   /*================================================*/

   if (multifieldSlot)
     {
      nextNode = GetLHSParseNode(theEnv);
      nextNode->type = MF_WILDCARD;
      nextNode->multifieldSlot = TRUE;
      nextNode->bottom = topNode;
      nextNode->slot = theSlot;
      nextNode->slotNumber = slotNumber;
      nextNode->index = startPosition;
      nextNode->constraints = theConstraints;
      topNode = nextNode;
      TallyFieldTypes(topNode->bottom);
     }

   /*=================================*/
   /* Return the list of constraints. */
   /*=================================*/

   return(topNode);
  }

/***************************************************************/
/* TallyFieldTypes: Determines the number of single field and  */
/*   multifield variables and wildcards that appear before and */
/*   after each restriction found in a multifield slot.        */
/***************************************************************/
static void TallyFieldTypes(
  struct lhsParseNode *theRestrictions)
  {
   struct lhsParseNode *tempNode1, *tempNode2, *tempNode3;
   unsigned short totalSingleFields = 0, totalMultiFields = 0;
   unsigned short runningSingleFields = 0, runningMultiFields = 0;

   /*========================================*/
   /* Compute the total number of single and */
   /* multifield variables and wildcards.    */
   /*========================================*/

   for (tempNode1 = theRestrictions; tempNode1 != NULL; tempNode1 = tempNode1->right)
     {
      if ((tempNode1->type == SF_VARIABLE) || (tempNode1->type == SF_WILDCARD))
        { totalSingleFields++; }
      else
        { totalMultiFields++; }
     }

   /*======================================================*/
   /* Loop through each constraint tallying the numbers of */
   /* the variable types before and after along the way.   */
   /*======================================================*/

   for (tempNode1 = theRestrictions; tempNode1 != NULL; tempNode1 = tempNode1->right)
     {
      /*===================================*/
      /* Assign the values to the "binding */
      /* variable" node of the constraint. */
      /*===================================*/

      tempNode1->singleFieldsBefore = runningSingleFields;
      tempNode1->multiFieldsBefore = runningMultiFields;
      tempNode1->withinMultifieldSlot = TRUE;

      if ((tempNode1->type == SF_VARIABLE) || (tempNode1->type == SF_WILDCARD))
        {
         tempNode1->singleFieldsAfter = (unsigned short) (totalSingleFields - (runningSingleFields + 1));
         tempNode1->multiFieldsAfter = (unsigned short) (totalMultiFields - runningMultiFields);
        }
      else
        {
         tempNode1->singleFieldsAfter = (unsigned short) (totalSingleFields - runningSingleFields);
         tempNode1->multiFieldsAfter = (unsigned short) (totalMultiFields - (runningMultiFields + 1));
        }

      /*=====================================================*/
      /* Assign the values to each of the and (&) and or (|) */
      /* connected constraints within the constraint.        */
      /*=====================================================*/

      for (tempNode2 = tempNode1->bottom; tempNode2 != NULL; tempNode2 = tempNode2->bottom)
        {
         for (tempNode3 = tempNode2; tempNode3 != NULL; tempNode3 = tempNode3->right)
           {
            tempNode3->singleFieldsBefore = tempNode1->singleFieldsBefore;
            tempNode3->singleFieldsAfter = tempNode1->singleFieldsAfter;
            tempNode3->multiFieldsBefore = tempNode1->multiFieldsBefore;
            tempNode3->multiFieldsAfter = tempNode1->multiFieldsAfter;
            tempNode3->withinMultifieldSlot = TRUE;
           }
        }

      /*=======================================*/
      /* Calculate the running total of single */
      /* and multifield constraints.           */
      /*=======================================*/

      if ((tempNode1->type == SF_VARIABLE) || (tempNode1->type == SF_WILDCARD))
        { runningSingleFields++; }
      else
        { runningMultiFields++; }
     }
  }

/*******************************************************************/
/* ConjuctiveRestrictionParse: Parses a single constraint field in */
/*   a pattern that is not a single field wildcard, multifield     */
/*   wildcard, or multifield variable. The field may consist of a  */
/*   number of subfields tied together using the & connective      */
/*   constraint and/or the | connective constraint.                */
/*                                                                 */
/* <connected-constraint>                                          */
/*            ::= <single-constraint> |                            */
/*                <single-constraint> & <connected-constraint> |   */
/*                <single-constraint> | <connected-constraint>     */
/*******************************************************************/
static struct lhsParseNode *ConjuctiveRestrictionParse(
  void *theEnv,
  char *readSource,
  struct token *theToken,
  int *error)
  {
   struct lhsParseNode *bindNode;
   struct lhsParseNode *theNode, *nextOr, *nextAnd;
   int connectorType;

   /*=====================================*/
   /* Get the first node and determine if */
   /* it is a binding variable.           */
   /*=====================================*/

   theNode = LiteralRestrictionParse(theEnv,readSource,theToken,error);

   if (*error == TRUE)
     { return(NULL); }

   GetToken(theEnv,readSource,theToken);

   if (((theNode->type == SF_VARIABLE) || (theNode->type == MF_VARIABLE)) &&
       (theNode->negated == FALSE) &&
       (theToken->type != OR_CONSTRAINT))
     {
      theNode->bindingVariable = TRUE;
      bindNode = theNode;
      nextOr = NULL;
      nextAnd = NULL;
     }
   else
     {
      bindNode = GetLHSParseNode(theEnv);
      if (theNode->type == MF_VARIABLE) bindNode->type = MF_WILDCARD;
      else bindNode->type = SF_WILDCARD;
      bindNode->negated = FALSE;
      bindNode->bottom = theNode;
      nextOr = theNode;
      nextAnd = theNode;
     }

   /*===================================*/
   /* Process the connected constraints */
   /* within the constraint             */
   /*===================================*/

   while ((theToken->type == OR_CONSTRAINT) || (theToken->type == AND_CONSTRAINT))
     {
      /*==========================*/
      /* Get the next constraint. */
      /*==========================*/

      connectorType = theToken->type;

      GetToken(theEnv,readSource,theToken);
      theNode = LiteralRestrictionParse(theEnv,readSource,theToken,error);

      if (*error == TRUE)
        {
         ReturnLHSParseNodes(theEnv,bindNode);
         return(NULL);
        }

      /*=======================================*/
      /* Attach the new constraint to the list */
      /* of constraints for this field.        */
      /*=======================================*/

      if (connectorType == OR_CONSTRAINT)
        {
         if (nextOr == NULL)
           { bindNode->bottom = theNode; }
         else
           { nextOr->bottom = theNode; }
         nextOr = theNode;
         nextAnd = theNode;
        }
      else if (connectorType == AND_CONSTRAINT)
        {
         if (nextAnd == NULL)
           {
            bindNode->bottom = theNode;
            nextOr = theNode;
           }
         else
           { nextAnd->right = theNode; }
         nextAnd = theNode;
        }
      else
        {
         SystemError(theEnv,"RULEPSR",1);
         EnvExitRouter(theEnv,EXIT_FAILURE);
        }

      /*==================================================*/
      /* Determine if any more restrictions are connected */
      /* to the current list of restrictions.             */
      /*==================================================*/

      GetToken(theEnv,readSource,theToken);
     }

   /*==========================================*/
   /* Check for illegal mixing of single and   */
   /* multifield values within the constraint. */
   /*==========================================*/

   if (CheckForVariableMixing(theEnv,bindNode))
     {
      *error = TRUE;
      ReturnLHSParseNodes(theEnv,bindNode);
      return(NULL);
     }

   /*========================*/
   /* Return the constraint. */
   /*========================*/

   return(bindNode);
  }

/*****************************************************/
/* CheckForVariableMixing: Checks a field constraint */
/*   to determine if single and multifield variables */
/*   are illegally mixed within it.                  */
/*****************************************************/
static int CheckForVariableMixing(
  void *theEnv,
  struct lhsParseNode *theRestriction)
  {
   struct lhsParseNode *tempRestriction;
   CONSTRAINT_RECORD *theConstraint;
   int multifield = FALSE;
   int singlefield = FALSE;
   int constant = FALSE;
   int singleReturnValue = FALSE;
   int multiReturnValue = FALSE;

   /*================================================*/
   /* If the constraint contains a binding variable, */
   /* determine whether it is a single field or      */
   /* multifield variable.                           */
   /*================================================*/

   if (theRestriction->type == SF_VARIABLE) singlefield = TRUE;
   else if (theRestriction->type == MF_VARIABLE) multifield = TRUE;

   /*===========================================*/
   /* Loop through each of the or (|) connected */
   /* constraints within the constraint.        */
   /*===========================================*/

   for (theRestriction = theRestriction->bottom;
        theRestriction != NULL;
        theRestriction = theRestriction->bottom)
     {
      /*============================================*/
      /* Loop through each of the and (&) connected */
      /* constraints within the or (|) constraint.  */
      /*============================================*/

      for (tempRestriction = theRestriction;
           tempRestriction != NULL;
           tempRestriction = tempRestriction->right)
        {
         /*=====================================================*/
         /* Determine if the constraint contains a single field */
         /* variable, multifield variable, constant (a single   */
         /* field), a return value constraint of a function     */
         /* returning a single field value, or a return value   */
         /* constraint of a function returning a multifield     */
         /* value.                                              */
         /*=====================================================*/

         if (tempRestriction->type == SF_VARIABLE) singlefield = TRUE;
         else if (tempRestriction->type == MF_VARIABLE) multifield = TRUE;
         else if (ConstantType(tempRestriction->type)) constant = TRUE;
         else if (tempRestriction->type == RETURN_VALUE_CONSTRAINT)
           {
            theConstraint = FunctionCallToConstraintRecord(theEnv,tempRestriction->expression->value);
            if (theConstraint->anyAllowed) { /* Do nothing. */ }
            else if (theConstraint->multifieldsAllowed) multiReturnValue = TRUE;
            else singleReturnValue = TRUE;
            RemoveConstraint(theEnv,theConstraint);
           }
        }
     }

   /*================================================================*/
   /* Using a single field value (a single field variable, constant, */
   /* or function returning a single field value) together with a    */
   /* multifield value (a multifield variable or function returning  */
   /* a multifield value) is illegal. Return TRUE if this occurs.    */
   /*================================================================*/

   if ((singlefield || constant || singleReturnValue) &&
       (multifield || multiReturnValue))

     {
      PrintErrorID(theEnv,"PATTERN",2,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Single and multifield constraints cannot be mixed in a field constraint\n");
      return(TRUE);
     }

   /*=======================================*/
   /* Otherwise return FALSE to indicate no */
   /* illegal variable mixing was detected. */
   /*=======================================*/

   return(FALSE);
  }

/***********************************************************/
/* LiteralRestrictionParse: Parses a single constraint.    */
/*   The constraint may be a literal constraint, a         */
/*   predicate constraint, a return value constraint, or a */
/*   variable constraint. The constraints may also be      */
/*   negated using the ~ connective constraint.            */
/*                                                         */
/* <single-constraint>     ::= <term> | ~<term>            */
/*                                                         */
/*  <term>                 ::= <constant> |                */
/*                             <single-field-variable> |   */
/*                             <multi-field-variable> |    */
/*                             :<function-call> |          */
/*                             =<function-call>            */
/***********************************************************/
static struct lhsParseNode *LiteralRestrictionParse(
  void *theEnv,
  char *readSource,
  struct token *theToken,
  int *error)
  {
   struct lhsParseNode *topNode;
   struct expr *theExpression;

   /*============================================*/
   /* Create a node to represent the constraint. */
   /*============================================*/

   topNode = GetLHSParseNode(theEnv);

   /*=================================================*/
   /* Determine if the constraint has a '~' preceding */
   /* it. If it  does, then the field is negated      */
   /* (e.g. ~red means "not the constant red."        */
   /*=================================================*/

   if (theToken->type == NOT_CONSTRAINT)
     {
      GetToken(theEnv,readSource,theToken);
      topNode->negated = TRUE;
     }
   else
     { topNode->negated = FALSE; }

   /*===========================================*/
   /* Determine if the constraint is one of the */
   /* recognized types. These are ?variables,   */
   /* symbols, strings, numbers, :(expression), */
   /* and =(expression).                        */
   /*===========================================*/

   topNode->type = theToken->type;

   /*============================================*/
   /* Any symbol is valid, but an = signifies a  */
   /* return value constraint and an : signifies */
   /* a predicate constraint.                    */
   /*============================================*/

   if (theToken->type == SYMBOL)
     {
      /*==============================*/
      /* If the symbol is an =, parse */
      /* a return value constraint.   */
      /*==============================*/

      if (strcmp(ValueToString(theToken->value),"=") == 0)
        {
         theExpression = Function0Parse(theEnv,readSource);
         if (theExpression == NULL)
           {
            *error = TRUE;
            ReturnLHSParseNodes(theEnv,topNode);
            return(NULL);
           }
         topNode->type = RETURN_VALUE_CONSTRAINT;
         topNode->expression = ExpressionToLHSParseNodes(theEnv,theExpression);
         ReturnExpression(theEnv,theExpression);
        }

      /*=============================*/
      /* If the symbol is a :, parse */
      /* a predicate constraint.     */
      /*=============================*/

      else if (strcmp(ValueToString(theToken->value),":") == 0)
        {
         theExpression = Function0Parse(theEnv,readSource);
         if (theExpression == NULL)
           {
            *error = TRUE;
            ReturnLHSParseNodes(theEnv,topNode);
            return(NULL);
           }
         topNode->type = PREDICATE_CONSTRAINT;
         topNode->expression = ExpressionToLHSParseNodes(theEnv,theExpression);
         ReturnExpression(theEnv,theExpression);
        }

      /*==============================================*/
      /* Otherwise, treat the constraint as a symbol. */
      /*==============================================*/

      else
        { topNode->value = theToken->value; }
     }

   /*=====================================================*/
   /* Single and multifield variables and float, integer, */
   /* string, and instance name constants are also valid. */
   /*=====================================================*/

   else if ((theToken->type == SF_VARIABLE)  ||
            (theToken->type == MF_VARIABLE)  ||
            (theToken->type == FLOAT) ||
            (theToken->type == INTEGER) ||
            (theToken->type == STRING) ||
            (theToken->type == INSTANCE_NAME))
     { topNode->value = theToken->value; }

   /*===========================*/
   /* Anything else is invalid. */
   /*===========================*/

   else
     {
      SyntaxErrorMessage(theEnv,"defrule");
      *error = TRUE;
      ReturnLHSParseNodes(theEnv,topNode);
      return(NULL);
     }

   /*===============================*/
   /* Return the parsed constraint. */
   /*===============================*/

   return(topNode);
  }

#endif /* (! RUN_TIME) && (! BLOAD_ONLY) */

#endif /* DEFRULE_CONSTRUCT */




