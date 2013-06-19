   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  07/01/05          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Query Functions for Objects                      */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Gary D. Riley                                        */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Added fact-set queries.                        */
/*                                                           */
/*      6.24: Corrected errors when compiling as a C++ file. */
/*            DR0868                                         */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if FACT_SET_QUERIES

#include "argacces.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "exprnpsr.h"
#include "modulutl.h"
#include "tmpltutl.h"
#include "insfun.h"
#include "factqpsr.h"
#include "prcdrfun.h"
#include "router.h"
#include "utility.h"

#define _FACTQURY_SOURCE_
#include "factqury.h"

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static void PushQueryCore(void *);
static void PopQueryCore(void *);
static QUERY_CORE *FindQueryCore(void *,int);
static QUERY_TEMPLATE *DetermineQueryTemplates(void *,EXPRESSION *,char *,unsigned *);
static QUERY_TEMPLATE *FormChain(void *,char *,DATA_OBJECT *);
static void DeleteQueryTemplates(void *,QUERY_TEMPLATE *);
static int TestForFirstInChain(void *,QUERY_TEMPLATE *,int);
static int TestForFirstFactInTemplate(void *,struct deftemplate *,QUERY_TEMPLATE *,int);
static void TestEntireChain(void *,QUERY_TEMPLATE *,int);
static void TestEntireTemplate(void *,struct deftemplate *,QUERY_TEMPLATE *,int);
static void AddSolution(void *);
static void PopQuerySoln(void *);

/****************************************************
  NAME         : SetupFactQuery
  DESCRIPTION  : Initializes fact query H/L
                   functions and parsers
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Sets up kernel functions and parsers
  NOTES        : None
 ****************************************************/
globle void SetupFactQuery(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,FACT_QUERY_DATA,sizeof(struct factQueryData),NULL);

#if RUN_TIME                                                 
   FactQueryData(theEnv)->QUERY_DELIMETER_SYMBOL = FindSymbolHN(theEnv,QUERY_DELIMETER_STRING);
#endif  

#if ! RUN_TIME
   FactQueryData(theEnv)->QUERY_DELIMETER_SYMBOL = (SYMBOL_HN *) EnvAddSymbol(theEnv,QUERY_DELIMETER_STRING);
   IncrementSymbolCount(FactQueryData(theEnv)->QUERY_DELIMETER_SYMBOL);

   EnvDefineFunction2(theEnv,"(query-fact)",'u',
                  PTIEF GetQueryFact,"GetQueryFact",NULL);

   EnvDefineFunction2(theEnv,"(query-fact-slot)",'u',
                  PTIEF GetQueryFactSlot,"GetQueryFactSlot",NULL);

   EnvDefineFunction2(theEnv,"any-factp",'b',PTIEF AnyFacts,"AnyFacts",NULL);
   AddFunctionParser(theEnv,"any-factp",FactParseQueryNoAction);

   EnvDefineFunction2(theEnv,"find-fact",'m',
                  PTIEF QueryFindFact,"QueryFindFact",NULL);
   AddFunctionParser(theEnv,"find-fact",FactParseQueryNoAction);

   EnvDefineFunction2(theEnv,"find-all-facts",'m',
                  PTIEF QueryFindAllFacts,"QueryFindAllFacts",NULL);
   AddFunctionParser(theEnv,"find-all-facts",FactParseQueryNoAction);

   EnvDefineFunction2(theEnv,"do-for-fact",'u',
                  PTIEF QueryDoForFact,"QueryDoForFact",NULL);
   AddFunctionParser(theEnv,"do-for-fact",FactParseQueryAction);

   EnvDefineFunction2(theEnv,"do-for-all-facts",'u',
                  PTIEF QueryDoForAllFacts,"QueryDoForAllFacts",NULL);
   AddFunctionParser(theEnv,"do-for-all-facts",FactParseQueryAction);

   EnvDefineFunction2(theEnv,"delayed-do-for-all-facts",'u',
                  PTIEF DelayedQueryDoForAllFacts,
                  "DelayedQueryDoForAllFacts",NULL);
   AddFunctionParser(theEnv,"delayed-do-for-all-facts",FactParseQueryAction);
#endif
  }

/*************************************************************
  NAME         : GetQueryFact
  DESCRIPTION  : Internal function for referring to fact
                    array on fact-queries
  INPUTS       : None
  RETURNS      : The name of the specified fact-set member
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : ((query-fact) <index>)
 *************************************************************/
globle void GetQueryFact(
  void *theEnv,
  DATA_OBJECT *result)
  {
   register QUERY_CORE *core;

   core = FindQueryCore(theEnv,ValueToInteger(GetpValue(GetFirstArgument())));
   
   result->type = FACT_ADDRESS;
   result->value = core->solns[ValueToInteger(GetpValue(GetFirstArgument()->nextArg))];
  }

/***************************************************************************
  NAME         : GetQueryFactSlot
  DESCRIPTION  : Internal function for referring to slots of fact in
                    fact array on fact-queries
  INPUTS       : The caller's result buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Caller's result buffer set appropriately
  NOTES        : H/L Syntax : ((query-fact-slot) <index> <slot-name>)
 **************************************************************************/
