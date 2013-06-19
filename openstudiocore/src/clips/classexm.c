   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  03/04/08            */
   /*                                                     */
   /*                 CLASS EXAMINATION MODULE            */
   /*******************************************************/

/**************************************************************/
/* Purpose: Class browsing and examination commands           */
/*                                                            */
/* Principal Programmer(s):                                   */
/*      Brian L. Dantes                                       */
/*                                                            */
/* Contributing Programmer(s):                                */
/*                                                            */
/* Revision History:                                          */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859   */
/*                                                            */
/*            Modified the slot-writablep function to return  */
/*            FALSE for slots having initialize-only access.  */
/*            DR0860                                          */
/*                                                            */
/*      6.24: Added allowed-classes slot facet.               */
/*                                                            */
/*            Converted INSTANCE_PATTERN_MATCHING to          */
/*            DEFRULE_CONSTRUCT.                              */
/*                                                            */
/*            Renamed BOOLEAN macro type to intBool.          */
/*                                                            */
/*            The slot-default-value function crashes when no */
/*            default exists for a slot (the ?NONE value was  */
/*            specified). DR0870                              */
/*                                                            */
/*      6.30: Used %zd for printing size_t arguments.         */
/*                                                            */
/*            Added EnvSlotDefaultP function.                 */
/*                                                            */
/**************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if OBJECT_SYSTEM

#include <string.h>

#include "argacces.h"
#include "classcom.h"
#include "classfun.h"
#include "classini.h"
#include "envrnmnt.h"
#include "insfun.h"
#include "memalloc.h"
#include "msgcom.h"
#include "msgfun.h"
#include "router.h"
#include "strngrtr.h"
#include "sysdep.h"

#define _CLASSEXM_SOURCE_
#include "classexm.h"

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static int CheckTwoClasses(void *,char *,DEFCLASS **,DEFCLASS **);
static SLOT_DESC *CheckSlotExists(void *,char *,DEFCLASS **,intBool,intBool);
static SLOT_DESC *LookupSlot(void *,DEFCLASS *,char *,intBool);

#if DEBUGGING_FUNCTIONS
static DEFCLASS *CheckClass(void *,char *,char *);
static char *GetClassNameArgument(void *,char *);
static void PrintClassBrowse(void *,char *,DEFCLASS *,long);
static void DisplaySeparator(void *,char *,char *,int,int);
static void DisplaySlotBasicInfo(void *,char *,char *,char *,char *,DEFCLASS *);
static intBool PrintSlotSources(void *,char *,SYMBOL_HN *,PACKED_CLASS_LINKS *,long,int);
static void DisplaySlotConstraintInfo(void *,char *,char *,char *,unsigned,DEFCLASS *);
static char *ConstraintCode(CONSTRAINT_RECORD *,unsigned,unsigned);
#endif

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

#if DEBUGGING_FUNCTIONS

/****************************************************************
  NAME         : BrowseClassesCommand
  DESCRIPTION  : Displays a "graph" of the class hierarchy
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : Syntax : (browse-classes [<class>])
 ****************************************************************/
globle void BrowseClassesCommand(
  void *theEnv)
  {
   register DEFCLASS *cls;
   
   if (EnvRtnArgCount(theEnv) == 0)
      /* ================================================
         Find the OBJECT root class (has no superclasses)
         ================================================ */
      cls = LookupDefclassByMdlOrScope(theEnv,OBJECT_TYPE_NAME);
   else
     {
      DATA_OBJECT tmp;

      if (EnvArgTypeCheck(theEnv,"browse-classes",1,SYMBOL,&tmp) == FALSE)
        return;
      cls = LookupDefclassByMdlOrScope(theEnv,DOToString(tmp));
      if (cls == NULL)
        {
         ClassExistError(theEnv,"browse-classes",DOToString(tmp));
         return;
        }
     }
   EnvBrowseClasses(theEnv,WDISPLAY,(void *) cls);
  }

/****************************************************************
  NAME         : EnvBrowseClasses
  DESCRIPTION  : Displays a "graph" of the class hierarchy
  INPUTS       : 1) The logical name of the output
                 2) Class pointer
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 ****************************************************************/
globle void EnvBrowseClasses(
  void *theEnv,
  char *logicalName,
  void *clsptr)
  {
   PrintClassBrowse(theEnv,logicalName,(DEFCLASS *) clsptr,0);
  }

/****************************************************************
  NAME         : DescribeClassCommand
  DESCRIPTION  : Displays direct superclasses and
                   subclasses and the entire precedence
                   list for a class
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : Syntax : (describe-class <class-name>)
 ****************************************************************/
globle void DescribeClassCommand(
  void *theEnv)
  {
   char *cname;
   DEFCLASS *cls;
   
   cname = GetClassNameArgument(theEnv,"describe-class");
   if (cname == NULL)
     return;
   cls = CheckClass(theEnv,"describe-class",cname);
   if (cls == NULL)
     return;
   EnvDescribeClass(theEnv,WDISPLAY,(void *) cls);
  }

