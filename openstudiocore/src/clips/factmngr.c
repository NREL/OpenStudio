   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                 FACT MANAGER MODULE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides core routines for maintaining the fact  */
/*   list including assert/retract operations, data          */
/*   structure creation/deletion, printing, slot access,     */
/*   and other utility functions.                            */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Added support for templates maintaining their  */
/*            own list of facts.                             */
/*                                                           */
/*      6.24: Removed LOGICAL_DEPENDENCIES compilation flag. */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            AssignFactSlotDefaults function does not       */
/*            properly handle defaults for multifield slots. */
/*            DR0869                                         */
/*                                                           */
/*            Support for ppfact command.                    */
/*                                                           */
/*************************************************************/


#define _FACTMNGR_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT && DEFRULE_CONSTRUCT

#include "constant.h"
#include "symbol.h"
#include "memalloc.h"
#include "exprnpsr.h"
#include "argacces.h"
#include "scanner.h"
#include "router.h"
#include "strngrtr.h"
#include "match.h"
#include "factbld.h"
#include "factqury.h"
#include "reteutil.h"
#include "retract.h"
#include "factcmp.h"
#include "filecom.h"
#include "factfun.h"
#include "factcom.h"
#include "constrct.h"
#include "factrhs.h"
#include "factmch.h"
#include "watch.h"
#include "utility.h"
#include "factbin.h"
#include "factmngr.h"
#include "facthsh.h"
#include "default.h"
#include "commline.h"
#include "envrnmnt.h"
#include "sysdep.h"

#include "engine.h"
#include "lgcldpnd.h"
#include "drive.h"
#include "ruledlt.h"

#include "tmpltbsc.h"
#include "tmpltdef.h"
#include "tmpltutl.h"
#include "tmpltfun.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                    ResetFacts(void *);
   static int                     ClearFactsReady(void *);
   static void                    RemoveGarbageFacts(void *);
   static void                    DeallocateFactData(void *);

/**************************************************************/
/* InitializeFacts: Initializes the fact data representation. */
/*   Facts are only available when both the defrule and       */
/*   deftemplate constructs are available.                    */
/**************************************************************/
globle void InitializeFacts(
  void *theEnv)
  {
   struct patternEntityRecord factInfo = { { "FACT_ADDRESS", FACT_ADDRESS,1,0,0,
                                                     PrintFactIdentifier,
                                                     PrintFactIdentifierInLongForm,
                                                     EnvRetract,
                                                     NULL,
                                                     EnvGetNextFact,
                                                     EnvIncrementFactCount,
                                                     EnvDecrementFactCount,NULL,NULL,NULL,NULL,NULL
                                                   },
                                                   DecrementFactBasisCount,
                                                   IncrementFactBasisCount,
                                                   MatchFactFunction,
                                                   NULL
                                                 };
                                                 
   struct fact dummyFact = { { NULL, NULL, 0, 0L }, NULL, NULL, -1L, 0, 0, 1,
                                  NULL, NULL, NULL, NULL, { 1, 0, 0UL, NULL, { { 0, NULL } } } };
   
   AllocateEnvironmentData(theEnv,FACTS_DATA,sizeof(struct factsData),DeallocateFactData);

   memcpy(&FactData(theEnv)->FactInfo,&factInfo,sizeof(struct patternEntityRecord)); 
   dummyFact.factHeader.theInfo = &FactData(theEnv)->FactInfo;    
   memcpy(&FactData(theEnv)->DummyFact,&dummyFact,sizeof(struct fact));  
   FactData(theEnv)->LastModuleIndex = -1;

   /*=========================================*/
   /* Initialize the fact hash table (used to */
   /* quickly determine if a fact exists).    */
   /*=========================================*/

   InitializeFactHashTable(theEnv);

   /*============================================*/
   /* Initialize the fact callback functions for */
   /* use with the reset and clear commands.     */
   /*============================================*/

   EnvAddResetFunction(theEnv,"facts",ResetFacts,60);
   AddClearReadyFunction(theEnv,"facts",ClearFactsReady,0);

   /*=============================*/
   /* Initialize periodic garbage */
   /* collection for facts.       */
   /*=============================*/

   AddCleanupFunction(theEnv,"facts",RemoveGarbageFacts,0);

   /*===================================*/
   /* Initialize fact pattern matching. */
   /*===================================*/

   InitializeFactPatterns(theEnv);

   /*==================================*/
   /* Initialize the facts keyword for */
   /* use with the watch command.      */
   /*==================================*/

#if DEBUGGING_FUNCTIONS
   AddWatchItem(theEnv,"facts",0,&FactData(theEnv)->WatchFacts,80,DeftemplateWatchAccess,DeftemplateWatchPrint);
#endif

   /*=========================================*/
   /* Initialize fact commands and functions. */
   /*=========================================*/

   FactCommandDefinitions(theEnv);
   FactFunctionDefinitions(theEnv);
   
   /*==============================*/
   /* Initialize fact set queries. */
   /*==============================*/
  
#if FACT_SET_QUERIES
   SetupFactQuery(theEnv);
#endif

   /*==================================*/
   /* Initialize fact patterns for use */
   /* with the bload/bsave commands.   */
   /*==================================*/

#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME)
   FactBinarySetup(theEnv);
#endif

   /*===================================*/
   /* Initialize fact patterns for use  */
   /* with the constructs-to-c command. */
   /*===================================*/

#if CONSTRUCT_COMPILER && (! RUN_TIME)
   FactPatternsCompilerSetup(theEnv);
#endif
  }
  
