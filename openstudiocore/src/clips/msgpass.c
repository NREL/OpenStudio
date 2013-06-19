   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  05/17/06          */
   /*                                                     */
   /*              OBJECT MESSAGE DISPATCH CODE           */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Removed IMPERATIVE_MESSAGE_HANDLERS and        */
/*            AUXILIARY_MESSAGE_HANDLERS compilation flags.  */
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

#if OBJECT_SYSTEM

#ifndef _STDIO_INCLUDED_
#include <stdio.h>
#define _STDIO_INCLUDED_
#endif
#include <stdlib.h>

#include "argacces.h"
#include "classcom.h"
#include "classfun.h"
#include "memalloc.h"
#include "constrct.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "insfun.h"
#include "msgcom.h"
#include "msgfun.h"
#include "multifld.h"
#include "prcdrfun.h"
#include "prccode.h"
#include "proflfun.h"
#include "router.h"
#include "strngfun.h"
#include "utility.h"
#include "commline.h"

#define _MSGPASS_SOURCE_
#include "msgpass.h"

#include "inscom.h"

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static void PerformMessage(void *,DATA_OBJECT *,EXPRESSION *,SYMBOL_HN *);
static HANDLER_LINK *FindApplicableHandlers(void *,DEFCLASS *,SYMBOL_HN *);
static void CallHandlers(void *,DATA_OBJECT *);
static void EarlySlotBindError(void *,INSTANCE_TYPE *,DEFCLASS *,unsigned);

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/*****************************************************
  NAME         : DirectMessage
  DESCRIPTION  : Plugs in given instance and
                  performs specified message
  INPUTS       : 1) Message symbolic name
                 2) The instance address
                 3) Address of DATA_OBJECT buffer
                    (NULL if don't care)
                 4) Message argument expressions
  RETURNS      : Nothing useful
  SIDE EFFECTS : Side effects of message execution
  NOTES        : None
 *****************************************************/
globle void DirectMessage(
  void *theEnv,
  SYMBOL_HN *msg,
  INSTANCE_TYPE *ins,
  DATA_OBJECT *resultbuf,
  EXPRESSION *remargs)
  {
   EXPRESSION args;
   DATA_OBJECT temp;

   if (resultbuf == NULL)
     resultbuf = &temp;
   args.nextArg = remargs;
   args.argList = NULL;
   args.type = INSTANCE_ADDRESS;
   args.value = (void *) ins;
   PerformMessage(theEnv,resultbuf,&args,msg);
  }

/***************************************************
  NAME         : EnvSend
  DESCRIPTION  : C Interface for sending messages to
                  instances
  INPUTS       : 1) The data object of the instance
                 2) The message name-string
                 3) The message arguments string
                    (Constants only)
                 4) Caller's buffer for result
  RETURNS      : Nothing useful
  SIDE EFFECTS : Executes message and stores result
                   caller's buffer
  NOTES        : None
 ***************************************************/
globle void EnvSend(
  void *theEnv,
  DATA_OBJECT *idata,
  char *msg,
  char *args,
  DATA_OBJECT *result)
  {
   int error;
   EXPRESSION *iexp;
   SYMBOL_HN *msym;

   if ((EvaluationData(theEnv)->CurrentEvaluationDepth == 0) && (! CommandLineData(theEnv)->EvaluatingTopLevelCommand) &&
       (EvaluationData(theEnv)->CurrentExpression == NULL))
     { PeriodicCleanup(theEnv,TRUE,FALSE); }

   SetEvaluationError(theEnv,FALSE);
   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   msym = FindSymbolHN(theEnv,msg);
   if (msym == NULL)
     {
      PrintNoHandlerError(theEnv,msg);
      SetEvaluationError(theEnv,TRUE);
      return;
     }
   iexp = GenConstant(theEnv,idata->type,idata->value);
   iexp->nextArg = ParseConstantArguments(theEnv,args,&error);
   if (error == TRUE)
     {
      ReturnExpression(theEnv,iexp);
      SetEvaluationError(theEnv,TRUE);
      return;
     }
   PerformMessage(theEnv,result,iexp,msym);
   ReturnExpression(theEnv,iexp);
  }

/*****************************************************
  NAME         : DestroyHandlerLinks
  DESCRIPTION  : Iteratively deallocates handler-links
  INPUTS       : The handler-link list
  RETURNS      : Nothing useful
  SIDE EFFECTS : Deallocation of links
  NOTES        : None
 *****************************************************/
globle void DestroyHandlerLinks(
  void *theEnv,
  HANDLER_LINK *mhead)
  {
   HANDLER_LINK *tmp;

   while (mhead != NULL)
     {
      tmp = mhead;
      mhead = mhead->nxt;
      tmp->hnd->busy--;
      DecrementDefclassBusyCount(theEnv,(void *) tmp->hnd->cls);
      rtn_struct(theEnv,messageHandlerLink,tmp);
     }
  }

/***********************************************************************
  NAME         : SendCommand
  DESCRIPTION  : Determines the applicable handler(s) and sets up the
                   core calling frame.  Then calls the core frame.
  INPUTS       : Caller's space for storing the result of the handler(s)
  RETURNS      : Nothing useful
  SIDE EFFECTS : Any side-effects caused by the execution of handlers in
                   the core framework
  NOTES        : H/L Syntax : (send <instance> <hnd> <args>*)
 ***********************************************************************/
globle void SendCommand(
  void *theEnv,
  DATA_OBJECT *result)
  {
   EXPRESSION args;
   SYMBOL_HN *msg;
   DATA_OBJECT temp;

   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   if (EnvArgTypeCheck(theEnv,"send",2,SYMBOL,&temp) == FALSE)
     return;
   msg = (SYMBOL_HN *) temp.value;

   /* =============================================
      Get the instance or primitive for the message
      ============================================= */
   args.type = GetFirstArgument()->type;
   args.value = GetFirstArgument()->value;
   args.argList = GetFirstArgument()->argList;
   args.nextArg = GetFirstArgument()->nextArg->nextArg;

   PerformMessage(theEnv,result,&args,msg);
  }

