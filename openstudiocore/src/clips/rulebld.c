   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                  RULE BUILD MODULE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines to ntegrates a set of pattern  */
/*   and join tests associated with a rule into the pattern  */
/*   and join networks. The joins are integrated into the    */
/*   join network by routines in this module. The pattern    */
/*   is integrated by calling the external routine           */
/*   associated with the pattern parser that originally      */
/*   parsed the pattern.                                     */
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
/*            Corrected code to remove compiler warnings.    */
/*                                                           */
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*************************************************************/

#define _RULEBLD_SOURCE_

#include "setup.h"

#if DEFRULE_CONSTRUCT && (! RUN_TIME) && (! BLOAD_ONLY)

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>

#include "constant.h"
#include "envrnmnt.h"
#include "constrct.h"
#include "drive.h"
#include "incrrset.h"
#include "memalloc.h"
#include "pattern.h"
#include "reteutil.h"
#include "router.h"
#include "rulebld.h"
#include "watch.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static struct joinNode        *FindShareableJoin(struct joinLink *,struct joinNode *,intBool,void *,unsigned,unsigned,
                                                    unsigned,unsigned,struct expr *,struct expr *,
                                                    struct expr *,struct expr *);
   static int                     TestJoinForReuse(struct joinNode *,unsigned,unsigned,
                                                   unsigned,unsigned,struct expr *,struct expr *,
                                                   struct expr *,struct expr *);
   static struct joinNode        *CreateNewJoin(void *,struct expr *,struct expr *,struct joinNode *,void *,
                                                int,int,int,struct expr *,struct expr *);
   static void                    AttachTestCEsToPatternCEs(void *,struct lhsParseNode *);

