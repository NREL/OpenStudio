   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/***************************************************************/
/* Purpose: Procedural Code Support Routines for Deffunctions, */
/*          Generic Function Methods,Message-Handlers          */
/*          and Rules                                          */
/*                                                             */
/* Principal Programmer(s):                                    */
/*      Brian L. Dantes                                        */
/*                                                             */
/* Contributing Programmer(s):                                 */
/*                                                             */
/* Revision History:                                           */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859    */
/*                                                             */
/*            Changed name of variable log to logName          */
/*            because of Unix compiler warnings of shadowed    */
/*            definitions.                                     */
/*                                                             */
/*      6.24: Renamed BOOLEAN macro type to intBool.           */
/*                                                             */
/*            Added pragmas to remove compilation warnings.    */
/*                                                             */
/***************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#ifndef _STDIO_INCLUDED_
#include <stdio.h>
#define _STDIO_INCLUDED_
#endif

#include <stdlib.h>
#include <ctype.h>

#include "memalloc.h"
#include "constant.h"
#include "envrnmnt.h"
#if DEFGLOBAL_CONSTRUCT
#include "globlpsr.h"
#endif
#include "exprnpsr.h"
#include "multifld.h"
#if OBJECT_SYSTEM
#include "object.h"
#endif
#include "prcdrpsr.h"
#include "router.h"
#include "utility.h"

#define _PRCCODE_SOURCE_
#include "prccode.h"

/* =========================================
   *****************************************
               MACROS AND TYPES
   =========================================
   ***************************************** */
typedef struct
  {
   unsigned firstFlag  : 1;
   unsigned first      : 15;
   unsigned secondFlag : 1;
   unsigned second     : 15;
  } PACKED_PROC_VAR;

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static void EvaluateProcParameters(void *,EXPRESSION *,int,char *,char *);
static intBool RtnProcParam(void *,void *,DATA_OBJECT *);
static intBool GetProcBind(void *,void *,DATA_OBJECT *);
static intBool PutProcBind(void *,void *,DATA_OBJECT *);
static intBool RtnProcWild(void *,void *,DATA_OBJECT *);
static void DeallocateProceduralPrimitiveData(void *);
static void ReleaseProcParameters(void *);

#if (! BLOAD_ONLY) && (! RUN_TIME)
static int FindProcParameter(SYMBOL_HN *,EXPRESSION *,SYMBOL_HN *);
static int ReplaceProcBinds(void *,EXPRESSION *,
                             int (*)(void *,EXPRESSION *,void *),void *);
static EXPRESSION *CompactActions(void *,EXPRESSION *);
#endif

#if (! DEFFUNCTION_CONSTRUCT) || (! DEFGENERIC_CONSTRUCT)
static intBool EvaluateBadCall(void *,void *,DATA_OBJECT *);
#endif

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/****************************************************
  NAME         : InstallProcedurePrimitives
  DESCRIPTION  : Installs primitive function handlers
                 for accessing parameters and local
                 variables within the bodies of
                 message-handlers, methods, rules and
                 deffunctions.
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Primitive entities installed
  NOTES        : None
 ****************************************************/
