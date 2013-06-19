   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.21  06/15/03            */
   /*                                                     */
   /*             DEFMODULE BSAVE/BLOAD MODULE            */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the binary save/load feature for the  */
/*    defmodule construct.                                   */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#define _MODULBIN_SOURCE_

#include "setup.h"

#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME)

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "memalloc.h"
#include "constrct.h"
#include "moduldef.h"
#include "bload.h"
#include "bsave.h"
#include "envrnmnt.h"

#include "modulbin.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if BLOAD_AND_BSAVE
   static void                    BsaveFind(void *);
   static void                    BsaveStorage(void *,FILE *);
   static void                    BsaveBinaryItem(void *,FILE *);
#endif
   static void                    BloadStorage(void *);
   static void                    BloadBinaryItem(void *);
   static void                    UpdateDefmodule(void *,void *,long);
   static void                    UpdatePortItem(void *,void *,long);
   static void                    ClearBload(void *);

/*********************************************/
/* DefmoduleBinarySetup: Installs the binary */
/*   save/load feature for defmodules.       */
/*********************************************/
globle void DefmoduleBinarySetup(
  void *theEnv)
  {
   AddBeforeBloadFunction(theEnv,"defmodule",RemoveAllDefmodules,2000);

#if BLOAD_AND_BSAVE
   AddBinaryItem(theEnv,"defmodule",0,BsaveFind,NULL,
                             BsaveStorage,BsaveBinaryItem,
                             BloadStorage,BloadBinaryItem,
                             ClearBload);
#endif

   AddAbortBloadFunction(theEnv,"defmodule",CreateMainModule,0);

#if (BLOAD || BLOAD_ONLY)
   AddBinaryItem(theEnv,"defmodule",0,NULL,NULL,NULL,NULL,
                             BloadStorage,BloadBinaryItem,
                             ClearBload);
#endif
  }

/**************************************************************/
/* UpdateDefmoduleItemHeader: Updates the values in defmodule */
/*   item headers for the loaded binary image.                */
/**************************************************************/
globle void UpdateDefmoduleItemHeader(
  void *theEnv,
  struct bsaveDefmoduleItemHeader *theBsaveHeader,
  struct defmoduleItemHeader *theHeader,
  int itemSize,
  void *itemArray)
  {
   long firstOffset,lastOffset;

   theHeader->theModule = ModulePointer(theBsaveHeader->theModule);
   if (theBsaveHeader->firstItem == -1L)
     {
      theHeader->firstItem = NULL;
      theHeader->lastItem = NULL;
     }
   else
     {
      firstOffset = itemSize * theBsaveHeader->firstItem;
      lastOffset = itemSize * theBsaveHeader->lastItem;
      theHeader->firstItem =
        (struct constructHeader *) &((char *) itemArray)[firstOffset];
      theHeader->lastItem =
        (struct constructHeader *) &((char *) itemArray)[lastOffset];
     }
  }

#if BLOAD_AND_BSAVE

/*********************************************************/
/* AssignBsaveDefmdlItemHdrVals: Assigns the appropriate */
/*   values to a bsave defmodule item header record.     */
/*********************************************************/
globle void AssignBsaveDefmdlItemHdrVals(
  struct bsaveDefmoduleItemHeader *theBsaveHeader,
  struct defmoduleItemHeader *theHeader)
  {
   theBsaveHeader->theModule = theHeader->theModule->bsaveID;
   if (theHeader->firstItem == NULL)
     {
      theBsaveHeader->firstItem = -1L;
      theBsaveHeader->lastItem = -1L;
     }
   else
     {
      theBsaveHeader->firstItem = theHeader->firstItem->bsaveID;
      theBsaveHeader->lastItem = theHeader->lastItem->bsaveID;
     }
  }

