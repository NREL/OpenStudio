   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.22  06/15/04          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Binary Load/Save Functions for Deffunctions      */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if DEFFUNCTION_CONSTRUCT && (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE)

#include "bload.h"
#include "bsave.h"

#include "memalloc.h"
#include "cstrcbin.h"
#include "envrnmnt.h"
#include "modulbin.h"

#define _DFFNXBIN_SOURCE_
#include "dffnxbin.h"

/* =========================================
   *****************************************
                   CONSTANTS
   =========================================
   ***************************************** */

/* =========================================
   *****************************************
               MACROS AND TYPES
   =========================================
   ***************************************** */
typedef struct bsaveDeffunctionModule
  {
   struct bsaveDefmoduleItemHeader header;
  } BSAVE_DEFFUNCTION_MODULE;

typedef struct bsaveDeffunctionStruct
  {
   struct bsaveConstructHeader header;
   int minNumberOfParameters,
       maxNumberOfParameters,
       numberOfLocalVars;
   long name,
        code;
  } BSAVE_DEFFUNCTION;

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

#if BLOAD_AND_BSAVE
static void BsaveDeffunctionFind(void *);
static void MarkDeffunctionItems(void *,struct constructHeader *,void *);
static void BsaveDeffunctionExpressions(void *,FILE *);
static void BsaveDeffunctionExpression(void *,struct constructHeader *,void *);
static void BsaveStorageDeffunctions(void *,FILE *);
static void BsaveDeffunctions(void *,FILE *);
static void BsaveDeffunction(void *,struct constructHeader *,void *);
#endif

static void BloadStorageDeffunctions(void *);
static void BloadDeffunctions(void *);
static void UpdateDeffunctionModule(void *,void *,long);
static void UpdateDeffunction(void *,void *,long);
static void ClearDeffunctionBload(void *);
static void DeallocateDeffunctionBloadData(void *);

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***********************************************************
  NAME         : SetupDeffunctionsBload
  DESCRIPTION  : Initializes data structures and
                   routines for binary loads of deffunctions
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Routines defined and structures initialized
  NOTES        : None
 ***********************************************************/
globle void SetupDeffunctionsBload(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,DFFNXBIN_DATA,sizeof(struct deffunctionBinaryData),DeallocateDeffunctionBloadData);
#if BLOAD_AND_BSAVE
   AddBinaryItem(theEnv,"deffunctions",0,BsaveDeffunctionFind,BsaveDeffunctionExpressions,
                             BsaveStorageDeffunctions,BsaveDeffunctions,
                             BloadStorageDeffunctions,BloadDeffunctions,
                             ClearDeffunctionBload);
#else
   AddBinaryItem(theEnv,"deffunctions",0,NULL,NULL,NULL,NULL,
                             BloadStorageDeffunctions,BloadDeffunctions,
                             ClearDeffunctionBload);
#endif
  }
  
/***********************************************************/
/* DeallocateDeffunctionBloadData: Deallocates environment */
/*    data for the deffunction bsave functionality.        */
/***********************************************************/
static void DeallocateDeffunctionBloadData(
  void *theEnv)
  {
   size_t space;

#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME)
   space = DeffunctionBinaryData(theEnv)->DeffunctionCount * sizeof(struct deffunctionStruct);
   if (space != 0) genfree(theEnv,(void *) DeffunctionBinaryData(theEnv)->DeffunctionArray,space);

   space =  DeffunctionBinaryData(theEnv)->ModuleCount * sizeof(struct deffunctionModule);
   if (space != 0) genfree(theEnv,(void *) DeffunctionBinaryData(theEnv)->ModuleArray,space);
#endif
  }

/***************************************************
  NAME         : BloadDeffunctionModuleReference
  DESCRIPTION  : Returns a pointer to the
                 appropriate deffunction module
  INPUTS       : The index of the module
  RETURNS      : A pointer to the module
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
globle void *BloadDeffunctionModuleReference(
  void *theEnv,
  int theIndex)
  {
   return ((void *) &DeffunctionBinaryData(theEnv)->ModuleArray[theIndex]);
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

#if BLOAD_AND_BSAVE

/***************************************************************************
  NAME         : BsaveDeffunctionFind
  DESCRIPTION  : For all deffunctions, this routine marks all
                   the needed symbols.
                 Also, it also counts the number of
                   expression structures needed.
                 Also, counts total number of deffunctions.
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : ExpressionCount (a global from BSAVE.C) is incremented
                   for every expression needed
                 Symbols are marked in their structures
  NOTES        : Also sets bsaveIndex for each deffunction (assumes
                   deffunctions will be bsaved in order of binary list)
 ***************************************************************************/