/***********************************/
/* DeallocateFactData: Deallocates */
/*   environment data for facts.   */
/***********************************/
static void DeallocateFactData(
  void *theEnv)
  {
   struct factHashEntry *tmpFHEPtr, *nextFHEPtr;
   struct fact *tmpFactPtr, *nextFactPtr;
   unsigned long i;
   struct patternMatch *theMatch, *tmpMatch;
   
   for (i = 0; i < FactData(theEnv)->FactHashTableSize; i++) 
     {
      tmpFHEPtr = FactData(theEnv)->FactHashTable[i];
      
      while (tmpFHEPtr != NULL)
        {
         nextFHEPtr = tmpFHEPtr->next;
         rtn_struct(theEnv,factHashEntry,tmpFHEPtr);
         tmpFHEPtr = nextFHEPtr;
        }
     }
  
   rm3(theEnv,FactData(theEnv)->FactHashTable,
       sizeof(struct factHashEntry *) * FactData(theEnv)->FactHashTableSize);
                 
   tmpFactPtr = FactData(theEnv)->FactList;
   while (tmpFactPtr != NULL)
     {
      nextFactPtr = tmpFactPtr->nextFact;

      theMatch = (struct patternMatch *) tmpFactPtr->list;        
      while (theMatch != NULL)
        {
         tmpMatch = theMatch->next;
         rtn_struct(theEnv,patternMatch,theMatch);
         theMatch = tmpMatch;
        }

      ReturnEntityDependencies(theEnv,(struct patternEntity *) tmpFactPtr);

      ReturnFact(theEnv,tmpFactPtr);
      tmpFactPtr = nextFactPtr; 
     }
     
   tmpFactPtr = FactData(theEnv)->GarbageFacts;
   while (tmpFactPtr != NULL)
     {
      nextFactPtr = tmpFactPtr->nextFact;

      ReturnFact(theEnv,tmpFactPtr);
      tmpFactPtr = nextFactPtr; 
     }
  }

/**********************************************/
/* PrintFactWithIdentifier: Displays a single */
/*   fact preceded by its fact identifier.    */
/**********************************************/
globle void PrintFactWithIdentifier(
  void *theEnv,
  char *logicalName,
  struct fact *factPtr)
  {
   char printSpace[20];

   gensprintf(printSpace,"f-%-5lld ",factPtr->factIndex);
   EnvPrintRouter(theEnv,logicalName,printSpace);
   PrintFact(theEnv,logicalName,factPtr,FALSE,FALSE);
  }

/****************************************************/
/* PrintFactIdentifier: Displays a fact identifier. */
/****************************************************/
globle void PrintFactIdentifier(
  void *theEnv,
  char *logicalName,
  void *factPtr)
  {
   char printSpace[20];

   gensprintf(printSpace,"f-%lld",((struct fact *) factPtr)->factIndex);
   EnvPrintRouter(theEnv,logicalName,printSpace);
  }

/********************************************/
/* PrintFactIdentifierInLongForm: Display a */
/*   fact identifier in a longer format.    */
/********************************************/
globle void PrintFactIdentifierInLongForm(
  void *theEnv,
  char *logicalName,
  void *factPtr)
  {
   if (PrintUtilityData(theEnv)->AddressesToStrings) EnvPrintRouter(theEnv,logicalName,"\"");
   if (factPtr != (void *) &FactData(theEnv)->DummyFact)
     {
      EnvPrintRouter(theEnv,logicalName,"<Fact-");
      PrintLongInteger(theEnv,logicalName,((struct fact *) factPtr)->factIndex);
      EnvPrintRouter(theEnv,logicalName,">");
     }
   else
     { EnvPrintRouter(theEnv,logicalName,"<Dummy Fact>"); }

   if (PrintUtilityData(theEnv)->AddressesToStrings) EnvPrintRouter(theEnv,logicalName,"\"");
  }

/*******************************************/
/* DecrementFactBasisCount: Decrements the */
/*   partial match busy count of a fact    */
/*******************************************/
globle void DecrementFactBasisCount(
  void *theEnv,
  void *vFactPtr)
  {
   struct fact *factPtr = (struct fact *) vFactPtr;
   struct multifield *theSegment;
   int i;

   EnvDecrementFactCount(theEnv,factPtr);

   theSegment = &factPtr->theProposition;

   for (i = 0 ; i < (int) theSegment->multifieldLength ; i++)
     {
      AtomDeinstall(theEnv,theSegment->theFields[i].type,theSegment->theFields[i].value);
     }
  }

/*******************************************/
/* IncrementFactBasisCount: Increments the */
/*   partial match busy count of a fact.   */
/*******************************************/
globle void IncrementFactBasisCount(
  void *theEnv,
  void *vFactPtr)
  {
   struct fact *factPtr = (struct fact *) vFactPtr;
   struct multifield *theSegment;
   int i;

   EnvIncrementFactCount(theEnv,factPtr);

   theSegment = &factPtr->theProposition;

   for (i = 0 ; i < (int) theSegment->multifieldLength ; i++)
     {
      AtomInstall(theEnv,theSegment->theFields[i].type,theSegment->theFields[i].value);
     }
  }

/**************************************************/
/* PrintFact: Displays the printed representation */
/*   of a fact containing the relation name and   */
/*   all of the fact's slots or fields.           */
/**************************************************/
globle void PrintFact(
  void *theEnv,
  char *logicalName,
  struct fact *factPtr,
  int seperateLines,
  int ignoreDefaults)
  {
   struct multifield *theMultifield;

   /*=========================================*/
   /* Print a deftemplate (non-ordered) fact. */
   /*=========================================*/

   if (factPtr->whichDeftemplate->implied == FALSE)
     {
      PrintTemplateFact(theEnv,logicalName,factPtr,seperateLines,ignoreDefaults);
      return;
     }

   /*==============================*/
   /* Print an ordered fact (which */
   /* has an implied deftemplate). */
   /*==============================*/

   EnvPrintRouter(theEnv,logicalName,"(");

   EnvPrintRouter(theEnv,logicalName,factPtr->whichDeftemplate->header.name->contents);

   theMultifield = (struct multifield *) factPtr->theProposition.theFields[0].value;
   if (theMultifield->multifieldLength != 0)
     {
      EnvPrintRouter(theEnv,logicalName," ");
      PrintMultifield(theEnv,logicalName,theMultifield,0,
                      (long) (theMultifield->multifieldLength - 1),
                      FALSE);
     }

   EnvPrintRouter(theEnv,logicalName,")");
  }