/***************************************************
  NAME         : GetNthMessageArgument
  DESCRIPTION  : Returns the address of the nth
                 (starting at 1) which is an
                 argument of the current message
                 dispatch
  INPUTS       : None
  RETURNS      : The message argument
  SIDE EFFECTS : None
  NOTES        : The active instance is always
                 stored as the first argument (0) in
                 the call frame of the message
 ***************************************************/
globle DATA_OBJECT *GetNthMessageArgument(
  void *theEnv,
  int n)
  {
   return(&ProceduralPrimitiveData(theEnv)->ProcParamArray[n]);
  }

/*****************************************************
  NAME         : NextHandlerAvailable
  DESCRIPTION  : Determines if there the currently
                   executing handler can call a
                   shadowed handler
                 Used before calling call-next-handler
  INPUTS       : None
  RETURNS      : TRUE if shadow ready, FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : H/L Syntax: (next-handlerp)
 *****************************************************/
globle int NextHandlerAvailable(
  void *theEnv)
  {
   if (MessageHandlerData(theEnv)->CurrentCore == NULL)
     return(FALSE);
   if (MessageHandlerData(theEnv)->CurrentCore->hnd->type == MAROUND)
     return((MessageHandlerData(theEnv)->NextInCore != NULL) ? TRUE : FALSE);
   if ((MessageHandlerData(theEnv)->CurrentCore->hnd->type == MPRIMARY) && (MessageHandlerData(theEnv)->NextInCore != NULL))
     return((MessageHandlerData(theEnv)->NextInCore->hnd->type == MPRIMARY) ? TRUE : FALSE);
   return(FALSE);
  }

/********************************************************
  NAME         : CallNextHandler
  DESCRIPTION  : This function allows around-handlers
                   to execute the rest of the core frame.
                 It also allows primary handlers
                   to execute shadowed primaries.

                 The original handler arguments are
                   left intact.
  INPUTS       : The caller's result-value buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : The core frame is called and any
                   appropriate changes are made when
                   used in an around handler
                   See CallHandlers()
                 But when call-next-handler is called
                   from a primary, the same shadowed
                   primary is called over and over
                   again for repeated calls to
                   call-next-handler.
  NOTES        : H/L Syntax: (call-next-handler) OR
                    (override-next-handler <arg> ...)
 ********************************************************/
globle void CallNextHandler(
  void *theEnv,
  DATA_OBJECT *result)
  {
   EXPRESSION args;
   int overridep;
   HANDLER_LINK *oldNext,*oldCurrent;
#if PROFILING_FUNCTIONS
   struct profileFrameInfo profileFrame;
#endif

   SetpType(result,SYMBOL);
   SetpValue(result,EnvFalseSymbol(theEnv));
   EvaluationData(theEnv)->EvaluationError = FALSE;
   if (EvaluationData(theEnv)->HaltExecution)
     return;
   if (NextHandlerAvailable(theEnv) == FALSE)
     {
      PrintErrorID(theEnv,"MSGPASS",1,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Shadowed message-handlers not applicable in current context.\n");
      SetEvaluationError(theEnv,TRUE);
      return;
     }
   if (EvaluationData(theEnv)->CurrentExpression->value == (void *) FindFunction(theEnv,"override-next-handler"))
     {
      overridep = 1;
      args.type = ProceduralPrimitiveData(theEnv)->ProcParamArray[0].type;
      if (args.type != MULTIFIELD)
        args.value = (void *) ProceduralPrimitiveData(theEnv)->ProcParamArray[0].value;
      else
        args.value = (void *) &ProceduralPrimitiveData(theEnv)->ProcParamArray[0];
      args.nextArg = GetFirstArgument();
      args.argList = NULL;
      PushProcParameters(theEnv,&args,CountArguments(&args),
                          ValueToString(MessageHandlerData(theEnv)->CurrentMessageName),"message",
                          UnboundHandlerErr);
      if (EvaluationData(theEnv)->EvaluationError)
        {
         ProcedureFunctionData(theEnv)->ReturnFlag = FALSE;
         return;
        }
     }
   else
     overridep = 0;
   oldNext = MessageHandlerData(theEnv)->NextInCore;
   oldCurrent = MessageHandlerData(theEnv)->CurrentCore;
   if (MessageHandlerData(theEnv)->CurrentCore->hnd->type == MAROUND)
     {
      if (MessageHandlerData(theEnv)->NextInCore->hnd->type == MAROUND)
        {
         MessageHandlerData(theEnv)->CurrentCore = MessageHandlerData(theEnv)->NextInCore;
         MessageHandlerData(theEnv)->NextInCore = MessageHandlerData(theEnv)->NextInCore->nxt;
#if DEBUGGING_FUNCTIONS
         if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
           WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,BEGIN_TRACE);
#endif
         if (CheckHandlerArgCount(theEnv))
           {
#if PROFILING_FUNCTIONS
            StartProfile(theEnv,&profileFrame,
                         &MessageHandlerData(theEnv)->CurrentCore->hnd->usrData,
                         ProfileFunctionData(theEnv)->ProfileConstructs);
#endif

            EvaluateProcActions(theEnv,MessageHandlerData(theEnv)->CurrentCore->hnd->cls->header.whichModule->theModule,
                               MessageHandlerData(theEnv)->CurrentCore->hnd->actions,
                               MessageHandlerData(theEnv)->CurrentCore->hnd->localVarCount,
                               result,UnboundHandlerErr);
#if PROFILING_FUNCTIONS
            EndProfile(theEnv,&profileFrame);
#endif
           }
#if DEBUGGING_FUNCTIONS
         if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
           WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,END_TRACE);
#endif
        }
      else
        CallHandlers(theEnv,result);
     }
   else
     {
      MessageHandlerData(theEnv)->CurrentCore = MessageHandlerData(theEnv)->NextInCore;
      MessageHandlerData(theEnv)->NextInCore = MessageHandlerData(theEnv)->NextInCore->nxt;
#if DEBUGGING_FUNCTIONS
      if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
        WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,BEGIN_TRACE);
