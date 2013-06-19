   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Generic Functions Parsing Routines               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            If the last construct in a loaded file is a    */
/*            deffunction or defmethod with no closing right */
/*            parenthesis, an error should be issued, but is */
/*            not. DR0872                                    */
/*                                                           */
/*************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if DEFGENERIC_CONSTRUCT && (! BLOAD_ONLY) && (! RUN_TIME)

#if BLOAD || BLOAD_AND_BSAVE
#include "bload.h"
#endif

#if DEFFUNCTION_CONSTRUCT
#include "dffnxfun.h"
#endif

#if OBJECT_SYSTEM
#include "classfun.h"
#include "classcom.h"
#endif

#include "memalloc.h"
#include "cstrcpsr.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "genrccom.h"
#include "immthpsr.h"
#include "modulutl.h"
#include "prcdrpsr.h"
#include "prccode.h"
#include "router.h"
#include "scanner.h"

#define _GENRCPSR_SOURCE_
#include "genrcpsr.h"

/* =========================================
   *****************************************
                   CONSTANTS
   =========================================
   ***************************************** */
#define HIGHER_PRECEDENCE -1
#define IDENTICAL          0
#define LOWER_PRECEDENCE   1

#define CURR_ARG_VAR "current-argument"

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static intBool ValidGenericName(void *,char *);
static SYMBOL_HN *ParseMethodNameAndIndex(void *,char *,int *);

#if DEBUGGING_FUNCTIONS
static void CreateDefaultGenericPPForm(void *,DEFGENERIC *);
#endif

static int ParseMethodParameters(void *,char *,EXPRESSION **,SYMBOL_HN **);
static RESTRICTION *ParseRestriction(void *,char *);
static void ReplaceCurrentArgRefs(void *,EXPRESSION *);
static int DuplicateParameters(void *,EXPRESSION *,EXPRESSION **,SYMBOL_HN *);
static EXPRESSION *AddParameter(void *,EXPRESSION *,EXPRESSION *,SYMBOL_HN *,RESTRICTION *);
static EXPRESSION *ValidType(void *,SYMBOL_HN *);
static intBool RedundantClasses(void *,void *,void *);
static DEFGENERIC *AddGeneric(void *,SYMBOL_HN *,int *);
static DEFMETHOD *AddGenericMethod(void *,DEFGENERIC *,int,short);
static int RestrictionsCompare(EXPRESSION *,int,int,int,DEFMETHOD *);
static int TypeListCompare(RESTRICTION *,RESTRICTION *);
static DEFGENERIC *NewGeneric(void *,SYMBOL_HN *);

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***************************************************************************
  NAME         : ParseDefgeneric
  DESCRIPTION  : Parses the defgeneric construct
  INPUTS       : The input logical name
  RETURNS      : FALSE if successful parse, TRUE otherwise
  SIDE EFFECTS : Inserts valid generic function defn into generic entry
  NOTES        : H/L Syntax :
                 (defgeneric <name> [<comment>])
 ***************************************************************************/
