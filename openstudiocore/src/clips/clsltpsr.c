   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  05/17/06          */
   /*                                                     */
   /*                  CLASS PARSER MODULE                */
   /*******************************************************/

/**************************************************************/
/* Purpose: Parsing Routines for Defclass Construct           */
/*                                                            */
/* Principal Programmer(s):                                   */
/*      Brian L. Dantes                                       */
/*                                                            */
/* Contributing Programmer(s):                                */
/*                                                            */
/* Revision History:                                          */
/*                                                            */
/*      6.24: Converted INSTANCE_PATTERN_MATCHING to          */
/*            DEFRULE_CONSTRUCT.                              */
/*                                                            */
/*            Renamed BOOLEAN macro type to intBool.          */
/*                                                            */
/**************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if OBJECT_SYSTEM && (! BLOAD_ONLY) && (! RUN_TIME)

#include <string.h>

#include "classcom.h"
#include "classfun.h"
#include "cstrnchk.h"
#include "cstrnpsr.h"
#include "cstrnutl.h"
#include "default.h"
#include "envrnmnt.h"
#include "insfun.h"
#include "memalloc.h"
#include "prntutil.h"
#include "router.h"
#include "scanner.h"

#define _CLSLTPSR_SOURCE_
#include "clsltpsr.h"

/* =========================================
   *****************************************
                   CONSTANTS
   =========================================
   ***************************************** */
#define DEFAULT_FACET         "default"
#define DYNAMIC_FACET         "default-dynamic"
#define VARIABLE_VAR          "VARIABLE"

#define STORAGE_FACET         "storage"
#define SLOT_SHARE_RLN        "shared"
#define SLOT_LOCAL_RLN        "local"

#define ACCESS_FACET          "access"
#define SLOT_RDONLY_RLN       "read-only"
#define SLOT_RDWRT_RLN        "read-write"
#define SLOT_INIT_RLN         "initialize-only"

#define PROPAGATION_FACET     "propagation"
#define SLOT_NO_INH_RLN       "no-inherit"
#define SLOT_INH_RLN          "inherit"

#define SOURCE_FACET          "source"
#define SLOT_COMPOSITE_RLN    "composite"
#define SLOT_EXCLUSIVE_RLN    "exclusive"

#define MATCH_FACET           MATCH_RLN
#define SLOT_REACTIVE_RLN     REACTIVE_RLN
#define SLOT_NONREACTIVE_RLN  NONREACTIVE_RLN

#define VISIBILITY_FACET      "visibility"
#define SLOT_PUBLIC_RLN       "public"
#define SLOT_PRIVATE_RLN      "private"

#define CREATE_ACCESSOR_FACET "create-accessor"
#define SLOT_READ_RLN         "read"
#define SLOT_WRITE_RLN        "write"
#define SLOT_NONE_RLN         "NONE"

#define OVERRIDE_MSG_FACET    "override-message"
#define SLOT_DEFAULT_RLN      "DEFAULT"

#define STORAGE_BIT           0
#define FIELD_BIT             1
#define ACCESS_BIT            2
#define PROPAGATION_BIT       3
#define SOURCE_BIT            4
#define MATCH_BIT             5
#define DEFAULT_BIT           6
#define DEFAULT_DYNAMIC_BIT   7
#define VISIBILITY_BIT        8
#define CREATE_ACCESSOR_BIT   9
#define OVERRIDE_MSG_BIT      10

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static SLOT_DESC *NewSlot(void *,SYMBOL_HN *);
static TEMP_SLOT_LINK *InsertSlot(void *,TEMP_SLOT_LINK *,SLOT_DESC *);
static int ParseSimpleFacet(void *,char *,char*,char *,int,char *,char *,char *,char *,SYMBOL_HN **);
static intBool ParseDefaultFacet(void *,char *,char *,SLOT_DESC *);
static void BuildCompositeFacets(void *,SLOT_DESC *,PACKED_CLASS_LINKS *,char *,
                                 CONSTRAINT_PARSE_RECORD *);