/******************************************************
  NAME         : EnvDescribeClass
  DESCRIPTION  : Displays direct superclasses and
                   subclasses and the entire precedence
                   list for a class
  INPUTS       : 1) The logical name of the output
                 2) Class pointer
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 ******************************************************/
globle void EnvDescribeClass(
  void *theEnv,
  char *logicalName,
  void *clsptr)
  {
   DEFCLASS *cls;
   char buf[83],
        slotNamePrintFormat[12],
        overrideMessagePrintFormat[12];
   int messageBanner;
   long i;
   size_t slotNameLength, maxSlotNameLength;
   size_t overrideMessageLength, maxOverrideMessageLength;

   cls = (DEFCLASS *) clsptr;
   DisplaySeparator(theEnv,logicalName,buf,82,'=');
   DisplaySeparator(theEnv,logicalName,buf,82,'*');
   if (cls->abstract)
     EnvPrintRouter(theEnv,logicalName,"Abstract: direct instances of this class cannot be created.\n\n");
   else
     {
      EnvPrintRouter(theEnv,logicalName,"Concrete: direct instances of this class can be created.\n");
#if DEFRULE_CONSTRUCT
      if (cls->reactive)
        EnvPrintRouter(theEnv,logicalName,"Reactive: direct instances of this class can match defrule patterns.\n\n");
      else
        EnvPrintRouter(theEnv,logicalName,"Non-reactive: direct instances of this class cannot match defrule patterns.\n\n");
#else
      EnvPrintRouter(theEnv,logicalName,"\n");
#endif
     }
   PrintPackedClassLinks(theEnv,logicalName,"Direct Superclasses:",&cls->directSuperclasses);
   PrintPackedClassLinks(theEnv,logicalName,"Inheritance Precedence:",&cls->allSuperclasses);
   PrintPackedClassLinks(theEnv,logicalName,"Direct Subclasses:",&cls->directSubclasses);
   if (cls->instanceTemplate != NULL)
     {
      DisplaySeparator(theEnv,logicalName,buf,82,'-');
      maxSlotNameLength = 5;
      maxOverrideMessageLength = 8;
      for (i = 0 ; i < cls->instanceSlotCount ; i++)
        {
         slotNameLength = strlen(ValueToString(cls->instanceTemplate[i]->slotName->name));
         if (slotNameLength > maxSlotNameLength)
           maxSlotNameLength = slotNameLength;
         if (cls->instanceTemplate[i]->noWrite == 0)
           {
            overrideMessageLength =
              strlen(ValueToString(cls->instanceTemplate[i]->overrideMessage));
            if (overrideMessageLength > maxOverrideMessageLength)
              maxOverrideMessageLength = overrideMessageLength;
           }
        }
      if (maxSlotNameLength > 16)
        maxSlotNameLength = 16;
      if (maxOverrideMessageLength > 12)
        maxOverrideMessageLength = 12;
#if WIN_MVC || WIN_BTC
      gensprintf(slotNamePrintFormat,"%%-%Id.%Ids : ",maxSlotNameLength,maxSlotNameLength);
      gensprintf(overrideMessagePrintFormat,"%%-%Id.%Ids ",maxOverrideMessageLength,
                                              maxOverrideMessageLength);
#elif WIN_GCC
      gensprintf(slotNamePrintFormat,"%%-%ld.%lds : ",(long) maxSlotNameLength,(long) maxSlotNameLength);
      gensprintf(overrideMessagePrintFormat,"%%-%ld.%lds ",(long) maxOverrideMessageLength,
                                            (long) maxOverrideMessageLength);
#else
      gensprintf(slotNamePrintFormat,"%%-%zd.%zds : ",maxSlotNameLength,maxSlotNameLength);
      gensprintf(overrideMessagePrintFormat,"%%-%zd.%zds ",maxOverrideMessageLength,
                                              maxOverrideMessageLength);
#endif

      DisplaySlotBasicInfo(theEnv,logicalName,slotNamePrintFormat,overrideMessagePrintFormat,buf,cls);
      EnvPrintRouter(theEnv,logicalName,"\nConstraint information for slots:\n\n");
      DisplaySlotConstraintInfo(theEnv,logicalName,slotNamePrintFormat,buf,82,cls);
     }
   if (cls->handlerCount > 0)
     messageBanner = TRUE;
   else
     {
      messageBanner = FALSE;
      for (i = 1 ; i < cls->allSuperclasses.classCount ; i++)
        if (cls->allSuperclasses.classArray[i]->handlerCount > 0)
          {
           messageBanner = TRUE;
           break;
          }
     }
   if (messageBanner)
     {
      DisplaySeparator(theEnv,logicalName,buf,82,'-');
      EnvPrintRouter(theEnv,logicalName,"Recognized message-handlers:\n");
      DisplayHandlersInLinks(theEnv,logicalName,&cls->allSuperclasses,0);
     }
   DisplaySeparator(theEnv,logicalName,buf,82,'*');
   DisplaySeparator(theEnv,logicalName,buf,82,'=');
  }

#endif

/**********************************************************
  NAME         : GetCreateAccessorString
  DESCRIPTION  : Gets a string describing which
                 accessors are implicitly created
                 for a slot: R, W, RW or NIL
  INPUTS       : The slot descriptor
  RETURNS      : The string description
  SIDE EFFECTS : None
  NOTES        : Used by (describe-class) and (slot-facets)
 **********************************************************/
globle char *GetCreateAccessorString(
  void *vsd)
  {
   SLOT_DESC *sd = (SLOT_DESC *) vsd;

   if (sd->createReadAccessor && sd->createWriteAccessor)
     return("RW");
   if ((sd->createReadAccessor == 0) && (sd->createWriteAccessor == 0))
     return("NIL");
   else
     {
      if (sd->createReadAccessor) return "R";
      else return "W";
     }
  }

/************************************************************
  NAME         : GetDefclassModuleCommand
  DESCRIPTION  : Determines to which module a class belongs
  INPUTS       : None
  RETURNS      : The symbolic name of the module
  SIDE EFFECTS : None
  NOTES        : H/L Syntax: (defclass-module <class-name>)
 ************************************************************/