globle void GetQueryFactSlot(
  void *theEnv,
  DATA_OBJECT *result)
  {
   struct fact *theFact;
   DATA_OBJECT temp;
   QUERY_CORE *core;
   short position;

   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);

   core = FindQueryCore(theEnv,ValueToInteger(GetpValue(GetFirstArgument())));
   theFact = core->solns[ValueToInteger(GetpValue(GetFirstArgument()->nextArg))];
   EvaluateExpression(theEnv,GetFirstArgument()->nextArg->nextArg,&temp);
   if (temp.type != SYMBOL)
     {
      ExpectedTypeError1(theEnv,"get",1,"symbol");
      SetEvaluationError(theEnv,TRUE);
      return;
     }
     
   /*==================================================*/
   /* Make sure the slot exists (the symbol implied is */
   /* used for the implied slot of an ordered fact).   */
   /*==================================================*/

   if (theFact->whichDeftemplate->implied)
     {
      if (strcmp(ValueToString(temp.value),"implied") != 0)
        {
         SlotExistError(theEnv,ValueToString(temp.value),"fact-set query");
         return;
        }
      position = 1;
     }

   else if (FindSlot((struct deftemplate *) theFact->whichDeftemplate,
                     (struct symbolHashNode *) temp.value,&position) == NULL)
     {
      SlotExistError(theEnv,ValueToString(temp.value),"fact-set query");
      return;
     }
     
   result->type = theFact->theProposition.theFields[position-1].type;
   result->value = theFact->theProposition.theFields[position-1].value;
   if (result->type == MULTIFIELD)
     {
      SetpDOBegin(result,1);
      SetpDOEnd(result,((struct multifield *) result->value)->multifieldLength);
     }
  }

/* =============================================================================
   =============================================================================
   Following are the instance query functions :

     any-factp         : Determines if any facts satisfy the query
     find-fact         : Finds first (set of) fact(s) which satisfies
                               the query and stores it in a multi-field
     find-all-facts    : Finds all (sets of) facts which satisfy the
                               the query and stores them in a multi-field
     do-for-fact       : Executes a given action for the first (set of)
                               fact(s) which satisfy the query
     do-for-all-facts  : Executes an action for all facts which satisfy
                               the query as they are found
     delayed-do-for-all-facts : Same as above - except that the list of facts
                               which satisfy the query is formed before any
                               actions are executed

     Fact candidate search algorithm :

     All permutations of first restriction template facts with other
       restriction template facts (Rightmost are varied first)

     For any one template, fact are examined in the order they were defined

     Example :
     (deftemplate a (slot v))
     (deftemplate b (slot v))
     (deftemplate c (slot v))
     (assert (a (v a1)))
     (assert (a (v a2)))
     (assert (b (v b1)))
     (assert (b (v b2)))
     (assert (c (v c1)))
     (assert (c (v c2)))
     (assert (d (v d1)))
     (assert (d (v d2)))

     (any-factp ((?a a b) (?b c)) <query>)

     The permutations (?a ?b) would be examined in the following order :

     (a1 c1),(a1 c2),(a2 c1),(a2 c2),
     (b1 c1),(b1 c2),(b2 c1),(b2 c2)

   =============================================================================
   ============================================================================= */

/******************************************************************************
  NAME         : AnyFacts
  DESCRIPTION  : Determines if there any existing facts which satisfy
                   the query
  INPUTS       : None
  RETURNS      : TRUE if the query is satisfied, FALSE otherwise
  SIDE EFFECTS : The query template-expressions are evaluated once,
                   and the query boolean-expression is evaluated
                   zero or more times (depending on fact restrictions
                   and how early the expression evaluates to TRUE - if at all).
  NOTES        : H/L Syntax : See FactParseQueryNoAction()
 ******************************************************************************/
globle intBool AnyFacts(
  void *theEnv)
  {
   QUERY_TEMPLATE *qtemplates;
   unsigned rcnt;
   int TestResult;

   qtemplates = DetermineQueryTemplates(theEnv,GetFirstArgument()->nextArg,
                                      "any-factp",&rcnt);
   if (qtemplates == NULL)
     return(FALSE);
   PushQueryCore(theEnv);
   FactQueryData(theEnv)->QueryCore = get_struct(theEnv,query_core);
   FactQueryData(theEnv)->QueryCore->solns = (struct fact **) gm2(theEnv,(sizeof(struct fact *) * rcnt));
   FactQueryData(theEnv)->QueryCore->query = GetFirstArgument();
   TestResult = TestForFirstInChain(theEnv,qtemplates,0);
   FactQueryData(theEnv)->AbortQuery = FALSE;
   rm(theEnv,(void *) FactQueryData(theEnv)->QueryCore->solns,(sizeof(struct fact *) * rcnt));
   rtn_struct(theEnv,query_core,FactQueryData(theEnv)->QueryCore);
   PopQueryCore(theEnv);
   DeleteQueryTemplates(theEnv,qtemplates);
   return(TestResult);
  }

/******************************************************************************
  NAME         : QueryFindFact
  DESCRIPTION  : Finds the first set of facts which satisfy the query and
                   stores their addresses in the user's multi-field variable
  INPUTS       : Caller's result buffer
  RETURNS      : TRUE if the query is satisfied, FALSE otherwise
  SIDE EFFECTS : The query template-expressions are evaluated once,
                   and the query boolean-expression is evaluated
                   zero or more times (depending on fact restrictions
                   and how early the expression evaulates to TRUE - if at all).
  NOTES        : H/L Syntax : See ParseQueryNoAction()
 ******************************************************************************/
