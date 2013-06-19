   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*          PROCEDURAL FUNCTIONS PARSER MODULE         */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Changed name of variable exp to theExp         */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#define _PRCDRPSR_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "setup.h"

#include "argacces.h"
#include "constrnt.h"
#include "cstrnchk.h"
#include "cstrnops.h"
#include "cstrnutl.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "memalloc.h"
#include "modulutl.h"
#include "multifld.h"
#include "router.h"
#include "scanner.h"
#include "utility.h"

#include "prcdrpsr.h"

#if DEFGLOBAL_CONSTRUCT
#include "globldef.h"
#include "globlpsr.h"
#endif

#if ! RUN_TIME
#define PRCDRPSR_DATA 12

struct procedureParserData
  { 
   struct BindInfo *ListOfParsedBindNames;
  };

#define ProcedureParserData(theEnv) ((struct procedureParserData *) GetEnvironmentData(theEnv,PRCDRPSR_DATA))
#endif

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if (! RUN_TIME) && (! BLOAD_ONLY)
   static struct expr            *WhileParse(void *,struct expr *,char *);
   static struct expr            *LoopForCountParse(void *,struct expr *,char *);
   static void                    ReplaceLoopCountVars(void *,SYMBOL_HN *,EXPRESSION *,int);
   static struct expr            *IfParse(void *,struct expr *,char *);
   static struct expr            *PrognParse(void *,struct expr *,char *);
   static struct expr            *BindParse(void *,struct expr *,char *);
   static int                     AddBindName(void *,struct symbolHashNode *,CONSTRAINT_RECORD *);
   static struct expr            *ReturnParse(void *,struct expr *,char *);
   static struct expr            *BreakParse(void *,struct expr *,char *);
   static struct expr            *SwitchParse(void *,struct expr *,char *);
   static void                    DeallocateProceduralFunctionData(void *);
#endif

#if ! RUN_TIME
/*******************************************/
/* ProceduralFunctionParsers        */
/*******************************************/
globle void ProceduralFunctionParsers(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,PRCDRPSR_DATA,sizeof(struct procedureParserData),DeallocateProceduralFunctionData);

#if (! BLOAD_ONLY)
   AddFunctionParser(theEnv,"bind",BindParse);
   AddFunctionParser(theEnv,"progn",PrognParse);
   AddFunctionParser(theEnv,"if",IfParse);
   AddFunctionParser(theEnv,"while",WhileParse);
   AddFunctionParser(theEnv,"loop-for-count",LoopForCountParse);
   AddFunctionParser(theEnv,"return",ReturnParse);
   AddFunctionParser(theEnv,"break",BreakParse);
   AddFunctionParser(theEnv,"switch",SwitchParse);
#endif
  }

/*************************************************************/
/* DeallocateProceduralFunctionData: Deallocates environment */
/*    data for procedural functions.                         */
/*************************************************************/
static void DeallocateProceduralFunctionData(
  void *theEnv)
  { 
   struct BindInfo *temp_bind;

   while (ProcedureParserData(theEnv)->ListOfParsedBindNames != NULL)
     {
      temp_bind = ProcedureParserData(theEnv)->ListOfParsedBindNames->next;
      rtn_struct(theEnv,BindInfo,ProcedureParserData(theEnv)->ListOfParsedBindNames);
      ProcedureParserData(theEnv)->ListOfParsedBindNames = temp_bind;
     }
  }

/********************************************************/
/* GetParsedBindNames:                                      */
/********************************************************/
globle struct BindInfo *GetParsedBindNames(
  void *theEnv)
  {
   return(ProcedureParserData(theEnv)->ListOfParsedBindNames);
  }

/********************************************************/
/* SetParsedBindNames:                                      */
/********************************************************/
globle void SetParsedBindNames(
  void *theEnv,
  struct BindInfo *newValue)
  {
   ProcedureParserData(theEnv)->ListOfParsedBindNames = newValue;
  }