globle void *GetDefclassModuleCommand(
  void *theEnv)
  {
   return(GetConstructModuleCommand(theEnv,"defclass-module",DefclassData(theEnv)->DefclassConstruct));
  }

/*********************************************************************
  NAME         : SuperclassPCommand
  DESCRIPTION  : Determines if a class is a superclass of another
  INPUTS       : None
  RETURNS      : TRUE if class-1 is a superclass of class-2
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : (superclassp <class-1> <class-2>)
 *********************************************************************/
globle intBool SuperclassPCommand(
  void *theEnv)
  {
   DEFCLASS *c1,*c2;
   
   if (CheckTwoClasses(theEnv,"superclassp",&c1,&c2) == FALSE)
     return(FALSE);
   return(EnvSuperclassP(theEnv,(void *) c1,(void *) c2));
  }

/***************************************************
  NAME         : EnvSuperclassP
  DESCRIPTION  : Determines if the first class is
                 a superclass of the other
  INPUTS       : 1) First class
                 2) Second class
  RETURNS      : TRUE if first class is a
                 superclass of the first,
                 FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle intBool EnvSuperclassP(
  void *theEnv,
  void *firstClass,
  void *secondClass)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   return(HasSuperclass((DEFCLASS *) secondClass,(DEFCLASS *) firstClass));
  }

/*********************************************************************
  NAME         : SubclassPCommand
  DESCRIPTION  : Determines if a class is a subclass of another
  INPUTS       : None
  RETURNS      : TRUE if class-1 is a subclass of class-2
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : (subclassp <class-1> <class-2>)
 *********************************************************************/
globle intBool SubclassPCommand(
  void *theEnv)
  {
   DEFCLASS *c1,*c2;
   
   if (CheckTwoClasses(theEnv,"subclassp",&c1,&c2) == FALSE)
     return(FALSE);
   return(EnvSubclassP(theEnv,(void *) c1,(void *) c2));
  }

/***************************************************
  NAME         : EnvSubclassP
  DESCRIPTION  : Determines if the first class is
                 a subclass of the other
  INPUTS       : 1) First class
                 2) Second class
  RETURNS      : TRUE if first class is a
                 subclass of the first,
                 FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle intBool EnvSubclassP(
  void *theEnv,
  void *firstClass,
  void *secondClass)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   return(HasSuperclass((DEFCLASS *) firstClass,(DEFCLASS *) secondClass));
  }

/*********************************************************************
  NAME         : SlotExistPCommand
  DESCRIPTION  : Determines if a slot is present in a class
  INPUTS       : None
  RETURNS      : TRUE if the slot exists, FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : (slot-existp <class> <slot> [inherit])
 *********************************************************************/
globle int SlotExistPCommand(
  void *theEnv)
  {
   DEFCLASS *cls;
   SLOT_DESC *sd;
   int inheritFlag = FALSE;
   DATA_OBJECT dobj;
   
   sd = CheckSlotExists(theEnv,"slot-existp",&cls,FALSE,TRUE);
   if (sd == NULL)
     return(FALSE);
   if (EnvRtnArgCount(theEnv) == 3)
     {
      if (EnvArgTypeCheck(theEnv,"slot-existp",3,SYMBOL,&dobj) == FALSE)
        return(FALSE);
      if (strcmp(DOToString(dobj),"inherit") != 0)
        {
         ExpectedTypeError1(theEnv,"slot-existp",3,"keyword \"inherit\"");
         SetEvaluationError(theEnv,TRUE);
         return(FALSE);
        }
      inheritFlag = TRUE;
     }
   return((sd->cls == cls) ? TRUE : inheritFlag);
  }

/***************************************************
  NAME         : EnvSlotExistP
  DESCRIPTION  : Determines if a slot exists
  INPUTS       : 1) The class
                 2) The slot name
                 3) A flag indicating if the slot
                    can be inherited or not
  RETURNS      : TRUE if slot exists,
                 FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
globle intBool EnvSlotExistP(
  void *theEnv,
  void *theDefclass,
  char *slotName,
  intBool inheritFlag)
  {
   return((LookupSlot(theEnv,(DEFCLASS *) theDefclass,slotName,inheritFlag) != NULL)
           ? TRUE : FALSE);
  }

/************************************************************************************
  NAME         : MessageHandlerExistPCommand
  DESCRIPTION  : Determines if a message-handler is present in a class
  INPUTS       : None
  RETURNS      : TRUE if the message header is present, FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : (message-handler-existp <class> <hnd> [<type>])
 ************************************************************************************/
globle int MessageHandlerExistPCommand(
  void *theEnv)
  {
   DEFCLASS *cls;
   SYMBOL_HN *mname;
   DATA_OBJECT temp;
   unsigned mtype = MPRIMARY;
   
   if (EnvArgTypeCheck(theEnv,"message-handler-existp",1,SYMBOL,&temp) == FALSE)
     return(FALSE);
   cls = LookupDefclassByMdlOrScope(theEnv,DOToString(temp));
   if (cls == NULL)
     {
      ClassExistError(theEnv,"message-handler-existp",DOToString(temp));
      return(FALSE);
     }
   if (EnvArgTypeCheck(theEnv,"message-handler-existp",2,SYMBOL,&temp) == FALSE)
     return(FALSE);
   mname = (SYMBOL_HN *) GetValue(temp);
   if (EnvRtnArgCount(theEnv) == 3)
     {
      if (EnvArgTypeCheck(theEnv,"message-handler-existp",3,SYMBOL,&temp) == FALSE)
        return(FALSE);
      mtype = HandlerType(theEnv,"message-handler-existp",DOToString(temp));
      if (mtype == MERROR)
        {
         SetEvaluationError(theEnv,TRUE);
         return(FALSE);
        }
     }
   if (FindHandlerByAddress(cls,mname,mtype) != NULL)
     return(TRUE);
   return(FALSE);
  }

