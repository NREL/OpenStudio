   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  07/01/05            */
   /*                                                     */
   /*             DEFTEMPLATE FUNCTIONS MODULE            */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the modify and duplicate functions.   */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Added deftemplate-slot-names,                  */
/*            deftemplate-slot-default-value,                */
/*            deftemplate-slot-cardinality,                  */
/*            deftemplate-slot-allowed-values,               */
/*            deftemplate-slot-range,                        */
/*            deftemplate-slot-types,                        */
/*            deftemplate-slot-multip,                       */
/*            deftemplate-slot-singlep,                      */
/*            deftemplate-slot-existp, and                   */
/*            deftemplate-slot-defaultp functions.           */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#define _TMPLTFUN_SOURCE_

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <string.h>

#include "constant.h"
#include "memalloc.h"
#include "symbol.h"
#include "scanner.h"
#include "exprnpsr.h"
#include "envrnmnt.h"
#include "argacces.h"
#include "router.h"
#include "cstrnchk.h"
#include "default.h"
#include "factmngr.h"
#include "commline.h"
#include "factrhs.h"
#include "modulutl.h"
#include "reorder.h"
#include "sysdep.h"
#include "tmpltdef.h"
#include "tmpltlhs.h"
#include "tmpltutl.h"
#include "tmpltrhs.h"

#include "tmpltfun.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                    DuplicateModifyCommand(void *,int,DATA_OBJECT_PTR);
   static SYMBOL_HN              *CheckDeftemplateAndSlotArguments(void *,char *,struct deftemplate **,int);

#if (! RUN_TIME) && (! BLOAD_ONLY)
   static struct expr            *ModAndDupParse(void *,struct expr *,char *,char *);
   static SYMBOL_HN              *FindTemplateForFactAddress(SYMBOL_HN *,struct lhsParseNode *);
#endif

/****************************************************************/
/* DeftemplateFunctions: Initializes the deftemplate functions. */
/****************************************************************/
globle void DeftemplateFunctions(
  void *theEnv)
  {
#if ! RUN_TIME
   EnvDefineFunction(theEnv,"modify",'u', PTIEF ModifyCommand,"ModifyCommand");
   EnvDefineFunction(theEnv,"duplicate",'u', PTIEF DuplicateCommand,"DuplicateCommand");

   EnvDefineFunction2(theEnv,"deftemplate-slot-names",'u', PTIEF DeftemplateSlotNamesFunction,
                   "DeftemplateSlotNamesFunction", "11z");
   EnvDefineFunction2(theEnv,"deftemplate-slot-default-value",'u',PTIEF DeftemplateSlotDefaultValueFunction,
                   "DeftemplateSlotDefaultValueFunction","22w");
   EnvDefineFunction2(theEnv,"deftemplate-slot-cardinality",'u',PTIEF DeftemplateSlotCardinalityFunction,
                   "DeftemplateSlotCardinalityFunction","22w");
   EnvDefineFunction2(theEnv,"deftemplate-slot-allowed-values",'u',PTIEF DeftemplateSlotAllowedValuesFunction,
                   "DeftemplateSlotAllowedValuesFunction","22w");
   EnvDefineFunction2(theEnv,"deftemplate-slot-range",'u',PTIEF DeftemplateSlotRangeFunction,
                   "DeftemplateSlotRangeFunction","22w");
   EnvDefineFunction2(theEnv,"deftemplate-slot-types",'u',PTIEF DeftemplateSlotTypesFunction,
                   "DeftemplateSlotTypesFunction","22w");

   EnvDefineFunction2(theEnv,"deftemplate-slot-multip",'b',PTIEF DeftemplateSlotMultiPFunction,
                   "DeftemplateSlotMultiPFunction","22w");
   EnvDefineFunction2(theEnv,"deftemplate-slot-singlep",'b',PTIEF DeftemplateSlotSinglePFunction,
                   "DeftemplateSlotSinglePFunction","22w");
   EnvDefineFunction2(theEnv,"deftemplate-slot-existp",'b',PTIEF DeftemplateSlotExistPFunction,
                   "DeftemplateSlotExistPFunction","22w");
   EnvDefineFunction2(theEnv,"deftemplate-slot-defaultp",'w',PTIEF DeftemplateSlotDefaultPFunction,
                   "DeftemplateSlotDefaultPFunction","22w");

   EnvDefineFunction2(theEnv,"deftemplate-slot-facet-existp",'b',PTIEF DeftemplateSlotFacetExistPFunction,
                   "DeftemplateSlotFacetExistPFunction","33w");

   EnvDefineFunction2(theEnv,"deftemplate-slot-facet-value",'u',PTIEF DeftemplateSlotFacetValueFunction,
                   "DeftemplateSlotFacetValueFunction","33w");

#if (! BLOAD_ONLY)
   AddFunctionParser(theEnv,"modify",ModifyParse);
   AddFunctionParser(theEnv,"duplicate",DuplicateParse);
#endif
   FuncSeqOvlFlags(theEnv,"modify",FALSE,FALSE);
   FuncSeqOvlFlags(theEnv,"duplicate",FALSE,FALSE);
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
#endif
  }

/*********************************************************************/
/* ModifyCommand: H/L access routine for the modify command. Calls   */
/*   the DuplicateModifyCommand function to perform the actual work. */
/*********************************************************************/
globle void ModifyCommand(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DuplicateModifyCommand(theEnv,TRUE,returnValue);
  }

/***************************************************************************/
/* DuplicateCommand: H/L access routine for the duplicate command. Calls   */
/*   the DuplicateModifyCommand function to perform the actual work.       */
/***************************************************************************/
globle void DuplicateCommand(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DuplicateModifyCommand(theEnv,FALSE,returnValue);
  }