/********************************************************/
/* ClearParsedBindNames:                                     */
/********************************************************/
globle void ClearParsedBindNames(
  void *theEnv)
  {
   struct BindInfo *temp_bind;

   while (ProcedureParserData(theEnv)->ListOfParsedBindNames != NULL)
     {
      temp_bind = ProcedureParserData(theEnv)->ListOfParsedBindNames->next;
      RemoveConstraint(theEnv,ProcedureParserData(theEnv)->ListOfParsedBindNames->constraints);
      rtn_struct(theEnv,BindInfo,ProcedureParserData(theEnv)->ListOfParsedBindNames);
      ProcedureParserData(theEnv)->ListOfParsedBindNames = temp_bind;
     }
  }

/********************************************************/
/* ParsedBindNamesEmpty:                                     */
/********************************************************/
globle intBool ParsedBindNamesEmpty(
  void *theEnv)
  {
   if (ProcedureParserData(theEnv)->ListOfParsedBindNames != NULL) return(FALSE);

   return(TRUE);
  }

#if (! BLOAD_ONLY)

/*********************************************************/
/* WhileParse: purpose is to parse the while statement.  */
/*   The parse of the statement is the return value.     */
/*   Syntax: (while <expression> do <action>+)           */
/*********************************************************/
static struct expr *WhileParse(
  void *theEnv,
  struct expr *parse,
  char *infile)
  {
   struct token theToken;
   int read_first_paren;

   /*===============================*/
   /* Process the while expression. */
   /*===============================*/

   SavePPBuffer(theEnv," ");

   parse->argList = ParseAtomOrExpression(theEnv,infile,NULL);
   if (parse->argList == NULL)
     {
      ReturnExpression(theEnv,parse);
      return(NULL);
     }

   /*====================================*/
   /* Process the do keyword if present. */
   /*====================================*/

   GetToken(theEnv,infile,&theToken);
   if ((theToken.type == SYMBOL) && (strcmp(ValueToString(theToken.value),"do") == 0))
     {
      read_first_paren = TRUE;
      PPBackup(theEnv);
      SavePPBuffer(theEnv," ");
      SavePPBuffer(theEnv,theToken.printForm);
      IncrementIndentDepth(theEnv,3);
      PPCRAndIndent(theEnv);
     }
   else if (theToken.type == LPAREN)
     {
      read_first_paren = FALSE;
      PPBackup(theEnv);
      IncrementIndentDepth(theEnv,3);
      PPCRAndIndent(theEnv);
      SavePPBuffer(theEnv,theToken.printForm);
     }
   else
     {
      SyntaxErrorMessage(theEnv,"while function");
      ReturnExpression(theEnv,parse);
      return(NULL);
     }

   /*============================*/
   /* Process the while actions. */
   /*============================*/
   if (ExpressionData(theEnv)->svContexts->rtn == TRUE)
     ExpressionData(theEnv)->ReturnContext = TRUE;
   ExpressionData(theEnv)->BreakContext = TRUE;
   parse->argList->nextArg = GroupActions(theEnv,infile,&theToken,read_first_paren,NULL,FALSE);

   if (parse->argList->nextArg == NULL)
     {
      ReturnExpression(theEnv,parse);
      return(NULL);
     }
   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,theToken.printForm);

   /*=======================================================*/
   /* Check for the closing right parenthesis of the while. */
   /*=======================================================*/

   if (theToken.type != RPAREN)
     {
      SyntaxErrorMessage(theEnv,"while function");
      ReturnExpression(theEnv,parse);
      return(NULL);
     }

   DecrementIndentDepth(theEnv,3);

   return(parse);
  }

