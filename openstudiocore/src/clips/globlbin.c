   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.21  06/15/03            */
   /*                                                     */
   /*            DEFGLOBAL BSAVE/BLOAD MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the binary save/load feature for the  */
/*    defglobal construct.                                   */
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

#define _GLOBLBIN_SOURCE_

#include "setup.h"

#if DEFGLOBAL_CONSTRUCT && (BLOAD || BLOAD_AND_BSAVE || BLOAD_ONLY) && (! RUN_TIME)

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "memalloc.h"
#include "multifld.h"
#include "globldef.h"
#include "bload.h"
#include "bsave.h"
#include "moduldef.h"
#include "globlbsc.h"
#include "envrnmnt.h"

#include "globlbin.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if BLOAD_AND_BSAVE
   static void                    BsaveFind(void *);
   static void                    BsaveStorage(void *,FILE *);
   static void                    BsaveBinaryItem(void *,FILE *);
#endif
   static void                    BloadStorageDefglobals(void *);
   static void                    BloadBinaryItem(void *);
   static void                    UpdateDefglobalModule(void *,void *,long);
   static void                    UpdateDefglobal(void *,void *,long);
   static void                    ClearBload(void *);
   static void                    DeallocateDefglobalBloadData(void *);

/*********************************************/
/* DefglobalBinarySetup: Installs the binary */
/*   save/load feature for the defglobals.   */
/*********************************************/
globle void DefglobalBinarySetup(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,GLOBLBIN_DATA,sizeof(struct defglobalBinaryData),DeallocateDefglobalBloadData);
#if (BLOAD_AND_BSAVE || BLOAD)
   AddAfterBloadFunction(theEnv,"defglobal",ResetDefglobals,50);
#endif

#if BLOAD_AND_BSAVE
   AddBinaryItem(theEnv,"defglobal",0,BsaveFind,NULL,
                             BsaveStorage,BsaveBinaryItem,
                             BloadStorageDefglobals,BloadBinaryItem,
                             ClearBload);
#endif

#if (BLOAD || BLOAD_ONLY)
   AddBinaryItem(theEnv,"defglobal",0,NULL,NULL,NULL,NULL,
                             BloadStorageDefglobals,BloadBinaryItem,
                             ClearBload);
#endif
  }

/*********************************************************/
/* DeallocateDefglobalBloadData: Deallocates environment */
/*    data for the defglobal bsave functionality.        */
/*********************************************************/
static void DeallocateDefglobalBloadData(
  void *theEnv)
  {
#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME)
   size_t space;
   long i;

   for (i = 0; i < DefglobalBinaryData(theEnv)->NumberOfDefglobals; i++)
     {
      if (DefglobalBinaryData(theEnv)->DefglobalArray[i].current.type == MULTIFIELD)
        { ReturnMultifield(theEnv,(struct multifield *) DefglobalBinaryData(theEnv)->DefglobalArray[i].current.value); }
     }

   space = DefglobalBinaryData(theEnv)->NumberOfDefglobals * sizeof(struct defglobal);
   if (space != 0) 
     { genfree(theEnv,(void *) DefglobalBinaryData(theEnv)->DefglobalArray,space); }

   space =  DefglobalBinaryData(theEnv)->NumberOfDefglobalModules * sizeof(struct defglobalModule);
   if (space != 0) 
     { genfree(theEnv,(void *) DefglobalBinaryData(theEnv)->ModuleArray,space); }
#endif
  }

#if BLOAD_AND_BSAVE

