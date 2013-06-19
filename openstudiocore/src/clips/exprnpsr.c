   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*              EXPRESSION PARSER MODULE               */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for parsing expressions.       */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Changed name of variable exp to theExp         */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Module specifier can be used within an         */
/*            expression to refer to a deffunction or        */
/*            defgeneric exported by the specified module,   */
/*            but not necessarily imported by the current    */
/*            module.                                        */
/*************************************************************/

#define _EXPRNPSR_SOURCE_

#include "setup.h"

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "constant.h"
#include "envrnmnt.h"
#include "router.h"
#include "strngrtr.h"
#include "scanner.h"
#include "memalloc.h"
#include "argacces.h"
#include "prntutil.h"
#include "cstrnchk.h"
#include "extnfunc.h"
#include "exprnpsr.h"
#include "modulutl.h"
#include "prcdrfun.h"

#if DEFRULE_CONSTRUCT
#include "network.h"
#endif

#if DEFGENERIC_CONSTRUCT
#include "genrccom.h"
#endif

#if DEFFUNCTION_CONSTRUCT
#include "dffnxfun.h"
#endif

#if (! RUN_TIME)

/***************************************************/
/* Function0Parse: Parses a function. Assumes that */
/*   none of the function has been parsed yet.     */
/***************************************************/
globle struct expr *Function0Parse(
  void *theEnv,
  char *logicalName)
  {
   struct token theToken;
   struct expr *top;

   /*=================================*/
   /* All functions begin with a '('. */
   /*=================================*/

   GetToken(theEnv,logicalName,&theToken);
   if (theToken.type != LPAREN)
     {
      SyntaxErrorMessage(theEnv,"function calls");
      return(NULL);
     }

   /*=================================*/
   /* Parse the rest of the function. */
   /*=================================*/

   top = Function1Parse(theEnv,logicalName);
   return(top);
  }

/*******************************************************/
/* Function1Parse: Parses a function. Assumes that the */
/*   opening left parenthesis has already been parsed. */
/*******************************************************/
globle struct expr *Function1Parse(
  void *theEnv,
  char *logicalName)
  {
   struct token theToken;
   struct expr *top;

   /*========================*/
   /* Get the function name. */
   /*========================*/

   GetToken(theEnv,logicalName,&theToken);
   if (theToken.type != SYMBOL)
     {
      PrintErrorID(theEnv,"EXPRNPSR",1,TRUE);
      EnvPrintRouter(theEnv,WERROR,"A function name must be a symbol\n");
      return(NULL);
     }

   /*=================================*/
   /* Parse the rest of the function. */
   /*=================================*/

   top = Function2Parse(theEnv,logicalName,ValueToString(theToken.value));
   return(top);
  }