/****************************************************************/
/* ConstructJoins: Integrates a set of pattern and join tests   */
/*   associated with a rule into the pattern and join networks. */
/****************************************************************/
globle struct joinNode *ConstructJoins(
  void *theEnv,
  int logicalJoin,
  struct lhsParseNode *theLHS,
  int startDepth)
  {
   struct joinNode *lastJoin = NULL;
   struct patternNodeHeader *lastPattern;
   unsigned firstJoin = TRUE;
   int tryToReuse = TRUE;
   struct joinNode *listOfJoins = NULL;
   struct joinNode *oldJoin;
   int joinNumber = 1;
   int isLogical, isExists;
   struct joinNode *lastRightJoin;
   int lastIteration = FALSE;
   int rhsType;
   struct expr *leftHash, *rightHash;
   void *rhsStruct;
   struct lhsParseNode *nextLHS;
   struct expr *networkTest, *secondaryNetworkTest, *secondaryExternalTest;
   int joinFromTheRight;
   struct joinLink *theLinks;
   intBool useLinks;

   /*===================================================*/
   /* Remove any test CEs from the LHS and attach their */
   /* expression to the closest preceeding non-negated  */
   /* join at the same not/and depth.                   */
   /*===================================================*/

   if (startDepth == 1)
     { AttachTestCEsToPatternCEs(theEnv,theLHS); }

   if (theLHS == NULL)
     {
      lastJoin = FindShareableJoin(DefruleData(theEnv)->RightPrimeJoins,NULL,TRUE,NULL,TRUE,
                                   FALSE,FALSE,FALSE,NULL,NULL,NULL,NULL);
                                        
      if (lastJoin == NULL)
        { lastJoin = CreateNewJoin(theEnv,NULL,NULL,NULL,NULL,FALSE,FALSE,FALSE,NULL,NULL); }
     }

   /*=====================================================*/
   /* Process each pattern CE in the rule. At this point, */
   /* there should be no and/or/not/test CEs in the LHS.  */
   /*=====================================================*/

   while (theLHS != NULL)
     {
      /*======================================================*/
      /* Find the beginning of the next group of patterns. If */
      /* the current pattern is not the beginning of a "join  */
      /* from the right" group of patterns, then the next     */
      /* pattern is the next pattern. Otherwise skip over all */
      /* the patterns that belong to the group of subjoins.   */
      /*======================================================*/
             
      nextLHS = theLHS->bottom;
      secondaryExternalTest = NULL;

      if (theLHS->endNandDepth > startDepth)
        {
         while ((nextLHS != NULL) &&
                (nextLHS->endNandDepth > startDepth))
           { nextLHS = nextLHS->bottom; }
         
         /*====================================================*/
         /* Variable nextLHS is now pointing to the end of the */
         /* not/and group beginning with variable theLHS. If   */ 
         /* the end depth of the group is less than the depth  */
         /* of the current enclosing not/and group, then this  */
         /* is the last iteration for the enclosing group.     */
         /*====================================================*/
           
         if (nextLHS != NULL)
           {
            if (nextLHS->endNandDepth < startDepth)
              { lastIteration = TRUE; }
           }
           
         if (nextLHS != NULL)
           { nextLHS = nextLHS->bottom; }
           
         if ((nextLHS != NULL) && (nextLHS->type == TEST_CE))
           { 
            secondaryExternalTest = nextLHS->networkTest;
            nextLHS = nextLHS->bottom; 
           }
        }       

      /*=======================================*/
      /* Is this the last pattern to be added? */
      /*=======================================*/
      
      if (nextLHS == NULL)
        { lastIteration = TRUE; }
      else if (theLHS->endNandDepth < startDepth)
        { lastIteration = TRUE; } 
      else if ((nextLHS->type == TEST_CE) &&
               (theLHS->beginNandDepth > startDepth) &&
               (nextLHS->endNandDepth < startDepth))
        { lastIteration = TRUE; } 

      /*===============================================*/
      /* If the pattern is a join from the right, then */
      /* construct the subgroup of patterns and use    */
      /* that as the RHS of the join to be added.      */
      /*===============================================*/
                                         
      if (theLHS->beginNandDepth > startDepth)
        {
         joinFromTheRight = TRUE;
         isExists = theLHS->existsNand;

         lastRightJoin = ConstructJoins(theEnv,logicalJoin,theLHS,startDepth+1);
           
         rhsStruct = lastRightJoin;
         rhsType = 0;
         lastPattern = NULL;
         networkTest = theLHS->externalNetworkTest; /* TBD */
         secondaryNetworkTest = secondaryExternalTest;
         leftHash = theLHS->externalLeftHash;
         rightHash = theLHS->externalRightHash;
        } 
        
      /*=======================================================*/
      /* Otherwise, add the pattern to the appropriate pattern */
      /* network and use the pattern node containing the alpha */
      /* memory as the RHS of the join to be added.            */
      /*=======================================================*/
      
      else if (theLHS->right == NULL)
        {
         joinFromTheRight = FALSE;
         rhsType = 0;
         lastPattern = NULL;
         rhsStruct = NULL;
         lastRightJoin = NULL;
         isExists = theLHS->exists;
         networkTest = theLHS->networkTest;
         secondaryNetworkTest = theLHS->secondaryNetworkTest;
         leftHash = NULL;
         rightHash = NULL;
        }
      else
        {
         joinFromTheRight = FALSE;
         rhsType = theLHS->patternType->positionInArray;
         lastPattern = (*theLHS->patternType->addPatternFunction)(theEnv,theLHS);
         rhsStruct = lastPattern;
         lastRightJoin = NULL;
         isExists = theLHS->exists;
         networkTest = theLHS->networkTest;
         secondaryNetworkTest = theLHS->secondaryNetworkTest;
         leftHash = theLHS->leftHash;
         rightHash = theLHS->rightHash;
        }      

      /*======================================================*/
      /* Determine if the join being added is a logical join. */
      /*======================================================*/

      if ((startDepth == 1) && (joinNumber == logicalJoin)) isLogical = TRUE;
      else isLogical = FALSE;

      /*===============================================*/
      /* Get the list of joins which could potentially */
      /* be reused in place of the join being added.   */
      /*===============================================*/

      useLinks = TRUE;
      if (firstJoin == TRUE)
        { 
         if (theLHS->right == NULL)
           { theLinks = DefruleData(theEnv)->RightPrimeJoins; }
         else if (lastPattern != NULL)
           { 
            listOfJoins = lastPattern->entryJoin;
            theLinks = NULL;
            useLinks = FALSE;
           }
         else
           { theLinks = lastRightJoin->nextLinks; }
        }
      else
        { theLinks = lastJoin->nextLinks; }

      /*=======================================================*/
      /* Determine if the next join to be added can be shared. */
      /*=======================================================*/

      if ((tryToReuse == TRUE) &&
          ((oldJoin = FindShareableJoin(theLinks,listOfJoins,useLinks,rhsStruct,firstJoin,
                                        theLHS->negated,isExists,isLogical,
                                        networkTest,secondaryNetworkTest,
                                        leftHash,rightHash)) != NULL) )
        {
#if DEBUGGING_FUNCTIONS
         if ((EnvGetWatchItem(theEnv,"compilations") == TRUE) && GetPrintWhileLoading(theEnv))
           { EnvPrintRouter(theEnv,WDIALOG,"=j"); }
#endif
         lastJoin = oldJoin;
        }
      else
        {
         tryToReuse = FALSE;
         if (! joinFromTheRight)
           {
            lastJoin = CreateNewJoin(theEnv,networkTest,secondaryNetworkTest,lastJoin,
                                     lastPattern,FALSE,(int) theLHS->negated, isExists,
                                     leftHash,rightHash);
            lastJoin->rhsType = rhsType;
           }
         else
           {
            lastJoin = CreateNewJoin(theEnv,networkTest,secondaryNetworkTest,lastJoin,
                                     lastRightJoin,TRUE,(int) theLHS->negated, isExists,
                                     leftHash,rightHash);
            lastJoin->rhsType = rhsType;
           }
        }
      
      /*============================================*/
      /* If we've reached the end of the subgroup,  */
      /* then return the last join of the subgroup. */
      /*============================================*/
      
      if (lastIteration)
        { break; }
        
      /*=======================================*/
      /* Move on to the next join to be added. */
      /*=======================================*/

      theLHS = nextLHS;
      joinNumber++;
      firstJoin = FALSE;
     }

   /*=================================================*/
   /* Add the final join which stores the activations */
   /* of the rule. This join is never shared.         */
   /*=================================================*/
   
   if (startDepth == 1)
     {
      lastJoin = CreateNewJoin(theEnv,NULL,NULL,lastJoin,NULL,
                               FALSE,FALSE,FALSE,NULL,NULL);
     }

   /*===================================================*/
   /* If compilations are being watched, put a carriage */
   /* return after all of the =j's and +j's             */
   /*===================================================*/

#if DEBUGGING_FUNCTIONS
   if ((startDepth == 1) &&
       (EnvGetWatchItem(theEnv,"compilations") == TRUE) && 
       GetPrintWhileLoading(theEnv))
     { EnvPrintRouter(theEnv,WDIALOG,"\n"); }
#endif

   /*=============================*/
   /* Return the last join added. */
   /*=============================*/

   return(lastJoin);
  }

