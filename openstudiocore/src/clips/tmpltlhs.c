   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*                DEFTEMPLATE LHS MODULE               */
   /*******************************************************/

/*************************************************************/
/* Purpose: Parses LHS deftemplate patterns.                 */
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
/*************************************************************/

#define _TMPLTLHS_SOURCE_

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT && DEFRULE_CONSTRUCT && (! RUN_TIME) && (! BLOAD_ONLY)

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <string.h>

#include "constant.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "symbol.h"
#include "scanner.h"
#include "exprnpsr.h"
#include "router.h"
#include "constrnt.h"
#include "constrct.h"
#include "reorder.h"
#include "pattern.h"
#include "factrhs.h"
#include "modulutl.h"
#include "tmpltutl.h"
#include "tmpltdef.h"

#include "tmpltlhs.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static struct lhsParseNode    *GetLHSSlots(void *,char *,struct token *,struct deftemplate *,int *);
   static struct lhsParseNode    *GetSingleLHSSlot(void *,char *,struct token *,
                                                   struct templateSlot *,int *,short);
   static intBool                 MultiplyDefinedLHSSlots(void *,struct lhsParseNode *,SYMBOL_HN *);

/*********************************************/
/* DeftemplateLHSParse: Parses a LHS pattern */
/*   that uses the deftemplate format.       */
/*********************************************/
globle struct lhsParseNode *DeftemplateLHSParse(
  void *theEnv,
  char *readSource,
  struct deftemplate *theDeftemplate)
  {
   struct lhsParseNode *head, *firstSlot;
   struct token theToken;
   int error;
   
   /*===============================================================*/
   /* Make sure the deftemplate name is not connected to subfields. */
   /*===============================================================*/

   GetToken(theEnv,readSource,&theToken);
   if ((theToken.type == OR_CONSTRAINT) || (theToken.type == AND_CONSTRAINT))
     {
      SyntaxErrorMessage(theEnv,"deftemplate patterns");
      return(NULL);
     }

   /*===================================================*/
   /* Create the pattern node for the deftemplate name. */
   /*===================================================*/

   head = GetLHSParseNode(theEnv);
   head->type = SF_WILDCARD;
   head->negated = FALSE;
   head->exists = FALSE;
   head->index = 0;
   head->slotNumber = 1;
   head->bottom = GetLHSParseNode(theEnv);
   head->bottom->type = SYMBOL;
   head->bottom->negated = FALSE;
   head->bottom->exists = FALSE;
   head->bottom->value = (void *) theDeftemplate->header.name;

   /*==========================================*/
   /* Get the other fields in the deftemplate. */
   /*==========================================*/

   error = FALSE;
   firstSlot = GetLHSSlots(theEnv,readSource,&theToken,theDeftemplate,&error);
   if (error)
     {
      ReturnLHSParseNodes(theEnv,firstSlot);
      ReturnLHSParseNodes(theEnv,head);
      return(NULL);
     }

   /*=========================*/
   /* Return the LHS pattern. */
   /*=========================*/

   head->right = firstSlot;
   return(head);
  }

/******************************************/
/* GetLHSSlots: Retrieves all of the slot */
/*   values used in a LHS pattern.        */
/******************************************/
static struct lhsParseNode *GetLHSSlots(
  void *theEnv,
  char *readSource,
  struct token *tempToken,
  struct deftemplate *theDeftemplate,
  int *error)
  {
   struct lhsParseNode *firstSlot = NULL, *nextSlot, *lastSlot = NULL;
   struct templateSlot *slotPtr;
   short position;

   /*=======================================================*/
   /* Continue parsing slot definitions until the pattern's */
   /* closing right parenthesis is encountered.             */
   /*=======================================================*/

   while (tempToken->type != RPAREN)
     {
      PPBackup(theEnv);
      SavePPBuffer(theEnv," ");
      SavePPBuffer(theEnv,tempToken->printForm);

      /*=================================================*/
      /* Slot definitions begin with a left parenthesis. */
      /*=================================================*/

      if (tempToken->type != LPAREN)
        {
         *error = TRUE;
         SyntaxErrorMessage(theEnv,"deftemplate patterns");
         ReturnLHSParseNodes(theEnv,firstSlot);
         return(NULL);
        }

      /*====================*/
      /* Get the slot name. */
      /*====================*/

      GetToken(theEnv,readSource,tempToken);
      if (tempToken->type != SYMBOL)
        {
         *error = TRUE;
         SyntaxErrorMessage(theEnv,"deftemplate patterns");
         ReturnLHSParseNodes(theEnv,firstSlot);
         return(NULL);
        }

      /*==========================================================*/
      /* Determine if the slot name is valid for the deftemplate. */
      /*==========================================================*/

      if ((slotPtr = FindSlot(theDeftemplate,(SYMBOL_HN *) tempToken->value,&position)) == NULL)
        {
         *error = TRUE;
         InvalidDeftemplateSlotMessage(theEnv,ValueToString(tempToken->value),
                                       ValueToString(theDeftemplate->header.name),TRUE);
         ReturnLHSParseNodes(theEnv,firstSlot);
         return(NULL);
        }

      /*============================================*/
      /* Determine if the slot is multiply defined. */
      /*============================================*/

      if (MultiplyDefinedLHSSlots(theEnv,firstSlot,(SYMBOL_HN *) tempToken->value) == TRUE)
        {
         *error = TRUE;
         ReturnLHSParseNodes(theEnv,firstSlot);
         return(NULL);
        }

      /*==============================================================*/
      /* Get the pattern matching values used in the slot definition. */
      /*==============================================================*/

      nextSlot = GetSingleLHSSlot(theEnv,readSource,tempToken,slotPtr,error,(short) (position+1));
      if (*error)
        {
         ReturnLHSParseNodes(theEnv,firstSlot);
         ReturnLHSParseNodes(theEnv,nextSlot);
         return(NULL);
        }

      /*=====================================*/
      /* Add the slot definition to the list */
      /* of slot definitions already parsed. */
      /*=====================================*/

      if (lastSlot == NULL)
        { firstSlot = nextSlot; }
      else
        { lastSlot->right = nextSlot; }

      while (nextSlot->right != NULL) nextSlot = nextSlot->right;
      lastSlot = nextSlot;

      /*==============================*/
      /* Begin parsing the next slot. */
      /*==============================*/

      GetToken(theEnv,readSource,tempToken);
     }

   /*===========================================================*/
   /* Return all the slot definitions found in the lhs pattern. */
   /*===========================================================*/

   return(firstSlot);
  }