/*********************************************/
/* MatchFactFunction: Filters a fact through */
/*   the appropriate fact pattern network.   */
/*********************************************/
globle void MatchFactFunction(
  void *theEnv,
  void *vTheFact)
  {
   struct fact *theFact = (struct fact *) vTheFact;

   FactPatternMatch(theEnv,theFact,theFact->whichDeftemplate->patternNetwork,0,NULL,NULL);
  }

/*********************************************************/
/* EnvRetract: C access routine for the retract command. */
/*********************************************************/
globle intBool EnvRetract(
  void *theEnv,
  void *vTheFact)
  {
   struct fact *theFact = (struct fact *) vTheFact;
   struct deftemplate *theTemplate = theFact->whichDeftemplate;

   /*===========================================*/
   /* A fact can not be retracted while another */
   /* fact is being asserted or retracted.      */
   /*===========================================*/

   if (EngineData(theEnv)->JoinOperationInProgress)
     {
      PrintErrorID(theEnv,"FACTMNGR",1,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Facts may not be retracted during pattern-matching\n");
      return(FALSE);
     }

   /*====================================*/
   /* A NULL fact pointer indicates that */
   /* all facts should be retracted.     */
   /*====================================*/

   if (theFact == NULL)
     {
      RemoveAllFacts(theEnv);
      return(TRUE);
     }

   /*======================================================*/
   /* Check to see if the fact has already been retracted. */
   /*======================================================*/

   if (theFact->garbage) return(FALSE);

   /*============================*/
   /* Print retraction output if */
   /* facts are being watched.   */
   /*============================*/

#if DEBUGGING_FUNCTIONS
   if (theFact->whichDeftemplate->watch)
     {
      EnvPrintRouter(theEnv,WTRACE,"<== ");
      PrintFactWithIdentifier(theEnv,WTRACE,theFact);
      EnvPrintRouter(theEnv,WTRACE,"\n");
     }
#endif

   /*==================================*/
   /* Set the change flag to indicate  */
   /* the fact-list has been modified. */
   /*==================================*/

   FactData(theEnv)->ChangeToFactList = TRUE;

   /*===============================================*/
   /* Remove any links between the fact and partial */
   /* matches in the join network. These links are  */
   /* used to keep track of logical dependencies.   */
   /*===============================================*/

   RemoveEntityDependencies(theEnv,(struct patternEntity *) theFact);

   /*===========================================*/
   /* Remove the fact from the fact hash table. */
   /*===========================================*/

   RemoveHashedFact(theEnv,theFact);

   /*=========================================*/
   /* Remove the fact from its template list. */
   /*=========================================*/
   
   if (theFact == theTemplate->lastFact)
     { theTemplate->lastFact = theFact->previousTemplateFact; }

   if (theFact->previousTemplateFact == NULL)
     {
      theTemplate->factList = theTemplate->factList->nextTemplateFact;
      if (theTemplate->factList != NULL)
        { theTemplate->factList->previousTemplateFact = NULL; }
     }
   else
     {
      theFact->previousTemplateFact->nextTemplateFact = theFact->nextTemplateFact;
      if (theFact->nextTemplateFact != NULL)
        { theFact->nextTemplateFact->previousTemplateFact = theFact->previousTemplateFact; }
     }
  
   /*=====================================*/
   /* Remove the fact from the fact list. */
   /*=====================================*/

   if (theFact == FactData(theEnv)->LastFact)
     { FactData(theEnv)->LastFact = theFact->previousFact; }

   if (theFact->previousFact == NULL)
     {
      FactData(theEnv)->FactList = FactData(theEnv)->FactList->nextFact;
      if (FactData(theEnv)->FactList != NULL)
        { FactData(theEnv)->FactList->previousFact = NULL; }
     }
   else
     {
      theFact->previousFact->nextFact = theFact->nextFact;
      if (theFact->nextFact != NULL)
        { theFact->nextFact->previousFact = theFact->previousFact; }
     }

   /*==================================*/
   /* Update busy counts and ephemeral */
   /* garbage information.             */
   /*==================================*/

   FactDeinstall(theEnv,theFact);
   UtilityData(theEnv)->EphemeralItemCount++;
   UtilityData(theEnv)->EphemeralItemSize += sizeof(struct fact) + (sizeof(struct field) * theFact->theProposition.multifieldLength);

   /*========================================*/
   /* Add the fact to the fact garbage list. */
   /*========================================*/

   theFact->nextFact = FactData(theEnv)->GarbageFacts;
   FactData(theEnv)->GarbageFacts = theFact;
   theFact->garbage = TRUE;

   /*===================================================*/
   /* Reset the evaluation error flag since expressions */
   /* will be evaluated as part of the retract.         */
   /*===================================================*/

   SetEvaluationError(theEnv,FALSE);

   /*===========================================*/
   /* Loop through the list of all the patterns */
   /* that matched the fact and process the     */
   /* retract operation for each one.           */
   /*===========================================*/

   EngineData(theEnv)->JoinOperationInProgress = TRUE;
   NetworkRetract(theEnv,(struct patternMatch *) theFact->list);
   EngineData(theEnv)->JoinOperationInProgress = FALSE;

   /*=========================================*/
   /* Free partial matches that were released */
   /* by the retraction of the fact.          */
   /*=========================================*/

   if (EngineData(theEnv)->ExecutingRule == NULL)
     { FlushGarbagePartialMatches(theEnv); }

   /*=========================================*/
   /* Retract other facts that were logically */
   /* dependent on the fact just retracted.   */
   /*=========================================*/

   ForceLogicalRetractions(theEnv);

   /*===========================================*/
   /* Force periodic cleanup if the retract was */
   /* executed from an embedded application.    */
   /*===========================================*/

   if ((EvaluationData(theEnv)->CurrentEvaluationDepth == 0) && (! CommandLineData(theEnv)->EvaluatingTopLevelCommand) &&
       (EvaluationData(theEnv)->CurrentExpression == NULL))
     { PeriodicCleanup(theEnv,TRUE,FALSE); }

   /*==================================*/
   /* Return TRUE to indicate the fact */
   /* was successfully retracted.      */
   /*==================================*/

   return(TRUE);
  }

/*******************************************************************/
/* RemoveGarbageFacts: Returns facts that have been retracted to   */
/*   the pool of available memory. It is necessary to postpone     */
/*   returning the facts to memory because RHS actions retrieve    */
/*   their variable bindings directly from the fact data structure */
/*   and the facts may be in use in other data structures.         */
/*******************************************************************/
static void RemoveGarbageFacts(
  void *theEnv)
  {
   struct fact *factPtr, *nextPtr, *lastPtr = NULL;

   factPtr = FactData(theEnv)->GarbageFacts;

   while (factPtr != NULL)
     {
      nextPtr = factPtr->nextFact;
      if ((factPtr->factHeader.busyCount == 0) &&
          (((int) factPtr->depth) > EvaluationData(theEnv)->CurrentEvaluationDepth))
        {
         UtilityData(theEnv)->EphemeralItemCount--;
         UtilityData(theEnv)->EphemeralItemSize -= sizeof(struct fact) + (sizeof(struct field) * factPtr->theProposition.multifieldLength);
         ReturnFact(theEnv,factPtr);
         if (lastPtr == NULL) FactData(theEnv)->GarbageFacts = nextPtr;
         else lastPtr->nextFact = nextPtr;
        }
      else
        { lastPtr = factPtr; }

      factPtr = nextPtr;
     }
  }

/********************************************************/
/* EnvAssert: C access routine for the assert function. */
/********************************************************/
globle void *EnvAssert(
  void *theEnv,
  void *vTheFact)
  {
   unsigned long hashValue;
   unsigned long length, i;
   struct field *theField;
   struct fact *theFact = (struct fact *) vTheFact;
   intBool duplicate;

   /*==========================================*/
   /* A fact can not be asserted while another */
   /* fact is being asserted or retracted.     */
   /*==========================================*/

   if (EngineData(theEnv)->JoinOperationInProgress)
     {
      ReturnFact(theEnv,theFact);
      PrintErrorID(theEnv,"FACTMNGR",2,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Facts may not be asserted during pattern-matching\n");
      return(NULL);
     }

   /*=============================================================*/
   /* Replace invalid data types in the fact with the symbol nil. */
   /*=============================================================*/

   length = theFact->theProposition.multifieldLength;
   theField = theFact->theProposition.theFields;

   for (i = 0; i < length; i++)
     {
      if (theField[i].type == RVOID)
        {
         theField[i].type = SYMBOL;
         theField[i].value = (void *) EnvAddSymbol(theEnv,"nil");
        }
     }

   /*========================================================*/
   /* If fact assertions are being checked for duplications, */
   /* then search the fact list for a duplicate fact.        */
   /*========================================================*/

   hashValue = HandleFactDuplication(theEnv,theFact,&duplicate);
   if (duplicate) return(NULL);

   /*==========================================================*/
   /* If necessary, add logical dependency links between the   */
   /* fact and the partial match which is its logical support. */
   /*==========================================================*/

   if (AddLogicalDependencies(theEnv,(struct patternEntity *) theFact,FALSE) == FALSE)
     {
      ReturnFact(theEnv,theFact);
      return(NULL);
     }

   /*======================================*/
   /* Add the fact to the fact hash table. */
   /*======================================*/

   AddHashedFact(theEnv,theFact,hashValue);

   /*================================*/
   /* Add the fact to the fact list. */
   /*================================*/

   theFact->nextFact = NULL;
   theFact->list = NULL;
   theFact->previousFact = FactData(theEnv)->LastFact;
   if (FactData(theEnv)->LastFact == NULL)
     { FactData(theEnv)->FactList = theFact; }
   else
     { FactData(theEnv)->LastFact->nextFact = theFact; }
   FactData(theEnv)->LastFact = theFact;

   /*====================================*/
   /* Add the fact to its template list. */
   /*====================================*/
   
   theFact->previousTemplateFact = theFact->whichDeftemplate->lastFact;
   theFact->nextTemplateFact = NULL;
   
   if (theFact->whichDeftemplate->lastFact == NULL)
     { theFact->whichDeftemplate->factList = theFact; }
   else
     { theFact->whichDeftemplate->lastFact->nextTemplateFact = theFact; }
     
   theFact->whichDeftemplate->lastFact = theFact;
   
   /*==================================*/
   /* Set the fact index and time tag. */
   /*==================================*/

   theFact->factIndex = FactData(theEnv)->NextFactIndex++;
   theFact->factHeader.timeTag = DefruleData(theEnv)->CurrentEntityTimeTag++;

   /*=====================*/
   /* Update busy counts. */
   /*=====================*/

   FactInstall(theEnv,theFact);

   /*==========================*/
   /* Print assert output if   */
   /* facts are being watched. */
   /*==========================*/

#if DEBUGGING_FUNCTIONS
   if (theFact->whichDeftemplate->watch)
     {
      EnvPrintRouter(theEnv,WTRACE,"==> ");
      PrintFactWithIdentifier(theEnv,WTRACE,theFact);
      EnvPrintRouter(theEnv,WTRACE,"\n");
     }
#endif

   /*==================================*/
   /* Set the change flag to indicate  */
   /* the fact-list has been modified. */
   /*==================================*/

   FactData(theEnv)->ChangeToFactList = TRUE;

   /*==========================================*/
   /* Check for constraint errors in the fact. */
   /*==========================================*/

   CheckTemplateFact(theEnv,theFact);

   /*===================================================*/
   /* Reset the evaluation error flag since expressions */
   /* will be evaluated as part of the assert .         */
   /*===================================================*/

   SetEvaluationError(theEnv,FALSE);

   /*=============================================*/
   /* Pattern match the fact using the associated */
   /* deftemplate's pattern network.              */
   /*=============================================*/

   EngineData(theEnv)->JoinOperationInProgress = TRUE;
   FactPatternMatch(theEnv,theFact,theFact->whichDeftemplate->patternNetwork,0,NULL,NULL);
   EngineData(theEnv)->JoinOperationInProgress = FALSE;

   /*===================================================*/
   /* Retract other facts that were logically dependent */
   /* on the non-existence of the fact just asserted.   */
   /*===================================================*/

   ForceLogicalRetractions(theEnv);

   /*=========================================*/
   /* Free partial matches that were released */
   /* by the assertion of the fact.           */
   /*=========================================*/

   if (EngineData(theEnv)->ExecutingRule == NULL) FlushGarbagePartialMatches(theEnv);

   /*==========================================*/
   /* Force periodic cleanup if the assert was */
   /* executed from an embedded application.   */
   /*==========================================*/

   if ((EvaluationData(theEnv)->CurrentEvaluationDepth == 0) && (! CommandLineData(theEnv)->EvaluatingTopLevelCommand) &&
       (EvaluationData(theEnv)->CurrentExpression == NULL))
     { PeriodicCleanup(theEnv,TRUE,FALSE); }

   /*===============================*/
   /* Return a pointer to the fact. */
   /*===============================*/

   return((void *) theFact);
  }

/**************************************/
/* RemoveAllFacts: Loops through the  */
/*   fact-list and removes each fact. */
/**************************************/
globle void RemoveAllFacts(
  void *theEnv)
  {
   while (FactData(theEnv)->FactList != NULL)
     { EnvRetract(theEnv,(void *) FactData(theEnv)->FactList); }
  }

/************************************************/
/* EnvCreateFact: Creates a fact data structure */
/*   of the specified deftemplate.              */
/************************************************/
globle struct fact *EnvCreateFact(
  void *theEnv,
  void *vTheDeftemplate)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   struct fact *newFact;
   int i;

   /*=================================*/
   /* A deftemplate must be specified */
   /* in order to create a fact.      */
   /*=================================*/

   if (theDeftemplate == NULL) return(NULL);

   /*============================================*/
   /* Create a fact for an explicit deftemplate. */
   /*============================================*/

   if (theDeftemplate->implied == FALSE)
     {
      newFact = CreateFactBySize(theEnv,theDeftemplate->numberOfSlots);
      for (i = 0;
           i < (int) theDeftemplate->numberOfSlots;
           i++)
        { newFact->theProposition.theFields[i].type = RVOID; }
     }

   /*===========================================*/
   /* Create a fact for an implied deftemplate. */
   /*===========================================*/

   else
     {
      newFact = CreateFactBySize(theEnv,1);
      newFact->theProposition.theFields[0].type = MULTIFIELD;
      newFact->theProposition.theFields[0].value = CreateMultifield2(theEnv,0L);
     }

   /*===============================*/
   /* Return a pointer to the fact. */
   /*===============================*/

   newFact->whichDeftemplate = theDeftemplate;

   return(newFact);
  }

/******************************************/
/* EnvGetFactSlot: Returns the slot value */
/*   from the specified slot of a fact.   */
/******************************************/
globle intBool EnvGetFactSlot(
  void *theEnv,
  void *vTheFact,
  char *slotName,
  DATA_OBJECT *theValue)
  {
   struct fact *theFact = (struct fact *) vTheFact;
   struct deftemplate *theDeftemplate;
   short whichSlot;

   /*===============================================*/
   /* Get the deftemplate associated with the fact. */
   /*===============================================*/

   theDeftemplate = theFact->whichDeftemplate;

   /*==============================================*/
   /* Handle retrieving the slot value from a fact */
   /* having an implied deftemplate. An implied    */
   /* facts has a single multifield slot.          */
   /*==============================================*/

   if (theDeftemplate->implied)
     {
      if (slotName != NULL) return(FALSE);
      theValue->type = theFact->theProposition.theFields[0].type;
      theValue->value = theFact->theProposition.theFields[0].value;
      SetpDOBegin(theValue,1);
      SetpDOEnd(theValue,((struct multifield *) theValue->value)->multifieldLength);
      return(TRUE);
     }

   /*===================================*/
   /* Make sure the slot name requested */
   /* corresponds to a valid slot name. */
   /*===================================*/

   if (FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&whichSlot) == NULL)
     { return(FALSE); }

   /*======================================================*/
   /* Return the slot value. If the slot value wasn't set, */
   /* then return FALSE to indicate that an appropriate    */
   /* slot value wasn't available.                         */
   /*======================================================*/

   theValue->type = theFact->theProposition.theFields[whichSlot-1].type;
   theValue->value = theFact->theProposition.theFields[whichSlot-1].value;
   if (theValue->type == MULTIFIELD)
     {
      SetpDOBegin(theValue,1);
      SetpDOEnd(theValue,((struct multifield *) theValue->value)->multifieldLength);
     }

   if (theValue->type == RVOID) return(FALSE);

   return(TRUE);
  }