globle void InstallProcedurePrimitives(
  void *theEnv)
  {
   ENTITY_RECORD procParameterInfo = { "PROC_PARAM", PROC_PARAM,0,1,0,NULL,NULL,NULL,
                                           RtnProcParam,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL },
                     procWildInfo =      { "PROC_WILD_PARAM", PROC_WILD_PARAM,0,1,0,NULL,NULL,NULL,
                                           RtnProcWild,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL },
                     procGetInfo =       { "PROC_GET_BIND", PROC_GET_BIND,0,1,0,NULL,NULL,NULL,
                                           GetProcBind,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL },
                     procBindInfo =      { "PROC_BIND", PROC_BIND,0,1,0,NULL,NULL,NULL,
                                           PutProcBind,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
#if ! DEFFUNCTION_CONSTRUCT
   ENTITY_RECORD deffunctionEntityRecord =
                     { "PCALL", PCALL,0,0,1,
                       NULL,NULL,NULL,
                       EvaluateBadCall,
                       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
#endif
#if ! DEFGENERIC_CONSTRUCT
   ENTITY_RECORD genericEntityRecord =
                     { "GCALL", GCALL,0,0,1,
                       NULL,NULL,NULL,
                       EvaluateBadCall,
                       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
#endif

   AllocateEnvironmentData(theEnv,PROCEDURAL_PRIMITIVE_DATA,sizeof(struct proceduralPrimitiveData),DeallocateProceduralPrimitiveData);

   memcpy(&ProceduralPrimitiveData(theEnv)->ProcParameterInfo,&procParameterInfo,sizeof(struct entityRecord));  
   memcpy(&ProceduralPrimitiveData(theEnv)->ProcWildInfo,&procWildInfo,sizeof(struct entityRecord));  
   memcpy(&ProceduralPrimitiveData(theEnv)->ProcGetInfo,&procGetInfo,sizeof(struct entityRecord));  
   memcpy(&ProceduralPrimitiveData(theEnv)->ProcBindInfo,&procBindInfo,sizeof(struct entityRecord));  

   InstallPrimitive(theEnv,&ProceduralPrimitiveData(theEnv)->ProcParameterInfo,PROC_PARAM);
   InstallPrimitive(theEnv,&ProceduralPrimitiveData(theEnv)->ProcWildInfo,PROC_WILD_PARAM);
   InstallPrimitive(theEnv,&ProceduralPrimitiveData(theEnv)->ProcGetInfo,PROC_GET_BIND);
   InstallPrimitive(theEnv,&ProceduralPrimitiveData(theEnv)->ProcBindInfo,PROC_BIND);

   ProceduralPrimitiveData(theEnv)->Oldindex = -1;
   
   /* ===============================================
      Make sure a default evaluation function is
      in place for deffunctions and generic functions
      in the event that a binary image containing
      these items is loaded into a configuration
      that does not support them.
      =============================================== */

#if ! DEFFUNCTION_CONSTRUCT
   memcpy(&ProceduralPrimitiveData(theEnv)->DeffunctionEntityRecord,&deffunctionEntityRecord,sizeof(struct entityRecord));  
   InstallPrimitive(theEnv,&ProceduralPrimitiveData(theEnv)->DeffunctionEntityRecord,PCALL);
#endif

#if ! DEFGENERIC_CONSTRUCT
   memcpy(&ProceduralPrimitiveData(theEnv)->GenericEntityRecord,&genericEntityRecord,sizeof(struct entityRecord));  
   InstallPrimitive(theEnv,&ProceduralPrimitiveData(theEnv)->GenericEntityRecord,GCALL);
#endif

   /* =============================================
      Install the special empty multifield to
      let callers distinguish between no parameters
      and zero-length multifield parameters
      ============================================= */
   ProceduralPrimitiveData(theEnv)->NoParamValue = CreateMultifield2(theEnv,0L);
   MultifieldInstall(theEnv,(MULTIFIELD_PTR) ProceduralPrimitiveData(theEnv)->NoParamValue);
  }

/**************************************************************/
/* DeallocateProceduralPrimitiveData: Deallocates environment */
/*    data for the procedural primitives functionality.       */
/**************************************************************/
static void DeallocateProceduralPrimitiveData(
  void *theEnv)
  {
   ReturnMultifield(theEnv,(struct multifield *) ProceduralPrimitiveData(theEnv)->NoParamValue);
   ReleaseProcParameters(theEnv);
  }

#if (! BLOAD_ONLY) && (! RUN_TIME)

#if DEFFUNCTION_CONSTRUCT || OBJECT_SYSTEM

/************************************************************
  NAME         : ParseProcParameters
  DESCRIPTION  : Parses a parameter list for a
                  procedural routine, such as a
                  deffunction or message-handler
  INPUTS       : 1) The logical name of the input
                 2) A buffer for scanned tokens
                 3) The partial list of parameters so far
                    (can be NULL)
                 3) A buffer for a wildcard symbol (if any)
                 4) A buffer for a minimum of parameters
                 5) A buffer for a maximum of parameters
                    (will be set to -1 if there is a wilcard)
                 6) A buffer for an error flag
                 7) The address of a function to do specialized
                    checking on a parameter (can be NULL)
                    The function should accept a string and
                    return FALSE if the parameter is OK, TRUE
                    otherwise.
  RETURNS      : A list of expressions containing the
                   parameter names
  SIDE EFFECTS : Parameters parsed and expressions formed
  NOTES        : None
 ************************************************************/
globle EXPRESSION *ParseProcParameters(
  void *theEnv,
  char *readSource,
  struct token *tkn,
  EXPRESSION *parameterList,
  SYMBOL_HN **wildcard,
  int *min,
  int *max,
  int *error,
  int (*checkfunc)(void *,char *))
  {
   EXPRESSION *nextOne,*lastOne,*check;
   int paramprintp = 0;

   *wildcard = NULL;
   *min = 0;
   *error = TRUE;
   lastOne = nextOne = parameterList;
   while (nextOne != NULL)
     {
      (*min)++;
      lastOne = nextOne;
      nextOne = nextOne->nextArg;
     }
   if (tkn->type != LPAREN)
     {
      SyntaxErrorMessage(theEnv,"parameter list");
      ReturnExpression(theEnv,parameterList);
      return(NULL);
     }
   GetToken(theEnv,readSource,tkn);
   while ((tkn->type == SF_VARIABLE) || (tkn->type == MF_VARIABLE))
     {
      for (check = parameterList ; check != NULL ; check = check->nextArg)
        if (check->value == tkn->value)
         {
          PrintErrorID(theEnv,"PRCCODE",7,FALSE);
          EnvPrintRouter(theEnv,WERROR,"Duplicate parameter names not allowed.\n");
          ReturnExpression(theEnv,parameterList);
          return(NULL);
         }
      if (*wildcard != NULL)
        {
         PrintErrorID(theEnv,"PRCCODE",8,FALSE);
         EnvPrintRouter(theEnv,WERROR,"No parameters allowed after wildcard parameter.\n");
         ReturnExpression(theEnv,parameterList);
         return(NULL);
        }
      if ((checkfunc != NULL) ? (*checkfunc)(theEnv,ValueToString(tkn->value)) : FALSE)
        {
         ReturnExpression(theEnv,parameterList);
         return(NULL);
        }
      nextOne = GenConstant(theEnv,tkn->type,tkn->value);
      if (tkn->type == MF_VARIABLE)
        *wildcard = (SYMBOL_HN *) tkn->value;
      else
        (*min)++;
      if (lastOne == NULL)
        { parameterList = nextOne; }
      else
        { lastOne->nextArg = nextOne; }
      lastOne = nextOne;
      SavePPBuffer(theEnv," ");
      paramprintp = 1;
      GetToken(theEnv,readSource,tkn);
     }
   if (tkn->type != RPAREN)
     {
      SyntaxErrorMessage(theEnv,"parameter list");
      ReturnExpression(theEnv,parameterList);
      return(NULL);
     }
   if (paramprintp)
     {
      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv,")");
     }
   *error = FALSE;
   *max = (*wildcard != NULL) ? -1 : *min;
   return(parameterList);
  }

#endif

/*************************************************************************
  NAME         : ParseProcActions
  DESCRIPTION  : Parses the bodies of deffunctions, generic function
                 methods and message-handlers.  Replaces parameter
                 and local variable references with appropriate
                 runtime access functions
  INPUTS       : 1) The type of procedure body being parsed
                 2) The logical name of the input
                 3) A buffer for scanned tokens
                 4) A list of expressions containing the names
                    of the parameters
                 5) The wilcard parameter symbol (NULL if none)
                 6) A pointer to a function to parse variables not
                    recognized by the standard parser
                    The function should accept the variable
                    expression and a generic pointer for special
                    data (can be NULL) as arguments.  If the variable
                    is recognized, the function should modify the
                    expression to access this variable.  Return 1
                    if recognized, 0 if not, -1 on errors
                    This argument can be NULL.
                 7) A pointer to a function to handle binds in a
                    special way. The function should accept the
                    bind function call expression as an argument.
                    If the variable is recognized and treated specially,
                    the function should modify the expression
                    appropriately (including attaching/removing
                    any necessary argument expressions).  Return 1
                    if recognized, 0 if not, -1 on errors.
                    This argument can be NULL.
                 8) A buffer for holding the number of local vars
                    used by this procedure body.
                 9) Special user data buffer to pass to variable
                    reference and bind replacement functions
RETURNS      : A packed expression containing the body, NULL on
                   errors.
SIDE EFFECTS : Variable references replaced with runtime calls
                  to access the paramter and local variable array
NOTES        : None
*************************************************************************/
globle EXPRESSION *ParseProcActions(
  void *theEnv,
  char *bodytype,
  char *readSource,
  struct token *tkn,
  EXPRESSION *params,
  SYMBOL_HN *wildcard,
  int (*altvarfunc)(void *,EXPRESSION *,void *),
  int (*altbindfunc)(void *,EXPRESSION *,void *),
  int *lvarcnt,
  void *userBuffer)
  {
   EXPRESSION *actions,*pactions;

   /* ====================================================================
      Clear parsed bind list - so that only local vars from this body will
      be on it.  The position of vars on thsi list are used to generate
      indices into the LocalVarArray at runtime.  The parsing of the
      "bind" function adds vars to this list.
      ==================================================================== */
   ClearParsedBindNames(theEnv);
   actions = GroupActions(theEnv,readSource,tkn,TRUE,NULL,FALSE);
   if (actions == NULL)
     return(NULL);

   /* ====================================================================
      Replace any bind functions with special functions before replacing
      any variable references.  This allows those bind names to be removed
      before they can be seen by variable replacement and thus generate
      incorrect indices.
      ==================================================================== */
   if (altbindfunc != NULL)
     {
      if (ReplaceProcBinds(theEnv,actions,altbindfunc,userBuffer))
        {
         ClearParsedBindNames(theEnv);
         ReturnExpression(theEnv,actions);
         return(NULL);
        }
     }

   /* ======================================================================
      The number of names left on the bind list is the number of local
      vars for this procedure body.  Replace all variable reference with
      runtime access functions for ProcParamArray, LocalVarArray or
      other special items, such as direct slot references, global variables,
      or fact field references.
      ====================================================================== */
   *lvarcnt = CountParsedBindNames(theEnv);
   if (ReplaceProcVars(theEnv,bodytype,actions,params,wildcard,altvarfunc,userBuffer))
     {
      ClearParsedBindNames(theEnv);
      ReturnExpression(theEnv,actions);
      return(NULL);
     }

   /* =======================================================================
      Normally, actions are grouped in a progn.  If there is only one action,
      the progn is unnecessary and can be removed.  Also, the actions are
      packed into a contiguous array to save on memory overhead.  The
      intermediate parsed bind names are freed to avoid tying up memory.
      ======================================================================= */
   actions = CompactActions(theEnv,actions);
   pactions = PackExpression(theEnv,actions);
   ReturnExpression(theEnv,actions);
   ClearParsedBindNames(theEnv);
   return(pactions);
  }

/*************************************************************************
  NAME         : ReplaceProcVars
  DESCRIPTION  : Examines an expression for variables
                   and replaces any that correspond to
                   procedure parameters or globals
                   with function calls that get these
                   variables' values at run-time.
                   For example, procedure arguments
                   are stored an array at run-time, so at
                   parse-time, parameter-references are replaced
                   with function calls referencing this array at
                   the appropriate position.
  INPUTS       : 1) The type of procedure being parsed
                 2) The expression-actions to be examined
                 3) The parameter list
                 4) The wildcard parameter symbol (NULL if none)
                 5) A pointer to a function to parse variables not
                    recognized by the standard parser
                    The function should accept the variable
                    expression and a generic pointer for special
                    data (can be NULL) as arguments.  If the variable
                    is recognized, the function should modify the
                    expression to access this variable.  Return 1
                    if recognized, 0 if not, -1 on errors
                    This argument can be NULL.
                 6) Data buffer to be passed to alternate parsing
                    function
  RETURNS      : FALSE if OK, TRUE on errors
  SIDE EFFECTS : Variable references replaced with function calls
  NOTES        : This function works from the ParsedBindNames list in
                    SPCLFORM.C to access local binds.  Make sure that
                    the list accurately reflects the binds by calling
                    ClearParsedBindNames(theEnv) before the parse of the body
                    in which variables are being replaced.
 *************************************************************************/
globle int ReplaceProcVars(
  void *theEnv,
  char *bodytype,
  EXPRESSION *actions,
  EXPRESSION *parameterList,
  SYMBOL_HN *wildcard,
  int (*altvarfunc)(void *,EXPRESSION *,void *),
  void *specdata)
  {
   int position,altcode;
   intBool boundPosn;
   EXPRESSION *arg_lvl,*altvarexp;
   SYMBOL_HN *bindName;
   PACKED_PROC_VAR pvar;

   while (actions != NULL)
     {
      if (actions->type == SF_VARIABLE)
        {
         /*===============================================*/
         /* See if the variable is in the parameter list. */
         /*===============================================*/
         bindName = (SYMBOL_HN *) actions->value;
         position = FindProcParameter(bindName,parameterList,wildcard);

         /*=============================================================*/
         /* Check to see if the variable is bound within the procedure. */
         /*=============================================================*/
         boundPosn = SearchParsedBindNames(theEnv,bindName);

         /*=============================================*/
         /* If variable is not defined in the parameter */
         /* list or as part of a bind action then...    */
         /*=============================================*/

         if ((position == 0) && (boundPosn == 0))
           {
            /*================================================================*/
            /* Check to see if the variable has a special access function,    */
            /* such as direct slot reference or a rule RHS pattern reference. */
            /*================================================================*/
            if ((altvarfunc != NULL) ? ((*altvarfunc)(theEnv,actions,specdata) != 1) : TRUE)
              {
               PrintErrorID(theEnv,"PRCCODE",3,TRUE);
               EnvPrintRouter(theEnv,WERROR,"Undefined variable ");
               EnvPrintRouter(theEnv,WERROR,ValueToString(bindName));
               EnvPrintRouter(theEnv,WERROR," referenced in ");
               EnvPrintRouter(theEnv,WERROR,bodytype);
               EnvPrintRouter(theEnv,WERROR,".\n");
               return(TRUE);
              }
           }

         /*===================================================*/
         /* Else if variable is defined in the parameter list */
         /* and not rebound within the procedure then...      */
         /*===================================================*/

         else if ((position > 0) && (boundPosn == 0))
           {
            actions->type = (unsigned short) ((bindName != wildcard) ? PROC_PARAM : PROC_WILD_PARAM);
            actions->value = EnvAddBitMap(theEnv,(void *) &position,(int) sizeof(int));
           }

         /*=========================================================*/
         /* Else the variable is rebound within the procedure so... */
         /*=========================================================*/

         else
           {
            if (altvarfunc != NULL)
              {
               altvarexp = GenConstant(theEnv,actions->type,actions->value);
               altcode = (*altvarfunc)(theEnv,altvarexp,specdata);
               if (altcode == 0)
                 {
                  rtn_struct(theEnv,expr,altvarexp);
                  altvarexp = NULL;
                 }
               else if (altcode == -1)
                 {
                  rtn_struct(theEnv,expr,altvarexp);
                  return(TRUE);
                 }
              }
            else
              altvarexp = NULL;
            actions->type = PROC_GET_BIND;
            ClearBitString((void *) &pvar,(int) sizeof(PACKED_PROC_VAR));
            pvar.first = boundPosn;
            pvar.second = position;
            pvar.secondFlag = (bindName != wildcard) ? 0 : 1;
            actions->value = EnvAddBitMap(theEnv,(void *) &pvar,(int) sizeof(PACKED_PROC_VAR));
            actions->argList = GenConstant(theEnv,SYMBOL,(void *) bindName);
            actions->argList->nextArg = altvarexp;
           }
        }
#if DEFGLOBAL_CONSTRUCT
      else if (actions->type == GBL_VARIABLE)
        {
         if (ReplaceGlobalVariable(theEnv,actions) == FALSE)
           return(-1);
        }
#endif
      if ((altvarfunc != NULL) ? ((*altvarfunc)(theEnv,actions,specdata) == -1) : FALSE)
        return(TRUE);
      if (actions->argList != NULL)
        {
         if (ReplaceProcVars(theEnv,bodytype,actions->argList,parameterList,
                                        wildcard,altvarfunc,specdata))
           return(TRUE);

         /* ====================================================================
            Check to see if this is a call to the bind function.  If so (and the
            second argument is a symbol) then it is a locally bound variable
            (as opposed to a global).

            Replace the call to "bind" with a call to PROC_BIND - the
            special internal function for procedure local variables.
            ==================================================================== */
         if ((actions->value == (void *) FindFunction(theEnv,"bind")) &&
             (actions->argList->type == SYMBOL))
           {
            actions->type = PROC_BIND;
            boundPosn = SearchParsedBindNames(theEnv,(SYMBOL_HN *) actions->argList->value);
            actions->value = EnvAddBitMap(theEnv,(void *) &boundPosn,(int) sizeof(intBool));
            arg_lvl = actions->argList->nextArg;
            rtn_struct(theEnv,expr,actions->argList);
            actions->argList = arg_lvl;
           }
        }
      actions = actions->nextArg;
     }
   return(FALSE);
  }

#if DEFGENERIC_CONSTRUCT

/*****************************************************
  NAME         : GenProcWildcardReference
  DESCRIPTION  : Returns an expression to access the
                    wildcard parameter for a method
  INPUTS       : The starting index of the wildcard
  RETURNS      : An expression containing the wildcard
                 reference
  SIDE EFFECTS : Expression allocated
  NOTES        : None
 *****************************************************/
globle EXPRESSION *GenProcWildcardReference(
  void *theEnv,
  int theIndex)
  {
   return(GenConstant(theEnv,PROC_WILD_PARAM,EnvAddBitMap(theEnv,(void *) &theIndex,(int) sizeof(int))));
  }

#endif

#endif

/*******************************************************************
  NAME         : PushProcParameters
  DESCRIPTION  : Given a list of parameter expressions,
                   this function evaluates each expression
                   and stores the results in a contiguous
                   array of DATA_OBJECTS.  Used in creating a new
                   ProcParamArray for the execution of a
                   procedure
                 The current arrays are saved on a stack.
  INPUTS       : 1) The paramter expression list
                 2) The number of parameters in the list
                 3) The name of the procedure for which
                    these parameters are being evaluated
                 4) The type of procedure
                 5) A pointer to a function to print out a trace
                    message about the currently executing
                    procedure when unbound variables are detected
                    at runtime (The function should take no
                    arguments and have no return value.  The
                    function should print its synopsis to WERROR
                    and include the final carriage-return.)
  RETURNS      : Nothing useful
  SIDE EFFECTS : Any side-effects of the evaluation of the
                   parameter expressions
                 DATA_OBJECT array allocated (deallocated on errors)
                 ProcParamArray set
  NOTES        : EvaluationError set on errors
 *******************************************************************/
globle void PushProcParameters(
  void *theEnv,
  EXPRESSION *parameterList,
  int numberOfParameters,
  char *pname,
  char *bodytype,
  void (*UnboundErrFunc)(void *))
  {
   register PROC_PARAM_STACK *ptmp;

   ptmp = get_struct(theEnv,ProcParamStack);
   ptmp->ParamArray = ProceduralPrimitiveData(theEnv)->ProcParamArray;
   ptmp->ParamArraySize = ProceduralPrimitiveData(theEnv)->ProcParamArraySize;
   ptmp->UnboundErrFunc = ProceduralPrimitiveData(theEnv)->ProcUnboundErrFunc;
   ptmp->nxt = ProceduralPrimitiveData(theEnv)->pstack;
   ProceduralPrimitiveData(theEnv)->pstack = ptmp;
   EvaluateProcParameters(theEnv,parameterList,numberOfParameters,pname,bodytype);
   if (EvaluationData(theEnv)->EvaluationError)
     {
      ptmp = ProceduralPrimitiveData(theEnv)->pstack;
      ProceduralPrimitiveData(theEnv)->pstack = ProceduralPrimitiveData(theEnv)->pstack->nxt;
      rtn_struct(theEnv,ProcParamStack,ptmp);
      return;
     }

   /* ================================================================
      Record ProcParamExpressions and WildcardValue for previous frame
      AFTER evaluating arguments for the new frame, because they could
      have gone from NULL to non-NULL (if they were already non-NULL,
      they would remain unchanged.)
      ================================================================ */
#if DEFGENERIC_CONSTRUCT
   ptmp->ParamExpressions = ProceduralPrimitiveData(theEnv)->ProcParamExpressions;
   ProceduralPrimitiveData(theEnv)->ProcParamExpressions = NULL;
#endif
   ptmp->WildcardValue = ProceduralPrimitiveData(theEnv)->WildcardValue;
   ProceduralPrimitiveData(theEnv)->WildcardValue = NULL;
   ProceduralPrimitiveData(theEnv)->ProcUnboundErrFunc = UnboundErrFunc;
  }

/******************************************************************
  NAME         : PopProcParameters
  DESCRIPTION  : Restores old procedure arrays
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Stack popped and globals restored
  NOTES        : Assumes pstack != NULL
 ******************************************************************/
globle void PopProcParameters(
  void *theEnv)
  {
   register PROC_PARAM_STACK *ptmp;

   if (ProceduralPrimitiveData(theEnv)->ProcParamArray != NULL)
     rm(theEnv,(void *) ProceduralPrimitiveData(theEnv)->ProcParamArray,(sizeof(DATA_OBJECT) * ProceduralPrimitiveData(theEnv)->ProcParamArraySize));

#if DEFGENERIC_CONSTRUCT
   if (ProceduralPrimitiveData(theEnv)->ProcParamExpressions != NULL)
     rm(theEnv,(void *) ProceduralPrimitiveData(theEnv)->ProcParamExpressions,(sizeof(EXPRESSION) * ProceduralPrimitiveData(theEnv)->ProcParamArraySize));
#endif

   ptmp = ProceduralPrimitiveData(theEnv)->pstack;
   ProceduralPrimitiveData(theEnv)->pstack = ProceduralPrimitiveData(theEnv)->pstack->nxt;
   ProceduralPrimitiveData(theEnv)->ProcParamArray = ptmp->ParamArray;
   ProceduralPrimitiveData(theEnv)->ProcParamArraySize = ptmp->ParamArraySize;

#if DEFGENERIC_CONSTRUCT
   ProceduralPrimitiveData(theEnv)->ProcParamExpressions = ptmp->ParamExpressions;
#endif

   if (ProceduralPrimitiveData(theEnv)->WildcardValue != NULL)
     {
      MultifieldDeinstall(theEnv,(MULTIFIELD_PTR) ProceduralPrimitiveData(theEnv)->WildcardValue->value);
      if (ProceduralPrimitiveData(theEnv)->WildcardValue->value != ProceduralPrimitiveData(theEnv)->NoParamValue)
        AddToMultifieldList(theEnv,(MULTIFIELD_PTR) ProceduralPrimitiveData(theEnv)->WildcardValue->value);
      rtn_struct(theEnv,dataObject,ProceduralPrimitiveData(theEnv)->WildcardValue);
     }
   ProceduralPrimitiveData(theEnv)->WildcardValue = ptmp->WildcardValue;
   ProceduralPrimitiveData(theEnv)->ProcUnboundErrFunc = ptmp->UnboundErrFunc;
   rtn_struct(theEnv,ProcParamStack,ptmp);
  }

/******************************************************************
  NAME         : ReleaseProcParameters
  DESCRIPTION  : Restores old procedure arrays
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Stack popped and globals restored
  NOTES        : Assumes pstack != NULL
 ******************************************************************/
static void ReleaseProcParameters(
  void *theEnv)
  {
   register PROC_PARAM_STACK *ptmp, *next;

   if (ProceduralPrimitiveData(theEnv)->ProcParamArray != NULL)
     rm(theEnv,(void *) ProceduralPrimitiveData(theEnv)->ProcParamArray,(sizeof(DATA_OBJECT) * ProceduralPrimitiveData(theEnv)->ProcParamArraySize));


   if (ProceduralPrimitiveData(theEnv)->WildcardValue != NULL)
     {
      if (ProceduralPrimitiveData(theEnv)->WildcardValue->value != ProceduralPrimitiveData(theEnv)->NoParamValue)
        { ReturnMultifield(theEnv,(struct multifield *) ProceduralPrimitiveData(theEnv)->WildcardValue->value); }
     
      rtn_struct(theEnv,dataObject,ProceduralPrimitiveData(theEnv)->WildcardValue); 
     }
     
#if DEFGENERIC_CONSTRUCT
   if (ProceduralPrimitiveData(theEnv)->ProcParamExpressions != NULL)
     rm(theEnv,(void *) ProceduralPrimitiveData(theEnv)->ProcParamExpressions,(sizeof(EXPRESSION) * ProceduralPrimitiveData(theEnv)->ProcParamArraySize));
#endif

   ptmp = ProceduralPrimitiveData(theEnv)->pstack;
   
   while (ptmp != NULL)
     {
      next = ptmp->nxt;

      if (ptmp->ParamArray != NULL)
        { rm(theEnv,(void *) ptmp->ParamArray,(sizeof(DATA_OBJECT) * ptmp->ParamArraySize)); }

#if DEFGENERIC_CONSTRUCT
      if (ptmp->ParamExpressions != NULL)
        { rm(theEnv,(void *) ptmp->ParamExpressions,(sizeof(EXPRESSION) * ptmp->ParamArraySize)); }
#endif

      if (ptmp->WildcardValue != NULL)
        { 
         if (ptmp->WildcardValue->value != ProceduralPrimitiveData(theEnv)->NoParamValue)
           { ReturnMultifield(theEnv,(struct multifield *) ptmp->WildcardValue->value); }

         rtn_struct(theEnv,dataObject,ptmp->WildcardValue); 
        }
     
      rtn_struct(theEnv,ProcParamStack,ptmp);
      ptmp = next;
     }
  }
  
#if DEFGENERIC_CONSTRUCT

/***********************************************************
  NAME         : GetProcParamExpressions
  DESCRIPTION  : Forms an array of expressions equivalent to
                 the current procedure paramter array.  Used
                 to conveniently attach these parameters as
                 arguments to a H/L system function call
                 (used by the generic dispatch).
  INPUTS       : None
  RETURNS      : A pointer to an array of expressions
  SIDE EFFECTS : Expression array created
  NOTES        : None
 ***********************************************************/
globle EXPRESSION *GetProcParamExpressions(
  void *theEnv)
  {
   register int i;

   if ((ProceduralPrimitiveData(theEnv)->ProcParamArray == NULL) || (ProceduralPrimitiveData(theEnv)->ProcParamExpressions != NULL))
     return(ProceduralPrimitiveData(theEnv)->ProcParamExpressions);
   ProceduralPrimitiveData(theEnv)->ProcParamExpressions = (EXPRESSION *)
               gm2(theEnv,(sizeof(EXPRESSION) * ProceduralPrimitiveData(theEnv)->ProcParamArraySize));
   for (i = 0 ; i < ProceduralPrimitiveData(theEnv)->ProcParamArraySize ; i++)
     {
      ProceduralPrimitiveData(theEnv)->ProcParamExpressions[i].type = ProceduralPrimitiveData(theEnv)->ProcParamArray[i].type;
      if (ProceduralPrimitiveData(theEnv)->ProcParamArray[i].type != MULTIFIELD)
        ProceduralPrimitiveData(theEnv)->ProcParamExpressions[i].value = ProceduralPrimitiveData(theEnv)->ProcParamArray[i].value;
      else
        ProceduralPrimitiveData(theEnv)->ProcParamExpressions[i].value = (void *) &ProceduralPrimitiveData(theEnv)->ProcParamArray[i];
      ProceduralPrimitiveData(theEnv)->ProcParamExpressions[i].argList = NULL;
      ProceduralPrimitiveData(theEnv)->ProcParamExpressions[i].nextArg =
        ((i + 1) != ProceduralPrimitiveData(theEnv)->ProcParamArraySize) ? &ProceduralPrimitiveData(theEnv)->ProcParamExpressions[i+1] : NULL;
     }
   return(ProceduralPrimitiveData(theEnv)->ProcParamExpressions);
  }

#endif

/***********************************************************
  NAME         : EvaluateProcActions
  DESCRIPTION  : Evaluates the actions of a deffunction,
                 generic function method or message-handler.
  INPUTS       : 1) The module where the actions should be
                    executed
                 2) The actions (linked by nextArg fields)
                 3) The number of local variables to reserve
                    space for.
                 4) A buffer to hold the result of evaluating
                    the actions.
                 5) A function which prints out the name of
                    the currently executing body for error
                    messages (can be NULL).
  RETURNS      : Nothing useful
  SIDE EFFECTS : Allocates and deallocates space for
                 local variable array.
  NOTES        : None
 ***********************************************************/
globle void EvaluateProcActions(
  void *theEnv,
  struct defmodule *theModule,
  EXPRESSION *actions,
  int lvarcnt,
  DATA_OBJECT *result,
  void (*crtproc)(void *))
  {
   DATA_OBJECT *oldLocalVarArray;
   register int i;
   struct defmodule *oldModule;
   EXPRESSION *oldActions;
   struct trackedMemory *theTM;

   oldLocalVarArray = ProceduralPrimitiveData(theEnv)->LocalVarArray;
   ProceduralPrimitiveData(theEnv)->LocalVarArray = (lvarcnt == 0) ? NULL :
                   (DATA_OBJECT *) gm2(theEnv,(sizeof(DATA_OBJECT) * lvarcnt));

   if (lvarcnt != 0)
     { theTM = AddTrackedMemory(theEnv,ProceduralPrimitiveData(theEnv)->LocalVarArray,sizeof(DATA_OBJECT) * lvarcnt); }
   else
     { theTM = NULL; }
     
   for (i = 0 ; i < lvarcnt ; i++)
     ProceduralPrimitiveData(theEnv)->LocalVarArray[i].supplementalInfo = EnvFalseSymbol(theEnv);

   oldModule = ((struct defmodule *) EnvGetCurrentModule(theEnv));
   if (oldModule != theModule)
     EnvSetCurrentModule(theEnv,(void *) theModule);
   oldActions = ProceduralPrimitiveData(theEnv)->CurrentProcActions;
   ProceduralPrimitiveData(theEnv)->CurrentProcActions = actions;

   if (EvaluateExpression(theEnv,actions,result))
     {
      result->type = SYMBOL;
      result->value = EnvFalseSymbol(theEnv);
     }

   ProceduralPrimitiveData(theEnv)->CurrentProcActions = oldActions;
   if (oldModule != ((struct defmodule *) EnvGetCurrentModule(theEnv)))
     EnvSetCurrentModule(theEnv,(void *) oldModule);
   if ((crtproc != NULL) ? EvaluationData(theEnv)->HaltExecution : FALSE)
     {
      PrintErrorID(theEnv,"PRCCODE",4,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Execution halted during the actions of ");
      (*crtproc)(theEnv);
     }
   if ((ProceduralPrimitiveData(theEnv)->WildcardValue != NULL) ? (result->value == ProceduralPrimitiveData(theEnv)->WildcardValue->value) : FALSE)
     {
      MultifieldDeinstall(theEnv,(MULTIFIELD_PTR) ProceduralPrimitiveData(theEnv)->WildcardValue->value);
      if (ProceduralPrimitiveData(theEnv)->WildcardValue->value != ProceduralPrimitiveData(theEnv)->NoParamValue)
        AddToMultifieldList(theEnv,(MULTIFIELD_PTR) ProceduralPrimitiveData(theEnv)->WildcardValue->value);
      rtn_struct(theEnv,dataObject,ProceduralPrimitiveData(theEnv)->WildcardValue);
      ProceduralPrimitiveData(theEnv)->WildcardValue = NULL;
     }

   if (lvarcnt != 0)
     {
      RemoveTrackedMemory(theEnv,theTM);
      for (i = 0 ; i < lvarcnt ; i++)
        if (ProceduralPrimitiveData(theEnv)->LocalVarArray[i].supplementalInfo == EnvTrueSymbol(theEnv))
          ValueDeinstall(theEnv,&ProceduralPrimitiveData(theEnv)->LocalVarArray[i]);
      rm(theEnv,(void *) ProceduralPrimitiveData(theEnv)->LocalVarArray,(sizeof(DATA_OBJECT) * lvarcnt));
     }

   ProceduralPrimitiveData(theEnv)->LocalVarArray = oldLocalVarArray;
  }

/****************************************************
  NAME         : PrintProcParamArray
  DESCRIPTION  : Displays the contents of the
                 current procedure parameter array
  INPUTS       : The logical name of the output
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 ****************************************************/
globle void PrintProcParamArray(
  void *theEnv,
  char *logName)
  {
   register int i;

   EnvPrintRouter(theEnv,logName," (");
   for (i = 0 ; i < ProceduralPrimitiveData(theEnv)->ProcParamArraySize ; i++)
     {
      PrintDataObject(theEnv,logName,&ProceduralPrimitiveData(theEnv)->ProcParamArray[i]);
      if (i != ProceduralPrimitiveData(theEnv)->ProcParamArraySize-1)
        EnvPrintRouter(theEnv,logName," ");
     }
   EnvPrintRouter(theEnv,logName,")\n");
  }

/****************************************************************
  NAME         : GrabProcWildargs
  DESCRIPTION  : Groups a portion of the ProcParamArray
                   into a multi-field variable
  INPUTS       : 1) Starting index in ProcParamArray
                      for grouping of arguments into
                      multi-field variable
                 2) Caller's result value buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Multi-field variable allocated and set
                   with corresponding values of ProcParamArray
  NOTES        : Multi-field is NOT on list of ephemeral segments
 ****************************************************************/
globle void GrabProcWildargs(
  void *theEnv,
  DATA_OBJECT *result,
  int theIndex)
  {
   register int i,j;
   long k; /* 6.04 Bug Fix */
   long size;
   DATA_OBJECT *val;

   result->type = MULTIFIELD;
   result->begin = 0;
   if (ProceduralPrimitiveData(theEnv)->WildcardValue == NULL)
     {
      ProceduralPrimitiveData(theEnv)->WildcardValue = get_struct(theEnv,dataObject);
      ProceduralPrimitiveData(theEnv)->WildcardValue->begin = 0;
     }
   else if (theIndex == ProceduralPrimitiveData(theEnv)->Oldindex)
     {
      result->end = ProceduralPrimitiveData(theEnv)->WildcardValue->end;
      result->value = ProceduralPrimitiveData(theEnv)->WildcardValue->value;
      return;
     }
   else
     {
      MultifieldDeinstall(theEnv,(MULTIFIELD_PTR) ProceduralPrimitiveData(theEnv)->WildcardValue->value);
      if (ProceduralPrimitiveData(theEnv)->WildcardValue->value != ProceduralPrimitiveData(theEnv)->NoParamValue)
        AddToMultifieldList(theEnv,(MULTIFIELD_PTR) ProceduralPrimitiveData(theEnv)->WildcardValue->value);
     }
   ProceduralPrimitiveData(theEnv)->Oldindex = theIndex;
   size = ProceduralPrimitiveData(theEnv)->ProcParamArraySize - theIndex + 1;
   if (size <= 0)
     {
      result->end = ProceduralPrimitiveData(theEnv)->WildcardValue->end = -1;
      result->value = ProceduralPrimitiveData(theEnv)->WildcardValue->value = ProceduralPrimitiveData(theEnv)->NoParamValue;
      MultifieldInstall(theEnv,(MULTIFIELD_PTR) ProceduralPrimitiveData(theEnv)->WildcardValue->value);
      return;
     }
   for (i = theIndex-1 ; i < ProceduralPrimitiveData(theEnv)->ProcParamArraySize ; i++)
     {
      if (ProceduralPrimitiveData(theEnv)->ProcParamArray[i].type == MULTIFIELD)
        size += ProceduralPrimitiveData(theEnv)->ProcParamArray[i].end - ProceduralPrimitiveData(theEnv)->ProcParamArray[i].begin;
     }
   result->end = ProceduralPrimitiveData(theEnv)->WildcardValue->end = size-1;
   result->value = ProceduralPrimitiveData(theEnv)->WildcardValue->value = (void *) CreateMultifield2(theEnv,(unsigned long) size);
   for (i = theIndex-1 , j = 1 ; i < ProceduralPrimitiveData(theEnv)->ProcParamArraySize ; i++)
     {
      if (ProceduralPrimitiveData(theEnv)->ProcParamArray[i].type != MULTIFIELD)
        {
         SetMFType(result->value,j,(short) ProceduralPrimitiveData(theEnv)->ProcParamArray[i].type);
         SetMFValue(result->value,j,ProceduralPrimitiveData(theEnv)->ProcParamArray[i].value);
         j++;
        }
      else
        {
         val = &ProceduralPrimitiveData(theEnv)->ProcParamArray[i];
         for (k = val->begin + 1 ; k <= val->end + 1 ; k++ , j++)
           {
            SetMFType(result->value,j,GetMFType(val->value,k));
            SetMFValue(result->value,j,GetMFValue(val->value,k));
           }
        }
     }
   MultifieldInstall(theEnv,(MULTIFIELD_PTR) ProceduralPrimitiveData(theEnv)->WildcardValue->value);
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/*******************************************************************
  NAME         : EvaluateProcParameters
  DESCRIPTION  : Given a list of parameter expressions,
                   this function evaluates each expression
                   and stores the results in a contiguous
                   array of DATA_OBJECTS.  Used in creating a new
                   ProcParamArray for the execution of a
                   procedure
  INPUTS       : 1) The paramter expression list
                 2) The number of parameters in the list
                 3) The name of the procedure for which
                    these parameters are being evaluated
                 4) The type of procedure
  RETURNS      : Nothing useful
  SIDE EFFECTS : Any side-effects of the evaluation of the
                   parameter expressions
                 DATA_OBJECT array allocated (deallocated on errors)
                 ProcParamArray set
  NOTES        : EvaluationError set on errors
 *******************************************************************/
static void EvaluateProcParameters(
  void *theEnv,
  EXPRESSION *parameterList,
  int numberOfParameters,
  char *pname,
  char *bodytype)
  {
   DATA_OBJECT *rva,temp;
   int i = 0;

   if (numberOfParameters == 0)
     {
      ProceduralPrimitiveData(theEnv)->ProcParamArray = NULL;
      ProceduralPrimitiveData(theEnv)->ProcParamArraySize = 0;
      return;
     }

   rva = (DATA_OBJECT *) gm2(theEnv,(sizeof(DATA_OBJECT) * numberOfParameters));
   while (parameterList != NULL)
     {
      if ((EvaluateExpression(theEnv,parameterList,&temp) == TRUE) ? TRUE :
          (temp.type == RVOID))
        {
         if (temp.type == RVOID)
           {
            PrintErrorID(theEnv,"PRCCODE",2,FALSE);
            EnvPrintRouter(theEnv,WERROR,"Functions without a return value are illegal as ");
            EnvPrintRouter(theEnv,WERROR,bodytype);
            EnvPrintRouter(theEnv,WERROR," arguments.\n");
            SetEvaluationError(theEnv,TRUE);
           }
         PrintErrorID(theEnv,"PRCCODE",6,FALSE);
         EnvPrintRouter(theEnv,WERROR,"This error occurred while evaluating arguments ");
         EnvPrintRouter(theEnv,WERROR,"for the ");
         EnvPrintRouter(theEnv,WERROR,bodytype);
         EnvPrintRouter(theEnv,WERROR," ");
         EnvPrintRouter(theEnv,WERROR,pname);
         EnvPrintRouter(theEnv,WERROR,".\n");
         rm(theEnv,(void *) rva,(sizeof(DATA_OBJECT) * numberOfParameters));
         return;
        }
      rva[i].type = temp.type;
      rva[i].value = temp.value;
      rva[i].begin = temp.begin;
      rva[i].end = temp.end;
      parameterList = parameterList->nextArg;
      i++;
     }
   ProceduralPrimitiveData(theEnv)->ProcParamArraySize = numberOfParameters;
   ProceduralPrimitiveData(theEnv)->ProcParamArray = rva;
  }

/***************************************************
  NAME         : RtnProcParam
  DESCRIPTION  : Internal function for getting the
                   value of an argument passed to
                   a procedure
  INPUTS       : 1) Expression to evaluate
                    (PROC_PARAM index)
                 2) Caller's result value buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Caller's buffer set to specified
                   node of ProcParamArray
  NOTES        : None
 ***************************************************/
static intBool RtnProcParam(
  void *theEnv,
  void *value,
  DATA_OBJECT *result)
  {
   register DATA_OBJECT *src;
   
   src = &ProceduralPrimitiveData(theEnv)->ProcParamArray[*((int *) ValueToBitMap(value)) - 1];
   result->type = src->type;
   result->value = src->value;
   result->begin = src->begin;
   result->end = src->end;
   return(TRUE);
  }

/**************************************************************
  NAME         : GetProcBind
  DESCRIPTION  : Internal function for looking up the
                    values of parameters or bound variables
                    within procedures
  INPUTS       : 1) Expression to evaluate
                    (PROC_GET_BIND index)
                 2) Caller's result value buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Caller's buffer set to parameter value in
                   ProcParamArray or the value in LocalVarArray
  NOTES        : None
 **************************************************************/
static intBool GetProcBind(
  void *theEnv,
  void *value,
  DATA_OBJECT *result)
  {
   register DATA_OBJECT *src;
   PACKED_PROC_VAR *pvar;

   pvar = (PACKED_PROC_VAR *) ValueToBitMap(value);
   src = &ProceduralPrimitiveData(theEnv)->LocalVarArray[pvar->first - 1];
   if (src->supplementalInfo == EnvTrueSymbol(theEnv))
     {
      result->type = src->type;
      result->value = src->value;
      result->begin = src->begin;
      result->end = src->end;
      return(TRUE);
     }
   if (GetFirstArgument()->nextArg != NULL)
     {
      EvaluateExpression(theEnv,GetFirstArgument()->nextArg,result);
      return(TRUE);
     }
   if (pvar->second == 0)
     {
      PrintErrorID(theEnv,"PRCCODE",5,FALSE);
      SetEvaluationError(theEnv,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Variable ");
      EnvPrintRouter(theEnv,WERROR,ValueToString(GetFirstArgument()->value));
      if (ProceduralPrimitiveData(theEnv)->ProcUnboundErrFunc != NULL)
        {
         EnvPrintRouter(theEnv,WERROR," unbound in ");
         (*ProceduralPrimitiveData(theEnv)->ProcUnboundErrFunc)(theEnv);
        }
      else
        EnvPrintRouter(theEnv,WERROR," unbound.\n");
      result->type = SYMBOL;
      result->value = EnvFalseSymbol(theEnv);
      return(TRUE);
     }
   if (pvar->secondFlag == 0)
     {
      src = &ProceduralPrimitiveData(theEnv)->ProcParamArray[pvar->second - 1];
      result->type = src->type;
      result->value = src->value;
      result->begin = src->begin;
      result->end = src->end;
     }
   else
     GrabProcWildargs(theEnv,result,(int) pvar->second);
   return(TRUE);
  }

/**************************************************************
  NAME         : PutProcBind
  DESCRIPTION  : Internal function for setting the values of
                 of locally bound variables within procedures
  INPUTS       : 1) Expression to evaluate
                    (PROC_PARAM index)
                 2) Caller's result value buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Bound variable in LocalVarArray set to
                   value in caller's buffer.
  NOTES        : None
 **************************************************************/
static intBool PutProcBind(
  void *theEnv,
  void *value,
  DATA_OBJECT *result)
  {
   register DATA_OBJECT *dst;

   dst = &ProceduralPrimitiveData(theEnv)->LocalVarArray[*((int *) ValueToBitMap(value)) - 1];
   if (GetFirstArgument() == NULL)
     {
      if (dst->supplementalInfo == EnvTrueSymbol(theEnv))
        ValueDeinstall(theEnv,dst);
      dst->supplementalInfo = EnvFalseSymbol(theEnv);
      result->type = SYMBOL;
      result->value = EnvFalseSymbol(theEnv);
     }
   else
     {
      if (GetFirstArgument()->nextArg != NULL)
        StoreInMultifield(theEnv,result,GetFirstArgument(),TRUE);
      else
        EvaluateExpression(theEnv,GetFirstArgument(),result);
      if (dst->supplementalInfo == EnvTrueSymbol(theEnv))
        ValueDeinstall(theEnv,dst);
      dst->supplementalInfo = EnvTrueSymbol(theEnv);
      dst->type = result->type;
      dst->value = result->value;
      dst->begin = result->begin;
      dst->end = result->end;
      ValueInstall(theEnv,dst);
     }
   return(TRUE);
  }

/****************************************************************
  NAME         : RtnProcWild
  DESCRIPTION  : Groups a portion of the ProcParamArray
                   into a multi-field variable
  INPUTS       : 1) Starting index in ProcParamArray
                      for grouping of arguments into
                      multi-field variable (expression value)
                 2) Caller's result value buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Multi-field variable allocated and set
                   with corresponding values of ProcParamArray
  NOTES        : Multi-field is NOT on list of ephemeral segments
 ****************************************************************/
static intBool RtnProcWild(
  void *theEnv,
  void *value,
  DATA_OBJECT *result)
  {
   GrabProcWildargs(theEnv,result,*(int *) ValueToBitMap(value));
   return(TRUE);
  }

#if (! BLOAD_ONLY) && (! RUN_TIME)

/***************************************************
  NAME         : FindProcParameter
  DESCRIPTION  : Determines the relative position in
                   an n-element list of a certain
                   parameter.  The index is 1..n.
  INPUTS       : 1) Parameter name
                 2) Parameter list
                 3) Wildcard symbol (NULL if none)
  RETURNS      : Index of parameter in list, 0 if
                   not found
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
static int FindProcParameter(
  SYMBOL_HN *name,
  EXPRESSION *parameterList,
  SYMBOL_HN *wildcard)
  {
   int i = 1;

   while (parameterList != NULL)
     {
      if (parameterList->value == (void *) name)
        return(i);
      i++;
      parameterList = parameterList->nextArg;
     }

   /* ===================================================================
      Wildcard may not be stored in actual list but know is always at end
      =================================================================== */
   if (name == wildcard)
     return(i);
   return(0);
  }

/*************************************************************************
  NAME         : ReplaceProcBinds
  DESCRIPTION  : Examines an expression and replaces calls to the
                 "bind" function which are specially recognized

                 For example, in a message-handler,

                   (bind ?self <value>) would be illegal

                   and

                   (bind ?self:<slot-name> <value>) would be
                   replaced with
                   (put <slot-name> <value>)

  INPUTS       : 1) The actions in which to replace special binds
                 2) A pointer to a function to handle binds in a
                    special way. The function should accept the
                    bind function call expression and a specialized
                    data buffer (can be NULL) as arguments.
                    If the variable is recognized and treated specially,
                    the function should modify the expression
                    appropriately (including attaching/removing
                    any necessary argument expressions).  Return 1
                    if recognized, 0 if not, -1 on errors.
                    This argument CANNOT be NULL.
                 3) Specialized user data buffer
  RETURNS      : FALSE if OK, TRUE on errors
  SIDE EFFECTS : Some binds replaced with specialized calls
  NOTES        : Local variable binds are replaced in ReplaceProcVars
                 (after this routine has had a chance to replace all
                  special binds and remove the names from the parsed
                  bind list)
 *************************************************************************/
static int ReplaceProcBinds(
  void *theEnv,
  EXPRESSION *actions,
  int (*altbindfunc)(void *,EXPRESSION *,void *),
  void *userBuffer)
  {
   int bcode;
   SYMBOL_HN *bname;

   while (actions != NULL)
     {
      if (actions->argList != NULL)
        {
         if (ReplaceProcBinds(theEnv,actions->argList,altbindfunc,userBuffer))
           return(TRUE);
         if ((actions->value == (void *) FindFunction(theEnv,"bind")) &&
             (actions->argList->type == SYMBOL))
           {
            bname = (SYMBOL_HN *) actions->argList->value;
            bcode = (*altbindfunc)(theEnv,actions,userBuffer);
            if (bcode == -1)
              return(TRUE);
            if (bcode == 1)
              RemoveParsedBindName(theEnv,bname);
           }
        }
      actions = actions->nextArg;
     }
   return(FALSE);
  }

/*****************************************************
  NAME         : CompactActions
  DESCRIPTION  : Examines a progn expression chain,
                 and if there is only one action,
                 the progn header is deallocated and
                 the action is returned.  If there are
                 no actions, the progn expression is
                 modified to be the FALSE symbol
                 and returned.  Otherwise, the progn
                 is simply returned.
  INPUTS       : The action expression
  RETURNS      : The compacted expression
  SIDE EFFECTS : Some expressions possibly deallocated
  NOTES        : Assumes actions is a progn expression
                 and actions->nextArg == NULL
 *****************************************************/
static EXPRESSION *CompactActions(
  void *theEnv,
  EXPRESSION *actions)
  {
   register struct expr *tmp;

   if (actions->argList == NULL)
     {
      actions->type = SYMBOL;
      actions->value = EnvFalseSymbol(theEnv);
     }
   else if (actions->argList->nextArg == NULL)
     {
      tmp = actions;
      actions = actions->argList;
      rtn_struct(theEnv,expr,tmp);
     }
   return(actions);
  }

#endif

#if (! DEFFUNCTION_CONSTRUCT) || (! DEFGENERIC_CONSTRUCT)

/******************************************************
  NAME         : EvaluateBadCall
  DESCRIPTION  : Default evaluation function for
                 deffunctions and gneric functions
                 in configurations where either
                 capability is not present.
  INPUTS       : 1) The function (ignored)
                 2) A data object buffer for the result
  RETURNS      : FALSE
  SIDE EFFECTS : Data object buffer set to the
                 symbol FALSE and evaluation error set
  NOTES        : Used for binary images which
                 contain deffunctions and generic
                 functions which cannot be used
 ******************************************************/
#if WIN_BTC
#pragma argsused
#endif
static intBool EvaluateBadCall(
  void *theEnv,
  void *value,
  DATA_OBJECT *result)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(value)
#endif
   PrintErrorID(theEnv,"PRCCODE",1,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Attempted to call a deffunction/generic function ");
   EnvPrintRouter(theEnv,WERROR,"which does not exist.\n");
   SetEvaluationError(theEnv,TRUE);
   SetpType(result,SYMBOL);
   SetpValue(result,EnvFalseSymbol(theEnv));
   return(FALSE);
  }

#endif

