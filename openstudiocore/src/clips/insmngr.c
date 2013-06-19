   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*              CLIPS Version 6.24  05/17/06           */
   /*                                                     */
   /*            INSTANCE PRIMITIVE SUPPORT MODULE        */
   /*******************************************************/

/*************************************************************/
/* Purpose:  Creation and Deletion of Instances Routines     */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Removed LOGICAL_DEPENDENCIES compilation flag. */
/*                                                           */
/*            Converted INSTANCE_PATTERN_MATCHING to         */
/*            DEFRULE_CONSTRUCT.                             */
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

#if DEFRULE_CONSTRUCT
#include "network.h"
#include "drive.h"
#include "objrtmch.h"
#include "lgcldpnd.h"
#endif

#include "classcom.h"
#include "classfun.h"
#include "engine.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "extnfunc.h"
#include "insfun.h"
#include "modulutl.h"
#include "msgcom.h"
#include "msgfun.h"
#include "prccode.h"
#include "router.h"
#include "sysdep.h"
#include "utility.h"

#define _INSMNGR_SOURCE_
#include "insmngr.h"

#include "inscom.h"
#include "watch.h"

/* =========================================
   *****************************************
                   CONSTANTS
   =========================================
   ***************************************** */
#define MAKE_TRACE   "==>"
#define UNMAKE_TRACE "<=="

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static INSTANCE_TYPE *NewInstance(void *);
static INSTANCE_TYPE *InstanceLocationInfo(void *,DEFCLASS *,SYMBOL_HN *,INSTANCE_TYPE **,
                                           unsigned *);
static void InstallInstance(void *,INSTANCE_TYPE *,int);
static void BuildDefaultSlots(void *,intBool);
static int CoreInitializeInstance(void *,INSTANCE_TYPE *,EXPRESSION *);
static int InsertSlotOverrides(void *,INSTANCE_TYPE *,EXPRESSION *);
static void EvaluateClassDefaults(void *,INSTANCE_TYPE *);

#if DEBUGGING_FUNCTIONS
static void PrintInstanceWatch(void *,char *,INSTANCE_TYPE *);
#endif

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***********************************************************
  NAME         : InitializeInstanceCommand
  DESCRIPTION  : Initializes an instance of a class
  INPUTS       : The address of the result value
  RETURNS      : Nothing useful
  SIDE EFFECTS : Instance intialized
  NOTES        : H/L Syntax:
                 (active-initialize-instance <instance-name>
                    <slot-override>*)
 ***********************************************************/
globle void InitializeInstanceCommand(
  void *theEnv,
  DATA_OBJECT *result)
  {
   INSTANCE_TYPE *ins;

   SetpType(result,SYMBOL);
   SetpValue(result,EnvFalseSymbol(theEnv));
   ins = CheckInstance(theEnv,"initialize-instance");
   if (ins == NULL)
     return;
   if (CoreInitializeInstance(theEnv,ins,GetFirstArgument()->nextArg) == TRUE)
     {
      SetpType(result,INSTANCE_NAME);
      SetpValue(result,(void *) ins->name);
     }
  }

/****************************************************************
  NAME         : MakeInstanceCommand
  DESCRIPTION  : Creates and initializes an instance of a class
  INPUTS       : The address of the result value
  RETURNS      : Nothing useful
  SIDE EFFECTS : Instance intialized
  NOTES        : H/L Syntax:
                 (active-make-instance <instance-name> of <class>
                    <slot-override>*)
 ****************************************************************/
