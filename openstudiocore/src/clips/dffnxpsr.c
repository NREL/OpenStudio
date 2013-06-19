   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Deffunction Parsing Routines                     */
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
/*            Added pragmas to prevent unused variable       */
/*            warnings.                                      */
/*                                                           */
/*************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if DEFFUNCTION_CONSTRUCT && (! BLOAD_ONLY) && (! RUN_TIME)

#if BLOAD || BLOAD_AND_BSAVE
#include "bload.h"
#endif

#if DEFRULE_CONSTRUCT
#include "network.h"
#endif

#if DEFGENERIC_CONSTRUCT
#include "genrccom.h"
#endif

#include "constant.h"
#include "cstrcpsr.h"
#include "constrct.h"
#include "dffnxfun.h"
#include "envrnmnt.h"
#include "expressn.h"
#include "exprnpsr.h"
#include "extnfunc.h"
#include "memalloc.h"
#include "prccode.h"
#include "router.h"
#include "scanner.h"
#include "symbol.h"

#define _DFFNXPSR_SOURCE_
#include "dffnxpsr.h"

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static intBool ValidDeffunctionName(void *,char *);
static DEFFUNCTION *AddDeffunction(void *,SYMBOL_HN *,EXPRESSION *,int,int,int,int);

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***************************************************************************
  NAME         : ParseDeffunction
  DESCRIPTION  : Parses the deffunction construct
  INPUTS       : The input logical name
  RETURNS      : FALSE if successful parse, TRUE otherwise
  SIDE EFFECTS : Creates valid deffunction definition
  NOTES        : H/L Syntax :
                 (deffunction <name> [<comment>]
                    (<single-field-varible>* [<multifield-variable>])
                    <action>*)
 ***************************************************************************/
globle intBool ParseDeffunction(
  void *theEnv,
  char *readSource)
  {
   SYMBOL_HN *deffunctionName;
   EXPRESSION *actions;
   EXPRESSION *parameterList;
   SYMBOL_HN *wildcard;
   int min,max,lvars,DeffunctionError = FALSE;
   short overwrite = FALSE, owMin = 0, owMax = 0;
   DEFFUNCTION *dptr;

   SetPPBufferStatus(theEnv,ON);

   FlushPPBuffer(theEnv);
   SetIndentDepth(theEnv,3);
   SavePPBuffer(theEnv,"(deffunction ");

#if BLOAD || BLOAD_AND_BSAVE
   if ((Bloaded(theEnv) == TRUE) && (! ConstructData(theEnv)->CheckSyntaxMode))
     {
      CannotLoadWithBloadMessage(theEnv,"deffunctions");
      return(TRUE);
     }
#endif

   /* =====================================================
      Parse the name and comment fields of the deffunction.
      ===================================================== */
   deffunctionName = GetConstructNameAndComment(theEnv,readSource,&DeffunctionData(theEnv)->DFInputToken,"deffunction",
                                                EnvFindDeffunction,NULL,
                                                "!",TRUE,TRUE,TRUE);
   if (deffunctionName == NULL)
     return(TRUE);

   if (ValidDeffunctionName(theEnv,ValueToString(deffunctionName)) == FALSE)
     return(TRUE);

   /*==========================*/
   /* Parse the argument list. */
   /*==========================*/
   parameterList = ParseProcParameters(theEnv,readSource,&DeffunctionData(theEnv)->DFInputToken,NULL,&wildcard,
                                       &min,&max,&DeffunctionError,NULL);
   if (DeffunctionError)
     return(TRUE);

   /*===================================================================*/
   /* Go ahead and add the deffunction so it can be recursively called. */
   /*===================================================================*/

   if (ConstructData(theEnv)->CheckSyntaxMode)
     {
      dptr = (DEFFUNCTION *) EnvFindDeffunction(theEnv,ValueToString(deffunctionName));
      if (dptr == NULL)
        { dptr = AddDeffunction(theEnv,deffunctionName,NULL,min,max,0,TRUE); }
      else
        {
         overwrite = TRUE;
         owMin = (short) dptr->minNumberOfParameters;
         owMax = (short) dptr->maxNumberOfParameters;
         dptr->minNumberOfParameters = min;
         dptr->maxNumberOfParameters = max;
        }
     }
   else
     { dptr = AddDeffunction(theEnv,deffunctionName,NULL,min,max,0,TRUE); }

   if (dptr == NULL)
     {
      ReturnExpression(theEnv,parameterList);
      return(TRUE);
     }

   /*==================================================*/
   /* Parse the actions contained within the function. */
   /*==================================================*/

   PPCRAndIndent(theEnv);

   ExpressionData(theEnv)->ReturnContext = TRUE;
   actions = ParseProcActions(theEnv,"deffunction",readSource,
                              &DeffunctionData(theEnv)->DFInputToken,parameterList,wildcard,
                              NULL,NULL,&lvars,NULL);

   /*=============================================================*/
   /* Check for the closing right parenthesis of the deffunction. */
   /*=============================================================*/

   if ((DeffunctionData(theEnv)->DFInputToken.type != RPAREN) && /* DR0872 */
       (actions != NULL))
     {
      SyntaxErrorMessage(theEnv,"deffunction");
      
      ReturnExpression(theEnv,parameterList);
      ReturnPackedExpression(theEnv,actions);

      if (overwrite)
        {
         dptr->minNumberOfParameters = owMin;
         dptr->maxNumberOfParameters = owMax;
        }

      if ((dptr->busy == 0) && (! overwrite))
        {
         RemoveConstructFromModule(theEnv,(struct constructHeader *) dptr);
         RemoveDeffunction(theEnv,dptr);
        }

      return(TRUE);
     }

   if (actions == NULL)
     {
      ReturnExpression(theEnv,parameterList);
      if (overwrite)
        {
         dptr->minNumberOfParameters = owMin;
         dptr->maxNumberOfParameters = owMax;
        }

      if ((dptr->busy == 0) && (! overwrite))
        {
         RemoveConstructFromModule(theEnv,(struct constructHeader *) dptr);
         RemoveDeffunction(theEnv,dptr);
        }
      return(TRUE);
     }

   /*==============================================*/
   /* If we're only checking syntax, don't add the */
   /* successfully parsed deffunction to the KB.   */
   /*==============================================*/

   if (ConstructData(theEnv)->CheckSyntaxMode)
     {
      ReturnExpression(theEnv,parameterList);
      ReturnPackedExpression(theEnv,actions);
      if (overwrite)
        {
         dptr->minNumberOfParameters = owMin;
         dptr->maxNumberOfParameters = owMax;
        }
      else
        {
         RemoveConstructFromModule(theEnv,(struct constructHeader *) dptr);
         RemoveDeffunction(theEnv,dptr);
        }
      return(FALSE);
     }

   /*=============================*/
   /* Reformat the closing token. */
   /*=============================*/

   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,DeffunctionData(theEnv)->DFInputToken.printForm);
   SavePPBuffer(theEnv,"\n");

   /*======================*/
   /* Add the deffunction. */
   /*======================*/

   AddDeffunction(theEnv,deffunctionName,actions,min,max,lvars,FALSE);

   ReturnExpression(theEnv,parameterList);

   return(DeffunctionError);
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/************************************************************
  NAME         : ValidDeffunctionName
  DESCRIPTION  : Determines if a new deffunction of the given
                 name can be defined in the current module
  INPUTS       : The new deffunction name
  RETURNS      : TRUE if OK, FALSE otherwise
  SIDE EFFECTS : Error message printed if not OK
  NOTES        : GetConstructNameAndComment() (called before
                 this function) ensures that the deffunction
                 name does not conflict with one from
                 another module
 ************************************************************/