/***************************************************************/
/* DuplicateModifyCommand: Implements the duplicate and modify */
/*   commands. The fact being duplicated or modified is first  */
/*   copied to a new fact. Replacements to the fields of the   */
/*   new fact are then made. If a modify command is being      */
/*   performed, the original fact is retracted. Lastly, the    */
/*   new fact is asserted.                                     */
/***************************************************************/
static void DuplicateModifyCommand(
  void *theEnv,
  int retractIt,
  DATA_OBJECT_PTR returnValue)
  {
   long long factNum;
   struct fact *oldFact, *newFact, *theFact;
   struct expr *testPtr;
   DATA_OBJECT computeResult;
   struct deftemplate *templatePtr;
   struct templateSlot *slotPtr;
   int i, position, found;

   /*===================================================*/
   /* Set the default return value to the symbol FALSE. */
   /*===================================================*/

   SetpType(returnValue,SYMBOL);
   SetpValue(returnValue,EnvFalseSymbol(theEnv));

   /*==================================================*/
   /* Evaluate the first argument which is used to get */
   /* a pointer to the fact to be modified/duplicated. */
   /*==================================================*/

   testPtr = GetFirstArgument();
   EvaluateExpression(theEnv,testPtr,&computeResult);

   /*==============================================================*/
   /* If an integer is supplied, then treat it as a fact-index and */
   /* search the fact-list for the fact with that fact-index.      */
   /*==============================================================*/

   if (computeResult.type == INTEGER)
     {
      factNum = ValueToLong(computeResult.value);
      if (factNum < 0)
        {
         if (retractIt) ExpectedTypeError2(theEnv,"modify",1);
         else ExpectedTypeError2(theEnv,"duplicate",1);
         SetEvaluationError(theEnv,TRUE);
         return;
        }

      oldFact = (struct fact *) EnvGetNextFact(theEnv,NULL);
      while (oldFact != NULL)
        {
         if (oldFact->factIndex == factNum)
           { break; }
         else
           { oldFact = oldFact->nextFact; }
        }

      if (oldFact == NULL)
        {
         char tempBuffer[20];
         gensprintf(tempBuffer,"f-%lld",factNum);
         CantFindItemErrorMessage(theEnv,"fact",tempBuffer);
         return;
        }
     }

   /*==========================================*/
   /* Otherwise, if a pointer is supplied then */
   /* no lookup is required.                   */
   /*==========================================*/

   else if (computeResult.type == FACT_ADDRESS)
     { oldFact = (struct fact *) computeResult.value; }

   /*===========================================*/
   /* Otherwise, the first argument is invalid. */
   /*===========================================*/

   else
     {
      if (retractIt) ExpectedTypeError2(theEnv,"modify",1);
      else ExpectedTypeError2(theEnv,"duplicate",1);
      SetEvaluationError(theEnv,TRUE);
      return;
     }

   /*==================================*/
   /* See if it is a deftemplate fact. */
   /*==================================*/

   templatePtr = oldFact->whichDeftemplate;

   if (templatePtr->implied) return;

   /*================================================================*/
   /* Duplicate the values from the old fact (skipping multifields). */
   /*================================================================*/

   newFact = (struct fact *) CreateFactBySize(theEnv,oldFact->theProposition.multifieldLength);
   newFact->whichDeftemplate = templatePtr;
   for (i = 0; i < (int) oldFact->theProposition.multifieldLength; i++)
     {
      newFact->theProposition.theFields[i].type = oldFact->theProposition.theFields[i].type;
      if (newFact->theProposition.theFields[i].type != MULTIFIELD)
        { newFact->theProposition.theFields[i].value = oldFact->theProposition.theFields[i].value; }
      else
        { newFact->theProposition.theFields[i].value = NULL; }
     }

   /*========================*/
   /* Start replacing slots. */
   /*========================*/

   testPtr = testPtr->nextArg;
   while (testPtr != NULL)
     {
      /*============================================================*/
      /* If the slot identifier is an integer, then the slot was    */
      /* previously identified and its position within the template */
      /* was stored. Otherwise, the position of the slot within the */
      /* deftemplate has to be determined by comparing the name of  */
      /* the slot against the list of slots for the deftemplate.    */
      /*============================================================*/

      if (testPtr->type == INTEGER)
        { position = (int) ValueToLong(testPtr->value); }
      else
        {
         found = FALSE;
         position = 0;
         slotPtr = templatePtr->slotList;
         while (slotPtr != NULL)
           {
            if (slotPtr->slotName == (SYMBOL_HN *) testPtr->value)
              {
               found = TRUE;
               slotPtr = NULL;
              }
            else
              {
               slotPtr = slotPtr->next;
               position++;
              }
           }

         if (! found)
           {
            InvalidDeftemplateSlotMessage(theEnv,ValueToString(testPtr->value),
                                          ValueToString(templatePtr->header.name),TRUE);
            SetEvaluationError(theEnv,TRUE);
            ReturnFact(theEnv,newFact);
            return;
           }
        }

      /*===================================================*/
      /* If a single field slot is being replaced, then... */
      /*===================================================*/

      if (newFact->theProposition.theFields[position].type != MULTIFIELD)
        {
         /*======================================================*/
         /* If the list of values to store in the slot is empty  */
         /* or contains more than one member than an error has   */
         /* occured because a single field slot can only contain */
         /* a single value.                                      */
         /*======================================================*/

         if ((testPtr->argList == NULL) ? TRUE : (testPtr->argList->nextArg != NULL))
           {
            MultiIntoSingleFieldSlotError(theEnv,GetNthSlot(templatePtr,position),templatePtr);
            ReturnFact(theEnv,newFact);
            return;
           }

         /*===================================================*/
         /* Evaluate the expression to be stored in the slot. */
         /*===================================================*/

         EvaluateExpression(theEnv,testPtr->argList,&computeResult);
         SetEvaluationError(theEnv,FALSE);

         /*====================================================*/
         /* If the expression evaluated to a multifield value, */
         /* then an error occured since a multifield value can */
         /* not be stored in a single field slot.              */
         /*====================================================*/

         if (computeResult.type == MULTIFIELD)
           {
            ReturnFact(theEnv,newFact);
            MultiIntoSingleFieldSlotError(theEnv,GetNthSlot(templatePtr,position),templatePtr);
            return;
           }

         /*=============================*/
         /* Store the value in the slot */
         /*=============================*/

         newFact->theProposition.theFields[position].type =
            computeResult.type;
         newFact->theProposition.theFields[position].value =
            computeResult.value;
        }

      /*=================================*/
      /* Else replace a multifield slot. */
      /*=================================*/

      else
        {
         /*======================================*/
         /* Determine the new value of the slot. */
         /*======================================*/

         StoreInMultifield(theEnv,&computeResult,testPtr->argList,FALSE);
         SetEvaluationError(theEnv,FALSE);

         /*=============================*/
         /* Store the value in the slot */
         /*=============================*/

         newFact->theProposition.theFields[position].type =
            computeResult.type;
         newFact->theProposition.theFields[position].value =
            computeResult.value;
        }

      testPtr = testPtr->nextArg;
     }

   /*=====================================*/
   /* Copy the multifield values from the */
   /* old fact that were not replaced.    */
   /*=====================================*/

   for (i = 0; i < (int) oldFact->theProposition.multifieldLength; i++)
     {
      if ((newFact->theProposition.theFields[i].type == MULTIFIELD) &&
          (newFact->theProposition.theFields[i].value == NULL))

        {
         newFact->theProposition.theFields[i].value =
            CopyMultifield(theEnv,(struct multifield *) oldFact->theProposition.theFields[i].value);
        }
     }

   /*======================================*/
   /* Perform the duplicate/modify action. */
   /*======================================*/

   if (retractIt) EnvRetract(theEnv,oldFact);
   theFact = (struct fact *) EnvAssert(theEnv,newFact);

   /*========================================*/
   /* The asserted fact is the return value. */
   /*========================================*/

   if (theFact != NULL)
     {
      SetpDOBegin(returnValue,1);
      SetpDOEnd(returnValue,theFact->theProposition.multifieldLength);
      SetpType(returnValue,FACT_ADDRESS);
      SetpValue(returnValue,(void *) theFact);
     }

   return;
  }
  
/****************************************************/
/* DeftemplateSlotNamesFunction: H/L access routine */
/*   for the deftemplate-slot-names function.       */
/****************************************************/
globle void DeftemplateSlotNamesFunction(
  void *theEnv,
  DATA_OBJECT *returnValue)
  {
   char *deftemplateName;
   struct deftemplate *theDeftemplate;

   /*=============================================*/
   /* Set up the default return value for errors. */
   /*=============================================*/

   returnValue->type = SYMBOL;
   returnValue->value = EnvFalseSymbol(theEnv);

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if (EnvArgCountCheck(theEnv,"deftemplate-slot-names",EXACTLY,1) == -1) return;

   /*=======================================*/
   /* Get the reference to the deftemplate. */
   /*=======================================*/

   deftemplateName = GetConstructName(theEnv,"deftemplate-slot-names","deftemplate name");
   if (deftemplateName == NULL) return;

   theDeftemplate = (struct deftemplate *) EnvFindDeftemplate(theEnv,deftemplateName);
   if (theDeftemplate == NULL)
     {
      CantFindItemErrorMessage(theEnv,"deftemplate",deftemplateName);
      return;
     }

   /*=====================*/
   /* Get the slot names. */
   /*=====================*/

   EnvDeftemplateSlotNames(theEnv,theDeftemplate,returnValue);
  }

