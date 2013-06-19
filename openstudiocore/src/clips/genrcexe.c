   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  05/17/06            */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Generic Function Execution Routines              */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Removed IMPERATIVE_METHODS compilation flag.   */
/*                                                           */
/*************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if DEFGENERIC_CONSTRUCT

#if OBJECT_SYSTEM
#include "classcom.h"
#include "classfun.h"
#include "insfun.h"
#endif

#include "argacces.h"
#include "constrct.h"
#include "envrnmnt.h"
#include "genrccom.h"
#include "prcdrfun.h"
#include "prccode.h"
#include "proflfun.h"
#include "router.h"
#include "utility.h"

#define _GENRCEXE_SOURCE_
#include "genrcexe.h"

/* =========================================
   *****************************************
                   CONSTANTS
   =========================================
   ***************************************** */

#define BEGIN_TRACE     ">>"
#define END_TRACE       "<<"

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static DEFMETHOD *FindApplicableMethod(void *,DEFGENERIC *,DEFMETHOD *);

#if DEBUGGING_FUNCTIONS
static void WatchGeneric(void *,char *);
static void WatchMethod(void *,char *);
#endif

#if OBJECT_SYSTEM
static DEFCLASS *DetermineRestrictionClass(void *,DATA_OBJECT *);
#endif

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***********************************************************************************
  NAME         : GenericDispatch
  DESCRIPTION  : Executes the most specific applicable method
  INPUTS       : 1) The generic function
                 2) The method to start after in the search for an applicable
                    method (ignored if arg #3 is not NULL).
                 3) A specific method to call (NULL if want highest precedence
                    method to be called)
                 4) The generic function argument expressions
                 5) The caller's result value buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Any side-effects of evaluating the generic function arguments
                 Any side-effects of evaluating query functions on method parameter
                   restrictions when determining the core (see warning #1)
                 Any side-effects of actual execution of methods (see warning #2)
                 Caller's buffer set to the result of the generic function call

                 In case of errors, the result is FALSE, otherwise it is the
                   result returned by the most specific method (which can choose
                   to ignore or return the values of more general methods)
  NOTES        : WARNING #1: Query functions on method parameter restrictions
                    should not have side-effects, for they might be evaluated even
                    for methods that aren't applicable to the generic function call.
                 WARNING #2: Side-effects of method execution should not always rely
                    on only being executed once per generic function call.  Every
                    time a method calls (shadow-call) the same next-most-specific
                    method is executed.  Thus, it is possible for a method to be
                    executed multiple times per generic function call.
 ***********************************************************************************/
globle void GenericDispatch(
  void *theEnv,
  DEFGENERIC *gfunc,
  DEFMETHOD *prevmeth,
  DEFMETHOD *meth,
  EXPRESSION *params,
  DATA_OBJECT *result)
  {
   DEFGENERIC *previousGeneric;
   DEFMETHOD *previousMethod;
   int oldce;
#if PROFILING_FUNCTIONS
   struct profileFrameInfo profileFrame;
#endif

   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   EvaluationData(theEnv)->EvaluationError = FALSE;
   if (EvaluationData(theEnv)->HaltExecution)
     return;
   oldce = ExecutingConstruct(theEnv);
   SetExecutingConstruct(theEnv,TRUE);
   previousGeneric = DefgenericData(theEnv)->CurrentGeneric;
   previousMethod = DefgenericData(theEnv)->CurrentMethod;
   DefgenericData(theEnv)->CurrentGeneric = gfunc;
   EvaluationData(theEnv)->CurrentEvaluationDepth++;
   gfunc->busy++;
   PushProcParameters(theEnv,params,CountArguments(params),
                      EnvGetDefgenericName(theEnv,(void *) gfunc),
                      "generic function",UnboundMethodErr);
   if (EvaluationData(theEnv)->EvaluationError)
     {
      gfunc->busy--;
      DefgenericData(theEnv)->CurrentGeneric = previousGeneric;
      DefgenericData(theEnv)->CurrentMethod = previousMethod;
      EvaluationData(theEnv)->CurrentEvaluationDepth--;
      PeriodicCleanup(theEnv,FALSE,TRUE);
      SetExecutingConstruct(theEnv,oldce);
      return;
     }
   if (meth != NULL)
     {
      if (IsMethodApplicable(theEnv,meth))
        {
         meth->busy++;
         DefgenericData(theEnv)->CurrentMethod = meth;
        }
      else
        {
         PrintErrorID(theEnv,"GENRCEXE",4,FALSE);
         SetEvaluationError(theEnv,TRUE);
         DefgenericData(theEnv)->CurrentMethod = NULL;
         EnvPrintRouter(theEnv,WERROR,"Generic function ");
         EnvPrintRouter(theEnv,WERROR,EnvGetDefgenericName(theEnv,(void *) gfunc));
         EnvPrintRouter(theEnv,WERROR," method #");
         PrintLongInteger(theEnv,WERROR,(long long) meth->index);
         EnvPrintRouter(theEnv,WERROR," is not applicable to the given arguments.\n");
        }
     }
   else
     DefgenericData(theEnv)->CurrentMethod = FindApplicableMethod(theEnv,gfunc,prevmeth);
   if (DefgenericData(theEnv)->CurrentMethod != NULL)
     {
#if DEBUGGING_FUNCTIONS
      if (DefgenericData(theEnv)->CurrentGeneric->trace)
        WatchGeneric(theEnv,BEGIN_TRACE);
      if (DefgenericData(theEnv)->CurrentMethod->trace)
        WatchMethod(theEnv,BEGIN_TRACE);
#endif
      if (DefgenericData(theEnv)->CurrentMethod->system)
        {
         EXPRESSION fcall;

         fcall.type = FCALL;
         fcall.value = DefgenericData(theEnv)->CurrentMethod->actions->value;
         fcall.nextArg = NULL;
         fcall.argList = GetProcParamExpressions(theEnv);
         EvaluateExpression(theEnv,&fcall,result);
        }
      else
        {
#if PROFILING_FUNCTIONS
         StartProfile(theEnv,&profileFrame,
                      &DefgenericData(theEnv)->CurrentMethod->usrData,
                      ProfileFunctionData(theEnv)->ProfileConstructs);
#endif

         EvaluateProcActions(theEnv,DefgenericData(theEnv)->CurrentGeneric->header.whichModule->theModule,
                             DefgenericData(theEnv)->CurrentMethod->actions,DefgenericData(theEnv)->CurrentMethod->localVarCount,
                             result,UnboundMethodErr);

#if PROFILING_FUNCTIONS
         EndProfile(theEnv,&profileFrame);
#endif
        }
      DefgenericData(theEnv)->CurrentMethod->busy--;
#if DEBUGGING_FUNCTIONS
      if (DefgenericData(theEnv)->CurrentMethod->trace)
        WatchMethod(theEnv,END_TRACE);
      if (DefgenericData(theEnv)->CurrentGeneric->trace)
        WatchGeneric(theEnv,END_TRACE);
#endif
     }
   else if (! EvaluationData(theEnv)->EvaluationError)
     {
      PrintErrorID(theEnv,"GENRCEXE",1,FALSE);
      EnvPrintRouter(theEnv,WERROR,"No applicable methods for ");
      EnvPrintRouter(theEnv,WERROR,EnvGetDefgenericName(theEnv,(void *) gfunc));
      EnvPrintRouter(theEnv,WERROR,".\n");
      SetEvaluationError(theEnv,TRUE);
     }
   gfunc->busy--;
   ProcedureFunctionData(theEnv)->ReturnFlag = FALSE;
   PopProcParameters(theEnv);
   DefgenericData(theEnv)->CurrentGeneric = previousGeneric;
   DefgenericData(theEnv)->CurrentMethod = previousMethod;
   EvaluationData(theEnv)->CurrentEvaluationDepth--;
   PropagateReturnValue(theEnv,result);
   PeriodicCleanup(theEnv,FALSE,TRUE);
   SetExecutingConstruct(theEnv,oldce);
  }

/*******************************************************
  NAME         : UnboundMethodErr
  DESCRIPTION  : Print out a synopis of the currently
                   executing method for unbound variable
                   errors
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Error synopsis printed to WERROR
  NOTES        : None
 *******************************************************/
globle void UnboundMethodErr(
  void *theEnv)
  {
   EnvPrintRouter(theEnv,WERROR,"generic function ");
   EnvPrintRouter(theEnv,WERROR,EnvGetDefgenericName(theEnv,(void *) DefgenericData(theEnv)->CurrentGeneric));
   EnvPrintRouter(theEnv,WERROR," method #");
   PrintLongInteger(theEnv,WERROR,(long long) DefgenericData(theEnv)->CurrentMethod->index);
   EnvPrintRouter(theEnv,WERROR,".\n");
  }

/***********************************************************************
  NAME         : IsMethodApplicable
  DESCRIPTION  : Tests to see if a method satsifies the arguments of a
                   generic function
                 A method is applicable if all its restrictions are
                   satisfied by the corresponding arguments
  INPUTS       : The method address
  RETURNS      : TRUE if method is applicable, FALSE otherwise
  SIDE EFFECTS : Any query functions are evaluated
  NOTES        : Uses globals ProcParamArraySize and ProcParamArray
 ***********************************************************************/
globle intBool IsMethodApplicable(
  void *theEnv,
  DEFMETHOD *meth)
  {
   DATA_OBJECT temp;
   short i,j,k;
   register RESTRICTION *rp;
#if OBJECT_SYSTEM
   void *type;
#else
   int type;
#endif

   if ((ProceduralPrimitiveData(theEnv)->ProcParamArraySize < meth->minRestrictions) ||
       ((ProceduralPrimitiveData(theEnv)->ProcParamArraySize > meth->minRestrictions) && (meth->maxRestrictions != -1)))
     return(FALSE);
   for (i = 0 , k = 0 ; i < ProceduralPrimitiveData(theEnv)->ProcParamArraySize ; i++)
     {
      rp = &meth->restrictions[k];
      if (rp->tcnt != 0)
        {
#if OBJECT_SYSTEM
         type = (void *) DetermineRestrictionClass(theEnv,&ProceduralPrimitiveData(theEnv)->ProcParamArray[i]);
         if (type == NULL)
           return(FALSE);
         for (j = 0 ; j < rp->tcnt ; j++)
           {
            if (type == rp->types[j])
              break;
            if (HasSuperclass((DEFCLASS *) type,(DEFCLASS *) rp->types[j]))
              break;
            if (rp->types[j] == (void *) DefclassData(theEnv)->PrimitiveClassMap[INSTANCE_ADDRESS])
              {
               if (ProceduralPrimitiveData(theEnv)->ProcParamArray[i].type == INSTANCE_ADDRESS)
                 break;
              }
            else if (rp->types[j] == (void *) DefclassData(theEnv)->PrimitiveClassMap[INSTANCE_NAME])
              {
               if (ProceduralPrimitiveData(theEnv)->ProcParamArray[i].type == INSTANCE_NAME)
                 break;
              }
            else if (rp->types[j] ==
                (void *) DefclassData(theEnv)->PrimitiveClassMap[INSTANCE_NAME]->directSuperclasses.classArray[0])
              {
               if ((ProceduralPrimitiveData(theEnv)->ProcParamArray[i].type == INSTANCE_NAME) ||
                   (ProceduralPrimitiveData(theEnv)->ProcParamArray[i].type == INSTANCE_ADDRESS))
                 break;
              }
           }
#else
         type = ProceduralPrimitiveData(theEnv)->ProcParamArray[i].type;
         for (j = 0 ; j < rp->tcnt ; j++)
           {
            if (type == ValueToInteger(rp->types[j]))
              break;
            if (SubsumeType(type,ValueToInteger(rp->types[j])))
              break;
           }
#endif
         if (j == rp->tcnt)
           return(FALSE);
        }
      if (rp->query != NULL)
        {
         DefgenericData(theEnv)->GenericCurrentArgument = &ProceduralPrimitiveData(theEnv)->ProcParamArray[i];
         EvaluateExpression(theEnv,rp->query,&temp);
         if ((temp.type != SYMBOL) ? FALSE :
             (temp.value == EnvFalseSymbol(theEnv)))
           return(FALSE);
        }
      if (((int) k) != meth->restrictionCount-1)
        k++;
     }
   return(TRUE);
  }

/***************************************************
  NAME         : NextMethodP
  DESCRIPTION  : Determines if a shadowed generic
                   function method is available for
                   execution
  INPUTS       : None
  RETURNS      : TRUE if there is a method available,
                   FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : H/L Syntax: (next-methodp)
 ***************************************************/
globle int NextMethodP(
  void *theEnv)
  {
   register DEFMETHOD *meth;

   if (DefgenericData(theEnv)->CurrentMethod == NULL)
     return(FALSE);
   meth = FindApplicableMethod(theEnv,DefgenericData(theEnv)->CurrentGeneric,DefgenericData(theEnv)->CurrentMethod);
   if (meth != NULL)
     {
      meth->busy--;
      return(TRUE);
     }
   return(FALSE);
  }

/****************************************************
  NAME         : CallNextMethod
  DESCRIPTION  : Executes the next available method
                   in the core for a generic function
  INPUTS       : Caller's buffer for the result
  RETURNS      : Nothing useful
  SIDE EFFECTS : Side effects of execution of shadow
                 EvaluationError set if no method
                   is available to execute.
  NOTES        : H/L Syntax: (call-next-method)
 ****************************************************/
globle void CallNextMethod(
  void *theEnv,
  DATA_OBJECT *result)
  {
   DEFMETHOD *oldMethod;
#if PROFILING_FUNCTIONS
   struct profileFrameInfo profileFrame;
#endif

   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   if (EvaluationData(theEnv)->HaltExecution)
     return;
   oldMethod = DefgenericData(theEnv)->CurrentMethod;
   if (DefgenericData(theEnv)->CurrentMethod != NULL)
     DefgenericData(theEnv)->CurrentMethod = FindApplicableMethod(theEnv,DefgenericData(theEnv)->CurrentGeneric,DefgenericData(theEnv)->CurrentMethod);
   if (DefgenericData(theEnv)->CurrentMethod == NULL)
     {
      DefgenericData(theEnv)->CurrentMethod = oldMethod;
      PrintErrorID(theEnv,"GENRCEXE",2,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Shadowed methods not applicable in current context.\n");
      SetEvaluationError(theEnv,TRUE);
      return;
     }

#if DEBUGGING_FUNCTIONS
   if (DefgenericData(theEnv)->CurrentMethod->trace)
     WatchMethod(theEnv,BEGIN_TRACE);
#endif
   if (DefgenericData(theEnv)->CurrentMethod->system)
     {
      EXPRESSION fcall;

      fcall.type = FCALL;
      fcall.value = DefgenericData(theEnv)->CurrentMethod->actions->value;
      fcall.nextArg = NULL;
      fcall.argList = GetProcParamExpressions(theEnv);
      EvaluateExpression(theEnv,&fcall,result);
     }
   else
     {
#if PROFILING_FUNCTIONS
      StartProfile(theEnv,&profileFrame,
                   &DefgenericData(theEnv)->CurrentGeneric->header.usrData,
                   ProfileFunctionData(theEnv)->ProfileConstructs);
#endif

      EvaluateProcActions(theEnv,DefgenericData(theEnv)->CurrentGeneric->header.whichModule->theModule,
                         DefgenericData(theEnv)->CurrentMethod->actions,DefgenericData(theEnv)->CurrentMethod->localVarCount,
                         result,UnboundMethodErr);

#if PROFILING_FUNCTIONS
      EndProfile(theEnv,&profileFrame);
#endif
     }

   DefgenericData(theEnv)->CurrentMethod->busy--;
#if DEBUGGING_FUNCTIONS
   if (DefgenericData(theEnv)->CurrentMethod->trace)
     WatchMethod(theEnv,END_TRACE);
#endif
   DefgenericData(theEnv)->CurrentMethod = oldMethod;
   ProcedureFunctionData(theEnv)->ReturnFlag = FALSE;
  }

/**************************************************************************
  NAME         : CallSpecificMethod
  DESCRIPTION  : Allows a specific method to be called without regards to
                   higher precedence methods which might also be applicable
                   However, shadowed methods can still be called.
  INPUTS       : A data object buffer to hold the method evaluation result
  RETURNS      : Nothing useful
  SIDE EFFECTS : Side-effects of method applicability tests and the
                 evaluation of methods
  NOTES        : H/L Syntax: (call-specific-method
                                <generic-function> <method-index> <args>)
 **************************************************************************/
globle void CallSpecificMethod(
  void *theEnv,
  DATA_OBJECT *result)
  {
   DATA_OBJECT temp;
   DEFGENERIC *gfunc;
   int mi;

   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   if (EnvArgTypeCheck(theEnv,"call-specific-method",1,SYMBOL,&temp) == FALSE)
     return;
   gfunc = CheckGenericExists(theEnv,"call-specific-method",DOToString(temp));
   if (gfunc == NULL)
     return;
   if (EnvArgTypeCheck(theEnv,"call-specific-method",2,INTEGER,&temp) == FALSE)
     return;
   mi = CheckMethodExists(theEnv,"call-specific-method",gfunc,(long) DOToLong(temp));
   if (mi == -1)
     return;
   gfunc->methods[mi].busy++;
   GenericDispatch(theEnv,gfunc,NULL,&gfunc->methods[mi],
                   GetFirstArgument()->nextArg->nextArg,result);
   gfunc->methods[mi].busy--;
  }

/***********************************************************************
  NAME         : OverrideNextMethod
  DESCRIPTION  : Changes the arguments to shadowed methods, thus the set
                 of applicable methods to this call may change
  INPUTS       : A buffer to hold the result of the call
  RETURNS      : Nothing useful
  SIDE EFFECTS : Any of evaluating method restrictions and bodies
  NOTES        : H/L Syntax: (override-next-method <args>)
 ***********************************************************************/
globle void OverrideNextMethod(
  void *theEnv,
  DATA_OBJECT *result)
  {
   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   if (EvaluationData(theEnv)->HaltExecution)
     return;
   if (DefgenericData(theEnv)->CurrentMethod == NULL)
     {
      PrintErrorID(theEnv,"GENRCEXE",2,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Shadowed methods not applicable in current context.\n");
      SetEvaluationError(theEnv,TRUE);
      return;
     }
   GenericDispatch(theEnv,DefgenericData(theEnv)->CurrentGeneric,DefgenericData(theEnv)->CurrentMethod,NULL,
                   GetFirstArgument(),result);
  }

/***********************************************************
  NAME         : GetGenericCurrentArgument
  DESCRIPTION  : Returns the value of the generic function
                   argument being tested in the method
                   applicability determination process
  INPUTS       : A data-object buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Data-object set
  NOTES        : Useful for queries in wildcard restrictions
 ***********************************************************/
globle void GetGenericCurrentArgument(
  void *theEnv,
  DATA_OBJECT *result)
  {
   result->type = DefgenericData(theEnv)->GenericCurrentArgument->type;
   result->value = DefgenericData(theEnv)->GenericCurrentArgument->value;
   result->begin = DefgenericData(theEnv)->GenericCurrentArgument->begin;
   result->end = DefgenericData(theEnv)->GenericCurrentArgument->end;
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/************************************************************
  NAME         : FindApplicableMethod
  DESCRIPTION  : Finds the first/next applicable
                   method for a generic function call
  INPUTS       : 1) The generic function pointer
                 2) The address of the current method
                    (NULL to find the first)
  RETURNS      : The address of the first/next
                   applicable method (NULL on errors)
  SIDE EFFECTS : Any from evaluating query restrictions
                 Methoid busy count incremented if applicable
  NOTES        : None
 ************************************************************/
static DEFMETHOD *FindApplicableMethod(
  void *theEnv,
  DEFGENERIC *gfunc,
  DEFMETHOD *meth)
  {
   if (meth != NULL)
     meth++;
   else
     meth = gfunc->methods;
   for ( ; meth < &gfunc->methods[gfunc->mcnt] ; meth++)
     {
      meth->busy++;
      if (IsMethodApplicable(theEnv,meth))
        return(meth);
      meth->busy--;
     }
   return(NULL);
  }

#if DEBUGGING_FUNCTIONS

/**********************************************************************
  NAME         : WatchGeneric
  DESCRIPTION  : Prints out a trace of the beginning or end
                   of the execution of a generic function
  INPUTS       : A string to indicate beginning or end of execution
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : Uses the globals CurrentGeneric, ProcParamArraySize and
                   ProcParamArray for other trace info
 **********************************************************************/
static void WatchGeneric(
  void *theEnv,
  char *tstring)
  {
   EnvPrintRouter(theEnv,WTRACE,"GNC ");
   EnvPrintRouter(theEnv,WTRACE,tstring);
   EnvPrintRouter(theEnv,WTRACE," ");
   if (DefgenericData(theEnv)->CurrentGeneric->header.whichModule->theModule != ((struct defmodule *) EnvGetCurrentModule(theEnv)))
     {
      EnvPrintRouter(theEnv,WTRACE,EnvGetDefmoduleName(theEnv,(void *)
                        DefgenericData(theEnv)->CurrentGeneric->header.whichModule->theModule));
      EnvPrintRouter(theEnv,WTRACE,"::");
     }
   EnvPrintRouter(theEnv,WTRACE,ValueToString((void *) DefgenericData(theEnv)->CurrentGeneric->header.name));
   EnvPrintRouter(theEnv,WTRACE," ");
   EnvPrintRouter(theEnv,WTRACE," ED:");
   PrintLongInteger(theEnv,WTRACE,(long long) EvaluationData(theEnv)->CurrentEvaluationDepth);
   PrintProcParamArray(theEnv,WTRACE);
  }

/**********************************************************************
  NAME         : WatchMethod
  DESCRIPTION  : Prints out a trace of the beginning or end
                   of the execution of a generic function
                   method
  INPUTS       : A string to indicate beginning or end of execution
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : Uses the globals CurrentGeneric, CurrentMethod,
                   ProcParamArraySize and ProcParamArray for
                   other trace info
 **********************************************************************/
static void WatchMethod(
  void *theEnv,
  char *tstring)
  {
   EnvPrintRouter(theEnv,WTRACE,"MTH ");
   EnvPrintRouter(theEnv,WTRACE,tstring);
   EnvPrintRouter(theEnv,WTRACE," ");
   if (DefgenericData(theEnv)->CurrentGeneric->header.whichModule->theModule != ((struct defmodule *) EnvGetCurrentModule(theEnv)))
     {
      EnvPrintRouter(theEnv,WTRACE,EnvGetDefmoduleName(theEnv,(void *)
                        DefgenericData(theEnv)->CurrentGeneric->header.whichModule->theModule));
      EnvPrintRouter(theEnv,WTRACE,"::");
     }
   EnvPrintRouter(theEnv,WTRACE,ValueToString((void *) DefgenericData(theEnv)->CurrentGeneric->header.name));
   EnvPrintRouter(theEnv,WTRACE,":#");
   if (DefgenericData(theEnv)->CurrentMethod->system)
     EnvPrintRouter(theEnv,WTRACE,"SYS");
   PrintLongInteger(theEnv,WTRACE,(long long) DefgenericData(theEnv)->CurrentMethod->index);
   EnvPrintRouter(theEnv,WTRACE," ");
   EnvPrintRouter(theEnv,WTRACE," ED:");
   PrintLongInteger(theEnv,WTRACE,(long long) EvaluationData(theEnv)->CurrentEvaluationDepth);
   PrintProcParamArray(theEnv,WTRACE);
  }

#endif

#if OBJECT_SYSTEM

/***************************************************
  NAME         : DetermineRestrictionClass
  DESCRIPTION  : Finds the class of an argument in
                   the ProcParamArray
  INPUTS       : The argument data object
  RETURNS      : The class address, NULL if error
  SIDE EFFECTS : EvaluationError set on errors
  NOTES        : None
 ***************************************************/
static DEFCLASS *DetermineRestrictionClass(
  void *theEnv,
  DATA_OBJECT *dobj)
  {
   INSTANCE_TYPE *ins;
   DEFCLASS *cls;

   if (dobj->type == INSTANCE_NAME)
     {
      ins = FindInstanceBySymbol(theEnv,(SYMBOL_HN *) dobj->value);
      cls = (ins != NULL) ? ins->cls : NULL;
     }
   else if (dobj->type == INSTANCE_ADDRESS)
     {
      ins = (INSTANCE_TYPE *) dobj->value;
      cls = (ins->garbage == 0) ? ins->cls : NULL;
     }
   else
     return(DefclassData(theEnv)->PrimitiveClassMap[dobj->type]);
   if (cls == NULL)
     {
      SetEvaluationError(theEnv,TRUE);
      PrintErrorID(theEnv,"GENRCEXE",3,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Unable to determine class of ");
      PrintDataObject(theEnv,WERROR,dobj);
      EnvPrintRouter(theEnv,WERROR," in generic function ");
      EnvPrintRouter(theEnv,WERROR,EnvGetDefgenericName(theEnv,(void *) DefgenericData(theEnv)->CurrentGeneric));
      EnvPrintRouter(theEnv,WERROR,".\n");
     }
   return(cls);
  }

#endif

#endif

