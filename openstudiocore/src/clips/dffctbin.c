   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.21  06/15/03            */
   /*                                                     */
   /*             DEFFACTS BSAVE/BLOAD MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the binary save/load feature for the  */
/*    deffacts construct.                                    */
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

#define _DFFCTBIN_SOURCE_

#include "setup.h"

#if DEFFACTS_CONSTRUCT && (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME)

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "memalloc.h"
#include "dffctdef.h"
#include "moduldef.h"
#include "bload.h"
#include "bsave.h"
#include "envrnmnt.h"

#include "dffctbin.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if BLOAD_AND_BSAVE
   static void                    BsaveFind(void *);
   static void                    BsaveExpressions(void *,FILE *);
   static void                    BsaveStorage(void *,FILE *);
   static void                    BsaveBinaryItem(void *,FILE *);
#endif
   static void                    BloadStorage(void *);
   static void                    BloadBinaryItem(void *);
   static void                    UpdateDeffactsModule(void *,void *,long);
   static void                    UpdateDeffacts(void *,void *,long);
   static void                    ClearBload(void *);
   static void                    DeallocateDeffactsBloadData(void *);

/********************************************/
/* DeffactsBinarySetup: Installs the binary */
/*   save/load feature for deffacts.        */
/********************************************/
globle void DeffactsBinarySetup(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,DFFCTBIN_DATA,sizeof(struct deffactsBinaryData),DeallocateDeffactsBloadData);
#if BLOAD_AND_BSAVE
   AddBinaryItem(theEnv,"deffacts",0,BsaveFind,BsaveExpressions,
                             BsaveStorage,BsaveBinaryItem,
                             BloadStorage,BloadBinaryItem,
                             ClearBload);
#endif

#if (BLOAD || BLOAD_ONLY)
   AddBinaryItem(theEnv,"deffacts",0,NULL,NULL,NULL,NULL,
                             BloadStorage,BloadBinaryItem,
                             ClearBload);
#endif
  }
  
/********************************************************/
/* DeallocateDeffactsBloadData: Deallocates environment */
/*    data for the deffacts bsave functionality.        */
/********************************************************/
static void DeallocateDeffactsBloadData(
  void *theEnv)
  {
   size_t space;

   space = DeffactsBinaryData(theEnv)->NumberOfDeffacts * sizeof(struct deffacts);
   if (space != 0) genfree(theEnv,(void *) DeffactsBinaryData(theEnv)->DeffactsArray,space);
   
   space = DeffactsBinaryData(theEnv)->NumberOfDeffactsModules * sizeof(struct deffactsModule);
   if (space != 0) genfree(theEnv,(void *) DeffactsBinaryData(theEnv)->ModuleArray,space);
  }

#if BLOAD_AND_BSAVE

/*********************************************************/
/* BsaveFind: Counts the number of data structures which */
/*   must be saved in the binary image for the deffacts  */
/*   in the current environment.                         */
/*********************************************************/
static void BsaveFind(
  void *theEnv)
  {
   struct deffacts *theDeffacts;
   struct defmodule *theModule;

   /*=======================================================*/
   /* If a binary image is already loaded, then temporarily */
   /* save the count values since these will be overwritten */
   /* in the process of saving the binary image.            */
   /*=======================================================*/

   SaveBloadCount(theEnv,DeffactsBinaryData(theEnv)->NumberOfDeffactsModules);
   SaveBloadCount(theEnv,DeffactsBinaryData(theEnv)->NumberOfDeffacts);

   /*========================================*/
   /* Set the count of deffacts and deffacts */
   /* module data structures to zero.        */
   /*========================================*/

   DeffactsBinaryData(theEnv)->NumberOfDeffacts = 0;
   DeffactsBinaryData(theEnv)->NumberOfDeffactsModules = 0;

   /*===========================*/
   /* Loop through each module. */
   /*===========================*/

   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      /*===============================================*/
      /* Set the current module to the module being    */
      /* examined and increment the number of deffacts */
      /* modules encountered.                          */
      /*===============================================*/

      EnvSetCurrentModule(theEnv,(void *) theModule);
      DeffactsBinaryData(theEnv)->NumberOfDeffactsModules++;

      /*===================================================*/
      /* Loop through each deffacts in the current module. */
      /*===================================================*/

      for (theDeffacts = (struct deffacts *) EnvGetNextDeffacts(theEnv,NULL);
           theDeffacts != NULL;
           theDeffacts = (struct deffacts *) EnvGetNextDeffacts(theEnv,theDeffacts))
        {
         /*======================================================*/
         /* Initialize the construct header for the binary save. */
         /*======================================================*/

         MarkConstructHeaderNeededItems(&theDeffacts->header,DeffactsBinaryData(theEnv)->NumberOfDeffacts++);

         /*============================================================*/
         /* Count the number of expressions contained in the deffacts' */
         /* assertion list and mark any atomic values contained there  */
         /* as in use.                                                 */
         /*============================================================*/

         ExpressionData(theEnv)->ExpressionCount += ExpressionSize(theDeffacts->assertList);
         MarkNeededItems(theEnv,theDeffacts->assertList);
        }
     }
  }