/**********************************************************/
/* BsaveFind: Counts the number of data structures which  */
/*   must be saved in the binary image for the defmodules */
/*   in the current environment.                          */
/**********************************************************/
static void BsaveFind(
  void *theEnv)
  {
   struct defmodule *defmodulePtr;
   struct portItem *theList;

   /*=======================================================*/
   /* If a binary image is already loaded, then temporarily */
   /* save the count values since these will be overwritten */
   /* in the process of saving the binary image.            */
   /*=======================================================*/

   SaveBloadCount(theEnv,DefmoduleData(theEnv)->BNumberOfDefmodules);
   SaveBloadCount(theEnv,DefmoduleData(theEnv)->NumberOfPortItems);

   /*==========================================*/
   /* Set the count of defmodule and defmodule */
   /* port items data structures to zero.      */
   /*==========================================*/

   DefmoduleData(theEnv)->BNumberOfDefmodules = 0;
   DefmoduleData(theEnv)->NumberOfPortItems = 0;

   /*===========================*/
   /* Loop through each module. */
   /*===========================*/

   for (defmodulePtr = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        defmodulePtr != NULL;
        defmodulePtr = (struct defmodule *) EnvGetNextDefmodule(theEnv,defmodulePtr))
     {
      /*==============================================*/
      /* Increment the number of modules encountered. */
      /*==============================================*/

      DefmoduleData(theEnv)->BNumberOfDefmodules++;

      /*===========================*/
      /* Mark the defmodule's name */
      /* as being a needed symbol. */
      /*===========================*/

      defmodulePtr->name->neededSymbol = TRUE;

      /*==============================================*/
      /* Loop through each of the port items in the   */
      /* defmodule's import list incrementing the     */
      /* number of port items encountered and marking */
      /* needed symbols.                              */
      /*==============================================*/

      for (theList = defmodulePtr->importList;
           theList != NULL;
           theList = theList->next)
        {
         DefmoduleData(theEnv)->NumberOfPortItems++;
         if (theList->moduleName != NULL)
           { theList->moduleName->neededSymbol = TRUE; }
         if (theList->constructType != NULL)
           { theList->constructType->neededSymbol = TRUE; }
         if (theList->constructName != NULL)
           { theList->constructName->neededSymbol = TRUE; }
        }

      /*==============================================*/
      /* Loop through each of the port items in the   */
      /* defmodule's export list incrementing the     */
      /* number of port items encountered and marking */
      /* needed symbols.                              */
      /*==============================================*/

      for (theList = defmodulePtr->exportList;
           theList != NULL;
           theList = theList->next)
        {
         DefmoduleData(theEnv)->NumberOfPortItems++;
         if (theList->moduleName != NULL)
           { theList->moduleName->neededSymbol = TRUE; }
         if (theList->constructType != NULL)
           { theList->constructType->neededSymbol = TRUE; }
         if (theList->constructName != NULL)
           { theList->constructName->neededSymbol = TRUE; }
        }
     }
  }

/*********************************************************/
/* BsaveStorage: Writes out the storage requirements for */
/*    all defmodule structures to the binary file.       */
/*********************************************************/
static void BsaveStorage(
  void *theEnv,
  FILE *fp)
  {
   size_t space;

   space = sizeof(long) * 2;
   GenWrite(&space,sizeof(size_t),fp);
   GenWrite(&DefmoduleData(theEnv)->BNumberOfDefmodules,sizeof(long int),fp);
   GenWrite(&DefmoduleData(theEnv)->NumberOfPortItems,sizeof(long int),fp);
  }