globle void MakeInstanceCommand(
  void *theEnv,
  DATA_OBJECT *result)
  {
   SYMBOL_HN *iname;
   INSTANCE_TYPE *ins;
   DATA_OBJECT temp;
   DEFCLASS *cls;

   SetpType(result,SYMBOL);
   SetpValue(result,EnvFalseSymbol(theEnv));
   EvaluateExpression(theEnv,GetFirstArgument(),&temp);
   if ((GetType(temp) != SYMBOL) &&
       (GetType(temp) != INSTANCE_NAME))
     {
      PrintErrorID(theEnv,"INSMNGR",1,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Expected a valid name for new instance.\n");
      SetEvaluationError(theEnv,TRUE);
      return;
     }
   iname = (SYMBOL_HN *) GetValue(temp);

   if (GetFirstArgument()->nextArg->type == DEFCLASS_PTR)
     cls = (DEFCLASS *) GetFirstArgument()->nextArg->value;
   else
     {
      EvaluateExpression(theEnv,GetFirstArgument()->nextArg,&temp);
      if (GetType(temp) != SYMBOL)
        {
         PrintErrorID(theEnv,"INSMNGR",2,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Expected a valid class name for new instance.\n");
         SetEvaluationError(theEnv,TRUE);
         return;
        }
      cls = LookupDefclassInScope(theEnv,DOToString(temp));
      if (cls == NULL)
        {
         ClassExistError(theEnv,ValueToString(ExpressionFunctionCallName(EvaluationData(theEnv)->CurrentExpression)),
                         DOToString(temp));
         SetEvaluationError(theEnv,TRUE);
         return;
        }
     }

   ins = BuildInstance(theEnv,iname,cls,TRUE);
   if (ins == NULL)
     return;
     
   if (CoreInitializeInstance(theEnv,ins,GetFirstArgument()->nextArg->nextArg) == TRUE)
     {
      result->type = INSTANCE_NAME;
      result->value = (void *) GetFullInstanceName(theEnv,ins);
     }
   else
     QuashInstance(theEnv,ins);
  }

/***************************************************
  NAME         : GetFullInstanceName
  DESCRIPTION  : If this function is called while
                 the current module is other than
                 the one in which the instance
                 resides, then the module name is
                 prepended to the instance name.
                 Otherwise - the base name only is
                 returned.
  INPUTS       : The instance
  RETURNS      : The instance name symbol (with
                 module name and :: prepended)
  SIDE EFFECTS : Temporary buffer allocated possibly
                 and new symbol created
  NOTES        : Used to differentiate between
                 instances of the same name in
                 different modules
 ***************************************************/
globle SYMBOL_HN *GetFullInstanceName(
  void *theEnv,
  INSTANCE_TYPE *ins)
  {
   char *moduleName,*buffer;
   size_t bufsz;
   SYMBOL_HN *iname;

   if (ins == &InstanceData(theEnv)->DummyInstance)
     return((SYMBOL_HN *) EnvAddSymbol(theEnv,"Dummy Instance"));
   if (ins->garbage)
     return(ins->name);
   if (ins->cls->header.whichModule->theModule == ((struct defmodule *) EnvGetCurrentModule(theEnv)))
     return(ins->name);
   moduleName = EnvGetDefmoduleName(theEnv,(void *) ins->cls->header.whichModule->theModule);
   bufsz = (sizeof(char) * (strlen(moduleName) +
                                  strlen(ValueToString(ins->name)) + 3));
   buffer = (char *) gm2(theEnv,bufsz);
   gensprintf(buffer,"%s::%s",moduleName,ValueToString(ins->name));
   iname = (SYMBOL_HN *) EnvAddSymbol(theEnv,buffer);
   rm(theEnv,(void *) buffer,bufsz);
   return(iname);
  }

/***************************************************
  NAME         : BuildInstance
  DESCRIPTION  : Creates an uninitialized instance
  INPUTS       : 1) Name of the instance
                 2) Class pointer
                 3) Flag indicating whether init
                    message will be called for
                    this instance or not
  RETURNS      : The address of the new instance,
                   NULL on errors (or when a
                   a logical basis in a rule was
                   deleted int the same RHS in
                   which the instance creation
                   occurred)
  SIDE EFFECTS : Old definition (if any) is deleted
  NOTES        : None
 ***************************************************/
globle INSTANCE_TYPE *BuildInstance(
  void *theEnv,
  SYMBOL_HN *iname,
  DEFCLASS *cls,
  intBool initMessage)
  {
   INSTANCE_TYPE *ins,*iprv;
   unsigned hashTableIndex;
   unsigned modulePosition;
   SYMBOL_HN *moduleName;
   DATA_OBJECT temp;

#if DEFRULE_CONSTRUCT
   if (EngineData(theEnv)->JoinOperationInProgress && cls->reactive)
     {
      PrintErrorID(theEnv,"INSMNGR",10,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Cannot create instances of reactive classes while\n");
      EnvPrintRouter(theEnv,WERROR,"  pattern-matching is in process.\n");
      SetEvaluationError(theEnv,TRUE);
      return(NULL);
     }
#endif
   if (cls->abstract)
     {
      PrintErrorID(theEnv,"INSMNGR",3,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Cannot create instances of abstract class ");
      EnvPrintRouter(theEnv,WERROR,EnvGetDefclassName(theEnv,(void *) cls));
      EnvPrintRouter(theEnv,WERROR,".\n");
      SetEvaluationError(theEnv,TRUE);
      return(NULL);
     }
   modulePosition = FindModuleSeparator(ValueToString(iname));
   if (modulePosition)
     {
      moduleName = ExtractModuleName(theEnv,modulePosition,ValueToString(iname));
      if ((moduleName == NULL) ||
          (moduleName != cls->header.whichModule->theModule->name))
        {
         PrintErrorID(theEnv,"INSMNGR",11,TRUE);
         EnvPrintRouter(theEnv,WERROR,"Invalid module specifier in new instance name.\n");
         SetEvaluationError(theEnv,TRUE);
         return(NULL);
        }
      iname = ExtractConstructName(theEnv,modulePosition,ValueToString(iname));
     }
   ins = InstanceLocationInfo(theEnv,cls,iname,&iprv,&hashTableIndex);
   if (ins != NULL)
     {
      if (ins->installed == 0)
        {
         PrintErrorID(theEnv,"INSMNGR",4,FALSE);
         EnvPrintRouter(theEnv,WERROR,"The instance ");
         EnvPrintRouter(theEnv,WERROR,ValueToString(iname));
         EnvPrintRouter(theEnv,WERROR," has a slot-value which depends on the instance definition.\n");
         SetEvaluationError(theEnv,TRUE);
         return(NULL);
        }
      ins->busy++;
      IncrementSymbolCount(iname);
      if (ins->garbage == 0)
        {
         if (InstanceData(theEnv)->MkInsMsgPass)
           DirectMessage(theEnv,MessageHandlerData(theEnv)->DELETE_SYMBOL,ins,NULL,NULL);
         else
           QuashInstance(theEnv,ins);
        }
      ins->busy--;
      DecrementSymbolCount(theEnv,iname);
      if (ins->garbage == 0)
        {
         PrintErrorID(theEnv,"INSMNGR",5,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Unable to delete old instance ");
         EnvPrintRouter(theEnv,WERROR,ValueToString(iname));
         EnvPrintRouter(theEnv,WERROR,".\n");
         SetEvaluationError(theEnv,TRUE);
         return(NULL);
        }
     }

   /* =============================================================
      Create the base instance from the defaults of the inheritance
      precedence list
      ============================================================= */
   InstanceData(theEnv)->CurrentInstance = NewInstance(theEnv);

#if DEFRULE_CONSTRUCT
   /* ==============================================
      Add this new instance as a dependent to
      any currently active basis - if the partial
      match was deleted, abort the instance creation
      ============================================== */
   if (AddLogicalDependencies(theEnv,(struct patternEntity *) InstanceData(theEnv)->CurrentInstance,FALSE)
        == FALSE)
     {
      rtn_struct(theEnv,instance,InstanceData(theEnv)->CurrentInstance);
      InstanceData(theEnv)->CurrentInstance = NULL;
      return(NULL);
     }
#endif

   InstanceData(theEnv)->CurrentInstance->name = iname;
   InstanceData(theEnv)->CurrentInstance->cls = cls;
   BuildDefaultSlots(theEnv,initMessage);

   /* ============================================================
      Put the instance in the instance hash table and put it on its
        class's instance list
      ============================================================ */
   InstanceData(theEnv)->CurrentInstance->hashTableIndex = hashTableIndex;
   if (iprv == NULL)
     {
      InstanceData(theEnv)->CurrentInstance->nxtHash = InstanceData(theEnv)->InstanceTable[hashTableIndex];
      if (InstanceData(theEnv)->InstanceTable[hashTableIndex] != NULL)
        InstanceData(theEnv)->InstanceTable[hashTableIndex]->prvHash = InstanceData(theEnv)->CurrentInstance;
      InstanceData(theEnv)->InstanceTable[hashTableIndex] = InstanceData(theEnv)->CurrentInstance;
     }
   else
     {
      InstanceData(theEnv)->CurrentInstance->nxtHash = iprv->nxtHash;
      if (iprv->nxtHash != NULL)
        iprv->nxtHash->prvHash = InstanceData(theEnv)->CurrentInstance;
      iprv->nxtHash = InstanceData(theEnv)->CurrentInstance;
      InstanceData(theEnv)->CurrentInstance->prvHash = iprv;
     }

   /* ======================================
      Put instance in global and class lists
      ====================================== */
   if (InstanceData(theEnv)->CurrentInstance->cls->instanceList == NULL)
     InstanceData(theEnv)->CurrentInstance->cls->instanceList = InstanceData(theEnv)->CurrentInstance;
   else
     InstanceData(theEnv)->CurrentInstance->cls->instanceListBottom->nxtClass = InstanceData(theEnv)->CurrentInstance;
   InstanceData(theEnv)->CurrentInstance->prvClass = InstanceData(theEnv)->CurrentInstance->cls->instanceListBottom;
   InstanceData(theEnv)->CurrentInstance->cls->instanceListBottom = InstanceData(theEnv)->CurrentInstance;

   if (InstanceData(theEnv)->InstanceList == NULL)
     InstanceData(theEnv)->InstanceList = InstanceData(theEnv)->CurrentInstance;
   else
     InstanceData(theEnv)->InstanceListBottom->nxtList = InstanceData(theEnv)->CurrentInstance;
   InstanceData(theEnv)->CurrentInstance->prvList = InstanceData(theEnv)->InstanceListBottom;
   InstanceData(theEnv)->InstanceListBottom = InstanceData(theEnv)->CurrentInstance;
   InstanceData(theEnv)->ChangesToInstances = TRUE;

   /* ==============================================================================
      Install the instance's name and slot-value symbols (prevent them from becoming
      ephemeral) - the class name and slot names are accounted for by the class
      ============================================================================== */
   InstallInstance(theEnv,InstanceData(theEnv)->CurrentInstance,TRUE);

   ins = InstanceData(theEnv)->CurrentInstance;
   InstanceData(theEnv)->CurrentInstance = NULL;

   if (InstanceData(theEnv)->MkInsMsgPass)
     { DirectMessage(theEnv,MessageHandlerData(theEnv)->CREATE_SYMBOL,ins,&temp,NULL); }

#if DEFRULE_CONSTRUCT
   if (ins->cls->reactive)
     ObjectNetworkAction(theEnv,OBJECT_ASSERT,(INSTANCE_TYPE *) ins,-1);
#endif

   return(ins);
  }

/*****************************************************************************
  NAME         : InitSlotsCommand
  DESCRIPTION  : Calls Kernel Expression Evaluator EvaluateExpression
                   for each expression-value of an instance expression

                 Evaluates default slots only - slots that were specified
                 by overrides (sp->override == 1) are ignored)
  INPUTS       : 1) Instance address
  RETURNS      : Nothing useful
  SIDE EFFECTS : Each DATA_OBJECT slot in the instance's slot array is replaced
                   by the evaluation (by EvaluateExpression) of the expression
                   in the slot list.  The old expression-values
                   are deleted.
  NOTES        : H/L Syntax: (init-slots <instance>)
 *****************************************************************************/
globle void InitSlotsCommand(
  void *theEnv,
  DATA_OBJECT *result)
  {
   SetpType(result,SYMBOL);
   SetpValue(result,EnvFalseSymbol(theEnv));
   EvaluationData(theEnv)->EvaluationError = FALSE;
   if (CheckCurrentMessage(theEnv,"init-slots",TRUE) == FALSE)
     return;
   EvaluateClassDefaults(theEnv,GetActiveInstance(theEnv));
   if (! EvaluationData(theEnv)->EvaluationError)
     {
      SetpType(result,INSTANCE_ADDRESS);
      SetpValue(result,(void *) GetActiveInstance(theEnv));
     }
  }

/******************************************************
  NAME         : QuashInstance
  DESCRIPTION  : Deletes an instance if it is not in
                   use, otherwise sticks it on the
                   garbage list
  INPUTS       : The instance
  RETURNS      : 1 if successful, 0 otherwise
  SIDE EFFECTS : Instance deleted or added to garbage
  NOTES        : Even though the instance is removed
                   from the class list, hash table and
                   instance list, its links remain
                   unchanged so that outside loops
                   can still determine where the next
                   node in the list is (assuming the
                   instance was garbage collected).
 ******************************************************/
globle intBool QuashInstance(
  void *theEnv,
  INSTANCE_TYPE *ins)
  {
   register int iflag;
   IGARBAGE *gptr;

#if DEFRULE_CONSTRUCT
   if (EngineData(theEnv)->JoinOperationInProgress && ins->cls->reactive)
     {
      PrintErrorID(theEnv,"INSMNGR",12,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Cannot delete instances of reactive classes while\n");
      EnvPrintRouter(theEnv,WERROR,"  pattern-matching is in process.\n");
      SetEvaluationError(theEnv,TRUE);
      return(0);
     }
#endif
   if (ins->garbage == 1)
     return(0);
   if (ins->installed == 0)
     {
      PrintErrorID(theEnv,"INSMNGR",6,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Cannot delete instance ");
      EnvPrintRouter(theEnv,WERROR,ValueToString(ins->name));
      EnvPrintRouter(theEnv,WERROR," during initialization.\n");
      SetEvaluationError(theEnv,TRUE);
      return(0);
     }
#if DEBUGGING_FUNCTIONS
   if (ins->cls->traceInstances)
     PrintInstanceWatch(theEnv,UNMAKE_TRACE,ins);
#endif

#if DEFRULE_CONSTRUCT
   RemoveEntityDependencies(theEnv,(struct patternEntity *) ins);

   if (ins->cls->reactive)
     ObjectNetworkAction(theEnv,OBJECT_RETRACT,(INSTANCE_TYPE *) ins,-1);
#endif

   if (ins->prvHash != NULL)
     ins->prvHash->nxtHash = ins->nxtHash;
   else
     InstanceData(theEnv)->InstanceTable[ins->hashTableIndex] = ins->nxtHash;
   if (ins->nxtHash != NULL)
     ins->nxtHash->prvHash = ins->prvHash;

   if (ins->prvClass != NULL)
     ins->prvClass->nxtClass = ins->nxtClass;
   else
     ins->cls->instanceList = ins->nxtClass;
   if (ins->nxtClass != NULL)
     ins->nxtClass->prvClass = ins->prvClass;
   else
     ins->cls->instanceListBottom = ins->prvClass;

   if (ins->prvList != NULL)
     ins->prvList->nxtList = ins->nxtList;
   else
     InstanceData(theEnv)->InstanceList = ins->nxtList;
   if (ins->nxtList != NULL)
     ins->nxtList->prvList = ins->prvList;
   else
     InstanceData(theEnv)->InstanceListBottom = ins->prvList;

   iflag = ins->installed;
   InstallInstance(theEnv,ins,FALSE);

   /* ==============================================
      If the instance is the basis for an executing
      rule, don't bother deleting its slots yet, for
      they may still be needed by pattern variables
      ============================================== */
   if ((iflag == 1)
#if DEFRULE_CONSTRUCT
       && (ins->header.busyCount == 0)
#endif
     )
     RemoveInstanceData(theEnv,ins);

   if ((ins->busy == 0) && (ins->depth > EvaluationData(theEnv)->CurrentEvaluationDepth) &&
       (InstanceData(theEnv)->MaintainGarbageInstances == FALSE)
#if DEFRULE_CONSTRUCT
        && (ins->header.busyCount == 0)
#endif
       )
     {
      DecrementSymbolCount(theEnv,ins->name);
      rtn_struct(theEnv,instance,ins);
     }
   else
     {
      gptr = get_struct(theEnv,igarbage);
      ins->garbage = 1;
      gptr->ins = ins;
      gptr->nxt = InstanceData(theEnv)->InstanceGarbageList;
      InstanceData(theEnv)->InstanceGarbageList = gptr;
      UtilityData(theEnv)->EphemeralItemCount += 2;
      UtilityData(theEnv)->EphemeralItemSize += InstanceSizeHeuristic(ins) + sizeof(IGARBAGE);
     }
   InstanceData(theEnv)->ChangesToInstances = TRUE;
   return(1);
  }


#if DEFRULE_CONSTRUCT

/****************************************************
  NAME         : InactiveInitializeInstance
  DESCRIPTION  : Initializes an instance of a class
                 Pattern-matching is automatically
                 delayed until the instance is
                 completely initialized
  INPUTS       : The address of the result value
  RETURNS      : Nothing useful
  SIDE EFFECTS : Instance intialized
  NOTES        : H/L Syntax:
                 (initialize-instance <instance-name>
                    <slot-override>*)
 ****************************************************/
globle void InactiveInitializeInstance(
  void *theEnv,
  DATA_OBJECT *result)
  {
   int ov;

   ov = SetDelayObjectPatternMatching(theEnv,TRUE);
   InitializeInstanceCommand(theEnv,result);
   SetDelayObjectPatternMatching(theEnv,ov);
  }

/**************************************************************
  NAME         : InactiveMakeInstance
  DESCRIPTION  : Creates and initializes an instance of a class
                 Pattern-matching is automatically
                 delayed until the instance is
                 completely initialized
  INPUTS       : The address of the result value
  RETURNS      : Nothing useful
  SIDE EFFECTS : Instance intialized
  NOTES        : H/L Syntax:
                 (make-instance <instance-name> of <class>
                    <slot-override>*)
 **************************************************************/
globle void InactiveMakeInstance(
  void *theEnv,
  DATA_OBJECT *result)
  {
   int ov;

   ov = SetDelayObjectPatternMatching(theEnv,TRUE);
   MakeInstanceCommand(theEnv,result);
   SetDelayObjectPatternMatching(theEnv,ov);
  }

#endif

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/********************************************************
  NAME         : NewInstance
  DESCRIPTION  : Allocates and initializes a new instance
  INPUTS       : None
  RETURNS      : The address of the new instance
  SIDE EFFECTS : None
  NOTES        : None
 ********************************************************/
static INSTANCE_TYPE *NewInstance(
  void *theEnv)
  {
   INSTANCE_TYPE *instance;

   instance = get_struct(theEnv,instance);
#if DEFRULE_CONSTRUCT
   instance->header.theInfo = &InstanceData(theEnv)->InstanceInfo;

   instance->header.dependents = NULL;
   instance->header.busyCount = 0;
   instance->header.timeTag = 0L;

   instance->partialMatchList = NULL;
   instance->basisSlots = NULL;
   instance->reteSynchronized = FALSE;
#endif
   instance->busy = 0;
   instance->installed = 0;
   instance->garbage = 0;
   instance->initSlotsCalled = 0;
   instance->initializeInProgress = 0;
   instance->depth = EvaluationData(theEnv)->CurrentEvaluationDepth;
   instance->name = NULL;
   instance->hashTableIndex = 0;
   instance->cls = NULL;
   instance->slots = NULL;
   instance->slotAddresses = NULL;
   instance->prvClass = NULL;
   instance->nxtClass = NULL;
   instance->prvHash = NULL;
   instance->nxtHash = NULL;
   instance->prvList = NULL;
   instance->nxtList = NULL;
   return(instance);
  }

/*****************************************************************
  NAME         : InstanceLocationInfo
  DESCRIPTION  : Determines where a specified instance belongs
                   in the instance hash table
  INPUTS       : 1) The class of the new instance
                 2) The symbol for the name of the instance
                 3) Caller's buffer for previous node address
                 4) Caller's buffer for hash value
  RETURNS      : The address of the found instance, NULL otherwise
  SIDE EFFECTS : None
  NOTES        : Instance names only have to be unique within
                 a module
 *****************************************************************/
static INSTANCE_TYPE *InstanceLocationInfo(
  void *theEnv,
  DEFCLASS *cls,
  SYMBOL_HN *iname,
  INSTANCE_TYPE **prv,
  unsigned *hashTableIndex)
  {
   INSTANCE_TYPE *ins;

   *hashTableIndex = HashInstance(iname);
   ins = InstanceData(theEnv)->InstanceTable[*hashTableIndex];

   /* ========================================
      Make sure all instances of the same name
      are grouped together regardless of what
      module their classes are in
      ======================================== */
   *prv = NULL;
   while ((ins != NULL) ? (ins->name != iname) : FALSE)
     {
      *prv = ins;
      ins = ins->nxtHash;
     }
   while ((ins != NULL) ? (ins->name == iname) : FALSE)
     {
      if (ins->cls->header.whichModule->theModule ==
          cls->header.whichModule->theModule)
        return(ins);
      *prv = ins;
      ins = ins->nxtHash;
     }
   return(NULL);
  }

/********************************************************
  NAME         : InstallInstance
  DESCRIPTION  : Prevent name and slot value symbols
                   from being ephemeral (all others
                   taken care of by class defn)
  INPUTS       : 1) The address of the instance
                 2) A flag indicating whether to
                    install or deinstall
  RETURNS      : Nothing useful
  SIDE EFFECTS : Symbol counts incremented or decremented
  NOTES        : Slot symbol installations are handled
                   by PutSlotValue
 ********************************************************/
static void InstallInstance(
  void *theEnv,
  INSTANCE_TYPE *ins,
  int set)
  {
   if (set == TRUE)
     {
      if (ins->installed)
        return;
#if DEBUGGING_FUNCTIONS
      if (ins->cls->traceInstances)
        PrintInstanceWatch(theEnv,MAKE_TRACE,ins);
#endif
      ins->installed = 1;
      ins->depth = EvaluationData(theEnv)->CurrentEvaluationDepth;
      IncrementSymbolCount(ins->name);
      IncrementDefclassBusyCount(theEnv,(void *) ins->cls);
      InstanceData(theEnv)->GlobalNumberOfInstances++;
     }
   else
     {
      if (! ins->installed)
        return;
      ins->installed = 0;
      InstanceData(theEnv)->GlobalNumberOfInstances--;

      /* =======================================
         Class counts is decremented by
         RemoveInstanceData() when slot data is
         truly deleted - and name count is
         deleted by CleanupInstances() or
         QuashInstance() when instance is
         truly deleted
         ======================================= */
     }
  }

/****************************************************************
  NAME         : BuildDefaultSlots
  DESCRIPTION  : The current instance's address is
                   in the global variable CurrentInstance.
                   This builds the slots and the default values
                   from the direct class of the instance and its
                   inheritances.
  INPUTS       : Flag indicating whether init message will be
                 called for this instance or not
  RETURNS      : Nothing useful
  SIDE EFFECTS : Allocates the slot array for
                   the current instance
  NOTES        : The current instance's address is
                 stored in a global variable
 ****************************************************************/
static void BuildDefaultSlots(
  void *theEnv,
  intBool initMessage)
  {
   register unsigned i,j;
   unsigned scnt;
   unsigned lscnt;
   INSTANCE_SLOT *dst = NULL,**adst;
   SLOT_DESC **src;

   scnt = InstanceData(theEnv)->CurrentInstance->cls->instanceSlotCount;
   lscnt = InstanceData(theEnv)->CurrentInstance->cls->localInstanceSlotCount;
   if (scnt > 0)
     {
      InstanceData(theEnv)->CurrentInstance->slotAddresses = adst =
         (INSTANCE_SLOT **) gm2(theEnv,(sizeof(INSTANCE_SLOT *) * scnt));
      if (lscnt != 0)
        InstanceData(theEnv)->CurrentInstance->slots = dst =
           (INSTANCE_SLOT *) gm2(theEnv,(sizeof(INSTANCE_SLOT) * lscnt));
      src = InstanceData(theEnv)->CurrentInstance->cls->instanceTemplate;

      /* ==================================================
         A map of slot addresses is created - shared slots
         point at values in the class, and local slots
         point at values in the instance

         Also - slots are always given an initial value
         (since slots cannot be unbound). If there is
         already an instance of a class with a shared slot,
         that value is left alone
         ================================================== */
      for (i = 0 , j = 0 ; i < scnt ; i++)
        {
         if (src[i]->shared)
           {
            src[i]->sharedCount++;
            adst[i] = &(src[i]->sharedValue);
           }
         else
           {
            dst[j].desc = src[i];
            dst[j].value = NULL;
            adst[i] = &dst[j++];
           }
         if (adst[i]->value == NULL)
           {
            adst[i]->valueRequired = initMessage;
            if (adst[i]->desc->multiple)
              {
               adst[i]->type = MULTIFIELD;
               adst[i]->value = CreateMultifield2(theEnv,0L);
               MultifieldInstall(theEnv,(MULTIFIELD_PTR) adst[i]->value);
              }
            else
              {
               adst[i]->type = SYMBOL;
               adst[i]->value = EnvAddSymbol(theEnv,"nil");
               AtomInstall(theEnv,(int) adst[i]->type,adst[i]->value);
              }
           }
         else
           adst[i]->valueRequired = FALSE;
         adst[i]->override = FALSE;
        }
     }
  }

/*******************************************************************
  NAME         : CoreInitializeInstance
  DESCRIPTION  : Performs the core work for initializing an instance
  INPUTS       : 1) The instance address
                 2) Slot override expressions
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : EvaluationError set on errors - slots evaluated
  NOTES        : None
 *******************************************************************/
static int CoreInitializeInstance(
  void *theEnv,
  INSTANCE_TYPE *ins,
  EXPRESSION *ovrexp)
  {
   DATA_OBJECT temp;

   if (ins->installed == 0)
     {
      PrintErrorID(theEnv,"INSMNGR",7,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Instance ");
      EnvPrintRouter(theEnv,WERROR,ValueToString(ins->name));
      EnvPrintRouter(theEnv,WERROR," is already being initialized.\n");
      SetEvaluationError(theEnv,TRUE);
      return(FALSE);
     }

   /* =======================================================
      Replace all default-slot values with any slot-overrides
      ======================================================= */
   ins->busy++;
   ins->installed = 0;

   /* =================================================================
      If the slots are initialized properly - the initializeInProgress
      flag will be turned off.
      ================================================================= */
   ins->initializeInProgress = 1;
   ins->initSlotsCalled = 0;

   if (InsertSlotOverrides(theEnv,ins,ovrexp) == FALSE)
      {
       ins->installed = 1;
       ins->busy--;
       return(FALSE);
      }

   /* =================================================================
      Now that all the slot expressions are established - replace them
      with their evaluation
      ================================================================= */

   if (InstanceData(theEnv)->MkInsMsgPass)
     DirectMessage(theEnv,MessageHandlerData(theEnv)->INIT_SYMBOL,ins,&temp,NULL);
   else
     EvaluateClassDefaults(theEnv,ins);

   ins->busy--;
   ins->installed = 1;
   if (EvaluationData(theEnv)->EvaluationError)
     {
      PrintErrorID(theEnv,"INSMNGR",8,FALSE);
      EnvPrintRouter(theEnv,WERROR,"An error occurred during the initialization of instance ");
      EnvPrintRouter(theEnv,WERROR,ValueToString(ins->name));
      EnvPrintRouter(theEnv,WERROR,".\n");
      return(FALSE);
     }
     
   ins->initializeInProgress = 0;
   return((ins->initSlotsCalled == 0) ? FALSE : TRUE);
  }

/**********************************************************
  NAME         : InsertSlotOverrides
  DESCRIPTION  : Replaces value-expression for a slot
  INPUTS       : 1) The instance address
                 2) The address of the beginning of the
                    list of slot-expressions
  RETURNS      : TRUE if all okay, FALSE otherwise
  SIDE EFFECTS : Old slot expression deallocated
  NOTES        : Assumes symbols not yet installed
                 EVALUATES the slot-name expression but
                    simply copies the slot value-expression
 **********************************************************/
static int InsertSlotOverrides(
  void *theEnv,
  INSTANCE_TYPE *ins,
  EXPRESSION *slot_exp)
  {
   INSTANCE_SLOT *slot;
   DATA_OBJECT temp,junk;

   EvaluationData(theEnv)->EvaluationError = FALSE;
   while (slot_exp != NULL)
     {
      if ((EvaluateExpression(theEnv,slot_exp,&temp) == TRUE) ? TRUE :
          (GetType(temp) != SYMBOL))
        {
         PrintErrorID(theEnv,"INSMNGR",9,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Expected a valid slot name for slot-override.\n");
         SetEvaluationError(theEnv,TRUE);
         return(FALSE);
        }
      slot = FindInstanceSlot(theEnv,ins,(SYMBOL_HN *) GetValue(temp));
      if (slot == NULL)
        {
         PrintErrorID(theEnv,"INSMNGR",13,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Slot ");
         EnvPrintRouter(theEnv,WERROR,DOToString(temp));
         EnvPrintRouter(theEnv,WERROR," does not exist in instance ");
         EnvPrintRouter(theEnv,WERROR,ValueToString(ins->name));
         EnvPrintRouter(theEnv,WERROR,".\n");
         SetEvaluationError(theEnv,TRUE);
         return(FALSE);
        }

      if (InstanceData(theEnv)->MkInsMsgPass)
        { DirectMessage(theEnv,slot->desc->overrideMessage,
                       ins,NULL,slot_exp->nextArg->argList); }
      else if (slot_exp->nextArg->argList)
        {
         if (EvaluateAndStoreInDataObject(theEnv,(int) slot->desc->multiple,
                               slot_exp->nextArg->argList,&temp,TRUE))
             PutSlotValue(theEnv,ins,slot,&temp,&junk,"function make-instance");
        }
      else
        {
         SetpDOBegin(&temp,1);
         SetpDOEnd(&temp,0);
         SetpType(&temp,MULTIFIELD);
         SetpValue(&temp,ProceduralPrimitiveData(theEnv)->NoParamValue);
         PutSlotValue(theEnv,ins,slot,&temp,&junk,"function make-instance");
        }

      if (EvaluationData(theEnv)->EvaluationError)
        return(FALSE);
      slot->override = TRUE;
      slot_exp = slot_exp->nextArg->nextArg;
     }
   return(TRUE);
  }

/*****************************************************************************
  NAME         : EvaluateClassDefaults
  DESCRIPTION  : Evaluates default slots only - slots that were specified
                 by overrides (sp->override == 1) are ignored)
  INPUTS       : 1) Instance address
  RETURNS      : Nothing useful
  SIDE EFFECTS : Each DATA_OBJECT slot in the instance's slot array is replaced
                   by the evaluation (by EvaluateExpression) of the expression
                   in the slot list.  The old expression-values
                   are deleted.
  NOTES        : None
 *****************************************************************************/
static void EvaluateClassDefaults(
  void *theEnv,
  INSTANCE_TYPE *ins)
  {
   INSTANCE_SLOT *slot;
   DATA_OBJECT temp,junk;
   long i;

   if (ins->initializeInProgress == 0)
     {
      PrintErrorID(theEnv,"INSMNGR",15,FALSE);
      SetEvaluationError(theEnv,TRUE);
      EnvPrintRouter(theEnv,WERROR,"init-slots not valid in this context.\n");
      return;
     }
   for (i = 0 ; i < ins->cls->instanceSlotCount ; i++)
     {
      slot = ins->slotAddresses[i];

      /* ===========================================================
         Slot-overrides are just a short-hand for put-slots, so they
         should be done with messages.  Defaults are from the class
         definition and can be placed directly.
         =========================================================== */
      if (!slot->override)
        {
         if (slot->desc->dynamicDefault)
           {
            if (EvaluateAndStoreInDataObject(theEnv,(int) slot->desc->multiple,
                                             (EXPRESSION *) slot->desc->defaultValue,
                                             &temp,TRUE))
              PutSlotValue(theEnv,ins,slot,&temp,&junk,"function init-slots");
           }
         else if (((slot->desc->shared == 0) || (slot->desc->sharedCount == 1)) &&
                  (slot->desc->noDefault == 0))
           DirectPutSlotValue(theEnv,ins,slot,(DATA_OBJECT *) slot->desc->defaultValue,&junk);
         else if (slot->valueRequired)
           {
            PrintErrorID(theEnv,"INSMNGR",14,FALSE);
            EnvPrintRouter(theEnv,WERROR,"Override required for slot ");
            EnvPrintRouter(theEnv,WERROR,ValueToString(slot->desc->slotName->name));
            EnvPrintRouter(theEnv,WERROR," in instance ");
            EnvPrintRouter(theEnv,WERROR,ValueToString(ins->name));
            EnvPrintRouter(theEnv,WERROR,".\n");
            SetEvaluationError(theEnv,TRUE);
           }
         slot->valueRequired = FALSE;
         if (ins->garbage == 1)
           {
            EnvPrintRouter(theEnv,WERROR,ValueToString(ins->name));
            EnvPrintRouter(theEnv,WERROR," instance deleted by slot-override evaluation.\n");
            SetEvaluationError(theEnv,TRUE);
           }
         if (EvaluationData(theEnv)->EvaluationError)
            return;
        }
      slot->override = FALSE;
     }
   ins->initSlotsCalled = 1;
  }

#if DEBUGGING_FUNCTIONS

/***************************************************
  NAME         : PrintInstanceWatch
  DESCRIPTION  : Prints out a trace message for the
                 creation/deletion of an instance
  INPUTS       : 1) The trace string indicating if
                    this is a creation or deletion
                 2) The instance
  RETURNS      : Nothing usful
  SIDE EFFECTS : Watch message printed
  NOTES        : None
 ***************************************************/
static void PrintInstanceWatch(
  void *theEnv,
  char *traceString,
  INSTANCE_TYPE *theInstance)
  {
   EnvPrintRouter(theEnv,WTRACE,traceString);
   EnvPrintRouter(theEnv,WTRACE," instance ");
   PrintInstanceNameAndClass(theEnv,WTRACE,theInstance,TRUE);
  }

#endif

#endif