static void BsaveDeffunctionFind(
  void *theEnv)
  {
   SaveBloadCount(theEnv,DeffunctionBinaryData(theEnv)->ModuleCount);
   SaveBloadCount(theEnv,DeffunctionBinaryData(theEnv)->DeffunctionCount);
   DeffunctionBinaryData(theEnv)->DeffunctionCount = 0L;

   DeffunctionBinaryData(theEnv)->ModuleCount = 
      DoForAllConstructs(theEnv,MarkDeffunctionItems,DeffunctionData(theEnv)->DeffunctionModuleIndex,
                         FALSE,NULL);
  }

/***************************************************
  NAME         : MarkDeffunctionItems
  DESCRIPTION  : Marks the needed items for
                 a deffunction bsave
  INPUTS       : 1) The deffunction
                 2) User data buffer (ignored)
  RETURNS      : Nothing useful
  SIDE EFFECTS : Needed items marked
  NOTES        : None
 ***************************************************/
#if WIN_BTC
#pragma argsused
#endif
static void MarkDeffunctionItems(
  void *theEnv,
  struct constructHeader *theDeffunction,
  void *userBuffer)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(userBuffer)
#endif

   MarkConstructHeaderNeededItems(theDeffunction,DeffunctionBinaryData(theEnv)->DeffunctionCount++);
   ExpressionData(theEnv)->ExpressionCount += ExpressionSize(((DEFFUNCTION *) theDeffunction)->code);
   MarkNeededItems(theEnv,((DEFFUNCTION *) theDeffunction)->code);
  }

/***************************************************
  NAME         : BsaveDeffunctionExpressions
  DESCRIPTION  : Writes out all expressions needed
                   by deffunctyions
  INPUTS       : The file pointer of the binary file
  RETURNS      : Nothing useful
  SIDE EFFECTS : File updated
  NOTES        : None
 ***************************************************/
static void BsaveDeffunctionExpressions(
  void *theEnv,
  FILE *fp)
  {
   DoForAllConstructs(theEnv,BsaveDeffunctionExpression,DeffunctionData(theEnv)->DeffunctionModuleIndex,
                      FALSE,(void *) fp);
  }

/***************************************************
  NAME         : BsaveDeffunctionExpression
  DESCRIPTION  : Saves the needed expressions for
                 a deffunction bsave
  INPUTS       : 1) The deffunction
                 2) Output data file pointer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Expressions saved
  NOTES        : None
 ***************************************************/
static void BsaveDeffunctionExpression(
  void *theEnv,
  struct constructHeader *theDeffunction,
  void *userBuffer)
  {
   BsaveExpression(theEnv,((DEFFUNCTION *) theDeffunction)->code,(FILE *) userBuffer);
  }

/***********************************************************
  NAME         : BsaveStorageDeffunctions
  DESCRIPTION  : Writes out number of each type of structure
                   required for deffunctions
                 Space required for counts (unsigned long)
  INPUTS       : File pointer of binary file
  RETURNS      : Nothing useful
  SIDE EFFECTS : Binary file adjusted
  NOTES        : None
 ***********************************************************/
static void BsaveStorageDeffunctions(
  void *theEnv,
  FILE *fp)
  {
   size_t space;

   space = sizeof(unsigned long) * 2;
   GenWrite((void *) &space,sizeof(size_t),fp);
   GenWrite((void *) &DeffunctionBinaryData(theEnv)->ModuleCount,sizeof(unsigned long),fp);
   GenWrite((void *) &DeffunctionBinaryData(theEnv)->DeffunctionCount,sizeof(unsigned long),fp);
  }

/*************************************************************************************
  NAME         : BsaveDeffunctions
  DESCRIPTION  : Writes out deffunction in binary format
                 Space required (unsigned long)
                 All deffunctions (sizeof(DEFFUNCTION) * Number of deffunctions)
  INPUTS       : File pointer of binary file
  RETURNS      : Nothing useful
  SIDE EFFECTS : Binary file adjusted
  NOTES        : None
 *************************************************************************************/