/**********************************************************************
  NAME         : SlotWritablePCommand
  DESCRIPTION  : Determines if an existing slot can be written to
  INPUTS       : None
  RETURNS      : TRUE if the slot is writable, FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : (slot-writablep <class> <slot>)
 **********************************************************************/
globle intBool SlotWritablePCommand(
  void *theEnv)
  {
   DEFCLASS *theDefclass;
   SLOT_DESC *sd;
   
   sd = CheckSlotExists(theEnv,"slot-writablep",&theDefclass,TRUE,TRUE);
   if (sd == NULL)
     return(FALSE);
   return((sd->noWrite || sd->initializeOnly) ? FALSE : TRUE);
  }

/***************************************************
  NAME         : EnvSlotWritableP
  DESCRIPTION  : Determines if a slot is writable
  INPUTS       : 1) The class
                 2) The slot name
  RETURNS      : TRUE if slot is writable,
                 FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
globle intBool EnvSlotWritableP(
  void *theEnv,
  void *theDefclass,
  char *slotName)
  {
   SLOT_DESC *sd;

   if ((sd = LookupSlot(theEnv,(DEFCLASS *) theDefclass,slotName,TRUE)) == NULL)
     return(FALSE);
   return((sd->noWrite || sd->initializeOnly) ? FALSE : TRUE);
  }

/**********************************************************************
  NAME         : SlotInitablePCommand
  DESCRIPTION  : Determines if an existing slot can be initialized
                   via an init message-handler or slot-override
  INPUTS       : None
  RETURNS      : TRUE if the slot is writable, FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : (slot-initablep <class> <slot>)
 **********************************************************************/
globle intBool SlotInitablePCommand(
  void *theEnv)
  {
   DEFCLASS *theDefclass;
   SLOT_DESC *sd;
   
   sd = CheckSlotExists(theEnv,"slot-initablep",&theDefclass,TRUE,TRUE);
   if (sd == NULL)
     return(FALSE);
   return((sd->noWrite && (sd->initializeOnly == 0)) ? FALSE : TRUE);
  }

/***************************************************
  NAME         : EnvSlotInitableP
  DESCRIPTION  : Determines if a slot is initable
  INPUTS       : 1) The class
                 2) The slot name
  RETURNS      : TRUE if slot is initable,
                 FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
globle intBool EnvSlotInitableP(
  void *theEnv,
  void *theDefclass,
  char *slotName)
  {
   SLOT_DESC *sd;

   if ((sd = LookupSlot(theEnv,(DEFCLASS *) theDefclass,slotName,TRUE)) == NULL)
     return(FALSE);
   return((sd->noWrite && (sd->initializeOnly == 0)) ? FALSE : TRUE);
  }

/**********************************************************************
  NAME         : SlotPublicPCommand
  DESCRIPTION  : Determines if an existing slot is publicly visible
                   for direct reference by subclasses
  INPUTS       : None
  RETURNS      : TRUE if the slot is public, FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : (slot-publicp <class> <slot>)
 **********************************************************************/
globle intBool SlotPublicPCommand(
  void *theEnv)
  {
   DEFCLASS *theDefclass;
   SLOT_DESC *sd;
   
   sd = CheckSlotExists(theEnv,"slot-publicp",&theDefclass,TRUE,FALSE);
   if (sd == NULL)
     return(FALSE);
   return(sd->publicVisibility ? TRUE : FALSE);
  }

/***************************************************
  NAME         : EnvSlotPublicP
  DESCRIPTION  : Determines if a slot is public
  INPUTS       : 1) The class
                 2) The slot name
  RETURNS      : TRUE if slot is public,
                 FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
globle intBool EnvSlotPublicP(
  void *theEnv,
  void *theDefclass,
  char *slotName)
  {
   SLOT_DESC *sd;

   if ((sd = LookupSlot(theEnv,(DEFCLASS *) theDefclass,slotName,FALSE)) == NULL)
     return(FALSE);
   return(sd->publicVisibility ? TRUE : FALSE);
  }

/***************************************************
  NAME         : EnvSlotDefaultP
  DESCRIPTION  : Determines if a slot has a default value
  INPUTS       : 1) The class
                 2) The slot name
  RETURNS      : TRUE if slot is public,
                 FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
globle int EnvSlotDefaultP(
  void *theEnv,
  void *theDefclass,
  char *slotName)
  {
   SLOT_DESC *sd;

   if ((sd = LookupSlot(theEnv,(DEFCLASS *) theDefclass,slotName,FALSE)) == NULL)
     return(NO_DEFAULT);
     
   if (sd->noDefault)
     { return(NO_DEFAULT); }
   else if (sd->dynamicDefault)
     { return(DYNAMIC_DEFAULT); }
   
   return(STATIC_DEFAULT);
  }
  
  
/**********************************************************************
  NAME         : SlotDirectAccessPCommand
  DESCRIPTION  : Determines if an existing slot can be directly
                   referenced by the class - i.e., if the slot is
                   private, is the slot defined in the class
  INPUTS       : None
  RETURNS      : TRUE if the slot is private,
                    FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : (slot-direct-accessp <class> <slot>)
 **********************************************************************/
