   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.22  06/15/04            */
   /*                                                     */
   /*            FACT LHS PATTERN PARSING MODULE          */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains routines for integration of ordered and */
/*   deftemplate fact patterns with the defrule LHS pattern  */
/*   parser including routines for recognizing fact          */
/*   patterns, parsing ordered fact patterns, initiating the */
/*   parsing of deftemplate fact patterns, and creating the  */
/*   default initial-fact fact pattern.                      */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#define _FACTLHS_SOURCE_

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT && DEFRULE_CONSTRUCT && (! RUN_TIME) && (! BLOAD_ONLY)

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "cstrcpsr.h"
#include "envrnmnt.h"
#include "pattern.h"
#include "router.h"
#include "reorder.h"
#include "tmpltpsr.h"
#include "tmpltdef.h"
#include "tmpltlhs.h"
#include "tmpltutl.h"
#include "modulutl.h"
#include "modulpsr.h"

#include "factlhs.h"

/***********************************************/
/* SequenceRestrictionParse: Parses an ordered */
/*   fact pattern conditional element.         */
/*                                             */
/*   <ordered-fact-pattern-CE>                 */
/*             ::= (<symbol> <constraint>+)    */
/***********************************************/
globle struct lhsParseNode *SequenceRestrictionParse(
  void *theEnv,
  char *readSource,
  struct token *theToken)
  {
   struct lhsParseNode *topNode;
   struct lhsParseNode *nextField;

   /*================================================*/
   /* Create the pattern node for the relation name. */
   /*================================================*/

   topNode = GetLHSParseNode(theEnv);
   topNode->type = SF_WILDCARD;
   topNode->negated = FALSE;
   topNode->exists = FALSE;
   topNode->index = -1;
   topNode->slotNumber = 1;
   topNode->bottom = GetLHSParseNode(theEnv);
   topNode->bottom->type = SYMBOL;
   topNode->bottom->negated = FALSE;
   topNode->bottom->exists = FALSE;
   topNode->bottom->value = (void *) theToken->value;

   /*======================================================*/
   /* Connective constraints cannot be used in conjunction */
   /* with the first field of a pattern.                   */
   /*======================================================*/

   SavePPBuffer(theEnv," ");
   GetToken(theEnv,readSource,theToken);
   if ((theToken->type == OR_CONSTRAINT) || (theToken->type == AND_CONSTRAINT))
     {
      ReturnLHSParseNodes(theEnv,topNode);
      SyntaxErrorMessage(theEnv,"the first field of a pattern");
      return(NULL);
     }

   /*============================================================*/
   /* Treat the remaining constraints of an ordered fact pattern */
   /* as if they were contained in a multifield slot.            */
   /*============================================================*/

   nextField = RestrictionParse(theEnv,readSource,theToken,TRUE,NULL,1,NULL,1);
   if (nextField == NULL)
     {
      ReturnLHSParseNodes(theEnv,topNode);
      return(NULL);
     }
   topNode->right = nextField;

   /*================================================*/
   /* The pattern must end with a right parenthesis. */
   /*================================================*/

   if (theToken->type != RPAREN)
     {
      PPBackup(theEnv);
      SavePPBuffer(theEnv," ");
      SavePPBuffer(theEnv,theToken->printForm);
      SyntaxErrorMessage(theEnv,"fact patterns");
      ReturnLHSParseNodes(theEnv,topNode);
      return(NULL);
     }

   /*====================================*/
   /* Fix the pretty print output if the */
   /* slot contained no restrictions.    */
   /*====================================*/

   if (nextField->bottom == NULL)
     {
      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv,")");
     }

   /*===================================*/
   /* If no errors, return the pattern. */
   /*===================================*/

   return(topNode);
  }