#endif
      if (CheckHandlerArgCount(theEnv))
        {
#if PROFILING_FUNCTIONS
        StartProfile(theEnv,&profileFrame,
                     &MessageHandlerData(theEnv)->CurrentCore->hnd->usrData,
                     ProfileFunctionData(theEnv)->ProfileConstructs);
#endif

        EvaluateProcActions(theEnv,MessageHandlerData(theEnv)->CurrentCore->hnd->cls->header.whichModule->theModule,
                            MessageHandlerData(theEnv)->CurrentCore->hnd->actions,
                            MessageHandlerData(theEnv)->CurrentCore->hnd->localVarCount,
                            result,UnboundHandlerErr);
#if PROFILING_FUNCTIONS
         EndProfile(theEnv,&profileFrame);
#endif
        }

#if DEBUGGING_FUNCTIONS
      if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
        WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,END_TRACE);
#endif
     }
   MessageHandlerData(theEnv)->NextInCore = oldNext;
   MessageHandlerData(theEnv)->CurrentCore = oldCurrent;
   if (overridep)
     PopProcParameters(theEnv);
   ProcedureFunctionData(theEnv)->ReturnFlag = FALSE;
  }

/*************************************************************************
  NAME         : FindApplicableOfName
  DESCRIPTION  : Groups all handlers of all types of the specified
                   class of the specified name into the applicable handler
                   list
  INPUTS       : 1) The class address
                 2-3) The tops and bottoms of the four handler type lists:
                      around, before, primary and after
                 4) The message name symbol
  RETURNS      : Nothing useful
  SIDE EFFECTS : Modifies the handler lists to include applicable handlers
  NOTES        : None
 *************************************************************************/
globle void FindApplicableOfName(
  void *theEnv,
  DEFCLASS *cls,
  HANDLER_LINK *tops[4],
  HANDLER_LINK *bots[4],
  SYMBOL_HN *mname)
  {
   register int i;
   register int e;
   HANDLER *hnd;
   unsigned *arr;
   HANDLER_LINK *tmp;

   i = FindHandlerNameGroup(cls,mname);
   if (i == -1)
     return;
   e = ((int) cls->handlerCount) - 1;
   hnd = cls->handlers;
   arr = cls->handlerOrderMap;
   for ( ; i <= e ; i++)
     {
      if (hnd[arr[i]].name != mname)
        break;

      tmp = get_struct(theEnv,messageHandlerLink);
      hnd[arr[i]].busy++;
      IncrementDefclassBusyCount(theEnv,(void *) hnd[arr[i]].cls);
      tmp->hnd = &hnd[arr[i]];
      if (tops[tmp->hnd->type] == NULL)
        {
         tmp->nxt = NULL;
         tops[tmp->hnd->type] = bots[tmp->hnd->type] = tmp;
        }

      else if (tmp->hnd->type == MAFTER)
        {
         tmp->nxt = tops[tmp->hnd->type];
         tops[tmp->hnd->type] = tmp;
        }

      else
        {
         bots[tmp->hnd->type]->nxt = tmp;
         bots[tmp->hnd->type] = tmp;
         tmp->nxt = NULL;
        }
     }
  }

/*************************************************************************
  NAME         : JoinHandlerLinks
  DESCRIPTION  : Joins the queues of different handlers together
  INPUTS       : 1-2) The tops and bottoms of the four handler type lists:
                      around, before, primary and after
                 3) The message name symbol
  RETURNS      : The top of the joined lists, NULL on errors
  SIDE EFFECTS : Links all the handler type lists together, or all the
                   lists are destroyed if there are no primary handlers
  NOTES        : None
 *************************************************************************/
globle HANDLER_LINK *JoinHandlerLinks(
  void *theEnv,
  HANDLER_LINK *tops[4],
  HANDLER_LINK *bots[4],
  SYMBOL_HN *mname)
  {
   register int i;
   HANDLER_LINK *mlink;

   if (tops[MPRIMARY] == NULL)
    {
     PrintNoHandlerError(theEnv,ValueToString(mname));
     for (i = MAROUND ; i <= MAFTER ; i++)
       DestroyHandlerLinks(theEnv,tops[i]);
     SetEvaluationError(theEnv,TRUE);
     return(NULL);
    }

   mlink = tops[MPRIMARY];

   if (tops[MBEFORE] != NULL)
     {
      bots[MBEFORE]->nxt = mlink;
      mlink = tops[MBEFORE];
     }

   if (tops[MAROUND] != NULL)
     {
      bots[MAROUND]->nxt = mlink;
      mlink = tops[MAROUND];
     }

   bots[MPRIMARY]->nxt = tops[MAFTER];

   return(mlink);
  }

/***************************************************
  NAME         : PrintHandlerSlotGetFunction
  DESCRIPTION  : Developer access function for
                 printing direct slot references
                 in message-handlers
  INPUTS       : 1) The logical name of the output
                 2) The bitmap expression
  RETURNS      : Nothing useful
  SIDE EFFECTS : Expression printed
  NOTES        : None
 ***************************************************/
#if WIN_BTC && (! DEVELOPER)
#pragma argsused
#endif
globle void PrintHandlerSlotGetFunction(
  void *theEnv,
  char *logicalName,
  void *theValue)
  {
#if DEVELOPER
   HANDLER_SLOT_REFERENCE *theReference;
   DEFCLASS *theDefclass;
   SLOT_DESC *sd;

   theReference = (HANDLER_SLOT_REFERENCE *) ValueToBitMap(theValue);
   EnvPrintRouter(theEnv,logicalName,"?self:[");
   theDefclass = DefclassData(theEnv)->ClassIDMap[theReference->classID];
   EnvPrintRouter(theEnv,logicalName,ValueToString(theDefclass->header.name));
   EnvPrintRouter(theEnv,logicalName,"]");
   sd = theDefclass->instanceTemplate[theDefclass->slotNameMap[theReference->slotID] - 1];
   EnvPrintRouter(theEnv,logicalName,ValueToString(sd->slotName->name));
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#pragma unused(logicalName)
#pragma unused(theValue)
#endif
#endif
  }

/***************************************************
  NAME         : HandlerSlotGetFunction
  DESCRIPTION  : Access function for handling the
                 statically-bound direct slot
                 references in message-handlers
  INPUTS       : 1) The bitmap expression
                 2) A data object buffer
  RETURNS      : TRUE if OK, FALSE
                 on errors
  SIDE EFFECTS : Data object buffer gets value of
                 slot. On errors, buffer gets
                 symbol FALSE, EvaluationError
                 is set and error messages are
                 printed
  NOTES        : It is possible for a handler
                 (attached to a superclass of
                  the currently active instance)
                 containing these static references
                 to be called for an instance
                 which does not contain the slots
                 (e.g., an instance of a subclass
                  where the original slot was
                  no-inherit or the subclass
                  overrode the original slot)
 ***************************************************/