static intBool CheckForFacetConflicts(void *,SLOT_DESC *,CONSTRAINT_PARSE_RECORD *);
static intBool EvaluateSlotDefaultValue(void *,SLOT_DESC *,char *);

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/************************************************************
  NAME         : ParseSlot
  DESCRIPTION  : Parses slot definitions for a
                   defclass statement
  INPUTS       : 1) The logical name of the input source
                 2) The current slot list
                 3) The class precedence list for the class
                    to which this slot is being attached
                    (used to find facets for composite slots)
                 4) A flag indicating if this is a multifield
                    slot or not
                 5) A flag indicating if the type of slot
                    (single or multi) was explicitly
                    specified or not
  RETURNS      : The address of the list of slots,
                   NULL if there was an error
  SIDE EFFECTS : The slot list is allocated
  NOTES        : Assumes "(slot" has already been parsed.
 ************************************************************/
globle TEMP_SLOT_LINK *ParseSlot(
  void *theEnv,
  char *readSource,
  TEMP_SLOT_LINK *slist,
  PACKED_CLASS_LINKS *preclist,
  int multiSlot,
  int fieldSpecified)
  {
   SLOT_DESC *slot;
   CONSTRAINT_PARSE_RECORD parsedConstraint;
   char specbits[2];
   int rtnCode;
   SYMBOL_HN *newOverrideMsg;

   /* ===============================================================
      Bits in specbits are when slot qualifiers are specified so that
      duplicate or conflicting qualifiers can be detected.

      Shared/local                          bit-0
      Single/multiple                       bit-1
      Read-only/Read-write/Initialize-Only  bit-2
      Inherit/No-inherit                    bit-3
      Composite/Exclusive                   bit-4
      Reactive/Nonreactive                  bit-5
      Default                               bit-6
      Default-dynamic                       bit-7
      Visibility                            bit-8
      Override-message                      bit-9
      =============================================================== */
   SavePPBuffer(theEnv," ");
   specbits[0] = specbits[1] = '\0';
   GetToken(theEnv,readSource,&DefclassData(theEnv)->ObjectParseToken);
   if (GetType(DefclassData(theEnv)->ObjectParseToken) != SYMBOL)
     {
      DeleteSlots(theEnv,slist);
      SyntaxErrorMessage(theEnv,"defclass slot");
      return(NULL);
     }
   if ((DefclassData(theEnv)->ObjectParseToken.value == (void *) DefclassData(theEnv)->ISA_SYMBOL) ||
       (DefclassData(theEnv)->ObjectParseToken.value == (void *) DefclassData(theEnv)->NAME_SYMBOL))
     {
      DeleteSlots(theEnv,slist);
      SyntaxErrorMessage(theEnv,"defclass slot");
      return(NULL);
     }
   slot = NewSlot(theEnv,(SYMBOL_HN *) GetValue(DefclassData(theEnv)->ObjectParseToken));
   slist = InsertSlot(theEnv,slist,slot);
   if (slist == NULL)
     return(NULL);
   if (multiSlot)
     slot->multiple = TRUE;
   if (fieldSpecified)
     SetBitMap(specbits,FIELD_BIT);
   GetToken(theEnv,readSource,&DefclassData(theEnv)->ObjectParseToken);
   IncrementIndentDepth(theEnv,3);
   InitializeConstraintParseRecord(&parsedConstraint);
   while (GetType(DefclassData(theEnv)->ObjectParseToken) == LPAREN)
     {
      PPBackup(theEnv);
      PPCRAndIndent(theEnv);
      SavePPBuffer(theEnv,"(");
      GetToken(theEnv,readSource,&DefclassData(theEnv)->ObjectParseToken);
      if (GetType(DefclassData(theEnv)->ObjectParseToken) != SYMBOL)
        {
         SyntaxErrorMessage(theEnv,"defclass slot");
         goto ParseSlotError;
        }
      else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),DEFAULT_FACET) == 0)
        {
         if (ParseDefaultFacet(theEnv,readSource,specbits,slot) == FALSE)
           goto ParseSlotError;
        }
      else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),DYNAMIC_FACET) == 0)
        {
         SetBitMap(specbits,DEFAULT_DYNAMIC_BIT);
         if (ParseDefaultFacet(theEnv,readSource,specbits,slot) == FALSE)
           goto ParseSlotError;
        }
      else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),ACCESS_FACET) == 0)
        {
         rtnCode = ParseSimpleFacet(theEnv,readSource,specbits,ACCESS_FACET,ACCESS_BIT,
                                    SLOT_RDWRT_RLN,SLOT_RDONLY_RLN,SLOT_INIT_RLN,
                                    NULL,NULL);
         if (rtnCode == -1)
           goto ParseSlotError;
         else if (rtnCode == 1)
           slot->noWrite = 1;
         else if (rtnCode == 2)
           slot->initializeOnly = 1;
        }
      else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),STORAGE_FACET) == 0)
        {
         rtnCode = ParseSimpleFacet(theEnv,readSource,specbits,STORAGE_FACET,STORAGE_BIT,
                                    SLOT_LOCAL_RLN,SLOT_SHARE_RLN,NULL,NULL,NULL);
         if (rtnCode == -1)
           goto ParseSlotError;
         slot->shared = rtnCode;
        }
      else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),PROPAGATION_FACET) == 0)
        {
         rtnCode = ParseSimpleFacet(theEnv,readSource,specbits,PROPAGATION_FACET,PROPAGATION_BIT,
                                    SLOT_INH_RLN,SLOT_NO_INH_RLN,NULL,NULL,NULL);
         if (rtnCode == -1)
           goto ParseSlotError;
         slot->noInherit = rtnCode;
        }
      else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),SOURCE_FACET) == 0)
        {
         rtnCode = ParseSimpleFacet(theEnv,readSource,specbits,SOURCE_FACET,SOURCE_BIT,
                                    SLOT_EXCLUSIVE_RLN,SLOT_COMPOSITE_RLN,NULL,NULL,NULL);
         if (rtnCode == -1)
           goto ParseSlotError;
         slot->composite = rtnCode;
        }
