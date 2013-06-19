   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  05/17/06          */
   /*                                                     */
   /*                  OBJECT MESSAGE FUNCTIONS           */
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
/*      6.23: Changed name of variable log to logName        */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
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

#include "classcom.h"
#include "classfun.h"
#include "memalloc.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "insfun.h"
#include "msgcom.h"
#include "prccode.h"
#include "router.h"

#define _MSGFUN_SOURCE_
#include "msgfun.h"

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

#if DEBUGGING_FUNCTIONS
static HANDLER_LINK *DisplayPrimaryCore(void *,char *,HANDLER_LINK *,int);
static void PrintPreviewHandler(void *,char *,HANDLER_LINK *,int,char *);
#endif

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/********************************************************
  NAME         : UnboundHandlerErr
  DESCRIPTION  : Print out a synopis of the currently
                   executing handler for unbound variable
                   errors
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Error synopsis printed to WERROR
  NOTES        : None
 ********************************************************/
globle void UnboundHandlerErr(
  void *theEnv)
  {
   EnvPrintRouter(theEnv,WERROR,"message-handler ");
   PrintHandler(theEnv,WERROR,MessageHandlerData(theEnv)->CurrentCore->hnd,TRUE);
  }

/*****************************************************************
  NAME         : PrintNoHandlerError
  DESCRIPTION  : Print "No primaries found" error message for send
  INPUTS       : The name of the message
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 *****************************************************************/
globle void PrintNoHandlerError(
  void *theEnv,
  char *msg)
  {
   PrintErrorID(theEnv,"MSGFUN",1,FALSE);
   EnvPrintRouter(theEnv,WERROR,"No applicable primary message-handlers found for ");
   EnvPrintRouter(theEnv,WERROR,msg);
   EnvPrintRouter(theEnv,WERROR,".\n");
  }

/***************************************************************
  NAME         : CheckHandlerArgCount
  DESCRIPTION  : Verifies that the current argument
                   list satisfies the current
                   handler's parameter count restriction
  INPUTS       : None
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : EvaluationError set on errors
  NOTES        : Uses ProcParamArraySize and CurrentCore globals
 ***************************************************************/