globle intBool ParseDefgeneric(
  void *theEnv,
  char *readSource)
  {
   SYMBOL_HN *gname;
   DEFGENERIC *gfunc;
   int newGeneric;

   SetPPBufferStatus(theEnv,ON);
   FlushPPBuffer(theEnv);
   SavePPBuffer(theEnv,"(defgeneric ");
   SetIndentDepth(theEnv,3);

#if BLOAD || BLOAD_AND_BSAVE
   if ((Bloaded(theEnv) == TRUE) && (! ConstructData(theEnv)->CheckSyntaxMode))
     {
      CannotLoadWithBloadMessage(theEnv,"defgeneric");
      return(TRUE);
     }
#endif

   gname = GetConstructNameAndComment(theEnv,readSource,&DefgenericData(theEnv)->GenericInputToken,"defgeneric",
                                      EnvFindDefgeneric,NULL,"^",TRUE,
                                      TRUE,TRUE);
   if (gname == NULL)
     return(TRUE);

   if (ValidGenericName(theEnv,ValueToString(gname)) == FALSE)
     return(TRUE);

   if (DefgenericData(theEnv)->GenericInputToken.type != RPAREN)
     {
      PrintErrorID(theEnv,"GENRCPSR",1,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Expected ')' to complete defgeneric.\n");
      return(TRUE);
     }
   SavePPBuffer(theEnv,"\n");

   /* ========================================================
      If we're only checking syntax, don't add the
      successfully parsed deffacts to the KB.
      ======================================================== */

   if (ConstructData(theEnv)->CheckSyntaxMode)
     { return(FALSE); }

   gfunc = AddGeneric(theEnv,gname,&newGeneric);

#if DEBUGGING_FUNCTIONS
   SetDefgenericPPForm((void *) gfunc,EnvGetConserveMemory(theEnv) ? NULL : CopyPPBuffer(theEnv));
#endif
   return(FALSE);
  }

/***************************************************************************
  NAME         : ParseDefmethod
  DESCRIPTION  : Parses the defmethod construct
  INPUTS       : The input logical name
  RETURNS      : FALSE if successful parse, TRUE otherwise
  SIDE EFFECTS : Inserts valid method definition into generic entry
  NOTES        : H/L Syntax :
                 (defmethod <name> [<index>] [<comment>]
                    (<restriction>* [<wildcard>])
                    <action>*)
                 <restriction> :== ?<name> |
                                   (?<name> <type>* [<restriction-query>])
                 <wildcard>    :== $?<name> |
                                   ($?<name> <type>* [<restriction-query>])
 ***************************************************************************/
globle intBool ParseDefmethod(
  void *theEnv,
  char *readSource)
  {
   SYMBOL_HN *gname;
   int rcnt,mposn,mi,newMethod,mnew = FALSE,lvars,error;
   EXPRESSION *params,*actions,*tmp;
   SYMBOL_HN *wildcard;
   DEFMETHOD *meth;
   DEFGENERIC *gfunc;
   int theIndex;

   SetPPBufferStatus(theEnv,ON);
   FlushPPBuffer(theEnv);
   SetIndentDepth(theEnv,3);
   SavePPBuffer(theEnv,"(defmethod ");

#if BLOAD || BLOAD_AND_BSAVE
   if ((Bloaded(theEnv) == TRUE) && (! ConstructData(theEnv)->CheckSyntaxMode))
     {
      CannotLoadWithBloadMessage(theEnv,"defmethod");
      return(TRUE);
     }
#endif

   gname = ParseMethodNameAndIndex(theEnv,readSource,&theIndex);
   if (gname == NULL)
     return(TRUE);

   if (ValidGenericName(theEnv,ValueToString(gname)) == FALSE)
     return(TRUE);

   /* ========================================================
      Go ahead and add the header so that the generic function
      can be called recursively
      ======================================================== */
   gfunc = AddGeneric(theEnv,gname,&newMethod);

#if DEBUGGING_FUNCTIONS
   if (newMethod && (! ConstructData(theEnv)->CheckSyntaxMode))
      CreateDefaultGenericPPForm(theEnv,gfunc);
#endif

   IncrementIndentDepth(theEnv,1);
   rcnt = ParseMethodParameters(theEnv,readSource,&params,&wildcard);
   DecrementIndentDepth(theEnv,1);
   if (rcnt == -1)
     goto DefmethodParseError;
   PPCRAndIndent(theEnv);
   for (tmp = params ; tmp != NULL ; tmp = tmp->nextArg)
     {
      ReplaceCurrentArgRefs(theEnv,((RESTRICTION *) tmp->argList)->query);
      if (ReplaceProcVars(theEnv,"method",((RESTRICTION *) tmp->argList)->query,
                                  params,wildcard,NULL,NULL))
        {
         DeleteTempRestricts(theEnv,params);
         goto DefmethodParseError;
        }
     }
   meth = FindMethodByRestrictions(gfunc,params,rcnt,wildcard,&mposn);
   error = FALSE;
   if (meth != NULL)
     {
      if (meth->system)
        {
         PrintErrorID(theEnv,"GENRCPSR",17,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Cannot replace the implicit system method #");
         PrintLongInteger(theEnv,WERROR,(long long) meth->index);
         EnvPrintRouter(theEnv,WERROR,".\n");
         error = TRUE;
        }
      else if ((theIndex != 0) && (theIndex != meth->index))
        {
         PrintErrorID(theEnv,"GENRCPSR",2,FALSE);
         EnvPrintRouter(theEnv,WERROR,"New method #");
         PrintLongInteger(theEnv,WERROR,(long long) theIndex);
         EnvPrintRouter(theEnv,WERROR," would be indistinguishable from method #");
         PrintLongInteger(theEnv,WERROR,(long long) meth->index);
         EnvPrintRouter(theEnv,WERROR,".\n");
         error = TRUE;
        }
     }
   else if (theIndex != 0)
     {
      mi = FindMethodByIndex(gfunc,theIndex);
      if (mi == -1)
        mnew = TRUE;
      else if (gfunc->methods[mi].system)
        {
         PrintErrorID(theEnv,"GENRCPSR",17,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Cannot replace the implicit system method #");
         PrintLongInteger(theEnv,WERROR,(long long) theIndex);
         EnvPrintRouter(theEnv,WERROR,".\n");
         error = TRUE;
        }
     }
   else
     mnew = TRUE;
   if (error)
     {
      DeleteTempRestricts(theEnv,params);
      goto DefmethodParseError;
     }
   ExpressionData(theEnv)->ReturnContext = TRUE;
   actions = ParseProcActions(theEnv,"method",readSource,
                              &DefgenericData(theEnv)->GenericInputToken,params,wildcard,
                              NULL,NULL,&lvars,NULL);
                              
   /*===========================================================*/
   /* Check for the closing right parenthesis of the defmethod. */
   /*===========================================================*/

   if ((DefgenericData(theEnv)->GenericInputToken.type != RPAREN) &&  /* DR0872 */
       (actions != NULL))
     {
      SyntaxErrorMessage(theEnv,"defmethod");
      DeleteTempRestricts(theEnv,params);
      ReturnPackedExpression(theEnv,actions);
      goto DefmethodParseError;
     }

   if (actions == NULL)
     {
      DeleteTempRestricts(theEnv,params);
      goto DefmethodParseError;
     }

   /*==============================================*/
   /* If we're only checking syntax, don't add the */
   /* successfully parsed deffunction to the KB.   */
   /*==============================================*/

   if (ConstructData(theEnv)->CheckSyntaxMode)
     {
      DeleteTempRestricts(theEnv,params);
      ReturnPackedExpression(theEnv,actions);
      if (newMethod)
        {
         RemoveConstructFromModule(theEnv,(struct constructHeader *) gfunc);
         RemoveDefgeneric(theEnv,(struct constructHeader *) gfunc);
        }
      return(FALSE);
     }

   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,DefgenericData(theEnv)->GenericInputToken.printForm);
   SavePPBuffer(theEnv,"\n");

#if DEBUGGING_FUNCTIONS
   meth = AddMethod(theEnv,gfunc,meth,mposn,(short) theIndex,params,rcnt,lvars,wildcard,actions,
             EnvGetConserveMemory(theEnv) ? NULL : CopyPPBuffer(theEnv),FALSE);
#else
   meth = AddMethod(theEnv,gfunc,meth,mposn,theIndex,params,rcnt,lvars,wildcard,actions,NULL,FALSE);
#endif
   DeleteTempRestricts(theEnv,params);
   if (GetPrintWhileLoading(theEnv) && GetCompilationsWatch(theEnv))
     {
      EnvPrintRouter(theEnv,WDIALOG,"   Method #");
      PrintLongInteger(theEnv,WDIALOG,(long long) meth->index);
      if (mnew)
        EnvPrintRouter(theEnv,WDIALOG," defined.\n");
      else
        EnvPrintRouter(theEnv,WDIALOG," redefined.\n");
     }
   return(FALSE);

DefmethodParseError:
   if (newMethod)
     {
      RemoveConstructFromModule(theEnv,(struct constructHeader *) gfunc);
      RemoveDefgeneric(theEnv,(void *) gfunc);
     }
   return(TRUE);
  }

/************************************************************************
  NAME         : AddMethod
  DESCRIPTION  : (Re)defines a new method for a generic
                 If method already exists, deletes old information
                    before proceeding.
  INPUTS       : 1) The generic address
                 2) The old method address (can be NULL)
                 3) The old method array position (can be -1)
                 4) The method index to assign (0 if don't care)
                 5) The parameter expression-list
                    (restrictions attached to argList pointers)
                 6) The number of restrictions
                 7) The number of locals vars reqd
                 8) The wildcard symbol (NULL if none)
                 9) Method actions
                 10) Method pretty-print form
                 11) A flag indicating whether to copy the
                     restriction types or just use the pointers
  RETURNS      : The new (old) method address
  SIDE EFFECTS : Method added to (or changed in) method array for generic
                 Restrictions repacked into new method
                 Actions and pretty-print form attached
  NOTES        : Assumes if a method is being redefined, its busy
                   count is 0!!
                 IMPORTANT: Expects that FindMethodByRestrictions() has
                   previously been called to determine if this method
                   is already present or not.  Arguments #1 and #2
                   should be the values obtained from FindMethod...().
 ************************************************************************/
globle DEFMETHOD *AddMethod(
  void *theEnv,
  DEFGENERIC *gfunc,
  DEFMETHOD *meth,
  int mposn,
  short mi,
  EXPRESSION *params,
  int rcnt,
  int lvars,
  SYMBOL_HN *wildcard,
  EXPRESSION *actions,
  char *ppForm,
  int copyRestricts)
  {
   RESTRICTION *rptr,*rtmp;
   register int i,j;
   int mai;

   SaveBusyCount(gfunc);
   if (meth == NULL)
     {
      mai = (mi != 0) ? FindMethodByIndex(gfunc,mi) : -1;
      if (mai == -1)
        meth = AddGenericMethod(theEnv,gfunc,mposn,mi);
      else
        {
         DeleteMethodInfo(theEnv,gfunc,&gfunc->methods[mai]);
         if (mai < mposn)
           {
            mposn--;
            for (i = mai+1 ; i <= mposn ; i++)
              GenCopyMemory(DEFMETHOD,1,&gfunc->methods[i-1],&gfunc->methods[i]);
           }
         else
           {
            for (i = mai-1 ; i >= mposn ; i--)
              GenCopyMemory(DEFMETHOD,1,&gfunc->methods[i+1],&gfunc->methods[i]);
           }
         meth = &gfunc->methods[mposn];
         meth->index = mi;
        }
     }
   else
     {
      /* ================================
         The old trace state is preserved
         ================================ */
      ExpressionDeinstall(theEnv,meth->actions);
      ReturnPackedExpression(theEnv,meth->actions);
      if (meth->ppForm != NULL)
        rm(theEnv,(void *) meth->ppForm,(sizeof(char) * (strlen(meth->ppForm)+1)));
     }
   meth->system = 0;
   meth->actions = actions;
   ExpressionInstall(theEnv,meth->actions);
   meth->ppForm = ppForm;
   if (mposn == -1)
     {
      RestoreBusyCount(gfunc);
      return(meth);
     }

   meth->localVarCount = (short) lvars;
   meth->restrictionCount = (short) rcnt;
   if (wildcard != NULL)
     {
      meth->minRestrictions = (short) (rcnt-1);
      meth->maxRestrictions = -1;
     }
   else
     meth->minRestrictions = meth->maxRestrictions = (short) rcnt;
   if (rcnt != 0)
     meth->restrictions = (RESTRICTION *)
                          gm2(theEnv,(sizeof(RESTRICTION) * rcnt));
   else
     meth->restrictions = NULL;
   for (i = 0 ; i < rcnt ; i++)
     {
      rptr = &meth->restrictions[i];
      rtmp = (RESTRICTION *) params->argList;
      rptr->query = PackExpression(theEnv,rtmp->query);
      rptr->tcnt = rtmp->tcnt;
      if (copyRestricts)
        {
         if (rtmp->types != NULL)
           {
            rptr->types = (void **) gm2(theEnv,(rptr->tcnt * sizeof(void *)));
            GenCopyMemory(void *,rptr->tcnt,rptr->types,rtmp->types);
           }
         else
           rptr->types = NULL;
        }
      else
        {
         rptr->types = rtmp->types;

         /* =====================================================
            Make sure the types-array is not deallocated when the
              temporary restriction nodes are
            ===================================================== */
         rtmp->tcnt = 0;
         rtmp->types = NULL;
        }
      ExpressionInstall(theEnv,rptr->query);
      for (j = 0 ; j < rptr->tcnt ; j++)
#if OBJECT_SYSTEM
        IncrementDefclassBusyCount(theEnv,rptr->types[j]);
#else
        IncrementIntegerCount((INTEGER_HN *) rptr->types[j]);
#endif
      params = params->nextArg;
     }
   RestoreBusyCount(gfunc);
   return(meth);
  }

/*****************************************************
  NAME         : PackRestrictionTypes
  DESCRIPTION  : Takes the restriction type list
                   and packs it into a contiguous
                   array of void *.
  INPUTS       : 1) The restriction structure
                 2) The types expression list
  RETURNS      : Nothing useful
  SIDE EFFECTS : Array allocated & expressions freed
  NOTES        : None
 *****************************************************/
globle void PackRestrictionTypes(
  void *theEnv,
  RESTRICTION *rptr,
  EXPRESSION *types)
  {
   EXPRESSION *tmp;
   long i;

   rptr->tcnt = 0;
   for (tmp = types ; tmp != NULL ; tmp = tmp->nextArg)
     rptr->tcnt++;
   if (rptr->tcnt != 0)
     rptr->types = (void **) gm2(theEnv,(sizeof(void *) * rptr->tcnt));
   else
     rptr->types = NULL;
   for (i = 0 , tmp = types ; i < rptr->tcnt ; i++ , tmp = tmp->nextArg)
     rptr->types[i] = (void *) tmp->value;
   ReturnExpression(theEnv,types);
  }

/***************************************************
  NAME         : DeleteTempRestricts
  DESCRIPTION  : Deallocates the method
                   temporary parameter list
  INPUTS       : The head of the list
  RETURNS      : Nothing useful
  SIDE EFFECTS : List deallocated
  NOTES        : None
 ***************************************************/
globle void DeleteTempRestricts(
  void *theEnv,
  EXPRESSION *phead)
  {
   EXPRESSION *ptmp;
   RESTRICTION *rtmp;

   while (phead != NULL)
     {
      ptmp = phead;
      phead = phead->nextArg;
      rtmp = (RESTRICTION *) ptmp->argList;
      rtn_struct(theEnv,expr,ptmp);
      ReturnExpression(theEnv,rtmp->query);
      if (rtmp->tcnt != 0)
        rm(theEnv,(void *) rtmp->types,(sizeof(void *) * rtmp->tcnt));
      rtn_struct(theEnv,restriction,rtmp);
     }
  }

/**********************************************************
  NAME         : FindMethodByRestrictions
  DESCRIPTION  : See if a method for the specified
                   generic satsifies the given restrictions
  INPUTS       : 1) Generic function
                 2) Parameter/restriction expression list
                 3) Number of restrictions
                 4) Wildcard symbol (can be NULL)
                 5) Caller's buffer for holding array posn
                      of where to add new generic method
                      (-1 if method already present)
  RETURNS      : The address of the found method, NULL if
                    not found
  SIDE EFFECTS : Sets the caller's buffer to the index of
                   where to place the new method, -1 if
                   already present
  NOTES        : None
 **********************************************************/
globle DEFMETHOD *FindMethodByRestrictions(
  DEFGENERIC *gfunc,
  EXPRESSION *params,
  int rcnt,
  SYMBOL_HN *wildcard,
  int *posn)
  {
   register int i,cmp;
   int min,max;

   if (wildcard != NULL)
     {
      min = rcnt-1;
      max = -1;
     }
   else
     min = max = rcnt;
   for (i = 0 ; i < gfunc->mcnt ; i++)
     {
      cmp = RestrictionsCompare(params,rcnt,min,max,&gfunc->methods[i]);
      if (cmp == IDENTICAL)
        {
         *posn = -1;
         return(&gfunc->methods[i]);
        }
      else if (cmp == HIGHER_PRECEDENCE)
        {
         *posn = i;
         return(NULL);
        }
     }
   *posn = i;
   return(NULL);
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***********************************************************
  NAME         : ValidGenericName
  DESCRIPTION  : Determines if a particular function name
                    can be overloaded
  INPUTS       : The name
  RETURNS      : TRUE if OK, FALSE otherwise
  SIDE EFFECTS : Error message printed
  NOTES        : GetConstructNameAndComment() (called before
                 this function) ensures that the defgeneric
                 name does not conflict with one from
                 another module
 ***********************************************************/
static intBool ValidGenericName(
  void *theEnv,
  char *theDefgenericName)
  {
   struct constructHeader *theDefgeneric;
#if DEFFUNCTION_CONSTRUCT
   struct defmodule *theModule;
   struct constructHeader *theDeffunction;
#endif
   struct FunctionDefinition *systemFunction;

   /* ============================================
      A defgeneric cannot be named the same as a
      construct type, e.g, defclass, defrule, etc.
      ============================================ */
   if (FindConstruct(theEnv,theDefgenericName) != NULL)
     {
      PrintErrorID(theEnv,"GENRCPSR",3,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Defgenerics are not allowed to replace constructs.\n");
      return(FALSE);
     }

#if DEFFUNCTION_CONSTRUCT
   /* ========================================
      A defgeneric cannot be named the same as
      a defffunction (either in this module or
      imported from another)
      ======================================== */
   theDeffunction =
      (struct constructHeader *) LookupDeffunctionInScope(theEnv,theDefgenericName);
   if (theDeffunction != NULL)
     {
      theModule = GetConstructModuleItem(theDeffunction)->theModule;
      if (theModule != ((struct defmodule *) EnvGetCurrentModule(theEnv)))
        {
         PrintErrorID(theEnv,"GENRCPSR",4,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Deffunction ");
         EnvPrintRouter(theEnv,WERROR,EnvGetDeffunctionName(theEnv,(void *) theDeffunction));
         EnvPrintRouter(theEnv,WERROR," imported from module ");
         EnvPrintRouter(theEnv,WERROR,EnvGetDefmoduleName(theEnv,(void *) theModule));
         EnvPrintRouter(theEnv,WERROR," conflicts with this defgeneric.\n");
         return(FALSE);
        }
      else
        {
         PrintErrorID(theEnv,"GENRCPSR",5,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Defgenerics are not allowed to replace deffunctions.\n");
        }
      return(FALSE);
     }
#endif

   /* =========================================
      See if the defgeneric already exists in
      this module (or is imported from another)
      ========================================= */
   theDefgeneric = (struct constructHeader *) EnvFindDefgeneric(theEnv,theDefgenericName);
   if (theDefgeneric != NULL)
     {
      /* ===========================================
         And the redefinition of a defgeneric in
         the current module is only valid if none
         of its methods are executing
         =========================================== */
      if (MethodsExecuting((DEFGENERIC *) theDefgeneric))
        {
         MethodAlterError(theEnv,(DEFGENERIC *) theDefgeneric);
         return(FALSE);
        }
     }

   /* =======================================
      Only certain specific system functions
      may be overloaded by generic functions
      ======================================= */
   systemFunction = FindFunction(theEnv,theDefgenericName);
   if ((systemFunction != NULL) ?
       (systemFunction->overloadable == FALSE) : FALSE)
     {
      PrintErrorID(theEnv,"GENRCPSR",16,FALSE);
      EnvPrintRouter(theEnv,WERROR,"The system function ");
      EnvPrintRouter(theEnv,WERROR,theDefgenericName);
      EnvPrintRouter(theEnv,WERROR," cannot be overloaded.\n");
      return(FALSE);
     }
   return(TRUE);
  }

#if DEBUGGING_FUNCTIONS

/***************************************************
  NAME         : CreateDefaultGenericPPForm
  DESCRIPTION  : Adds a default pretty-print form
                 for a gneric function when it is
                 impliciylt created by the defn
                 of its first method
  INPUTS       : The generic function
  RETURNS      : Nothing useful
  SIDE EFFECTS : Pretty-print form created and
                 attached.
  NOTES        : None
 ***************************************************/
static void CreateDefaultGenericPPForm(
  void *theEnv,
  DEFGENERIC *gfunc)
  {
   char *moduleName,*genericName,*buf;

   moduleName = EnvGetDefmoduleName(theEnv,(void *) ((struct defmodule *) EnvGetCurrentModule(theEnv)));
   genericName = EnvGetDefgenericName(theEnv,(void *) gfunc);
   buf = (char *) gm2(theEnv,(sizeof(char) * (strlen(moduleName) + strlen(genericName) + 17)));
   gensprintf(buf,"(defgeneric %s::%s)\n",moduleName,genericName);
   SetDefgenericPPForm((void *) gfunc,buf);
  }

#endif

/*******************************************************
  NAME         : ParseMethodNameAndIndex
  DESCRIPTION  : Parses the name of the method and
                   optional method index
  INPUTS       : 1) The logical name of the input source
                 2) Caller's buffer for method index
                    (0 if not specified)
  RETURNS      : The symbolic name of the method
  SIDE EFFECTS : None
  NOTES        : Assumes "(defmethod " already parsed
 *******************************************************/
static SYMBOL_HN *ParseMethodNameAndIndex(
  void *theEnv,
  char *readSource,
  int *theIndex)
  {
   SYMBOL_HN *gname;

   *theIndex = 0;
   gname = GetConstructNameAndComment(theEnv,readSource,&DefgenericData(theEnv)->GenericInputToken,"defgeneric",
                                      EnvFindDefgeneric,NULL,"&",TRUE,FALSE,TRUE);
   if (gname == NULL)
     return(NULL);
   if (GetType(DefgenericData(theEnv)->GenericInputToken) == INTEGER)
     {
      int tmp;

      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv," ");
      SavePPBuffer(theEnv,DefgenericData(theEnv)->GenericInputToken.printForm);
      tmp = (int) ValueToLong(GetValue(DefgenericData(theEnv)->GenericInputToken));
      if (tmp < 1)
        {
         PrintErrorID(theEnv,"GENRCPSR",6,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Method index out of range.\n");
         return(NULL);
        }
      *theIndex = tmp;
      PPCRAndIndent(theEnv);
      GetToken(theEnv,readSource,&DefgenericData(theEnv)->GenericInputToken);
     }
   if (GetType(DefgenericData(theEnv)->GenericInputToken) == STRING)
     {
      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv," ");
      SavePPBuffer(theEnv,DefgenericData(theEnv)->GenericInputToken.printForm);
      PPCRAndIndent(theEnv);
      GetToken(theEnv,readSource,&DefgenericData(theEnv)->GenericInputToken);
     }
   return(gname);
  }

/************************************************************************
  NAME         : ParseMethodParameters
  DESCRIPTION  : Parses method restrictions
                   (parameter names with class and expression specifiers)
  INPUTS       : 1) The logical name of the input source
                 2) Caller's buffer for the parameter name list
                    (Restriction structures are attached to
                     argList pointers of parameter nodes)
                 3) Caller's buffer for wildcard symbol (if any)
  RETURNS      : The number of parameters, or -1 on errors
  SIDE EFFECTS : Memory allocated for parameters and restrictions
                 Parameter names stored in expression list
                 Parameter restrictions stored in contiguous array
  NOTES        : Any memory allocated is freed on errors
                 Assumes first opening parenthesis has been scanned
 ************************************************************************/
static int ParseMethodParameters(
  void *theEnv,
  char *readSource,
  EXPRESSION **params,
  SYMBOL_HN **wildcard)
  {
   EXPRESSION *phead = NULL,*pprv;
   SYMBOL_HN *pname;
   RESTRICTION *rtmp;
   int rcnt = 0;

   *wildcard = NULL;
   *params = NULL;
   if (GetType(DefgenericData(theEnv)->GenericInputToken) != LPAREN)
     {
      PrintErrorID(theEnv,"GENRCPSR",7,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Expected a '(' to begin method parameter restrictions.\n");
      return(-1);
     }
   GetToken(theEnv,readSource,&DefgenericData(theEnv)->GenericInputToken);
   while (DefgenericData(theEnv)->GenericInputToken.type != RPAREN)
     {
      if (*wildcard != NULL)
        {
         DeleteTempRestricts(theEnv,phead);
         PrintErrorID(theEnv,"PRCCODE",8,FALSE);
         EnvPrintRouter(theEnv,WERROR,"No parameters allowed after wildcard parameter.\n");
         return(-1);
        }
      if ((DefgenericData(theEnv)->GenericInputToken.type == SF_VARIABLE) || (DefgenericData(theEnv)->GenericInputToken.type == MF_VARIABLE))
        {
         pname = (SYMBOL_HN *) DefgenericData(theEnv)->GenericInputToken.value;
         if (DuplicateParameters(theEnv,phead,&pprv,pname))
           {
            DeleteTempRestricts(theEnv,phead);
            return(-1);
           }
         if (DefgenericData(theEnv)->GenericInputToken.type == MF_VARIABLE)
           *wildcard = pname;
         rtmp = get_struct(theEnv,restriction);
         PackRestrictionTypes(theEnv,rtmp,NULL);
         rtmp->query = NULL;
         phead = AddParameter(theEnv,phead,pprv,pname,rtmp);
         rcnt++;
        }
      else if (DefgenericData(theEnv)->GenericInputToken.type == LPAREN)
        {
         GetToken(theEnv,readSource,&DefgenericData(theEnv)->GenericInputToken);
         if ((DefgenericData(theEnv)->GenericInputToken.type != SF_VARIABLE) &&
             (DefgenericData(theEnv)->GenericInputToken.type != MF_VARIABLE))
           {
            DeleteTempRestricts(theEnv,phead);
            PrintErrorID(theEnv,"GENRCPSR",8,FALSE);
            EnvPrintRouter(theEnv,WERROR,"Expected a variable for parameter specification.\n");
            return(-1);
           }
         pname = (SYMBOL_HN *) DefgenericData(theEnv)->GenericInputToken.value;
         if (DuplicateParameters(theEnv,phead,&pprv,pname))
           {
            DeleteTempRestricts(theEnv,phead);
            return(-1);
           }
         if (DefgenericData(theEnv)->GenericInputToken.type == MF_VARIABLE)
           *wildcard = pname;
         SavePPBuffer(theEnv," ");
         rtmp = ParseRestriction(theEnv,readSource);
         if (rtmp == NULL)
           {
            DeleteTempRestricts(theEnv,phead);
            return(-1);
           }
         phead = AddParameter(theEnv,phead,pprv,pname,rtmp);
         rcnt++;
        }
      else
        {
         DeleteTempRestricts(theEnv,phead);
         PrintErrorID(theEnv,"GENRCPSR",9,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Expected a variable or '(' for parameter specification.\n");
         return(-1);
        }
      PPCRAndIndent(theEnv);
      GetToken(theEnv,readSource,&DefgenericData(theEnv)->GenericInputToken);
     }
   if (rcnt != 0)
     {
      PPBackup(theEnv);
      PPBackup(theEnv);
      SavePPBuffer(theEnv,")");
     }
   *params = phead;
   return(rcnt);
  }

/************************************************************
  NAME         : ParseRestriction
  DESCRIPTION  : Parses the restriction for a parameter of a
                   method
                 This restriction is comprised of:
                   1) A list of classes (or types) that are
                      allowed for the parameter (None
                      if no type restriction)
                   2) And an optional restriction-query
                      expression
  INPUTS       : The logical name of the input source
  RETURNS      : The address of a RESTRICTION node, NULL on
                   errors
  SIDE EFFECTS : RESTRICTION node allocated
                   Types are in a contiguous array of void *
                   Query is an expression
  NOTES        : Assumes "(?<var> " has already been parsed
                 H/L Syntax: <type>* [<query>])
 ************************************************************/
static RESTRICTION *ParseRestriction(
  void *theEnv,
  char *readSource)
  {
   EXPRESSION *types = NULL,*new_types,
              *typesbot,*tmp,*tmp2,
              *query = NULL;
   RESTRICTION *rptr;

   GetToken(theEnv,readSource,&DefgenericData(theEnv)->GenericInputToken);
   while (DefgenericData(theEnv)->GenericInputToken.type != RPAREN)
     {
      if (query != NULL)
        {
         PrintErrorID(theEnv,"GENRCPSR",10,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Query must be last in parameter restriction.\n");
         ReturnExpression(theEnv,query);
         ReturnExpression(theEnv,types);
         return(NULL);
        }
      if (DefgenericData(theEnv)->GenericInputToken.type == SYMBOL)
        {
         new_types = ValidType(theEnv,(SYMBOL_HN *) DefgenericData(theEnv)->GenericInputToken.value);
         if (new_types == NULL)
           {
            ReturnExpression(theEnv,types);
            ReturnExpression(theEnv,query);
            return(NULL);
           }
         if (types == NULL)
           types = new_types;
         else
           {
            for (typesbot = tmp = types ; tmp != NULL ; tmp = tmp->nextArg)
              {
               for (tmp2 = new_types ; tmp2 != NULL ; tmp2 = tmp2->nextArg)
                 {
                  if (tmp->value == tmp2->value)
                    {
                     PrintErrorID(theEnv,"GENRCPSR",11,FALSE);
#if OBJECT_SYSTEM
                     EnvPrintRouter(theEnv,WERROR,"Duplicate classes not allowed in parameter restriction.\n");
#else
                     EnvPrintRouter(theEnv,WERROR,"Duplicate types not allowed in parameter restriction.\n");
#endif
                     ReturnExpression(theEnv,query);
                     ReturnExpression(theEnv,types);
                     ReturnExpression(theEnv,new_types);
                     return(NULL);
                    }
                  if (RedundantClasses(theEnv,tmp->value,tmp2->value))
                    {
                     ReturnExpression(theEnv,query);
                     ReturnExpression(theEnv,types);
                     ReturnExpression(theEnv,new_types);
                     return(NULL);
                    }
                 }
               typesbot = tmp;
              }
            typesbot->nextArg = new_types;
           }
        }
      else if (DefgenericData(theEnv)->GenericInputToken.type == LPAREN)
        {
         query = Function1Parse(theEnv,readSource);
         if (query == NULL)
           {
            ReturnExpression(theEnv,types);
            return(NULL);
           }
         if (GetParsedBindNames(theEnv) != NULL)
           {
            PrintErrorID(theEnv,"GENRCPSR",12,FALSE);
            EnvPrintRouter(theEnv,WERROR,"Binds are not allowed in query expressions.\n");
            ReturnExpression(theEnv,query);
            ReturnExpression(theEnv,types);
            return(NULL);
           }
        }
#if DEFGLOBAL_CONSTRUCT
      else if (DefgenericData(theEnv)->GenericInputToken.type == GBL_VARIABLE)
        query = GenConstant(theEnv,GBL_VARIABLE,DefgenericData(theEnv)->GenericInputToken.value);
#endif
      else
        {
         PrintErrorID(theEnv,"GENRCPSR",13,FALSE);
#if OBJECT_SYSTEM
         EnvPrintRouter(theEnv,WERROR,"Expected a valid class name or query.\n");
#else
         EnvPrintRouter(theEnv,WERROR,"Expected a valid type name or query.\n");
#endif
         ReturnExpression(theEnv,query);
         ReturnExpression(theEnv,types);
         return(NULL);
        }
      SavePPBuffer(theEnv," ");
      GetToken(theEnv,readSource,&DefgenericData(theEnv)->GenericInputToken);
     }
   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,")");
   if ((types == NULL) && (query == NULL))
     {
      PrintErrorID(theEnv,"GENRCPSR",13,FALSE);
#if OBJECT_SYSTEM
      EnvPrintRouter(theEnv,WERROR,"Expected a valid class name or query.\n");
#else
      EnvPrintRouter(theEnv,WERROR,"Expected a valid type name or query.\n");
#endif
      return(NULL);
     }
   rptr = get_struct(theEnv,restriction);
   rptr->query = query;
   PackRestrictionTypes(theEnv,rptr,types);
   return(rptr);
  }

/*****************************************************************
  NAME         : ReplaceCurrentArgRefs
  DESCRIPTION  : Replaces all references to ?current-argument in
                  method parameter queries with special calls
                  to (gnrc-current-arg)
  INPUTS       : The query expression
  RETURNS      : Nothing useful
  SIDE EFFECTS : Variable references to ?current-argument replaced
  NOTES        : None
 *****************************************************************/
static void ReplaceCurrentArgRefs(
  void *theEnv,
  EXPRESSION *query)
  {
   while (query != NULL)
     {
      if ((query->type != SF_VARIABLE) ? FALSE :
          (strcmp(ValueToString(query->value),CURR_ARG_VAR) == 0))
        {
         query->type = FCALL;
         query->value = (void *) FindFunction(theEnv,"(gnrc-current-arg)");
        }
      if (query->argList != NULL)
        ReplaceCurrentArgRefs(theEnv,query->argList);
      query = query->nextArg;
     }
  }

/**********************************************************
  NAME         : DuplicateParameters
  DESCRIPTION  : Examines the parameter expression
                   chain for a method looking duplicates.
  INPUTS       : 1) The parameter chain (can be NULL)
                 2) Caller's buffer for address of
                    last node searched (can be used to
                    later attach new parameter)
                 3) The name of the parameter being checked
  RETURNS      : TRUE if duplicates found, FALSE otherwise
  SIDE EFFECTS : Caller's prv address set
  NOTES        : Assumes all parameter list nodes are WORDS
 **********************************************************/
static int DuplicateParameters(
  void *theEnv,
  EXPRESSION *head,
  EXPRESSION **prv,
  SYMBOL_HN *name)
  {
   *prv = NULL;
   while (head != NULL)
     {
      if (head->value == (void *) name)
        {
         PrintErrorID(theEnv,"PRCCODE",7,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Duplicate parameter names not allowed.\n");
         return(TRUE);
        }
      *prv = head;
      head = head->nextArg;
     }
   return(FALSE);
  }

/*****************************************************************
  NAME         : AddParameter
  DESCRIPTION  : Shoves a new paramter with its restriction
                   onto the list for a method
                 The parameter list is a list of expressions
                   linked by neext_arg pointers, and the
                   argList pointers are used for the restrictions
  INPUTS       : 1) The head of the list
                 2) The bottom of the list
                 3) The parameter name
                 4) The parameter restriction
  RETURNS      : The (new) head of the list
  SIDE EFFECTS : New parameter expression node allocated, set,
                   and attached
  NOTES        : None
 *****************************************************************/
static EXPRESSION *AddParameter(
  void *theEnv,
  EXPRESSION *phead,
  EXPRESSION *pprv,
  SYMBOL_HN *pname,
  RESTRICTION *rptr)
  {
   EXPRESSION *ptmp;

   ptmp = GenConstant(theEnv,SYMBOL,(void *) pname);
   if (phead == NULL)
     phead = ptmp;
   else
     pprv->nextArg = ptmp;
   ptmp->argList = (EXPRESSION *) rptr;
   return(phead);
  }

/**************************************************************
  NAME         : ValidType
  DESCRIPTION  : Examines the name of a restriction type and
                   forms a list of integer-code expressions
                   corresponding to the primitive types
                 (or a Class address if COOL is installed)
  INPUTS       : The type name
  RETURNS      : The expression chain (NULL on errors)
  SIDE EFFECTS : Expression type chain allocated
                   one or more nodes holding codes for types
                   (or class addresses)
  NOTES        : None
 *************************************************************/
static EXPRESSION *ValidType(
  void *theEnv,
  SYMBOL_HN *tname)
  {
#if OBJECT_SYSTEM
   DEFCLASS *cls;

   if (FindModuleSeparator(ValueToString(tname)))
     IllegalModuleSpecifierMessage(theEnv);
   else
     {
      cls = LookupDefclassInScope(theEnv,ValueToString(tname));
      if (cls == NULL)
        {
         PrintErrorID(theEnv,"GENRCPSR",14,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Unknown class in method.\n");
         return(NULL);
        }
      return(GenConstant(theEnv,DEFCLASS_PTR,(void *) cls));
     }
#else
   if (strcmp(ValueToString(tname),INTEGER_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) INTEGER)));
   if (strcmp(ValueToString(tname),FLOAT_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) FLOAT)));
   if (strcmp(ValueToString(tname),SYMBOL_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) SYMBOL)));
   if (strcmp(ValueToString(tname),STRING_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) STRING)));
   if (strcmp(ValueToString(tname),MULTIFIELD_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) MULTIFIELD)));
   if (strcmp(ValueToString(tname),EXTERNAL_ADDRESS_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) EXTERNAL_ADDRESS)));
   if (strcmp(ValueToString(tname),FACT_ADDRESS_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) FACT_ADDRESS)));
   if (strcmp(ValueToString(tname),NUMBER_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) NUMBER_TYPE_CODE)));
   if (strcmp(ValueToString(tname),LEXEME_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) LEXEME_TYPE_CODE)));
   if (strcmp(ValueToString(tname),ADDRESS_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) ADDRESS_TYPE_CODE)));
   if (strcmp(ValueToString(tname),PRIMITIVE_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) PRIMITIVE_TYPE_CODE)));
   if (strcmp(ValueToString(tname),OBJECT_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) OBJECT_TYPE_CODE)));
   if (strcmp(ValueToString(tname),INSTANCE_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) INSTANCE_TYPE_CODE)));
   if (strcmp(ValueToString(tname),INSTANCE_NAME_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) INSTANCE_NAME)));
   if (strcmp(ValueToString(tname),INSTANCE_ADDRESS_TYPE_NAME) == 0)
     return(GenConstant(theEnv,INTEGER,(void *) EnvAddLong(theEnv,(long long) INSTANCE_ADDRESS)));

   PrintErrorID(theEnv,"GENRCPSR",14,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Unknown type in method.\n");
#endif
   return(NULL);
  }