/****************************************************************/
/* CreateInitialFactPattern: Creates the pattern (initial-fact) */
/*   for use in rules which have no LHS patterns.               */
/****************************************************************/
globle struct lhsParseNode *CreateInitialFactPattern(
  void *theEnv)
  {
   struct lhsParseNode *topNode;
   struct deftemplate *theDeftemplate;
   int count;
   
   /*==================================*/
   /* If the initial-fact deftemplate  */
   /* doesn't exist, then create it.   */
   /*==================================*/

   theDeftemplate = (struct deftemplate *)
                    FindImportedConstruct(theEnv,"deftemplate",NULL,"initial-fact",
                                          &count,TRUE,NULL);
   if (theDeftemplate == NULL)
     {
      PrintWarningID(theEnv,"FACTLHS",1,FALSE);
      EnvPrintRouter(theEnv,WWARNING,"Creating implied initial-fact deftemplate in module ");
      EnvPrintRouter(theEnv,WWARNING,EnvGetDefmoduleName(theEnv,EnvGetCurrentModule(theEnv)));
      EnvPrintRouter(theEnv,WWARNING,".\n");
      EnvPrintRouter(theEnv,WWARNING,"  You probably want to import this deftemplate from the MAIN module.\n");
      CreateImpliedDeftemplate(theEnv,(SYMBOL_HN *) EnvAddSymbol(theEnv,"initial-fact"),FALSE);
     }

   /*====================================*/
   /* Create the (initial-fact) pattern. */
   /*====================================*/

   topNode = GetLHSParseNode(theEnv);
   topNode->type = SF_WILDCARD;
   topNode->index = 0;
   topNode->slotNumber = 1;

   topNode->bottom = GetLHSParseNode(theEnv);
   topNode->bottom->type = SYMBOL;
   topNode->bottom->value = (void *) EnvAddSymbol(theEnv,"initial-fact");

   /*=====================*/
   /* Return the pattern. */
   /*=====================*/

   return(topNode);
  }

/**********************************************************************/
/* FactPatternParserFind: This function is the pattern find function  */
/*   for facts. It tells the pattern parsing code that the specified  */
/*   pattern can be parsed as a fact pattern. By default, any pattern */
/*   beginning with a symbol can be parsed as a fact pattern. Since   */
/*   all patterns begin with a symbol, it follows that all patterns   */
/*   can be parsed as a fact pattern.                                 */
/**********************************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle int FactPatternParserFind(
  SYMBOL_HN *theRelation)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theRelation)
#endif
   return(TRUE);
  }

/******************************************************/
/* FactPatternParse: This function is called to parse */
/*  both deftemplate and ordered fact patterns.       */
/******************************************************/
globle struct lhsParseNode *FactPatternParse(
  void *theEnv,
  char *readSource,
  struct token *theToken)
  {
   struct deftemplate *theDeftemplate;
   int count;

   /*=========================================*/
   /* A module separator can not be included  */
   /* as part of the pattern's relation name. */
   /*=========================================*/

   if (FindModuleSeparator(ValueToString(theToken->value)))
     {
      IllegalModuleSpecifierMessage(theEnv);
      return(NULL);
     }

   /*=========================================================*/
   /* Find the deftemplate associated with the relation name. */
   /*=========================================================*/

   theDeftemplate = (struct deftemplate *)
                    FindImportedConstruct(theEnv,"deftemplate",NULL,ValueToString(theToken->value),
                                          &count,TRUE,NULL);

   if (count > 1)
     {
      AmbiguousReferenceErrorMessage(theEnv,"deftemplate",ValueToString(theToken->value));
      return(NULL);
     }

   /*======================================================*/
   /* If no deftemplate exists with the specified relation */
   /* name, then create an implied deftemplate.            */
   /*======================================================*/

   if (theDeftemplate == NULL)
     {
#if DEFMODULE_CONSTRUCT
      if (FindImportExportConflict(theEnv,"deftemplate",((struct defmodule *) EnvGetCurrentModule(theEnv)),ValueToString(theToken->value)))
        {
         ImportExportConflictMessage(theEnv,"implied deftemplate",ValueToString(theToken->value),NULL,NULL);
         return(NULL);
        }
#endif /* DEFMODULE_CONSTRUCT */

      if (! ConstructData(theEnv)->CheckSyntaxMode)
        { theDeftemplate = CreateImpliedDeftemplate(theEnv,(SYMBOL_HN *) theToken->value,TRUE); }
      else
        { theDeftemplate = NULL; }
     }

   /*===============================================*/
   /* If an explicit deftemplate exists, then parse */
   /* the pattern as a deftemplate pattern.         */
   /*===============================================*/

   if ((theDeftemplate != NULL) && (theDeftemplate->implied == FALSE))
     { return(DeftemplateLHSParse(theEnv,readSource,theDeftemplate)); }

   /*================================*/
   /* Parse an ordered fact pattern. */
   /*================================*/

   return(SequenceRestrictionParse(theEnv,readSource,theToken));
  }

#endif /* DEFTEMPLATE_CONSTRUCT && DEFRULE_CONSTRUCT && (! RUN_TIME) && (! BLOAD_ONLY) */