/*********************************************/
/* BsaveBinaryItem: Writes out all defmodule */
/*   structures to the binary file.          */
/*********************************************/
static void BsaveBinaryItem(
  void *theEnv,
  FILE *fp)
  {
   size_t space;
   struct defmodule *defmodulePtr;
   struct bsaveDefmodule newDefmodule;
   struct bsavePortItem newPortItem;
   struct portItem *theList;

   /*=========================================================*/
   /* Write out the amount of space taken up by the defmodule */
   /* and port item data structures in the binary image.      */
   /*=========================================================*/

   space = DefmoduleData(theEnv)->BNumberOfDefmodules * sizeof(struct bsaveDefmodule);
   space += DefmoduleData(theEnv)->NumberOfPortItems * sizeof(struct bsavePortItem);
   GenWrite(&space,sizeof(size_t),fp);

   /*==========================================*/
   /* Write out each defmodule data structure. */
   /*==========================================*/

   DefmoduleData(theEnv)->BNumberOfDefmodules = 0;
   DefmoduleData(theEnv)->NumberOfPortItems = 0;
   for (defmodulePtr = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        defmodulePtr != NULL;
        defmodulePtr = (struct defmodule *) EnvGetNextDefmodule(theEnv,defmodulePtr))
     {
      newDefmodule.name = defmodulePtr->name->bucket;

      DefmoduleData(theEnv)->BNumberOfDefmodules++;
      if (defmodulePtr->next != NULL)
        { newDefmodule.next = DefmoduleData(theEnv)->BNumberOfDefmodules; }
      else
        { newDefmodule.next = -1L; }

      if (defmodulePtr->importList == NULL)
        { newDefmodule.importList = -1L; }
      else
        {
         newDefmodule.importList = DefmoduleData(theEnv)->NumberOfPortItems;
         for (theList = defmodulePtr->importList;
              theList != NULL;
              theList = theList->next)
           { DefmoduleData(theEnv)->NumberOfPortItems++; }
        }

      if (defmodulePtr->exportList == NULL)
        { newDefmodule.exportList = -1L; }
      else
        {
         newDefmodule.exportList = DefmoduleData(theEnv)->NumberOfPortItems;
         for (theList = defmodulePtr->exportList;
              theList != NULL;
              theList = theList->next)
           { DefmoduleData(theEnv)->NumberOfPortItems++; }
        }

      newDefmodule.bsaveID = defmodulePtr->bsaveID;
      GenWrite(&newDefmodule,sizeof(struct bsaveDefmodule),fp);
     }

   /*==========================================*/
   /* Write out each port item data structure. */
   /*==========================================*/

   DefmoduleData(theEnv)->NumberOfPortItems = 0;
   defmodulePtr = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
   while (defmodulePtr != NULL)
     {
      for (theList = defmodulePtr->importList;
           theList != NULL;
           theList = theList->next)
        {
         DefmoduleData(theEnv)->NumberOfPortItems++;
         if (theList->moduleName == NULL) newPortItem.moduleName = -1L;
         else newPortItem.moduleName = (long) theList->moduleName->bucket;

         if (theList->constructType == NULL) newPortItem.constructType = -1L;
         else newPortItem.constructType = (long) theList->constructType->bucket;

         if (theList->constructName == NULL) newPortItem.constructName = -1L;
         else newPortItem.constructName = (long) theList->constructName->bucket;

         if (theList->next == NULL) newPortItem.next = -1L;
         else newPortItem.next = DefmoduleData(theEnv)->NumberOfPortItems;

         GenWrite(&newPortItem,sizeof(struct bsavePortItem),fp);
        }

      for (theList = defmodulePtr->exportList;
           theList != NULL;
           theList = theList->next)
        {
         DefmoduleData(theEnv)->NumberOfPortItems++;
         if (theList->moduleName == NULL) newPortItem.moduleName = -1L;
         else newPortItem.moduleName = (long) theList->moduleName->bucket;

         if (theList->constructType == NULL) newPortItem.constructType = -1L;
         else newPortItem.constructType = (long) theList->constructType->bucket;

         if (theList->constructName == NULL) newPortItem.constructName = -1L;
         else newPortItem.constructName = (long) theList->constructName->bucket;

         if (theList->next == NULL) newPortItem.next = -1L;
         else newPortItem.next = DefmoduleData(theEnv)->NumberOfPortItems;

         GenWrite(&newPortItem,sizeof(struct bsavePortItem),fp);
        }

      defmodulePtr = (struct defmodule *) EnvGetNextDefmodule(theEnv,defmodulePtr);
     }

   /*=============================================================*/
   /* If a binary image was already loaded when the bsave command */
   /* was issued, then restore the counts indicating the number   */
   /* of defmodule and port items in the binary image (these were */
   /* overwritten by the binary save).                            */
   /*=============================================================*/

   RestoreBloadCount(theEnv,&DefmoduleData(theEnv)->BNumberOfDefmodules);
   RestoreBloadCount(theEnv,&DefmoduleData(theEnv)->NumberOfPortItems);
  }

#endif /* BLOAD_AND_BSAVE */

/**********************************************************/
/* BloadStorage: Allocates storage requirements for the   */
/*   defmodules and port items used by this binary image. */
/**********************************************************/
static void BloadStorage(
  void *theEnv)
  {
   size_t space;

   /*=======================================*/
   /* Determine the number of defmodule and */
   /* port item data structures to be read. */
   /*=======================================*/

   GenReadBinary(theEnv,&space,sizeof(size_t));
   GenReadBinary(theEnv,&DefmoduleData(theEnv)->BNumberOfDefmodules,sizeof(long int));
   GenReadBinary(theEnv,&DefmoduleData(theEnv)->NumberOfPortItems,sizeof(long int));

   /*================================*/
   /* Allocate the space needed for  */
   /* the defmodule data structures. */
   /*================================*/

   if (DefmoduleData(theEnv)->BNumberOfDefmodules == 0)
     {
      DefmoduleData(theEnv)->DefmoduleArray = NULL;
      return;
     }

   space = (DefmoduleData(theEnv)->BNumberOfDefmodules * sizeof(struct defmodule));
   DefmoduleData(theEnv)->DefmoduleArray = (struct defmodule *) genalloc(theEnv,space);

   /*================================*/
   /* Allocate the space needed for  */
   /* the port item data structures. */
   /*================================*/

   if (DefmoduleData(theEnv)->NumberOfPortItems == 0)
     {
      DefmoduleData(theEnv)->PortItemArray = NULL;
      return;
     }

   space = (DefmoduleData(theEnv)->NumberOfPortItems * sizeof(struct portItem));
   DefmoduleData(theEnv)->PortItemArray = (struct portItem *) genalloc(theEnv,space);
  }

/********************************************/
/* BloadBinaryItem: Loads and refreshes the */
/*   defmodules used by this binary image.  */
/********************************************/
static void BloadBinaryItem(
  void *theEnv)
  {
   size_t space;

   GenReadBinary(theEnv,&space,sizeof(size_t));
   if (DefmoduleData(theEnv)->BNumberOfDefmodules == 0) return;

   BloadandRefresh(theEnv,DefmoduleData(theEnv)->BNumberOfDefmodules,sizeof(struct bsaveDefmodule),UpdateDefmodule);
   BloadandRefresh(theEnv,DefmoduleData(theEnv)->NumberOfPortItems,sizeof(struct bsavePortItem),UpdatePortItem);

   SetListOfDefmodules(theEnv,(void *) DefmoduleData(theEnv)->DefmoduleArray);
   EnvSetCurrentModule(theEnv,(void *) EnvGetNextDefmodule(theEnv,NULL));
  }

/******************************************/
/* UpdateDefmodule: Bload refresh routine */
/*   for defmodule data structure.        */
/******************************************/
static void UpdateDefmodule(
  void *theEnv,
  void *buf,
  long obji)
  {
   struct bsaveDefmodule *bdp;
   struct moduleItem *theItem;
   int i;

   bdp = (struct bsaveDefmodule *) buf;
   DefmoduleData(theEnv)->DefmoduleArray[obji].name = SymbolPointer(bdp->name);
   IncrementSymbolCount(DefmoduleData(theEnv)->DefmoduleArray[obji].name);
   if (bdp->next != -1L)
     { DefmoduleData(theEnv)->DefmoduleArray[obji].next = (struct defmodule *) &DefmoduleData(theEnv)->DefmoduleArray[bdp->next]; }
   else
     { DefmoduleData(theEnv)->DefmoduleArray[obji].next = NULL; }

   if (GetNumberOfModuleItems(theEnv) == 0)
     { DefmoduleData(theEnv)->DefmoduleArray[obji].itemsArray = NULL; }
   else
     {
      DefmoduleData(theEnv)->DefmoduleArray[obji].itemsArray = 
         (struct defmoduleItemHeader **) gm2(theEnv,sizeof(void *) * GetNumberOfModuleItems(theEnv));
     }

   for (i = 0, theItem = GetListOfModuleItems(theEnv);
        (i < GetNumberOfModuleItems(theEnv)) && (theItem != NULL) ;
        i++, theItem = theItem->next)
     {
      if (theItem->bloadModuleReference == NULL)
        { DefmoduleData(theEnv)->DefmoduleArray[obji].itemsArray[i] = NULL; }
      else
        {
         DefmoduleData(theEnv)->DefmoduleArray[obji].itemsArray[i] =
             (struct defmoduleItemHeader *)
             (*theItem->bloadModuleReference)(theEnv,obji);
        }
     }

   DefmoduleData(theEnv)->DefmoduleArray[obji].ppForm = NULL;

   if (bdp->importList != -1L)
     { DefmoduleData(theEnv)->DefmoduleArray[obji].importList = (struct portItem *) &DefmoduleData(theEnv)->PortItemArray[bdp->importList]; }
   else
     { DefmoduleData(theEnv)->DefmoduleArray[obji].importList = NULL; }

   if (bdp->exportList != -1L)
     { DefmoduleData(theEnv)->DefmoduleArray[obji].exportList = (struct portItem *) &DefmoduleData(theEnv)->PortItemArray[bdp->exportList]; }
   else
     { DefmoduleData(theEnv)->DefmoduleArray[obji].exportList = NULL; }
   DefmoduleData(theEnv)->DefmoduleArray[obji].bsaveID = bdp->bsaveID;
  }