/************************************************/
/* BsaveExpressions: Saves the expressions used */
/*   by deffacts to the binary save file.       */
/************************************************/
static void BsaveExpressions(
  void *theEnv,
  FILE *fp)
  {
   struct deffacts *theDeffacts;
   struct defmodule *theModule;

   /*===========================*/
   /* Loop through each module. */
   /*===========================*/

   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      /*======================================================*/
      /* Set the current module to the module being examined. */
      /*======================================================*/

      EnvSetCurrentModule(theEnv,(void *) theModule);

      /*==================================================*/
      /* Loop through each deffacts in the current module */
      /* and save the assertion list expression.          */
      /*==================================================*/

      for (theDeffacts = (struct deffacts *) EnvGetNextDeffacts(theEnv,NULL);
           theDeffacts != NULL;
           theDeffacts = (struct deffacts *) EnvGetNextDeffacts(theEnv,theDeffacts))
        { BsaveExpression(theEnv,theDeffacts->assertList,fp); }
     }
  }

/******************************************************/
/* BsaveStorage: Writes out the storage requirements  */
/*    for all deffacts structures to the binary file. */
/******************************************************/
static void BsaveStorage(
  void *theEnv,
  FILE *fp)
  {
   size_t space;

   /*=================================================================*/
   /* Only two data structures are saved as part of a deffacts binary */
   /* image: the deffacts data structure and the deffactsModule data  */
   /* structure. The assertion list expressions are not save with the */
   /* deffacts portion of the binary image.                           */
   /*=================================================================*/

   space = sizeof(long) * 2;
   GenWrite(&space,sizeof(size_t),fp);
   GenWrite(&DeffactsBinaryData(theEnv)->NumberOfDeffacts,sizeof(long int),fp);
   GenWrite(&DeffactsBinaryData(theEnv)->NumberOfDeffactsModules,sizeof(long int),fp);
  }