/****************************************/
/* GetFactSlot: Returns the slot value  */
/*   from the specified slot of a fact. */
/****************************************/
#if ALLOW_ENVIRONMENT_GLOBALS
globle intBool GetFactSlot(
  void *vTheFact,
  char *slotName,
  DATA_OBJECT *theValue)
  {
   return(EnvGetFactSlot(GetCurrentEnvironment(),vTheFact,slotName,theValue));
  }
#endif

/***************************************/
/* EnvPutFactSlot: Sets the slot value */
/*   of the specified slot of a fact.  */
/***************************************/
globle intBool EnvPutFactSlot(
  void *theEnv,
  void *vTheFact,
  char *slotName,
  DATA_OBJECT *theValue)
  {
   struct fact *theFact = (struct fact *) vTheFact;
   struct deftemplate *theDeftemplate;
   struct templateSlot *theSlot;
   short whichSlot;

   /*===============================================*/
   /* Get the deftemplate associated with the fact. */
   /*===============================================*/

   theDeftemplate = theFact->whichDeftemplate;

   /*============================================*/
   /* Handle setting the slot value of a fact    */
   /* having an implied deftemplate. An implied  */
   /* facts has a single multifield slot.        */
   /*============================================*/

   if (theDeftemplate->implied)
     {
      if ((slotName != NULL) || (theValue->type != MULTIFIELD))
        { return(FALSE); }

      if (theFact->theProposition.theFields[0].type == MULTIFIELD)
        { ReturnMultifield(theEnv,(struct multifield *) theFact->theProposition.theFields[0].value); }

      theFact->theProposition.theFields[0].type = theValue->type;
      theFact->theProposition.theFields[0].value = DOToMultifield(theEnv,theValue);
      
      return(TRUE);
     }

   /*===================================*/
   /* Make sure the slot name requested */
   /* corresponds to a valid slot name. */
   /*===================================*/

   if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&whichSlot)) == NULL)
     { return(FALSE); }

   /*=============================================*/
   /* Make sure a single field value is not being */
   /* stored in a multifield slot or vice versa.  */
   /*=============================================*/

   if (((theSlot->multislot == 0) && (theValue->type == MULTIFIELD)) ||
       ((theSlot->multislot == 1) && (theValue->type != MULTIFIELD)))
     { return(FALSE); }

   /*=====================*/
   /* Set the slot value. */
   /*=====================*/

   if (theFact->theProposition.theFields[whichSlot-1].type == MULTIFIELD)
     { ReturnMultifield(theEnv,(struct multifield *) theFact->theProposition.theFields[whichSlot-1].value); }

   theFact->theProposition.theFields[whichSlot-1].type = theValue->type;

   if (theValue->type == MULTIFIELD)
     { theFact->theProposition.theFields[whichSlot-1].value = DOToMultifield(theEnv,theValue); }
   else
     { theFact->theProposition.theFields[whichSlot-1].value = theValue->value; }
   
   return(TRUE);
  }