globle intBool HandlerSlotGetFunction(
  void *theEnv,
  void *theValue,
  DATA_OBJECT *theResult)
  {
   HANDLER_SLOT_REFERENCE *theReference;
   DEFCLASS *theDefclass;
   INSTANCE_TYPE *theInstance;
   INSTANCE_SLOT *sp;
   unsigned instanceSlotIndex;

   theReference = (HANDLER_SLOT_REFERENCE *) ValueToBitMap(theValue);
   theInstance = (INSTANCE_TYPE *) ProceduralPrimitiveData(theEnv)->ProcParamArray[0].value;
   theDefclass = DefclassData(theEnv)->ClassIDMap[theReference->classID];

   if (theInstance->garbage)
     {
      StaleInstanceAddress(theEnv,"for slot get",0);
      theResult->type = SYMBOL;
      theResult->value = EnvFalseSymbol(theEnv);
      SetEvaluationError(theEnv,TRUE);
      return(FALSE);
     }

   if (theInstance->cls == theDefclass)
     {
      instanceSlotIndex = theInstance->cls->slotNameMap[theReference->slotID];
      sp = theInstance->slotAddresses[instanceSlotIndex - 1];
     }
   else
     {
      if (theReference->slotID > theInstance->cls->maxSlotNameID)
        goto HandlerGetError;
      instanceSlotIndex = theInstance->cls->slotNameMap[theReference->slotID];
      if (instanceSlotIndex == 0)
        goto HandlerGetError;
      instanceSlotIndex--;
      sp = theInstance->slotAddresses[instanceSlotIndex];
      if (sp->desc->cls != theDefclass)
        goto HandlerGetError;
     }
   theResult->type = (unsigned short) sp->type;
   theResult->value = sp->value;
   if (sp->type == MULTIFIELD)
     {
      theResult->begin = 0;
      SetpDOEnd(theResult,GetInstanceSlotLength(sp));
     }
   return(TRUE);

HandlerGetError:
   EarlySlotBindError(theEnv,theInstance,theDefclass,theReference->slotID);
   theResult->type = SYMBOL;
   theResult->value = EnvFalseSymbol(theEnv);
   SetEvaluationError(theEnv,TRUE);
   return(FALSE);
  }

/***************************************************
  NAME         : PrintHandlerSlotPutFunction
  DESCRIPTION  : Developer access function for
                 printing direct slot bindings
                 in message-handlers
  INPUTS       : 1) The logical name of the output
                 2) The bitmap expression
  RETURNS      : Nothing useful
  SIDE EFFECTS : Expression printed
  NOTES        : None
 ***************************************************/
#if WIN_BTC && (! DEVELOPER)
#pragma argsused
#endif
globle void PrintHandlerSlotPutFunction(
  void *theEnv,
  char *logicalName,
  void *theValue)
  {
#if DEVELOPER
   HANDLER_SLOT_REFERENCE *theReference;
   DEFCLASS *theDefclass;
   SLOT_DESC *sd;

   theReference = (HANDLER_SLOT_REFERENCE *) ValueToBitMap(theValue);
   EnvPrintRouter(theEnv,logicalName,"(bind ?self:[");
   theDefclass = DefclassData(theEnv)->ClassIDMap[theReference->classID];
   EnvPrintRouter(theEnv,logicalName,ValueToString(theDefclass->header.name));
   EnvPrintRouter(theEnv,logicalName,"]");
   sd = theDefclass->instanceTemplate[theDefclass->slotNameMap[theReference->slotID] - 1];
   EnvPrintRouter(theEnv,logicalName,ValueToString(sd->slotName->name));
   if (GetFirstArgument() != NULL)
     {
      EnvPrintRouter(theEnv,logicalName," ");
      PrintExpression(theEnv,logicalName,GetFirstArgument());
     }
   EnvPrintRouter(theEnv,logicalName,")");
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#pragma unused(logicalName)
#pragma unused(theValue)
#endif
#endif
  }

/***************************************************
  NAME         : HandlerSlotPutFunction
  DESCRIPTION  : Access function for handling the
                 statically-bound direct slot
                 bindings in message-handlers
  INPUTS       : 1) The bitmap expression
                 2) A data object buffer
  RETURNS      : TRUE if OK, FALSE
                 on errors
  SIDE EFFECTS : Data object buffer gets symbol
                 TRUE and slot is set. On errors,
                 buffer gets symbol FALSE,
                 EvaluationError is set and error
                 messages are printed
  NOTES        : It is possible for a handler
                 (attached to a superclass of
                  the currently active instance)
                 containing these static references
                 to be called for an instance
                 which does not contain the slots
                 (e.g., an instance of a subclass
                  where the original slot was
                  no-inherit or the subclass
                  overrode the original slot)
 ***************************************************/