static void BsaveDeffunctions(
  void *theEnv,
  FILE *fp)
  {
   size_t space;
   struct defmodule *theModule;
   DEFFUNCTION_MODULE *theModuleItem;
   BSAVE_DEFFUNCTION_MODULE dummy_mitem;

   space = ((sizeof(BSAVE_DEFFUNCTION_MODULE) * DeffunctionBinaryData(theEnv)->ModuleCount) +
            (sizeof(BSAVE_DEFFUNCTION) * DeffunctionBinaryData(theEnv)->DeffunctionCount));
   GenWrite((void *) &space,sizeof(size_t),fp);

   /* =================================
      Write out each deffunction module
      ================================= */
   DeffunctionBinaryData(theEnv)->DeffunctionCount = 0L;
   theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
   while (theModule != NULL)
     {
      theModuleItem = (DEFFUNCTION_MODULE *)
                      GetModuleItem(theEnv,theModule,FindModuleItem(theEnv,"deffunction")->moduleIndex);
      AssignBsaveDefmdlItemHdrVals(&dummy_mitem.header,&theModuleItem->header);
      GenWrite((void *) &dummy_mitem,sizeof(BSAVE_DEFFUNCTION_MODULE),fp);
      theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,(void *) theModule);
     }

   /* ==========================
      Write out each deffunction
      ========================== */
   DoForAllConstructs(theEnv,BsaveDeffunction,DeffunctionData(theEnv)->DeffunctionModuleIndex,
                      FALSE,(void *) fp);

   RestoreBloadCount(theEnv,&DeffunctionBinaryData(theEnv)->ModuleCount);
   RestoreBloadCount(theEnv,&DeffunctionBinaryData(theEnv)->DeffunctionCount);
  }

/***************************************************
  NAME         : BsaveDeffunction
  DESCRIPTION  : Bsaves a deffunction
  INPUTS       : 1) The deffunction
                 2) Output data file pointer
  RETURNS      : Nothing useful
  SIDE EFFECTS : Deffunction saved
  NOTES        : None
 ***************************************************/
static void BsaveDeffunction(
  void *theEnv,
  struct constructHeader *theDeffunction,
  void *userBuffer)
  {
   DEFFUNCTION *dptr = (DEFFUNCTION *) theDeffunction;
   BSAVE_DEFFUNCTION dummy_df;

   AssignBsaveConstructHeaderVals(&dummy_df.header,&dptr->header);
   dummy_df.minNumberOfParameters = dptr->minNumberOfParameters;
   dummy_df.maxNumberOfParameters = dptr->maxNumberOfParameters;
   dummy_df.numberOfLocalVars = dptr->numberOfLocalVars;
   if (dptr->code != NULL)
     {
      dummy_df.code = ExpressionData(theEnv)->ExpressionCount;
      ExpressionData(theEnv)->ExpressionCount += ExpressionSize(dptr->code);
     }
   else
     dummy_df.code = -1L;
   GenWrite((void *) &dummy_df,sizeof(BSAVE_DEFFUNCTION),(FILE *) userBuffer);
  }

#endif

/***********************************************************************
  NAME         : BloadStorageDeffunctions
  DESCRIPTION  : This routine space required for deffunction
                   structures and allocates space for them
  INPUTS       : Nothing
  RETURNS      : Nothing useful
  SIDE EFFECTS : Arrays allocated and set
  NOTES        : This routine makes no attempt to reset any pointers
                   within the structures
 ***********************************************************************/
static void BloadStorageDeffunctions(
  void *theEnv)
  {
   size_t space;

   GenReadBinary(theEnv,(void *) &space,sizeof(size_t));
   if (space == 0L)
     return;
   GenReadBinary(theEnv,(void *) &DeffunctionBinaryData(theEnv)->ModuleCount,sizeof(unsigned long));
   GenReadBinary(theEnv,(void *) &DeffunctionBinaryData(theEnv)->DeffunctionCount,sizeof(unsigned long));
   if (DeffunctionBinaryData(theEnv)->ModuleCount == 0L)
     {
      DeffunctionBinaryData(theEnv)->ModuleArray = NULL;
      DeffunctionBinaryData(theEnv)->DeffunctionArray = NULL;
      return;
     }

   space = (DeffunctionBinaryData(theEnv)->ModuleCount * sizeof(DEFFUNCTION_MODULE));
   DeffunctionBinaryData(theEnv)->ModuleArray = (DEFFUNCTION_MODULE *) genalloc(theEnv,space);

   if (DeffunctionBinaryData(theEnv)->DeffunctionCount == 0L)
     {
      DeffunctionBinaryData(theEnv)->DeffunctionArray = NULL;
      return;
     }

   space = (DeffunctionBinaryData(theEnv)->DeffunctionCount * sizeof(DEFFUNCTION));
   DeffunctionBinaryData(theEnv)->DeffunctionArray = (DEFFUNCTION *) genalloc(theEnv,space);
  }

/*********************************************************************
  NAME         : BloadDeffunctions
  DESCRIPTION  : This routine reads deffunction information from
                   a binary file
                 This routine moves through the deffunction
                   binary array updating pointers
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Pointers reset from array indices
  NOTES        : Assumes all loading is finished
 ********************************************************************/