/*************************************************************
  NAME         : RedundantClasses
  DESCRIPTION  : Determines if one class (type) is
                 subsumes (or is subsumed by) another.
  INPUTS       : Two void pointers which are class pointers
                 if COOL is installed or integer hash nodes
                 for type codes otherwise.
  RETURNS      : TRUE if there is subsumption, FALSE otherwise
  SIDE EFFECTS : An error message is printed, if appropriate.
  NOTES        : None
 *************************************************************/
static intBool RedundantClasses(
  void *theEnv,
  void *c1,
  void *c2)
  {
   char *tname;

#if OBJECT_SYSTEM
   if (HasSuperclass((DEFCLASS *) c1,(DEFCLASS *) c2))
     tname = EnvGetDefclassName(theEnv,c1);
   else if (HasSuperclass((DEFCLASS *) c2,(DEFCLASS *) c1))
     tname = EnvGetDefclassName(theEnv,c2);
#else
   if (SubsumeType(ValueToInteger(c1),ValueToInteger(c2)))
     tname = TypeName(theEnv,ValueToInteger(c1));
   else if (SubsumeType(ValueToInteger(c2),ValueToInteger(c1)))
     tname = TypeName(theEnv,ValueToInteger(c2));
#endif
   else
     return(FALSE);
   PrintErrorID(theEnv,"GENRCPSR",15,FALSE);
   EnvPrintRouter(theEnv,WERROR,tname);
   EnvPrintRouter(theEnv,WERROR," class is redundant.\n");
   return(TRUE);
  }