/**********************************************/
/* EnvDeftemplateSlotNames: C access routine  */
/*   for the deftemplate-slot-names function. */
/**********************************************/
globle void EnvDeftemplateSlotNames(
  void *theEnv,
  void *vTheDeftemplate,
  DATA_OBJECT *returnValue)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   struct multifield *theList;
   struct templateSlot *theSlot;
   unsigned long count;

   /*===============================================*/
   /* If we're dealing with an implied deftemplate, */
   /* then the only slot names is "implied."        */
   /*===============================================*/

   if (theDeftemplate->implied)
     {
      SetpType(returnValue,MULTIFIELD);
      SetpDOBegin(returnValue,1);
      SetpDOEnd(returnValue,1);
      theList = (struct multifield *) EnvCreateMultifield(theEnv,(int) 1);
      SetMFType(theList,1,SYMBOL);
      SetMFValue(theList,1,EnvAddSymbol(theEnv,"implied"));
      SetpValue(returnValue,(void *) theList);
      return;
     }

   /*=================================*/
   /* Count the number of slot names. */
   /*=================================*/

   for (count = 0, theSlot = theDeftemplate->slotList;
        theSlot != NULL;
        count++, theSlot = theSlot->next)
     { /* Do Nothing */ }

   /*=============================================================*/
   /* Create a multifield value in which to store the slot names. */
   /*=============================================================*/

   SetpType(returnValue,MULTIFIELD);
   SetpDOBegin(returnValue,1);
   SetpDOEnd(returnValue,(long) count);
   theList = (struct multifield *) EnvCreateMultifield(theEnv,count);
   SetpValue(returnValue,(void *) theList);

   /*===============================================*/
   /* Store the slot names in the multifield value. */
   /*===============================================*/

   for (count = 1, theSlot = theDeftemplate->slotList;
        theSlot != NULL;
        count++, theSlot = theSlot->next)
     {
      SetMFType(theList,count,SYMBOL);
      SetMFValue(theList,count,theSlot->slotName);
     }
  }

/*******************************************************/
/* DeftemplateSlotDefaultPFunction: H/L access routine */
/*   for the deftemplate-slot-defaultp function.       */
/*******************************************************/
globle void *DeftemplateSlotDefaultPFunction(
  void *theEnv)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;
   int defaultType;

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-existp",&theDeftemplate,2);
   if (slotName == NULL)
     { return(EnvFalseSymbol(theEnv)); }

   /*===============================*/
   /* Does the slot have a default? */
   /*===============================*/
   
   defaultType = EnvDeftemplateSlotDefaultP(theEnv,theDeftemplate,ValueToString(slotName));
   
   if (defaultType == STATIC_DEFAULT)
     { return(EnvAddSymbol(theEnv,"static")); }
   else if (defaultType == DYNAMIC_DEFAULT)
     { return(EnvAddSymbol(theEnv,"dynamic")); }
   
   return(EnvFalseSymbol(theEnv)); 
  }

/*************************************************/
/* EnvDeftemplateSlotDefaultP: C access routine  */
/*   for the deftemplate-slot-defaultp function. */
/*************************************************/
globle int EnvDeftemplateSlotDefaultP(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName)
  {
   short position;
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   struct templateSlot *theSlot;
    
   /*==================================================*/
   /* Make sure the slot exists (the symbol implied is */
   /* used for the implied slot of an ordered fact).   */
   /*==================================================*/

   if (theDeftemplate->implied)
     {
      if (strcmp(slotName,"implied") == 0)
        {
         return(STATIC_DEFAULT);
        }
      else
        {
         SetEvaluationError(theEnv,TRUE);
         InvalidDeftemplateSlotMessage(theEnv,slotName,
                                       ValueToString(theDeftemplate->header.name),FALSE);
         return(NO_DEFAULT);
        }
     }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/
   
   else if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position)) == NULL)
     {
      SetEvaluationError(theEnv,TRUE);
      InvalidDeftemplateSlotMessage(theEnv,slotName,
                                    ValueToString(theDeftemplate->header.name),FALSE);
      return(NO_DEFAULT);
     }
     
   /*======================================*/
   /* Return the default type of the slot. */
   /*======================================*/
   
   if (theSlot->noDefault)
     { return(NO_DEFAULT); }
   else if (theSlot->defaultDynamic)
     { return(DYNAMIC_DEFAULT); }
   
   return(STATIC_DEFAULT);
  }

/*************************************************************/
/* DeftemplateSlotDefaultValueFunction: H/L access routine   */
/*   for the deftemplate-slot-default-value function.        */
/*************************************************************/
globle void DeftemplateSlotDefaultValueFunction(
  void *theEnv,
  DATA_OBJECT_PTR theValue)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-default-value",&theDeftemplate,2);
   if (slotName == NULL)
     {
      theValue->type = SYMBOL;
      theValue->value = EnvFalseSymbol(theEnv);
      return;
     }

   /*=========================================*/
   /* Get the deftemplate slot default value. */
   /*=========================================*/
   
   EnvDeftemplateSlotDefaultValue(theEnv,theDeftemplate,ValueToString(slotName),theValue);
  }

/******************************************************/
/* EnvDeftemplateSlotDefaultValue: C access routine   */
/*   for the deftemplate-slot-default-value function. */
/******************************************************/
globle intBool EnvDeftemplateSlotDefaultValue(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName,
  DATA_OBJECT_PTR theValue)
  {
   short position;
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   struct templateSlot *theSlot;
   DATA_OBJECT tempDO;
   
   /*=============================================*/
   /* Set up the default return value for errors. */
   /*=============================================*/

   SetpType(theValue,SYMBOL);
   SetpValue(theValue,EnvFalseSymbol(theEnv));
 
   /*==================================================*/
   /* Make sure the slot exists (the symbol implied is */
   /* used for the implied slot of an ordered fact).   */
   /*==================================================*/

   if (theDeftemplate->implied)
     {
      if (strcmp(slotName,"implied") == 0)
        {
         theValue->type = MULTIFIELD;
         theValue->value = EnvCreateMultifield(theEnv,0L);
         theValue->begin = 1;
         theValue->end = 0;
         return(TRUE);
        }
      else
        {
         SetEvaluationError(theEnv,TRUE);
         InvalidDeftemplateSlotMessage(theEnv,slotName,
                                       ValueToString(theDeftemplate->header.name),FALSE);
         return(FALSE);
        }
     }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/

   else if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position)) == NULL)
     {
      SetEvaluationError(theEnv,TRUE);
      InvalidDeftemplateSlotMessage(theEnv,slotName,
                                    ValueToString(theDeftemplate->header.name),FALSE);
      return(FALSE);
     }
     
   /*=======================================*/
   /* Return the default value of the slot. */
   /*=======================================*/
   
   if (theSlot->noDefault)
     {
      SetpType(theValue,SYMBOL);
      SetpValue(theValue,EnvAddSymbol(theEnv,"?NONE"));
     }
   else if (DeftemplateSlotDefault(theEnv,theDeftemplate,theSlot,&tempDO,TRUE))
     {
      SetpDOBegin(theValue,GetDOBegin(tempDO));
      SetpDOEnd(theValue,GetDOEnd(tempDO));
      SetpType(theValue,tempDO.type);
      SetpValue(theValue,tempDO.value);
     }
   else
     { return (FALSE); }

   return(TRUE);
  }