globle void QueryFindFact(
  void *theEnv,
  DATA_OBJECT *result)
  {
   QUERY_TEMPLATE *qtemplates;
   unsigned rcnt,i;

   result->type = MULTIFIELD;
   result->begin = 0;
   result->end = -1;
   qtemplates = DetermineQueryTemplates(theEnv,GetFirstArgument()->nextArg,
                                      "find-fact",&rcnt);
   if (qtemplates == NULL)
     {
      result->value = (void *) EnvCreateMultifield(theEnv,0L);
      return;
     }
   PushQueryCore(theEnv);
   FactQueryData(theEnv)->QueryCore = get_struct(theEnv,query_core);
   FactQueryData(theEnv)->QueryCore->solns = (struct fact **)
                      gm2(theEnv,(sizeof(struct fact *) * rcnt));
   FactQueryData(theEnv)->QueryCore->query = GetFirstArgument();
   if (TestForFirstInChain(theEnv,qtemplates,0) == TRUE)
     {
      result->value = (void *) EnvCreateMultifield(theEnv,rcnt);
      SetpDOEnd(result,rcnt);
      for (i = 1 ; i <= rcnt ; i++)
        {
         SetMFType(result->value,i,FACT_ADDRESS);
         SetMFValue(result->value,i,FactQueryData(theEnv)->QueryCore->solns[i - 1]);
        }
     }
   else
      result->value = (void *) EnvCreateMultifield(theEnv,0L);
   FactQueryData(theEnv)->AbortQuery = FALSE;
   rm(theEnv,(void *) FactQueryData(theEnv)->QueryCore->solns,(sizeof(struct fact *) * rcnt));
   rtn_struct(theEnv,query_core,FactQueryData(theEnv)->QueryCore);
   PopQueryCore(theEnv);
   DeleteQueryTemplates(theEnv,qtemplates);
  }

/******************************************************************************
  NAME         : QueryFindAllFacts
  DESCRIPTION  : Finds all sets of facts which satisfy the query and
                   stores their names in the user's multi-field variable

                 The sets are stored sequentially :

                   Number of sets = (Multi-field length) / (Set length)

                 The first set is if the first (set length) atoms of the
                   multi-field variable, and so on.
  INPUTS       : Caller's result buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : The query template-expressions are evaluated once,
                   and the query boolean-expression is evaluated
                   once for every fact set.
  NOTES        : H/L Syntax : See ParseQueryNoAction()
 ******************************************************************************/
globle void QueryFindAllFacts(
  void *theEnv,
  DATA_OBJECT *result)
  {
   QUERY_TEMPLATE *qtemplates;
   unsigned rcnt;
   register unsigned i,j;

   result->type = MULTIFIELD;
   result->begin = 0;
   result->end = -1;
   qtemplates = DetermineQueryTemplates(theEnv,GetFirstArgument()->nextArg,
                                      "find-all-facts",&rcnt);
   if (qtemplates == NULL)
     {
      result->value = (void *) EnvCreateMultifield(theEnv,0L);
      return;
     }
   PushQueryCore(theEnv);
   FactQueryData(theEnv)->QueryCore = get_struct(theEnv,query_core);
   FactQueryData(theEnv)->QueryCore->solns = (struct fact **) gm2(theEnv,(sizeof(struct fact *) * rcnt));
   FactQueryData(theEnv)->QueryCore->query = GetFirstArgument();
   FactQueryData(theEnv)->QueryCore->action = NULL;
   FactQueryData(theEnv)->QueryCore->soln_set = NULL;
   FactQueryData(theEnv)->QueryCore->soln_size = rcnt;
   FactQueryData(theEnv)->QueryCore->soln_cnt = 0;
   TestEntireChain(theEnv,qtemplates,0);
   FactQueryData(theEnv)->AbortQuery = FALSE;
   result->value = (void *) EnvCreateMultifield(theEnv,FactQueryData(theEnv)->QueryCore->soln_cnt * rcnt);
   while (FactQueryData(theEnv)->QueryCore->soln_set != NULL)
     {
      for (i = 0 , j = (unsigned) (result->end + 2) ; i < rcnt ; i++ , j++)
        {
         SetMFType(result->value,j,FACT_ADDRESS);
         SetMFValue(result->value,j,FactQueryData(theEnv)->QueryCore->soln_set->soln[i]);
        }
      result->end = (long) j-2;
      PopQuerySoln(theEnv);
     }
   rm(theEnv,(void *) FactQueryData(theEnv)->QueryCore->solns,(sizeof(struct fact *) * rcnt));
   rtn_struct(theEnv,query_core,FactQueryData(theEnv)->QueryCore);
   PopQueryCore(theEnv);
   DeleteQueryTemplates(theEnv,qtemplates);
  }

