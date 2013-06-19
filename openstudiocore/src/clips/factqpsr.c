   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
   /*                                                     */
   /*            FACT-SET QUERIES PARSER MODULE           */
   /*******************************************************/

/*************************************************************/
/* Purpose: Fact_set Queries Parsing Routines                */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Gary D. Riley                                        */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Added fact-set queries.                        */
/*                                                           */
/*            Changed name of variable exp to theExp         */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if FACT_SET_QUERIES && (! RUN_TIME)

#include <string.h>

#include "envrnmnt.h"
#include "exprnpsr.h"
#include "extnfunc.h"
#include "factqury.h"
#include "modulutl.h"
#include "prcdrpsr.h"
#include "prntutil.h"
#include "router.h"
#include "scanner.h"
#include "strngrtr.h"

#define _FACTQPSR_SOURCE_
#include "factqpsr.h"

/* =========================================
   *****************************************
                   CONSTANTS
   =========================================
   ***************************************** */
#define FACT_SLOT_REF ':'

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */
   
/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static EXPRESSION             *ParseQueryRestrictions(void *,EXPRESSION *,char *,struct token *);
   static intBool                 ReplaceTemplateNameWithReference(void *,EXPRESSION *);
   static int                     ParseQueryTestExpression(void *,EXPRESSION *,char *);
   static int                     ParseQueryActionExpression(void *,EXPRESSION *,char *,EXPRESSION *,struct token *);
   static void                    ReplaceFactVariables(void *,EXPRESSION *,EXPRESSION *,int,int);
   static void                    ReplaceSlotReference(void *,EXPRESSION *,EXPRESSION *,
                                                       struct FunctionDefinition *,int);
   static int                     IsQueryFunction(EXPRESSION *);

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***********************************************************************
  NAME         : FactParseQueryNoAction
  DESCRIPTION  : Parses the following functions :
                   (any-factp)
                   (find-first-fact)
                   (find-all-facts)
  INPUTS       : 1) The address of the top node of the query function
                 2) The logical name of the input
  RETURNS      : The completed expression chain, or NULL on errors
  SIDE EFFECTS : The expression chain is extended, or the "top" node
                 is deleted on errors
  NOTES        : H/L Syntax :

                 (<function> <query-block>)

                 <query-block>  :== (<fact-var>+) <query-expression>
                 <fact-var> :== (<var-name> <template-name>+)

                 Parses into following form :

                 <query-function>
                      |
                      V
                 <query-expression>  ->

                 <template-1a> -> <template-1b> -> (QDS) ->

                 <template-2a> -> <template-2b> -> (QDS) -> ...
 ***********************************************************************/
globle EXPRESSION *FactParseQueryNoAction(
  void *theEnv,
  EXPRESSION *top,
  char *readSource)
  {
   EXPRESSION *factQuerySetVars;
   struct token queryInputToken;

   factQuerySetVars = ParseQueryRestrictions(theEnv,top,readSource,&queryInputToken);
   if (factQuerySetVars == NULL)
     { return(NULL); }
     
   IncrementIndentDepth(theEnv,3);
   PPCRAndIndent(theEnv);
   
   if (ParseQueryTestExpression(theEnv,top,readSource) == FALSE)
     {
      DecrementIndentDepth(theEnv,3);
      ReturnExpression(theEnv,factQuerySetVars);
      return(NULL);
     }
     
   DecrementIndentDepth(theEnv,3);
   
   GetToken(theEnv,readSource,&queryInputToken);   
   if (GetType(queryInputToken) != RPAREN)
     {
      SyntaxErrorMessage(theEnv,"fact-set query function");
      ReturnExpression(theEnv,top);
      ReturnExpression(theEnv,factQuerySetVars);
      return(NULL);
     }
     
   ReplaceFactVariables(theEnv,factQuerySetVars,top->argList,TRUE,0);
   ReturnExpression(theEnv,factQuerySetVars);
   
   return(top);
  }

/***********************************************************************
  NAME         : FactParseQueryAction
  DESCRIPTION  : Parses the following functions :
                   (do-for-fact)
                   (do-for-all-facts)
                   (delayed-do-for-all-facts)
  INPUTS       : 1) The address of the top node of the query function
                 2) The logical name of the input
  RETURNS      : The completed expression chain, or NULL on errors
  SIDE EFFECTS : The expression chain is extended, or the "top" node
                 is deleted on errors
  NOTES        : H/L Syntax :

                 (<function> <query-block> <query-action>)

                 <query-block>  :== (<fact-var>+) <query-expression>
                 <fact-var> :== (<var-name> <template-name>+)

                 Parses into following form :

                 <query-function>
                      |
                      V
                 <query-expression> -> <query-action>  ->

                 <template-1a> -> <template-1b> -> (QDS) ->

                 <template-2a> -> <template-2b> -> (QDS) -> ...
 ***********************************************************************/