/*********************************************************
  NAME         : AddGeneric
  DESCRIPTION  : Inserts a new generic function
                   header into the generic list
  INPUTS       : 1) Symbolic name of the new generic
                 2) Caller's input buffer for flag
                      if added generic is new or not
  RETURNS      : The address of the new node, or
                   address of old node if already present
  SIDE EFFECTS : Generic header inserted
                 If the node is already present, it is
                   moved to the end of the list, otherwise
                   the new node is inserted at the end
  NOTES        : None
 *********************************************************/
static DEFGENERIC *AddGeneric(
  void *theEnv,
  SYMBOL_HN *name,
  int *newGeneric)
  {
   DEFGENERIC *gfunc;

   gfunc = (DEFGENERIC *) EnvFindDefgeneric(theEnv,ValueToString(name));
   if (gfunc != NULL)
     {
      *newGeneric = FALSE;

      if (ConstructData(theEnv)->CheckSyntaxMode)
        { return(gfunc); }

      /* ================================
         The old trace state is preserved
         ================================ */
      RemoveConstructFromModule(theEnv,(struct constructHeader *) gfunc);
     }
   else
     {
      *newGeneric = TRUE;
      gfunc = NewGeneric(theEnv,name);
      IncrementSymbolCount(name);
      AddImplicitMethods(theEnv,gfunc);
     }
   AddConstructToModule((struct constructHeader *) gfunc);
   return(gfunc);
  }