#if DEFRULE_CONSTRUCT
      else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),MATCH_FACET) == 0)
        {
         rtnCode = ParseSimpleFacet(theEnv,readSource,specbits,MATCH_FACET,MATCH_BIT,
                                    SLOT_NONREACTIVE_RLN,SLOT_REACTIVE_RLN,NULL,NULL,NULL);
         if (rtnCode == -1)
           goto ParseSlotError;
         slot->reactive = rtnCode;
        }
#endif
      else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),VISIBILITY_FACET) == 0)
        {
         rtnCode = ParseSimpleFacet(theEnv,readSource,specbits,VISIBILITY_FACET,VISIBILITY_BIT,
                                    SLOT_PRIVATE_RLN,SLOT_PUBLIC_RLN,NULL,NULL,NULL);
         if (rtnCode == -1)
           goto ParseSlotError;
         slot->publicVisibility = rtnCode;
        }
      else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),CREATE_ACCESSOR_FACET) == 0)
        {
         rtnCode = ParseSimpleFacet(theEnv,readSource,specbits,CREATE_ACCESSOR_FACET,
                                    CREATE_ACCESSOR_BIT,
                                    SLOT_READ_RLN,SLOT_WRITE_RLN,SLOT_RDWRT_RLN,
                                    SLOT_NONE_RLN,NULL);
         if (rtnCode == -1)
           goto ParseSlotError;
         if ((rtnCode == 0) || (rtnCode == 2))
           slot->createReadAccessor = TRUE;
         if ((rtnCode == 1) || (rtnCode == 2))
           slot->createWriteAccessor = TRUE;
        }
      else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),OVERRIDE_MSG_FACET) == 0)
        {
         rtnCode = ParseSimpleFacet(theEnv,readSource,specbits,OVERRIDE_MSG_FACET,OVERRIDE_MSG_BIT,
                                    NULL,NULL,NULL,SLOT_DEFAULT_RLN,&newOverrideMsg);
         if (rtnCode == -1)
           goto ParseSlotError;
         if (rtnCode == 4)
           {
            DecrementSymbolCount(theEnv,slot->overrideMessage);
            slot->overrideMessage = newOverrideMsg;
            IncrementSymbolCount(slot->overrideMessage);
           }
         slot->overrideMessageSpecified = TRUE;
        }
      else if (StandardConstraint(DOToString(DefclassData(theEnv)->ObjectParseToken)))
        {
         if (ParseStandardConstraint(theEnv,readSource,DOToString(DefclassData(theEnv)->ObjectParseToken),
                slot->constraint,&parsedConstraint,TRUE) == FALSE)
           goto ParseSlotError;
        }
      else
        {
         SyntaxErrorMessage(theEnv,"defclass slot");
         goto ParseSlotError;
        }
      GetToken(theEnv,readSource,&DefclassData(theEnv)->ObjectParseToken);
     }
   if (GetType(DefclassData(theEnv)->ObjectParseToken) != RPAREN)
     {
      SyntaxErrorMessage(theEnv,"defclass slot");
      goto ParseSlotError;
     }
     
   if (DefclassData(theEnv)->ClassDefaultsMode == CONVENIENCE_MODE)
     {
      if (! TestBitMap(specbits,CREATE_ACCESSOR_BIT))
        {
         slot->createReadAccessor = TRUE;
      
         if (! slot->noWrite)
           { slot->createWriteAccessor = TRUE; }   
        }
     }
     
   if (slot->composite)
     BuildCompositeFacets(theEnv,slot,preclist,specbits,&parsedConstraint);
   if (CheckForFacetConflicts(theEnv,slot,&parsedConstraint) == FALSE)
     goto ParseSlotError;
   if (CheckConstraintParseConflicts(theEnv,slot->constraint) == FALSE)
     goto ParseSlotError;
   if (EvaluateSlotDefaultValue(theEnv,slot,specbits) == FALSE)
     goto ParseSlotError;
   if ((slot->dynamicDefault == 0) && (slot->noWrite == 1) &&
       (slot->initializeOnly == 0))
     slot->shared = 1;
   slot->constraint = AddConstraint(theEnv,slot->constraint);
   DecrementIndentDepth(theEnv,3);
   return(slist);