globle EXPRESSION *FactParseQueryAction(
  void *theEnv,
  EXPRESSION *top,
  char *readSource)
  {
   EXPRESSION *factQuerySetVars;
   struct token queryInputToken;

   factQuerySetVars = ParseQueryRestrictions(theEnv,top,readSource,&queryInputToken);
   if (factQuerySetVars == NULL)
     { return(NULL); }
     
   IncrementIndentDepth(theEnv,3);
   PPCRAndIndent(theEnv);
   
   if (ParseQueryTestExpression(theEnv,top,readSource) == FALSE)
     {
      DecrementIndentDepth(theEnv,3);
      ReturnExpression(theEnv,factQuerySetVars);
      return(NULL);
     }
     
   PPCRAndIndent(theEnv);
   
   if (ParseQueryActionExpression(theEnv,top,readSource,factQuerySetVars,&queryInputToken) == FALSE)
     {
      DecrementIndentDepth(theEnv,3);
      ReturnExpression(theEnv,factQuerySetVars);
      return(NULL);
     }
     
   DecrementIndentDepth(theEnv,3);
   
   if (GetType(queryInputToken) != RPAREN)
     {
      SyntaxErrorMessage(theEnv,"fact-set query function");
      ReturnExpression(theEnv,top);
      ReturnExpression(theEnv,factQuerySetVars);
      return(NULL);
     }
     
   ReplaceFactVariables(theEnv,factQuerySetVars,top->argList,TRUE,0);
   ReplaceFactVariables(theEnv,factQuerySetVars,top->argList->nextArg,FALSE,0);
   ReturnExpression(theEnv,factQuerySetVars);
   
   return(top);
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***************************************************************
  NAME         : ParseQueryRestrictions
  DESCRIPTION  : Parses the template restrictions for a query
  INPUTS       : 1) The top node of the query expression
                 2) The logical name of the input
                 3) Caller's token buffer
  RETURNS      : The fact-variable expressions
  SIDE EFFECTS : Entire query expression deleted on errors
                 Nodes allocated for restrictions and fact
                   variable expressions
                 Template restrictions attached to query-expression
                   as arguments
  NOTES        : Expects top != NULL
 ***************************************************************/
static EXPRESSION *ParseQueryRestrictions(
  void *theEnv,
  EXPRESSION *top,
  char *readSource,
  struct token *queryInputToken)
  {
   EXPRESSION *factQuerySetVars = NULL,*lastFactQuerySetVars = NULL,
              *templateExp = NULL,*lastTemplateExp,
              *tmp,*lastOne = NULL;
   int error = FALSE;

   SavePPBuffer(theEnv," ");
   
   GetToken(theEnv,readSource,queryInputToken);
   if (queryInputToken->type != LPAREN)
     { goto ParseQueryRestrictionsError1; }
     
   GetToken(theEnv,readSource,queryInputToken);
   if (queryInputToken->type != LPAREN)
     { goto ParseQueryRestrictionsError1; }
     
   while (queryInputToken->type == LPAREN)
     {
      GetToken(theEnv,readSource,queryInputToken);
      if (queryInputToken->type != SF_VARIABLE)
        { goto ParseQueryRestrictionsError1; }
        
      tmp = factQuerySetVars;
      while (tmp != NULL)
        {
         if (tmp->value == queryInputToken->value)
           {
            PrintErrorID(theEnv,"FACTQPSR",1,FALSE);
            EnvPrintRouter(theEnv,WERROR,"Duplicate fact member variable name in function ");
            EnvPrintRouter(theEnv,WERROR,ValueToString(ExpressionFunctionCallName(top)));
            EnvPrintRouter(theEnv,WERROR,".\n");
            goto ParseQueryRestrictionsError2;
           }
           
         tmp = tmp->nextArg;
        }
        
      tmp = GenConstant(theEnv,SF_VARIABLE,queryInputToken->value);
      if (factQuerySetVars == NULL)
        { factQuerySetVars = tmp; }
      else
        { lastFactQuerySetVars->nextArg = tmp; }
      
      lastFactQuerySetVars = tmp;
      SavePPBuffer(theEnv," ");
      
      templateExp = ArgumentParse(theEnv,readSource,&error);
      
      if (error)
        { goto ParseQueryRestrictionsError2; }
      
      if (templateExp == NULL)
        { goto ParseQueryRestrictionsError1; }
      
      if (ReplaceTemplateNameWithReference(theEnv,templateExp) == FALSE)
        { goto ParseQueryRestrictionsError2; }
      
      lastTemplateExp = templateExp;
      SavePPBuffer(theEnv," ");
      
      while ((tmp = ArgumentParse(theEnv,readSource,&error)) != NULL)
        {
         if (ReplaceTemplateNameWithReference(theEnv,tmp) == FALSE)
           goto ParseQueryRestrictionsError2;
         lastTemplateExp->nextArg = tmp;
         lastTemplateExp = tmp;
         SavePPBuffer(theEnv," ");
        }
        
      if (error)
        { goto ParseQueryRestrictionsError2; }
        
      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv,")");
      
      tmp = GenConstant(theEnv,SYMBOL,(void *) FactQueryData(theEnv)->QUERY_DELIMETER_SYMBOL);
      
      lastTemplateExp->nextArg = tmp;
      lastTemplateExp = tmp;
      
      if (top->argList == NULL)
        { top->argList = templateExp; }
      else
        { lastOne->nextArg = templateExp; }
      
      lastOne = lastTemplateExp;
      templateExp = NULL;
      SavePPBuffer(theEnv," ");
      GetToken(theEnv,readSource,queryInputToken);
     }
     
   if (queryInputToken->type != RPAREN)
     { goto ParseQueryRestrictionsError1; }
     
   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,")");
   return(factQuerySetVars);

