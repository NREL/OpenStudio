   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*          DEFTEMPLATE RHS PARSING HEADER FILE        */
   /*******************************************************/

/*************************************************************/
/* Purpose: Parses deftemplate fact patterns used with the   */
/*   assert function.                                        */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Added additional argument required for         */
/*            DeriveDefaultFromConstraints.                  */
/*                                                           */
/*            Added additional argument required for         */
/*            InvalidDeftemplateSlotMessage.                 */
/*                                                           */
/*************************************************************/

#define _TMPLTRHS_SOURCE_

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "memalloc.h"
#include "prntutil.h"
#include "router.h"
#include "tmpltfun.h"
#include "tmpltdef.h"
#include "factrhs.h"
#include "extnfunc.h"
#include "modulutl.h"
#include "default.h"
#include "tmpltutl.h"
#include "tmpltlhs.h"

#include "tmpltrhs.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static struct expr            *ParseAssertSlotValues(void *,char *,struct token *,struct templateSlot *,int *,int);
   static struct expr            *ReorderAssertSlotValues(void *,struct templateSlot *,struct expr *,int *);
   static struct expr            *GetSlotAssertValues(void *,struct templateSlot *,struct expr *,int *);
   static struct expr            *FindAssertSlotItem(struct templateSlot *,struct expr *);
   static struct templateSlot    *ParseSlotLabel(void *,char *,struct token *,struct deftemplate *,int *,int);

/******************************************************************/
/* ParseAssertTemplate: Parses and builds the list of values that */
/*   are used for an assert of a fact with a deftemplate.         */
/******************************************************************/
globle struct expr *ParseAssertTemplate(
  void *theEnv,
  char *readSource,
  struct token *theToken,
  int *error,
  int endType,
  int constantsOnly,
  struct deftemplate *theDeftemplate)
  {
   struct expr *firstSlot, *lastSlot, *nextSlot;
   struct expr *firstArg, *tempSlot;
   struct templateSlot *slotPtr;

   firstSlot = NULL;
   lastSlot = NULL;

   /*==============================================*/
   /* Parse each of the slot fields in the assert. */
   /*==============================================*/

   while ((slotPtr = ParseSlotLabel(theEnv,readSource,theToken,theDeftemplate,error,endType)) != NULL)
     {
      /*========================================================*/
      /* Check to see that the slot hasn't already been parsed. */
      /*========================================================*/

      for (tempSlot = firstSlot;
           tempSlot != NULL;
           tempSlot = tempSlot->nextArg)
        {
         if (tempSlot->value == (void *) slotPtr->slotName)
           {
            AlreadyParsedErrorMessage(theEnv,"slot ",ValueToString(slotPtr->slotName));
            *error = TRUE;
            ReturnExpression(theEnv,firstSlot);
            return(NULL);
           }
        }

      /*============================================*/
      /* Parse the values to be stored in the slot. */
      /*============================================*/

      nextSlot = ParseAssertSlotValues(theEnv,readSource,theToken,
                                       slotPtr,error,constantsOnly);

      if (*error)
        {
         ReturnExpression(theEnv,firstSlot);
         return(NULL);
        }

      /*============================================*/
      /* Check to see if the values to be stored in */
      /* the slot violate the slot's constraints.   */
      /*============================================*/

      if (CheckRHSSlotTypes(theEnv,nextSlot->argList,slotPtr,"assert") == 0)
        {
         *error = TRUE;
         ReturnExpression(theEnv,firstSlot);
         ReturnExpression(theEnv,nextSlot);
         return(NULL);
        }

      /*===================================================*/
      /* Add the slot to the list of slots already parsed. */
      /*===================================================*/

      if (lastSlot == NULL)
        { firstSlot = nextSlot; }
      else
        { lastSlot->nextArg = nextSlot; }

      lastSlot = nextSlot;
     }

   /*=================================================*/
   /* Return if an error occured parsing a slot name. */
   /*=================================================*/

   if (*error)
     {
      ReturnExpression(theEnv,firstSlot);
      return(NULL);
     }

   /*=============================================================*/
   /* Reorder the arguments to the order used by the deftemplate. */
   /*=============================================================*/

   firstArg = ReorderAssertSlotValues(theEnv,theDeftemplate->slotList,firstSlot,error);
   ReturnExpression(theEnv,firstSlot);

   /*==============================*/
   /* Return the assert arguments. */
   /*==============================*/

   return(firstArg);
  }