/******************************************************************************************/
/* LoopForCountParse: purpose is to parse the loop-for-count statement.                   */
/*   The parse of the statement is the return value.                                      */
/*   Syntax: (loop-for-count <range> [do] <action>+)                                      */
/*           <range> ::= (<sf-var> [<start-integer-expression>] <end-integer-expression>) */
/******************************************************************************************/
static struct expr *LoopForCountParse(
  void *theEnv,
  struct expr *parse,
  char *infile)
  {
   struct token theToken;
   SYMBOL_HN *loopVar = NULL;
   EXPRESSION *tmpexp;
   int read_first_paren;
   struct BindInfo *oldBindList,*newBindList,*prev;

   /*======================================*/
   /* Process the loop counter expression. */
   /*======================================*/

   SavePPBuffer(theEnv," ");
   GetToken(theEnv,infile,&theToken);

   /* ==========================================
      Simple form: loop-for-count <end> [do] ...
      ========================================== */
   if (theToken.type != LPAREN)
     {
      parse->argList = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,1LL));
      parse->argList->nextArg = ParseAtomOrExpression(theEnv,infile,&theToken);
      if (parse->argList->nextArg == NULL)
        {
         ReturnExpression(theEnv,parse);
         return(NULL);
        }
     }
   else
     {
      GetToken(theEnv,infile,&theToken);
      if (theToken.type != SF_VARIABLE)
        {
         if (theToken.type != SYMBOL)
           goto LoopForCountParseError;
         parse->argList = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,1LL));
         parse->argList->nextArg = Function2Parse(theEnv,infile,ValueToString(theToken.value));
         if (parse->argList->nextArg == NULL)
           {
            ReturnExpression(theEnv,parse);
            return(NULL);
           }
        }

      /* =============================================================
         Complex form: loop-for-count (<var> [<start>] <end>) [do] ...
         ============================================================= */
      else
        {
         loopVar = (SYMBOL_HN *) theToken.value;
         SavePPBuffer(theEnv," ");
         parse->argList = ParseAtomOrExpression(theEnv,infile,NULL);
         if (parse->argList == NULL)
           {
            ReturnExpression(theEnv,parse);
            return(NULL);
           }
         if (CheckArgumentAgainstRestriction(theEnv,parse->argList,(int) 'i'))
           goto LoopForCountParseError;
         SavePPBuffer(theEnv," ");
         GetToken(theEnv,infile,&theToken);
         if (theToken.type == RPAREN)
           {
            PPBackup(theEnv);
            PPBackup(theEnv);
            SavePPBuffer(theEnv,theToken.printForm);
            tmpexp = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,1LL));
            tmpexp->nextArg = parse->argList;
            parse->argList = tmpexp;
           }
         else
          {
            parse->argList->nextArg = ParseAtomOrExpression(theEnv,infile,&theToken);
            if (parse->argList->nextArg == NULL)
              {
               ReturnExpression(theEnv,parse);
               return(NULL);
              }
            GetToken(theEnv,infile,&theToken);
            if (theToken.type != RPAREN)
              goto LoopForCountParseError;
           }
         SavePPBuffer(theEnv," ");
        }
     }

   if (CheckArgumentAgainstRestriction(theEnv,parse->argList->nextArg,(int) 'i'))
     goto LoopForCountParseError;

   /*====================================*/
   /* Process the do keyword if present. */
   /*====================================*/

   GetToken(theEnv,infile,&theToken);
   if ((theToken.type == SYMBOL) && (strcmp(ValueToString(theToken.value),"do") == 0))
     {
      read_first_paren = TRUE;
      PPBackup(theEnv);
      SavePPBuffer(theEnv," ");
      SavePPBuffer(theEnv,theToken.printForm);
      IncrementIndentDepth(theEnv,3);
      PPCRAndIndent(theEnv);
     }
   else if (theToken.type == LPAREN)
     {
      read_first_paren = FALSE;
      PPBackup(theEnv);
      IncrementIndentDepth(theEnv,3);
      PPCRAndIndent(theEnv);
      SavePPBuffer(theEnv,theToken.printForm);
     }
   else
     goto LoopForCountParseError;

   /*=====================================*/
   /* Process the loop-for-count actions. */
   /*=====================================*/
   if (ExpressionData(theEnv)->svContexts->rtn == TRUE)
     ExpressionData(theEnv)->ReturnContext = TRUE;
   ExpressionData(theEnv)->BreakContext = TRUE;
   oldBindList = GetParsedBindNames(theEnv);
   SetParsedBindNames(theEnv,NULL);
   parse->argList->nextArg->nextArg =
      GroupActions(theEnv,infile,&theToken,read_first_paren,NULL,FALSE);

   if (parse->argList->nextArg->nextArg == NULL)
     {
      SetParsedBindNames(theEnv,oldBindList);
      ReturnExpression(theEnv,parse);
      return(NULL);
     }
   newBindList = GetParsedBindNames(theEnv);
   prev = NULL;
   while (newBindList != NULL)
     {
      if ((loopVar == NULL) ? FALSE :
          (strcmp(ValueToString(newBindList->name),ValueToString(loopVar)) == 0))
        {
         ClearParsedBindNames(theEnv);
         SetParsedBindNames(theEnv,oldBindList);
         PrintErrorID(theEnv,"PRCDRPSR",1,TRUE);
         EnvPrintRouter(theEnv,WERROR,"Cannot rebind loop variable in function loop-for-count.\n");
         ReturnExpression(theEnv,parse);
         return(NULL);
        }
      prev = newBindList;
      newBindList = newBindList->next;
     }
   if (prev == NULL)
     SetParsedBindNames(theEnv,oldBindList);
   else
     prev->next = oldBindList;
   if (loopVar != NULL)
     ReplaceLoopCountVars(theEnv,loopVar,parse->argList->nextArg->nextArg,0);
   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,theToken.printForm);

   /*================================================================*/
   /* Check for the closing right parenthesis of the loop-for-count. */
   /*================================================================*/

   if (theToken.type != RPAREN)
     {
      SyntaxErrorMessage(theEnv,"loop-for-count function");
      ReturnExpression(theEnv,parse);
      return(NULL);
     }

   DecrementIndentDepth(theEnv,3);

   return(parse);