/**********************************************************/
/* DeftemplateSlotCardinalityFunction: H/L access routine */
/*   for the deftemplate-slot-cardinality function.       */
/**********************************************************/
globle void DeftemplateSlotCardinalityFunction(
  void *theEnv,
  DATA_OBJECT_PTR theValue)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-cardinality",&theDeftemplate,2);
   if (slotName == NULL)
     {
      EnvSetMultifieldErrorValue(theEnv,theValue);
      return;
     }

   /*=======================================*/
   /* Get the deftemplate slot cardinality. */
   /*=======================================*/
   
   EnvDeftemplateSlotCardinality(theEnv,theDeftemplate,ValueToString(slotName),theValue);
  }

/****************************************************/
/* EnvDeftemplateSlotCardinality: C access routine  */
/*   for the deftemplate-slot-cardinality function. */
/****************************************************/
globle void EnvDeftemplateSlotCardinality(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName,
  DATA_OBJECT *result)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   short position;
   struct templateSlot *theSlot;

   /*===============================================*/
   /* If we're dealing with an implied deftemplate, */
   /* then the only slot names is "implied."        */
   /*===============================================*/

   if (theDeftemplate->implied)
     {
      if (strcmp(slotName,"implied") == 0)
        {
         result->type = MULTIFIELD;
         result->begin = 0;
         result->end = 1;
         result->value = EnvCreateMultifield(theEnv,2L);
         SetMFType(result->value,1,INTEGER);
         SetMFValue(result->value,1,SymbolData(theEnv)->Zero);
         SetMFType(result->value,2,SYMBOL);
         SetMFValue(result->value,2,SymbolData(theEnv)->PositiveInfinity);
         return;
        }
      else
        {     
         EnvSetMultifieldErrorValue(theEnv,result);
         SetEvaluationError(theEnv,TRUE);
         InvalidDeftemplateSlotMessage(theEnv,slotName,
                                       ValueToString(theDeftemplate->header.name),FALSE);
         return;
        }
     }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/

   else if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position)) == NULL)
     {
      EnvSetMultifieldErrorValue(theEnv,result);
      SetEvaluationError(theEnv,TRUE);
      InvalidDeftemplateSlotMessage(theEnv,slotName,
                                    ValueToString(theDeftemplate->header.name),FALSE);
      return;
     }
     
   /*=====================================*/
   /* Return the cardinality of the slot. */
   /*=====================================*/
   
   if (theSlot->multislot == 0)
     {
      EnvSetMultifieldErrorValue(theEnv,result);
      return;
     }
     
   result->type = MULTIFIELD;
   result->begin = 0;
   result->end = 1;
   result->value = EnvCreateMultifield(theEnv,2L);
   
   if (theSlot->constraints != NULL)
     {
      SetMFType(result->value,1,theSlot->constraints->minFields->type);
      SetMFValue(result->value,1,theSlot->constraints->minFields->value);
      SetMFType(result->value,2,theSlot->constraints->maxFields->type);
      SetMFValue(result->value,2,theSlot->constraints->maxFields->value);
     }
   else
     {
      SetMFType(result->value,1,INTEGER);
      SetMFValue(result->value,1,SymbolData(theEnv)->Zero);
      SetMFType(result->value,2,SYMBOL);
      SetMFValue(result->value,2,SymbolData(theEnv)->PositiveInfinity);
     }
  }

/************************************************************/
/* DeftemplateSlotAllowedValuesFunction: H/L access routine */
/*   for the deftemplate-slot-allowed-values function.      */
/************************************************************/
globle void DeftemplateSlotAllowedValuesFunction(
  void *theEnv,
  DATA_OBJECT_PTR theValue)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-allowed-values",&theDeftemplate,2);
   if (slotName == NULL)
     {
      EnvSetMultifieldErrorValue(theEnv,theValue);
      return;
     }

   /*==========================================*/
   /* Get the deftemplate slot allowed values. */
   /*==========================================*/
   
   EnvDeftemplateSlotAllowedValues(theEnv,theDeftemplate,ValueToString(slotName),theValue);
  }

/*******************************************************/
/* EnvDeftemplateSlotAllowedValues: C access routine   */
/*   for the deftemplate-slot-allowed-values function. */
/*******************************************************/
globle void EnvDeftemplateSlotAllowedValues(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName,
  DATA_OBJECT *result)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   short position;
   struct templateSlot *theSlot;
   int i;
   EXPRESSION *theExp;

   /*===============================================*/
   /* If we're dealing with an implied deftemplate, */
   /* then the only slot names is "implied."        */
   /*===============================================*/

   if (theDeftemplate->implied)
     {
      if (strcmp(slotName,"implied") == 0)
        {
         result->type = SYMBOL;
         result->value = EnvFalseSymbol(theEnv);
         return;
        }
      else
        {     
         EnvSetMultifieldErrorValue(theEnv,result);
         SetEvaluationError(theEnv,TRUE);
         InvalidDeftemplateSlotMessage(theEnv,slotName,
                                       ValueToString(theDeftemplate->header.name),FALSE);
         return;
        }
     }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/

   else if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position)) == NULL)
     {
      EnvSetMultifieldErrorValue(theEnv,result);
      SetEvaluationError(theEnv,TRUE);
      InvalidDeftemplateSlotMessage(theEnv,slotName,
                                    ValueToString(theDeftemplate->header.name),FALSE);
      return;
     }
     
   /*========================================*/
   /* Return the allowed values of the slot. */
   /*========================================*/
   
   if ((theSlot->constraints != NULL) ? (theSlot->constraints->restrictionList == NULL) : TRUE)
     {
      result->type = SYMBOL;
      result->value = EnvFalseSymbol(theEnv);
      return;
     }
   
   result->type = MULTIFIELD;
   result->begin = 0;  
   result->end = ExpressionSize(theSlot->constraints->restrictionList) - 1;
   result->value = EnvCreateMultifield(theEnv,(unsigned long) (result->end + 1));
   i = 1;
   
   theExp = theSlot->constraints->restrictionList;
   while (theExp != NULL)
     {
      SetMFType(result->value,i,theExp->type);
      SetMFValue(result->value,i,theExp->value);
      theExp = theExp->nextArg;
      i++;
     }
  }

/****************************************************/
/* DeftemplateSlotRangeFunction: H/L access routine */
/*   for the deftemplate-slot-range function.       */
/****************************************************/
globle void DeftemplateSlotRangeFunction(
  void *theEnv,
  DATA_OBJECT_PTR theValue)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-range",&theDeftemplate,2);
   if (slotName == NULL)
     {
      EnvSetMultifieldErrorValue(theEnv,theValue);
      return;
     }

   /*=================================*/
   /* Get the deftemplate slot range. */
   /*=================================*/
   
   EnvDeftemplateSlotRange(theEnv,theDeftemplate,ValueToString(slotName),theValue);
  }