/****************************************************************/
/* ParseSlotLabel: Parses the beginning of a slot definition.   */
/*   Checks for opening left parenthesis and a valid slot name. */
/****************************************************************/
static struct templateSlot *ParseSlotLabel(
  void *theEnv,
  char *inputSource,
  struct token *tempToken,
  struct deftemplate *theDeftemplate,
  int *error,
  int endType)
  {
   struct templateSlot *slotPtr;
   short position;

   /*========================*/
   /* Initialize error flag. */
   /*========================*/

   *error = FALSE;

   /*============================================*/
   /* If token is a right parenthesis, then fact */
   /* template definition is complete.           */
   /*============================================*/

   GetToken(theEnv,inputSource,tempToken);
   if (tempToken->type == endType)
     { return(NULL); }

   /*=======================================*/
   /* Put a space between the template name */
   /* and the first slot definition.        */
   /*=======================================*/

   PPBackup(theEnv);
   SavePPBuffer(theEnv," ");
   SavePPBuffer(theEnv,tempToken->printForm);

   /*=======================================================*/
   /* Slot definition begins with opening left parenthesis. */
   /*=======================================================*/

   if (tempToken->type != LPAREN)
     {
      SyntaxErrorMessage(theEnv,"deftemplate pattern");
      *error = TRUE;
      return(NULL);
     }

   /*=============================*/
   /* Slot name must be a symbol. */
   /*=============================*/

   GetToken(theEnv,inputSource,tempToken);
   if (tempToken->type != SYMBOL)
     {
      SyntaxErrorMessage(theEnv,"deftemplate pattern");
      *error = TRUE;
      return(NULL);
     }

   /*======================================================*/
   /* Check that the slot name is valid for this template. */
   /*======================================================*/

   if ((slotPtr = FindSlot(theDeftemplate,(SYMBOL_HN *) tempToken->value,&position)) == NULL)
     {
      InvalidDeftemplateSlotMessage(theEnv,ValueToString(tempToken->value),
                                    ValueToString(theDeftemplate->header.name),TRUE);
      *error = TRUE;
      return(NULL);
     }

   /*====================================*/
   /* Return a pointer to the slot name. */
   /*====================================*/

   return(slotPtr);
  }