/********************************************************/
/* EnvAssignFactSlotDefaults: Sets a fact's slot values */
/*   to its default value if the value of the slot has  */
/*   not yet been set.                                  */
/********************************************************/
globle intBool EnvAssignFactSlotDefaults(
  void *theEnv,
  void *vTheFact)
  {
   struct fact *theFact = (struct fact *) vTheFact;
   struct deftemplate *theDeftemplate;
   struct templateSlot *slotPtr;
   int i;
   DATA_OBJECT theResult;

   /*===============================================*/
   /* Get the deftemplate associated with the fact. */
   /*===============================================*/

   theDeftemplate = theFact->whichDeftemplate;

   /*================================================*/
   /* The value for the implied multifield slot of   */
   /* an implied deftemplate is set to a multifield  */
   /* of length zero when the fact is created.       */
   /*================================================*/

   if (theDeftemplate->implied) return(TRUE);

   /*============================================*/
   /* Loop through each slot of the deftemplate. */
   /*============================================*/

   for (i = 0, slotPtr = theDeftemplate->slotList;
        i < (int) theDeftemplate->numberOfSlots;
        i++, slotPtr = slotPtr->next)
     {
      /*===================================*/
      /* If the slot's value has been set, */
      /* then move on to the next slot.    */
      /*===================================*/

      if (theFact->theProposition.theFields[i].type != RVOID) continue;

      /*======================================================*/
      /* Assign the default value for the slot if one exists. */
      /*======================================================*/
      
      if (DeftemplateSlotDefault(theEnv,theDeftemplate,slotPtr,&theResult,FALSE))
        {
         theFact->theProposition.theFields[i].type = theResult.type;
         theFact->theProposition.theFields[i].value = theResult.value;
        }
     }

   /*==========================================*/
   /* Return TRUE to indicate that the default */
   /* values have been successfully set.       */
   /*==========================================*/

   return(TRUE);
  }
  