ParseSlotError:
   DecrementIndentDepth(theEnv,3);
   DeleteSlots(theEnv,slist);
   return(NULL);
  }

/***************************************************
  NAME         : DeleteSlots
  DESCRIPTION  : Deallocates a list of slots and
                   their values
  INPUTS       : The address of the slot list
  RETURNS      : Nothing useful
  SIDE EFFECTS : The slot list is destroyed
  NOTES        : None
 ***************************************************/
globle void DeleteSlots(
  void *theEnv,
  TEMP_SLOT_LINK *slots)
  {
   TEMP_SLOT_LINK *stmp;

   while (slots != NULL)
     {
      stmp = slots;
      slots = slots->nxt;
      DeleteSlotName(theEnv,stmp->desc->slotName);
      DecrementSymbolCount(theEnv,stmp->desc->overrideMessage);
      RemoveConstraint(theEnv,stmp->desc->constraint);
      if (stmp->desc->dynamicDefault == 1)
        {
         ExpressionDeinstall(theEnv,(EXPRESSION *) stmp->desc->defaultValue);
         ReturnPackedExpression(theEnv,(EXPRESSION *) stmp->desc->defaultValue);
        }
      else if (stmp->desc->defaultValue != NULL)
        {
         ValueDeinstall(theEnv,(DATA_OBJECT *) stmp->desc->defaultValue);
         rtn_struct(theEnv,dataObject,stmp->desc->defaultValue);
        }
      rtn_struct(theEnv,slotDescriptor,stmp->desc);
      rtn_struct(theEnv,tempSlotLink,stmp);
     }
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/**************************************************************
  NAME         : NewSlot
  DESCRIPTION  : Allocates and initalizes a new slot structure
  INPUTS       : The symbolic name of the new slot
  RETURNS      : The address of the new slot
  SIDE EFFECTS : None
  NOTES        : Also adds symbols of the form get-<name> and
                   put-<name> for slot accessors
 **************************************************************/
static SLOT_DESC *NewSlot(
  void *theEnv,
  SYMBOL_HN *name)
  {
   SLOT_DESC *slot;

   slot = get_struct(theEnv,slotDescriptor);
   slot->dynamicDefault = 1;
   slot->defaultSpecified = 0;
   slot->noDefault = 0;
#if DEFRULE_CONSTRUCT
   slot->reactive = 1;
#endif
   slot->noInherit = 0;
   slot->noWrite = 0;
   slot->initializeOnly = 0;
   slot->shared = 0;
   slot->multiple = 0;
   slot->composite = 0;
   slot->sharedCount = 0;
   slot->publicVisibility = 0;
   slot->createReadAccessor = FALSE;
   slot->createWriteAccessor = FALSE;
   slot->overrideMessageSpecified = 0;
   slot->cls = NULL;
   slot->defaultValue = NULL;
   slot->constraint = GetConstraintRecord(theEnv);
   slot->slotName = AddSlotName(theEnv,name,0,FALSE);
   slot->overrideMessage = slot->slotName->putHandlerName;
   IncrementSymbolCount(slot->overrideMessage);
   return(slot);
  }

/**********************************************************
  NAME         : InsertSlot
  DESCRIPTION  : Inserts a slot into the list of slots
  INPUTS       : 1) The current head of the slot list
                 2) The slot to be inserted
  RETURNS      : The head of the slot list
  SIDE EFFECTS : The slot is inserted if no errors,
                   otherwise the original list and the
                   new slot are destroyed
  NOTES        : None
 **********************************************************/
static TEMP_SLOT_LINK *InsertSlot(
  void *theEnv,
  TEMP_SLOT_LINK *slist,
  SLOT_DESC *slot)
  {
   TEMP_SLOT_LINK *stmp,*sprv,*tmp;

   tmp = get_struct(theEnv,tempSlotLink);
   tmp->desc = slot;
   tmp->nxt = NULL;
   if (slist == NULL)
     slist = tmp;
   else
     {
      stmp = slist;
      sprv = NULL;
      while (stmp != NULL)
        {
         if (stmp->desc->slotName == slot->slotName)
           {
            tmp->nxt = slist;
            DeleteSlots(theEnv,tmp);
            PrintErrorID(theEnv,"CLSLTPSR",1,FALSE);
            EnvPrintRouter(theEnv,WERROR,"Duplicate slots not allowed.\n");
            return(NULL);
           }
         sprv = stmp;
         stmp = stmp->nxt;
        }
      sprv->nxt = tmp;
     }
   return(slist);
  }

/****************************************************************
  NAME         : ParseSimpleFacet
  DESCRIPTION  : Parses the following facets for a slot:
                   access, source, propagation, storage,
                   pattern-match, visibility and override-message
  INPUTS       : 1) The input logical name
                 2) The bitmap indicating which facets have
                    already been parsed
                 3) The name of the facet
                 4) The bit to test/set in arg #2 for this facet
                 5) The facet value string which indicates the
                    facet should be false
                 6) The facet value string which indicates the
                    facet should be TRUE
                 7) An alternate value string for use when the
                    first two don't match (can be NULL)
                 7) An alternate value string for use when the
                    first three don't match (can be NULL)
                    (will be an SF_VARIABLE type)
                 9) A buffer to hold the facet value symbol
                    (can be NULL - only set if args #5 and #6
                     are both NULL)
  RETURNS      : -1 on errors
                  0 if first value string matched
                  1 if second value string matched
                  2 if alternate value string matched
                  3 if variable value string matched
                  4 if facet value buffer was set
  SIDE EFFECTS : Messages printed on errors
                 Bitmap marked indicating facet was parsed
                 Facet value symbol buffer set, if appropriate
  NOTES        : None
 *****************************************************************/
static int ParseSimpleFacet(
  void *theEnv,
  char *readSource,
  char *specbits,
  char *facetName,
  int testBit,
  char *clearRelation,
  char *setRelation,
  char *alternateRelation,
  char *varRelation,
  SYMBOL_HN **facetSymbolicValue)
  {
   int rtnCode;

   if (TestBitMap(specbits,testBit))
     {
      PrintErrorID(theEnv,"CLSLTPSR",2,FALSE);
      EnvPrintRouter(theEnv,WERROR,facetName);
      EnvPrintRouter(theEnv,WERROR," facet already specified.\n");
      return(-1);
     }
   SetBitMap(specbits,testBit);
   SavePPBuffer(theEnv," ");
   GetToken(theEnv,readSource,&DefclassData(theEnv)->ObjectParseToken);

   /* ===============================
      Check for the variable relation
      =============================== */
   if (DefclassData(theEnv)->ObjectParseToken.type == SF_VARIABLE)
     {
      if ((varRelation == NULL) ? FALSE :
          (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),varRelation) == 0))
        rtnCode = 3;
      else
        goto ParseSimpleFacetError;
     }
   else
     {
      if (DefclassData(theEnv)->ObjectParseToken.type != SYMBOL)
        goto ParseSimpleFacetError;

      /* ===================================================
         If the facet value buffer is non-NULL
         simply get the value and do not check any relations
         =================================================== */
      if (facetSymbolicValue == NULL)
        {
         if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),clearRelation) == 0)
           rtnCode = 0;
         else if (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),setRelation) == 0)
           rtnCode = 1;
         else if ((alternateRelation == NULL) ? FALSE :
                  (strcmp(DOToString(DefclassData(theEnv)->ObjectParseToken),alternateRelation) == 0))
           rtnCode = 2;
         else
           goto ParseSimpleFacetError;
        }
      else
        {
         rtnCode = 4;
         *facetSymbolicValue = (SYMBOL_HN *) DefclassData(theEnv)->ObjectParseToken.value;
        }
     }
   GetToken(theEnv,readSource,&DefclassData(theEnv)->ObjectParseToken);
   if (DefclassData(theEnv)->ObjectParseToken.type != RPAREN)
     goto ParseSimpleFacetError;
   return(rtnCode);