/**********************************************/
/* EnvDeftemplateSlotRange: C access routine  */
/*   for the deftemplate-slot-range function. */
/**********************************************/
globle void EnvDeftemplateSlotRange(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName,
  DATA_OBJECT *result)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   short position;
   struct templateSlot *theSlot;

   /*===============================================*/
   /* If we're dealing with an implied deftemplate, */
   /* then the only slot names is "implied."        */
   /*===============================================*/

   if (theDeftemplate->implied)
     {
      if (strcmp(slotName,"implied") == 0)
        {
         result->type = MULTIFIELD;
         result->begin = 0;
         result->end = 1;
         result->value = EnvCreateMultifield(theEnv,2L);
         SetMFType(result->value,1,SYMBOL);
         SetMFValue(result->value,1,SymbolData(theEnv)->NegativeInfinity);
         SetMFType(result->value,2,SYMBOL);
         SetMFValue(result->value,2,SymbolData(theEnv)->PositiveInfinity);
         return;
        }
      else
        {     
         EnvSetMultifieldErrorValue(theEnv,result);
         SetEvaluationError(theEnv,TRUE);
         InvalidDeftemplateSlotMessage(theEnv,slotName,
                                       ValueToString(theDeftemplate->header.name),FALSE);
         return;
        }
     }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/

   else if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position)) == NULL)
     {
      EnvSetMultifieldErrorValue(theEnv,result);
      SetEvaluationError(theEnv,TRUE);
      InvalidDeftemplateSlotMessage(theEnv,slotName,
                                    ValueToString(theDeftemplate->header.name),FALSE);
      return;
     }
     
   /*===============================*/
   /* Return the range of the slot. */
   /*===============================*/
   
   if ((theSlot->constraints == NULL) ? FALSE :
       (theSlot->constraints->anyAllowed || theSlot->constraints->floatsAllowed ||
        theSlot->constraints->integersAllowed))
     {
      result->type = MULTIFIELD;
      result->begin = 0;
      result->end = 1;
      result->value = EnvCreateMultifield(theEnv,2L);
      SetMFType(result->value,1,theSlot->constraints->minValue->type);
      SetMFValue(result->value,1,theSlot->constraints->minValue->value);
      SetMFType(result->value,2,theSlot->constraints->maxValue->type);
      SetMFValue(result->value,2,theSlot->constraints->maxValue->value);
     }
   else
     {
      result->type = SYMBOL;
      result->value = EnvFalseSymbol(theEnv);
      return;
     }
  }
  
/****************************************************/
/* DeftemplateSlotTypesFunction: H/L access routine */
/*   for the deftemplate-slot-types function.       */
/****************************************************/
globle void DeftemplateSlotTypesFunction(
  void *theEnv,
  DATA_OBJECT_PTR theValue)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-types",&theDeftemplate,2);
   if (slotName == NULL)
     {
      EnvSetMultifieldErrorValue(theEnv,theValue);
      return;
     }

   /*=================================*/
   /* Get the deftemplate slot types. */
   /*=================================*/
   
   EnvDeftemplateSlotTypes(theEnv,theDeftemplate,ValueToString(slotName),theValue);
  }

/**********************************************/
/* EnvDeftemplateSlotTypes: C access routine  */
/*   for the deftemplate-slot-types function. */
/**********************************************/
globle void EnvDeftemplateSlotTypes(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName,
  DATA_OBJECT *result)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   short position;
   struct templateSlot *theSlot = NULL;
   int numTypes, i, allTypes = FALSE;

   /*===============================================*/
   /* If we're dealing with an implied deftemplate, */
   /* then the only slot name is "implied."         */
   /*===============================================*/

   if (theDeftemplate->implied)
     {
      if (strcmp(slotName,"implied") != 0)
        {     
         EnvSetMultifieldErrorValue(theEnv,result);
         SetEvaluationError(theEnv,TRUE);
         InvalidDeftemplateSlotMessage(theEnv,slotName,
                                       ValueToString(theDeftemplate->header.name),FALSE);
         return;
        }
     }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/

   else if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position)) == NULL)
     {
      EnvSetMultifieldErrorValue(theEnv,result);
      SetEvaluationError(theEnv,TRUE);
      InvalidDeftemplateSlotMessage(theEnv,slotName,
                                    ValueToString(theDeftemplate->header.name),FALSE);
      return;
     }

   /*==============================================*/
   /* If the slot has no constraint information or */
   /* there is no type restriction, then all types */
   /* are allowed for the slot.                    */
   /*==============================================*/
   
   if ((theDeftemplate->implied) ||
       ((theSlot->constraints != NULL) ? theSlot->constraints->anyAllowed : TRUE))
     {
#if OBJECT_SYSTEM
      numTypes = 8;
#else
      numTypes = 6;
#endif
      allTypes = TRUE;
     }
     
   /*==============================================*/
   /* Otherwise count the number of types allowed. */
   /*==============================================*/
   
   else
     {
      numTypes = theSlot->constraints->symbolsAllowed +
                 theSlot->constraints->stringsAllowed +
                 theSlot->constraints->floatsAllowed +
                 theSlot->constraints->integersAllowed +
                 theSlot->constraints->instanceNamesAllowed +
                 theSlot->constraints->instanceAddressesAllowed +
                 theSlot->constraints->externalAddressesAllowed +
                 theSlot->constraints->factAddressesAllowed;
     }
  
   /*========================================*/
   /* Return the allowed types for the slot. */
   /*========================================*/
   
   result->type = MULTIFIELD;
   result->begin = 0;
   result->end = numTypes - 1;
   result->value = EnvCreateMultifield(theEnv,(long) numTypes);

   i = 1;

   if (allTypes || theSlot->constraints->floatsAllowed)
     {
      SetMFType(result->value,i,SYMBOL);
      SetMFValue(result->value,i++,EnvAddSymbol(theEnv,"FLOAT"));
     }
        
   if (allTypes || theSlot->constraints->integersAllowed)
     {
      SetMFType(result->value,i,SYMBOL);
      SetMFValue(result->value,i++,EnvAddSymbol(theEnv,"INTEGER"));
     }
        
   if (allTypes || theSlot->constraints->symbolsAllowed)
     {
      SetMFType(result->value,i,SYMBOL);
      SetMFValue(result->value,i++,EnvAddSymbol(theEnv,"SYMBOL"));
     }
        
   if (allTypes || theSlot->constraints->stringsAllowed)
     {
      SetMFType(result->value,i,SYMBOL);
      SetMFValue(result->value,i++,EnvAddSymbol(theEnv,"STRING"));
     }
      
   if (allTypes || theSlot->constraints->externalAddressesAllowed)
     {
      SetMFType(result->value,i,SYMBOL);
      SetMFValue(result->value,i++,EnvAddSymbol(theEnv,"EXTERNAL-ADDRESS"));
     }
      
   if (allTypes || theSlot->constraints->factAddressesAllowed)
     {
      SetMFType(result->value,i,SYMBOL);
      SetMFValue(result->value,i++,EnvAddSymbol(theEnv,"FACT-ADDRESS"));
     }
        
#if OBJECT_SYSTEM
   if (allTypes || theSlot->constraints->instanceAddressesAllowed)
     {
      SetMFType(result->value,i,SYMBOL);
      SetMFValue(result->value,i++,EnvAddSymbol(theEnv,"INSTANCE-ADDRESS"));
     }
        
   if (allTypes || theSlot->constraints->instanceNamesAllowed)
     {       
      SetMFType(result->value,i,SYMBOL);
      SetMFValue(result->value,i,EnvAddSymbol(theEnv,"INSTANCE-NAME"));
     }