/****************************************************************/
/* AttachTestCEsToPatternCEs: Attaches the expressions found in */
/*   test CEs to the closest preceeding pattern CE that is not  */
/*   negated and is at the same not/and depth.                  */
/****************************************************************/
static void AttachTestCEsToPatternCEs(
  void *theEnv,
  struct lhsParseNode *theLHS)
  {
   struct lhsParseNode *lastNode, *tempNode;

   if (theLHS == NULL) return;

   /*=============================================================*/
   /* Attach test CEs that can be attached directly to a pattern. */
   /*=============================================================*/
   
   lastNode = theLHS;
   theLHS = lastNode->bottom;
   
   while (theLHS != NULL)
     {
      if ((theLHS->type != TEST_CE) ||
          (lastNode->beginNandDepth != lastNode->endNandDepth) ||
          (lastNode->beginNandDepth != theLHS->beginNandDepth))
        {
         lastNode = theLHS;
         theLHS = theLHS->bottom;
         continue;
        }
        
      if (lastNode->negated)
        {
         lastNode->secondaryNetworkTest =
            CombineExpressions(theEnv,lastNode->secondaryNetworkTest,theLHS->networkTest);
        }
      else
        {
         lastNode->networkTest =
            CombineExpressions(theEnv,lastNode->networkTest,theLHS->networkTest);
        }
        
      theLHS->networkTest = NULL;
      tempNode = theLHS->bottom;
      theLHS->bottom = NULL;
      lastNode->bottom = tempNode;
      lastNode->endNandDepth = theLHS->endNandDepth;
      ReturnLHSParseNodes(theEnv,theLHS);
      theLHS = tempNode;
     }
  }
  