globle intBool HandlerSlotPutFunction(
  void *theEnv,
  void *theValue,
  DATA_OBJECT *theResult)
  {
   HANDLER_SLOT_REFERENCE *theReference;
   DEFCLASS *theDefclass;
   INSTANCE_TYPE *theInstance;
   INSTANCE_SLOT *sp;
   unsigned instanceSlotIndex;
   DATA_OBJECT theSetVal;

   theReference = (HANDLER_SLOT_REFERENCE *) ValueToBitMap(theValue);
   theInstance = (INSTANCE_TYPE *) ProceduralPrimitiveData(theEnv)->ProcParamArray[0].value;
   theDefclass = DefclassData(theEnv)->ClassIDMap[theReference->classID];

   if (theInstance->garbage)
     {
      StaleInstanceAddress(theEnv,"for slot put",0);
      theResult->type = SYMBOL;
      theResult->value = EnvFalseSymbol(theEnv);
      SetEvaluationError(theEnv,TRUE);
      return(FALSE);
     }

   if (theInstance->cls == theDefclass)
     {
      instanceSlotIndex = theInstance->cls->slotNameMap[theReference->slotID];
      sp = theInstance->slotAddresses[instanceSlotIndex - 1];
     }
   else
     {
      if (theReference->slotID > theInstance->cls->maxSlotNameID)
        goto HandlerPutError;
      instanceSlotIndex = theInstance->cls->slotNameMap[theReference->slotID];
      if (instanceSlotIndex == 0)
        goto HandlerPutError;
      instanceSlotIndex--;
      sp = theInstance->slotAddresses[instanceSlotIndex];
      if (sp->desc->cls != theDefclass)
        goto HandlerPutError;
     }

   /* =======================================================
      The slot has already been verified not to be read-only.
      However, if it is initialize-only, we need to make sure
      that we are initializing the instance (something we
      could not verify at parse-time)
      ======================================================= */
   if (sp->desc->initializeOnly && (!theInstance->initializeInProgress))
     {
      SlotAccessViolationError(theEnv,ValueToString(sp->desc->slotName->name),
                               TRUE,(void *) theInstance);
      goto HandlerPutError2;
     }

   /* ======================================
      No arguments means to use the
      special NoParamValue to reset the slot
      to its default value
      ====================================== */
   if (GetFirstArgument())
     {
      if (EvaluateAndStoreInDataObject(theEnv,(int) sp->desc->multiple,
                                       GetFirstArgument(),&theSetVal,TRUE) == FALSE)
         goto HandlerPutError2;
     }
   else
     {
      SetDOBegin(theSetVal,1);
      SetDOEnd(theSetVal,0);
      SetType(theSetVal,MULTIFIELD);
      SetValue(theSetVal,ProceduralPrimitiveData(theEnv)->NoParamValue);
     }
   if (PutSlotValue(theEnv,theInstance,sp,&theSetVal,theResult,NULL) == FALSE)
      goto HandlerPutError2;
   return(TRUE);

HandlerPutError:
   EarlySlotBindError(theEnv,theInstance,theDefclass,theReference->slotID);

HandlerPutError2:
   theResult->type = SYMBOL;
   theResult->value = EnvFalseSymbol(theEnv);
   SetEvaluationError(theEnv,TRUE);

   return(FALSE);
  }

/*****************************************************
  NAME         : DynamicHandlerGetSlot
  DESCRIPTION  : Directly references a slot's value
                 (uses dynamic binding to lookup slot)
  INPUTS       : The caller's result buffer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Caller's result buffer set
  NOTES        : H/L Syntax: (get <slot>)
 *****************************************************/
globle void DynamicHandlerGetSlot(
  void *theEnv,
  DATA_OBJECT *result)
  {
   INSTANCE_SLOT *sp;
   INSTANCE_TYPE *ins;
   DATA_OBJECT temp;

   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   if (CheckCurrentMessage(theEnv,"dynamic-get",TRUE) == FALSE)
     return;
   EvaluateExpression(theEnv,GetFirstArgument(),&temp);
   if (temp.type != SYMBOL)
     {
      ExpectedTypeError1(theEnv,"dynamic-get",1,"symbol");
      SetEvaluationError(theEnv,TRUE);
      return;
     }
   ins = GetActiveInstance(theEnv);
   sp = FindInstanceSlot(theEnv,ins,(SYMBOL_HN *) temp.value);
   if (sp == NULL)
     {
      SlotExistError(theEnv,ValueToString(temp.value),"dynamic-get");
      return;
     }
   if ((sp->desc->publicVisibility == 0) &&
       (MessageHandlerData(theEnv)->CurrentCore->hnd->cls != sp->desc->cls))
     {
      SlotVisibilityViolationError(theEnv,sp->desc,MessageHandlerData(theEnv)->CurrentCore->hnd->cls);
      SetEvaluationError(theEnv,TRUE);
      return;
     }
   result->type = (unsigned short) sp->type;
   result->value = sp->value;
   if (sp->type == MULTIFIELD)
     {
      result->begin = 0;
      SetpDOEnd(result,GetInstanceSlotLength(sp));
     }
  }

/***********************************************************
  NAME         : DynamicHandlerPutSlot
  DESCRIPTION  : Directly puts a slot's value
                 (uses dynamic binding to lookup slot)
  INPUTS       : Data obejct buffer for holding slot value
  RETURNS      : Nothing useful
  SIDE EFFECTS : Slot modified - and caller's buffer set
                 to value (or symbol FALSE on errors)
  NOTES        : H/L Syntax: (put <slot> <value>*)
 ***********************************************************/