/********************************************/
/* BsaveBinaryItem: Writes out all deffacts */
/*   structures to the binary file.         */
/********************************************/
static void BsaveBinaryItem(
  void *theEnv,
  FILE *fp)
  {
   size_t space;
   struct deffacts *theDeffacts;
   struct bsaveDeffacts newDeffacts;
   struct defmodule *theModule;
   struct bsaveDeffactsModule tempDeffactsModule;
   struct deffactsModule *theModuleItem;

   /*=========================================================*/
   /* Write out the amount of space taken up by the deffacts  */
   /* and deffactsModule data structures in the binary image. */
   /*=========================================================*/

   space = DeffactsBinaryData(theEnv)->NumberOfDeffacts * sizeof(struct bsaveDeffacts) +
           (DeffactsBinaryData(theEnv)->NumberOfDeffactsModules * sizeof(struct bsaveDeffactsModule));
   GenWrite(&space,sizeof(size_t),fp);

   /*================================================*/
   /* Write out each deffacts module data structure. */
   /*================================================*/

   DeffactsBinaryData(theEnv)->NumberOfDeffacts = 0;
   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      theModuleItem = (struct deffactsModule *) GetModuleItem(theEnv,NULL,DeffactsData(theEnv)->DeffactsModuleIndex);
      AssignBsaveDefmdlItemHdrVals(&tempDeffactsModule.header,&theModuleItem->header);
      GenWrite(&tempDeffactsModule,(unsigned long) sizeof(struct bsaveDeffactsModule),fp);
     }

   /*==========================*/
   /* Write out each deffacts. */
   /*==========================*/

   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      for (theDeffacts = (struct deffacts *) EnvGetNextDeffacts(theEnv,NULL);
           theDeffacts != NULL;
           theDeffacts = (struct deffacts *) EnvGetNextDeffacts(theEnv,theDeffacts))
        {
         AssignBsaveConstructHeaderVals(&newDeffacts.header,&theDeffacts->header);
         if (theDeffacts->assertList != NULL)
           {
            newDeffacts.assertList = ExpressionData(theEnv)->ExpressionCount;
            ExpressionData(theEnv)->ExpressionCount += ExpressionSize(theDeffacts->assertList);
           }
         else
           { newDeffacts.assertList = -1L; }

         GenWrite(&newDeffacts,(unsigned long) sizeof(struct bsaveDeffacts),fp);
        }
     }

   /*=============================================================*/
   /* If a binary image was already loaded when the bsave command */
   /* was issued, then restore the counts indicating the number   */
   /* of deffacts and deffacts modules in the binary image (these */
   /* were overwritten by the binary save).                       */
   /*=============================================================*/

   RestoreBloadCount(theEnv,&DeffactsBinaryData(theEnv)->NumberOfDeffactsModules);
   RestoreBloadCount(theEnv,&DeffactsBinaryData(theEnv)->NumberOfDeffacts);
  }

#endif /* BLOAD_AND_BSAVE */

/****************************************************/
/* BloadStorage: Allocates storage requirements for */
/*   the deffacts used by this binary image.        */
/****************************************************/
static void BloadStorage(
  void *theEnv)
  {
   size_t space;

   /*=====================================================*/
   /* Determine the number of deffacts and deffactsModule */
   /* data structures to be read.                         */
   /*=====================================================*/

   GenReadBinary(theEnv,&space,sizeof(size_t));
   GenReadBinary(theEnv,&DeffactsBinaryData(theEnv)->NumberOfDeffacts,sizeof(long int));
   GenReadBinary(theEnv,&DeffactsBinaryData(theEnv)->NumberOfDeffactsModules,sizeof(long int));

   /*===================================*/
   /* Allocate the space needed for the */
   /* deffactsModule data structures.   */
   /*===================================*/

   if (DeffactsBinaryData(theEnv)->NumberOfDeffactsModules == 0)
     {
      DeffactsBinaryData(theEnv)->DeffactsArray = NULL;
      DeffactsBinaryData(theEnv)->ModuleArray = NULL;
      return;
     }

   space = DeffactsBinaryData(theEnv)->NumberOfDeffactsModules * sizeof(struct deffactsModule);
   DeffactsBinaryData(theEnv)->ModuleArray = (struct deffactsModule *) genalloc(theEnv,space);

   /*===================================*/
   /* Allocate the space needed for the */
   /* deffacts data structures.         */
   /*===================================*/

   if (DeffactsBinaryData(theEnv)->NumberOfDeffacts == 0)
     {
      DeffactsBinaryData(theEnv)->DeffactsArray = NULL;
      return;
     }

   space = (DeffactsBinaryData(theEnv)->NumberOfDeffacts * sizeof(struct deffacts));
   DeffactsBinaryData(theEnv)->DeffactsArray = (struct deffacts *) genalloc(theEnv,space);
  }