/********************************************************/
/* DeftemplateSlotDefault: Determines the default value */
/*   for the specified slot of a deftemplate.           */
/********************************************************/
globle intBool DeftemplateSlotDefault(
  void *theEnv,
  struct deftemplate *theDeftemplate,
  struct templateSlot *slotPtr,
  DATA_OBJECT *theResult,
  int garbageMultifield)
  {
   /*================================================*/
   /* The value for the implied multifield slot of an */
   /* implied deftemplate does not have a default.    */
   /*=================================================*/

   if (theDeftemplate->implied) return(FALSE);

   /*===============================================*/
   /* If the (default ?NONE) attribute was declared */
   /* for the slot, then return FALSE to indicate   */
   /* the default values for the fact couldn't be   */
   /* supplied since this attribute requires that a */
   /* default value can't be used for the slot.     */
   /*===============================================*/

   if (slotPtr->noDefault) return(FALSE);

   /*==============================================*/
   /* Otherwise if a static default was specified, */
   /* use this as the default value.               */
   /*==============================================*/

   else if (slotPtr->defaultPresent)
     {
      if (slotPtr->multislot)
        {
         StoreInMultifield(theEnv,theResult,slotPtr->defaultList,garbageMultifield);
        }
      else
        {
         theResult->type = slotPtr->defaultList->type;
         theResult->value = slotPtr->defaultList->value;
        }
     }

   /*================================================*/
   /* Otherwise if a dynamic-default was specified,  */
   /* evaluate it and use this as the default value. */
   /*================================================*/

   else if (slotPtr->defaultDynamic)
     {
      if (! EvaluateAndStoreInDataObject(theEnv,(int) slotPtr->multislot,
                                         (EXPRESSION *) slotPtr->defaultList,
                                         theResult,garbageMultifield))
        { return(FALSE); }
     }

   /*====================================*/
   /* Otherwise derive the default value */
   /* from the slot's constraints.       */
   /*====================================*/

   else
     {
      DeriveDefaultFromConstraints(theEnv,slotPtr->constraints,theResult,
                                  (int) slotPtr->multislot,garbageMultifield);
     }

   /*==========================================*/
   /* Return TRUE to indicate that the default */
   /* values have been successfully set.       */
   /*==========================================*/

   return(TRUE);
  }