#endif
  }  

/*****************************************************/
/* DeftemplateSlotMultiPFunction: H/L access routine */
/*   for the deftemplate-slot-multip function.       */
/*****************************************************/
globle int DeftemplateSlotMultiPFunction(
  void *theEnv)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-multip",&theDeftemplate,2);
   if (slotName == NULL)
     { return(FALSE); }

   /*================================*/
   /* Is the slot a multifield slot? */
   /*================================*/
   
   return EnvDeftemplateSlotMultiP(theEnv,theDeftemplate,ValueToString(slotName));
  }
  
/***********************************************/
/* EnvDeftemplateSlotMultiP: C access routine  */
/*   for the deftemplate-slot-multip function. */
/***********************************************/
globle int EnvDeftemplateSlotMultiP(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   short position;
   struct templateSlot *theSlot;

   /*===============================================*/
   /* If we're dealing with an implied deftemplate, */
   /* then the only slot names is "implied."        */
   /*===============================================*/

   if (theDeftemplate->implied)
     {
      if (strcmp(slotName,"implied") == 0)
        { return(TRUE); }
      else
        {
         SetEvaluationError(theEnv,TRUE);
         InvalidDeftemplateSlotMessage(theEnv,slotName,
                                       ValueToString(theDeftemplate->header.name),FALSE);
         return(FALSE); 
        }
     }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/

   else if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position)) == NULL)
     { 
      SetEvaluationError(theEnv,TRUE);
      InvalidDeftemplateSlotMessage(theEnv,slotName,
                                    ValueToString(theDeftemplate->header.name),FALSE);
      return(FALSE);
     }

   /*================================*/
   /* Is the slot a multifield slot? */
   /*================================*/
   
   return(theSlot->multislot);   
  }  

/******************************************************/
/* DeftemplateSlotSinglePFunction: H/L access routine */
/*   for the deftemplate-slot-singlep function.       */
/******************************************************/
globle int DeftemplateSlotSinglePFunction(
  void *theEnv)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-singlep",&theDeftemplate,2);
   if (slotName == NULL)
     { return(FALSE); }

   /*==================================*/
   /* Is the slot a single field slot? */
   /*==================================*/
   
   return EnvDeftemplateSlotSingleP(theEnv,theDeftemplate,ValueToString(slotName));
  }

/************************************************/
/* EnvDeftemplateSlotSingleP: C access routine  */
/*   for the deftemplate-slot-singlep function. */
/************************************************/
globle int EnvDeftemplateSlotSingleP(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   short position;
   struct templateSlot *theSlot;

   /*===============================================*/
   /* If we're dealing with an implied deftemplate, */
   /* then the only slot names is "implied."        */
   /*===============================================*/

   if (theDeftemplate->implied)
     {
      if (strcmp(slotName,"implied") == 0)
        { return(FALSE); }
      else
        {
         SetEvaluationError(theEnv,TRUE);
         InvalidDeftemplateSlotMessage(theEnv,slotName,
                                       ValueToString(theDeftemplate->header.name),FALSE);
         return(FALSE); 
        }
     }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/

   else if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position)) == NULL)
     {
      SetEvaluationError(theEnv,TRUE);
      InvalidDeftemplateSlotMessage(theEnv,slotName,
                                    ValueToString(theDeftemplate->header.name),FALSE);
      return(FALSE); 
     }

   /*==================================*/
   /* Is the slot a single field slot? */
   /*==================================*/

   return(! theSlot->multislot);   
  }  

/*****************************************************/
/* DeftemplateSlotExistPFunction: H/L access routine */
/*   for the deftemplate-slot-existp function.       */
/*****************************************************/
globle int DeftemplateSlotExistPFunction(
  void *theEnv)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-existp",&theDeftemplate,2);
   if (slotName == NULL)
     { return(FALSE); }

   /*======================*/
   /* Does the slot exist? */
   /*======================*/
   
   return EnvDeftemplateSlotExistP(theEnv,theDeftemplate,ValueToString(slotName));
  }

/************************************************/
/* EnvDeftemplateSlotExistP: C access routine  */
/*   for the deftemplate-slot-existp function. */
/************************************************/
globle int EnvDeftemplateSlotExistP(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   short position;

   /*===============================================*/
   /* If we're dealing with an implied deftemplate, */
   /* then the only slot names is "implied."        */
   /*===============================================*/

   if (theDeftemplate->implied)
     {
      if (strcmp(slotName,"implied") == 0)
        { return(TRUE); }
      else
        { return(FALSE); }
     }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/
     
   else if (FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position) == NULL)
     { return(FALSE); }

   /*==================*/
   /* The slot exists. */
   /*==================*/
   
   return(TRUE);   
  }  

/**********************************************************/
/* DeftemplateSlotFacetExistPFunction: H/L access routine */
/*   for the deftemplate-slot-facet-existp function.      */
/**********************************************************/
globle int DeftemplateSlotFacetExistPFunction(
  void *theEnv)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;
   DATA_OBJECT facetName;

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-facet-existp",&theDeftemplate,3);
   if (slotName == NULL)
     { return(FALSE); }

   /*============================*/
   /* Get the name of the facet. */
   /*============================*/

   if (EnvArgTypeCheck(theEnv,"deftemplate-slot-facet-existp",3,SYMBOL,&facetName) == FALSE)
     { return(FALSE); }
     
   /*======================*/
   /* Does the slot exist? */
   /*======================*/
   
   return EnvDeftemplateSlotFacetExistP(theEnv,theDeftemplate,ValueToString(slotName),DOToString(facetName));
  }

/*****************************************************/
/* EnvDeftemplateSlotFacetExistP: C access routine   */
/*   for the deftemplate-slot-facet-existp function. */
/*****************************************************/
globle int EnvDeftemplateSlotFacetExistP(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName,
  char *facetName)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   short position;
   struct templateSlot *theSlot;
   SYMBOL_HN *facetHN;
   struct expr *tempFacet;

   /*=================================================*/
   /* An implied deftemplate doesn't have any facets. */
   /*=================================================*/

   if (theDeftemplate->implied)
     { return(FALSE); }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/
     
   else if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position)) == NULL)
     { return(FALSE); }

   /*=======================*/
   /* Search for the facet. */
   /*=======================*/
   
   facetHN = FindSymbolHN(theEnv,facetName);
   for (tempFacet = theSlot->facetList;
        tempFacet != NULL;
        tempFacet = tempFacet->nextArg)
     {
      if (tempFacet->value == facetHN)
        { return(TRUE); }
     }
   
   /*===========================*/
   /* The facet does not exist. */
   /*===========================*/
   
   return(FALSE);   
  }  

