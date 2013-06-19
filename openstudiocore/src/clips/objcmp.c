   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Object System Construct Compiler Code            */
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
/*            Added environment parameter to GenClose.       */
/*                                                           */
/*************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if OBJECT_SYSTEM && CONSTRUCT_COMPILER && (! RUN_TIME)

#include "conscomp.h"
#include "classcom.h"
#include "classfun.h"
#include "classini.h"
#include "cstrncmp.h"
#include "envrnmnt.h"
#include "objrtfnx.h"
#include "sysdep.h"

#define _OBJCMP_SOURCE_
#include "objcmp.h"

/* =========================================
   *****************************************
                   CONSTANTS
   =========================================
   ***************************************** */
#define MODULEI    0
#define CLASSI     1
#define LINKI      2
#define SLOTI      3
#define TSLOTI     4
#define OSLOTI     5
#define HANDLERI   6
#define OHANDLERI  7

#define SAVE_ITEMS 8

/* =========================================
   *****************************************
               MACROS AND TYPES
   =========================================
   ***************************************** */
#define ClassPrefix()          ConstructPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem)
#define ClassLinkPrefix()      ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,2)
#define SlotPrefix()           ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,3)
#define TemplateSlotPrefix()   ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,4)
#define OrderedSlotPrefix()    ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,5)
#define HandlerPrefix()        ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,6)
#define OrderedHandlerPrefix() ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,7)
#define SlotNamePrefix()       ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,8)
#define SlotNameHashPrefix()   ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,9)
#define ClassHashPrefix()      ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,10)
#define ClassIDPrefix()        ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,11)
#define MaxClassIDPrefix()     ArbitraryPrefix(ObjectCompilerData(theEnv)->ObjectCodeItem,12)

typedef struct
  {
   long classCount;
   unsigned short currentPartition;
   unsigned short slotCount;
   int maxIndices;
  } MARK_INFO;

typedef union
  {
   struct
     {
      unsigned thePartition : 16;
      unsigned theOffset    : 16;
     } theLocation;
   long theLong;
  } PACKED_LOCATION_INFO;

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static void ReadyObjectsForCode(void *);
static void MarkDefclassAndSlots(void *,struct constructHeader *,void *);
static void PrintSlotNameReference(void *,FILE *,SLOT_NAME *,int,int);
static void InitObjectsCode(void *,FILE *,int,int);
static int ObjectsToCode(void *,char *,char *,char *,int,FILE *,int,int);
static int ClassIDMapToCode(void *,char *,char *,char *,int,FILE *,int,int,int *);
static int ClassHashTableToCode(void *,char *,char *,char *,int,FILE *,int,int,int *);
static int SlotNameHashTableToCode(void *,char *,char *,char *,int,FILE *,int,int,int *);
static int SlotNameEntriesToCode(void *,char *,char *,char *,int,FILE *,int,int,int *);
static void CloseObjectFiles(void *,FILE *[SAVE_ITEMS],int [SAVE_ITEMS],
                             struct CodeGeneratorFile [SAVE_ITEMS],int);
static void DefclassModuleToCode(void *,FILE *,struct defmodule *,int,int);
static void SingleDefclassToCode(void *,FILE *,int,int,DEFCLASS *,int,
                                 int,int,int,int,int,int,
                                 int,int,int,int,int,int);
static intBool InheritanceLinksToCode(void *,FILE **,char *,char *,char *,int,int,FILE *,
                                      int *,int,DEFCLASS *,int *,
                                      int *,int *,struct CodeGeneratorFile *);
static intBool SlotsToCode(void *,FILE **,char *,char *,char *,int,int,FILE *,
                           int *,int,DEFCLASS *,int *,
                           int *,int *,struct CodeGeneratorFile *);
static intBool TemplateSlotsToCode(void *,FILE **,char *,char *,char *,int,int,FILE *,
                                   int *,int,DEFCLASS *,int *,
                                   int *,int *,struct CodeGeneratorFile *);
static intBool OrderedSlotsToCode(void *,FILE **,char *,char *,char *,int,int,FILE *,
                                  int *,int,DEFCLASS *,int *,
                                  int *,int *,struct CodeGeneratorFile *);
static intBool HandlersToCode(void *,FILE **,char *,char *,char *,int,int,FILE *,
                              int *,int,DEFCLASS *,int *,
                              int *,int *,struct CodeGeneratorFile *);
static intBool OrderedHandlersToCode(void *,FILE **,char *,char *,char *,int,int,FILE *,
                                     int *,int,DEFCLASS *,int *,
                                     int *,int *,struct CodeGeneratorFile *);

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***************************************************
  NAME         : SetupObjectsCompiler
  DESCRIPTION  : Initializes the construct compiler
                   item for defclasses & handlers
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Code generator item initialized
  NOTES        : None
 ***************************************************/
globle void SetupObjectsCompiler(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,OBJECT_COMPILER_DATA,sizeof(struct objectCompilerData),NULL);

   ObjectCompilerData(theEnv)->ObjectCodeItem = AddCodeGeneratorItem(theEnv,"objects",0,ReadyObjectsForCode,
                                         InitObjectsCode,ObjectsToCode,13);
  }


/*********************************************************
  NAME         : PrintClassReference
  DESCRIPTION  : Writes out a reference to the class array
  INPUTS       : 1) Output file pointer
                 2) Class address
                 3) Construct set image id
                 4) The maximum number of indices allowed
                    in an array
  RETURNS      : Nothing useful
  SIDE EFFECTS : Writes out class array reference to file
  NOTES        : None
 *********************************************************/
globle void PrintClassReference(
  void *theEnv,
  FILE *fp,
  DEFCLASS *cls,
  int imageID,
  int maxIndices)
  {
   if (cls == NULL)
     fprintf(fp,"NULL");
   else
     fprintf(fp,"&%s%d_%d[%d]",
                 ClassPrefix(),
                 imageID,
                 (int) ((cls->header.bsaveID / maxIndices) + 1),
                 (int) (cls->header.bsaveID % maxIndices));
  }

/****************************************************
  NAME         : DefclassCModuleReference
  DESCRIPTION  : Prints out a reference to a
                 defclass module
  INPUTS       : 1) The output file
                 2) The id of the module item
                 3) The id of the image
                 4) The maximum number of elements
                    allowed in an array
  RETURNS      : Nothing useful
  SIDE EFFECTS : Defclass module reference printed
  NOTES        : None
 ****************************************************/