globle intBool SlotDirectAccessPCommand(
  void *theEnv)
  {
   DEFCLASS *theDefclass;
   SLOT_DESC *sd;
   
   sd = CheckSlotExists(theEnv,"slot-direct-accessp",&theDefclass,TRUE,TRUE);
   if (sd == NULL)
     return(FALSE);
   return((sd->publicVisibility || (sd->cls == theDefclass)) ? TRUE : FALSE);
  }

/***************************************************
  NAME         : EnvSlotDirectAccessP
  DESCRIPTION  : Determines if a slot is directly
                 accessible from message-handlers
                 on class
  INPUTS       : 1) The class
                 2) The slot name
  RETURNS      : TRUE if slot is directly
                 accessible, FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
globle intBool EnvSlotDirectAccessP(
  void *theEnv,
  void *theDefclass,
  char *slotName)
  {
   SLOT_DESC *sd;

   if ((sd = LookupSlot(theEnv,(DEFCLASS *) theDefclass,slotName,TRUE)) == NULL)
     return(FALSE);
   return((sd->publicVisibility || (sd->cls == (DEFCLASS *) theDefclass)) ?
           TRUE : FALSE);
  }

/**********************************************************************
  NAME         : SlotDefaultValueCommand
  DESCRIPTION  : Determines the default avlue for the specified slot
                 of the specified class
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : (slot-default-value <class> <slot>)
 **********************************************************************/
globle void SlotDefaultValueCommand(
  void *theEnv,
  DATA_OBJECT_PTR theValue)
  {
   DEFCLASS *theDefclass;
   SLOT_DESC *sd;

   SetpType(theValue,SYMBOL);
   SetpValue(theValue,EnvFalseSymbol(theEnv));
   sd = CheckSlotExists(theEnv,"slot-default-value",&theDefclass,TRUE,TRUE);
   if (sd == NULL)
     return;
   
   if (sd->noDefault)
     {
      SetpType(theValue,SYMBOL);
      SetpValue(theValue,EnvAddSymbol(theEnv,"?NONE"));
      return; 
     }
     
   if (sd->dynamicDefault)
     EvaluateAndStoreInDataObject(theEnv,(int) sd->multiple,
                                  (EXPRESSION *) sd->defaultValue,
                                  theValue,TRUE);
   else
     GenCopyMemory(DATA_OBJECT,1,theValue,sd->defaultValue);
  }

/*********************************************************
  NAME         : SlotDefaultValue
  DESCRIPTION  : Determines the default value for
                 the specified slot of the specified class
  INPUTS       : 1) The class
                 2) The slot name
  RETURNS      : TRUE if slot default value is set,
                 FALSE otherwise
  SIDE EFFECTS : Slot default value evaluated - dynamic
                 defaults will cause any side effects
  NOTES        : None
 *********************************************************/
globle intBool EnvSlotDefaultValue(
  void *theEnv,
  void *theDefclass,
  char *slotName,
  DATA_OBJECT_PTR theValue)
  {
   SLOT_DESC *sd;

   SetpType(theValue,SYMBOL);
   SetpValue(theValue,EnvFalseSymbol(theEnv));
   if ((sd = LookupSlot(theEnv,(DEFCLASS *) theDefclass,slotName,TRUE)) == NULL)
     return(FALSE);
   
   if (sd->noDefault)
     {
      SetpType(theValue,SYMBOL);
      SetpValue(theValue,EnvAddSymbol(theEnv,"?NONE"));
      return(TRUE); 
     }
     
   if (sd->dynamicDefault)
     return(EvaluateAndStoreInDataObject(theEnv,(int) sd->multiple,
                                         (EXPRESSION *) sd->defaultValue,
                                         theValue,TRUE));
   GenCopyMemory(DATA_OBJECT,1,theValue,sd->defaultValue);
   return(TRUE);
  }

/********************************************************
  NAME         : ClassExistPCommand
  DESCRIPTION  : Determines if a class exists
  INPUTS       : None
  RETURNS      : TRUE if class exists, FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : H/L Syntax : (class-existp <arg>)
 ********************************************************/