/*********************************************************/
/* DeftemplateSlotFacetValueFunction: H/L access routine */
/*   for the deftemplate-slot-facet-value function.      */
/*********************************************************/
globle void DeftemplateSlotFacetValueFunction(
  void *theEnv,
  DATA_OBJECT *returnValue)
  {
   struct deftemplate *theDeftemplate;
   SYMBOL_HN *slotName;
   DATA_OBJECT facetName;

   /*=============================================*/
   /* Set up the default return value for errors. */
   /*=============================================*/

   returnValue->type = SYMBOL;
   returnValue->value = EnvFalseSymbol(theEnv);

   /*===================================================*/
   /* Retrieve the deftemplate and slot name arguments. */
   /*===================================================*/
   
   slotName = CheckDeftemplateAndSlotArguments(theEnv,"deftemplate-slot-facet-existp",&theDeftemplate,3);
   if (slotName == NULL)
     { return; }

   /*============================*/
   /* Get the name of the facet. */
   /*============================*/

   if (EnvArgTypeCheck(theEnv,"deftemplate-slot-facet-existp",3,SYMBOL,&facetName) == FALSE)
     { return; }
     
   /*===========================*/
   /* Retrieve the facet value. */
   /*===========================*/
   
   EnvDeftemplateSlotFacetValue(theEnv,theDeftemplate,ValueToString(slotName),DOToString(facetName),returnValue);
  }

/****************************************************/
/* EnvDeftemplateSlotFacetValue: C access routine   */
/*   for the deftemplate-slot-facet-value function. */
/****************************************************/
globle int EnvDeftemplateSlotFacetValue(
  void *theEnv,
  void *vTheDeftemplate,
  char *slotName,
  char *facetName,
  DATA_OBJECT *rv)
  {
   struct deftemplate *theDeftemplate = (struct deftemplate *) vTheDeftemplate;
   short position;
   struct templateSlot *theSlot;
   SYMBOL_HN *facetHN;
   struct expr *tempFacet;

   /*=================================================*/
   /* An implied deftemplate doesn't have any facets. */
   /*=================================================*/

   if (theDeftemplate->implied)
     { return(FALSE); }

   /*============================================*/
   /* Otherwise search for the slot name in the  */
   /* list of slots defined for the deftemplate. */
   /*============================================*/
     
   else if ((theSlot = FindSlot(theDeftemplate,(SYMBOL_HN *) EnvAddSymbol(theEnv,slotName),&position)) == NULL)
     { return(FALSE); }

   /*=======================*/
   /* Search for the facet. */
   /*=======================*/
   
   facetHN = FindSymbolHN(theEnv,facetName);
   for (tempFacet = theSlot->facetList;
        tempFacet != NULL;
        tempFacet = tempFacet->nextArg)
     {
      if (tempFacet->value == facetHN)
        { 
         EvaluateExpression(theEnv,tempFacet->argList,rv);
         return(TRUE); 
        }
     }
   
   /*===========================*/
   /* The facet does not exist. */
   /*===========================*/
   
   return(FALSE);   
  }  
  
/************************************************************/
/* CheckDeftemplateAndSlotArguments: Checks the deftemplate */
/*   and slot arguments for various functions.              */
/************************************************************/
static SYMBOL_HN *CheckDeftemplateAndSlotArguments(
  void *theEnv,
  char *functionName,
  struct deftemplate **theDeftemplate,
  int expectedArgs)
  {
   DATA_OBJECT tempDO;
   char *deftemplateName;

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if (EnvArgCountCheck(theEnv,functionName,EXACTLY,expectedArgs) == -1) 
     { return(NULL); }

   /*=====================================*/
   /* There must be at least 2 arguments. */
   /*=====================================*/

   if (EnvArgCountCheck(theEnv,functionName,AT_LEAST,2) == -1) 
     { return(NULL); }

   /*=======================================*/
   /* Get the reference to the deftemplate. */
   /*=======================================*/

   EnvRtnUnknown(theEnv,1,&tempDO);

   if (GetType(tempDO) != SYMBOL)
     {
      ExpectedTypeError1(theEnv,functionName,1,"deftemplate name");
      return(NULL);
     }
     
   deftemplateName = DOToString(tempDO);

   *theDeftemplate = (struct deftemplate *) EnvFindDeftemplate(theEnv,deftemplateName);
   if (*theDeftemplate == NULL)
     {
      CantFindItemErrorMessage(theEnv,"deftemplate",deftemplateName);
      return(NULL);
     }

   /*===========================*/
   /* Get the name of the slot. */
   /*===========================*/

   if (EnvArgTypeCheck(theEnv,functionName,2,SYMBOL,&tempDO) == FALSE)
     { return(NULL); }
     
   return((SYMBOL_HN *) GetValue(tempDO));
  }
  
#if (! RUN_TIME) && (! BLOAD_ONLY)

/***************************************************************/
/* UpdateModifyDuplicate: Changes the modify/duplicate command */
/*   found on the RHS of a rule such that the positions of the */
/*   slots for replacement are stored rather than the slot     */
/*   name which allows quicker replacement of slots. This      */
/*   substitution can only take place when the deftemplate     */
/*   type is known (i.e. if a fact-index is used you don't     */
/*   know which type of deftemplate is going to be replaced    */
/*   until you actually do the replacement of slots).          */
/***************************************************************/
globle intBool UpdateModifyDuplicate(
  void *theEnv,
  struct expr *top,
  char *name,
  void *vTheLHS)
  {
   struct expr *functionArgs, *tempArg;
   SYMBOL_HN *templateName;
   struct deftemplate *theDeftemplate;
   struct templateSlot *slotPtr;
   short position;

   /*========================================*/
   /* Determine the fact-address or index to */
   /* be retracted by the modify command.    */
   /*========================================*/

   functionArgs = top->argList;
   if (functionArgs->type == SF_VARIABLE)
     {
      templateName = FindTemplateForFactAddress((SYMBOL_HN *) functionArgs->value,
                                                (struct lhsParseNode *) vTheLHS);
      if (templateName == NULL) return(TRUE);
     }
   else
     { return(TRUE); }

   /*========================================*/
   /* Make sure that the fact being modified */
   /* has a corresponding deftemplate.       */
   /*========================================*/

   theDeftemplate = (struct deftemplate *)
                    LookupConstruct(theEnv,DeftemplateData(theEnv)->DeftemplateConstruct,
                                    ValueToString(templateName),
                                    FALSE);

   if (theDeftemplate == NULL) return(TRUE);

   if (theDeftemplate->implied) return(TRUE);

   /*=============================================================*/
   /* Make sure all the slot names are valid for the deftemplate. */
   /*=============================================================*/

   tempArg = functionArgs->nextArg;
   while (tempArg != NULL)
     {
      /*======================*/
      /* Does the slot exist? */
      /*======================*/

      if ((slotPtr = FindSlot(theDeftemplate,(SYMBOL_HN *) tempArg->value,&position)) == NULL)
        {
         InvalidDeftemplateSlotMessage(theEnv,ValueToString(tempArg->value),
                                       ValueToString(theDeftemplate->header.name),TRUE);
         return(FALSE);
        }

      /*=========================================================*/
      /* Is a multifield value being put in a single field slot? */
      /*=========================================================*/

      if (slotPtr->multislot == FALSE)
        {
         if (tempArg->argList == NULL)
           {
            SingleFieldSlotCardinalityError(theEnv,slotPtr->slotName->contents);
            return(FALSE);
           }
         else if (tempArg->argList->nextArg != NULL)
           {
            SingleFieldSlotCardinalityError(theEnv,slotPtr->slotName->contents);
            return(FALSE);
           }
         else if ((tempArg->argList->type == MF_VARIABLE) ||
                  ((tempArg->argList->type == FCALL) ?
                   (((struct FunctionDefinition *) tempArg->argList->value)->returnValueType == 'm') :
                      FALSE))
           {
            SingleFieldSlotCardinalityError(theEnv,slotPtr->slotName->contents);
            return(FALSE);
           }
        }

      /*======================================*/
      /* Are the slot restrictions satisfied? */
      /*======================================*/

      if (CheckRHSSlotTypes(theEnv,tempArg->argList,slotPtr,name) == 0)
        return(FALSE);

      /*=============================================*/
      /* Replace the slot with the integer position. */
      /*=============================================*/

      tempArg->type = INTEGER;
      tempArg->value = (void *) EnvAddLong(theEnv,(long long) (FindSlotPosition(theDeftemplate,(SYMBOL_HN *) tempArg->value) - 1));

      tempArg = tempArg->nextArg;
     }

   return(TRUE);
  }