/********************************************************************/
/* FindShareableJoin: Determines whether a join exists that can be  */
/*   reused for the join currently being added to the join network. */
/*   Returns a pointer to the join to be shared if one if found,    */
/*   otherwise returns a NULL pointer.                              */
/********************************************************************/
static struct joinNode *FindShareableJoin(
  struct joinLink *theLinks,
  struct joinNode *listOfJoins,
  intBool useLinks,
  void *rhsStruct,
  unsigned int firstJoin,
  unsigned int negatedRHS,
  unsigned int existsRHS,
  unsigned int isLogical,
  struct expr *joinTest,
  struct expr *secondaryJoinTest,
  struct expr *leftHash,
  struct expr *rightHash)
  {   
   /*========================================*/
   /* Loop through all of the joins in the   */
   /* list of potential candiates for reuse. */
   /*========================================*/

   if (useLinks)
     { 
      if (theLinks != NULL)
        { listOfJoins = theLinks->join; }
      else
        { listOfJoins = NULL; }
     }
     
   while (listOfJoins != NULL)
     {
      /*=========================================================*/
      /* If the join being tested for reuse is connected on the  */
      /* RHS to the end node of the pattern node associated with */
      /* the join to be added, then determine if the join can    */
      /* be reused. If so, return the join.                      */
      /*=========================================================*/

      if (listOfJoins->rightSideEntryStructure == rhsStruct)
        {
         if (TestJoinForReuse(listOfJoins,firstJoin,negatedRHS,existsRHS,
                              isLogical,joinTest,secondaryJoinTest,
                              leftHash,rightHash))
           { return(listOfJoins); }
        }

      /*====================================================*/
      /* Move on to the next potential candidate. Note that */
      /* the rightMatchNode link is used for traversing     */
      /* through the candidates for the first join of a     */
      /* rule and that rightDriveNode link is used for      */
      /* traversing through the candidates for subsequent   */
      /* joins of a rule.                                   */
      /*====================================================*/

      if (useLinks)
        {
         theLinks = theLinks->next;
         if (theLinks != NULL) 
           { listOfJoins = theLinks->join; }
         else
           { listOfJoins = NULL; }
        }
      else
        { listOfJoins = listOfJoins->rightMatchNode; }
     }

   /*================================*/
   /* Return a NULL pointer, since a */
   /* reusable join was not found.   */
   /*================================*/

   return(NULL);
  }