/******************************************************************************
  NAME         : QueryDoForFact
  DESCRIPTION  : Finds the first set of facts which satisfy the query and
                   executes a user-action with that set
  INPUTS       : None
  RETURNS      : Caller's result buffer
  SIDE EFFECTS : The query template-expressions are evaluated once,
                   and the query boolean-expression is evaluated
                   zero or more times (depending on fact restrictions
                   and how early the expression evaulates to TRUE - if at all).
                   Also the action expression is executed zero or once.
                 Caller's result buffer holds result of user-action
  NOTES        : H/L Syntax : See ParseQueryAction()
 ******************************************************************************/
globle void QueryDoForFact(
  void *theEnv,
  DATA_OBJECT *result)
  {
   QUERY_TEMPLATE *qtemplates;
   unsigned rcnt;

   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   qtemplates = DetermineQueryTemplates(theEnv,GetFirstArgument()->nextArg->nextArg,
                                      "do-for-fact",&rcnt);
   if (qtemplates == NULL)
     return;
   PushQueryCore(theEnv);
   FactQueryData(theEnv)->QueryCore = get_struct(theEnv,query_core);
   FactQueryData(theEnv)->QueryCore->solns = (struct fact **) gm2(theEnv,(sizeof(struct fact *) * rcnt));
   FactQueryData(theEnv)->QueryCore->query = GetFirstArgument();
   FactQueryData(theEnv)->QueryCore->action = GetFirstArgument()->nextArg;
   if (TestForFirstInChain(theEnv,qtemplates,0) == TRUE)
     EvaluateExpression(theEnv,FactQueryData(theEnv)->QueryCore->action,result);
   FactQueryData(theEnv)->AbortQuery = FALSE;
   ProcedureFunctionData(theEnv)->BreakFlag = FALSE;
   rm(theEnv,(void *) FactQueryData(theEnv)->QueryCore->solns,(sizeof(struct fact *) * rcnt));
   rtn_struct(theEnv,query_core,FactQueryData(theEnv)->QueryCore);
   PopQueryCore(theEnv);
   DeleteQueryTemplates(theEnv,qtemplates);
  }

/******************************************************************************
  NAME         : QueryDoForAllFacts
  DESCRIPTION  : Finds all sets of facts which satisfy the query and
                   executes a user-function for each set as it is found
  INPUTS       : Caller's result buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : The query template-expressions are evaluated once,
                   and the query boolean-expression is evaluated
                   once for every fact set.  Also, the action is
                   executed for every fact set.
                 Caller's result buffer holds result of last action executed.
  NOTES        : H/L Syntax : See FactParseQueryAction()
 ******************************************************************************/
globle void QueryDoForAllFacts(
  void *theEnv,
  DATA_OBJECT *result)
  {
   QUERY_TEMPLATE *qtemplates;
   unsigned rcnt;

   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   qtemplates = DetermineQueryTemplates(theEnv,GetFirstArgument()->nextArg->nextArg,
                                      "do-for-all-facts",&rcnt);
   if (qtemplates == NULL)
     return;
   PushQueryCore(theEnv);
   FactQueryData(theEnv)->QueryCore = get_struct(theEnv,query_core);
   FactQueryData(theEnv)->QueryCore->solns = (struct fact **) gm2(theEnv,(sizeof(struct fact *) * rcnt));
   FactQueryData(theEnv)->QueryCore->query = GetFirstArgument();
   FactQueryData(theEnv)->QueryCore->action = GetFirstArgument()->nextArg;
   FactQueryData(theEnv)->QueryCore->result = result;
   ValueInstall(theEnv,FactQueryData(theEnv)->QueryCore->result);
   TestEntireChain(theEnv,qtemplates,0);
   ValueDeinstall(theEnv,FactQueryData(theEnv)->QueryCore->result);
   PropagateReturnValue(theEnv,FactQueryData(theEnv)->QueryCore->result);
   FactQueryData(theEnv)->AbortQuery = FALSE;
   ProcedureFunctionData(theEnv)->BreakFlag = FALSE;
   rm(theEnv,(void *) FactQueryData(theEnv)->QueryCore->solns,(sizeof(struct fact *) * rcnt));
   rtn_struct(theEnv,query_core,FactQueryData(theEnv)->QueryCore);
   PopQueryCore(theEnv);
   DeleteQueryTemplates(theEnv,qtemplates);
  }

/******************************************************************************
  NAME         : DelayedQueryDoForAllFacts
  DESCRIPTION  : Finds all sets of facts which satisfy the query and
                   and exceutes a user-action for each set

                 This function differs from QueryDoForAllFacts() in
                   that it forms the complete list of query satisfactions
                   BEFORE executing any actions.
  INPUTS       : Caller's result buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : The query template-expressions are evaluated once,
                   and the query boolean-expression is evaluated
                   once for every fact set.  The action is executed
                   for evry query satisfaction.
                 Caller's result buffer holds result of last action executed.
  NOTES        : H/L Syntax : See FactParseQueryNoAction()
 ******************************************************************************/