globle intBool ClassExistPCommand(
  void *theEnv)
  {
   DATA_OBJECT temp;
   
   if (EnvArgTypeCheck(theEnv,"class-existp",1,SYMBOL,&temp) == FALSE)
     return(FALSE);
   return((LookupDefclassByMdlOrScope(theEnv,DOToString(temp)) != NULL) ? TRUE : FALSE);
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/******************************************************
  NAME         : CheckTwoClasses
  DESCRIPTION  : Checks for exactly two class arguments
                    for a H/L function
  INPUTS       : 1) The function name
                 2) Caller's buffer for first class
                 3) Caller's buffer for second class
  RETURNS      : TRUE if both found, FALSE otherwise
  SIDE EFFECTS : Caller's buffers set
  NOTES        : Assumes exactly 2 arguments
 ******************************************************/
static int CheckTwoClasses(
  void *theEnv,
  char *func,
  DEFCLASS **c1,
  DEFCLASS **c2)
  {
   DATA_OBJECT temp;

   if (EnvArgTypeCheck(theEnv,func,1,SYMBOL,&temp) == FALSE)
     return(FALSE);
   *c1 = LookupDefclassByMdlOrScope(theEnv,DOToString(temp));
   if (*c1 == NULL)
     {
      ClassExistError(theEnv,func,ValueToString(temp.value));
      return(FALSE);
     }
   if (EnvArgTypeCheck(theEnv,func,2,SYMBOL,&temp) == FALSE)
     return(FALSE);
   *c2 = LookupDefclassByMdlOrScope(theEnv,DOToString(temp));
   if (*c2 == NULL)
     {
      ClassExistError(theEnv,func,ValueToString(temp.value));
      return(FALSE);
     }
   return(TRUE);
  }

/***************************************************
  NAME         : CheckSlotExists
  DESCRIPTION  : Checks first two arguments of
                 a function for a valid class
                 and (inherited) slot
  INPUTS       : 1) The name of the function
                 2) A buffer to hold the found class
                 3) A flag indicating whether the
                    non-existence of the slot should
                    be an error
                 4) A flag indicating if the slot
                    can be inherited or not
  RETURNS      : NULL if slot not found, slot
                 descriptor otherwise
  SIDE EFFECTS : Class buffer set if no errors,
                 NULL on errors
  NOTES        : None
 ***************************************************/
static SLOT_DESC *CheckSlotExists(
  void *theEnv,
  char *func,
  DEFCLASS **classBuffer,
  intBool existsErrorFlag,
  intBool inheritFlag)
  {
   SYMBOL_HN *ssym;
   int slotIndex;
   SLOT_DESC *sd;

   ssym = CheckClassAndSlot(theEnv,func,classBuffer);
   if (ssym == NULL)
     return(NULL);
   slotIndex = FindInstanceTemplateSlot(theEnv,*classBuffer,ssym);
   if (slotIndex == -1)
     {
      if (existsErrorFlag)
        {
         SlotExistError(theEnv,ValueToString(ssym),func);
         SetEvaluationError(theEnv,TRUE);
        }
      return(NULL);
     }
   sd = (*classBuffer)->instanceTemplate[slotIndex];
   if ((sd->cls == *classBuffer) || inheritFlag)
     return(sd);
   PrintErrorID(theEnv,"CLASSEXM",1,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Inherited slot ");
   EnvPrintRouter(theEnv,WERROR,ValueToString(ssym));
   EnvPrintRouter(theEnv,WERROR," from class ");
   PrintClassName(theEnv,WERROR,sd->cls,FALSE);
   EnvPrintRouter(theEnv,WERROR," is not valid for function ");
   EnvPrintRouter(theEnv,WERROR,func);
   EnvPrintRouter(theEnv,WERROR,"\n");
   SetEvaluationError(theEnv,TRUE);
   return(NULL);
  }

/***************************************************
  NAME         : LookupSlot
  DESCRIPTION  : Finds a slot in a class
  INPUTS       : 1) The class
                 2) The slot name
                 3) A flag indicating if inherited
                    slots are OK or not
  RETURNS      : The slot descriptor address, or
                 NULL if not found
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
static SLOT_DESC *LookupSlot(
  void *theEnv,
  DEFCLASS *theDefclass,
  char *slotName,
  intBool inheritFlag)
  {
   SYMBOL_HN *slotSymbol;
   int slotIndex;
   SLOT_DESC *sd;

   slotSymbol = FindSymbolHN(theEnv,slotName);
   if (slotSymbol == NULL)
     return(NULL);
   slotIndex = FindInstanceTemplateSlot(theEnv,theDefclass,slotSymbol);
   if (slotIndex == -1)
     return(NULL);
   sd = theDefclass->instanceTemplate[slotIndex];
   if ((sd->cls != theDefclass) && (inheritFlag == FALSE))
     return(NULL);
   return(sd);
  }

#if DEBUGGING_FUNCTIONS

/*****************************************************
  NAME         : CheckClass
  DESCRIPTION  : Used for to check class name for
                 class accessor functions such
                 as ppdefclass and undefclass
  INPUTS       : 1) The name of the H/L function
                 2) Name of the class
  RETURNS      : The class address,
                   or NULL if ther was an error
  SIDE EFFECTS : None
  NOTES        : None
 ******************************************************/
static DEFCLASS *CheckClass(
  void *theEnv,
  char *func,
  char *cname)
  {
   DEFCLASS *cls;

   cls = LookupDefclassByMdlOrScope(theEnv,cname);
   if (cls == NULL)
     ClassExistError(theEnv,func,cname);
   return(cls);
  }

/*********************************************************
  NAME         : GetClassNameArgument
  DESCRIPTION  : Gets a class name-string
  INPUTS       : Calling function name
  RETURNS      : Class name (NULL on errors)
  SIDE EFFECTS : None
  NOTES        : Assumes only 1 argument
 *********************************************************/
static char *GetClassNameArgument(
  void *theEnv,
  char *fname)
  {
   DATA_OBJECT temp;

   if (EnvArgTypeCheck(theEnv,fname,1,SYMBOL,&temp) == FALSE)
     return(NULL);
   return(DOToString(temp));
  }

/****************************************************************
  NAME         : PrintClassBrowse
  DESCRIPTION  : Displays a "graph" of class and subclasses
  INPUTS       : 1) The logical name of the output
                 2) The class address
                 3) The depth of the graph
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 ****************************************************************/
static void PrintClassBrowse(
  void *theEnv,
  char *logicalName,
  DEFCLASS *cls,
  long depth)
  {
   long i;

   for (i = 0 ; i < depth ; i++)
     EnvPrintRouter(theEnv,logicalName,"  ");
   EnvPrintRouter(theEnv,logicalName,EnvGetDefclassName(theEnv,(void *) cls));
   if (cls->directSuperclasses.classCount > 1)
     EnvPrintRouter(theEnv,logicalName," *");
   EnvPrintRouter(theEnv,logicalName,"\n");
   for (i = 0 ;i < cls->directSubclasses.classCount ; i++)
     PrintClassBrowse(theEnv,logicalName,cls->directSubclasses.classArray[i],depth+1);
  }

/*********************************************************
  NAME         : DisplaySeparator
  DESCRIPTION  : Prints a separator line for DescribeClass
  INPUTS       : 1) The logical name of the output
                 2) The buffer to use for the line
                 3) The buffer size
                 4) The character to use
  RETURNS      : Nothing useful
  SIDE EFFECTS : Buffer overwritten and displayed
  NOTES        : None
 *********************************************************/
static void DisplaySeparator(
  void *theEnv,
  char *logicalName,
  char *buf,
  int maxlen,
  int sepchar)
  {
   register int i;

   for (i = 0 ; i < maxlen-2 ; i++)
     buf[i] = (char) sepchar;
   buf[i++] = '\n';
   buf[i] = '\0';
   EnvPrintRouter(theEnv,logicalName,buf);
  }

/*************************************************************
  NAME         : DisplaySlotBasicInfo
  DESCRIPTION  : Displays a table summary of basic
                  facets for the slots of a class
                  including:
                  single/multiple
                  default/no-default/default-dynamic
                  inherit/no-inherit
                  read-write/initialize-only/read-only
                  local/shared
                  composite/exclusive
                  reactive/non-reactive
                  public/private
                  create-accessor read/write
                  override-message

                  The function also displays the source
                  class(es) for the facets
  INPUTS       : 1) The logical name of the output
                 2) A format string for use in sprintf
                    (for printing slot names)
                 3) A format string for use in sprintf
                    (for printing slot override message names)
                 4) A buffer to store the display in
                 5) A pointer to the class
  RETURNS      : Nothing useful
  SIDE EFFECTS : Buffer written to and displayed
  NOTES        : None
 *************************************************************/
static void DisplaySlotBasicInfo(
  void *theEnv,
  char *logicalName,
  char *slotNamePrintFormat,
  char *overrideMessagePrintFormat,
  char *buf,
  DEFCLASS *cls)
  {
   long i;
   SLOT_DESC *sp;
   char *createString;

   gensprintf(buf,slotNamePrintFormat,"SLOTS");
#if DEFRULE_CONSTRUCT
   genstrcat(buf,"FLD DEF PRP ACC STO MCH SRC VIS CRT ");
#else
   genstrcat(buf,"FLD DEF PRP ACC STO SRC VIS CRT ");
#endif
   EnvPrintRouter(theEnv,logicalName,buf);
   gensprintf(buf,overrideMessagePrintFormat,"OVRD-MSG");
   EnvPrintRouter(theEnv,logicalName,buf);
   EnvPrintRouter(theEnv,logicalName,"SOURCE(S)\n");
   for (i = 0 ; i < cls->instanceSlotCount ; i++)
     {
      sp = cls->instanceTemplate[i];
      gensprintf(buf,slotNamePrintFormat,ValueToString(sp->slotName->name));
      genstrcat(buf,sp->multiple ? "MLT " : "SGL ");
      if (sp->noDefault)
        genstrcat(buf,"NIL ");
      else
        genstrcat(buf,sp->dynamicDefault ? "DYN " : "STC ");
      genstrcat(buf,sp->noInherit ? "NIL " : "INH ");
      if (sp->initializeOnly)
        genstrcat(buf,"INT ");
      else if (sp->noWrite)
        genstrcat(buf," R  ");
      else
        genstrcat(buf,"RW  ");
      genstrcat(buf,sp->shared ? "SHR " : "LCL ");
#if DEFRULE_CONSTRUCT
      genstrcat(buf,sp->reactive ? "RCT " : "NIL ");
#endif
      genstrcat(buf,sp->composite ? "CMP " : "EXC ");
      genstrcat(buf,sp->publicVisibility ? "PUB " : "PRV ");
      createString = GetCreateAccessorString(sp);
      if (createString[1] == '\0')
        genstrcat(buf," ");
      genstrcat(buf,createString);
      if ((createString[1] == '\0') ? TRUE : (createString[2] == '\0'))
        genstrcat(buf," ");
      genstrcat(buf," ");
      EnvPrintRouter(theEnv,logicalName,buf);
      gensprintf(buf,overrideMessagePrintFormat,
              sp->noWrite ? "NIL" : ValueToString(sp->overrideMessage));
      EnvPrintRouter(theEnv,logicalName,buf);
      PrintSlotSources(theEnv,logicalName,sp->slotName->name,&sp->cls->allSuperclasses,0,TRUE);
      EnvPrintRouter(theEnv,logicalName,"\n");
     }
  }

/***************************************************
  NAME         : PrintSlotSources
  DESCRIPTION  : Displays a list of source classes
                   for a composite class (in order
                   of most general to specific)
  INPUTS       : 1) The logical name of the output
                 2) The name of the slot
                 3) The precedence list of the class
                    of the slot (the source class
                    shold be first in the list)
                 4) The index into the packed
                    links array
                 5) Flag indicating whether to
                    disregard noniherit facet
  RETURNS      : TRUE if a class is printed, FALSE
                 otherwise
  SIDE EFFECTS : Recursively prints out appropriate
                 memebers from list in reverse order
  NOTES        : None
 ***************************************************/
static intBool PrintSlotSources(
  void *theEnv,
  char *logicalName,
  SYMBOL_HN *sname,
  PACKED_CLASS_LINKS *sprec,
  long theIndex,
  int inhp)
  {
   SLOT_DESC *csp;

   if (theIndex == sprec->classCount)
     return(FALSE);
   csp = FindClassSlot(sprec->classArray[theIndex],sname);
   if ((csp != NULL) ? ((csp->noInherit == 0) || inhp) : FALSE)
     {
      if (csp->composite)
        {
         if (PrintSlotSources(theEnv,logicalName,sname,sprec,theIndex+1,FALSE))
           EnvPrintRouter(theEnv,logicalName," ");
        }
      PrintClassName(theEnv,logicalName,sprec->classArray[theIndex],FALSE);
      return(TRUE);
     }
   else
     return(PrintSlotSources(theEnv,logicalName,sname,sprec,theIndex+1,FALSE));
  }

/*********************************************************
  NAME         : DisplaySlotConstraintInfo
  DESCRIPTION  : Displays a table summary of type-checking
                  facets for the slots of a class
                  including:
                  type
                  allowed-symbols
                  allowed-integers
                  allowed-floats
                  allowed-values
                  allowed-instance-names
                  range
                  min-number-of-elements
                  max-number-of-elements

                  The function also displays the source
                  class(es) for the facets
  INPUTS       : 1) A format string for use in sprintf
                 2) A buffer to store the display in
                 3) Maximum buffer size
                 4) A pointer to the class
  RETURNS      : Nothing useful
  SIDE EFFECTS : Buffer written to and displayed
  NOTES        : None
 *********************************************************/
static void DisplaySlotConstraintInfo(
  void *theEnv,
  char *logicalName,
  char *slotNamePrintFormat,
  char *buf,
  unsigned maxlen,
  DEFCLASS *cls)
  {
   long i;
   CONSTRAINT_RECORD *cr;
   char *strdest = "***describe-class***";

   gensprintf(buf,slotNamePrintFormat,"SLOTS");
   genstrcat(buf,"SYM STR INN INA EXA FTA INT FLT\n");
   EnvPrintRouter(theEnv,logicalName,buf);
   for (i = 0 ; i < cls->instanceSlotCount ; i++)
     {
      cr = cls->instanceTemplate[i]->constraint;
      gensprintf(buf,slotNamePrintFormat,ValueToString(cls->instanceTemplate[i]->slotName->name));
      if (cr != NULL)
        {
         genstrcat(buf,ConstraintCode(cr,(unsigned) cr->symbolsAllowed,
                                      (unsigned) cr->symbolRestriction));
         genstrcat(buf,ConstraintCode(cr,(unsigned) cr->stringsAllowed,
                                      (unsigned) cr->stringRestriction));
         genstrcat(buf,ConstraintCode(cr,(unsigned) cr->instanceNamesAllowed,
                                      (unsigned) (cr->instanceNameRestriction || cr->classRestriction)));
         genstrcat(buf,ConstraintCode(cr,(unsigned) cr->instanceAddressesAllowed,
                                      (unsigned) cr->classRestriction));
         genstrcat(buf,ConstraintCode(cr,(unsigned) cr->externalAddressesAllowed,0));
         genstrcat(buf,ConstraintCode(cr,(unsigned) cr->factAddressesAllowed,0));
         genstrcat(buf,ConstraintCode(cr,(unsigned) cr->integersAllowed,
                                      (unsigned) cr->integerRestriction));
         genstrcat(buf,ConstraintCode(cr,(unsigned) cr->floatsAllowed,
                                      (unsigned) cr->floatRestriction));
         OpenStringDestination(theEnv,strdest,buf + strlen(buf),(maxlen - strlen(buf) - 1));
         if (cr->integersAllowed || cr->floatsAllowed || cr->anyAllowed)
           {
            EnvPrintRouter(theEnv,strdest,"RNG:[");
            PrintExpression(theEnv,strdest,cr->minValue);
            EnvPrintRouter(theEnv,strdest,"..");
            PrintExpression(theEnv,strdest,cr->maxValue);
            EnvPrintRouter(theEnv,strdest,"] ");
           }
         if (cls->instanceTemplate[i]->multiple)
           {
            EnvPrintRouter(theEnv,strdest,"CRD:[");
            PrintExpression(theEnv,strdest,cr->minFields);
            EnvPrintRouter(theEnv,strdest,"..");
            PrintExpression(theEnv,strdest,cr->maxFields);
            EnvPrintRouter(theEnv,strdest,"]");
           }
        }
      else
        {
         OpenStringDestination(theEnv,strdest,buf,maxlen);
         EnvPrintRouter(theEnv,strdest," +   +   +   +   +   +   +   +  RNG:[-oo..+oo]");
         if (cls->instanceTemplate[i]->multiple)
           EnvPrintRouter(theEnv,strdest," CRD:[0..+oo]");
        }
      EnvPrintRouter(theEnv,strdest,"\n");
      CloseStringDestination(theEnv,strdest);
      EnvPrintRouter(theEnv,logicalName,buf);
     }
  }

/******************************************************
  NAME         : ConstraintCode
  DESCRIPTION  : Gives a string code representing the
                 type of constraint
  INPUTS       : 1) The constraint record
                 2) Allowed Flag
                 3) Restricted Values flag
  RETURNS      : "    " for type not allowed
                 " +  " for any value of type allowed
                 " #  " for some values of type allowed
  SIDE EFFECTS : None
  NOTES        : Used by DisplaySlotConstraintInfo
 ******************************************************/
static char *ConstraintCode(
  CONSTRAINT_RECORD *cr,
  unsigned allow,
  unsigned restrictValues)
  {
   if (allow || cr->anyAllowed)
     {
      if (restrictValues || cr->anyRestriction) return " #  ";
      else return " +  ";
     }
   return("    ");
  }

#endif

#endif