ParseSimpleFacetError:
   SyntaxErrorMessage(theEnv,"slot facet");
   return(-1);
  }

/*************************************************************
  NAME         : ParseDefaultFacet
  DESCRIPTION  : Parses the facet for a slot
  INPUTS       : 1) The input logical name
                 2) The bitmap indicating which facets have
                    already been parsed
                 3) The slot descriptor to set
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : Slot  set and parsed facet bitmap set
  NOTES        : Syntax: (default ?NONE|<expression>*)
                         (default-dynamic <expression>*)
 *************************************************************/
static intBool ParseDefaultFacet(
  void *theEnv,
  char *readSource,
  char *specbits,
  SLOT_DESC *slot)
  {
   EXPRESSION *tmp;
   int error,noneSpecified,deriveSpecified;

   if (TestBitMap(specbits,DEFAULT_BIT))
     {
      PrintErrorID(theEnv,"CLSLTPSR",2,FALSE);
      EnvPrintRouter(theEnv,WERROR,"default facet already specified.\n");
      return(FALSE);
     }
   SetBitMap(specbits,DEFAULT_BIT);
   error = FALSE;
   tmp = ParseDefault(theEnv,readSource,1,(int) TestBitMap(specbits,DEFAULT_DYNAMIC_BIT),
                      0,&noneSpecified,&deriveSpecified,&error);
   if (error == TRUE)
     return(FALSE);
   if (noneSpecified || deriveSpecified)
     {
     if (noneSpecified)
       {
        slot->noDefault = 1;
        slot->defaultSpecified = 1;
       }
      else
        ClearBitMap(specbits,DEFAULT_BIT);
     }
   else
     {
      slot->defaultValue = (void *) PackExpression(theEnv,tmp);
      ReturnExpression(theEnv,tmp);
      ExpressionInstall(theEnv,(EXPRESSION *) slot->defaultValue);
      slot->defaultSpecified = 1;
     }
   return(TRUE);
  }