ParseQueryRestrictionsError1:
   SyntaxErrorMessage(theEnv,"fact-set query function");

ParseQueryRestrictionsError2:
   ReturnExpression(theEnv,templateExp);
   ReturnExpression(theEnv,top);
   ReturnExpression(theEnv,factQuerySetVars);
   return(NULL);
  }

/***************************************************
  NAME         : ReplaceTemplateNameWithReference
  DESCRIPTION  : In parsing an fact-set query,
                 this function replaces a constant
                 template name with an actual pointer
                 to the template
  INPUTS       : The expression
  RETURNS      : TRUE if all OK, FALSE
                 if template cannot be found
  SIDE EFFECTS : The expression type and value are
                 modified if template is found
  NOTES        : Searches current and imported
                 modules for reference
 ***************************************************/
static intBool ReplaceTemplateNameWithReference(
  void *theEnv,
  EXPRESSION *theExp)
  {
   char *theTemplateName;
   void *theDeftemplate;
   int count;

   if (theExp->type == SYMBOL)
     {
      theTemplateName = ValueToString(theExp->value);
      
      theDeftemplate = (struct deftemplate *)
                       FindImportedConstruct(theEnv,"deftemplate",NULL,theTemplateName,
                                             &count,TRUE,NULL);

      if (theDeftemplate == NULL)
        {
         CantFindItemErrorMessage(theEnv,"deftemplate",theTemplateName);
         return(FALSE);
        }
        
      if (count > 1)
        {
         AmbiguousReferenceErrorMessage(theEnv,"deftemplate",theTemplateName);
         return(FALSE);
        }

      theExp->type = DEFTEMPLATE_PTR;
      theExp->value = theDeftemplate;
     }
   return(TRUE);
  }

/*************************************************************
  NAME         : ParseQueryTestExpression
  DESCRIPTION  : Parses the test-expression for a query
  INPUTS       : 1) The top node of the query expression
                 2) The logical name of the input
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : Entire query-expression deleted on errors
                 Nodes allocated for new expression
                 Test shoved in front of class-restrictions on
                    query argument list
  NOTES        : Expects top != NULL
 *************************************************************/