/**********************************************************************
  NAME         : AddGenericMethod
  DESCRIPTION  : Inserts a blank method (with the method-index set)
                   into the specified position of the generic
                   method array
  INPUTS       : 1) The generic function
                 2) The index where to add the method in the array
                 3) The method user-index (0 if don't care)
  RETURNS      : The address of the new method
  SIDE EFFECTS : Fields initialized (index set) and new method inserted
                 Generic function new method-index set to specified
                   by user-index if > current new method-index
  NOTES        : None
 **********************************************************************/
static DEFMETHOD *AddGenericMethod(
  void *theEnv,
  DEFGENERIC *gfunc,
  int mposn,
  short mi)
  {
   DEFMETHOD *narr;
   long b, e;

   narr = (DEFMETHOD *) gm2(theEnv,(sizeof(DEFMETHOD) * (gfunc->mcnt+1)));
   for (b = e = 0 ; b < gfunc->mcnt ; b++ , e++)
     {
      if (b == mposn)
        e++;
      GenCopyMemory(DEFMETHOD,1,&narr[e],&gfunc->methods[b]);
     }
   if (mi == 0)
     narr[mposn].index = gfunc->new_index++;
   else
     {
      narr[mposn].index = mi;
      if (mi >= gfunc->new_index)
        gfunc->new_index = (short) (mi+1);
     }
   narr[mposn].busy = 0;
#if DEBUGGING_FUNCTIONS
   narr[mposn].trace = DefgenericData(theEnv)->WatchMethods;
#endif
   narr[mposn].minRestrictions = 0;
   narr[mposn].maxRestrictions = 0;
   narr[mposn].restrictionCount = 0;
   narr[mposn].localVarCount = 0;
   narr[mposn].system = 0;
   narr[mposn].restrictions = NULL;
   narr[mposn].actions = NULL;
   narr[mposn].ppForm = NULL;
   narr[mposn].usrData = NULL;
   if (gfunc->mcnt != 0)
     rm(theEnv,(void *) gfunc->methods,(sizeof(DEFMETHOD) * gfunc->mcnt));
   gfunc->mcnt++;
   gfunc->methods = narr;
   return(&narr[mposn]);
  }