/**************************************************************************/
/* ParseAssertSlotValues: Gets a single assert slot value for a template. */
/**************************************************************************/
static struct expr *ParseAssertSlotValues(
  void *theEnv,
  char *inputSource,
  struct token *tempToken,
  struct templateSlot *slotPtr,
  int *error,
  int constantsOnly)
  {
   struct expr *nextSlot;
   struct expr *newField, *valueList, *lastValue;
   int printError;

   /*=============================*/
   /* Handle a single field slot. */
   /*=============================*/

   if (slotPtr->multislot == FALSE)
     {
      /*=====================*/
      /* Get the slot value. */
      /*=====================*/

      SavePPBuffer(theEnv," ");

      newField = GetAssertArgument(theEnv,inputSource,tempToken,
                                   error,RPAREN,constantsOnly,&printError);
      if (*error)
        {
         if (printError) SyntaxErrorMessage(theEnv,"deftemplate pattern");
         return(NULL);
        }

      /*=================================================*/
      /* A single field slot value must contain a value. */
      /* Only a multifield slot can be empty.            */
      /*=================================================*/

      if (newField == NULL)
       {
        *error = TRUE;
        SingleFieldSlotCardinalityError(theEnv,slotPtr->slotName->contents);
        return(NULL);
       }

      /*==============================================*/
      /* A function returning a multifield value can  */
      /* not be called to get the value for the slot. */
      /*==============================================*/

      if ((newField->type == FCALL) ? (ExpressionFunctionType(newField) == 'm') :
                                      (newField->type == MF_VARIABLE))
       {
        *error = TRUE;
        SingleFieldSlotCardinalityError(theEnv,slotPtr->slotName->contents);
        ReturnExpression(theEnv,newField);
        return(NULL);
       }

      /*============================*/
      /* Move on to the next token. */
      /*============================*/

      GetToken(theEnv,inputSource,tempToken);
     }

   /*========================================*/
   /* Handle a multifield slot. Build a list */
   /* of the values stored in the slot.      */
   /*========================================*/

   else
     {
      SavePPBuffer(theEnv," ");
      valueList = GetAssertArgument(theEnv,inputSource,tempToken,
                                     error,RPAREN,constantsOnly,&printError);
      if (*error)
        {
         if (printError) SyntaxErrorMessage(theEnv,"deftemplate pattern");
         return(NULL);
        }

      if (valueList == NULL)
        {
         PPBackup(theEnv);
         PPBackup(theEnv);
         SavePPBuffer(theEnv,")");
        }

      lastValue = valueList;

      while (lastValue != NULL) /* (tempToken->type != RPAREN) */
        {
         if (tempToken->type == RPAREN)
           { SavePPBuffer(theEnv," "); }
         else
           {
            /* PPBackup(theEnv); */
            SavePPBuffer(theEnv," ");
            /* SavePPBuffer(theEnv,tempToken->printForm); */
           }

         newField = GetAssertArgument(theEnv,inputSource,tempToken,error,RPAREN,constantsOnly,&printError);
         if (*error)
           {
            if (printError) SyntaxErrorMessage(theEnv,"deftemplate pattern");
            ReturnExpression(theEnv,valueList);
            return(NULL);
           }

         if (newField == NULL)
           {
            PPBackup(theEnv);
            PPBackup(theEnv);
            SavePPBuffer(theEnv,")");
           }

         lastValue->nextArg = newField;
         lastValue = newField;
        }

      newField = valueList;
     }

   /*==========================================================*/
   /* Slot definition must be closed with a right parenthesis. */
   /*==========================================================*/

   if (tempToken->type != RPAREN)
     {
      SingleFieldSlotCardinalityError(theEnv,slotPtr->slotName->contents);
      *error = TRUE;
      ReturnExpression(theEnv,newField);
      return(NULL);
     }

   /*=========================================================*/
   /* Build and return a structure describing the slot value. */
   /*=========================================================*/

   nextSlot = GenConstant(theEnv,SYMBOL,slotPtr->slotName);
   nextSlot->argList = newField;

   return(nextSlot);
  }

/*************************************************************************/
/* ReorderAssertSlotValues: Rearranges the asserted values to correspond */
/*   to the order of the values described by the deftemplate.            */
/*************************************************************************/
static struct expr *ReorderAssertSlotValues(
  void *theEnv,
  struct templateSlot *slotPtr,
  struct expr *firstSlot,
  int *error)
  {
   struct expr *firstArg = NULL;
   struct expr *lastArg = NULL, *newArg;

   /*=============================================*/
   /* Loop through each of the slots in the order */
   /* they're found in the deftemplate.           */
   /*=============================================*/

   for (;
        slotPtr != NULL;
        slotPtr = slotPtr->next)
     {
      /*==============================================*/
      /* Get either the value specified in the assert */
      /* command or the default value for the slot.   */
      /*==============================================*/

      newArg = GetSlotAssertValues(theEnv,slotPtr,firstSlot,error);

      if (*error)
        {
         ReturnExpression(theEnv,firstArg);
         return(NULL);
        }

      /*=====================================*/
      /* Add the value to the list of values */
      /* for the assert command.             */
      /*=====================================*/

      if (newArg != NULL)
        {
         if (lastArg == NULL)
           { firstArg = newArg; }
         else
           { lastArg->nextArg = newArg; }

         lastArg = newArg;
        }
     }

   /*==============================*/
   /* Return the list of arguments */
   /* for the assert command.      */
   /*==============================*/

   return(firstArg);
  }