/***************************************************************/
/* CopyFactSlotValues: Copies the slot values from one fact to */
/*   another. Both facts must have the same relation name.     */
/***************************************************************/
globle intBool CopyFactSlotValues(
  void *theEnv,
  void *vTheDestFact,
  void *vTheSourceFact)
  {
   struct fact *theDestFact = (struct fact *) vTheDestFact;
   struct fact *theSourceFact = (struct fact *) vTheSourceFact;
   struct deftemplate *theDeftemplate;
   struct templateSlot *slotPtr;
   int i;

   /*===================================*/
   /* Both facts must be the same type. */
   /*===================================*/

   theDeftemplate = theSourceFact->whichDeftemplate;
   if (theDestFact->whichDeftemplate != theDeftemplate)
     { return(FALSE); }

   /*===================================================*/
   /* Loop through each slot of the deftemplate copying */
   /* the source fact value to the destination fact.    */
   /*===================================================*/

   for (i = 0, slotPtr = theDeftemplate->slotList;
        i < (int) theDeftemplate->numberOfSlots;
        i++, slotPtr = slotPtr->next)
     {
      theDestFact->theProposition.theFields[i].type =
         theSourceFact->theProposition.theFields[i].type;
      if (theSourceFact->theProposition.theFields[i].type != MULTIFIELD)
        {
         theDestFact->theProposition.theFields[i].value =
           theSourceFact->theProposition.theFields[i].value;
        }
      else
        {
         theDestFact->theProposition.theFields[i].value =
           CopyMultifield(theEnv,(struct multifield *) theSourceFact->theProposition.theFields[i].value);
        }
     }

   /*========================================*/
   /* Return TRUE to indicate that fact slot */
   /* values were successfully copied.       */
   /*========================================*/

   return(TRUE);
  }

/*********************************************/
/* CreateFactBySize: Allocates a fact data   */
/*   structure based on the number of slots. */
/*********************************************/
globle struct fact *CreateFactBySize(
  void *theEnv,
  unsigned size)
  {
   struct fact *theFact;
   unsigned newSize;

   if (size <= 0) newSize = 1;
   else newSize = size;

   theFact = get_var_struct(theEnv,fact,sizeof(struct field) * (newSize - 1));

   theFact->depth = (unsigned) EvaluationData(theEnv)->CurrentEvaluationDepth;
   theFact->garbage = FALSE;
   theFact->factIndex = 0LL;
   theFact->factHeader.busyCount = 0;
   theFact->factHeader.theInfo = &FactData(theEnv)->FactInfo;
   theFact->factHeader.dependents = NULL;
   theFact->whichDeftemplate = NULL;
   theFact->nextFact = NULL;
   theFact->previousFact = NULL;
   theFact->previousTemplateFact = NULL;
   theFact->nextTemplateFact = NULL;
   theFact->list = NULL;

   theFact->theProposition.multifieldLength = size;
   theFact->theProposition.depth = (short) EvaluationData(theEnv)->CurrentEvaluationDepth;
   theFact->theProposition.busyCount = 0;

   return(theFact);
  }

/*********************************************/
/* ReturnFact: Returns a fact data structure */
/*   to the pool of free memory.             */
/*********************************************/
globle void ReturnFact(
  void *theEnv,
  struct fact *theFact)
  {
   struct multifield *theSegment, *subSegment;
   long newSize, i;

   theSegment = &theFact->theProposition;

   for (i = 0; i < theSegment->multifieldLength; i++)
     {
      if (theSegment->theFields[i].type == MULTIFIELD)
        {
         subSegment = (struct multifield *) theSegment->theFields[i].value;
         if (subSegment->busyCount == 0)
           { ReturnMultifield(theEnv,subSegment); }
         else
           { AddToMultifieldList(theEnv,subSegment); }
        }
     }

   if (theFact->theProposition.multifieldLength == 0) newSize = 1;
   else newSize = theFact->theProposition.multifieldLength;
      
   rtn_var_struct(theEnv,fact,sizeof(struct field) * (newSize - 1),theFact);
  }

/*************************************************************/
/* FactInstall: Increments the fact, deftemplate, and atomic */
/*   data value busy counts associated with the fact.        */
/*************************************************************/
globle void FactInstall(
  void *theEnv,
  struct fact *newFact)
  {
   struct multifield *theSegment;
   int i;

   FactData(theEnv)->NumberOfFacts++;
   newFact->whichDeftemplate->busyCount++;
   theSegment = &newFact->theProposition;

   for (i = 0 ; i < (int) theSegment->multifieldLength ; i++)
     {
      AtomInstall(theEnv,theSegment->theFields[i].type,theSegment->theFields[i].value);
     }

   newFact->factHeader.busyCount++;
  }

/***************************************************************/
/* FactDeinstall: Decrements the fact, deftemplate, and atomic */
/*   data value busy counts associated with the fact.          */
/***************************************************************/
globle void FactDeinstall(
  void *theEnv,
  struct fact *newFact)
  {
   struct multifield *theSegment;
   int i;

   FactData(theEnv)->NumberOfFacts--;
   theSegment = &newFact->theProposition;
   newFact->whichDeftemplate->busyCount--;

   for (i = 0 ; i < (int) theSegment->multifieldLength ; i++)
     {
      AtomDeinstall(theEnv,theSegment->theFields[i].type,theSegment->theFields[i].value);
     }

   newFact->factHeader.busyCount--;
  }

/************************************************/
/* EnvIncrementFactCount: Increments the number */
/*   of references to a specified fact.         */
/************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle void EnvIncrementFactCount(
  void *theEnv,
  void *factPtr)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   ((struct fact *) factPtr)->factHeader.busyCount++;
  }

/************************************************/
/* EnvDecrementFactCount: Decrements the number */
/*   of references to a specified fact.         */
/************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle void EnvDecrementFactCount(
  void *theEnv,
  void *factPtr)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   ((struct fact *) factPtr)->factHeader.busyCount--;
  }

/*********************************************************/
/* EnvGetNextFact: If passed a NULL pointer, returns the */
/*   first fact in the fact-list. Otherwise returns the  */
/*   next fact following the fact passed as an argument. */
/*********************************************************/
globle void *EnvGetNextFact(
  void *theEnv,
  void *factPtr)
  {
   if (factPtr == NULL)
     { return((void *) FactData(theEnv)->FactList); }

   if (((struct fact *) factPtr)->garbage) return(NULL);

   return((void *) ((struct fact *) factPtr)->nextFact);
  }