/****************************************************************
  NAME         : RestrictionsCompare
  DESCRIPTION  : Compares the restriction-expression list
                   with an existing methods restrictions to
                   determine an ordering
  INPUTS       : 1) The parameter/restriction expression list
                 2) The total number of restrictions
                 3) The number of minimum restrictions
                 4) The number of maximum restrictions (-1
                    if unlimited)
                 5) The method with which to compare restrictions
  RETURNS      : A code representing how the method restrictions
                   -1 : New restrictions have higher precedence
                    0 : New restrictions are identical
                    1 : New restrictions have lower precedence
  SIDE EFFECTS : None
  NOTES        : The new restrictions are stored in the argList
                   pointers of the parameter expressions
 ****************************************************************/
static int RestrictionsCompare(
  EXPRESSION *params,
  int rcnt,
  int min,
  int max,
  DEFMETHOD *meth)
  {
   register int i;
   register RESTRICTION *r1,*r2;
   int diff = FALSE,rtn;

   for (i = 0 ; (i < rcnt) && (i < meth->restrictionCount) ; i++)
     {
      /* =============================================================
         A wildcard parameter always has lower precedence than
         a regular parameter, regardless of the class restriction list
         ============================================================= */
      if ((i == rcnt-1) && (max == -1) &&
          (meth->maxRestrictions != -1))
        return(LOWER_PRECEDENCE);
      if ((i == meth->restrictionCount-1) && (max != -1) &&
          (meth->maxRestrictions == -1))
        return(HIGHER_PRECEDENCE);

      /* =============================================================
         The parameter with the most specific type list has precedence
         ============================================================= */
      r1 = (RESTRICTION *) params->argList;
      r2 = &meth->restrictions[i];
      rtn = TypeListCompare(r1,r2);
      if (rtn != IDENTICAL)
        return(rtn);

      /* =====================================================
         The parameter with a query restriction has precedence
         ===================================================== */
      if ((r1->query == NULL) && (r2->query != NULL))
        return(LOWER_PRECEDENCE);
      if ((r1->query != NULL) && (r2->query == NULL))
        return(HIGHER_PRECEDENCE);

      /* ==========================================================
         Remember if the method restrictions differ at all - query
         expressions must be identical as well for the restrictions
         to be the same
         ========================================================== */
      if (IdenticalExpression(r1->query,r2->query) == FALSE)
        diff = TRUE;
      params = params->nextArg;
     }

   /* =============================================================
      If the methods have the same number of parameters here, they
      are either the same restrictions, or they differ only in
      the query restrictions
      ============================================================= */
   if (rcnt == meth->restrictionCount)
     return(diff ? LOWER_PRECEDENCE : IDENTICAL);

   /* =============================================
      The method with the greater number of regular
      parameters has precedence

      If they require the smae # of reg params,
      then one without a wildcard has precedence
      ============================================= */
   if (min > meth->minRestrictions)
     return(HIGHER_PRECEDENCE);
   if (meth->minRestrictions < min)
     return(LOWER_PRECEDENCE);
   return((max == - 1) ? LOWER_PRECEDENCE : HIGHER_PRECEDENCE);
  }