globle void DynamicHandlerPutSlot(
  void *theEnv,
  DATA_OBJECT *theResult)
  {
   INSTANCE_SLOT *sp;
   INSTANCE_TYPE *ins;
   DATA_OBJECT temp;

   theResult->type = SYMBOL;
   theResult->value = EnvFalseSymbol(theEnv);
   if (CheckCurrentMessage(theEnv,"dynamic-put",TRUE) == FALSE)
     return;
   EvaluateExpression(theEnv,GetFirstArgument(),&temp);
   if (temp.type != SYMBOL)
     {
      ExpectedTypeError1(theEnv,"dynamic-put",1,"symbol");
      SetEvaluationError(theEnv,TRUE);
      return;
     }
   ins = GetActiveInstance(theEnv);
   sp = FindInstanceSlot(theEnv,ins,(SYMBOL_HN *) temp.value);
   if (sp == NULL)
     {
      SlotExistError(theEnv,ValueToString(temp.value),"dynamic-put");
      return;
     }
   if ((sp->desc->noWrite == 0) ? FALSE :
       ((sp->desc->initializeOnly == 0) || (!ins->initializeInProgress)))
     {
      SlotAccessViolationError(theEnv,ValueToString(sp->desc->slotName->name),
                               TRUE,(void *) ins);
      SetEvaluationError(theEnv,TRUE);
      return;
     }
   if ((sp->desc->publicVisibility == 0) &&
       (MessageHandlerData(theEnv)->CurrentCore->hnd->cls != sp->desc->cls))
     {
      SlotVisibilityViolationError(theEnv,sp->desc,MessageHandlerData(theEnv)->CurrentCore->hnd->cls);
      SetEvaluationError(theEnv,TRUE);
      return;
     }
   if (GetFirstArgument()->nextArg)
     {
      if (EvaluateAndStoreInDataObject(theEnv,(int) sp->desc->multiple,
                        GetFirstArgument()->nextArg,&temp,TRUE) == FALSE)
        return;
     }
   else
     {
      SetpDOBegin(&temp,1);
      SetpDOEnd(&temp,0);
      SetpType(&temp,MULTIFIELD);
      SetpValue(&temp,ProceduralPrimitiveData(theEnv)->NoParamValue);
     }
   PutSlotValue(theEnv,ins,sp,&temp,theResult,NULL);
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/*****************************************************
  NAME         : PerformMessage
  DESCRIPTION  : Calls core framework for a message
  INPUTS       : 1) Caller's result buffer
                 2) Message argument expressions
                    (including implicit object)
                 3) Message name
  RETURNS      : Nothing useful
  SIDE EFFECTS : Any side-effects of message execution
                    and caller's result buffer set
  NOTES        : None
 *****************************************************/
static void PerformMessage(
  void *theEnv,
  DATA_OBJECT *result,
  EXPRESSION *args,
  SYMBOL_HN *mname)
  {
   int oldce;
   /* HANDLER_LINK *oldCore; */
   DEFCLASS *cls = NULL;
   INSTANCE_TYPE *ins = NULL;
   SYMBOL_HN *oldName;
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
   oldName = MessageHandlerData(theEnv)->CurrentMessageName;
   MessageHandlerData(theEnv)->CurrentMessageName = mname;
   EvaluationData(theEnv)->CurrentEvaluationDepth++;

   PushProcParameters(theEnv,args,CountArguments(args),
                        ValueToString(MessageHandlerData(theEnv)->CurrentMessageName),"message",
                        UnboundHandlerErr);


   if (EvaluationData(theEnv)->EvaluationError)
     {
      EvaluationData(theEnv)->CurrentEvaluationDepth--;
      MessageHandlerData(theEnv)->CurrentMessageName = oldName;
      PeriodicCleanup(theEnv,FALSE,TRUE);
      SetExecutingConstruct(theEnv,oldce);
      return;
     }

   if (ProceduralPrimitiveData(theEnv)->ProcParamArray->type == INSTANCE_ADDRESS)
     {
      ins = (INSTANCE_TYPE *) ProceduralPrimitiveData(theEnv)->ProcParamArray->value;
      if (ins->garbage == 1)
        {
         StaleInstanceAddress(theEnv,"send",0);
         SetEvaluationError(theEnv,TRUE);
        }
      else if (DefclassInScope(theEnv,ins->cls,(struct defmodule *) EnvGetCurrentModule(theEnv)) == FALSE)
        NoInstanceError(theEnv,ValueToString(ins->name),"send");
      else
        {
         cls = ins->cls;
         ins->busy++;
        }
     }
   else if (ProceduralPrimitiveData(theEnv)->ProcParamArray->type == INSTANCE_NAME)
     {
      ins = FindInstanceBySymbol(theEnv,(SYMBOL_HN *) ProceduralPrimitiveData(theEnv)->ProcParamArray->value);
      if (ins == NULL)
        {
         PrintErrorID(theEnv,"MSGPASS",2,FALSE);
         EnvPrintRouter(theEnv,WERROR,"No such instance ");
         EnvPrintRouter(theEnv,WERROR,ValueToString((SYMBOL_HN *) ProceduralPrimitiveData(theEnv)->ProcParamArray->value));
         EnvPrintRouter(theEnv,WERROR," in function send.\n");
         SetEvaluationError(theEnv,TRUE);
        }
      else
        {
         ProceduralPrimitiveData(theEnv)->ProcParamArray->value = (void *) ins;
         ProceduralPrimitiveData(theEnv)->ProcParamArray->type = INSTANCE_ADDRESS;
         cls = ins->cls;
         ins->busy++;
        }
     }
   else if ((cls = DefclassData(theEnv)->PrimitiveClassMap[ProceduralPrimitiveData(theEnv)->ProcParamArray->type]) == NULL)
     {
      SystemError(theEnv,"MSGPASS",1);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }
   if (EvaluationData(theEnv)->EvaluationError)
     {
      PopProcParameters(theEnv);
      EvaluationData(theEnv)->CurrentEvaluationDepth--;
      MessageHandlerData(theEnv)->CurrentMessageName = oldName;
      PeriodicCleanup(theEnv,FALSE,TRUE);
      SetExecutingConstruct(theEnv,oldce);
      return;
     }

   /* oldCore = MessageHandlerData(theEnv)->TopOfCore; */

   if (MessageHandlerData(theEnv)->TopOfCore != NULL)
     { MessageHandlerData(theEnv)->TopOfCore->nxtInStack = MessageHandlerData(theEnv)->OldCore; }
   MessageHandlerData(theEnv)->OldCore = MessageHandlerData(theEnv)->TopOfCore;
   
   MessageHandlerData(theEnv)->TopOfCore = FindApplicableHandlers(theEnv,cls,mname);

   if (MessageHandlerData(theEnv)->TopOfCore != NULL)
     {
      HANDLER_LINK *oldCurrent,*oldNext;

      oldCurrent = MessageHandlerData(theEnv)->CurrentCore;
      oldNext = MessageHandlerData(theEnv)->NextInCore;

      if (MessageHandlerData(theEnv)->TopOfCore->hnd->type == MAROUND)
        {
         MessageHandlerData(theEnv)->CurrentCore = MessageHandlerData(theEnv)->TopOfCore;
         MessageHandlerData(theEnv)->NextInCore = MessageHandlerData(theEnv)->TopOfCore->nxt;
#if DEBUGGING_FUNCTIONS
         if (MessageHandlerData(theEnv)->WatchMessages)
           WatchMessage(theEnv,WTRACE,BEGIN_TRACE);
         if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
           WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,BEGIN_TRACE);
#endif
         if (CheckHandlerArgCount(theEnv))
           {
#if PROFILING_FUNCTIONS
            StartProfile(theEnv,&profileFrame,
                         &MessageHandlerData(theEnv)->CurrentCore->hnd->usrData,
                         ProfileFunctionData(theEnv)->ProfileConstructs);
#endif


           EvaluateProcActions(theEnv,MessageHandlerData(theEnv)->CurrentCore->hnd->cls->header.whichModule->theModule,
                               MessageHandlerData(theEnv)->CurrentCore->hnd->actions,
                               MessageHandlerData(theEnv)->CurrentCore->hnd->localVarCount,
                               result,UnboundHandlerErr);


#if PROFILING_FUNCTIONS
            EndProfile(theEnv,&profileFrame);
#endif
           }

#if DEBUGGING_FUNCTIONS
         if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
           WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,END_TRACE);
         if (MessageHandlerData(theEnv)->WatchMessages)
           WatchMessage(theEnv,WTRACE,END_TRACE);
#endif
        }
      else
        {
         MessageHandlerData(theEnv)->CurrentCore = NULL;
         MessageHandlerData(theEnv)->NextInCore = MessageHandlerData(theEnv)->TopOfCore;
#if DEBUGGING_FUNCTIONS
         if (MessageHandlerData(theEnv)->WatchMessages)
           WatchMessage(theEnv,WTRACE,BEGIN_TRACE);
#endif
         CallHandlers(theEnv,result);
#if DEBUGGING_FUNCTIONS
         if (MessageHandlerData(theEnv)->WatchMessages)
           WatchMessage(theEnv,WTRACE,END_TRACE);
#endif
        }

      DestroyHandlerLinks(theEnv,MessageHandlerData(theEnv)->TopOfCore);
      MessageHandlerData(theEnv)->CurrentCore = oldCurrent;
      MessageHandlerData(theEnv)->NextInCore = oldNext;
     }

   /* MessageHandlerData(theEnv)->TopOfCore = oldCore; */
   MessageHandlerData(theEnv)->TopOfCore = MessageHandlerData(theEnv)->OldCore;
   if (MessageHandlerData(theEnv)->OldCore != NULL)
     { MessageHandlerData(theEnv)->OldCore = MessageHandlerData(theEnv)->OldCore->nxtInStack; }

   ProcedureFunctionData(theEnv)->ReturnFlag = FALSE;

   if (ins != NULL)
     ins->busy--;

   /* ==================================
      Restore the original calling frame
      ================================== */
   PopProcParameters(theEnv);
   EvaluationData(theEnv)->CurrentEvaluationDepth--;
   MessageHandlerData(theEnv)->CurrentMessageName = oldName;
   PropagateReturnValue(theEnv,result);
   PeriodicCleanup(theEnv,FALSE,TRUE);
   SetExecutingConstruct(theEnv,oldce);

   if (EvaluationData(theEnv)->EvaluationError)
     {
      result->type = SYMBOL;
      result->value = EnvFalseSymbol(theEnv);
     }
  }