LoopForCountParseError:
   SyntaxErrorMessage(theEnv,"loop-for-count function");
   ReturnExpression(theEnv,parse);
   return(NULL);
  }

/***************************************************/
/* ReplaceLoopCountVars                            */
/***************************************************/
static void ReplaceLoopCountVars(
  void *theEnv,
  SYMBOL_HN *loopVar,
  EXPRESSION *theExp,
  int depth)
  {
   while (theExp != NULL)
     {
      if ((theExp->type != SF_VARIABLE) ? FALSE :
          (strcmp(ValueToString(theExp->value),ValueToString(loopVar)) == 0))
        {
         theExp->type = FCALL;
         theExp->value = (void *) FindFunction(theEnv,"(get-loop-count)");
         theExp->argList = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,(long long) depth));
        }
      else if (theExp->argList != NULL)
        {
         if ((theExp->type != FCALL) ? FALSE :
             (theExp->value == (void *) FindFunction(theEnv,"loop-for-count")))
           ReplaceLoopCountVars(theEnv,loopVar,theExp->argList,depth+1);
         else
           ReplaceLoopCountVars(theEnv,loopVar,theExp->argList,depth);
        }
      theExp = theExp->nextArg;
     }
  }

/*********************************************************/
/* IfParse: purpose is to parse the if statement.  The  */
/*   parse of the statement is the return value.         */
/*   Syntax: (if <expression> then <action>+             */
/*               [ else <action>+ ] )                    */
/*********************************************************/
static struct expr *IfParse(
  void *theEnv,
  struct expr *top,
  char *infile)
  {
   struct token theToken;

   /*============================*/
   /* Process the if expression. */
   /*============================*/

   SavePPBuffer(theEnv," ");

   top->argList = ParseAtomOrExpression(theEnv,infile,NULL);

   if (top->argList == NULL)
     {
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   /*========================================*/
   /* Keyword 'then' must follow expression. */
   /*========================================*/

   IncrementIndentDepth(theEnv,3);
   PPCRAndIndent(theEnv);

   GetToken(theEnv,infile,&theToken);
   if ((theToken.type != SYMBOL) || (strcmp(ValueToString(theToken.value),"then") != 0))
     {
      SyntaxErrorMessage(theEnv,"if function");
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   /*==============================*/
   /* Process the if then actions. */
   /*==============================*/

   PPCRAndIndent(theEnv);
   if (ExpressionData(theEnv)->svContexts->rtn == TRUE)
     ExpressionData(theEnv)->ReturnContext = TRUE;
   if (ExpressionData(theEnv)->svContexts->brk == TRUE)
     ExpressionData(theEnv)->BreakContext = TRUE;
   top->argList->nextArg = GroupActions(theEnv,infile,&theToken,TRUE,"else",FALSE);

   if (top->argList->nextArg == NULL)
     {
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   top->argList->nextArg = RemoveUnneededProgn(theEnv,top->argList->nextArg);

   /*===========================================*/
   /* A ')' signals an if then without an else. */
   /*===========================================*/

   if (theToken.type == RPAREN)
     {
      DecrementIndentDepth(theEnv,3);
      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv,theToken.printForm);
      return(top);
     }

   /*=============================================*/
   /* Keyword 'else' must follow if then actions. */
   /*=============================================*/

   if ((theToken.type != SYMBOL) || (strcmp(ValueToString(theToken.value),"else") != 0))
     {
      SyntaxErrorMessage(theEnv,"if function");
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   /*==============================*/
   /* Process the if else actions. */
   /*==============================*/

   PPCRAndIndent(theEnv);
   top->argList->nextArg->nextArg = GroupActions(theEnv,infile,&theToken,TRUE,NULL,FALSE);

   if (top->argList->nextArg->nextArg == NULL)
     {
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   top->argList->nextArg->nextArg = RemoveUnneededProgn(theEnv,top->argList->nextArg->nextArg);

   /*======================================================*/
   /* Check for the closing right parenthesis of the if. */
   /*======================================================*/

   if (theToken.type != RPAREN)
     {
      SyntaxErrorMessage(theEnv,"if function");
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   /*===========================================*/
   /* A ')' signals an if then without an else. */
   /*===========================================*/

   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,")");
   DecrementIndentDepth(theEnv,3);
   return(top);
  }

/********************************************************/
/* PrognParse: purpose is to parse the progn statement. */
/*   The parse of the statement is the return value.    */
/*   Syntax:  (progn <expression>*)                     */
/********************************************************/
static struct expr *PrognParse(
  void *theEnv,
  struct expr *top,
  char *infile)
  {
   struct token tkn;
   struct expr *tmp;

   ReturnExpression(theEnv,top);
   ExpressionData(theEnv)->BreakContext = ExpressionData(theEnv)->svContexts->brk;
   ExpressionData(theEnv)->ReturnContext = ExpressionData(theEnv)->svContexts->rtn;
   IncrementIndentDepth(theEnv,3);
   PPCRAndIndent(theEnv);
   tmp = GroupActions(theEnv,infile,&tkn,TRUE,NULL,FALSE);
   DecrementIndentDepth(theEnv,3);
   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,tkn.printForm);
   return(tmp);
  }

/***********************************************************/
/* BindParse: purpose is to parse the bind statement. The */
/*   parse of the statement is the return value.           */
/*   Syntax:  (bind ?var <expression>)                     */
/***********************************************************/
static struct expr *BindParse(
  void *theEnv,
  struct expr *top,
  char *infile)
  {
   struct token theToken;
   SYMBOL_HN *variableName;
   struct expr *texp;
   CONSTRAINT_RECORD *theConstraint = NULL;
#if DEFGLOBAL_CONSTRUCT
   struct defglobal *theGlobal;
   int count;
#endif

   SavePPBuffer(theEnv," ");

   /*=============================================*/
   /* Next token must be the name of the variable */
   /* to be bound.                                */
   /*=============================================*/

   GetToken(theEnv,infile,&theToken);
   if ((theToken.type != SF_VARIABLE) && (theToken.type != GBL_VARIABLE))
     {
      if ((theToken.type != MF_VARIABLE) || ExpressionData(theEnv)->SequenceOpMode)
        {
         SyntaxErrorMessage(theEnv,"bind function");
         ReturnExpression(theEnv,top);
         return(NULL);
        }
     }

   /*==============================*/
   /* Process the bind expression. */
   /*==============================*/

   top->argList = GenConstant(theEnv,SYMBOL,theToken.value);
   variableName = (SYMBOL_HN *) theToken.value;

#if DEFGLOBAL_CONSTRUCT
   if ((theToken.type == GBL_VARIABLE) ?
       ((theGlobal = (struct defglobal *)
                     FindImportedConstruct(theEnv,"defglobal",NULL,ValueToString(variableName),
                                           &count,TRUE,FALSE)) != NULL) :
       FALSE)
     {
      top->argList->type = DEFGLOBAL_PTR;
      top->argList->value = (void *) theGlobal;
     }
   else if (theToken.type == GBL_VARIABLE)
     {
      GlobalReferenceErrorMessage(theEnv,ValueToString(variableName));
      ReturnExpression(theEnv,top);
      return(NULL);
     }
#endif

   texp = get_struct(theEnv,expr);
   texp->argList = texp->nextArg = NULL;
   if (CollectArguments(theEnv,texp,infile) == NULL)
     {
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   top->argList->nextArg = texp->argList;
   rtn_struct(theEnv,expr,texp);

#if DEFGLOBAL_CONSTRUCT
   if (top->argList->type == DEFGLOBAL_PTR) return(top);
#endif

   if (top->argList->nextArg != NULL)
     { theConstraint = ExpressionToConstraintRecord(theEnv,top->argList->nextArg); }

   AddBindName(theEnv,variableName,theConstraint);

   return(top);
  }

/********************************************/
/* ReturnParse: Parses the return function. */
/********************************************/
static struct expr *ReturnParse(
  void *theEnv,
  struct expr *top,
  char *infile)
  {
   int error_flag = FALSE;
   struct token theToken;

   if (ExpressionData(theEnv)->svContexts->rtn == TRUE)
     ExpressionData(theEnv)->ReturnContext = TRUE;
   if (ExpressionData(theEnv)->ReturnContext == FALSE)
     {
      PrintErrorID(theEnv,"PRCDRPSR",2,TRUE);
      EnvPrintRouter(theEnv,WERROR,"The return function is not valid in this context.\n");
      ReturnExpression(theEnv,top);
      return(NULL);
     }
   ExpressionData(theEnv)->ReturnContext = FALSE;

   SavePPBuffer(theEnv," ");

   top->argList = ArgumentParse(theEnv,infile,&error_flag);
   if (error_flag)
     {
      ReturnExpression(theEnv,top);
      return(NULL);
     }
   else if (top->argList == NULL)
     {
      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv,")");
     }
   else
     {
      SavePPBuffer(theEnv," ");
      GetToken(theEnv,infile,&theToken);
      if (theToken.type != RPAREN)
        {
         SyntaxErrorMessage(theEnv,"return function");
         ReturnExpression(theEnv,top);
         return(NULL);
        }
      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv,")");
     }
   return(top);
  }

/**********************************************/
/* BreakParse:                                */
/**********************************************/
static struct expr *BreakParse(
  void *theEnv,
  struct expr *top,
  char *infile)
  {
   struct token theToken;

   if (ExpressionData(theEnv)->svContexts->brk == FALSE)
     {
      PrintErrorID(theEnv,"PRCDRPSR",2,TRUE);
      EnvPrintRouter(theEnv,WERROR,"The break function not valid in this context.\n");
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   SavePPBuffer(theEnv," ");
   GetToken(theEnv,infile,&theToken);
   if (theToken.type != RPAREN)
     {
      SyntaxErrorMessage(theEnv,"break function");
      ReturnExpression(theEnv,top);
      return(NULL);
     }
   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,")");
   return(top);
  }

/**********************************************/
/* SwitchParse:                               */
/**********************************************/
static struct expr *SwitchParse(
  void *theEnv,
  struct expr *top,
  char *infile)
  {
   struct token theToken;
   EXPRESSION *theExp,*chk;
   int default_count = 0;

   /*============================*/
   /* Process the switch value   */
   /*============================*/
   IncrementIndentDepth(theEnv,3);
   SavePPBuffer(theEnv," ");
   top->argList = theExp = ParseAtomOrExpression(theEnv,infile,NULL);
   if (theExp == NULL)
     goto SwitchParseError;

   /*========================*/
   /* Parse case statements. */
   /*========================*/
   GetToken(theEnv,infile,&theToken);
   while (theToken.type != RPAREN)
     {
      PPBackup(theEnv);
      PPCRAndIndent(theEnv);
      SavePPBuffer(theEnv,theToken.printForm);
      if (theToken.type != LPAREN)
        goto SwitchParseErrorAndMessage;
      GetToken(theEnv,infile,&theToken);
      SavePPBuffer(theEnv," ");
      if ((theToken.type == SYMBOL) &&
          (strcmp(ValueToString(theToken.value),"case") == 0))
        {
         if (default_count != 0)
           goto SwitchParseErrorAndMessage;
         theExp->nextArg = ParseAtomOrExpression(theEnv,infile,NULL);
         SavePPBuffer(theEnv," ");
         if (theExp->nextArg == NULL)
           goto SwitchParseError;
         for (chk = top->argList->nextArg ; chk != theExp->nextArg ; chk = chk->nextArg)
           {
            if ((chk->type == theExp->nextArg->type) &&
                (chk->value == theExp->nextArg->value) &&
                IdenticalExpression(chk->argList,theExp->nextArg->argList))
              {
               PrintErrorID(theEnv,"PRCDRPSR",3,TRUE);
               EnvPrintRouter(theEnv,WERROR,"Duplicate case found in switch function.\n");
               goto SwitchParseError;
              }
           }
         GetToken(theEnv,infile,&theToken);
         if ((theToken.type != SYMBOL) ? TRUE :
             (strcmp(ValueToString(theToken.value),"then") != 0))
           goto SwitchParseErrorAndMessage;
        }
      else if ((theToken.type == SYMBOL) &&
               (strcmp(ValueToString(theToken.value),"default") == 0))
        {
         if (default_count)
           goto SwitchParseErrorAndMessage;
         theExp->nextArg = GenConstant(theEnv,RVOID,NULL);
         default_count = 1;
        }
      else
        goto SwitchParseErrorAndMessage;
      theExp = theExp->nextArg;
      if (ExpressionData(theEnv)->svContexts->rtn == TRUE)
        ExpressionData(theEnv)->ReturnContext = TRUE;
      if (ExpressionData(theEnv)->svContexts->brk == TRUE)
        ExpressionData(theEnv)->BreakContext = TRUE;
      IncrementIndentDepth(theEnv,3);
      PPCRAndIndent(theEnv);
      theExp->nextArg = GroupActions(theEnv,infile,&theToken,TRUE,NULL,FALSE);
      DecrementIndentDepth(theEnv,3);
      ExpressionData(theEnv)->ReturnContext = FALSE;
      ExpressionData(theEnv)->BreakContext = FALSE;
      if (theExp->nextArg == NULL)
        goto SwitchParseError;
      theExp = theExp->nextArg;
      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv,theToken.printForm);
      GetToken(theEnv,infile,&theToken);
     }
   DecrementIndentDepth(theEnv,3);
   return(top);

SwitchParseErrorAndMessage:
   SyntaxErrorMessage(theEnv,"switch function");
SwitchParseError:
   ReturnExpression(theEnv,top);
   DecrementIndentDepth(theEnv,3);
   return(NULL);
  }

/********************************************************/
/* SearchParsedBindNames:                               */
/********************************************************/
globle int SearchParsedBindNames(
  void *theEnv,
  SYMBOL_HN *name_sought)
  {
   struct BindInfo *var_ptr;
   int theIndex = 1;

   var_ptr = ProcedureParserData(theEnv)->ListOfParsedBindNames;
   while (var_ptr != NULL)
     {
      if (var_ptr->name == name_sought)
        { return(theIndex); }
      var_ptr = var_ptr->next;
      theIndex++;
     }

   return(0);
  }

/********************************************************/
/* FindBindConstraints:                               */
/********************************************************/
globle struct constraintRecord *FindBindConstraints(
  void *theEnv,
  SYMBOL_HN *nameSought)
  {
   struct BindInfo *theVariable;

   theVariable = ProcedureParserData(theEnv)->ListOfParsedBindNames;
   while (theVariable != NULL)
     {
      if (theVariable->name == nameSought)
        { return(theVariable->constraints); }
      theVariable = theVariable->next;
     }

   return(NULL);
  }

/********************************************************/
/* CountParsedBindNames: Counts the number of variables */
/*   names that have been bound using the bind function */
/*   in the current context (e.g. the RHS of a rule).   */
/********************************************************/
globle int CountParsedBindNames(
  void *theEnv)
  {
   struct BindInfo *theVariable;
   int theIndex = 0;

   theVariable = ProcedureParserData(theEnv)->ListOfParsedBindNames;
   while (theVariable != NULL)
     {
      theVariable = theVariable->next;
      theIndex++;
     }

   return(theIndex);
  }

/****************************************************************/
/* AddBindName: Adds a variable name used as the first argument */
/*   of the bind function to the list of variable names parsed  */
/*   within the current semantic context (e.g. RHS of a rule).  */
/****************************************************************/
static int AddBindName(
  void *theEnv,
  SYMBOL_HN *variableName,
  CONSTRAINT_RECORD *theConstraint)
  {
   CONSTRAINT_RECORD *tmpConstraint;
   struct BindInfo *currentBind, *lastBind;
   int theIndex = 1;

   /*=========================================================*/
   /* Look for the variable name in the list of bind variable */
   /* names already parsed. If it is found, then return the   */
   /* index to the variable and union the new constraint      */
   /* information with the old constraint information.        */
   /*=========================================================*/

   lastBind = NULL;
   currentBind = ProcedureParserData(theEnv)->ListOfParsedBindNames;
   while (currentBind != NULL)
     {
      if (currentBind->name == variableName)
        {
         if (theConstraint != NULL)
           {
            tmpConstraint = currentBind->constraints;
            currentBind->constraints = UnionConstraints(theEnv,theConstraint,currentBind->constraints);
            RemoveConstraint(theEnv,tmpConstraint);
            RemoveConstraint(theEnv,theConstraint);
           }

         return(theIndex);
        }
      lastBind = currentBind;
      currentBind = currentBind->next;
      theIndex++;
     }

   /*===============================================================*/
   /* If the variable name wasn't found, then add it to the list of */
   /* variable names and store the constraint information with it.  */
   /*===============================================================*/

   currentBind = get_struct(theEnv,BindInfo);
   currentBind->name = variableName;
   currentBind->constraints = theConstraint;
   currentBind->next = NULL;

   if (lastBind == NULL) ProcedureParserData(theEnv)->ListOfParsedBindNames = currentBind;
   else lastBind->next = currentBind;

   return(theIndex);
  }

/********************************************************/
/* RemoveParsedBindName:                                     */
/********************************************************/
globle void RemoveParsedBindName(
  void *theEnv,
  struct symbolHashNode *bname)
  {
   struct BindInfo *prv,*tmp;

   prv = NULL;
   tmp = ProcedureParserData(theEnv)->ListOfParsedBindNames;
   while ((tmp != NULL) ? (tmp->name != bname) : FALSE)
     {
      prv = tmp;
      tmp = tmp->next;
     }
   if (tmp != NULL)
     {
      if (prv == NULL)
        ProcedureParserData(theEnv)->ListOfParsedBindNames = tmp->next;
      else
        prv->next = tmp->next;

      RemoveConstraint(theEnv,tmp->constraints);
      rtn_struct(theEnv,BindInfo,tmp);
     }
  }

#endif

#endif