static void BloadDeffunctions(
  void *theEnv)
  {
   size_t space;

   GenReadBinary(theEnv,(void *) &space,sizeof(size_t));
   BloadandRefresh(theEnv,DeffunctionBinaryData(theEnv)->ModuleCount,sizeof(BSAVE_DEFFUNCTION_MODULE),UpdateDeffunctionModule);
   BloadandRefresh(theEnv,DeffunctionBinaryData(theEnv)->DeffunctionCount,sizeof(BSAVE_DEFFUNCTION),UpdateDeffunction);
  }

/*******************************************************
  NAME         : UpdateDeffunctionModule
  DESCRIPTION  : Updates deffunction module with binary
                 load data - sets pointers from
                 offset information
  INPUTS       : 1) A pointer to the bloaded data
                 2) The index of the binary array
                    element to update
  RETURNS      : Nothing useful
  SIDE EFFECTS : Deffunction moudle pointers updated
  NOTES        : None
 *******************************************************/
static void UpdateDeffunctionModule(
  void *theEnv,
  void *buf,
  long obji)
  {
   BSAVE_DEFFUNCTION_MODULE *bdptr;

   bdptr = (BSAVE_DEFFUNCTION_MODULE *) buf;
   UpdateDefmoduleItemHeader(theEnv,&bdptr->header,&DeffunctionBinaryData(theEnv)->ModuleArray[obji].header,
                             (int) sizeof(DEFFUNCTION),(void *) DeffunctionBinaryData(theEnv)->DeffunctionArray);
  }

/***************************************************
  NAME         : UpdateDeffunction
  DESCRIPTION  : Updates deffunction with binary
                 load data - sets pointers from
                 offset information
  INPUTS       : 1) A pointer to the bloaded data
                 2) The index of the binary array
                    element to update
  RETURNS      : Nothing useful
  SIDE EFFECTS : Deffunction pointers upadted
  NOTES        : None
 ***************************************************/
static void UpdateDeffunction(
  void *theEnv,
  void *buf,
  long obji)
  {
   BSAVE_DEFFUNCTION *bdptr;
   DEFFUNCTION *dptr;

   bdptr = (BSAVE_DEFFUNCTION *) buf;
   dptr = (DEFFUNCTION *) &DeffunctionBinaryData(theEnv)->DeffunctionArray[obji];

   UpdateConstructHeader(theEnv,&bdptr->header,&dptr->header,
                         (int) sizeof(DEFFUNCTION_MODULE),(void *) DeffunctionBinaryData(theEnv)->ModuleArray,
                         (int) sizeof(DEFFUNCTION),(void *) DeffunctionBinaryData(theEnv)->DeffunctionArray);

   dptr->code = ExpressionPointer(bdptr->code);
   dptr->busy = 0;
   dptr->executing = 0;
#if DEBUGGING_FUNCTIONS
   dptr->trace = (unsigned short) DeffunctionData(theEnv)->WatchDeffunctions;
#endif
   dptr->minNumberOfParameters = bdptr->minNumberOfParameters;
   dptr->maxNumberOfParameters = bdptr->maxNumberOfParameters;
   dptr->numberOfLocalVars = bdptr->numberOfLocalVars;
  }

/***************************************************************
  NAME         : ClearDeffunctionBload
  DESCRIPTION  : Release all binary-loaded deffunction
                   structure arrays
                 Resets deffunction list to NULL
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Memory cleared
  NOTES        : Deffunction name symbol counts decremented
 ***************************************************************/
static void ClearDeffunctionBload(
  void *theEnv)
  {
   register long i;
   size_t space;

   space = (sizeof(DEFFUNCTION_MODULE) * DeffunctionBinaryData(theEnv)->ModuleCount);
   if (space == 0L)
     return;
   genfree(theEnv,(void *) DeffunctionBinaryData(theEnv)->ModuleArray,space);
   DeffunctionBinaryData(theEnv)->ModuleArray = NULL;
   DeffunctionBinaryData(theEnv)->ModuleCount = 0L;

   for (i = 0L ; i < DeffunctionBinaryData(theEnv)->DeffunctionCount ; i++)
     UnmarkConstructHeader(theEnv,&DeffunctionBinaryData(theEnv)->DeffunctionArray[i].header);
   space = (sizeof(DEFFUNCTION) * DeffunctionBinaryData(theEnv)->DeffunctionCount);
   if (space == 0L)
     return;
   genfree(theEnv,(void *) DeffunctionBinaryData(theEnv)->DeffunctionArray,space);
   DeffunctionBinaryData(theEnv)->DeffunctionArray = NULL;
   DeffunctionBinaryData(theEnv)->DeffunctionCount = 0L;
  }

#endif