static int ParseQueryTestExpression(
  void *theEnv,
  EXPRESSION *top,
  char *readSource)
  {
   EXPRESSION *qtest;
   int error;
   struct BindInfo *oldBindList;

   error = FALSE;
   oldBindList = GetParsedBindNames(theEnv);
   SetParsedBindNames(theEnv,NULL);
   
   qtest = ArgumentParse(theEnv,readSource,&error);
   
   if (error == TRUE)
     {
      SetParsedBindNames(theEnv,oldBindList);
      ReturnExpression(theEnv,top);
      return(FALSE);
     }
   
   if (qtest == NULL)
     {
      SetParsedBindNames(theEnv,oldBindList);
      SyntaxErrorMessage(theEnv,"fact-set query function");
      ReturnExpression(theEnv,top);
      return(FALSE);
     }
   
   qtest->nextArg = top->argList;
   top->argList = qtest;
   
   if (ParsedBindNamesEmpty(theEnv) == FALSE)
     {
      ClearParsedBindNames(theEnv);
      SetParsedBindNames(theEnv,oldBindList);
      PrintErrorID(theEnv,"FACTQPSR",2,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Binds are not allowed in fact-set query in function ");
      EnvPrintRouter(theEnv,WERROR,ValueToString(ExpressionFunctionCallName(top)));
      EnvPrintRouter(theEnv,WERROR,".\n");
      ReturnExpression(theEnv,top);
      return(FALSE);
     }
     
   SetParsedBindNames(theEnv,oldBindList);
   
   return(TRUE);
  }

/*************************************************************
  NAME         : ParseQueryActionExpression
  DESCRIPTION  : Parses the action-expression for a query
  INPUTS       : 1) The top node of the query expression
                 2) The logical name of the input
                 3) List of query parameters
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : Entire query-expression deleted on errors
                 Nodes allocated for new expression
                 Action shoved in front of template-restrictions
                    and in back of test-expression on query
                    argument list
  NOTES        : Expects top != NULL && top->argList != NULL
 *************************************************************/
static int ParseQueryActionExpression(
  void *theEnv,
  EXPRESSION *top,
  char *readSource,
  EXPRESSION *factQuerySetVars,
  struct token *queryInputToken)
  {
   EXPRESSION *qaction,*tmpFactSetVars;
   int error;
   struct BindInfo *oldBindList,*newBindList,*prev;

   error = FALSE;
   oldBindList = GetParsedBindNames(theEnv);
   SetParsedBindNames(theEnv,NULL);
   
   ExpressionData(theEnv)->BreakContext = TRUE;
   ExpressionData(theEnv)->ReturnContext = ExpressionData(theEnv)->svContexts->rtn;

   qaction = GroupActions(theEnv,readSource,queryInputToken,TRUE,NULL,FALSE);
   
   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,queryInputToken->printForm);

   ExpressionData(theEnv)->BreakContext = FALSE;
   
   if (error == TRUE)
     {
      SetParsedBindNames(theEnv,oldBindList);
      ReturnExpression(theEnv,top);
      return(FALSE);
     }
   
   if (qaction == NULL)
     {
      SetParsedBindNames(theEnv,oldBindList);
      SyntaxErrorMessage(theEnv,"fact-set query function");
      ReturnExpression(theEnv,top);
      return(FALSE);
     }
     
   qaction->nextArg = top->argList->nextArg;
   top->argList->nextArg = qaction;
   
   newBindList = GetParsedBindNames(theEnv);
   prev = NULL;
   while (newBindList != NULL)
     {
      tmpFactSetVars = factQuerySetVars;
      while (tmpFactSetVars != NULL)
        {
         if (tmpFactSetVars->value == (void *) newBindList->name)
           {
            ClearParsedBindNames(theEnv);
            SetParsedBindNames(theEnv,oldBindList);
            PrintErrorID(theEnv,"FACTQPSR",3,FALSE);
            EnvPrintRouter(theEnv,WERROR,"Cannot rebind fact-set member variable ");
            EnvPrintRouter(theEnv,WERROR,ValueToString(tmpFactSetVars->value));
            EnvPrintRouter(theEnv,WERROR," in function ");
            EnvPrintRouter(theEnv,WERROR,ValueToString(ExpressionFunctionCallName(top)));
            EnvPrintRouter(theEnv,WERROR,".\n");
            ReturnExpression(theEnv,top);
            return(FALSE);
           }
         tmpFactSetVars = tmpFactSetVars->nextArg;
        }
      prev = newBindList;
      newBindList = newBindList->next;
     }
     
   if (prev == NULL)
     { SetParsedBindNames(theEnv,oldBindList); }
   else
     { prev->next = oldBindList; }
   
   return(TRUE);
  }

/***********************************************************************************
  NAME         : ReplaceFactVariables
  DESCRIPTION  : Replaces all references to fact-variables within an
                   fact query-function with function calls to query-fact
                   (which references the fact array at run-time)
  INPUTS       : 1) The fact-variable list
                 2) A boolean expression containing variable references
                 3) A flag indicating whether to allow slot references of the type
                    <fact-query-variable>:<slot-name> for direct slot access
                    or not
                 4) Nesting depth of query functions
  RETURNS      : Nothing useful
  SIDE EFFECTS : If a SF_VARIABLE node is found and is on the list of fact
                   variables, it is replaced with a query-fact function call.
  NOTES        : Other SF_VARIABLE(S) are left alone for replacement by other
                   parsers.  This implies that a user may use defgeneric,
                   defrule, and defmessage-handler variables within a query-function
                   where they do not conflict with fact-variable names.
 ***********************************************************************************/