/****************************************************/
/* Function2Parse: Parses a function. Assumes that  */
/*   the opening left parenthesis and function name */
/*   have already been parsed.                      */
/****************************************************/
globle struct expr *Function2Parse(
  void *theEnv,
  char *logicalName,
  char *name)
  {
   struct FunctionDefinition *theFunction;
   struct expr *top;
   int moduleSpecified = FALSE;
   unsigned position;
   struct symbolHashNode *moduleName = NULL, *constructName = NULL;
#if DEFGENERIC_CONSTRUCT
   void *gfunc;
#endif
#if DEFFUNCTION_CONSTRUCT
   void *dptr;
#endif

   /*=========================================================*/
   /* Module specification cannot be used in a function call. */
   /*=========================================================*/

   if ((position = FindModuleSeparator(name)) != FALSE)
     { 
      moduleName = ExtractModuleName(theEnv,position,name);
      constructName = ExtractConstructName(theEnv,position,name);
      moduleSpecified = TRUE; 
     }

   /*================================*/
   /* Has the function been defined? */
   /*================================*/

   theFunction = FindFunction(theEnv,name);

#if DEFGENERIC_CONSTRUCT
   if (moduleSpecified)
     { 
      if (ConstructExported(theEnv,"defgeneric",moduleName,constructName) ||
          EnvGetCurrentModule(theEnv) == EnvFindDefmodule(theEnv,ValueToString(moduleName)))
        { gfunc = (void *) EnvFindDefgeneric(theEnv,name); }
      else
        { gfunc = NULL; }
     }
   else
     { gfunc = (void *) LookupDefgenericInScope(theEnv,name); }
#endif

#if DEFFUNCTION_CONSTRUCT
   if ((theFunction == NULL)
#if DEFGENERIC_CONSTRUCT
        && (gfunc == NULL)
#endif
     )
     if (moduleSpecified)
       { 
        if (ConstructExported(theEnv,"deffunction",moduleName,constructName) ||
            EnvGetCurrentModule(theEnv) == EnvFindDefmodule(theEnv,ValueToString(moduleName)))
          { dptr = (void *) EnvFindDeffunction(theEnv,name); }
        else
          { dptr = NULL; }
       }
     else
       { dptr = (void *) LookupDeffunctionInScope(theEnv,name); }
   else
     dptr = NULL;
#endif

   /*=============================*/
   /* Define top level structure. */
   /*=============================*/

#if DEFFUNCTION_CONSTRUCT
   if (dptr != NULL)
     top = GenConstant(theEnv,PCALL,dptr);
   else
#endif
#if DEFGENERIC_CONSTRUCT
   if (gfunc != NULL)
     top = GenConstant(theEnv,GCALL,gfunc);
   else
#endif
   if (theFunction != NULL)
     top = GenConstant(theEnv,FCALL,theFunction);
   else
     {
      PrintErrorID(theEnv,"EXPRNPSR",3,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Missing function declaration for ");
      EnvPrintRouter(theEnv,WERROR,name);
      EnvPrintRouter(theEnv,WERROR,".\n");
      return(NULL);
     }

   /*=======================================================*/
   /* Check to see if function has its own parsing routine. */
   /*=======================================================*/

   PushRtnBrkContexts(theEnv);
   ExpressionData(theEnv)->ReturnContext = FALSE;
   ExpressionData(theEnv)->BreakContext = FALSE;

#if DEFGENERIC_CONSTRUCT || DEFFUNCTION_CONSTRUCT
   if (top->type == FCALL)
#endif
     {
      if (theFunction->parser != NULL)
        {
         top = (*theFunction->parser)(theEnv,top,logicalName);
         PopRtnBrkContexts(theEnv);
         if (top == NULL) return(NULL);
         if (ReplaceSequenceExpansionOps(theEnv,top->argList,top,FindFunction(theEnv,"(expansion-call)"),
                                         FindFunction(theEnv,"expand$")))
           {
            ReturnExpression(theEnv,top);
            return(NULL);
           }
         return(top);
        }
     }

   /*========================================*/
   /* Default parsing routine for functions. */
   /*========================================*/

   top = CollectArguments(theEnv,top,logicalName);
   PopRtnBrkContexts(theEnv);
   if (top == NULL) return(NULL);

   if (ReplaceSequenceExpansionOps(theEnv,top->argList,top,FindFunction(theEnv,"(expansion-call)"),
                                    FindFunction(theEnv,"expand$")))
     {
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   /*============================================================*/
   /* If the function call uses the sequence expansion operator, */
   /* its arguments cannot be checked until runtime.             */
   /*============================================================*/

   if (top->value == (void *) FindFunction(theEnv,"(expansion-call)"))
     { return(top); }

   /*============================*/
   /* Check for argument errors. */
   /*============================*/

   if ((top->type == FCALL) && EnvGetStaticConstraintChecking(theEnv))
     {
      if (CheckExpressionAgainstRestrictions(theEnv,top,theFunction->restrictions,name))
        {
         ReturnExpression(theEnv,top);
         return(NULL);
        }
     }

#if DEFFUNCTION_CONSTRUCT
   else if (top->type == PCALL)
     {
      if (CheckDeffunctionCall(theEnv,top->value,CountArguments(top->argList)) == FALSE)
        {
         ReturnExpression(theEnv,top);
         return(NULL);
        }
     }
#endif

   /*========================*/
   /* Return the expression. */
   /*========================*/

   return(top);
  }

/***********************************************************************
  NAME         : ReplaceSequenceExpansionOps
  DESCRIPTION  : Replaces function calls which have multifield
                   references as arguments into a call to a
                   special function which expands the multifield
                   into single arguments at run-time.
                 Multifield references which are not function
                   arguments are errors
  INPUTS       : 1) The expression
                 2) The current function call
                 3) The address of the internal H/L function
                    (expansion-call)
                 4) The address of the H/L function expand$
  RETURNS      : FALSE if OK, TRUE on errors
  SIDE EFFECTS : Function call expressions modified, if necessary
  NOTES        : Function calls which truly want a multifield
                   to be passed need use only a single-field
                   refernce (i.e. ? instead of $? - the $ is
                   being treated as a special expansion operator)
 **********************************************************************/
globle intBool ReplaceSequenceExpansionOps(
  void *theEnv,
  EXPRESSION *actions,
  EXPRESSION *fcallexp,
  void *expcall,
  void *expmult)
  {
   EXPRESSION *theExp;

   while (actions != NULL)
     {
      if ((ExpressionData(theEnv)->SequenceOpMode == FALSE) && (actions->type == MF_VARIABLE))
        actions->type = SF_VARIABLE;
      if ((actions->type == MF_VARIABLE) || (actions->type == MF_GBL_VARIABLE) ||
          (actions->value == expmult))
        {
         if ((fcallexp->type != FCALL) ? FALSE :
             (((struct FunctionDefinition *) fcallexp->value)->sequenceuseok == FALSE))
           {
            PrintErrorID(theEnv,"EXPRNPSR",4,FALSE);
            EnvPrintRouter(theEnv,WERROR,"$ Sequence operator not a valid argument for ");
            EnvPrintRouter(theEnv,WERROR,ValueToString(((struct FunctionDefinition *)
                              fcallexp->value)->callFunctionName));
            EnvPrintRouter(theEnv,WERROR,".\n");
            return(TRUE);
           }
         if (fcallexp->value != expcall)
           {
            theExp = GenConstant(theEnv,fcallexp->type,fcallexp->value);
            theExp->argList = fcallexp->argList;
            theExp->nextArg = NULL;
            fcallexp->type = FCALL;
            fcallexp->value = expcall;
            fcallexp->argList = theExp;
           }
         if (actions->value != expmult)
           {
            theExp = GenConstant(theEnv,SF_VARIABLE,actions->value);
            if (actions->type == MF_GBL_VARIABLE)
              theExp->type = GBL_VARIABLE;
            actions->argList = theExp;
            actions->type = FCALL;
            actions->value = expmult;
           }
        }
      if (actions->argList != NULL)
        {
         if ((actions->type == GCALL) ||
             (actions->type == PCALL) ||
             (actions->type == FCALL))
           theExp = actions;
         else
           theExp = fcallexp;
         if (ReplaceSequenceExpansionOps(theEnv,actions->argList,theExp,expcall,expmult))
           return(TRUE);
        }
      actions = actions->nextArg;
     }
   return(FALSE);
  }

/*************************************************/
/* PushRtnBrkContexts: Saves the current context */
/*   for the break/return functions.             */
/*************************************************/
globle void PushRtnBrkContexts(
  void *theEnv)
  {
   SAVED_CONTEXTS *svtmp;

   svtmp = get_struct(theEnv,saved_contexts);
   svtmp->rtn = ExpressionData(theEnv)->ReturnContext;
   svtmp->brk = ExpressionData(theEnv)->BreakContext;
   svtmp->nxt = ExpressionData(theEnv)->svContexts;
   ExpressionData(theEnv)->svContexts = svtmp;
  }

/***************************************************/
/* PopRtnBrkContexts: Restores the current context */
/*   for the break/return functions.               */
/***************************************************/
globle void PopRtnBrkContexts(
  void *theEnv)
  {
   SAVED_CONTEXTS *svtmp;

   ExpressionData(theEnv)->ReturnContext = ExpressionData(theEnv)->svContexts->rtn;
   ExpressionData(theEnv)->BreakContext = ExpressionData(theEnv)->svContexts->brk;
   svtmp = ExpressionData(theEnv)->svContexts;
   ExpressionData(theEnv)->svContexts = ExpressionData(theEnv)->svContexts->nxt;
   rtn_struct(theEnv,saved_contexts,svtmp);
  }

/*****************************************************************/
/* CheckExpressionAgainstRestrictions: Compares the arguments to */
/*   a function to the set of restrictions for that function to  */
/*   determine if any incompatibilities exist. If so, the value  */
/*   TRUE is returned, otherwise FALSE is returned.              */
/*****************************************************************/
globle int CheckExpressionAgainstRestrictions(
  void *theEnv,
  struct expr *theExpression,
  char *restrictions,
  char *functionName)
  {
   char theChar[2];
   int i = 0, j = 1;
   int number1, number2;
   int argCount;
   char defaultRestriction, argRestriction;
   struct expr *argPtr;
   int theRestriction;

   theChar[0] = '0';
   theChar[1] = '\0';

   /*============================================*/
   /* If there are no restrictions, then there's */
   /* no need to check the function.             */
   /*============================================*/

   if (restrictions == NULL) return(FALSE);

   /*=========================================*/
   /* Count the number of function arguments. */
   /*=========================================*/

   argCount = CountArguments(theExpression->argList);

   /*======================================*/
   /* Get the minimum number of arguments. */
   /*======================================*/

   theChar[0] = restrictions[i++];

   if (isdigit(theChar[0]))
     { number1 = atoi(theChar); }
   else if (theChar[0] == '*')
     { number1 = -1; }
   else
     { return(FALSE); }

   /*======================================*/
   /* Get the maximum number of arguments. */
   /*======================================*/

   theChar[0] = restrictions[i++];
   if (isdigit(theChar[0]))
     { number2 = atoi(theChar); }
   else if (theChar[0] == '*')
     { number2 = 10000; }
   else
     { return(FALSE); }

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   if (number1 == number2)
     {
      if (argCount != number1)
        {
         ExpectedCountError(theEnv,functionName,EXACTLY,number1);
         return(TRUE);
        }
     }
   else if (argCount < number1)
     {
      ExpectedCountError(theEnv,functionName,AT_LEAST,number1);
      return(TRUE);
     }
   else if (argCount > number2)
     {
      ExpectedCountError(theEnv,functionName,NO_MORE_THAN,number2);
      return(TRUE);
     }

   /*=======================================*/
   /* Check for the default argument types. */
   /*=======================================*/

   defaultRestriction = restrictions[i];
   if (defaultRestriction == '\0')
     { defaultRestriction = 'u'; }
   else if (defaultRestriction == '*')
     {
      defaultRestriction = 'u';
      i++;
     }
   else
     { i++; }

   /*======================*/
   /* Check each argument. */
   /*======================*/

   for (argPtr = theExpression->argList;
        argPtr != NULL;
        argPtr = argPtr->nextArg)
     {
      argRestriction = restrictions[i];
      if (argRestriction == '\0')
        { argRestriction = defaultRestriction; }
      else
        { i++; }

      if (argRestriction != '*')
        { theRestriction = (int) argRestriction; }
      else
        { theRestriction = (int) defaultRestriction; }

      if (CheckArgumentAgainstRestriction(theEnv,argPtr,theRestriction))
        {
         ExpectedTypeError1(theEnv,functionName,j,GetArgumentTypeName(theRestriction));
         return(TRUE);
        }

      j++;
     }

   return(FALSE);
  }

/*******************************************************/
/* CollectArguments: Parses and groups together all of */
/*   the arguments for a function call expression.     */
/*******************************************************/
globle struct expr *CollectArguments(
  void *theEnv,
  struct expr *top,
  char *logicalName)
  {
   int errorFlag;
   struct expr *lastOne, *nextOne;

   /*========================================*/
   /* Default parsing routine for functions. */
   /*========================================*/

   lastOne = NULL;

   while (TRUE)
     {
      SavePPBuffer(theEnv," ");

      errorFlag = FALSE;
      nextOne = ArgumentParse(theEnv,logicalName,&errorFlag);

      if (errorFlag == TRUE)
        {
         ReturnExpression(theEnv,top);
         return(NULL);
        }

      if (nextOne == NULL)
        {
         PPBackup(theEnv);
         PPBackup(theEnv);
         SavePPBuffer(theEnv,")");
         return(top);
        }

      if (lastOne == NULL)
        { top->argList = nextOne; }
      else
        { lastOne->nextArg = nextOne; }

      lastOne = nextOne;
     }
  }

/********************************************/
/* ArgumentParse: Parses an argument within */
/*   a function call expression.            */
/********************************************/
globle struct expr *ArgumentParse(
  void *theEnv,
  char *logicalName,
  int *errorFlag)
  {
   struct expr *top;
   struct token theToken;

   /*===============*/
   /* Grab a token. */
   /*===============*/

   GetToken(theEnv,logicalName,&theToken);

   /*============================*/
   /* ')' counts as no argument. */
   /*============================*/

   if (theToken.type == RPAREN)
     { return(NULL); }

   /*================================*/
   /* Parse constants and variables. */
   /*================================*/

   if ((theToken.type == SF_VARIABLE) || (theToken.type == MF_VARIABLE) ||
       (theToken.type == SYMBOL) || (theToken.type == STRING) ||
#if DEFGLOBAL_CONSTRUCT
       (theToken.type == GBL_VARIABLE) ||
       (theToken.type == MF_GBL_VARIABLE) ||
#endif
#if OBJECT_SYSTEM
       (theToken.type == INSTANCE_NAME) ||
#endif
       (theToken.type == FLOAT) || (theToken.type == INTEGER))
     { return(GenConstant(theEnv,theToken.type,theToken.value)); }

   /*======================*/
   /* Parse function call. */
   /*======================*/

   if (theToken.type != LPAREN)
     {
      PrintErrorID(theEnv,"EXPRNPSR",2,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Expected a constant, variable, or expression.\n");
      *errorFlag = TRUE;
      return(NULL);
     }

   top = Function1Parse(theEnv,logicalName);
   if (top == NULL) *errorFlag = TRUE;
   return(top);
  }

/************************************************************/
/* ParseAtomOrExpression: Parses an expression which may be */
/*   a function call, atomic value (string, symbol, etc.),  */
/*   or variable (local or global).                         */
/************************************************************/
globle struct expr *ParseAtomOrExpression(
  void *theEnv,
  char *logicalName,
  struct token *useToken)
  {
   struct token theToken, *thisToken;
   struct expr *rv;

   if (useToken == NULL)
     {
      thisToken = &theToken;
      GetToken(theEnv,logicalName,thisToken);
     }
   else thisToken = useToken;

   if ((thisToken->type == SYMBOL) || (thisToken->type == STRING) ||
       (thisToken->type == INTEGER) || (thisToken->type == FLOAT) ||
#if OBJECT_SYSTEM
       (thisToken->type == INSTANCE_NAME) ||
#endif
#if DEFGLOBAL_CONSTRUCT
       (thisToken->type == GBL_VARIABLE) ||
       (thisToken->type == MF_GBL_VARIABLE) ||
#endif
       (thisToken->type == SF_VARIABLE) || (thisToken->type == MF_VARIABLE))
     { rv = GenConstant(theEnv,thisToken->type,thisToken->value); }
   else if (thisToken->type == LPAREN)
     {
      rv = Function1Parse(theEnv,logicalName);
      if (rv == NULL) return(NULL);
     }
   else
     {
      PrintErrorID(theEnv,"EXPRNPSR",2,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Expected a constant, variable, or expression.\n");
      return(NULL);
     }

   return(rv);
  }

/*********************************************/
/* GroupActions: Groups together a series of */
/*   actions within a progn expression. Used */
/*   for example to parse the RHS of a rule. */
/*********************************************/
globle struct expr *GroupActions(
  void *theEnv,
  char *logicalName,
  struct token *theToken,
  int readFirstToken,
  char *endWord,
  int functionNameParsed)
  {
   struct expr *top, *nextOne, *lastOne = NULL;

   /*=============================*/
   /* Create the enclosing progn. */
   /*=============================*/

   top = GenConstant(theEnv,FCALL,FindFunction(theEnv,"progn"));

   /*========================================================*/
   /* Continue until all appropriate commands are processed. */
   /*========================================================*/

   while (TRUE)
     {
      /*================================================*/
      /* Skip reading in the token if this is the first */
      /* pass and the initial token was already read    */
      /* before calling this function.                  */
      /*================================================*/

      if (readFirstToken)
        { GetToken(theEnv,logicalName,theToken); }
      else
        { readFirstToken = TRUE; }

      /*=================================================*/
      /* Look to see if a symbol has terminated the list */
      /* of actions (such as "else" in an if function).  */
      /*=================================================*/

      if ((theToken->type == SYMBOL) &&
          (endWord != NULL) &&
          (! functionNameParsed))
        {
         if (strcmp(ValueToString(theToken->value),endWord) == 0)
           { return(top); }
        }

      /*====================================*/
      /* Process a function if the function */
      /* name has already been read.        */
      /*====================================*/

      if (functionNameParsed)
        {
         nextOne = Function2Parse(theEnv,logicalName,ValueToString(theToken->value));
         functionNameParsed = FALSE;
        }

      /*========================================*/
      /* Process a constant or global variable. */
      /*========================================*/

      else if ((theToken->type == SYMBOL) || (theToken->type == STRING) ||
          (theToken->type == INTEGER) || (theToken->type == FLOAT) ||
#if DEFGLOBAL_CONSTRUCT
          (theToken->type == GBL_VARIABLE) ||
          (theToken->type == MF_GBL_VARIABLE) ||
#endif
#if OBJECT_SYSTEM
          (theToken->type == INSTANCE_NAME) ||
#endif
          (theToken->type == SF_VARIABLE) || (theToken->type == MF_VARIABLE))
        { nextOne = GenConstant(theEnv,theToken->type,theToken->value); }

      /*=============================*/
      /* Otherwise parse a function. */
      /*=============================*/

      else if (theToken->type == LPAREN)
        { nextOne = Function1Parse(theEnv,logicalName); }

      /*======================================*/
      /* Otherwise replace sequence expansion */
      /* variables and return the expression. */
      /*======================================*/

      else
        {
         if (ReplaceSequenceExpansionOps(theEnv,top,NULL,
                                         FindFunction(theEnv,"(expansion-call)"),
                                         FindFunction(theEnv,"expand$")))
           {
            ReturnExpression(theEnv,top);
            return(NULL);
           }

         return(top);
        }

      /*===========================*/
      /* Add the new action to the */
      /* list of progn arguments.  */
      /*===========================*/

      if (nextOne == NULL)
        {
         theToken->type = UNKNOWN_VALUE;
         ReturnExpression(theEnv,top);
         return(NULL);
        }

      if (lastOne == NULL)
        { top->argList = nextOne; }
      else
        { lastOne->nextArg = nextOne; }

      lastOne = nextOne;

      PPCRAndIndent(theEnv);
     }
  }

#endif /* (! RUN_TIME) */

/********************************************************/
/* EnvSetSequenceOperatorRecognition: C access routine  */
/*   for the set-sequence-operator-recognition function */
/********************************************************/
globle intBool EnvSetSequenceOperatorRecognition(
  void *theEnv,
  int value)
  {
   int ov;

   ov = ExpressionData(theEnv)->SequenceOpMode;
   ExpressionData(theEnv)->SequenceOpMode = value;
   return(ov);
  }

/********************************************************/
/* EnvSetSequenceOperatorRecognition: C access routine  */
/*   for the Get-sequence-operator-recognition function */
/********************************************************/
globle intBool EnvGetSequenceOperatorRecognition(
  void *theEnv)
  {
   return(ExpressionData(theEnv)->SequenceOpMode);
  }

/*******************************************/
/* ParseConstantArguments: Parses a string */
/*    into a set of constant expressions.  */
/*******************************************/
globle EXPRESSION *ParseConstantArguments(
  void *theEnv,
  char *argstr,
  int *error)
  {
   EXPRESSION *top = NULL,*bot = NULL,*tmp;
   char *router = "***FNXARGS***";
   struct token tkn;

   *error = FALSE;

   if (argstr == NULL) return(NULL);

   /*=====================================*/
   /* Open the string as an input source. */
   /*=====================================*/

   if (OpenStringSource(theEnv,router,argstr,0) == 0)
     {
      PrintErrorID(theEnv,"EXPRNPSR",6,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Cannot read arguments for external call.\n");
      *error = TRUE;
      return(NULL);
     }

   /*======================*/
   /* Parse the constants. */
   /*======================*/

   GetToken(theEnv,router,&tkn);
   while (tkn.type != STOP)
     {
      if ((tkn.type != SYMBOL) && (tkn.type != STRING) &&
          (tkn.type != FLOAT) && (tkn.type != INTEGER) &&
          (tkn.type != INSTANCE_NAME))
        {
         PrintErrorID(theEnv,"EXPRNPSR",7,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Only constant arguments allowed for external function call.\n");
         ReturnExpression(theEnv,top);
         *error = TRUE;
         CloseStringSource(theEnv,router);
         return(NULL);
        }
      tmp = GenConstant(theEnv,tkn.type,tkn.value);
      if (top == NULL)
        top = tmp;
      else
        bot->nextArg = tmp;
      bot = tmp;
      GetToken(theEnv,router,&tkn);
     }

   /*================================*/
   /* Close the string input source. */
   /*================================*/

   CloseStringSource(theEnv,router);

   /*=======================*/
   /* Return the arguments. */
   /*=======================*/

   return(top);
  }

/*********************************************/
/* RemoveUnneededProgn:  */
/*********************************************/
globle struct expr *RemoveUnneededProgn(
  void *theEnv,
  struct expr *theExpression)
  {
   struct FunctionDefinition *fptr;
   struct expr *temp;

   if (theExpression == NULL) return(theExpression);

   if (theExpression->type != FCALL) return(theExpression);

   fptr = (struct FunctionDefinition *) theExpression->value;

   if (fptr->functionPointer != PTIF PrognFunction)
     { return(theExpression); }

   if ((theExpression->argList != NULL) &&
       (theExpression->argList->nextArg == NULL))
     {
      temp = theExpression;
      theExpression = theExpression->argList;
      temp->argList = NULL;
      temp->nextArg = NULL;
      ReturnExpression(theEnv,temp);
     }

   return(theExpression);
  }