/**************************************************************************
  NAME         : BuildCompositeFacets
  DESCRIPTION  : Composite slots are ones that get their facets
                   from more than one class.  By default, the most
                   specific class in object's precedence list specifies
                   the complete set of facets for a slot.  The composite
                   facet in a slot allows facets that are not overridden
                   by the most specific class to be obtained from other
                   classes.

                 Since all superclasses are predetermined before creating
                   a new class based on them, this routine need only
                   examine the immediately next most specific class for
                   extra facets.  Even if that slot is also composite, the
                   other facets have already been filtered down.  If the
                   slot is no-inherit, the next most specific class must
                   be examined.
  INPUTS       : 1) The slot descriptor
                 2) The class precedence list
                 3) The bitmap marking which facets were specified in
                    the original slot definition
  RETURNS      : Nothing useful
  SIDE EFFECTS : Composite slot is updated to reflect facets from
                   a less specific class
  NOTES        : Assumes slot is composite
 *************************************************************************/
static void BuildCompositeFacets(
  void *theEnv,
  SLOT_DESC *sd,
  PACKED_CLASS_LINKS *preclist,
  char *specbits,
  CONSTRAINT_PARSE_RECORD *parsedConstraint)
  {
   SLOT_DESC *compslot = NULL;
   long i;

   for (i = 1 ; i < preclist->classCount ; i++)
     {
      compslot = FindClassSlot(preclist->classArray[i],sd->slotName->name);
      if ((compslot != NULL) ? (compslot->noInherit == 0) : FALSE)
        break;
     }
   if (compslot != NULL)
     {
      if ((sd->defaultSpecified == 0) && (compslot->defaultSpecified == 1))
        {
         sd->dynamicDefault = compslot->dynamicDefault;
         sd->noDefault = compslot->noDefault;
         sd->defaultSpecified = 1;
         if (compslot->defaultValue != NULL)
           {
            if (sd->dynamicDefault)
              {
               sd->defaultValue = (void *) PackExpression(theEnv,(EXPRESSION *) compslot->defaultValue);
               ExpressionInstall(theEnv,(EXPRESSION *) sd->defaultValue);
              }
            else
              {
               sd->defaultValue = (void *) get_struct(theEnv,dataObject);
               GenCopyMemory(DATA_OBJECT,1,sd->defaultValue,compslot->defaultValue);
               ValueInstall(theEnv,(DATA_OBJECT *) sd->defaultValue);
              }
           }
        }
      if (TestBitMap(specbits,FIELD_BIT) == 0)
        sd->multiple = compslot->multiple;
      if (TestBitMap(specbits,STORAGE_BIT) == 0)
        sd->shared = compslot->shared;
      if (TestBitMap(specbits,ACCESS_BIT) == 0)
        {
         sd->noWrite = compslot->noWrite;
         sd->initializeOnly = compslot->initializeOnly;
        }
#if DEFRULE_CONSTRUCT
      if (TestBitMap(specbits,MATCH_BIT) == 0)
        sd->reactive = compslot->reactive;
#endif
      if (TestBitMap(specbits,VISIBILITY_BIT) == 0)
        sd->publicVisibility = compslot->publicVisibility;
      if (TestBitMap(specbits,CREATE_ACCESSOR_BIT) == 0)
        {
         sd->createReadAccessor = compslot->createReadAccessor;
         sd->createWriteAccessor = compslot->createWriteAccessor;
        }
      if ((TestBitMap(specbits,OVERRIDE_MSG_BIT) == 0) &&
          compslot->overrideMessageSpecified)
        {
         DecrementSymbolCount(theEnv,sd->overrideMessage);
         sd->overrideMessage = compslot->overrideMessage;
         IncrementSymbolCount(sd->overrideMessage);
         sd->overrideMessageSpecified = TRUE;
        }
      OverlayConstraint(theEnv,parsedConstraint,sd->constraint,compslot->constraint);
     }
  }