/**************************************************/
/* GetNextFactInScope: Returns the next fact that */
/*   is in scope of the current module. Works in  */
/*   a similar fashion to GetNextFact, but skips  */
/*   facts that are out of scope.                 */
/**************************************************/
globle void *GetNextFactInScope(
  void *theEnv,
  void *vTheFact)
  {
   struct fact *theFact = (struct fact *) vTheFact;

   /*=======================================================*/
   /* If fact passed as an argument is a NULL pointer, then */
   /* we're just beginning a traversal of the fact list. If */
   /* the module index has changed since that last time the */
   /* fact list was traversed by this routine, then         */
   /* determine all of the deftemplates that are in scope   */
   /* of the current module.                                */
   /*=======================================================*/

   if (theFact == NULL)
     {
      theFact = FactData(theEnv)->FactList;
      if (FactData(theEnv)->LastModuleIndex != DefmoduleData(theEnv)->ModuleChangeIndex)
        {
         UpdateDeftemplateScope(theEnv);
         FactData(theEnv)->LastModuleIndex = DefmoduleData(theEnv)->ModuleChangeIndex;
        }
     }

   /*==================================================*/
   /* Otherwise, if the fact passed as an argument has */
   /* been retracted, then there's no way to determine */
   /* the next fact, so return a NULL pointer.         */
   /*==================================================*/

   else if (((struct fact *) theFact)->garbage)
     { return(NULL); }

   /*==================================================*/
   /* Otherwise, start the search for the next fact in */
   /* scope with the fact immediately following the    */
   /* fact passed as an argument.                      */
   /*==================================================*/

   else
     { theFact = theFact->nextFact; }

   /*================================================*/
   /* Continue traversing the fact-list until a fact */
   /* is found that's associated with a deftemplate  */
   /* that's in scope.                               */
   /*================================================*/

   while (theFact != NULL)
     {
      if (theFact->whichDeftemplate->inScope) return((void *) theFact);

      theFact = theFact->nextFact;
     }

   return(NULL);
  }

/****************************************/
/* EnvGetFactPPForm: Returns the pretty */
/*   print representation of a fact.    */
/****************************************/
globle void EnvGetFactPPForm(
  void *theEnv,
  char *buffer,
  unsigned bufferLength,
  void *theFact)
  {
   OpenStringDestination(theEnv,"FactPPForm",buffer,bufferLength);
   PrintFactWithIdentifier(theEnv,"FactPPForm",(struct fact *) theFact);
   CloseStringDestination(theEnv,"FactPPForm");
  }

/**********************************/
/* EnvFactIndex: C access routine */
/*   for the fact-index function. */
/**********************************/
#if WIN_BTC
#pragma argsused
#endif
globle long long EnvFactIndex(
  void *theEnv,
  void *factPtr)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   return(((struct fact *) factPtr)->factIndex);
  }

/**********************************/
/* FactIndex: C access routine    */
/*   for the fact-index function. */
/**********************************/
#if ALLOW_ENVIRONMENT_GLOBALS
globle long long FactIndex(
  void *factPtr)
  {
   return(EnvFactIndex(GetCurrentEnvironment(),factPtr));
  }
#endif

/*************************************/
/* EnvAssertString: C access routine */
/*   for the assert-string function. */
/*************************************/
globle void *EnvAssertString(
  void *theEnv,
  char *theString)
  {
   struct fact *theFact;

   if ((theFact = StringToFact(theEnv,theString)) == NULL) return(NULL);

   return((void *) EnvAssert(theEnv,(void *) theFact));
  }

/******************************************************/
/* EnvGetFactListChanged: Returns the flag indicating */
/*   whether a change to the fact-list has been made. */
/******************************************************/
globle int EnvGetFactListChanged(
  void *theEnv)
  {
   return(FactData(theEnv)->ChangeToFactList); 
  }

/***********************************************************/
/* EnvSetFactListChanged: Sets the flag indicating whether */
/*   a change to the fact-list has been made.              */
/***********************************************************/
globle void EnvSetFactListChanged(
  void *theEnv,
  int value)
  {
   FactData(theEnv)->ChangeToFactList = value;
  }

/****************************************/
/* GetNumberOfFacts: Returns the number */
/* of facts in the fact-list.           */
/****************************************/
globle unsigned long GetNumberOfFacts(
  void *theEnv)
  {   
   return(FactData(theEnv)->NumberOfFacts); 
  }

/***********************************************************/
/* ResetFacts: Reset function for facts. Sets the starting */
/*   fact index to zero and removes all facts.             */
/***********************************************************/
static void ResetFacts(
  void *theEnv)
  {
   /*====================================*/
   /* Initialize the fact index to zero. */
   /*====================================*/

   FactData(theEnv)->NextFactIndex = 0L;

   /*======================================*/
   /* Remove all facts from the fact list. */
   /*======================================*/

   RemoveAllFacts(theEnv);
  }

/************************************************************/
/* ClearFactsReady: Clear ready function for facts. Returns */
/*   TRUE if facts were successfully removed and the clear  */
/*   command can continue, otherwise FALSE.                 */
/************************************************************/
static int ClearFactsReady(
  void *theEnv)
  {
   /*====================================*/
   /* Initialize the fact index to zero. */
   /*====================================*/

   FactData(theEnv)->NextFactIndex = 0L;

   /*======================================*/
   /* Remove all facts from the fact list. */
   /*======================================*/

   RemoveAllFacts(theEnv);

   /*==============================================*/
   /* If for some reason there are any facts still */
   /* remaining, don't continue with the clear.    */
   /*==============================================*/

   if (EnvGetNextFact(theEnv,NULL) != NULL) return(FALSE);

   /*=============================*/
   /* Return TRUE to indicate the */
   /* clear command can continue. */
   /*=============================*/

   return(TRUE);
  }

/***************************************************/
/* FindIndexedFact: Returns a pointer to a fact in */
/*   the fact list with the specified fact index.  */
/***************************************************/
globle struct fact *FindIndexedFact(
  void *theEnv,
  long long factIndexSought)
  {
   struct fact *theFact;

   for (theFact = (struct fact *) EnvGetNextFact(theEnv,NULL);
        theFact != NULL;
        theFact = (struct fact *) EnvGetNextFact(theEnv,theFact))
     {
      if (theFact->factIndex == factIndexSought)
        { return(theFact); }
     }

   return(NULL);
  }

#endif /* DEFTEMPLATE_CONSTRUCT && DEFRULE_CONSTRUCT */