static intBool ValidDeffunctionName(
  void *theEnv,
  char *theDeffunctionName)
  {
   struct constructHeader *theDeffunction;
#if DEFGENERIC_CONSTRUCT
   struct defmodule *theModule;
   struct constructHeader *theDefgeneric;
#endif

   /* ============================================
      A deffunction cannot be named the same as a
      construct type, e.g, defclass, defrule, etc.
      ============================================ */
   if (FindConstruct(theEnv,theDeffunctionName) != NULL)
     {
      PrintErrorID(theEnv,"DFFNXPSR",1,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Deffunctions are not allowed to replace constructs.\n");
      return(FALSE);
     }

   /* ============================================
      A deffunction cannot be named the same as a
      pre-defined system function, e.g, watch,
      list-defrules, etc.
      ============================================ */
   if (FindFunction(theEnv,theDeffunctionName) != NULL)
     {
      PrintErrorID(theEnv,"DFFNXPSR",2,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Deffunctions are not allowed to replace external functions.\n");
      return(FALSE);
     }

#if DEFGENERIC_CONSTRUCT
   /* ============================================
      A deffunction cannot be named the same as a
      generic function (either in this module or
      imported from another)
      ============================================ */
   theDefgeneric =
     (struct constructHeader *) LookupDefgenericInScope(theEnv,theDeffunctionName);
   if (theDefgeneric != NULL)
     {
      theModule = GetConstructModuleItem(theDefgeneric)->theModule;
      if (theModule != ((struct defmodule *) EnvGetCurrentModule(theEnv)))
        {
         PrintErrorID(theEnv,"DFFNXPSR",5,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Defgeneric ");
         EnvPrintRouter(theEnv,WERROR,EnvGetDefgenericName(theEnv,(void *) theDefgeneric));
         EnvPrintRouter(theEnv,WERROR," imported from module ");
         EnvPrintRouter(theEnv,WERROR,EnvGetDefmoduleName(theEnv,(void *) theModule));
         EnvPrintRouter(theEnv,WERROR," conflicts with this deffunction.\n");
         return(FALSE);
        }
      else
        {
         PrintErrorID(theEnv,"DFFNXPSR",3,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Deffunctions are not allowed to replace generic functions.\n");
        }
      return(FALSE);
     }
#endif

   theDeffunction = (struct constructHeader *) EnvFindDeffunction(theEnv,theDeffunctionName);
   if (theDeffunction != NULL)
     {
      /* ===========================================
         And a deffunction in the current module can
         only be redefined if it is not executing.
         =========================================== */
      if (((DEFFUNCTION *) theDeffunction)->executing)
        {
         PrintErrorID(theEnv,"DFNXPSR",4,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Deffunction ");
         EnvPrintRouter(theEnv,WERROR,EnvGetDeffunctionName(theEnv,(void *) theDeffunction));
         EnvPrintRouter(theEnv,WERROR," may not be redefined while it is executing.\n");
         return(FALSE);
        }
     }
   return(TRUE);
  }


/****************************************************
  NAME         : AddDeffunction
  DESCRIPTION  : Adds a deffunction to the list of
                 deffunctions
  INPUTS       : 1) The symbolic name
                 2) The action expressions
                 3) The minimum number of arguments
                 4) The maximum number of arguments
                    (can be -1)
                 5) The number of local variables
                 6) A flag indicating if this is
                    a header call so that the
                    deffunction can be recursively
                    called
  RETURNS      : The new deffunction (NULL on errors)
  SIDE EFFECTS : Deffunction structures allocated
  NOTES        : Assumes deffunction is not executing
 ****************************************************/
#if WIN_BTC
#pragma argsused
#endif
static DEFFUNCTION *AddDeffunction(
  void *theEnv,
  SYMBOL_HN *name,
  EXPRESSION *actions,
  int min,
  int max,
  int lvars,
  int headerp)
  {
   DEFFUNCTION *dfuncPtr;
   unsigned oldbusy;
#if DEBUGGING_FUNCTIONS
   unsigned DFHadWatch = FALSE;
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(headerp)
#endif
#endif

   /*===============================================================*/
   /* If the deffunction doesn't exist, create a new structure to   */
   /* contain it and add it to the List of deffunctions. Otherwise, */
   /* use the existing structure and remove the pretty print form   */
   /* and interpretive code.                                        */
   /*===============================================================*/
   dfuncPtr = (DEFFUNCTION *) EnvFindDeffunction(theEnv,ValueToString(name));
   if (dfuncPtr == NULL)
     {
      dfuncPtr = get_struct(theEnv,deffunctionStruct);
      InitializeConstructHeader(theEnv,"deffunction",(struct constructHeader *) dfuncPtr,name);
      IncrementSymbolCount(name);
      dfuncPtr->code = NULL;
      dfuncPtr->minNumberOfParameters = min;
      dfuncPtr->maxNumberOfParameters = max;
      dfuncPtr->numberOfLocalVars = lvars;
      dfuncPtr->busy = 0;
      dfuncPtr->executing = 0;
     }
   else
     {
#if DEBUGGING_FUNCTIONS
      DFHadWatch = EnvGetDeffunctionWatch(theEnv,(void *) dfuncPtr);
#endif
      dfuncPtr->minNumberOfParameters = min;
      dfuncPtr->maxNumberOfParameters = max;
      dfuncPtr->numberOfLocalVars = lvars;
      oldbusy = dfuncPtr->busy;
      ExpressionDeinstall(theEnv,dfuncPtr->code);
      dfuncPtr->busy = oldbusy;
      ReturnPackedExpression(theEnv,dfuncPtr->code);
      dfuncPtr->code = NULL;
      SetDeffunctionPPForm((void *) dfuncPtr,NULL);

      /* =======================================
         Remove the deffunction from the list so
         that it can be added at the end
         ======================================= */
      RemoveConstructFromModule(theEnv,(struct constructHeader *) dfuncPtr);
     }

   AddConstructToModule((struct constructHeader *) dfuncPtr);

   /* ==================================
      Install the new interpretive code.
      ================================== */

   if (actions != NULL)
     {
      /* ===============================
         If a deffunction is recursive,
         do not increment its busy count
         based on self-references
         =============================== */
      oldbusy = dfuncPtr->busy;
      ExpressionInstall(theEnv,actions);
      dfuncPtr->busy = oldbusy;
      dfuncPtr->code = actions;
     }

   /* ===============================================================
      Install the pretty print form if memory is not being conserved.
      =============================================================== */

#if DEBUGGING_FUNCTIONS
   EnvSetDeffunctionWatch(theEnv,DFHadWatch ? TRUE : DeffunctionData(theEnv)->WatchDeffunctions,(void *) dfuncPtr);
   if ((EnvGetConserveMemory(theEnv) == FALSE) && (headerp == FALSE))
     SetDeffunctionPPForm((void *) dfuncPtr,CopyPPBuffer(theEnv));
#endif
   return(dfuncPtr);
  }

#endif /* DEFFUNCTION_CONSTRUCT && (! BLOAD_ONLY) && (! RUN_TIME) */