globle void DelayedQueryDoForAllFacts(
  void *theEnv,
  DATA_OBJECT *result)
  {
   QUERY_TEMPLATE *qtemplates;
   unsigned rcnt;
   register unsigned i;

   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   qtemplates = DetermineQueryTemplates(theEnv,GetFirstArgument()->nextArg->nextArg,
                                      "delayed-do-for-all-facts",&rcnt);
   if (qtemplates == NULL)
     return;
   PushQueryCore(theEnv);
   FactQueryData(theEnv)->QueryCore = get_struct(theEnv,query_core);
   FactQueryData(theEnv)->QueryCore->solns = (struct fact **) gm2(theEnv,(sizeof(struct fact *) * rcnt));
   FactQueryData(theEnv)->QueryCore->query = GetFirstArgument();
   FactQueryData(theEnv)->QueryCore->action = NULL;
   FactQueryData(theEnv)->QueryCore->soln_set = NULL;
   FactQueryData(theEnv)->QueryCore->soln_size = rcnt;
   FactQueryData(theEnv)->QueryCore->soln_cnt = 0;
   TestEntireChain(theEnv,qtemplates,0);
   FactQueryData(theEnv)->AbortQuery = FALSE;
   FactQueryData(theEnv)->QueryCore->action = GetFirstArgument()->nextArg;
   while (FactQueryData(theEnv)->QueryCore->soln_set != NULL)
     {
      for (i = 0 ; i < rcnt ; i++)
        FactQueryData(theEnv)->QueryCore->solns[i] = FactQueryData(theEnv)->QueryCore->soln_set->soln[i];
      PopQuerySoln(theEnv);
      EvaluationData(theEnv)->CurrentEvaluationDepth++;
      EvaluateExpression(theEnv,FactQueryData(theEnv)->QueryCore->action,result);
      EvaluationData(theEnv)->CurrentEvaluationDepth--;
      if (ProcedureFunctionData(theEnv)->ReturnFlag == TRUE)
        { PropagateReturnValue(theEnv,result); }
      PeriodicCleanup(theEnv,FALSE,TRUE);
      if (EvaluationData(theEnv)->HaltExecution || ProcedureFunctionData(theEnv)->BreakFlag || ProcedureFunctionData(theEnv)->ReturnFlag)
        {
         while (FactQueryData(theEnv)->QueryCore->soln_set != NULL)
           PopQuerySoln(theEnv);
         break;
        }
     }
   ProcedureFunctionData(theEnv)->BreakFlag = FALSE;
   rm(theEnv,(void *) FactQueryData(theEnv)->QueryCore->solns,(sizeof(struct fact *) * rcnt));
   rtn_struct(theEnv,query_core,FactQueryData(theEnv)->QueryCore);
   PopQueryCore(theEnv);
   DeleteQueryTemplates(theEnv,qtemplates);
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/*******************************************************
  NAME         : PushQueryCore
  DESCRIPTION  : Pushes the current QueryCore onto stack
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Allocates new stack node and changes
                   QueryCoreStack
  NOTES        : None
 *******************************************************/
static void PushQueryCore(
  void *theEnv)
  {
   QUERY_STACK *qptr;

   qptr = get_struct(theEnv,query_stack);
   qptr->core = FactQueryData(theEnv)->QueryCore;
   qptr->nxt = FactQueryData(theEnv)->QueryCoreStack;
   FactQueryData(theEnv)->QueryCoreStack = qptr;
  }

/******************************************************
  NAME         : PopQueryCore
  DESCRIPTION  : Pops top of QueryCore stack and
                   restores QueryCore to this core
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Stack node deallocated, QueryCoreStack
                   changed and QueryCore reset
  NOTES        : Assumes stack is not empty
 ******************************************************/
static void PopQueryCore(
  void *theEnv)
  {
   QUERY_STACK *qptr;

   FactQueryData(theEnv)->QueryCore = FactQueryData(theEnv)->QueryCoreStack->core;
   qptr = FactQueryData(theEnv)->QueryCoreStack;
   FactQueryData(theEnv)->QueryCoreStack = FactQueryData(theEnv)->QueryCoreStack->nxt;
   rtn_struct(theEnv,query_stack,qptr);
  }

/***************************************************
  NAME         : FindQueryCore
  DESCRIPTION  : Looks up a QueryCore Stack Frame
                    Depth 0 is current frame
                    1 is next deepest, etc.
  INPUTS       : Depth
  RETURNS      : Address of query core stack frame
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
static QUERY_CORE *FindQueryCore(
  void *theEnv,
  int depth)
  {
   QUERY_STACK *qptr;

   if (depth == 0)
     return(FactQueryData(theEnv)->QueryCore);
   qptr = FactQueryData(theEnv)->QueryCoreStack;
   while (depth > 1)
     {
      qptr = qptr->nxt;
      depth--;
     }
   return(qptr->core);
  }

/**********************************************************
  NAME         : DetermineQueryTemplates
  DESCRIPTION  : Builds a list of templates to be used in
                   fact queries - uses parse form.
  INPUTS       : 1) The parse template expression chain
                 2) The name of the function being executed
                 3) Caller's buffer for restriction count
                    (# of separate lists)
  RETURNS      : The query list, or NULL on errors
  SIDE EFFECTS : Memory allocated for list
                 Busy count incremented for all templates
  NOTES        : Each restriction is linked by nxt pointer,
                   multiple templates in a restriction are
                   linked by the chain pointer.
                 Rcnt caller's buffer is set to reflect the
                   total number of chains
                 Assumes classExp is not NULL and that each
                   restriction chain is terminated with
                   the QUERY_DELIMITER_SYMBOL "(QDS)"
 **********************************************************/
static QUERY_TEMPLATE *DetermineQueryTemplates(
  void *theEnv,
  EXPRESSION *templateExp,
  char *func,
  unsigned *rcnt)
  {
   QUERY_TEMPLATE *clist = NULL,*cnxt = NULL,*cchain = NULL,*tmp;
   int new_list = FALSE;
   DATA_OBJECT temp;

   *rcnt = 0;
   while (templateExp != NULL)
     {
      if (EvaluateExpression(theEnv,templateExp,&temp))
        {
         DeleteQueryTemplates(theEnv,clist);
         return(NULL);
        }
      if ((temp.type == SYMBOL) && (temp.value == (void *) FactQueryData(theEnv)->QUERY_DELIMETER_SYMBOL))
        {
         new_list = TRUE;
         (*rcnt)++;
        }
      else if ((tmp = FormChain(theEnv,func,&temp)) != NULL)
        {
         if (clist == NULL)
           clist = cnxt = cchain = tmp;
         else if (new_list == TRUE)
           {
            new_list = FALSE;
            cnxt->nxt = tmp;
            cnxt = cchain = tmp;
           }
         else
           cchain->chain = tmp;
         while (cchain->chain != NULL)
           cchain = cchain->chain;
        }
      else
        {
         SyntaxErrorMessage(theEnv,"fact-set query class restrictions");
         DeleteQueryTemplates(theEnv,clist);
         SetEvaluationError(theEnv,TRUE);
         return(NULL);
        }
      templateExp = templateExp->nextArg;
     }
   return(clist);
  }

/*************************************************************
  NAME         : FormChain
  DESCRIPTION  : Builds a list of classes to be used in
                   fact queries - uses parse form.
  INPUTS       : 1) Name of calling function for error msgs
                 2) Data object - must be a symbol or a
                      multifield value containing all symbols
                 The symbols must be names of existing templates
  RETURNS      : The query chain, or NULL on errors
  SIDE EFFECTS : Memory allocated for chain
                 Busy count incremented for all templates
  NOTES        : None
 *************************************************************/
static QUERY_TEMPLATE *FormChain(
  void *theEnv,
  char *func,
  DATA_OBJECT *val)
  {
   struct deftemplate *templatePtr;
   QUERY_TEMPLATE *head,*bot,*tmp;
   register long i,end; /* 6.04 Bug Fix */
   char *templateName;
   int count;

   if (val->type == DEFTEMPLATE_PTR)
     {
      IncrementDeftemplateBusyCount(theEnv,(void *) val->value);
      head = get_struct(theEnv,query_template);
      head->templatePtr = (struct deftemplate *) val->value;

      head->chain = NULL;
      head->nxt = NULL;
      return(head);
     }
   if (val->type == SYMBOL)
     {
      /* ===============================================
         Allow instance-set query restrictions to have a
         module specifier as part of the class name,
         but search imported defclasses too if a
         module specifier is not given
         =============================================== */
         
      templatePtr = (struct deftemplate *)
                       FindImportedConstruct(theEnv,"deftemplate",NULL,DOPToString(val),
                                             &count,TRUE,NULL);
      if (templatePtr == NULL)
        {
         CantFindItemInFunctionErrorMessage(theEnv,"deftemplate",DOPToString(val),func);
         return(NULL);
        }
      IncrementDeftemplateBusyCount(theEnv,(void *) templatePtr);
      head = get_struct(theEnv,query_template);
      head->templatePtr = templatePtr;

      head->chain = NULL;
      head->nxt = NULL;
      return(head);
     }
   if (val->type == MULTIFIELD)
     {
      head = bot = NULL;
      end = GetpDOEnd(val);
      for (i = GetpDOBegin(val) ; i <= end ; i++)
        {
         if (GetMFType(val->value,i) == SYMBOL)
           {
            templateName = ValueToString(GetMFValue(val->value,i));
            
            templatePtr = (struct deftemplate *)
                       FindImportedConstruct(theEnv,"deftemplate",NULL,templateName,
                                             &count,TRUE,NULL);

            if (templatePtr == NULL)
              {
               CantFindItemInFunctionErrorMessage(theEnv,"deftemplate",templateName,func);
               DeleteQueryTemplates(theEnv,head);
               return(NULL);
              }
           }
         else
           {
            DeleteQueryTemplates(theEnv,head);
            return(NULL);
           }
         IncrementDeftemplateBusyCount(theEnv,(void *) templatePtr);
         tmp = get_struct(theEnv,query_template);
         tmp->templatePtr = templatePtr;

         tmp->chain = NULL;
         tmp->nxt = NULL;
         if (head == NULL)
           head = tmp;
         else
           bot->chain = tmp;
         bot = tmp;
        }
      return(head);
     }
   return(NULL);
  }

/******************************************************
  NAME         : DeleteQueryTemplates
  DESCRIPTION  : Deletes a query class-list
  INPUTS       : The query list address
  RETURNS      : Nothing useful
  SIDE EFFECTS : Nodes deallocated
                 Busy count decremented for all templates
  NOTES        : None
 ******************************************************/
static void DeleteQueryTemplates(
  void *theEnv,
  QUERY_TEMPLATE *qlist)
  {
   QUERY_TEMPLATE *tmp;

   while (qlist != NULL)
     {
      while (qlist->chain != NULL)
        {
         tmp = qlist->chain;
         qlist->chain = qlist->chain->chain;
         DecrementDeftemplateBusyCount(theEnv,(void *) tmp->templatePtr);
         rtn_struct(theEnv,query_template,tmp);
        }
      tmp = qlist;
      qlist = qlist->nxt;
      DecrementDeftemplateBusyCount(theEnv,(void *) tmp->templatePtr);
      rtn_struct(theEnv,query_template,tmp);
     }
  }

/************************************************************
  NAME         : TestForFirstInChain
  DESCRIPTION  : Processes all templates in a restriction chain
                   until success or done
  INPUTS       : 1) The current chain
                 2) The index of the chain restriction
                     (e.g. the 4th query-variable)
  RETURNS      : TRUE if query succeeds, FALSE otherwise
  SIDE EFFECTS : Sets current restriction class
                 Fact variable values set
  NOTES        : None
 ************************************************************/
static int TestForFirstInChain(
  void *theEnv,
  QUERY_TEMPLATE *qchain,
  int indx)
  {
   QUERY_TEMPLATE *qptr;

   FactQueryData(theEnv)->AbortQuery = TRUE;
   for (qptr = qchain ; qptr != NULL ; qptr = qptr->chain)
     {
      FactQueryData(theEnv)->AbortQuery = FALSE;

      if (TestForFirstFactInTemplate(theEnv,qptr->templatePtr,qchain,indx))
        { return(TRUE); }
        
      if ((EvaluationData(theEnv)->HaltExecution == TRUE) || (FactQueryData(theEnv)->AbortQuery == TRUE))
        return(FALSE);
     }
   return(FALSE);
  }

/*****************************************************************
  NAME         : TestForFirstFactInTemplate
  DESCRIPTION  : Processes all facts in a template
  INPUTS       : 1) Visitation traversal id
                 2) The template
                 3) The current template restriction chain
                 4) The index of the current restriction
  RETURNS      : TRUE if query succeeds, FALSE otherwise
  SIDE EFFECTS : Fact variable values set
  NOTES        : None
 *****************************************************************/
static int TestForFirstFactInTemplate(
  void *theEnv,
  struct deftemplate *templatePtr,
  QUERY_TEMPLATE *qchain,
  int indx)
  {
   struct fact *theFact;
   DATA_OBJECT temp;

   theFact = templatePtr->factList;
   while (theFact != NULL)
     {
      FactQueryData(theEnv)->QueryCore->solns[indx] = theFact;
      if (qchain->nxt != NULL)
        {
         theFact->factHeader.busyCount++;
         if (TestForFirstInChain(theEnv,qchain->nxt,indx+1) == TRUE)
           {
            theFact->factHeader.busyCount--;
            break;
           }
         theFact->factHeader.busyCount--;
         if ((EvaluationData(theEnv)->HaltExecution == TRUE) || (FactQueryData(theEnv)->AbortQuery == TRUE))
           break;
        }
      else
        {
         theFact->factHeader.busyCount++;
         EvaluationData(theEnv)->CurrentEvaluationDepth++;
         EvaluateExpression(theEnv,FactQueryData(theEnv)->QueryCore->query,&temp);
         EvaluationData(theEnv)->CurrentEvaluationDepth--;
         PeriodicCleanup(theEnv,FALSE,TRUE);
         theFact->factHeader.busyCount--;
         if (EvaluationData(theEnv)->HaltExecution == TRUE)
           break;
         if ((temp.type != SYMBOL) ? TRUE :
             (temp.value != EnvFalseSymbol(theEnv)))
           break;
        }
      theFact = theFact->nextTemplateFact;
      while ((theFact != NULL) ? (theFact->garbage == 1) : FALSE)
        theFact = theFact->nextTemplateFact;
     }

   if (theFact != NULL)
     return(((EvaluationData(theEnv)->HaltExecution == TRUE) || (FactQueryData(theEnv)->AbortQuery == TRUE))
             ? FALSE : TRUE);

   return(FALSE);
  }

/************************************************************
  NAME         : TestEntireChain
  DESCRIPTION  : Processes all templates in a restriction chain
                   until done
  INPUTS       : 1) The current chain
                 2) The index of the chain restriction
                     (i.e. the 4th query-variable)
  RETURNS      : Nothing useful
  SIDE EFFECTS : Sets current restriction template
                 Query fact variables set
                 Solution sets stored in global list
  NOTES        : None
 ************************************************************/
static void TestEntireChain(
  void *theEnv,
  QUERY_TEMPLATE *qchain,
  int indx)
  {
   QUERY_TEMPLATE *qptr;

   FactQueryData(theEnv)->AbortQuery = TRUE;
   for (qptr = qchain ; qptr != NULL ; qptr = qptr->chain)
     {
      FactQueryData(theEnv)->AbortQuery = FALSE;

      TestEntireTemplate(theEnv,qptr->templatePtr,qchain,indx);

      if ((EvaluationData(theEnv)->HaltExecution == TRUE) || (FactQueryData(theEnv)->AbortQuery == TRUE))
        return;
     }
  }

/*****************************************************************
  NAME         : TestEntireTemplate
  DESCRIPTION  : Processes all facts in a template
  INPUTS       : 1) The module for which templates tested must be
                    in scope
                 3) The template
                 4) The current template restriction chain
                 5) The index of the current restriction
  RETURNS      : Nothing useful
  SIDE EFFECTS : Instance variable values set
                 Solution sets stored in global list
  NOTES        : None
 *****************************************************************/
static void TestEntireTemplate(
  void *theEnv,
  struct deftemplate *templatePtr,
  QUERY_TEMPLATE *qchain,
  int indx)
  {
   struct fact *theFact;
   DATA_OBJECT temp;

   theFact = templatePtr->factList;
   while (theFact != NULL)
     {
      FactQueryData(theEnv)->QueryCore->solns[indx] = theFact;
      if (qchain->nxt != NULL)
        {
         theFact->factHeader.busyCount++;
         TestEntireChain(theEnv,qchain->nxt,indx+1);
         theFact->factHeader.busyCount--;
         if ((EvaluationData(theEnv)->HaltExecution == TRUE) || (FactQueryData(theEnv)->AbortQuery == TRUE))
           break;
        }
      else
        { 
         theFact->factHeader.busyCount++;
         EvaluationData(theEnv)->CurrentEvaluationDepth++;
         EvaluateExpression(theEnv,FactQueryData(theEnv)->QueryCore->query,&temp);
         EvaluationData(theEnv)->CurrentEvaluationDepth--;
         PeriodicCleanup(theEnv,FALSE,TRUE);
         theFact->factHeader.busyCount--;
         if (EvaluationData(theEnv)->HaltExecution == TRUE)
           break;
         if ((temp.type != SYMBOL) ? TRUE :
             (temp.value != EnvFalseSymbol(theEnv)))
           {
            if (FactQueryData(theEnv)->QueryCore->action != NULL)
              {
               theFact->factHeader.busyCount++;
               EvaluationData(theEnv)->CurrentEvaluationDepth++;
               ValueDeinstall(theEnv,FactQueryData(theEnv)->QueryCore->result);
               EvaluateExpression(theEnv,FactQueryData(theEnv)->QueryCore->action,FactQueryData(theEnv)->QueryCore->result);
               ValueInstall(theEnv,FactQueryData(theEnv)->QueryCore->result);
               EvaluationData(theEnv)->CurrentEvaluationDepth--;
               PeriodicCleanup(theEnv,FALSE,TRUE);
               theFact->factHeader.busyCount--;
               if (ProcedureFunctionData(theEnv)->BreakFlag || ProcedureFunctionData(theEnv)->ReturnFlag)
                 {
                  FactQueryData(theEnv)->AbortQuery = TRUE;
                  break;
                 }
               if (EvaluationData(theEnv)->HaltExecution == TRUE)
                 break;
              }
            else
              AddSolution(theEnv);
           }
        }
        
      theFact = theFact->nextTemplateFact;
      while ((theFact != NULL) ? (theFact->garbage == 1) : FALSE)
        theFact = theFact->nextTemplateFact;
     }
  }

/***************************************************************************
  NAME         : AddSolution
  DESCRIPTION  : Adds the current fact set to a global list of
                   solutions
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Global list and count updated
  NOTES        : Solutions are stored as sequential arrays of struct fact *
 ***************************************************************************/
static void AddSolution(
  void *theEnv)
  {
   QUERY_SOLN *new_soln;
   register unsigned i;

   new_soln = (QUERY_SOLN *) gm2(theEnv,(int) sizeof(QUERY_SOLN));
   new_soln->soln = (struct fact **)
                    gm2(theEnv,(sizeof(struct fact *) * (FactQueryData(theEnv)->QueryCore->soln_size)));
   for (i = 0 ; i < FactQueryData(theEnv)->QueryCore->soln_size ; i++)
     new_soln->soln[i] = FactQueryData(theEnv)->QueryCore->solns[i];
   new_soln->nxt = NULL;
   if (FactQueryData(theEnv)->QueryCore->soln_set == NULL)
     FactQueryData(theEnv)->QueryCore->soln_set = new_soln;
   else
     FactQueryData(theEnv)->QueryCore->soln_bottom->nxt = new_soln;
   FactQueryData(theEnv)->QueryCore->soln_bottom = new_soln;
   FactQueryData(theEnv)->QueryCore->soln_cnt++;
  }

/***************************************************
  NAME         : PopQuerySoln
  DESCRIPTION  : Deallocates the topmost solution
                   set for an fact-set query
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Solution set deallocated
  NOTES        : Assumes QueryCore->soln_set != 0
 ***************************************************/
static void PopQuerySoln(
  void *theEnv)
  {
   FactQueryData(theEnv)->QueryCore->soln_bottom = FactQueryData(theEnv)->QueryCore->soln_set;
   FactQueryData(theEnv)->QueryCore->soln_set = FactQueryData(theEnv)->QueryCore->soln_set->nxt;
   rm(theEnv,(void *) FactQueryData(theEnv)->QueryCore->soln_bottom->soln,
      (sizeof(struct fact *) * FactQueryData(theEnv)->QueryCore->soln_size));
   rm(theEnv,(void *) FactQueryData(theEnv)->QueryCore->soln_bottom,sizeof(QUERY_SOLN));
  }
  
#endif