/***************************************************
  NAME         : CheckForFacetConflicts
  DESCRIPTION  : Determines if all facets specified
                 (and inherited) for a slot are
                 consistent
  INPUTS       : 1) The slot descriptor
                 2) The parse record for the
                    type constraints on the slot
  RETURNS      : TRUE if all OK,
                 FALSE otherwise
  SIDE EFFECTS : Min and Max fields replaced in
                 constraint for single-field slot
  NOTES        : None
 ***************************************************/
static intBool CheckForFacetConflicts(
  void *theEnv,
  SLOT_DESC *sd,
  CONSTRAINT_PARSE_RECORD *parsedConstraint)
  {
   if (sd->multiple == 0)
     {
      if (parsedConstraint->cardinality)
        {
         PrintErrorID(theEnv,"CLSLTPSR",3,TRUE);
         EnvPrintRouter(theEnv,WERROR,"Cardinality facet can only be used with multifield slots\n");
         return(FALSE);
        }
      else
        {
         ReturnExpression(theEnv,sd->constraint->minFields);
         ReturnExpression(theEnv,sd->constraint->maxFields);
         sd->constraint->minFields = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,1LL));
         sd->constraint->maxFields = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,1LL));
        }
     }
   if (sd->noDefault && sd->noWrite)
     {
      PrintErrorID(theEnv,"CLSLTPSR",4,TRUE);
      EnvPrintRouter(theEnv,WERROR,"read-only slots must have a default value\n");
      return(FALSE);
     }
   if (sd->noWrite && (sd->createWriteAccessor || sd->overrideMessageSpecified))
     {
      PrintErrorID(theEnv,"CLSLTPSR",5,TRUE);
      EnvPrintRouter(theEnv,WERROR,"read-only slots cannot have a write accessor\n");
      return(FALSE);
     }
   if (sd->noInherit && sd->publicVisibility)
     {
      PrintErrorID(theEnv,"CLSLTPSR",6,TRUE);
      EnvPrintRouter(theEnv,WERROR,"no-inherit slots cannot also be public\n");
      return(FALSE);
     }
   return(TRUE);
  }