/****************************************************/
/* BsaveFind:  Counts the number of data structures */
/*   which must be saved in the binary image for    */
/*   the defglobals in the current environment.     */
/****************************************************/
static void BsaveFind(
  void *theEnv)
  {
   struct defglobal *defglobalPtr;
   struct defmodule *theModule;

   /*=======================================================*/
   /* If a binary image is already loaded, then temporarily */
   /* save the count values since these will be overwritten */
   /* in the process of saving the binary image.            */
   /*=======================================================*/

   SaveBloadCount(theEnv,DefglobalBinaryData(theEnv)->NumberOfDefglobalModules);
   SaveBloadCount(theEnv,DefglobalBinaryData(theEnv)->NumberOfDefglobals);

   /*============================================*/
   /* Set the count of defglobals and defglobals */
   /* module data structures to zero.            */
   /*============================================*/

   DefglobalBinaryData(theEnv)->NumberOfDefglobals = 0;
   DefglobalBinaryData(theEnv)->NumberOfDefglobalModules = 0;

   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      /*================================================*/
      /* Set the current module to the module being     */
      /* examined and increment the number of defglobal */
      /* modules encountered.                           */
      /*================================================*/

      EnvSetCurrentModule(theEnv,(void *) theModule);
      DefglobalBinaryData(theEnv)->NumberOfDefglobalModules++;

      /*====================================================*/
      /* Loop through each defglobal in the current module. */
      /*====================================================*/

      for (defglobalPtr = (struct defglobal *) EnvGetNextDefglobal(theEnv,NULL);
           defglobalPtr != NULL;
           defglobalPtr = (struct defglobal *) EnvGetNextDefglobal(theEnv,defglobalPtr))
        {
         /*======================================================*/
         /* Initialize the construct header for the binary save. */
         /*======================================================*/

         MarkConstructHeaderNeededItems(&defglobalPtr->header,DefglobalBinaryData(theEnv)->NumberOfDefglobals++);
        }
     }
  }

/*****************************************************/
/* BsaveStorage: Writes out storage requirements for */
/*   all defglobal structures to the binary file     */
/*****************************************************/
static void BsaveStorage(
  void *theEnv,
  FILE *fp)
  {
   size_t space;

   /*===========================================================*/
   /* Only two data structures are saved as part of a defglobal */
   /* binary image: the defglobal data structure and the        */
   /* defglobalModule data structure.                           */
   /*===========================================================*/

   space = sizeof(long) * 2;
   GenWrite(&space,sizeof(size_t),fp);
   GenWrite(&DefglobalBinaryData(theEnv)->NumberOfDefglobals,sizeof(long int),fp);
   GenWrite(&DefglobalBinaryData(theEnv)->NumberOfDefglobalModules,sizeof(long int),fp);
  }

/*********************************************/
/* BsaveBinaryItem: Writes out all defglobal */
/*   structures to the binary file           */
/*********************************************/
static void BsaveBinaryItem(
  void *theEnv,
  FILE *fp)
  {
   size_t space;
   struct defglobal *theDefglobal;
   struct bsaveDefglobal newDefglobal;
   struct defmodule *theModule;
   struct bsaveDefglobalModule tempDefglobalModule;
   struct defglobalModule *theModuleItem;

   /*==========================================================*/
   /* Write out the amount of space taken up by the defglobal  */
   /* and defglobalModule data structures in the binary image. */
   /*==========================================================*/

   space = DefglobalBinaryData(theEnv)->NumberOfDefglobals * sizeof(struct bsaveDefglobal) +
           (DefglobalBinaryData(theEnv)->NumberOfDefglobalModules * sizeof(struct bsaveDefglobalModule));
   GenWrite(&space,sizeof(size_t),fp);

   /*=================================================*/
   /* Write out each defglobal module data structure. */
   /*=================================================*/

   DefglobalBinaryData(theEnv)->NumberOfDefglobals = 0;
   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      theModuleItem = (struct defglobalModule *)
                      GetModuleItem(theEnv,NULL,FindModuleItem(theEnv,"defglobal")->moduleIndex);
      AssignBsaveDefmdlItemHdrVals(&tempDefglobalModule.header,
                                           &theModuleItem->header);
      GenWrite(&tempDefglobalModule,sizeof(struct bsaveDefglobalModule),fp);
     }

   /*===========================*/
   /* Write out each defglobal. */
   /*===========================*/

   DefglobalBinaryData(theEnv)->NumberOfDefglobals = 0;
   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      for (theDefglobal = (struct defglobal *) EnvGetNextDefglobal(theEnv,NULL);
           theDefglobal != NULL;
           theDefglobal = (struct defglobal *) EnvGetNextDefglobal(theEnv,theDefglobal))
        {
         AssignBsaveConstructHeaderVals(&newDefglobal.header,
                                          &theDefglobal->header);
         newDefglobal.initial = HashedExpressionIndex(theEnv,theDefglobal->initial);

         GenWrite(&newDefglobal,sizeof(struct bsaveDefglobal),fp);
        }
     }

   /*=============================================================*/
   /* If a binary image was already loaded when the bsave command */
   /* was issued, then restore the counts indicating the number   */
   /* of defglobals and defglobal modules in the binary image     */
   /* (these were overwritten by the binary save).                */
   /*=============================================================*/

   RestoreBloadCount(theEnv,&DefglobalBinaryData(theEnv)->NumberOfDefglobalModules);
   RestoreBloadCount(theEnv,&DefglobalBinaryData(theEnv)->NumberOfDefglobals);
  }