globle void DefclassCModuleReference(
  void *theEnv,
  FILE *theFile,
  int count,
  int imageID,
  int maxIndices)
  {
   fprintf(theFile,"MIHS &%s%d_%d[%d]",
                   ModulePrefix(ObjectCompilerData(theEnv)->ObjectCodeItem),
                   imageID,
                   (count / maxIndices) + 1,
                   (count % maxIndices));
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/*******************************************************
  NAME         : ReadyObjectsForCode
  DESCRIPTION  : Sets index of classes and slot name
                   entries for use in compiled
                   expressions
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : BsaveIndices set
  NOTES        : None
 *******************************************************/
static void ReadyObjectsForCode(
  void *theEnv)
  {
   MARK_INFO markInfo;
   register long i;
   register int j;
   SLOT_NAME *snp;

   markInfo.classCount = 0L;
   markInfo.currentPartition = 1;
   markInfo.slotCount = 0;

   /* =====================================
      Gets the value of MaxIndices directly
      from the global in CONSCOMP.C
      ===================================== */
   markInfo.maxIndices = ConstructCompilerData(theEnv)->MaxIndices;
   DoForAllConstructs(theEnv,MarkDefclassAndSlots,DefclassData(theEnv)->DefclassModuleIndex,
                      FALSE,(void *) &markInfo);
   i = 0L;
   for (j = 0 ; j < SLOT_NAME_TABLE_HASH_SIZE ; j++)
     for (snp = DefclassData(theEnv)->SlotNameTable[j] ; snp != NULL ; snp = snp->nxt)
       snp->bsaveIndex = i++;
  }

/************************************************************
  NAME         : MarkDefclassAndSlots
  DESCRIPTION  : Sets the bsave indices of the classes
                 for use in printing references to
                 them later.

                 Also, the partitions and offsets are
                 predetermined for every slot and
                 packed into a single long (the slot
                 bsave index) for use in printing
                 references to them later
  INPUTS       : 1) The defclass
                 2) A buffer containing the info:
                    a) Total number of classes counted so far
                    b) The current partition # for slots
                    c) The current offset in that partition
                    d) The max # of elements in any array
  RETURNS      : Nothing useful
  SIDE EFFECTS : Bsave indices of classes and slots set
  NOTES        : The template slots are written at the
                 same time as the real slots - thus the
                 references must be predetermined
 ************************************************************/
#if WIN_BTC
#pragma argsused
#endif
static void MarkDefclassAndSlots(
  void *theEnv,
  struct constructHeader *vTheDefclass,
  void *vTheBuffer)
  {
   DEFCLASS *theDefclass = (DEFCLASS *) vTheDefclass;
   MARK_INFO *markInfo = (MARK_INFO *) vTheBuffer;
   long i;
   PACKED_LOCATION_INFO theLocationInfo;
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   theDefclass->header.bsaveID = markInfo->classCount++;
   for (i = 0 ; i < theDefclass->slotCount ; i++)
     {
      theLocationInfo.theLocation.thePartition = markInfo->currentPartition;
      theLocationInfo.theLocation.theOffset = markInfo->slotCount;
      theDefclass->slots[i].bsaveIndex = theLocationInfo.theLong;
      markInfo->slotCount++;
      if (markInfo->slotCount >= markInfo->maxIndices)
        {
         markInfo->currentPartition++;
         markInfo->slotCount = 0;
        }
     }
  }

/*************************************************************
  NAME         : PrintSlotNameReference
  DESCRIPTION  : Writes out a reference to the slot name array
  INPUTS       : 1) Output file pointer
                 2) Slot name address
                 3) Construct set image id
                 4) The maximum number of indices allowed
                    in an array
  RETURNS      : Nothing useful
  SIDE EFFECTS : Writes out slot name array reference to file
  NOTES        : None
 *************************************************************/
static void PrintSlotNameReference(
  void *theEnv,
  FILE *fp,
  SLOT_NAME *snp,
  int imageID,
  int maxIndices)
  {
   if (snp == NULL)
     fprintf(fp,"NULL");
   else
     fprintf(fp,"&%s%d_%d[%d]",
                 SlotNamePrefix(),
                 imageID,
                 (int) ((snp->bsaveIndex / maxIndices) + 1),
                 (int) (snp->bsaveIndex % maxIndices));
  }

/*******************************************************
  NAME         : InitObjectsCode
  DESCRIPTION  : Writes out initialization code for
                   generic functions
  INPUTS       : 1) The initialization code file pointer
                 2) The construct set image id
                 3) The max number of indices allowed in
                    an array for this construct set
  RETURNS      : Nothing useful
  SIDE EFFECTS : Writes out initialization code
  NOTES        : None
 *******************************************************/
#if WIN_BTC
#pragma argsused
#endif
static void InitObjectsCode(
  void *theEnv,
  FILE *initFP,
  int imageID,
  int maxIndices)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(maxIndices)
#endif

   fprintf(initFP,"   ObjectsRunTimeInitialize(theEnv,%s%d_1,%s%d_1,%s%d_1,%s%d);\n",
                      ClassHashPrefix(),imageID,SlotNameHashPrefix(),imageID,
                      ClassIDPrefix(),imageID,MaxClassIDPrefix(),imageID);
  }

/*************************************************************
  NAME         : ObjectsToCode
  DESCRIPTION  : Writes out static array code for
                   classes, message-handlers, and associated
                   structures
  INPUTS       : 1) The base name of the construct set
                 2) The base id for this construct
                 3) The file pointer for the header file
                 4) The base id for the construct set
                 5) The max number of indices allowed
                    in an array
  RETURNS      : -1 if no classes, 0 on errors,
                  1 if object system structures written
  SIDE EFFECTS : Code written to files
  NOTES        : None
 *************************************************************/
static int ObjectsToCode(
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
   DEFCLASS *theDefclass;
   register int i;
   int moduleCount = 0;
   int itemArrayCounts[SAVE_ITEMS];
   int itemArrayVersions[SAVE_ITEMS];
   FILE *itemFiles[SAVE_ITEMS];
   int itemReopenFlags[SAVE_ITEMS];
   struct CodeGeneratorFile itemCodeFiles[SAVE_ITEMS];

   for (i = 0 ; i < SAVE_ITEMS ; i++)
     {
      itemArrayCounts[i] = 0;
      itemArrayVersions[i] = 1;
      itemFiles[i] = NULL;
      itemReopenFlags[i] = FALSE;
      itemCodeFiles[i].filePrefix = NULL;
      itemCodeFiles[i].pathName = pathName;
      itemCodeFiles[i].fileNameBuffer = fileNameBuffer;
     }
   fprintf(headerFP,"#include \"classcom.h\"\n");
   fprintf(headerFP,"#include \"classini.h\"\n");
   if (ClassIDMapToCode(theEnv,fileName,pathName,fileNameBuffer,fileID,headerFP,imageID,maxIndices,&fileCount)
       == FALSE)
     return(0);
   if (ClassHashTableToCode(theEnv,fileName,pathName,fileNameBuffer,fileID,headerFP,imageID,maxIndices,&fileCount)
       == FALSE)
     return(0);
   if (SlotNameHashTableToCode(theEnv,fileName,pathName,fileNameBuffer,fileID,headerFP,imageID,maxIndices,&fileCount)
       == FALSE)
     return(0);
   if (SlotNameEntriesToCode(theEnv,fileName,pathName,fileNameBuffer,fileID,headerFP,imageID,maxIndices,&fileCount)
       == FALSE)
     return(0);

   /* =============================================================
      Loop through all the modules and all the defclasses writing
      their C code representation to the file as they are traversed
      ============================================================= */
   theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);

   while (theModule != NULL)
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      itemFiles[MODULEI] =
            OpenFileIfNeeded(theEnv,itemFiles[MODULEI],fileName,pathName,fileNameBuffer,fileID,imageID,&fileCount,
                             itemArrayVersions[MODULEI],headerFP,
                             "DEFCLASS_MODULE",ModulePrefix(ObjectCompilerData(theEnv)->ObjectCodeItem),
                             itemReopenFlags[MODULEI],&itemCodeFiles[MODULEI]);
      if (itemFiles[MODULEI] == NULL)
        goto ObjectCodeError;

      DefclassModuleToCode(theEnv,itemFiles[MODULEI],theModule,imageID,maxIndices);
      itemFiles[MODULEI] =
          CloseFileIfNeeded(theEnv,itemFiles[MODULEI],&itemArrayCounts[MODULEI],
                            &itemArrayVersions[MODULEI],maxIndices,
                            &itemReopenFlags[MODULEI],&itemCodeFiles[MODULEI]);

      for (theDefclass = (DEFCLASS *) EnvGetNextDefclass(theEnv,NULL) ;
           theDefclass != NULL ;
           theDefclass = (DEFCLASS *) EnvGetNextDefclass(theEnv,(void *) theDefclass))
        {
         itemFiles[CLASSI] =
            OpenFileIfNeeded(theEnv,itemFiles[CLASSI],fileName,pathName,fileNameBuffer,fileID,imageID,&fileCount,
                             itemArrayVersions[CLASSI],headerFP,
                             "DEFCLASS",ClassPrefix(),
                             itemReopenFlags[CLASSI],&itemCodeFiles[CLASSI]);
         if (itemFiles[CLASSI] == NULL)
           goto ObjectCodeError;
         SingleDefclassToCode(theEnv,itemFiles[CLASSI],imageID,maxIndices,
                              theDefclass,moduleCount,
                              itemArrayVersions[LINKI],itemArrayCounts[LINKI],
                              itemArrayVersions[SLOTI],itemArrayCounts[SLOTI],
                              itemArrayVersions[TSLOTI],itemArrayCounts[TSLOTI],
                              itemArrayVersions[OSLOTI],itemArrayCounts[OSLOTI],
                              itemArrayVersions[HANDLERI],itemArrayCounts[HANDLERI],
                              itemArrayVersions[OHANDLERI],itemArrayCounts[OHANDLERI]);
         itemArrayCounts[CLASSI]++;
         itemFiles[CLASSI] =
           CloseFileIfNeeded(theEnv,itemFiles[CLASSI],&itemArrayCounts[CLASSI],
                             &itemArrayVersions[CLASSI],maxIndices,
                             &itemReopenFlags[CLASSI],&itemCodeFiles[CLASSI]);

         if (InheritanceLinksToCode(theEnv,&itemFiles[LINKI],fileName,pathName,fileNameBuffer,fileID,imageID,
                                    headerFP,&fileCount,maxIndices,theDefclass,
                                    &itemArrayVersions[LINKI],&itemArrayCounts[LINKI],
                                    &itemReopenFlags[LINKI],&itemCodeFiles[LINKI])
              == FALSE)
           goto ObjectCodeError;

         if (SlotsToCode(theEnv,&itemFiles[SLOTI],fileName,pathName,fileNameBuffer,fileID,imageID,
                         headerFP,&fileCount,maxIndices,theDefclass,
                         &itemArrayVersions[SLOTI],&itemArrayCounts[SLOTI],
                         &itemReopenFlags[SLOTI],&itemCodeFiles[SLOTI])
              == FALSE)
           goto ObjectCodeError;

         if (TemplateSlotsToCode(theEnv,&itemFiles[TSLOTI],fileName,pathName,fileNameBuffer,fileID,imageID,
                                 headerFP,&fileCount,maxIndices,theDefclass,
                                 &itemArrayVersions[TSLOTI],&itemArrayCounts[TSLOTI],
                                 &itemReopenFlags[TSLOTI],&itemCodeFiles[TSLOTI])
              == FALSE)
           goto ObjectCodeError;

         if (OrderedSlotsToCode(theEnv,&itemFiles[OSLOTI],fileName,pathName,fileNameBuffer,fileID,imageID,
                                headerFP,&fileCount,maxIndices,theDefclass,
                                &itemArrayVersions[OSLOTI],&itemArrayCounts[OSLOTI],
                                &itemReopenFlags[OSLOTI],&itemCodeFiles[OSLOTI])
              == FALSE)
           goto ObjectCodeError;

         if (HandlersToCode(theEnv,&itemFiles[HANDLERI],fileName,pathName,fileNameBuffer,fileID,imageID,
                            headerFP,&fileCount,maxIndices,theDefclass,
                            &itemArrayVersions[HANDLERI],&itemArrayCounts[HANDLERI],
                            &itemReopenFlags[HANDLERI],&itemCodeFiles[HANDLERI])
              == FALSE)
           goto ObjectCodeError;

         if (OrderedHandlersToCode(theEnv,&itemFiles[OHANDLERI],fileName,pathName,fileNameBuffer,fileID,imageID,
                                   headerFP,&fileCount,maxIndices,theDefclass,
                                   &itemArrayVersions[OHANDLERI],&itemArrayCounts[OHANDLERI],
                                   &itemReopenFlags[OHANDLERI],&itemCodeFiles[OHANDLERI])
              == FALSE)
           goto ObjectCodeError;
        }

      theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule);
      moduleCount++;
      itemArrayCounts[MODULEI]++;
     }

   CloseObjectFiles(theEnv,itemFiles,itemReopenFlags,itemCodeFiles,maxIndices);
   return(1);

ObjectCodeError:
   CloseObjectFiles(theEnv,itemFiles,itemReopenFlags,itemCodeFiles,maxIndices);
   return(0);
  }

/************************************************************
  NAME         : ClassIDMapToCode
  DESCRIPTION  : Writes out class id map
  INPUTS       : 1) Header file pointer
                 2) Output file pointer
                 3) The construct set image id
                 4) The max # of allowed indices
                 5) Caller's file count buffer
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : Class ID Map and Max Indices Written
  NOTES        : None
 ***********************************************************/
static int ClassIDMapToCode(
  void *theEnv,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices,
  int *fileCount)
  {
   FILE *classIDMapFile = NULL;
   int classIDMapArrayCount,
       classIDMapArrayVersion = 1;

   classIDMapFile = OpenFileIfNeeded(theEnv,classIDMapFile,fileName,pathName,fileNameBuffer,fileID,imageID,fileCount,
                                     classIDMapArrayVersion,headerFP,
                                     "DEFCLASS *",ClassIDPrefix(),FALSE,NULL);
   if (classIDMapFile == NULL)
     return(FALSE);
   for (classIDMapArrayCount = 0 ;
        classIDMapArrayCount < DefclassData(theEnv)->MaxClassID ;
        classIDMapArrayCount++)
     {
      if (classIDMapArrayCount > 0)
        fprintf(classIDMapFile,",\n");
      PrintClassReference(theEnv,classIDMapFile,DefclassData(theEnv)->ClassIDMap[classIDMapArrayCount],
                          imageID,maxIndices);
     }
   fprintf(classIDMapFile,"};\n\n");
   fprintf(classIDMapFile,"unsigned %s%d = %u;\n",
                          MaxClassIDPrefix(),imageID,(unsigned) DefclassData(theEnv)->MaxClassID);
   fprintf(headerFP,"extern unsigned %s%d;\n",MaxClassIDPrefix(),imageID);
   GenClose(theEnv,classIDMapFile);
   return(TRUE);
  }

/************************************************************
  NAME         : ClassHashTableToCode
  DESCRIPTION  : Writes out class hash table
  INPUTS       : 1) Header file pointer
                 2) Output file pointer
                 3) The construct set image id
                 4) The max # of allowed indices
                 5) Caller's file count buffer
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : Class Hash Table Written
  NOTES        : None
 ***********************************************************/
static int ClassHashTableToCode(
  void *theEnv,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices,
  int *fileCount)
  {
   FILE *classHashFile = NULL;
   int classHashArrayCount,
       classHashArrayVersion = 1;

   classHashFile = OpenFileIfNeeded(theEnv,classHashFile,fileName,pathName,fileNameBuffer,fileID,imageID,fileCount,
                                    classHashArrayVersion,headerFP,
                                    "DEFCLASS *",ClassHashPrefix(),FALSE,NULL);
   if (classHashFile == NULL)
     return(FALSE);
   for (classHashArrayCount = 0 ;
        classHashArrayCount < CLASS_TABLE_HASH_SIZE ;
        classHashArrayCount++)
     {
      if (classHashArrayCount > 0)
        fprintf(classHashFile,",\n");
      PrintClassReference(theEnv,classHashFile,DefclassData(theEnv)->ClassTable[classHashArrayCount],
                          imageID,maxIndices);
     }

   CloseFileIfNeeded(theEnv,classHashFile,&classHashArrayCount,
                     &classHashArrayVersion,classHashArrayCount,NULL,NULL);
   return(TRUE);
  }

/************************************************************
  NAME         : SlotNameHashTableToCode
  DESCRIPTION  : Writes out slot name entry hash table
  INPUTS       : 1) Header file pointer
                 2) Output file pointer
                 3) The construct set image id
                 4) The max # of allowed indices
                 5) Caller's version number buffer
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : Slot Name Hash Table Written
  NOTES        : None
 ***********************************************************/
static int SlotNameHashTableToCode(
  void *theEnv,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices,
  int *fileCount)
  {
   FILE *slotNameHashFile = NULL;
   int slotNameHashArrayCount,
       slotNameHashArrayVersion = 1;

   slotNameHashFile = OpenFileIfNeeded(theEnv,slotNameHashFile,fileName,pathName,fileNameBuffer,fileID,
                                       imageID,fileCount,
                                       slotNameHashArrayVersion,headerFP,
                                       "SLOT_NAME *",SlotNameHashPrefix(),FALSE,NULL);
   if (slotNameHashFile == NULL)
     return(FALSE);
   for (slotNameHashArrayCount = 0 ;
        slotNameHashArrayCount < SLOT_NAME_TABLE_HASH_SIZE ;
        slotNameHashArrayCount++)
     {
      if (slotNameHashArrayCount > 0)
        fprintf(slotNameHashFile,",\n");
      PrintSlotNameReference(theEnv,slotNameHashFile,DefclassData(theEnv)->SlotNameTable[slotNameHashArrayCount],
                             imageID,maxIndices);
     }
   CloseFileIfNeeded(theEnv,slotNameHashFile,&slotNameHashArrayCount,
                                        &slotNameHashArrayVersion,slotNameHashArrayCount,
                                        NULL,NULL);
   return(TRUE);
  }

/************************************************************
  NAME         : SlotNameEntriesToCode
  DESCRIPTION  : Writes out slot name entries
  INPUTS       : 1) Header file pointer
                 2) Output file pointer
                 3) The construct set image id
                 4) The max # of allowed indices
                 5) Caller's version number buffer
  RETURNS      : TRUE if all OK, FALSE otherwise
  SIDE EFFECTS : Slot name entries Written
  NOTES        : None
 ***********************************************************/
static int SlotNameEntriesToCode(
  void *theEnv,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices,
  int *fileCount)
  {
   FILE *slotNameFile = NULL;
   int slotNameArrayCount = 0,
       slotNameArrayVersion = 1;
   SLOT_NAME *snp;
   register unsigned i;

   for (i = 0 ; i < SLOT_NAME_TABLE_HASH_SIZE ; i++)
     {
      for (snp = DefclassData(theEnv)->SlotNameTable[i] ; snp != NULL ; snp = snp->nxt)
        {
         slotNameFile = OpenFileIfNeeded(theEnv,slotNameFile,fileName,pathName,fileNameBuffer,fileID,
                                       imageID,fileCount,
                                       slotNameArrayVersion,headerFP,
                                       "SLOT_NAME",SlotNamePrefix(),FALSE,NULL);
         if (slotNameFile == NULL)
           return(FALSE);
         fprintf(slotNameFile,"{ %u,1,%d,",snp->hashTableIndex,snp->id);
         PrintSymbolReference(theEnv,slotNameFile,snp->name);
         fprintf(slotNameFile,",");
         PrintSymbolReference(theEnv,slotNameFile,snp->putHandlerName);
         fprintf(slotNameFile,",");
         PrintSlotNameReference(theEnv,slotNameFile,snp->nxt,imageID,maxIndices);
         fprintf(slotNameFile,",0L }");
         slotNameArrayCount++;
         slotNameFile = CloseFileIfNeeded(theEnv,slotNameFile,&slotNameArrayCount,
                                          &slotNameArrayVersion,maxIndices,NULL,NULL);
        }
     }
   if (slotNameFile != NULL)
     CloseFileIfNeeded(theEnv,slotNameFile,&slotNameArrayCount,
                       &slotNameArrayVersion,slotNameArrayCount,NULL,NULL);
   return(TRUE);
  }

/******************************************************
  NAME         : CloseObjectFiles
  DESCRIPTION  : Closes construct compiler files
                  for defclass structures
  INPUTS       : 1) An array containing all the
                    pertinent file pointers
                 2) An array containing all the
                    pertinent file reopen flags
                 3) An array containing all the
                    pertinent file name/id/version info
                 4) The maximum number of indices
                    allowed in an array
  RETURNS      : Nothing useful
  SIDE EFFECTS : Files closed
  NOTES        : None
 *****************************************************/
static void CloseObjectFiles(
  void *theEnv,
  FILE *itemFiles[SAVE_ITEMS],
  int itemReopenFlags[SAVE_ITEMS],
  struct CodeGeneratorFile itemCodeFiles[SAVE_ITEMS],
  int maxIndices)
  {
   int count = maxIndices;
   int arrayVersion = 0;
   register int i;

   for (i = 0 ; i < SAVE_ITEMS ; i++)
     {
      count = maxIndices;
      itemFiles[i] = CloseFileIfNeeded(theEnv,itemFiles[i],&count,&arrayVersion,
                                       maxIndices,&itemReopenFlags[i],
                                       &itemCodeFiles[i]);
     }
  }

/***************************************************
  NAME         : DefclassModuleToCode
  DESCRIPTION  : Writes out the C values for a
                 defclass module item
  INPUTS       : 1) The output file
                 2) The module for the defclasses
                 3) The compile image id
                 4) The maximum number of elements
                    in an array
  RETURNS      : Nothing useful
  SIDE EFFECTS : Defclass module item written
  NOTES        : None
 ***************************************************/
static void DefclassModuleToCode(
  void *theEnv,
  FILE *theFile,
  struct defmodule *theModule,
  int imageID,
  int maxIndices)
  {
   fprintf(theFile,"{");
   ConstructModuleToCode(theEnv,theFile,theModule,imageID,maxIndices,
                         DefclassData(theEnv)->DefclassModuleIndex,ClassPrefix());
   fprintf(theFile,"}");
  }

/****************************************************************
  NAME         : SingleDefclassToCode
  DESCRIPTION  : Writes out a single defclass's
                 data to the file
  INPUTS       : 1)  The output file
                 2)  The compile image id
                 3)  The maximum number of
                     elements in an array
                 4)  The defclass
                 5)  The module index
                 6)  The partition holding the
                     defclass inheritance links
                 7)  The relative index of the
                     inheritance links in the partition
                 8)  The partition holding the
                     defclass slots
                 9)  The relative index of the
                     slots in the partition
                 10) The partition holding the
                     defclass template slots
                 11) The relative index of the
                     template slots in the partition
                 12) The partition holding the
                     defclass ordered slot map
                 13) The relative index of the
                     ordered slot map in the partition
                 14) The partition holding the
                     defclass message-handlers
                 15) The relative index of the
                     message-handlers in the partition
                 16) The partition holding the
                     defclass ordered handler map
                 17) The relative index of the
                     ordered handler map in the partition
  RETURNS      : Nothing useful
  SIDE EFFECTS : Defclass data written
  NOTES        : None
 ***************************************************************/
static void SingleDefclassToCode(
  void *theEnv,
  FILE *theFile,
  int imageID,
  int maxIndices,
  DEFCLASS *theDefclass,
  int moduleCount,
  int classLinkArrayVersion,
  int classLinkArrayCount,
  int slotArrayVersion,
  int slotArrayCount,
  int templateSlotArrayVersion,
  int templateSlotArrayCount,
  int orderedSlotArrayVersion,
  int orderedSlotArrayCount,
  int handlerArrayVersion,
  int handlerArrayCount,
  int orderedHandlerArrayVersion,
  int orderedHandlerArrayCount)
  {
   /* ==================
      Defclass Header
      ================== */
   fprintf(theFile,"{");
   ConstructHeaderToCode(theEnv,theFile,&theDefclass->header,imageID,maxIndices,moduleCount,
                         ModulePrefix(ObjectCompilerData(theEnv)->ObjectCodeItem),ClassPrefix());

   /* =========================
      Defclass specific data
      ========================= */
   fprintf(theFile,",1,%u,%u,%u,0,0,%u,0,%u,\n   ",
                   theDefclass->system,theDefclass->abstract,
                   theDefclass->reactive,(unsigned) theDefclass->id,
                   theDefclass->hashTableIndex);

   if (theDefclass->directSuperclasses.classCount > 0)
     fprintf(theFile,"{ %u,&%s%d_%d[%d] },",
                     (unsigned) theDefclass->directSuperclasses.classCount,
                     ClassLinkPrefix(),
                     imageID,classLinkArrayVersion,classLinkArrayCount);
   else
     fprintf(theFile,"{ 0,NULL },");
   classLinkArrayCount += theDefclass->directSuperclasses.classCount;

   if (theDefclass->directSubclasses.classCount > 0)
     fprintf(theFile,"{ %u,&%s%d_%d[%d] },",
                     (unsigned) theDefclass->directSubclasses.classCount,
                     ClassLinkPrefix(),
                     imageID,classLinkArrayVersion,classLinkArrayCount);
   else
     fprintf(theFile,"{ 0,NULL },");
   classLinkArrayCount += theDefclass->directSubclasses.classCount;

   if (theDefclass->allSuperclasses.classCount > 0)
     fprintf(theFile,"{ %u,&%s%d_%d[%d] },",
                     (unsigned) theDefclass->allSuperclasses.classCount,
                     ClassLinkPrefix(),
                     imageID,classLinkArrayVersion,classLinkArrayCount);
   else
     fprintf(theFile,"{ 0,NULL },\n   ");

   if (theDefclass->slots != NULL)
     fprintf(theFile,"&%s%d_%d[%d],",
                     SlotPrefix(),imageID,
                     slotArrayVersion,slotArrayCount);
   else
     fprintf(theFile,"NULL,");

   if (theDefclass->instanceTemplate != NULL)
     fprintf(theFile,"&%s%d_%d[%d],",
                     TemplateSlotPrefix(),imageID,
                     templateSlotArrayVersion,templateSlotArrayCount);
   else
     fprintf(theFile,"NULL,");

   if (theDefclass->slotNameMap != NULL)
     fprintf(theFile,"&%s%d_%d[%d],",
                     OrderedSlotPrefix(),imageID,
                     orderedSlotArrayVersion,orderedSlotArrayCount);
   else
     fprintf(theFile,"NULL,");

   fprintf(theFile,"%hd,%hd,%hd,%hd,NULL,NULL,\n   ",
                   theDefclass->slotCount,theDefclass->localInstanceSlotCount,
                   theDefclass->instanceSlotCount,theDefclass->maxSlotNameID);

   if (theDefclass->handlers != NULL)
     fprintf(theFile,"&%s%d_%d[%d],",
                     HandlerPrefix(),imageID,
                     handlerArrayVersion,handlerArrayCount);
   else
     fprintf(theFile,"NULL,");

   if (theDefclass->handlerOrderMap != NULL)
     fprintf(theFile,"&%s%d_%d[%d],",
                     OrderedHandlerPrefix(),imageID,
                     orderedHandlerArrayVersion,orderedHandlerArrayCount);
   else
     fprintf(theFile,"NULL,");

   fprintf(theFile,"%hd,",theDefclass->handlerCount);
   PrintClassReference(theEnv,theFile,theDefclass->nxtHash,imageID,maxIndices);
   fprintf(theFile,",");
   PrintBitMapReference(theEnv,theFile,theDefclass->scopeMap);
   fprintf(theFile,",\"\"}");
  }

/***********************************************************
  NAME         : InheritanceLinksToCode
  DESCRIPTION  : Prints out superclass/subclass
                 inheritance links - all links
                 for a particular class are
                 guaranteed to be in the same
                 array partition
  INPUTS       : 1)  A buffer for the inheritance links file
                 2)  The base image name
                 3)  The id for this type of data
                 4)  The base image id
                 5)  The general header file
                 6)  A buffer for the version number of
                     the file for this type of data
                 7)  The maximum # of elements in any array
                 8)  A pointer to the class
                 9)  A buffer holding the links partition #
                 10) A buffer holding the links relative
                     index in the partition
                 11) A buffer for a flag indicating if the
                     buffer file can be reopened later
                 12) A pointer to the file info for
                     this data if the last file needs
                     to be reopened for termination
  RETURNS      : TRUE if all OK, FALSE
                 otherwise
  SIDE EFFECTS : Inheritance links written
  NOTES        : None
 ***********************************************************/
static intBool InheritanceLinksToCode(
  void *theEnv,
  FILE **classLinkFile,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  int imageID,
  FILE *headerFP,
  int *fileCount,
  int maxIndices,
  DEFCLASS *theDefclass,
  int *classLinkArrayVersion,
  int *classLinkArrayCount,
  int *reopenClassLinkFile,
  struct CodeGeneratorFile *classLinkCodeFile)
  {
   long i;
   int inheritanceLinkCount,
       linkPrinted = FALSE;

   inheritanceLinkCount = theDefclass->directSuperclasses.classCount +
                          theDefclass->directSubclasses.classCount +
                          theDefclass->allSuperclasses.classCount;

   if (inheritanceLinkCount == 0)
     return(TRUE);

   *classLinkFile = OpenFileIfNeeded(theEnv,*classLinkFile,fileName,pathName,fileNameBuffer,fileID,
                                      imageID,fileCount,
                                      *classLinkArrayVersion,headerFP,
                                      "DEFCLASS *",ClassLinkPrefix(),
                                      *reopenClassLinkFile,classLinkCodeFile);
   if (*classLinkFile == NULL)
     return(FALSE);

   for (i = 0 ; i < theDefclass->directSuperclasses.classCount ; i++)
     {
      if (linkPrinted)
        fprintf(*classLinkFile,",");
      PrintClassReference(theEnv,*classLinkFile,
                          theDefclass->directSuperclasses.classArray[i],
                          imageID,maxIndices);
      linkPrinted = TRUE;
     }
   for (i = 0 ; i < theDefclass->directSubclasses.classCount ; i++)
     {
      if (linkPrinted)
        fprintf(*classLinkFile,",");
      PrintClassReference(theEnv,*classLinkFile,
                          theDefclass->directSubclasses.classArray[i],
                          imageID,maxIndices);
      linkPrinted = TRUE;
     }
   for (i = 0 ; i < theDefclass->allSuperclasses.classCount ; i++)
     {
      if (linkPrinted)
        fprintf(*classLinkFile,",");
      PrintClassReference(theEnv,*classLinkFile,
                          theDefclass->allSuperclasses.classArray[i],
                          imageID,maxIndices);
      linkPrinted = TRUE;
     }
   *classLinkArrayCount += inheritanceLinkCount;
   *classLinkFile = CloseFileIfNeeded(theEnv,*classLinkFile,classLinkArrayCount,
                                       classLinkArrayVersion,maxIndices,
                                       reopenClassLinkFile,classLinkCodeFile);
   return(TRUE);
  }

/***********************************************************
  NAME         : SlotsToCode
  DESCRIPTION  : Prints out slots - all slots
                 for a particular class are
                 guaranteed to be in the same
                 array partition
  INPUTS       : 1)  A buffer for the slots file
                 2)  The base image name
                 3)  The id for this type of data
                 4)  The base image id
                 5)  The general header file
                 6)  A buffer for the version number of
                     the file for this type of data
                 7)  The maximum # of elements in any array
                 8)  A pointer to the class
                 9)  A buffer holding the slots partition #
                 10) A buffer holding the slots relative
                     index in the partition
                 11) A buffer for a flag indicating if the
                     buffer file can be reopened later
                 12) A pointer to the file info for
                     this data if the last file needs
                     to be reopened for termination
  RETURNS      : TRUE if all OK, FALSE
                 otherwise
  SIDE EFFECTS : Slots written
  NOTES        : None
 ***********************************************************/
static intBool SlotsToCode(
  void *theEnv,
  FILE **slotFile,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  int imageID,
  FILE *headerFP,
  int *fileCount,
  int maxIndices,
  DEFCLASS *theDefclass,
  int *slotArrayVersion,
  int *slotArrayCount,
  int *reopenSlotFile,
  struct CodeGeneratorFile *slotCodeFile)
  {
   long i;
   SLOT_DESC *sd;
   EXPRESSION *tmpexp;
   PACKED_LOCATION_INFO theLocationInfo;

   if (theDefclass->slotCount == 0)
     return(TRUE);

   *slotFile = OpenFileIfNeeded(theEnv,*slotFile,fileName,pathName,fileNameBuffer,fileID,
                                imageID,fileCount,
                                *slotArrayVersion,headerFP,
                                "SLOT_DESC",SlotPrefix(),
                                *reopenSlotFile,slotCodeFile);
   if (*slotFile == NULL)
     return(FALSE);

   for (i = 0 ; i < theDefclass->slotCount ; i++)
     {
      sd = &theDefclass->slots[i];
      if (i > 0)
        fprintf(*slotFile,",\n");
      fprintf(*slotFile,"{ %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,",
                        sd->shared,sd->multiple,
                        sd->composite,sd->noInherit,
                        sd->noWrite,sd->initializeOnly,
                        sd->dynamicDefault,sd->defaultSpecified,
                        sd->noDefault,sd->reactive,
                        sd->publicVisibility,sd->createReadAccessor,
                        sd->createWriteAccessor,sd->overrideMessageSpecified);
      PrintClassReference(theEnv,*slotFile,sd->cls,imageID,maxIndices);
      fprintf(*slotFile,",");
      PrintSlotNameReference(theEnv,*slotFile,sd->slotName,imageID,maxIndices);
      fprintf(*slotFile,",\n   ");
      PrintSymbolReference(theEnv,*slotFile,sd->overrideMessage);
      if (sd->defaultValue != NULL)
        {
         fprintf(*slotFile,",(void *) ");
         if (sd->dynamicDefault)
           ExpressionToCode(theEnv,*slotFile,(EXPRESSION *) sd->defaultValue);
         else
           {
            tmpexp = ConvertValueToExpression(theEnv,(DATA_OBJECT *) sd->defaultValue);
            ExpressionToCode(theEnv,*slotFile,tmpexp);
            ReturnExpression(theEnv,tmpexp);
           }
        }
      else
        fprintf(*slotFile,",NULL");
      fprintf(*slotFile,",");
      PrintConstraintReference(theEnv,*slotFile,sd->constraint,imageID,maxIndices);
      fprintf(*slotFile,",0,0L,");
      if (sd->shared)
        {
         theLocationInfo.theLong = sd->sharedValue.desc->bsaveIndex;
         fprintf(*slotFile,"{ &%s%d_%u[%u],0,0,0,NULL } }",
                           SlotPrefix(),imageID,
                           theLocationInfo.theLocation.thePartition,
                           theLocationInfo.theLocation.theOffset);
        }
      else
        fprintf(*slotFile,"{ NULL,0,0,0,NULL } }");
     }
   *slotArrayCount += (int) theDefclass->slotCount;
   *slotFile = CloseFileIfNeeded(theEnv,*slotFile,slotArrayCount,
                                 slotArrayVersion,maxIndices,
                                 reopenSlotFile,slotCodeFile);
   return(TRUE);
  }

/*************************************************************
  NAME         : TemplateSlotsToCode
  DESCRIPTION  : Prints out instance template -
                 the entire instance slot template
                 for a particular class is
                 guaranteed to be in the same
                 array partition
  INPUTS       : 1)  A buffer for the template file
                 2)  The base image name
                 3)  The id for this type of data
                 4)  The base image id
                 5)  The general header file
                 6)  A buffer for the version number of
                     the file for this type of data
                 7)  The maximum # of elements in any array
                 8)  A pointer to the class
                 9)  A buffer holding the template partition #
                 10) A buffer holding the template relative
                     index in the partition
                 11) A buffer for a flag indicating if the
                     buffer file can be reopened later
                 12) A pointer to the file info for
                     this data if the last file needs
                     to be reopened for termination
  RETURNS      : TRUE if all OK, FALSE
                 otherwise
  SIDE EFFECTS : Templates written
  NOTES        : None
 *************************************************************/
static intBool TemplateSlotsToCode(
  void *theEnv,
  FILE **templateSlotFile,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  int imageID,
  FILE *headerFP,
  int *fileCount,
  int maxIndices,
  DEFCLASS *theDefclass,
  int *templateSlotArrayVersion,
  int *templateSlotArrayCount,
  int *reopenTemplateSlotFile,
  struct CodeGeneratorFile *templateSlotCodeFile)
  {
   long i;
   SLOT_DESC *sd;
   PACKED_LOCATION_INFO theLocationInfo;

   if (theDefclass->instanceSlotCount == 0)
     return(TRUE);

   *templateSlotFile = OpenFileIfNeeded(theEnv,*templateSlotFile,fileName,pathName,fileNameBuffer,fileID,
                                        imageID,fileCount,
                                        *templateSlotArrayVersion,headerFP,
                                        "SLOT_DESC *",TemplateSlotPrefix(),
                                        *reopenTemplateSlotFile,templateSlotCodeFile);
   if (*templateSlotFile == NULL)
     return(FALSE);

   for (i = 0 ; i < theDefclass->instanceSlotCount ; i++)
     {
      sd = theDefclass->instanceTemplate[i];
      if (i > 0)
        fprintf(*templateSlotFile,",");
      theLocationInfo.theLong = sd->bsaveIndex;
      fprintf(*templateSlotFile,"&%s%d_%u[%u]",
                                SlotPrefix(),imageID,
                                theLocationInfo.theLocation.thePartition,
                                theLocationInfo.theLocation.theOffset);
     }
   *templateSlotArrayCount += (int) theDefclass->instanceSlotCount;
   *templateSlotFile = CloseFileIfNeeded(theEnv,*templateSlotFile,templateSlotArrayCount,
                                         templateSlotArrayVersion,maxIndices,
                                         reopenTemplateSlotFile,templateSlotCodeFile);
   return(TRUE);
  }

/*************************************************************
  NAME         : OrderedSlotsToCode
  DESCRIPTION  : Prints out slot name map -
                 the entire slot name map
                 for a particular class is
                 guaranteed to be in the same
                 array partition
  INPUTS       : 1)  A buffer for the slot map file
                 2)  The base image name
                 3)  The id for this type of data
                 4)  The base image id
                 5)  The general header file
                 6)  A buffer for the version number of
                     the file for this type of data
                 7)  The maximum # of elements in any array
                 8)  A pointer to the class
                 9)  A buffer holding the slot map partition #
                 10) A buffer holding the slot map relative
                     index in the partition
                 11) A buffer for a flag indicating if the
                     buffer file can be reopened later
                 12) A pointer to the file info for
                     this data if the last file needs
                     to be reopened for termination
  RETURNS      : TRUE if all OK, FALSE
                 otherwise
  SIDE EFFECTS : Slot maps written
  NOTES        : None
 *************************************************************/
static intBool OrderedSlotsToCode(
  void *theEnv,
  FILE **orderedSlotFile,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  int imageID,
  FILE *headerFP,
  int *fileCount,
  int maxIndices,
  DEFCLASS *theDefclass,
  int *orderedSlotArrayVersion,
  int *orderedSlotArrayCount,
  int *reopenOrderedSlotFile,
  struct CodeGeneratorFile *orderedSlotCodeFile)
  {
   long i;

   if (theDefclass->instanceSlotCount == 0)
     return(TRUE);

   *orderedSlotFile = OpenFileIfNeeded(theEnv,*orderedSlotFile,fileName,pathName,fileNameBuffer,fileID,
                                        imageID,fileCount,
                                        *orderedSlotArrayVersion,headerFP,
                                        "unsigned",OrderedSlotPrefix(),
                                        *reopenOrderedSlotFile,orderedSlotCodeFile);
   if (*orderedSlotFile == NULL)
     return(FALSE);

   for (i = 0 ; i <= theDefclass->maxSlotNameID ; i++)
     {
      if (i > 0)
        fprintf(*orderedSlotFile,",");
      fprintf(*orderedSlotFile,"%u",theDefclass->slotNameMap[i]);
     }
   *orderedSlotArrayCount += (int) theDefclass->maxSlotNameID + 1;
   *orderedSlotFile = CloseFileIfNeeded(theEnv,*orderedSlotFile,orderedSlotArrayCount,
                                        orderedSlotArrayVersion,maxIndices,
                                        reopenOrderedSlotFile,orderedSlotCodeFile);
   return(TRUE);
  }

/*************************************************************
  NAME         : HandlersToCode
  DESCRIPTION  : Prints out message-handlers -
                 all message-handlers for a particular class
                 are guaranteed to be in the same array
                 partition
  INPUTS       : 1)  A buffer for the handler file
                 2)  The base image name
                 3)  The id for this type of data
                 4)  The base image id
                 5)  The general header file
                 6)  A buffer for the version number of
                     the file for this type of data
                 7)  The maximum # of elements in any array
                 8)  A pointer to the class
                 9)  A buffer holding the handler partition #
                 10) A buffer holding the handler relative
                     index in the partition
                 11) A buffer for a flag indicating if the
                     buffer file can be reopened later
                 12) A pointer to the file info for
                     this data if the last file needs
                     to be reopened for termination
  RETURNS      : TRUE if all OK, FALSE
                 otherwise
  SIDE EFFECTS : Handlers written
  NOTES        : None
 *************************************************************/
static intBool HandlersToCode(
  void *theEnv,
  FILE **handlerFile,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  int imageID,
  FILE *headerFP,
  int *fileCount,
  int maxIndices,
  DEFCLASS *theDefclass,
  int *handlerArrayVersion,
  int *handlerArrayCount,
  int *reopenHandlerFile,
  struct CodeGeneratorFile *handlerCodeFile)
  {
   long i;
   HANDLER *hnd;

   if (theDefclass->handlerCount == 0)
     return(TRUE);

   *handlerFile = OpenFileIfNeeded(theEnv,*handlerFile,fileName,pathName,fileNameBuffer,fileID,
                                        imageID,fileCount,
                                        *handlerArrayVersion,headerFP,
                                        "HANDLER",HandlerPrefix(),*reopenHandlerFile,
                                        handlerCodeFile);
   if (*handlerFile == NULL)
     return(FALSE);

   for (i = 0 ; i < theDefclass->handlerCount ; i++)
     {
      if (i > 0)
        fprintf(*handlerFile,",\n");
      hnd = &theDefclass->handlers[i];
      fprintf(*handlerFile,"{ %u,%u,0,0,0,",hnd->system,hnd->type);
      PrintSymbolReference(theEnv,*handlerFile,hnd->name);
      fprintf(*handlerFile,",");
      PrintClassReference(theEnv,*handlerFile,hnd->cls,imageID,maxIndices);
      fprintf(*handlerFile,",%hd,%hd,%hd,",hnd->minParams,hnd->maxParams,
                                        hnd->localVarCount);
      ExpressionToCode(theEnv,*handlerFile,hnd->actions);
      fprintf(*handlerFile,",NULL }");
     }
   *handlerArrayCount += (int) theDefclass->handlerCount;
   *handlerFile = CloseFileIfNeeded(theEnv,*handlerFile,handlerArrayCount,
                                    handlerArrayVersion,maxIndices,
                                    reopenHandlerFile,handlerCodeFile);
   return(TRUE);
  }

/****************************************************************
  NAME         : OrderedHandlersToCode
  DESCRIPTION  : Prints out handler map -
                 the entire handler map
                 for a particular class is
                 guaranteed to be in the same
                 array partition
  INPUTS       : 1)  A buffer for the handler map file
                 2)  The base image name
                 3)  The id for this type of data
                 4)  The base image id
                 5)  The general header file
                 6)  A buffer for the version number of
                     the file for this type of data
                 7)  The maximum # of elements in any array
                 8)  A pointer to the class
                 9)  A buffer holding the handler map partition #
                 10) A buffer holding the handler map relative
                     index in the partition
                 11) A buffer for a flag indicating if the
                     buffer file can be reopened later
                 12) A pointer to the file info for
                     this data if the last file needs
                     to be reopened for termination
  RETURNS      : TRUE if all OK, FALSE
                 otherwise
  SIDE EFFECTS : Handler maps written
  NOTES        : None
 ****************************************************************/
static intBool OrderedHandlersToCode(
  void *theEnv,
  FILE **orderedHandlerFile,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  int imageID,
  FILE *headerFP,
  int *fileCount,
  int maxIndices,
  DEFCLASS *theDefclass,
  int *orderedHandlerArrayVersion,
  int *orderedHandlerArrayCount,
  int *reopenOrderedHandlerFile,
  struct CodeGeneratorFile *orderedHandlerCodeFile)
  {
   long i;

   if (theDefclass->handlerCount == 0)
     return(TRUE);

   *orderedHandlerFile = OpenFileIfNeeded(theEnv,*orderedHandlerFile,fileName,pathName,fileNameBuffer,fileID,
                                          imageID,fileCount,
                                          *orderedHandlerArrayVersion,headerFP,
                                          "unsigned",OrderedHandlerPrefix(),
                                          *reopenOrderedHandlerFile,
                                          orderedHandlerCodeFile);
   if (*orderedHandlerFile == NULL)
     return(FALSE);

   for (i = 0 ; i < theDefclass->handlerCount ; i++)
     {
      if (i > 0)
        fprintf(*orderedHandlerFile,",");
      fprintf(*orderedHandlerFile,"%u",theDefclass->handlerOrderMap[i]);
     }
   *orderedHandlerArrayCount += (int) theDefclass->handlerCount;
   *orderedHandlerFile = CloseFileIfNeeded(theEnv,*orderedHandlerFile,orderedHandlerArrayCount,
                                           orderedHandlerArrayVersion,maxIndices,
                                           reopenOrderedHandlerFile,
                                           orderedHandlerCodeFile);
   return(TRUE);
  }

#endif