globle int CheckHandlerArgCount(
  void *theEnv)
  {
   HANDLER *hnd;

   hnd = MessageHandlerData(theEnv)->CurrentCore->hnd;
   if ((hnd->maxParams == -1) ? (ProceduralPrimitiveData(theEnv)->ProcParamArraySize < hnd->minParams) :
       (ProceduralPrimitiveData(theEnv)->ProcParamArraySize != hnd->minParams))
     {
      SetEvaluationError(theEnv,TRUE);
      PrintErrorID(theEnv,"MSGFUN",2,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Message-handler ");
      EnvPrintRouter(theEnv,WERROR,ValueToString(hnd->name));
      EnvPrintRouter(theEnv,WERROR," ");
      EnvPrintRouter(theEnv,WERROR,MessageHandlerData(theEnv)->hndquals[hnd->type]);
      EnvPrintRouter(theEnv,WERROR," in class ");
      EnvPrintRouter(theEnv,WERROR,EnvGetDefclassName(theEnv,(void *) hnd->cls));
      EnvPrintRouter(theEnv,WERROR," expected ");
      if (hnd->maxParams == -1)
        EnvPrintRouter(theEnv,WERROR,"at least ");
      else
        EnvPrintRouter(theEnv,WERROR,"exactly ");
      PrintLongInteger(theEnv,WERROR,(long long) (hnd->minParams-1));
      EnvPrintRouter(theEnv,WERROR," argument(s).\n");
      return(FALSE);
     }
   return(TRUE);
  }

/***************************************************
  NAME         : SlotAccessViolationError
  DESCRIPTION  : Prints out an error message when
                 attempt is made to set a read-only
                 or initialize-only slot improperly
  INPUTS       : 1) The slot name
                 2) A flag indicating if the source
                    is a class or an instance
                 3) A pointer to the source
                    instance/class
  RETURNS      : Nothing useful
  SIDE EFFECTS : Error message printed
  NOTES        : None
 ***************************************************/
globle void SlotAccessViolationError(
  void *theEnv,
  char *slotName,
  intBool instanceFlag,
  void *theInstanceOrClass)
  {
   PrintErrorID(theEnv,"MSGFUN",3,FALSE);
   EnvPrintRouter(theEnv,WERROR,slotName);
   EnvPrintRouter(theEnv,WERROR," slot in ");
   if (instanceFlag)
     PrintInstanceNameAndClass(theEnv,WERROR,(INSTANCE_TYPE *) theInstanceOrClass,FALSE);
   else
     {
      EnvPrintRouter(theEnv,WERROR,"class ");
      PrintClassName(theEnv,WERROR,(DEFCLASS *) theInstanceOrClass,FALSE);
     }
   EnvPrintRouter(theEnv,WERROR,": write access denied.\n");
  }

/***************************************************
  NAME         : SlotVisibilityViolationError
  DESCRIPTION  : Prints out an error message when
                 attempt is made to access a
                 private slot improperly
  INPUTS       : 1) The slot descriptor
                 2) A pointer to the source class
  RETURNS      : Nothing useful
  SIDE EFFECTS : Error message printed
  NOTES        : None
 ***************************************************/
globle void SlotVisibilityViolationError(
  void *theEnv,
  SLOT_DESC *sd,
  DEFCLASS *theDefclass)
  {
   PrintErrorID(theEnv,"MSGFUN",6,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Private slot ");
   EnvPrintRouter(theEnv,WERROR,ValueToString(sd->slotName->name));
   EnvPrintRouter(theEnv,WERROR," of class ");
   PrintClassName(theEnv,WERROR,sd->cls,FALSE);
   EnvPrintRouter(theEnv,WERROR," cannot be accessed directly\n   by handlers attached to class ");
   PrintClassName(theEnv,WERROR,theDefclass,TRUE);
  }

#if ! RUN_TIME

/******************************************************************************
  NAME         : NewSystemHandler
  DESCRIPTION  : Adds a new system handler for a system class

                 The handler is assumed to be primary and of
                 the form:

                 (defmessage-handler <class> <handler> () (<func>))

  INPUTS       : 1) Name-string of the system class
                 2) Name-string of the system handler
                 3) Name-string of the internal H/L function to implement
                      this handler
                 4) The number of extra arguments (past the instance itself)
                    that the handler willl accept
  RETURNS      : Nothing useful
  SIDE EFFECTS : Creates the new handler and inserts it in the system class's
                   handler array
                 On errors, generate a system error and exits.
  NOTES        : Does not check to see if handler already exists
 *******************************************************************************/
globle void NewSystemHandler(
  void *theEnv,
  char *cname,
  char *mname,
  char *fname,
  int extraargs)
  {
   DEFCLASS *cls;
   HANDLER *hnd;

   cls = LookupDefclassInScope(theEnv,cname);
   hnd = InsertHandlerHeader(theEnv,cls,(SYMBOL_HN *) EnvAddSymbol(theEnv,mname),MPRIMARY);
   IncrementSymbolCount(hnd->name);
   hnd->system = 1;
   hnd->minParams = hnd->maxParams = (short) (extraargs + 1);
   hnd->localVarCount = 0;
   hnd->actions = get_struct(theEnv,expr);
   hnd->actions->argList = NULL;
   hnd->actions->type = FCALL;
   hnd->actions->value = (void *) FindFunction(theEnv,fname);
   hnd->actions->nextArg = NULL;
  }

/***************************************************
  NAME         : InsertHandlerHeader
  DESCRIPTION  : Allocates a new handler header and
                   inserts it in the proper (sorted)
                   position in the class hnd array
  INPUTS       : 1) The class
                 2) The handler name
                 3) The handler type
  RETURNS      : The address of the new handler
                   header, NULL on errors
  SIDE EFFECTS : Class handler array reallocated
                   and resorted
  NOTES        : Assumes handler does not exist
 ***************************************************/
globle HANDLER *InsertHandlerHeader(
  void *theEnv,
  DEFCLASS *cls,
  SYMBOL_HN *mname,
  int mtype)
  {
   HANDLER *nhnd,*hnd;
   unsigned *narr,*arr;
   long i;
   long j,ni = -1;

   hnd = cls->handlers;
   arr = cls->handlerOrderMap;
   nhnd = (HANDLER *) gm2(theEnv,(sizeof(HANDLER) * (cls->handlerCount+1)));
   narr = (unsigned *) gm2(theEnv,(sizeof(unsigned) * (cls->handlerCount+1)));
   GenCopyMemory(HANDLER,cls->handlerCount,nhnd,hnd);
   for (i = 0 , j = 0 ; i < cls->handlerCount ; i++ , j++)
     {
      if (ni == -1)
        {
         if ((hnd[arr[i]].name->bucket > mname->bucket) ? TRUE :
             (hnd[arr[i]].name == mname))
           {
            ni = i;
            j++;
           }
        }
      narr[j] = arr[i];
     }
   if (ni == -1)
     ni = (int) cls->handlerCount;
   narr[ni] = cls->handlerCount;
   nhnd[cls->handlerCount].system = 0;
   nhnd[cls->handlerCount].type = mtype;
   nhnd[cls->handlerCount].busy = 0;
   nhnd[cls->handlerCount].mark = 0;
#if DEBUGGING_FUNCTIONS
   nhnd[cls->handlerCount].trace = MessageHandlerData(theEnv)->WatchHandlers;
#endif
   nhnd[cls->handlerCount].name = mname;
   nhnd[cls->handlerCount].cls = cls;
   nhnd[cls->handlerCount].minParams = 0;
   nhnd[cls->handlerCount].maxParams = 0;
   nhnd[cls->handlerCount].localVarCount = 0;
   nhnd[cls->handlerCount].actions = NULL;
   nhnd[cls->handlerCount].ppForm = NULL;
   nhnd[cls->handlerCount].usrData = NULL;
   if (cls->handlerCount != 0)
     {
      rm(theEnv,(void *) hnd,(sizeof(HANDLER) * cls->handlerCount));
      rm(theEnv,(void *) arr,(sizeof(unsigned) * cls->handlerCount));
     }
   cls->handlers = nhnd;
   cls->handlerOrderMap = narr;
   cls->handlerCount++;
   return(&nhnd[cls->handlerCount-1]);
  }

#endif

#if (! BLOAD_ONLY) && (! RUN_TIME)

/*****************************************************
  NAME         : HandlersExecuting
  DESCRIPTION  : Determines if any message-handlers
                   for a class are currently executing
  INPUTS       : The class address
  RETURNS      : TRUE if any handlers are executing,
                   FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 *****************************************************/
globle int HandlersExecuting(
  DEFCLASS *cls)
  {
   long i;

   for (i = 0 ; i < cls->handlerCount ; i++)
     if (cls->handlers[i].busy > 0)
       return(TRUE);
   return(FALSE);
  }

/*********************************************************************
  NAME         : DeleteHandler
  DESCRIPTION  : Deletes one or more message-handlers
                   from a class definition
  INPUTS       : 1) The class address
                 2) The message-handler name
                    (if this is * and there is no handler
                     called *, then the delete operations
                     will be applied to all handlers matching the type
                 3) The message-handler type
                    (if this is -1, then the delete operations will be
                     applied to all handlers matching the name
                 4) A flag saying whether to print error messages when
                     handlers are not found meeting specs
  RETURNS      : 1 if successful, 0 otherwise
  SIDE EFFECTS : Handlers deleted
  NOTES        : If any handlers for the class are
                   currently executing, this routine
                   will fail
 **********************************************************************/
globle int DeleteHandler(
   void *theEnv,
   DEFCLASS *cls,
   SYMBOL_HN *mname,
   int mtype,
   int indicate_missing)
  {
   long i;
   HANDLER *hnd;
   int found,success = 1;

   if (cls->handlerCount == 0)
     {
      if (indicate_missing)
        {
         HandlerDeleteError(theEnv,EnvGetDefclassName(theEnv,(void *) cls));
         return(0);
        }
      return(1);
     }
   if (HandlersExecuting(cls))
     {
      HandlerDeleteError(theEnv,EnvGetDefclassName(theEnv,(void *) cls));
      return(0);
     }
   if (mtype == -1)
     {
      found = FALSE;
      for (i = MAROUND ; i <= MAFTER ; i++)
        {
         hnd = FindHandlerByAddress(cls,mname,(unsigned) i);
         if (hnd != NULL)
           {
            found = TRUE;
            if (hnd->system == 0)
              hnd->mark = 1;
            else
              {
               PrintErrorID(theEnv,"MSGPSR",3,FALSE);
               EnvPrintRouter(theEnv,WERROR,"System message-handlers may not be modified.\n");
               success = 0;
              }
           }
        }
      if ((found == FALSE) ? (strcmp(ValueToString(mname),"*") == 0) : FALSE)
        {
         for (i = 0 ; i < cls->handlerCount ; i++)
           if (cls->handlers[i].system == 0)
             cls->handlers[i].mark = 1;
        }
     }
   else
     {
      hnd = FindHandlerByAddress(cls,mname,(unsigned) mtype);
      if (hnd == NULL)
        {
         if (strcmp(ValueToString(mname),"*") == 0)
           {
            for (i = 0 ; i < cls->handlerCount ; i++)
              if ((cls->handlers[i].type == (unsigned) mtype) &&
                  (cls->handlers[i].system == 0))
                cls->handlers[i].mark = 1;
           }
         else
           {
            if (indicate_missing)
              HandlerDeleteError(theEnv,EnvGetDefclassName(theEnv,(void *) cls));
            success = 0;
           }
        }
      else if (hnd->system == 0)
        hnd->mark = 1;
      else
        {
         if (indicate_missing)
           {
            PrintErrorID(theEnv,"MSGPSR",3,FALSE);
            EnvPrintRouter(theEnv,WERROR,"System message-handlers may not be modified.\n");
           }
         success = 0;
        }
     }
   DeallocateMarkedHandlers(theEnv,cls);
   return(success);
  }

/***************************************************
  NAME         : DeallocateMarkedHandlers
  DESCRIPTION  : Removes any handlers from a class
                   that have been previously marked
                   for deletion.
  INPUTS       : The class
  RETURNS      : Nothing useful
  SIDE EFFECTS : Marked handlers are deleted
  NOTES        : Assumes none of the handlers are
                   currently executing or have a
                   busy count != 0 for any reason
 ***************************************************/
globle void DeallocateMarkedHandlers(
  void *theEnv,
  DEFCLASS *cls)
  {
   short count;
   HANDLER *hnd,*nhnd;
   unsigned *arr,*narr;
   long i,j;

   for (i = 0 , count = 0 ; i < cls->handlerCount ; i++)
     {
      hnd = &cls->handlers[i];
      if (hnd->mark == 1)
        {
         count++;
         DecrementSymbolCount(theEnv,hnd->name);
         ExpressionDeinstall(theEnv,hnd->actions);
         ReturnPackedExpression(theEnv,hnd->actions);
         ClearUserDataList(theEnv,hnd->usrData);
         if (hnd->ppForm != NULL)
           rm(theEnv,(void *) hnd->ppForm,
              (sizeof(char) * (strlen(hnd->ppForm)+1)));
        }
      else
         /* ============================================
            Use the busy field to count how many
            message-handlers are removed before this one
            ============================================ */
        hnd->busy = count;
     }
   if (count == 0)
     return;
   if (count == cls->handlerCount)
     {
      rm(theEnv,(void *) cls->handlers,(sizeof(HANDLER) * cls->handlerCount));
      rm(theEnv,(void *) cls->handlerOrderMap,(sizeof(unsigned) * cls->handlerCount));
      cls->handlers = NULL;
      cls->handlerOrderMap = NULL;
      cls->handlerCount = 0;
     }
   else
     {
      count = (short) (cls->handlerCount - count);
      hnd = cls->handlers;
      arr = cls->handlerOrderMap;
      nhnd = (HANDLER *) gm2(theEnv,(sizeof(HANDLER) * count));
      narr = (unsigned *) gm2(theEnv,(sizeof(unsigned) * count));
      for (i = 0 , j = 0 ; j < count ; i++)
        {
         if (hnd[arr[i]].mark == 0)
           {
            /* ==============================================================
               The offsets in the map need to be decremented by the number of
               preceding nodes which were deleted.  Use the value of the busy
               field set in the first loop.
               ============================================================== */
            narr[j] = arr[i] - hnd[arr[i]].busy;
            j++;
           }
        }
      for (i = 0 , j = 0 ; j < count ; i++)
        {
         if (hnd[i].mark == 0)
           {
            hnd[i].busy = 0;
            GenCopyMemory(HANDLER,1,&nhnd[j],&hnd[i]);
            j++;
           }
        }
      rm(theEnv,(void *) hnd,(sizeof(HANDLER) * cls->handlerCount));
      rm(theEnv,(void *) arr,(sizeof(unsigned) * cls->handlerCount));
      cls->handlers = nhnd;
      cls->handlerOrderMap = narr;
      cls->handlerCount = count;
     }
  }

#endif

/*****************************************************
  NAME         : HandlerType
  DESCRIPTION  : Determines type of message-handler
  INPUTS       : 1) Calling function string
                 2) String representing type
  RETURNS      : MAROUND  (0) for "around"
                 MBEFORE  (1) for "before"
                 MPRIMARY (2) for "primary"
                 MAFTER   (3) for "after"
                 MERROR   (4) on errors
  SIDE EFFECTS : None
  NOTES        : None
 *****************************************************/
globle unsigned HandlerType(
  void *theEnv,
  char *func,
  char *str)
  {
   register unsigned i;

   for (i = MAROUND ; i <= MAFTER ; i++)
     if (strcmp(str,MessageHandlerData(theEnv)->hndquals[i]) == 0)
       {
        return(i);
       }

   PrintErrorID(theEnv,"MSGFUN",7,FALSE);
   EnvPrintRouter(theEnv,"werror","Unrecognized message-handler type in ");
   EnvPrintRouter(theEnv,"werror",func);
   EnvPrintRouter(theEnv,"werror",".\n");
   return(MERROR);
  }

/*****************************************************************
  NAME         : CheckCurrentMessage
  DESCRIPTION  : Makes sure that a message is available
                   and active for an internal message function
  INPUTS       : 1) The name of the function checking the message
                 2) A flag indicating whether the object must be
                      a class instance or not (it could be a
                      primitive type)
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : EvaluationError set on errors
  NOTES        : None
 *****************************************************************/
globle int CheckCurrentMessage(
  void *theEnv,
  char *func,
  int ins_reqd)
  {
   register DATA_OBJECT *activeMsgArg;

   if (!MessageHandlerData(theEnv)->CurrentCore || (MessageHandlerData(theEnv)->CurrentCore->hnd->actions != ProceduralPrimitiveData(theEnv)->CurrentProcActions))
     {
      PrintErrorID(theEnv,"MSGFUN",4,FALSE);
      EnvPrintRouter(theEnv,WERROR,func);
      EnvPrintRouter(theEnv,WERROR," may only be called from within message-handlers.\n");
      SetEvaluationError(theEnv,TRUE);
      return(FALSE);
     }
   activeMsgArg = GetNthMessageArgument(theEnv,0);
   if ((ins_reqd == TRUE) ? (activeMsgArg->type != INSTANCE_ADDRESS) : FALSE)
     {
      PrintErrorID(theEnv,"MSGFUN",5,FALSE);
      EnvPrintRouter(theEnv,WERROR,func);
      EnvPrintRouter(theEnv,WERROR," operates only on instances.\n");
      SetEvaluationError(theEnv,TRUE);
      return(FALSE);
     }
   if ((activeMsgArg->type == INSTANCE_ADDRESS) ?
       (((INSTANCE_TYPE *) activeMsgArg->value)->garbage == 1) : FALSE)
     {
      StaleInstanceAddress(theEnv,func,0);
      SetEvaluationError(theEnv,TRUE);
      return(FALSE);
     }
   return(TRUE);
  }

/***************************************************
  NAME         : PrintHandler
  DESCRIPTION  : Displays a handler synopsis
  INPUTS       : 1) Logical name of output
                 2) The handler
                 5) Flag indicating whether to
                    printout a terminating newline
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
globle void PrintHandler(
  void *theEnv,
  char *logName,
  HANDLER *theHandler,
  int crtn)
  {
   EnvPrintRouter(theEnv,logName,ValueToString(theHandler->name));
   EnvPrintRouter(theEnv,logName," ");
   EnvPrintRouter(theEnv,logName,MessageHandlerData(theEnv)->hndquals[theHandler->type]);
   EnvPrintRouter(theEnv,logName," in class ");
   PrintClassName(theEnv,logName,theHandler->cls,crtn);
  }

/***********************************************************
  NAME         : FindHandlerByAddress
  DESCRIPTION  : Uses a binary search on a class's
                   handler header array
  INPUTS       : 1) The class address
                 2) The handler symbolic name
                 3) The handler type (MPRIMARY,etc.)
  RETURNS      : The address of the found handler,
                   NULL if not found
  SIDE EFFECTS : None
  NOTES        : Assumes array is in ascending order
                   1st key: symbolic name of handler
                   2nd key: type of handler
 ***********************************************************/
globle HANDLER *FindHandlerByAddress(
  DEFCLASS *cls,
  SYMBOL_HN *name,
  unsigned type)
  {
   register int b;
   long i;
   HANDLER *hnd;
   unsigned *arr;

   if ((b = FindHandlerNameGroup(cls,name)) == -1)
     return(NULL);
   arr = cls->handlerOrderMap;
   hnd = cls->handlers;
   for (i = (unsigned) b ; i < cls->handlerCount ; i++)
     {
      if (hnd[arr[i]].name != name)
        return(NULL);
      if (hnd[arr[i]].type == type)
        return(&hnd[arr[i]]);
     }
   return(NULL);
  }

/***********************************************************
  NAME         : FindHandlerByIndex
  DESCRIPTION  : Uses a binary search on a class's
                   handler header array
  INPUTS       : 1) The class address
                 2) The handler symbolic name
                 3) The handler type (MPRIMARY,etc.)
  RETURNS      : The index of the found handler,
                   -1 if not found
  SIDE EFFECTS : None
  NOTES        : Assumes array is in ascending order
                   1st key: symbolic name of handler
                   2nd key: type of handler
 ***********************************************************/
globle int FindHandlerByIndex(
  DEFCLASS *cls,
  SYMBOL_HN *name,
  unsigned type)
  {
   register int b;
   long i;
   HANDLER *hnd;
   unsigned *arr;

   if ((b = FindHandlerNameGroup(cls,name)) == -1)
     return(-1);
   arr = cls->handlerOrderMap;
   hnd = cls->handlers;
   for (i = (unsigned) b ; i < cls->handlerCount ; i++)
     {
      if (hnd[arr[i]].name != name)
        return(-1);
      if (hnd[arr[i]].type == type)
        return((int) arr[i]);
     }
   return(-1);
  }

/*****************************************************
  NAME         : FindHandlerNameGroup
  DESCRIPTION  : Uses a binary search on a class's
                   handler header array
  INPUTS       : 1) The class address
                 2) The handler symbolic name
  RETURNS      : The index of the found handler group
                   -1 if not found
  SIDE EFFECTS : None
  NOTES        : Assumes array is in ascending order
                   1st key: handler name symbol bucket
 *****************************************************/
globle int FindHandlerNameGroup(
  DEFCLASS *cls,
  SYMBOL_HN *name)
  {
   register int b,e,i,j;
   HANDLER *hnd;
   unsigned *arr;
   int start;

   if (cls->handlerCount == 0)
     return(-1);
   hnd = cls->handlers;
   arr = cls->handlerOrderMap;
   b = 0;
   e = (int) (cls->handlerCount-1);
   start = -1;
   do
     {
      i = (b+e)/2;
      if (name->bucket == hnd[arr[i]].name->bucket)
        {
         for (j = i ; j >= b ; j--)
           {
            if (hnd[arr[j]].name == name)
              start = j;
            if (hnd[arr[j]].name->bucket != name->bucket)
              break;
           }
         if (start != -1)
           return(start);
         for (j = i+1 ; j <= e ; j++)
           {
            if (hnd[arr[j]].name == name)
              return(j);
            if (hnd[arr[j]].name->bucket != name->bucket)
              return(-1);
           }
         return(-1);
        }
      else if (name->bucket < hnd[arr[i]].name->bucket)
        e = i-1;
      else
        b = i+1;
     }
   while (b <= e);
   return(-1);
  }

/***************************************************
  NAME         : HandlerDeleteError
  DESCRIPTION  : Prints out an error message when
                   handlers cannot be deleted
  INPUTS       : Name-string of the class
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
globle void HandlerDeleteError(
  void *theEnv,
  char *cname)
  {
   PrintErrorID(theEnv,"MSGFUN",8,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Unable to delete message-handler(s) from class ");
   EnvPrintRouter(theEnv,WERROR,cname);
   EnvPrintRouter(theEnv,WERROR,".\n");
  }

#if DEBUGGING_FUNCTIONS

/********************************************************************
  NAME         : DisplayCore
  DESCRIPTION  : Gives a schematic "printout" of the
                   core framework for a message showing
                   arounds, primaries, shadows etc.
                 This routine uses recursion to print indentation
                   to indicate shadowing and where handlers begin
                   and end execution wrt one another.
  INPUTS       : 1) Logical name of output
                 2) The remaining core
                 3) The number of handlers this (partial) core
                    shadows
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : Expects that the core was created in PREVIEW mode,
                   i.e. implicit handlers are SLOT_DESC addresses
                   (in PERFORM mode they are INSTANCE_SLOT addresses)
                 Assumes (partial) core is not empty
 ********************************************************************/
globle void DisplayCore(
  void *theEnv,
  char *logicalName,
  HANDLER_LINK *core,
  int sdepth)
  {
   if (core->hnd->type == MAROUND)
     {
      PrintPreviewHandler(theEnv,logicalName,core,sdepth,BEGIN_TRACE);
      if (core->nxt != NULL)
        DisplayCore(theEnv,logicalName,core->nxt,sdepth+1);
      PrintPreviewHandler(theEnv,logicalName,core,sdepth,END_TRACE);
     }
   else
     {
      while ((core != NULL) ? (core->hnd->type == MBEFORE) : FALSE)
        {
         PrintPreviewHandler(theEnv,logicalName,core,sdepth,BEGIN_TRACE);
         PrintPreviewHandler(theEnv,logicalName,core,sdepth,END_TRACE);
         core = core->nxt;
        }
      if ((core != NULL) ? (core->hnd->type == MPRIMARY) : FALSE)

        core = DisplayPrimaryCore(theEnv,logicalName,core,sdepth);

      while ((core != NULL) ? (core->hnd->type == MAFTER) : FALSE)
        {
         PrintPreviewHandler(theEnv,logicalName,core,sdepth,BEGIN_TRACE);
         PrintPreviewHandler(theEnv,logicalName,core,sdepth,END_TRACE);
         core = core->nxt;
        }

     }
  }

/*******************************************************************
  NAME         : FindPreviewApplicableHandlers
  DESCRIPTION  : See FindApplicableHandlers
                 However, this function only examines classes rather
                   than instances for implicit slot-accessors
  INPUTS       : 1) The class address
                 2) The message name symbol
  RETURNS      : The links of applicable handlers, NULL on errors
  SIDE EFFECTS : Links are allocated for the list
  NOTES        : None
 ******************************************************************/
globle HANDLER_LINK *FindPreviewApplicableHandlers(
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

/***********************************************************
  NAME         : WatchMessage
  DESCRIPTION  : Prints a condensed description of a
                   message and its arguments
  INPUTS       : 1) The output logical name
                 2) BEGIN_TRACE or END_TRACE string
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : Uses the global variables ProcParamArray
                   and CurrentMessageName
 ***********************************************************/
globle void WatchMessage(
  void *theEnv,
  char *logName,
  char *tstring)
  {
   EnvPrintRouter(theEnv,logName,"MSG ");
   EnvPrintRouter(theEnv,logName,tstring);
   EnvPrintRouter(theEnv,logName," ");
   EnvPrintRouter(theEnv,logName,ValueToString(MessageHandlerData(theEnv)->CurrentMessageName));
   EnvPrintRouter(theEnv,logName," ED:");
   PrintLongInteger(theEnv,logName,(long long) EvaluationData(theEnv)->CurrentEvaluationDepth);
   PrintProcParamArray(theEnv,logName);
  }

/***********************************************************
  NAME         : WatchHandler
  DESCRIPTION  : Prints a condensed description of a
                   message-handler and its arguments
  INPUTS       : 1) The output logical name
                 2) The handler address
                 3) BEGIN_TRACE or END_TRACE string
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : Uses the global variables ProcParamArray
                   and CurrentMessageName
 ***********************************************************/
globle void WatchHandler(
  void *theEnv,
  char *logName,
  HANDLER_LINK *hndl,
  char *tstring)
  {
   HANDLER *hnd;
   
   EnvPrintRouter(theEnv,logName,"HND ");
   EnvPrintRouter(theEnv,logName,tstring);
   EnvPrintRouter(theEnv,logName," ");
   hnd = hndl->hnd;
   PrintHandler(theEnv,WTRACE,hnd,TRUE);
   EnvPrintRouter(theEnv,logName,"       ED:");
   PrintLongInteger(theEnv,logName,(long long) EvaluationData(theEnv)->CurrentEvaluationDepth);
   PrintProcParamArray(theEnv,logName);
  }

#endif /* DEBUGGING_FUNCTIONS */

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

#if DEBUGGING_FUNCTIONS

/********************************************************************
  NAME         : DisplayPrimaryCore
  DESCRIPTION  : Gives a schematic "printout" of the primary
                   message showing other shadowed primaries
                 This routine uses recursion to print indentation
                   to indicate shadowing and where handlers begin
                   and end execution wrt one another.
  INPUTS       : 1) The logical name of the output
                 2) The remaining core
                 3) The number of handlers this (partial) core
                    shadows
  RETURNS      : The address of the handler following the primary
                   group of handlers in the core
  SIDE EFFECTS : None
  NOTES        : Expects that the core was created in PREVIEW mode,
                   i.e. implicit handlers are SLOT_DESC addresses
                   (in PERFORM mode they are INSTANCE_SLOT addresses)
                 Assumes (partial) core is not empty
 ********************************************************************/
static HANDLER_LINK *DisplayPrimaryCore(
  void *theEnv,
  char *logicalName,
  HANDLER_LINK *core,
  int pdepth)
  {
   register HANDLER_LINK *rtn;

   PrintPreviewHandler(theEnv,logicalName,core,pdepth,BEGIN_TRACE);
   if ((core->nxt != NULL) ? (core->nxt->hnd->type == MPRIMARY) : FALSE)
     rtn = DisplayPrimaryCore(theEnv,logicalName,core->nxt,pdepth+1);
   else
     rtn = core->nxt;
   PrintPreviewHandler(theEnv,logicalName,core,pdepth,END_TRACE);
   return(rtn);
  }

/***************************************************
  NAME         : PrintPreviewHandler
  DESCRIPTION  : Displays a message preview
  INPUTS       : 1) The logical name of the output
                 2) Handler-link
                 3) Number of handlers shadowed
                 4) The trace-string
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
static void PrintPreviewHandler(
  void *theEnv,
  char *logicalName,
  HANDLER_LINK *cptr,
  int sdepth,
  char *tstr)
  {
   register int i;

   for (i = 0 ; i < sdepth ; i++)
     EnvPrintRouter(theEnv,logicalName,"| ");
   EnvPrintRouter(theEnv,logicalName,tstr);
   EnvPrintRouter(theEnv,logicalName," ");
   PrintHandler(theEnv,logicalName,cptr->hnd,TRUE);
  }

#endif

#endif

