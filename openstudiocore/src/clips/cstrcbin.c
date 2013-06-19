   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*          CONSTRUCT BINARY LOAD/SAVE MODULE          */
   /*******************************************************/

/*************************************************************/
/* Purpose: Binary load/save functions for construct         */
/*   headers.                                                */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#include "setup.h"

#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE

#include "bload.h"
#include "envrnmnt.h"

#if BLOAD_AND_BSAVE
#include "bsave.h"
#endif

#include "moduldef.h"

#define _CSTRCBIN_SOURCE_
#include "cstrcbin.h"

#if BLOAD_AND_BSAVE

/***************************************************
  NAME         : MarkConstructHeaderNeededItems
  DESCRIPTION  : Marks symbols and other ephemerals
                 needed by a construct header, and
                 sets the binary-save id for the
                 construct
  INPUTS       : 1) The construct header
                 2) The binary-save id to assign
  RETURNS      : Nothing useful
  SIDE EFFECTS : Id set and items marked
  NOTES        : None
 ***************************************************/
globle void MarkConstructHeaderNeededItems(
  struct constructHeader *theConstruct,
  long theBsaveID)
  {
   theConstruct->name->neededSymbol = TRUE;
   theConstruct->bsaveID = theBsaveID;
  }

/******************************************************
  NAME         : AssignBsaveConstructHeaderVals
  DESCRIPTION  : Assigns value to the construct
                 header for saving in the binary file
  INPUTS       : 1) The binary-save buffer for the
                    construct header values
                 2) The actual construct header
  RETURNS      : Nothing useful
  SIDE EFFECTS : Binary-save buffer for construct
                 header written with appropriate values
  NOTES        : Assumes that module items for this
                 construct were saved in the same
                 order as the defmodules.
                 The defmodule binary-save id is
                 used for the whichModule id of
                 this construct.
 ******************************************************/
globle void AssignBsaveConstructHeaderVals(
  struct bsaveConstructHeader *theBsaveConstruct,
  struct constructHeader *theConstruct)
  {
   theBsaveConstruct->name = (long) theConstruct->name->bucket;
   theBsaveConstruct->whichModule = theConstruct->whichModule->theModule->bsaveID;
   if (theConstruct->next != NULL)
     theBsaveConstruct->next = ((struct constructHeader *) theConstruct->next)->bsaveID;
   else
     theBsaveConstruct->next = -1L;
  }

#endif /* BLOAD_AND_BSAVE */

/***************************************************
  NAME         : UpdateConstructHeader
  DESCRIPTION  : Determines field values for
                 construct header from binary-load
                 buffer
  INPUTS       : 1) The binary-load data for the
                    construct header
                 2) The actual construct header
                 3) The size of a defmodule item for
                    this construct
                 4) The array of all defmodule items
                    for this construct
                 5) The size of this construct
                 6) The array of these constructs
  RETURNS      : Nothing useful
  SIDE EFFECTS : Header values set
  NOTES        : None
 ***************************************************/
LOCALE void UpdateConstructHeader(
  void *theEnv,
  struct bsaveConstructHeader *theBsaveConstruct,
  struct constructHeader *theConstruct,
  int itemModuleSize,
  void *itemModuleArray,
  int itemSize,
  void *itemArray)
  {
   long moduleOffset, itemOffset;

   moduleOffset = itemModuleSize * theBsaveConstruct->whichModule;
   theConstruct->whichModule =
     (struct defmoduleItemHeader *) &((char *) itemModuleArray)[moduleOffset];
   theConstruct->name = SymbolPointer(theBsaveConstruct->name);
   IncrementSymbolCount(theConstruct->name);
   if (theBsaveConstruct->next != -1L)
     {
      itemOffset = itemSize * theBsaveConstruct->next;
      theConstruct->next = (struct constructHeader *) &((char *) itemArray)[itemOffset];
     }
   else
     theConstruct->next = NULL;
   theConstruct->ppForm = NULL;
   theConstruct->bsaveID = 0L;
   theConstruct->usrData = NULL;
  }

/*******************************************************
  NAME         : UnmarkConstructHeader
  DESCRIPTION  : Releases any ephemerals (symbols, etc.)
                 of a construct header for removal
  INPUTS       : The construct header
  RETURNS      : Nothing useful
  SIDE EFFECTS : Busy counts fo ephemerals decremented
  NOTES        : None
 *******************************************************/
globle void UnmarkConstructHeader(
  void *theEnv,
  struct constructHeader *theConstruct)
  {
   DecrementSymbolCount(theEnv,theConstruct->name);
  }

#endif /* BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE */

