   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.23  01/31/05            */
   /*                                                     */
   /*          DEFTEMPLATE CONSTRUCTS-TO-C MODULE         */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the constructs-to-c feature for the   */
/*    deftemplate construct.                                 */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Added support for templates maintaining their  */
/*            own list of facts.                             */
/*                                                           */
/*************************************************************/

#define _TMPLTCMP_SOURCE_

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT && CONSTRUCT_COMPILER && (! RUN_TIME)

#define SlotPrefix() ArbitraryPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem,2)

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "conscomp.h"
#include "factcmp.h"
#include "cstrncmp.h"
#include "tmpltdef.h"
#include "envrnmnt.h"

#include "tmpltcmp.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static int                     ConstructToCode(void *,char *,char *,char *,int,FILE *,int,int);
   static void                    SlotToCode(void *,FILE *,struct templateSlot *,int,int,int);
   static void                    DeftemplateModuleToCode(void *,FILE *,struct defmodule *,int,int,int);
   static void                    DeftemplateToCode(void *,FILE *,struct deftemplate *,
                                                 int,int,int,int);
   static void                    CloseDeftemplateFiles(void *,FILE *,FILE *,FILE *,int);
   static void                    InitDeftemplateCode(void *,FILE *,int,int);

/*********************************************************/
/* DeftemplateCompilerSetup: Initializes the deftemplate */
/*   construct for use with the constructs-to-c command. */
/*********************************************************/
globle void DeftemplateCompilerSetup(
  void *theEnv)
  {
   DeftemplateData(theEnv)->DeftemplateCodeItem = AddCodeGeneratorItem(theEnv,"deftemplate",0,NULL,InitDeftemplateCode,ConstructToCode,3);
  }

/*************************************************************/
/* ConstructToCode: Produces deftemplate code for a run-time */
/*   module created using the constructs-to-c function.      */
/*************************************************************/
static int ConstructToCode(
  void *theEnv,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices)
  {
   int fileCount = 1;
   struct defmodule *theModule;
   struct deftemplate *theTemplate;
   struct templateSlot *slotPtr;
   int slotCount = 0, slotArrayCount = 0, slotArrayVersion = 1;
   int moduleCount = 0, moduleArrayCount = 0, moduleArrayVersion = 1;
   int templateArrayCount = 0, templateArrayVersion = 1;
   FILE *slotFile = NULL, *moduleFile = NULL, *templateFile = NULL;

   /*==================================================*/
   /* Include the appropriate deftemplate header file. */
   /*==================================================*/

   fprintf(headerFP,"#include \"tmpltdef.h\"\n");

   /*=============================================================*/
   /* Loop through all the modules, all the deftemplates, and all */
   /* the deftemplate slots writing their C code representation   */
   /* to the file as they are traversed.                          */
   /*=============================================================*/

   theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);

   while (theModule != NULL)
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      moduleFile = OpenFileIfNeeded(theEnv,moduleFile,fileName,pathName,fileNameBuffer,fileID,imageID,&fileCount,
                                    moduleArrayVersion,headerFP,
                                    "struct deftemplateModule",ModulePrefix(DeftemplateData(theEnv)->DeftemplateCodeItem),
                                    FALSE,NULL);

      if (moduleFile == NULL)
        {
         CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,maxIndices);
         return(0);
        }

      DeftemplateModuleToCode(theEnv,moduleFile,theModule,imageID,maxIndices,moduleCount);
      moduleFile = CloseFileIfNeeded(theEnv,moduleFile,&moduleArrayCount,&moduleArrayVersion,
                                     maxIndices,NULL,NULL);

      /*=======================================================*/
      /* Loop through each of the deftemplates in this module. */
      /*=======================================================*/

      theTemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,NULL);

      while (theTemplate != NULL)
        {
         templateFile = OpenFileIfNeeded(theEnv,templateFile,fileName,pathName,fileNameBuffer,fileID,imageID,&fileCount,
                                         templateArrayVersion,headerFP,
                                         "struct deftemplate",ConstructPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem),
                                         FALSE,NULL);
         if (templateFile == NULL)
           {
            CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,maxIndices);
            return(0);
           }

         DeftemplateToCode(theEnv,templateFile,theTemplate,imageID,maxIndices,
                        moduleCount,slotCount);
         templateArrayCount++;
         templateFile = CloseFileIfNeeded(theEnv,templateFile,&templateArrayCount,&templateArrayVersion,
                                          maxIndices,NULL,NULL);

         /*======================================================*/
         /* Loop through each of the slots for this deftemplate. */
         /*======================================================*/

         slotPtr = theTemplate->slotList;
         while (slotPtr != NULL)
           {
            slotFile = OpenFileIfNeeded(theEnv,slotFile,fileName,pathName,fileNameBuffer,fileID,imageID,&fileCount,
                                        slotArrayVersion,headerFP,
                                       "struct templateSlot",SlotPrefix(),FALSE,NULL);
            if (slotFile == NULL)
              {
               CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,maxIndices);
               return(0);
              }

            SlotToCode(theEnv,slotFile,slotPtr,imageID,maxIndices,slotCount);
            slotCount++;
            slotArrayCount++;
            slotFile = CloseFileIfNeeded(theEnv,slotFile,&slotArrayCount,&slotArrayVersion,
                                         maxIndices,NULL,NULL);
            slotPtr = slotPtr->next;
           }

         theTemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,theTemplate);
        }

      theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule);
      moduleCount++;
      moduleArrayCount++;

     }

   CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,maxIndices);

   return(1);
  }

/************************************************************/
/* CloseDeftemplateFiles: Closes all of the C files created */
/*   for deftemplates. Called when an error occurs or when  */
/*   the deftemplates have all been written to the files.   */
/************************************************************/
static void CloseDeftemplateFiles(
  void *theEnv,
  FILE *moduleFile,
  FILE *templateFile,
  FILE *slotFile,
  int maxIndices)
  {
   int count = maxIndices;
   int arrayVersion = 0;

   if (slotFile != NULL)
     {
      count = maxIndices;
      CloseFileIfNeeded(theEnv,slotFile,&count,&arrayVersion,maxIndices,NULL,NULL);
     }

   if (templateFile != NULL)
     {
      count = maxIndices;
      CloseFileIfNeeded(theEnv,templateFile,&count,&arrayVersion,maxIndices,NULL,NULL);
     }

   if (moduleFile != NULL)
     {
      count = maxIndices;
      CloseFileIfNeeded(theEnv,moduleFile,&count,&arrayVersion,maxIndices,NULL,NULL);
     }
  }

/*************************************************************/
/* DeftemplateModuleToCode: Writes the C code representation */
/*   of a single deftemplate module to the specified file.   */
/*************************************************************/
#if WIN_BTC
#pragma argsused
#endif
static void DeftemplateModuleToCode(
  void *theEnv,
  FILE *theFile,
  struct defmodule *theModule,
  int imageID,
  int maxIndices,
  int moduleCount)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(moduleCount)
#endif

   fprintf(theFile,"{");

   ConstructModuleToCode(theEnv,theFile,theModule,imageID,maxIndices,
                         DeftemplateData(theEnv)->DeftemplateModuleIndex,ConstructPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem));

   fprintf(theFile,"}");
  }

/************************************************************/
/* DeftemplateToCode: Writes the C code representation of a */
/*   single deftemplate construct to the specified file.    */
/************************************************************/
static void DeftemplateToCode(
  void *theEnv,
  FILE *theFile,
  struct deftemplate *theTemplate,
  int imageID,
  int maxIndices,
  int moduleCount,
  int slotCount)
  {
   /*====================*/
   /* Deftemplate Header */
   /*====================*/

   fprintf(theFile,"{");

   ConstructHeaderToCode(theEnv,theFile,&theTemplate->header,imageID,maxIndices,
                                  moduleCount,ModulePrefix(DeftemplateData(theEnv)->DeftemplateCodeItem),
                                  ConstructPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem));
   fprintf(theFile,",");

   /*===========*/
   /* Slot List */
   /*===========*/

   if (theTemplate->slotList == NULL)
     { fprintf(theFile,"NULL,"); }
   else
     {
      fprintf(theFile,"&%s%d_%d[%d],",SlotPrefix(),
                 imageID,
                 (slotCount / maxIndices) + 1,
                 slotCount % maxIndices);
     }

   /*==========================================*/
   /* Implied Flag, Watch Flag, In Scope Flag, */
   /* Number of Slots, and Busy Count.         */
   /*==========================================*/

   fprintf(theFile,"%d,0,0,%d,%ld,",theTemplate->implied,theTemplate->numberOfSlots,theTemplate->busyCount);

   /*=================*/
   /* Pattern Network */
   /*=================*/

   if (theTemplate->patternNetwork == NULL)
     { fprintf(theFile,"NULL"); }
   else
     { FactPatternNodeReference(theEnv,theTemplate->patternNetwork,theFile,imageID,maxIndices); }

   /*============================================*/
   /* Print the factList and lastFact references */
   /* and close the structure.                   */
   /*============================================*/
   
   fprintf(theFile,",NULL,NULL}");
  }

/*****************************************************/
/* SlotToCode: Writes the C code representation of a */
/*   single deftemplate slot to the specified file.  */
/*****************************************************/
static void SlotToCode(
  void *theEnv,
  FILE *theFile,
  struct templateSlot *theSlot,
  int imageID,
  int maxIndices,
  int slotCount)
  {
   /*===========*/
   /* Slot Name */
   /*===========*/

   fprintf(theFile,"{");
   PrintSymbolReference(theEnv,theFile,theSlot->slotName);

   /*=============================*/
   /* Multislot and Default Flags */
   /*=============================*/

   fprintf(theFile,",%d,%d,%d,%d,",theSlot->multislot,theSlot->noDefault,
                                   theSlot->defaultPresent,theSlot->defaultDynamic);

   /*=============*/
   /* Constraints */
   /*=============*/

   PrintConstraintReference(theEnv,theFile,theSlot->constraints,imageID,maxIndices);

   /*===============*/
   /* Default Value */
   /*===============*/

   fprintf(theFile,",");
   PrintHashedExpressionReference(theEnv,theFile,theSlot->defaultList,imageID,maxIndices);
   fprintf(theFile,",");

   /*===========*/
   /* Next Slot */
   /*===========*/

   if (theSlot->next == NULL)
     { fprintf(theFile,"NULL}"); }
   else
     {
      fprintf(theFile,"&%s%d_%d[%d]}",SlotPrefix(),imageID,
                               ((slotCount+1) / maxIndices) + 1,
                                (slotCount+1) % maxIndices);
     }
  }

/*****************************************************************/
/* DeftemplateCModuleReference: Writes the C code representation */
/*   of a reference to a deftemplate module data structure.      */
/*****************************************************************/
globle void DeftemplateCModuleReference(
  void *theEnv,
  FILE *theFile,
  int count,
  int imageID,
  int maxIndices)
  {
   fprintf(theFile,"MIHS &%s%d_%d[%d]",ModulePrefix(DeftemplateData(theEnv)->DeftemplateCodeItem),
                      imageID,
                      (count / maxIndices) + 1,
                      (count % maxIndices));
  }

/********************************************************************/
/* DeftemplateCConstructReference: Writes the C code representation */
/*   of a reference to a deftemplate data structure.                */
/********************************************************************/
globle void DeftemplateCConstructReference(
  void *theEnv,
  FILE *theFile,
  void *vTheTemplate,
  int imageID,
  int maxIndices)
  {
   struct deftemplate *theTemplate = (struct deftemplate *) vTheTemplate;

   if (theTemplate == NULL)
     { fprintf(theFile,"NULL"); }
   else
     {
      fprintf(theFile,"&%s%d_%ld[%ld]",ConstructPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem),
                      imageID,
                      (theTemplate->header.bsaveID / maxIndices) + 1,
                      theTemplate->header.bsaveID % maxIndices);
     }

  }
  
/*******************************************/
/* InitDeftemplateCode: Writes out runtime */
/*   initialization code for deftemplates. */
/*******************************************/
#if WIN_BTC
#pragma argsused
#endif
static void InitDeftemplateCode(
  void *theEnv,
  FILE *initFP,
  int imageID,
  int maxIndices)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#pragma unused(imageID)
#pragma unused(maxIndices)
#endif

   fprintf(initFP,"   DeftemplateRunTimeInitialize(theEnv);\n");
  }

#endif /* DEFTEMPLATE_CONSTRUCT && CONSTRUCT_COMPILER && (! RUN_TIME) */