/*****************************************************************************
  NAME         : FindApplicableHandlers
  DESCRIPTION  : Given a message name, this routine forms the "core frame"
                   for the message : a list of all applicable class handlers.
                   An applicable class handler is one whose name matches
                     the message and whose class matches the instance.

                   The list is in the following order :

                   All around handlers (from most specific to most general)
                   All before handlers (from most specific to most general)
                   All primary handlers (from most specific to most general)
                   All after handlers (from most general to most specific)

  INPUTS       : 1) The class of the instance (or primitive) for the message
                 2) The message name
  RETURNS      : NULL if no applicable handlers or errors,
                   the list of handlers otherwise
  SIDE EFFECTS : Links are allocated for the list
  NOTES        : The instance is the first thing on the ProcParamArray
                 The number of arguments is in ProcParamArraySize
 *****************************************************************************/
static HANDLER_LINK *FindApplicableHandlers(
  void *theEnv,
  DEFCLASS *cls,
  SYMBOL_HN *mname)
  {
   register int i;
   HANDLER_LINK *tops[4],*bots[4];

   for (i = MAROUND ; i <= MAFTER ; i++)
     tops[i] = bots[i] = NULL;

   for (i = 0 ; i < cls->allSuperclasses.classCount ; i++)
     FindApplicableOfName(theEnv,cls->allSuperclasses.classArray[i],tops,bots,mname);
   return(JoinHandlerLinks(theEnv,tops,bots,mname));
  }

/***************************************************************
  NAME         : CallHandlers
  DESCRIPTION  : Moves though the current message frame
                   for a send-message as follows :

                 Call all before handlers and ignore their
                   return values.
                 Call the first primary handler and
                   ignore the rest.  The return value
                   of the handler frame is this message's value.
                 Call all after handlers and ignore their
                   return values.
  INPUTS       : Caller's buffer for the return value of
                   the message
  RETURNS      : Nothing useful
  SIDE EFFECTS : The handlers are evaluated.
  NOTES        : IMPORTANT : The global NextInCore should be
                 pointing to the first handler to be executed.
 ***************************************************************/
static void CallHandlers(
  void *theEnv,
  DATA_OBJECT *result)
  {
#if WIN_BTC
   HANDLER_LINK *oldCurrent,*oldNext;                /* prevents warning */
#else
   HANDLER_LINK *oldCurrent = NULL,*oldNext = NULL;  /* prevents warning */
#endif
   DATA_OBJECT temp;
#if PROFILING_FUNCTIONS
   struct profileFrameInfo profileFrame;
#endif

   if (EvaluationData(theEnv)->HaltExecution)
     return;

   oldCurrent = MessageHandlerData(theEnv)->CurrentCore;
   oldNext = MessageHandlerData(theEnv)->NextInCore;

   while (MessageHandlerData(theEnv)->NextInCore->hnd->type == MBEFORE)
     {
      MessageHandlerData(theEnv)->CurrentCore = MessageHandlerData(theEnv)->NextInCore;
      MessageHandlerData(theEnv)->NextInCore = MessageHandlerData(theEnv)->NextInCore->nxt;
#if DEBUGGING_FUNCTIONS
      if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
        WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,BEGIN_TRACE);
#endif
      if (CheckHandlerArgCount(theEnv))
        {
#if PROFILING_FUNCTIONS
         StartProfile(theEnv,&profileFrame,
                      &MessageHandlerData(theEnv)->CurrentCore->hnd->usrData,
                      ProfileFunctionData(theEnv)->ProfileConstructs);
#endif

         EvaluateProcActions(theEnv,MessageHandlerData(theEnv)->CurrentCore->hnd->cls->header.whichModule->theModule,
                            MessageHandlerData(theEnv)->CurrentCore->hnd->actions,
                            MessageHandlerData(theEnv)->CurrentCore->hnd->localVarCount,
                            &temp,UnboundHandlerErr);


#if PROFILING_FUNCTIONS
         EndProfile(theEnv,&profileFrame);
#endif
        }

#if DEBUGGING_FUNCTIONS
      if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
        WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,END_TRACE);