/**************************************************/
/* FindTemplateForFactAddress: Searches for the   */
/*   deftemplate name associated with the pattern */
/*   to which a fact address has been bound.      */
/**************************************************/
static SYMBOL_HN *FindTemplateForFactAddress(
  SYMBOL_HN *factAddress,
  struct lhsParseNode *theLHS)
  {
   struct lhsParseNode *thePattern = NULL;

   /*===============================================*/
   /* Look through the LHS patterns for the pattern */
   /* which is bound to the fact address used by    */
   /* the modify/duplicate function.                */
   /*===============================================*/

   while (theLHS != NULL)
     {
      if (theLHS->value == (void *) factAddress)
        {
         thePattern = theLHS;
         theLHS = NULL;
        }
      else
        { theLHS = theLHS->bottom; }
     }

   if (thePattern == NULL) return(NULL);

   /*=====================================*/
   /* Verify that just a symbol is stored */
   /* as the first field of the pattern.  */
   /*=====================================*/

   thePattern = thePattern->right;
   if ((thePattern->type != SF_WILDCARD) || (thePattern->bottom == NULL))
     { return(NULL); }

   thePattern = thePattern->bottom;
   if ((thePattern->type != SYMBOL) ||
            (thePattern->right != NULL) ||
            (thePattern->bottom != NULL))
    { return(NULL); }

   /*==============================*/
   /* Return the deftemplate name. */
   /*==============================*/

   return((SYMBOL_HN *) thePattern->value);
  }

/*******************************************/
/* ModifyParse: Parses the modify command. */
/*******************************************/
globle struct expr *ModifyParse(
  void *theEnv,
  struct expr *top,
  char *logicalName)
  {
   return(ModAndDupParse(theEnv,top,logicalName,"modify"));
  }

/*************************************************/
/* DuplicateParse: Parses the duplicate command. */
/*************************************************/
globle struct expr *DuplicateParse(
  void *theEnv,
  struct expr *top,
  char *logicalName)
  {
   return(ModAndDupParse(theEnv,top,logicalName,"duplicate"));
  }

/*************************************************************/
/* ModAndDupParse: Parses the modify and duplicate commands. */
/*************************************************************/
static struct expr *ModAndDupParse(
  void *theEnv,
  struct expr *top,
  char *logicalName,
  char *name)
  {
   int error = FALSE;
   struct token theToken;
   struct expr *nextOne, *tempSlot;
   struct expr *newField, *firstField, *lastField;
   int printError;
   short done;

   /*==================================================================*/
   /* Parse the fact-address or index to the modify/duplicate command. */
   /*==================================================================*/

   SavePPBuffer(theEnv," ");
   GetToken(theEnv,logicalName,&theToken);

   if ((theToken.type == SF_VARIABLE) || (theToken.type == GBL_VARIABLE))
     { nextOne = GenConstant(theEnv,theToken.type,theToken.value); }
   else if (theToken.type == INTEGER)
     {
      if (! TopLevelCommand(theEnv))
        {
         PrintErrorID(theEnv,"TMPLTFUN",1,TRUE);
         EnvPrintRouter(theEnv,WERROR,"Fact-indexes can only be used by ");
         EnvPrintRouter(theEnv,WERROR,name);
         EnvPrintRouter(theEnv,WERROR," as a top level command.\n");
         ReturnExpression(theEnv,top);
         return(NULL);
        }

      nextOne = GenConstant(theEnv,INTEGER,theToken.value);
     }
   else
     {
      ExpectedTypeError2(theEnv,name,1);
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   nextOne->nextArg = NULL;
   nextOne->argList = NULL;
   top->argList = nextOne;
   nextOne = top->argList;

   /*=======================================================*/
   /* Parse the remaining modify/duplicate slot specifiers. */
   /*=======================================================*/

   GetToken(theEnv,logicalName,&theToken);
   while (theToken.type != RPAREN)
     {
      PPBackup(theEnv);
      SavePPBuffer(theEnv," ");
      SavePPBuffer(theEnv,theToken.printForm);

      /*=================================================*/
      /* Slot definition begins with a left parenthesis. */
      /*=================================================*/

      if (theToken.type != LPAREN)
        {
         SyntaxErrorMessage(theEnv,"duplicate/modify function");
         ReturnExpression(theEnv,top);
         return(NULL);
        }

      /*=================================*/
      /* The slot name must be a symbol. */
      /*=================================*/

      GetToken(theEnv,logicalName,&theToken);
      if (theToken.type != SYMBOL)
        {
         SyntaxErrorMessage(theEnv,"duplicate/modify function");
         ReturnExpression(theEnv,top);
         return(NULL);
        }

      /*=================================*/
      /* Check for duplicate slot names. */
      /*=================================*/

      for (tempSlot = top->argList->nextArg;
           tempSlot != NULL;
           tempSlot = tempSlot->nextArg)
        {
         if (tempSlot->value == theToken.value)
           {
            AlreadyParsedErrorMessage(theEnv,"slot ",ValueToString(theToken.value));
            ReturnExpression(theEnv,top);
            return(NULL);
           }
        }

      /*=========================================*/
      /* Add the slot name to the list of slots. */
      /*=========================================*/

      nextOne->nextArg = GenConstant(theEnv,SYMBOL,theToken.value);
      nextOne = nextOne->nextArg;

      /*====================================================*/
      /* Get the values to be stored in the specified slot. */
      /*====================================================*/

      firstField = NULL;
      lastField = NULL;
      done = FALSE;
      while (! done)
        {
         SavePPBuffer(theEnv," ");
         newField = GetAssertArgument(theEnv,logicalName,&theToken,&error,
                                      RPAREN,FALSE,&printError);

         if (error)
           {
            if (printError) SyntaxErrorMessage(theEnv,"deftemplate pattern");
            ReturnExpression(theEnv,top);
            return(NULL);
           }

         if (newField == NULL)
           { done = TRUE; }

         if (lastField == NULL)
           { firstField = newField; }
         else
           { lastField->nextArg = newField; }
         lastField = newField;
        }

      /*================================================*/
      /* Slot definition ends with a right parenthesis. */
      /*================================================*/

      if (theToken.type != RPAREN)
        {
         SyntaxErrorMessage(theEnv,"duplicate/modify function");
         ReturnExpression(theEnv,top);
         ReturnExpression(theEnv,firstField);
         return(NULL);
        }
      else
        {
         PPBackup(theEnv);
         PPBackup(theEnv);
         SavePPBuffer(theEnv,")");
        }

      nextOne->argList = firstField;

      GetToken(theEnv,logicalName,&theToken);
     }

   /*================================================*/
   /* Return the parsed modify/duplicate expression. */
   /*================================================*/

   return(top);
  }

#endif /* (! RUN_TIME) && (! BLOAD_ONLY) */

#endif /* DEFTEMPLATE_CONSTRUCT */