#endif /* BLOAD_AND_BSAVE */

/***********************************************/
/* BloadStorageDefglobals: Allocates space for */
/*   the defglobals used by this binary image. */
/***********************************************/
static void BloadStorageDefglobals(
  void *theEnv)
  {
   size_t space;

   /*=======================================================*/
   /* Determine the number of defglobal and defglobalModule */
   /* data structures to be read.                           */
   /*=======================================================*/

   GenReadBinary(theEnv,&space,sizeof(size_t));
   GenReadBinary(theEnv,&DefglobalBinaryData(theEnv)->NumberOfDefglobals,sizeof(long int));
   GenReadBinary(theEnv,&DefglobalBinaryData(theEnv)->NumberOfDefglobalModules,sizeof(long int));

   /*===================================*/
   /* Allocate the space needed for the */
   /* defglobalModule data structures.  */
   /*===================================*/

   if (DefglobalBinaryData(theEnv)->NumberOfDefglobalModules == 0)
     {
      DefglobalBinaryData(theEnv)->DefglobalArray = NULL;
      DefglobalBinaryData(theEnv)->ModuleArray = NULL;
     }

   space = DefglobalBinaryData(theEnv)->NumberOfDefglobalModules * sizeof(struct defglobalModule);
   DefglobalBinaryData(theEnv)->ModuleArray = (struct defglobalModule *) genalloc(theEnv,space);

   /*===================================*/
   /* Allocate the space needed for the */
   /* defglobal data structures.        */
   /*===================================*/

   if (DefglobalBinaryData(theEnv)->NumberOfDefglobals == 0)
     {
      DefglobalBinaryData(theEnv)->DefglobalArray = NULL;
      return;
     }

   space = (DefglobalBinaryData(theEnv)->NumberOfDefglobals * sizeof(struct defglobal));
   DefglobalBinaryData(theEnv)->DefglobalArray = (struct defglobal *) genalloc(theEnv,space);
  }

/******************************************************/
/* BloadBinaryItem: Loads and refreshes the defglobal */
/*   constructs used by this binary image.            */
/******************************************************/
static void BloadBinaryItem(
  void *theEnv)
  {
   size_t space;

   /*======================================================*/
   /* Read in the amount of space used by the binary image */
   /* (this is used to skip the construct in the event it  */
   /* is not available in the version being run).          */
   /*======================================================*/

   GenReadBinary(theEnv,&space,sizeof(size_t));

   /*=============================================*/
   /* Read in the defglobalModule data structures */
   /* and refresh the pointers.                   */
   /*=============================================*/

   BloadandRefresh(theEnv,DefglobalBinaryData(theEnv)->NumberOfDefglobalModules,
                   sizeof(struct bsaveDefglobalModule),
                   UpdateDefglobalModule);

   /*=======================================*/
   /* Read in the defglobal data structures */
   /* and refresh the pointers.             */
   /*=======================================*/

   BloadandRefresh(theEnv,DefglobalBinaryData(theEnv)->NumberOfDefglobals,
                   sizeof(struct bsaveDefglobal),
                   UpdateDefglobal);
  }