/*****************************************/
/* UpdatePortItem: Bload refresh routine */
/*   for port item data structure.       */
/*****************************************/
static void UpdatePortItem(
  void *theEnv,
  void *buf,
  long obji)
  {
   struct bsavePortItem *bdp;

   bdp = (struct bsavePortItem *) buf;

   if (bdp->moduleName != -1L)
     {
      DefmoduleData(theEnv)->PortItemArray[obji].moduleName = SymbolPointer(bdp->moduleName);
      IncrementSymbolCount(DefmoduleData(theEnv)->PortItemArray[obji].moduleName);
     }
   else
     { DefmoduleData(theEnv)->PortItemArray[obji].moduleName = NULL; }

   if (bdp->constructType != -1L)
     {
      DefmoduleData(theEnv)->PortItemArray[obji].constructType = SymbolPointer(bdp->constructType);
      IncrementSymbolCount(DefmoduleData(theEnv)->PortItemArray[obji].constructType);
     }
   else
     { DefmoduleData(theEnv)->PortItemArray[obji].constructType = NULL; }

   if (bdp->constructName != -1L)
     {
      DefmoduleData(theEnv)->PortItemArray[obji].constructName = SymbolPointer(bdp->constructName);
      IncrementSymbolCount(DefmoduleData(theEnv)->PortItemArray[obji].constructName);
     }
   else
     { DefmoduleData(theEnv)->PortItemArray[obji].constructName = NULL; }

   if (bdp->next != -1L)
     { DefmoduleData(theEnv)->PortItemArray[obji].next = (struct portItem *) &DefmoduleData(theEnv)->PortItemArray[bdp->next]; }
   else
     { DefmoduleData(theEnv)->PortItemArray[obji].next = NULL; }
  }

/***************************************/
/* ClearBload: Defmodule clear routine */
/*   when a binary load is in effect.  */
/***************************************/
static void ClearBload(
  void *theEnv)
  {
   long i;
   size_t space;
   struct portItem *theList;

   /*===========================*/
   /* Decrement in use counters */
   /* used by the binary image. */
   /*===========================*/

   for (i = 0; i < DefmoduleData(theEnv)->BNumberOfDefmodules; i++)
     {
      DecrementSymbolCount(theEnv,DefmoduleData(theEnv)->DefmoduleArray[i].name);
      for (theList = DefmoduleData(theEnv)->DefmoduleArray[i].importList;
           theList != NULL;
           theList = theList->next)
        {
         if (theList->moduleName != NULL) DecrementSymbolCount(theEnv,theList->moduleName);
         if (theList->constructType != NULL) DecrementSymbolCount(theEnv,theList->constructType);
         if (theList->constructName != NULL) DecrementSymbolCount(theEnv,theList->constructName);
        }

      for (theList = DefmoduleData(theEnv)->DefmoduleArray[i].exportList;
           theList != NULL;
           theList = theList->next)
        {
         if (theList->moduleName != NULL) DecrementSymbolCount(theEnv,theList->moduleName);
         if (theList->constructType != NULL) DecrementSymbolCount(theEnv,theList->constructType);
         if (theList->constructName != NULL) DecrementSymbolCount(theEnv,theList->constructName);
        }

      rm(theEnv,DefmoduleData(theEnv)->DefmoduleArray[i].itemsArray,sizeof(void *) * GetNumberOfModuleItems(theEnv));
     }

   /*================================*/
   /* Deallocate the space used for  */
   /* the defmodule data structures. */
   /*================================*/

   space = DefmoduleData(theEnv)->BNumberOfDefmodules * sizeof(struct defmodule);
   if (space != 0) genfree(theEnv,(void *) DefmoduleData(theEnv)->DefmoduleArray,space);
   DefmoduleData(theEnv)->BNumberOfDefmodules = 0;
   
   /*================================*/
   /* Deallocate the space used for  */
   /* the port item data structures. */
   /*================================*/

   space = DefmoduleData(theEnv)->NumberOfPortItems * sizeof(struct portItem);
   if (space != 0) genfree(theEnv,(void *) DefmoduleData(theEnv)->PortItemArray,space);
   DefmoduleData(theEnv)->NumberOfPortItems = 0;
   
   /*===========================*/
   /* Reset module information. */
   /*===========================*/

   SetListOfDefmodules(theEnv,NULL);
   CreateMainModule(theEnv);
   DefmoduleData(theEnv)->MainModuleRedefinable = TRUE;
  }

#endif /*  (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME) */