/**************************************************************/
/* TestJoinForReuse: Determines if the specified join can be  */
/*   shared with a join being added for a rule being defined. */
/*   Returns TRUE if the join can be shared, otherwise FALSE. */
/**************************************************************/
static int TestJoinForReuse(
  struct joinNode *testJoin,
  unsigned firstJoin,
  unsigned negatedRHS,
  unsigned existsRHS,
  unsigned int isLogical,
  struct expr *joinTest,
  struct expr *secondaryJoinTest,
  struct expr *leftHash,
  struct expr *rightHash)
  {
   /*==================================================*/
   /* The first join of a rule may only be shared with */
   /* a join that has its firstJoin field set to TRUE. */
   /*==================================================*/

   if (testJoin->firstJoin != firstJoin) return(FALSE);

   /*========================================================*/
   /* A join connected to a not CE may only be shared with a */
   /* join that has its patternIsNegated field set to TRUE.  */
   /*========================================================*/

   if ((testJoin->patternIsNegated != negatedRHS) && (! existsRHS)) return(FALSE);

   /*==========================================================*/
   /* A join connected to an exists CE may only be shared with */
   /* a join that has its patternIsExists field set to TRUE.   */
   /*==========================================================*/

   if (testJoin->patternIsExists != existsRHS) return(FALSE);
   
   /*==========================================================*/
   /* If the join added is associated with a logical CE, then  */
   /* either the join to be shared must be associated with a   */
   /* logical CE or the beta memory must be empty (since       */
   /* joins associate an extra field with each partial match). */
   /*==========================================================*/

   if ((isLogical == TRUE) &&
       (testJoin->logicalJoin == FALSE) &&
       BetaMemoryNotEmpty(testJoin))
     { return(FALSE); }

   /*===============================================================*/
   /* The expression associated with the join must be identical to  */
   /* the networkTest expression stored with the join to be shared. */
   /*===============================================================*/

   if (IdenticalExpression(testJoin->networkTest,joinTest) != TRUE)
     { return(FALSE); }

   if (IdenticalExpression(testJoin->secondaryNetworkTest,secondaryJoinTest) != TRUE)
     { return(FALSE); }
     
   /*====================================================================*/
   /* The alpha memory hashing values associated with the join must be   */
   /* identical to the hashing values stored with the join to be shared. */
   /*====================================================================*/

   if (IdenticalExpression(testJoin->leftHash,leftHash) != TRUE)
     { return(FALSE); }

   if (IdenticalExpression(testJoin->rightHash,rightHash) != TRUE)
     { return(FALSE); }
     
   /*=============================================*/
   /* The join can be shared since all conditions */
   /* for sharing have been satisfied.            */
   /*=============================================*/

   return(TRUE);
  }