/********************************************************************
  NAME         : EvaluateSlotDefaultValue
  DESCRIPTION  : Checks the default value against the slot
                 constraints and evaluates static default values
  INPUTS       : 1) The slot descriptor
                 2) The bitmap marking which facets were specified in
                    the original slot definition
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : Static default value expressions deleted and
                 replaced with data object evaluation
  NOTES        : On errors, slot is marked as dynamix so that
                 DeleteSlots() will erase the slot expression
 ********************************************************************/
static intBool EvaluateSlotDefaultValue(
  void *theEnv,
  SLOT_DESC *sd,
  char *specbits)
  {
   DATA_OBJECT temp;
   int oldce,olddcc,vCode;

   /* ===================================================================
      Slot default value expression is marked as dynamic until now so
      that DeleteSlots() would erase in the event of an error.  The delay
      was so that the evaluation of a static default value could be
      delayed until all the constraints were parsed.
      =================================================================== */
   if (TestBitMap(specbits,DEFAULT_DYNAMIC_BIT) == 0)
     sd->dynamicDefault = 0;

   if (sd->noDefault)
     return(TRUE);

   if (sd->dynamicDefault == 0)
     {
      if (TestBitMap(specbits,DEFAULT_BIT))
        {
         oldce = ExecutingConstruct(theEnv);
         SetExecutingConstruct(theEnv,TRUE);
         olddcc = EnvSetDynamicConstraintChecking(theEnv,EnvGetStaticConstraintChecking(theEnv));
         vCode = EvaluateAndStoreInDataObject(theEnv,(int) sd->multiple,
                  (EXPRESSION *) sd->defaultValue,&temp,TRUE);
         if (vCode != FALSE)
           vCode = ValidSlotValue(theEnv,&temp,sd,NULL,"slot default value");
         EnvSetDynamicConstraintChecking(theEnv,olddcc);
         SetExecutingConstruct(theEnv,oldce);
         if (vCode)
           {
            ExpressionDeinstall(theEnv,(EXPRESSION *) sd->defaultValue);
            ReturnPackedExpression(theEnv,(EXPRESSION *) sd->defaultValue);
            sd->defaultValue = (void *) get_struct(theEnv,dataObject);
            GenCopyMemory(DATA_OBJECT,1,sd->defaultValue,&temp);
            ValueInstall(theEnv,(DATA_OBJECT *) sd->defaultValue);
           }
         else
           {
            sd->dynamicDefault = 1;
            return(FALSE);
           }
        }
      else if (sd->defaultSpecified == 0)
        {
         sd->defaultValue = (void *) get_struct(theEnv,dataObject);
         DeriveDefaultFromConstraints(theEnv,sd->constraint,
                                      (DATA_OBJECT *) sd->defaultValue,(int) sd->multiple,TRUE);
         ValueInstall(theEnv,(DATA_OBJECT *) sd->defaultValue);
        }
     }
   else if (EnvGetStaticConstraintChecking(theEnv))
     {
      vCode = ConstraintCheckExpressionChain(theEnv,(EXPRESSION *) sd->defaultValue,sd->constraint);
      if (vCode != NO_VIOLATION)
        {
         PrintErrorID(theEnv,"CSTRNCHK",1,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Expression for ");
         PrintSlot(theEnv,WERROR,sd,NULL,"dynamic default value");
         ConstraintViolationErrorMessage(theEnv,NULL,NULL,0,0,NULL,0,
                                         vCode,sd->constraint,FALSE);
         return(FALSE);
        }
     }
   return(TRUE);
  }

#endif