/*****************************************************/
/* BloadBinaryItem: Loads and refreshes the deffacts */
/*   constructs used by this binary image.           */
/*****************************************************/
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

   /*============================================*/
   /* Read in the deffactsModule data structures */
   /* and refresh the pointers.                  */
   /*============================================*/

   BloadandRefresh(theEnv,DeffactsBinaryData(theEnv)->NumberOfDeffactsModules,
                   sizeof(struct bsaveDeffactsModule),UpdateDeffactsModule);

   /*======================================*/
   /* Read in the deffacts data structures */
   /* and refresh the pointers.            */
   /*======================================*/

   BloadandRefresh(theEnv,DeffactsBinaryData(theEnv)->NumberOfDeffacts,
                   sizeof(struct bsaveDeffacts),UpdateDeffacts);
  }

/***************************************************/
/* UpdateDeffactsModule: Bload refresh routine for */
/*   deffacts module data structures.              */
/***************************************************/
static void UpdateDeffactsModule(
  void *theEnv,
  void *buf,
  long obji)
  {
   struct bsaveDeffactsModule *bdmPtr;

   bdmPtr = (struct bsaveDeffactsModule *) buf;
   UpdateDefmoduleItemHeader(theEnv,&bdmPtr->header,&DeffactsBinaryData(theEnv)->ModuleArray[obji].header,
                             (int) sizeof(struct deffacts),(void *) DeffactsBinaryData(theEnv)->DeffactsArray);
  }

/*********************************************/
/* UpdateDeffacts: Bload refresh routine for */
/*   deffacts data structures.               */
/*********************************************/
static void UpdateDeffacts(
  void *theEnv,
  void *buf,
  long obji)
  {
   struct bsaveDeffacts *bdp;

   bdp = (struct bsaveDeffacts *) buf;
   UpdateConstructHeader(theEnv,&bdp->header,&DeffactsBinaryData(theEnv)->DeffactsArray[obji].header,
                         (int) sizeof(struct deffactsModule),(void *) DeffactsBinaryData(theEnv)->ModuleArray,
                         (int) sizeof(struct deffacts),(void *) DeffactsBinaryData(theEnv)->DeffactsArray);
   DeffactsBinaryData(theEnv)->DeffactsArray[obji].assertList = ExpressionPointer(bdp->assertList);
  }

/**************************************/
/* ClearBload: Deffacts clear routine */
/*   when a binary load is in effect. */
/**************************************/
static void ClearBload(
  void *theEnv)
  {
   long i;
   size_t space;

   /*=============================================*/
   /* Decrement in use counters for atomic values */
   /* contained in the construct headers.         */
   /*=============================================*/

   for (i = 0; i < DeffactsBinaryData(theEnv)->NumberOfDeffacts; i++)
     { UnmarkConstructHeader(theEnv,&DeffactsBinaryData(theEnv)->DeffactsArray[i].header); }

   /*=============================================================*/
   /* Deallocate the space used for the deffacts data structures. */
   /*=============================================================*/

   space = DeffactsBinaryData(theEnv)->NumberOfDeffacts * sizeof(struct deffacts);
   if (space != 0) genfree(theEnv,(void *) DeffactsBinaryData(theEnv)->DeffactsArray,space);
   DeffactsBinaryData(theEnv)->NumberOfDeffacts = 0;
   
   /*====================================================================*/
   /* Deallocate the space used for the deffacts module data structures. */
   /*====================================================================*/

   space = DeffactsBinaryData(theEnv)->NumberOfDeffactsModules * sizeof(struct deffactsModule);
   if (space != 0) genfree(theEnv,(void *) DeffactsBinaryData(theEnv)->ModuleArray,space);
   DeffactsBinaryData(theEnv)->NumberOfDeffactsModules = 0;
  }

/******************************************************/
/* BloadDeffactsModuleReference: Returns the deffacts */
/*   module pointer for use with the bload function.  */
/******************************************************/
globle void *BloadDeffactsModuleReference(
  void *theEnv,
  int theIndex)
  {
   return ((void *) &DeffactsBinaryData(theEnv)->ModuleArray[theIndex]);
  }

#endif /* DEFFACTS_CONSTRUCT && (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME) */