/*****************************************************/
/* GetSingleLHSSlot: Get the pattern matching values */
/*   to be associated with a slot name.              */
/*****************************************************/
static struct lhsParseNode *GetSingleLHSSlot(
  void *theEnv,
  char *readSource,
  struct token *tempToken,
  struct templateSlot *slotPtr,
  int *error,
  short position)
  {
   struct lhsParseNode *nextSlot;
   SYMBOL_HN *slotName;

   /*================================================*/
   /* Get the slot name and read in the first token. */
   /*================================================*/

   slotName = (SYMBOL_HN *) tempToken->value;
   SavePPBuffer(theEnv," ");
   GetToken(theEnv,readSource,tempToken);

   /*====================================*/
   /* Get value for a single field slot. */
   /*====================================*/

   if (slotPtr->multislot == FALSE)
     {
      /*=======================*/
      /* Get the single value. */
      /*=======================*/

      nextSlot = RestrictionParse(theEnv,readSource,tempToken,FALSE,
                                  slotPtr->slotName,(short) (position - 1),
                                  slotPtr->constraints,0);
      if (nextSlot == NULL)
        {
         *error = TRUE;
         return(NULL);
        }

      /*======================================*/
      /* Multi field wildcards and variables  */
      /* not allowed in a single field slot.  */
      /*======================================*/

      if ((nextSlot->type == MF_VARIABLE) ||
          (nextSlot->type == MULTIFIELD))
        {
         SingleFieldSlotCardinalityError(theEnv,slotPtr->slotName->contents);
         *error = TRUE;
         ReturnLHSParseNodes(theEnv,nextSlot);
         return(NULL);
        }
     }

   /*===================================*/
   /* Get values for a multifield slot. */
   /*===================================*/

   else
     {
      nextSlot = RestrictionParse(theEnv,readSource,tempToken,TRUE,slotName,(short) (position - 1),
                                  slotPtr->constraints,1);
      if (nextSlot == NULL)
        {
         *error = TRUE;
         return(NULL);
        }
     }

   /*========================================================*/
   /* The slot definition must end with a right parenthesis. */
   /*========================================================*/

   if (tempToken->type != RPAREN)
     {
      PPBackup(theEnv);
      SavePPBuffer(theEnv," ");
      SavePPBuffer(theEnv,tempToken->printForm);
      SyntaxErrorMessage(theEnv,"deftemplate patterns");
      *error = TRUE;
      ReturnLHSParseNodes(theEnv,nextSlot);
      return(NULL);
     }

   /*===============================================*/
   /* Fix the pretty print output if the multifield */
   /* slot contained no restrictions.               */
   /*===============================================*/

   if ((nextSlot->bottom == NULL) && slotPtr->multislot)
     {
      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv,")");
     }

   /*=================================*/
   /* Add the slot values to the slot */
   /* structure and return it.        */
   /*=================================*/

   return(nextSlot);
  }

/******************************************************/
/* MultiplyDefinedLHSSlots: Determines if a slot name */
/*   was used more than once in a LHS pattern.        */
/******************************************************/
static intBool MultiplyDefinedLHSSlots(
  void *theEnv,
  struct lhsParseNode *theSlots,
  SYMBOL_HN *slotName)
  {
   for (;
        theSlots != NULL;
        theSlots = theSlots->right)
     {
      if (theSlots->slot == slotName)
        {
         AlreadyParsedErrorMessage(theEnv,"slot ",ValueToString(slotName));
         return(TRUE);
        }
     }

   return(FALSE);
  }

#endif /* DEFTEMPLATE_CONSTRUCT && DEFRULE_CONSTRUCT && (! RUN_TIME) && (! BLOAD_ONLY) */