/*************************************************************************/
/* CreateNewJoin: Creates a new join and links it into the join network. */
/*************************************************************************/
static struct joinNode *CreateNewJoin(
  void *theEnv,
  struct expr *joinTest,
  struct expr *secondaryJoinTest,
  struct joinNode *lhsEntryStruct,
  void *rhsEntryStruct,
  int joinFromTheRight,
  int negatedRHSPattern,
  int existsRHSPattern,
  struct expr *leftHash,
  struct expr *rightHash)
  {
   struct joinNode *newJoin;
   struct joinLink *theLink;
   
   /*===============================================*/
   /* If compilations are being watch, print +j to  */
   /* indicate that a new join has been created for */
   /* this pattern of the rule (i.e. a join could   */
   /* not be shared with another rule.              */
   /*===============================================*/

#if DEBUGGING_FUNCTIONS
   if ((EnvGetWatchItem(theEnv,"compilations") == TRUE) && GetPrintWhileLoading(theEnv))
     { EnvPrintRouter(theEnv,WDIALOG,"+j"); }
#endif

   /*======================*/
   /* Create the new join. */
   /*======================*/

   newJoin = get_struct(theEnv,joinNode);
   
   /*======================================================*/
   /* The first join of a rule does not have a beta memory */
   /* unless the RHS pattern is an exists or not CE.       */
   /*======================================================*/
   
   if ((lhsEntryStruct != NULL) || existsRHSPattern || negatedRHSPattern || joinFromTheRight)
     {
      if (leftHash == NULL)     
        {      
         newJoin->leftMemory = get_struct(theEnv,betaMemory); 
         newJoin->leftMemory->beta = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *));
         newJoin->leftMemory->beta[0] = NULL;
         newJoin->leftMemory->size = 1;
         newJoin->leftMemory->count = 0;
         }
      else
        {
         newJoin->leftMemory = get_struct(theEnv,betaMemory); 
         newJoin->leftMemory->beta = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *) * INITIAL_BETA_HASH_SIZE);
         memset(newJoin->leftMemory->beta,0,sizeof(struct partialMatch *) * INITIAL_BETA_HASH_SIZE);
         newJoin->leftMemory->size = INITIAL_BETA_HASH_SIZE;
         newJoin->leftMemory->count = 0;
        }
      
      /*===========================================================*/
      /* If the first join of a rule connects to an exists or not  */
      /* CE, then we create an empty partial match for the usually */
      /* empty left beta memory so that we can track the current   */
      /* current right memory partial match satisfying the CE.     */
      /*===========================================================*/
         
      if ((lhsEntryStruct == NULL) && (existsRHSPattern || negatedRHSPattern || joinFromTheRight))
        {
         newJoin->leftMemory->beta[0] = CreateEmptyPartialMatch(theEnv); 
         newJoin->leftMemory->beta[0]->owner = newJoin;
         newJoin->leftMemory->count = 1;
        }
     }
   else
     { newJoin->leftMemory = NULL; }
     
   if (joinFromTheRight)
     {
      if (leftHash == NULL)     
        {      
         newJoin->rightMemory = get_struct(theEnv,betaMemory); 
         newJoin->rightMemory->beta = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *));
         newJoin->rightMemory->last = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *));
         newJoin->rightMemory->beta[0] = NULL;
         newJoin->rightMemory->last[0] = NULL;
         newJoin->rightMemory->size = 1;
         newJoin->rightMemory->count = 0;
         }
      else
        {
         newJoin->rightMemory = get_struct(theEnv,betaMemory); 
         newJoin->rightMemory->beta = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *) * INITIAL_BETA_HASH_SIZE);
         newJoin->rightMemory->last = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *) * INITIAL_BETA_HASH_SIZE);
         memset(newJoin->rightMemory->beta,0,sizeof(struct partialMatch *) * INITIAL_BETA_HASH_SIZE);
         memset(newJoin->rightMemory->last,0,sizeof(struct partialMatch *) * INITIAL_BETA_HASH_SIZE);
         newJoin->rightMemory->size = INITIAL_BETA_HASH_SIZE;
         newJoin->rightMemory->count = 0;
        }     
     }
   else if ((lhsEntryStruct == NULL) && (rhsEntryStruct == NULL))
     {
      newJoin->rightMemory = get_struct(theEnv,betaMemory); 
      newJoin->rightMemory->beta = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *));
      newJoin->rightMemory->last = (struct partialMatch **) genalloc(theEnv,sizeof(struct partialMatch *));
      newJoin->rightMemory->beta[0] = CreateEmptyPartialMatch(theEnv);
      newJoin->rightMemory->beta[0]->owner = newJoin;
      newJoin->rightMemory->beta[0]->rhsMemory = TRUE;
      newJoin->rightMemory->last[0] = newJoin->rightMemory->beta[0];
      newJoin->rightMemory->size = 1;
      newJoin->rightMemory->count = 1;    
     }
   else
     { newJoin->rightMemory = NULL; }
     
   newJoin->nextLinks = NULL;
   newJoin->joinFromTheRight = joinFromTheRight;
   
   if (existsRHSPattern)
     { newJoin->patternIsNegated = FALSE; }
   else
     { newJoin->patternIsNegated = negatedRHSPattern; }
   newJoin->patternIsExists = existsRHSPattern;

   newJoin->marked = FALSE;
   newJoin->initialize = EnvGetIncrementalReset(theEnv);
   newJoin->logicalJoin = FALSE;
   newJoin->ruleToActivate = NULL;
   newJoin->memoryAdds = 0;
   newJoin->memoryDeletes = 0;
   newJoin->memoryCompares = 0;

   /*==============================================*/
   /* Install the expressions used to determine    */
   /* if a partial match satisfies the constraints */
   /* associated with this join.                   */
   /*==============================================*/

   newJoin->networkTest = AddHashedExpression(theEnv,joinTest);
   newJoin->secondaryNetworkTest = AddHashedExpression(theEnv,secondaryJoinTest);
   
   /*=====================================================*/
   /* Install the expression used to hash the beta memory */
   /* partial match to determine the location to search   */
   /* in the alpha memory.                                */
   /*=====================================================*/
   
   newJoin->leftHash = AddHashedExpression(theEnv,leftHash);
   newJoin->rightHash = AddHashedExpression(theEnv,rightHash);

   /*============================================================*/
   /* Initialize the values associated with the LHS of the join. */
   /*============================================================*/

   newJoin->lastLevel = lhsEntryStruct;

   if (lhsEntryStruct == NULL)
     {
      newJoin->firstJoin = TRUE;
      newJoin->depth = 1;
     }
   else
     {
      newJoin->firstJoin = FALSE;
      newJoin->depth = lhsEntryStruct->depth;
      newJoin->depth++; /* To work around Sparcworks C compiler bug */
      
      theLink = get_struct(theEnv,joinLink);
      theLink->join = newJoin;
      theLink->enterDirection = LHS;
      theLink->next = lhsEntryStruct->nextLinks;
      lhsEntryStruct->nextLinks = theLink;
     }

   /*=======================================================*/
   /* Initialize the pointer values associated with the RHS */
   /* of the join (both for the new join and the join or    */
   /* pattern which enters this join from the right.        */
   /*=======================================================*/

   newJoin->rightSideEntryStructure = rhsEntryStruct;
   
   if (rhsEntryStruct == NULL)
     { 
      if (newJoin->firstJoin)
        {
         theLink = get_struct(theEnv,joinLink);
         theLink->join = newJoin;
         theLink->enterDirection = RHS;
         theLink->next = DefruleData(theEnv)->RightPrimeJoins;
         DefruleData(theEnv)->RightPrimeJoins = theLink;
        }
        
      newJoin->rightMatchNode = NULL;
        
      return(newJoin); 
     }
     
   /*===========================================================*/
   /* If the first join of a rule is a not CE, then it needs to */
   /* be "primed" under certain circumstances. This used to be  */
   /* handled by adding the (initial-fact) pattern to a rule    */
   /* with the not CE as its first pattern, but this alternate  */
   /* mechanism is now used so patterns don't have to be added. */
   /*===========================================================*/
     
   if (newJoin->firstJoin && (newJoin->patternIsNegated || newJoin->joinFromTheRight) && (! newJoin->patternIsExists))
     {
      theLink = get_struct(theEnv,joinLink);
      theLink->join = newJoin;
      theLink->enterDirection = LHS;
      theLink->next = DefruleData(theEnv)->LeftPrimeJoins;
      DefruleData(theEnv)->LeftPrimeJoins = theLink;
     }
       
   if (joinFromTheRight)
     {
      theLink = get_struct(theEnv,joinLink);
      theLink->join = newJoin;
      theLink->enterDirection = RHS;
      theLink->next = ((struct joinNode *) rhsEntryStruct)->nextLinks;
      ((struct joinNode *) rhsEntryStruct)->nextLinks = theLink;
      newJoin->rightMatchNode = NULL;
     }
   else
     {
      newJoin->rightMatchNode = ((struct patternNodeHeader *) rhsEntryStruct)->entryJoin;
      ((struct patternNodeHeader *) rhsEntryStruct)->entryJoin = newJoin;
     }

   /*================================*/
   /* Return the newly created join. */
   /*================================*/

   return(newJoin);
  }

#endif