/************************************************/
/* UpdateDefglobalModule: Bload refresh routine */
/*   for defglobal module data structures.      */
/************************************************/
static void UpdateDefglobalModule(
  void *theEnv,
  void *buf,
  long obji)
  {
   struct bsaveDefglobalModule *bdmPtr;

   bdmPtr = (struct bsaveDefglobalModule *) buf;

   UpdateDefmoduleItemHeader(theEnv,&bdmPtr->header,&DefglobalBinaryData(theEnv)->ModuleArray[obji].header,
                             (int) sizeof(struct defglobal),
                             (void *) DefglobalBinaryData(theEnv)->DefglobalArray);
  }

/******************************************/
/* UpdateDefglobal: Bload refresh routine */
/*   for defglobal data structures.       */
/******************************************/
static void UpdateDefglobal(
  void *theEnv,
  void *buf,
  long obji)
  {
   struct bsaveDefglobal *bdp;

   bdp = (struct bsaveDefglobal *) buf;
   UpdateConstructHeader(theEnv,&bdp->header,&DefglobalBinaryData(theEnv)->DefglobalArray[obji].header,
                         (int) sizeof(struct defglobalModule),(void *) DefglobalBinaryData(theEnv)->ModuleArray,
                         (int) sizeof(struct defglobal),(void *) DefglobalBinaryData(theEnv)->DefglobalArray);

#if DEBUGGING_FUNCTIONS
   DefglobalBinaryData(theEnv)->DefglobalArray[obji].watch = WatchGlobals;
#endif
   DefglobalBinaryData(theEnv)->DefglobalArray[obji].initial = HashedExpressionPointer(bdp->initial);
   DefglobalBinaryData(theEnv)->DefglobalArray[obji].current.type = RVOID;

  }

/***************************************/
/* ClearBload: Defglobal clear routine */
/*   when a binary load is in effect.  */
/***************************************/
static void ClearBload(
  void *theEnv)
  {
   long i;
   size_t space;

   /*=======================================================*/
   /* Decrement in use counters for atomic values contained */
   /* in the construct headers. Also decrement data         */
   /* structures used to store the defglobal's value.       */
   /*=======================================================*/

   for (i = 0; i < DefglobalBinaryData(theEnv)->NumberOfDefglobals; i++)
     {
      UnmarkConstructHeader(theEnv,&DefglobalBinaryData(theEnv)->DefglobalArray[i].header);

      ValueDeinstall(theEnv,&(DefglobalBinaryData(theEnv)->DefglobalArray[i].current));
      if (DefglobalBinaryData(theEnv)->DefglobalArray[i].current.type == MULTIFIELD)
        { ReturnMultifield(theEnv,(struct multifield *) DefglobalBinaryData(theEnv)->DefglobalArray[i].current.value); }
     }

   /*==============================================================*/
   /* Deallocate the space used for the defglobal data structures. */
   /*==============================================================*/

   space = DefglobalBinaryData(theEnv)->NumberOfDefglobals * sizeof(struct defglobal);
   if (space != 0) genfree(theEnv,(void *) DefglobalBinaryData(theEnv)->DefglobalArray,space);
   DefglobalBinaryData(theEnv)->NumberOfDefglobals = 0;
   
   /*=====================================================================*/
   /* Deallocate the space used for the defglobal module data structures. */
   /*=====================================================================*/

   space = DefglobalBinaryData(theEnv)->NumberOfDefglobalModules * sizeof(struct defglobalModule);
   if (space != 0) genfree(theEnv,(void *) DefglobalBinaryData(theEnv)->ModuleArray,space);
   DefglobalBinaryData(theEnv)->NumberOfDefglobalModules = 0;
  }

/********************************************************/
/* BloadDefglobalModuleReference: Returns the defglobal */
/*   module pointer for using with the bload function.  */
/********************************************************/
globle void *BloadDefglobalModuleReference(
  void *theEnv,
  int theIndex)
  {
   return ((void *) &DefglobalBinaryData(theEnv)->ModuleArray[theIndex]);
  }

#endif /* DEFGLOBAL_CONSTRUCT && (BLOAD || BLOAD_AND_BSAVE || BLOAD_ONLY) && (! RUN_TIME) */