#endif
      ProcedureFunctionData(theEnv)->ReturnFlag = FALSE;
      if ((MessageHandlerData(theEnv)->NextInCore == NULL) || EvaluationData(theEnv)->HaltExecution)
        {
         MessageHandlerData(theEnv)->NextInCore = oldNext;
         MessageHandlerData(theEnv)->CurrentCore = oldCurrent;
         return;
        }
     }
   if (MessageHandlerData(theEnv)->NextInCore->hnd->type == MPRIMARY)
     {
      MessageHandlerData(theEnv)->CurrentCore = MessageHandlerData(theEnv)->NextInCore;
      MessageHandlerData(theEnv)->NextInCore = MessageHandlerData(theEnv)->NextInCore->nxt;
#if DEBUGGING_FUNCTIONS
      if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
        WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,BEGIN_TRACE);
#endif
      if (CheckHandlerArgCount(theEnv))
        {
#if PROFILING_FUNCTIONS
         StartProfile(theEnv,&profileFrame,
                      &MessageHandlerData(theEnv)->CurrentCore->hnd->usrData,
                      ProfileFunctionData(theEnv)->ProfileConstructs);
#endif


        EvaluateProcActions(theEnv,MessageHandlerData(theEnv)->CurrentCore->hnd->cls->header.whichModule->theModule,
                            MessageHandlerData(theEnv)->CurrentCore->hnd->actions,
                            MessageHandlerData(theEnv)->CurrentCore->hnd->localVarCount,
                            result,UnboundHandlerErr);

#if PROFILING_FUNCTIONS
         EndProfile(theEnv,&profileFrame);
#endif
        }


#if DEBUGGING_FUNCTIONS
      if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
        WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,END_TRACE);
#endif
      ProcedureFunctionData(theEnv)->ReturnFlag = FALSE;

      if ((MessageHandlerData(theEnv)->NextInCore == NULL) || EvaluationData(theEnv)->HaltExecution)
        {
         MessageHandlerData(theEnv)->NextInCore = oldNext;
         MessageHandlerData(theEnv)->CurrentCore = oldCurrent;
         return;
        }
      while (MessageHandlerData(theEnv)->NextInCore->hnd->type == MPRIMARY)
        {
         MessageHandlerData(theEnv)->NextInCore = MessageHandlerData(theEnv)->NextInCore->nxt;
         if (MessageHandlerData(theEnv)->NextInCore == NULL)
           {
            MessageHandlerData(theEnv)->NextInCore = oldNext;
            MessageHandlerData(theEnv)->CurrentCore = oldCurrent;
            return;
           }
        }
     }
   while (MessageHandlerData(theEnv)->NextInCore->hnd->type == MAFTER)
     {
      MessageHandlerData(theEnv)->CurrentCore = MessageHandlerData(theEnv)->NextInCore;
      MessageHandlerData(theEnv)->NextInCore = MessageHandlerData(theEnv)->NextInCore->nxt;
#if DEBUGGING_FUNCTIONS
      if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
        WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,BEGIN_TRACE);
#endif
      if (CheckHandlerArgCount(theEnv))
        {
#if PROFILING_FUNCTIONS
         StartProfile(theEnv,&profileFrame,
                      &MessageHandlerData(theEnv)->CurrentCore->hnd->usrData,
                      ProfileFunctionData(theEnv)->ProfileConstructs);
#endif


         EvaluateProcActions(theEnv,MessageHandlerData(theEnv)->CurrentCore->hnd->cls->header.whichModule->theModule,
                            MessageHandlerData(theEnv)->CurrentCore->hnd->actions,
                            MessageHandlerData(theEnv)->CurrentCore->hnd->localVarCount,
                            &temp,UnboundHandlerErr);

#if PROFILING_FUNCTIONS
         EndProfile(theEnv,&profileFrame);
#endif
        }


#if DEBUGGING_FUNCTIONS
      if (MessageHandlerData(theEnv)->CurrentCore->hnd->trace)
        WatchHandler(theEnv,WTRACE,MessageHandlerData(theEnv)->CurrentCore,END_TRACE);
#endif
      ProcedureFunctionData(theEnv)->ReturnFlag = FALSE;
      if ((MessageHandlerData(theEnv)->NextInCore == NULL) || EvaluationData(theEnv)->HaltExecution)
        {
         MessageHandlerData(theEnv)->NextInCore = oldNext;
         MessageHandlerData(theEnv)->CurrentCore = oldCurrent;
         return;
        }
     }

   MessageHandlerData(theEnv)->NextInCore = oldNext;
   MessageHandlerData(theEnv)->CurrentCore = oldCurrent;
  }


/********************************************************
  NAME         : EarlySlotBindError
  DESCRIPTION  : Prints out an error message when
                 a message-handler from a superclass
                 which contains a static-bind
                 slot access is not valid for the
                 currently active instance (i.e.
                 the instance is not using the
                 superclass's slot)
  INPUTS       : 1) The currently active instance
                 2) The defclass holding the invalid slot
                 3) The canonical id of the slot
  RETURNS      : Nothing useful
  SIDE EFFECTS : Error message printed
  NOTES        : None
 ********************************************************/
static void EarlySlotBindError(
  void *theEnv,
  INSTANCE_TYPE *theInstance,
  DEFCLASS *theDefclass,
  unsigned slotID)
  {
   SLOT_DESC *sd;

   sd = theDefclass->instanceTemplate[theDefclass->slotNameMap[slotID] - 1];
   PrintErrorID(theEnv,"MSGPASS",3,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Static reference to slot ");
   EnvPrintRouter(theEnv,WERROR,ValueToString(sd->slotName->name));
   EnvPrintRouter(theEnv,WERROR," of class ");
   PrintClassName(theEnv,WERROR,theDefclass,FALSE);
   EnvPrintRouter(theEnv,WERROR," does not apply to ");
   PrintInstanceNameAndClass(theEnv,WERROR,theInstance,TRUE);
  }

#endif