/***************************************************************/
/* GetSlotAssertValues: Gets the assert value for a given slot */
/*   of a deftemplate. If the value was supplied by the user,  */
/*   it will be used. If not the default value or default      */
/*   default value will be used.                               */
/***************************************************************/
static struct expr *GetSlotAssertValues(
  void *theEnv,
  struct templateSlot *slotPtr,
  struct expr *firstSlot,
  int *error)
  {
   struct expr *slotItem;
   struct expr *newArg, *tempArg;
   DATA_OBJECT theDefault;
   char *nullBitMap = "\0";

   /*==================================================*/
   /* Determine if the slot is assigned in the assert. */
   /*==================================================*/

   slotItem = FindAssertSlotItem(slotPtr,firstSlot);

   /*==========================================*/
   /* If the slot is assigned, use that value. */
   /*==========================================*/

   if (slotItem != NULL)
     {
      newArg = slotItem->argList;
      slotItem->argList = NULL;
     }

   /*=================================*/
   /* Otherwise, use a default value. */
   /*=================================*/

   else
     {
      /*================================================*/
      /* If the (default ?NONE) attribute was specified */
      /* for the slot, then a value must be supplied.   */
      /*================================================*/

      if (slotPtr->noDefault)
        {
         PrintErrorID(theEnv,"TMPLTRHS",1,TRUE);
         EnvPrintRouter(theEnv,WERROR,"Slot ");
         EnvPrintRouter(theEnv,WERROR,slotPtr->slotName->contents);
         EnvPrintRouter(theEnv,WERROR," requires a value because of its (default ?NONE) attribute.\n");
         *error = TRUE;
         return(NULL);
        }

      /*===================================================*/
      /* If the (default ?DERIVE) attribute was specified  */
      /* (the default), then derive the default value from */
      /* the slot's constraints.                           */
      /*===================================================*/

      else if ((slotPtr->defaultPresent == FALSE) &&
               (slotPtr->defaultDynamic == FALSE))
        {
         DeriveDefaultFromConstraints(theEnv,slotPtr->constraints,&theDefault,
                                      (int) slotPtr->multislot,TRUE);
         newArg = ConvertValueToExpression(theEnv,&theDefault);
        }

      /*=========================================*/
      /* Otherwise, use the expression contained */
      /* in the default attribute.               */
      /*=========================================*/

      else
        { newArg = CopyExpression(theEnv,slotPtr->defaultList); }
     }

   /*=======================================================*/
   /* Since a multifield slot default can contain a list of */
   /* values, the values need to have a store-multifield    */
   /* function called wrapped around it to group all of the */
   /* values into a single multifield value.                */
   /*=======================================================*/

   if (slotPtr->multislot)
     {
      tempArg = GenConstant(theEnv,FACT_STORE_MULTIFIELD,EnvAddBitMap(theEnv,(void *) nullBitMap,1));
      tempArg->argList = newArg;
      newArg = tempArg;
     }

   /*==============================================*/
   /* Return the value to be asserted in the slot. */
   /*==============================================*/

   return(newArg);
  }

/*******************************************************************/
/* FindAssertSlotItem: Finds a particular slot in a list of slots. */
/*******************************************************************/
static struct expr *FindAssertSlotItem(
  struct templateSlot *slotPtr,
  struct expr *listOfSlots)
  {
   while (listOfSlots != NULL)
     {
      if (listOfSlots->value == (void *) slotPtr->slotName) return (listOfSlots);
      listOfSlots = listOfSlots->nextArg;
     }

   return(NULL);
  }

#endif /* DEFTEMPLATE_CONSTRUCT */