static void ReplaceFactVariables(
  void *theEnv,
  EXPRESSION *vlist,
  EXPRESSION *bexp,
  int sdirect,
  int ndepth)
  {
   EXPRESSION *eptr;
   struct FunctionDefinition *rindx_func,*rslot_func;
   int posn;

   rindx_func = FindFunction(theEnv,"(query-fact)");
   rslot_func = FindFunction(theEnv,"(query-fact-slot)");
   while (bexp != NULL)
     {
      if (bexp->type == SF_VARIABLE)
        {
         eptr = vlist;
         posn = 0;
         while ((eptr != NULL) ? (eptr->value != bexp->value) : FALSE)
           {
            eptr = eptr->nextArg;
            posn++;
           }
         if (eptr != NULL)
           {
            bexp->type = FCALL;
            bexp->value = (void *) rindx_func;
            eptr = GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) ndepth));
            eptr->nextArg = GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) posn));
            bexp->argList = eptr;
           }
         else if (sdirect == TRUE)
           { ReplaceSlotReference(theEnv,vlist,bexp,rslot_func,ndepth); }
        }
      if (bexp->argList != NULL)
        {
         if (IsQueryFunction(bexp))
           { ReplaceFactVariables(theEnv,vlist,bexp->argList,sdirect,ndepth+1); }
         else
           { ReplaceFactVariables(theEnv,vlist,bexp->argList,sdirect,ndepth); }
        }
      bexp = bexp->nextArg;
     }
  }

/*************************************************************************
  NAME         : ReplaceSlotReference
  DESCRIPTION  : Replaces fact-set query function variable
                   references of the form: <fact-variable>:<slot-name>
                   with function calls to get these fact-slots at run
                   time
  INPUTS       : 1) The fact-set variable list
                 2) The expression containing the variable
                 3) The address of the fact slot access function
                 4) Nesting depth of query functions
  RETURNS      : Nothing useful
  SIDE EFFECTS : If the variable is a slot reference, then it is replaced
                   with the appropriate function-call.
  NOTES        : None
 *************************************************************************/
static void ReplaceSlotReference(
  void *theEnv,
  EXPRESSION *vlist,
  EXPRESSION *theExp,
  struct FunctionDefinition *func,
  int ndepth)
  {
   size_t len;
   int posn,oldpp;
   size_t i;
   register char *str;
   EXPRESSION *eptr;
   struct token itkn;

   str = ValueToString(theExp->value);
   len =  strlen(str);
   if (len < 3)
     return;
   for (i = len-2 ; i >= 1 ; i--)
     {
      if ((str[i] == FACT_SLOT_REF) ? (i >= 1) : FALSE)
        {
         eptr = vlist;
         posn = 0;
         while (eptr && ((i != strlen(ValueToString(eptr->value))) ||
                         strncmp(ValueToString(eptr->value),str,
                                 (STD_SIZE) i)))
           {
            eptr = eptr->nextArg;
            posn++;
           }
         if (eptr != NULL)
           {
            OpenStringSource(theEnv,"query-var",str+i+1,0);
            oldpp = GetPPBufferStatus(theEnv);
            SetPPBufferStatus(theEnv,OFF);
            GetToken(theEnv,"query-var",&itkn);
            SetPPBufferStatus(theEnv,oldpp);
            CloseStringSource(theEnv,"query-var");
            theExp->type = FCALL;
            theExp->value = (void *) func;
            theExp->argList = GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) ndepth));
            theExp->argList->nextArg =
              GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) posn));
            theExp->argList->nextArg->nextArg = GenConstant(theEnv,itkn.type,itkn.value);
            break;
           }
        }
     }
  }

/********************************************************************
  NAME         : IsQueryFunction
  DESCRIPTION  : Determines if an expression is a query function call
  INPUTS       : The expression
  RETURNS      : TRUE if query function call, FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ********************************************************************/
static int IsQueryFunction(
  EXPRESSION *theExp)
  {
   int (*fptr)(void);

   if (theExp->type != FCALL)
     return(FALSE);
   fptr = (int (*)(void)) ExpressionFunctionPointer(theExp);

   if (fptr == (int (*)(void)) AnyFacts)
     return(TRUE);
   if (fptr == (int (*)(void)) QueryFindFact)
     return(TRUE);
   if (fptr == (int (*)(void)) QueryFindAllFacts)
     return(TRUE);
   if (fptr == (int (*)(void)) QueryDoForFact)
     return(TRUE);
   if (fptr == (int (*)(void)) QueryDoForAllFacts)
     return(TRUE);
   if (fptr == (int (*)(void)) DelayedQueryDoForAllFacts)
     return(TRUE);

   return(FALSE);
  }

#endif