/*****************************************************
  NAME         : TypeListCompare
  DESCRIPTION  : Determines the precedence between
                   the class lists on two restrictions
  INPUTS       : 1) Restriction address #1
                 2) Restriction address #2
  RETURNS      : -1 : r1 precedes r2
                  0 : Identical classes
                  1 : r2 precedes r1
  SIDE EFFECTS : None
  NOTES        : None
 *****************************************************/
static int TypeListCompare(
  RESTRICTION *r1,
  RESTRICTION *r2)
  {
   long i;
   int diff = FALSE;

   if ((r1->tcnt == 0) && (r2->tcnt == 0))
     return(IDENTICAL);
   if (r1->tcnt == 0)
     return(LOWER_PRECEDENCE);
   if (r2->tcnt == 0)
     return(HIGHER_PRECEDENCE);
   for (i = 0 ; (i < r1->tcnt) && (i < r2->tcnt) ; i++)
     {
      if (r1->types[i] != r2->types[i])
        {
         diff = TRUE;
#if OBJECT_SYSTEM
         if (HasSuperclass((DEFCLASS *) r1->types[i],(DEFCLASS *) r2->types[i]))
           return(HIGHER_PRECEDENCE);
         if (HasSuperclass((DEFCLASS *) r2->types[i],(DEFCLASS *) r1->types[i]))
           return(LOWER_PRECEDENCE);
#else
         if (SubsumeType(ValueToInteger(r1->types[i]),ValueToInteger(r2->types[i])))
           return(HIGHER_PRECEDENCE);
         if (SubsumeType(ValueToInteger(r2->types[i]),ValueToInteger(r1->types[i])))
           return(LOWER_PRECEDENCE);
#endif
        }
     }
   if (r1->tcnt < r2->tcnt)
     return(HIGHER_PRECEDENCE);
   if (r1->tcnt > r2->tcnt)
     return(LOWER_PRECEDENCE);
   if (diff)
     return(LOWER_PRECEDENCE);
   return(IDENTICAL);
  }

/***************************************************
  NAME         : NewGeneric
  DESCRIPTION  : Allocates and initializes a new
                   generic function header
  INPUTS       : The name of the new generic
  RETURNS      : The address of the new generic
  SIDE EFFECTS : Generic function  header created
  NOTES        : None
 ***************************************************/
static DEFGENERIC *NewGeneric(
  void *theEnv,
  SYMBOL_HN *gname)
  {
   DEFGENERIC *ngen;

   ngen = get_struct(theEnv,defgeneric);
   InitializeConstructHeader(theEnv,"defgeneric",(struct constructHeader *) ngen,gname);
   ngen->busy = 0;
   ngen->new_index = 1;
   ngen->methods = NULL;
   ngen->mcnt = 0;
#if DEBUGGING_FUNCTIONS
   ngen->trace = DefgenericData(theEnv)->WatchGenerics;
#endif
   return(ngen);
  }

#endif /* DEFGENERIC_CONSTRUCT && (! BLOAD_ONLY) && (! RUN_TIME) */

/***************************************************
  NAME         :
  DESCRIPTION  :
  INPUTS       :
  RETURNS      :
  SIDE EFFECTS :
  NOTES        :
 ***************************************************/
