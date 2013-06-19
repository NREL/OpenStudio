
   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*                 FILE COMMANDS MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for file commands including    */
/*   batch, dribble-on, dribble-off, save, load, bsave, and  */
/*   bload.                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Bebe Ly                                              */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Added environment parameter to GenClose.       */
/*            Added environment parameter to GenOpen.        */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#define _FILECOM_SOURCE_

#include <stdio.h>

#define _STDIO_INCLUDED_
#include <string.h>

#include "setup.h"

#include "argacces.h"
#include "constrct.h"
#include "commline.h"
#include "cstrcpsr.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "memalloc.h"
#include "prcdrfun.h"
#include "router.h"
#include "strngrtr.h"
#include "sysdep.h"
#include "utility.h"

#include "filecom.h"

#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
#include "bsave.h"
#include "bload.h"
#endif

/***************/
/* STRUCTURES  */
/***************/

struct batchEntry
  {
   int batchType;
   void *inputSource;
   char *theString;
   struct batchEntry *next;
  };

/***************/
/* DEFINITIONS */
/***************/

#define FILE_BATCH      0
#define STRING_BATCH    1

#define BUFFER_SIZE   120

#define FILECOM_DATA 14

struct fileCommandData
  { 
#if DEBUGGING_FUNCTIONS
   FILE *DribbleFP;
   char *DribbleBuffer;
   size_t DribbleCurrentPosition;
   size_t DribbleMaximumPosition;
   int (*DribbleStatusFunction)(void *,int);
#endif
   int BatchType;
   void *BatchSource;
   char *BatchBuffer;
   size_t BatchCurrentPosition;
   size_t BatchMaximumPosition;
   struct batchEntry *TopOfBatchList;
   struct batchEntry *BottomOfBatchList;
  };

#define FileCommandData(theEnv) ((struct fileCommandData *) GetEnvironmentData(theEnv,FILECOM_DATA))

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if DEBUGGING_FUNCTIONS
   static int                     FindDribble(void *,char *);
   static int                     GetcDribble(void *,char *);
   static int                     UngetcDribble(void *,int,char *);
   static int                     ExitDribble(void *,int);
   static int                     PrintDribble(void *,char *,char *);
   static void                    PutcDribbleBuffer(void *,int);
#endif
   static int                     FindBatch(void *,char *);
   static int                     GetcBatch(void *,char *);
   static int                     UngetcBatch(void *,int,char *);
   static int                     ExitBatch(void *,int);
   static void                    AddBatch(void *,int,void *,int,char *);
   static void                    DeallocateFileCommandData(void *);

/***************************************/
/* FileCommandDefinitions: Initializes */
/*   file commands.                    */
/***************************************/
globle void FileCommandDefinitions(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,FILECOM_DATA,sizeof(struct fileCommandData),DeallocateFileCommandData);

#if ! RUN_TIME
#if DEBUGGING_FUNCTIONS
   EnvDefineFunction2(theEnv,"batch",'b',PTIEF BatchCommand,"BatchCommand","11k");
   EnvDefineFunction2(theEnv,"batch*",'b',PTIEF BatchStarCommand,"BatchStarCommand","11k");
   EnvDefineFunction2(theEnv,"dribble-on",'b',PTIEF DribbleOnCommand,"DribbleOnCommand","11k");
   EnvDefineFunction2(theEnv,"dribble-off",'b',PTIEF DribbleOffCommand,"DribbleOffCommand","00");
   EnvDefineFunction2(theEnv,"save",'b',PTIEF SaveCommand,"SaveCommand","11k");
#endif
   EnvDefineFunction2(theEnv,"load",'b',PTIEF LoadCommand,"LoadCommand","11k");
   EnvDefineFunction2(theEnv,"load*",'b',PTIEF LoadStarCommand,"LoadStarCommand","11k");
#if BLOAD_AND_BSAVE
   InitializeBsaveData(theEnv);
   EnvDefineFunction2(theEnv,"bsave",'b', PTIEF BsaveCommand,"BsaveCommand","11k");
#endif
#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
   InitializeBloadData(theEnv);
   EnvDefineFunction2(theEnv,"bload",'b',PTIEF BloadCommand,"BloadCommand","11k");
#endif
#endif
  }
  
/******************************************************/
/* DeallocateFileCommandData: Deallocates environment */
/*    data for file commands.                         */
/******************************************************/
static void DeallocateFileCommandData(
  void *theEnv)
  {
   struct batchEntry *theEntry, *nextEntry;
   
   theEntry = FileCommandData(theEnv)->TopOfBatchList;
   while (theEntry != NULL)
     {
      nextEntry = theEntry->next;

      if (theEntry->batchType == FILE_BATCH)
        { GenClose(theEnv,(FILE *) FileCommandData(theEnv)->TopOfBatchList->inputSource); }
      else
        { rm(theEnv,theEntry->theString,strlen(theEntry->theString) + 1); }

      rtn_struct(theEnv,batchEntry,theEntry);
         
      theEntry = nextEntry;
     }
     
   if (FileCommandData(theEnv)->BatchBuffer != NULL)
     { rm(theEnv,FileCommandData(theEnv)->BatchBuffer,FileCommandData(theEnv)->BatchMaximumPosition); }

#if DEBUGGING_FUNCTIONS
   if (FileCommandData(theEnv)->DribbleBuffer != NULL)
     { rm(theEnv,FileCommandData(theEnv)->DribbleBuffer,FileCommandData(theEnv)->DribbleMaximumPosition); }
     
   if (FileCommandData(theEnv)->DribbleFP != NULL) 
     { GenClose(theEnv,FileCommandData(theEnv)->DribbleFP); }
#endif
  }
  
#if DEBUGGING_FUNCTIONS
/*****************************************************/
/* FindDribble: Find routine for the dribble router. */
/*****************************************************/
#if WIN_BTC
#pragma argsused
#endif
static int FindDribble(
  void *theEnv,
  char *logicalName)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   if ( (strcmp(logicalName,"stdout") == 0) ||
        (strcmp(logicalName,"stdin") == 0) ||
        (strcmp(logicalName,WPROMPT) == 0) ||
        (strcmp(logicalName,WTRACE) == 0) ||
        (strcmp(logicalName,WERROR) == 0) ||
        (strcmp(logicalName,WWARNING) == 0) ||
        (strcmp(logicalName,WDISPLAY) == 0) ||
        (strcmp(logicalName,WDIALOG) == 0) )
     { return(TRUE); }

    return(FALSE);
  }

/*******************************************************/
/* PrintDribble: Print routine for the dribble router. */
/*******************************************************/
static int PrintDribble(
  void *theEnv,
  char *logicalName,
  char *str)
  {
   int i;

   /*======================================*/
   /* Send the output to the dribble file. */
   /*======================================*/

   for (i = 0 ; str[i] != EOS ; i++)
     { PutcDribbleBuffer(theEnv,str[i]); }

   /*===========================================================*/
   /* Send the output to any routers interested in printing it. */
   /*===========================================================*/

   EnvDeactivateRouter(theEnv,"dribble");
   EnvPrintRouter(theEnv,logicalName,str);
   EnvActivateRouter(theEnv,"dribble");

   return(1);
  }

/*****************************************************/
/* GetcDribble: Getc routine for the dribble router. */
/*****************************************************/
static int GetcDribble(
  void *theEnv,
  char *logicalName)
  {
   int rv;

   /*===========================================*/
   /* Deactivate the dribble router and get the */
   /* character from another active router.     */
   /*===========================================*/

   EnvDeactivateRouter(theEnv,"dribble");
   rv = EnvGetcRouter(theEnv,logicalName);
   EnvActivateRouter(theEnv,"dribble");

   /*==========================================*/
   /* Put the character retrieved from another */
   /* router into the dribble buffer.          */
   /*==========================================*/

   PutcDribbleBuffer(theEnv,rv);

   /*=======================*/
   /* Return the character. */
   /*=======================*/

   return(rv);
  }

/***********************************************************/
/* PutcDribbleBuffer: Putc routine for the dribble router. */
/***********************************************************/
static void PutcDribbleBuffer(
  void *theEnv,
  int rv)
  {
   /*===================================================*/
   /* Receiving an end-of-file character will cause the */
   /* contents of the dribble buffer to be flushed.     */
   /*===================================================*/

   if (rv == EOF)
     {
      if (FileCommandData(theEnv)->DribbleCurrentPosition > 0)
        {
         fprintf(FileCommandData(theEnv)->DribbleFP,"%s",FileCommandData(theEnv)->DribbleBuffer);
         FileCommandData(theEnv)->DribbleCurrentPosition = 0;
         FileCommandData(theEnv)->DribbleBuffer[0] = EOS;
        }
     }

   /*===========================================================*/
   /* If we aren't receiving command input, then the character  */
   /* just received doesn't need to be placed in the dribble    */
   /* buffer--It can be written directly to the file. This will */
   /* occur for example when the command prompt is being        */
   /* printed (the AwaitingInput variable will be FALSE because */
   /* command input has not been receivied yet). Before writing */
   /* the character to the file, the dribble buffer is flushed. */
   /*===========================================================*/

   else if (RouterData(theEnv)->AwaitingInput == FALSE)
     {
      if (FileCommandData(theEnv)->DribbleCurrentPosition > 0)
        {
         fprintf(FileCommandData(theEnv)->DribbleFP,"%s",FileCommandData(theEnv)->DribbleBuffer);
         FileCommandData(theEnv)->DribbleCurrentPosition = 0;
         FileCommandData(theEnv)->DribbleBuffer[0] = EOS;
        }

      fputc(rv,FileCommandData(theEnv)->DribbleFP);
     }

   /*=====================================================*/
   /* Otherwise, add the character to the dribble buffer. */
   /*=====================================================*/

   else
     {
      FileCommandData(theEnv)->DribbleBuffer = ExpandStringWithChar(theEnv,rv,FileCommandData(theEnv)->DribbleBuffer,
                                           &FileCommandData(theEnv)->DribbleCurrentPosition,
                                           &FileCommandData(theEnv)->DribbleMaximumPosition,
                                           FileCommandData(theEnv)->DribbleMaximumPosition+BUFFER_SIZE);
     }
  }

/*********************************************************/
/* UngetcDribble: Ungetc routine for the dribble router. */
/*********************************************************/
static int UngetcDribble(
  void *theEnv,
  int ch,
  char *logicalName)
  {
   int rv;

   /*===============================================*/
   /* Remove the character from the dribble buffer. */
   /*===============================================*/

   if (FileCommandData(theEnv)->DribbleCurrentPosition > 0) FileCommandData(theEnv)->DribbleCurrentPosition--;
   FileCommandData(theEnv)->DribbleBuffer[FileCommandData(theEnv)->DribbleCurrentPosition] = EOS;

   /*=============================================*/
   /* Deactivate the dribble router and pass the  */
   /* ungetc request to the other active routers. */
   /*=============================================*/

   EnvDeactivateRouter(theEnv,"dribble");
   rv = EnvUngetcRouter(theEnv,ch,logicalName);
   EnvActivateRouter(theEnv,"dribble");

   /*==========================================*/
   /* Return the result of the ungetc request. */
   /*==========================================*/

   return(rv);
  }

/*****************************************************/
/* ExitDribble: Exit routine for the dribble router. */
/*****************************************************/
#if WIN_BTC
#pragma argsused
#endif
static int ExitDribble(
  void *theEnv,
  int num)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(num)
#endif

   if (FileCommandData(theEnv)->DribbleCurrentPosition > 0)
     { fprintf(FileCommandData(theEnv)->DribbleFP,"%s",FileCommandData(theEnv)->DribbleBuffer); }
 
   if (FileCommandData(theEnv)->DribbleFP != NULL) GenClose(theEnv,FileCommandData(theEnv)->DribbleFP);
   return(1);
  }

/******************************************/
/* DribbleOnCommand: H/L access routine   */
/*   for the dribble-on command.          */
/******************************************/
globle int DribbleOnCommand(
  void *theEnv)
  {
   char *fileName;

   if (EnvArgCountCheck(theEnv,"dribble-on",EXACTLY,1) == -1) return(FALSE);
   if ((fileName = GetFileName(theEnv,"dribble-on",1)) == NULL) return(FALSE);

   return (EnvDribbleOn(theEnv,fileName));
  }

/**********************************/
/* EnvDribbleOn: C access routine */
/*   for the dribble-on command.  */
/**********************************/
globle intBool EnvDribbleOn(
  void *theEnv,
  char *fileName)
  {
   /*==============================*/
   /* If a dribble file is already */
   /* open, then close it.         */
   /*==============================*/

   if (FileCommandData(theEnv)->DribbleFP != NULL)
     { EnvDribbleOff(theEnv); }

   /*========================*/
   /* Open the dribble file. */
   /*========================*/

   FileCommandData(theEnv)->DribbleFP = GenOpen(theEnv,fileName,"w");
   if (FileCommandData(theEnv)->DribbleFP == NULL)
     {
      OpenErrorMessage(theEnv,"dribble-on",fileName);
      return(0);
     }

   /*============================*/
   /* Create the dribble router. */
   /*============================*/

   EnvAddRouter(theEnv,"dribble", 40,
             FindDribble, PrintDribble,
             GetcDribble, UngetcDribble,
             ExitDribble);

   FileCommandData(theEnv)->DribbleCurrentPosition = 0;

   /*================================================*/
   /* Call the dribble status function. This is used */
   /* by some of the machine specific interfaces to  */
   /* do things such as changing the wording of menu */
   /* items from "Turn Dribble On..." to             */
   /* "Turn Dribble Off..."                          */
   /*================================================*/

   if (FileCommandData(theEnv)->DribbleStatusFunction != NULL)
     { (*FileCommandData(theEnv)->DribbleStatusFunction)(theEnv,TRUE); }

   /*=====================================*/
   /* Return TRUE to indicate the dribble */
   /* file was successfully opened.       */
   /*=====================================*/

   return(TRUE);
  }

/*************************************************/
/* EnvDribbleActive: Returns TRUE if the dribble */
/*   router is active, otherwise FALSE>          */
/*************************************************/
globle intBool EnvDribbleActive(
  void *theEnv)
  {
   if (FileCommandData(theEnv)->DribbleFP != NULL) return(TRUE);

   return(FALSE);
  }

/*******************************************/
/* DribbleOffCommand: H/L access  routine  */
/*   for the dribble-off command.          */
/*******************************************/
globle int DribbleOffCommand(
  void *theEnv)
  {
   if (EnvArgCountCheck(theEnv,"dribble-off",EXACTLY,0) == -1) return(FALSE);
   return(EnvDribbleOff(theEnv));
  }

/***********************************/
/* EnvDribbleOff: C access routine */
/*   for the dribble-off command.  */
/***********************************/
globle intBool EnvDribbleOff(
  void *theEnv)
  {
   int rv = 0;

   /*================================================*/
   /* Call the dribble status function. This is used */
   /* by some of the machine specific interfaces to  */
   /* do things such as changing the wording of menu */
   /* items from "Turn Dribble On..." to             */
   /* "Turn Dribble Off..."                          */
   /*================================================*/

   if (FileCommandData(theEnv)->DribbleStatusFunction != NULL)
     { (*FileCommandData(theEnv)->DribbleStatusFunction)(theEnv,FALSE); }

   /*=======================================*/
   /* Close the dribble file and deactivate */
   /* the dribble router.                   */
   /*=======================================*/

   if (FileCommandData(theEnv)->DribbleFP != NULL)
     {
      if (FileCommandData(theEnv)->DribbleCurrentPosition > 0)
        { fprintf(FileCommandData(theEnv)->DribbleFP,"%s",FileCommandData(theEnv)->DribbleBuffer); }
      EnvDeleteRouter(theEnv,"dribble");
      if (GenClose(theEnv,FileCommandData(theEnv)->DribbleFP) == 0) rv = 1;
     }
   else
     { rv = 1; }

   FileCommandData(theEnv)->DribbleFP = NULL;

   /*============================================*/
   /* Free the space used by the dribble buffer. */
   /*============================================*/

   if (FileCommandData(theEnv)->DribbleBuffer != NULL)
     {
      rm(theEnv,FileCommandData(theEnv)->DribbleBuffer,FileCommandData(theEnv)->DribbleMaximumPosition);
      FileCommandData(theEnv)->DribbleBuffer = NULL;
     }

   FileCommandData(theEnv)->DribbleCurrentPosition = 0;
   FileCommandData(theEnv)->DribbleMaximumPosition = 0;

   /*============================================*/
   /* Return TRUE if the dribble file was closed */
   /* without error, otherwise return FALSE.     */
   /*============================================*/

   return(rv);
  }

/*****************************************************/
/* SetDribbleStatusFunction: Sets the function which */
/*   is called whenever the dribble router is turned */
/*   on or off.                                      */
/*****************************************************/
globle void SetDribbleStatusFunction(
  void *theEnv,
  int (*fnptr)(void *,int))
  {
   FileCommandData(theEnv)->DribbleStatusFunction = fnptr;
  }
#endif

/*************************************************/
/* FindBatch: Find routine for the batch router. */
/*************************************************/
#if WIN_BTC
#pragma argsused
#endif
static int FindBatch(
  void *theEnv,
  char *logicalName)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif

   if (strcmp(logicalName,"stdin") == 0)
     { return(TRUE); }

   return(FALSE);
  }

/*************************************************/
/* GetcBatch: Getc routine for the batch router. */
/*************************************************/
static int GetcBatch(
  void *theEnv,
  char *logicalName)
  {
   return(LLGetcBatch(theEnv,logicalName,FALSE));
  }

/***************************************************/
/* LLGetcBatch: Lower level routine for retrieving */
/*   a character when a batch file is active.      */
/***************************************************/
globle int LLGetcBatch(
  void *theEnv,
  char *logicalName,
  int returnOnEOF)
  {
   int rv = EOF, flag = 1;

   /*=================================================*/
   /* Get a character until a valid character appears */
   /* or no more batch files are left.                */
   /*=================================================*/

   while ((rv == EOF) && (flag == 1))
     {
      if (FileCommandData(theEnv)->BatchType == FILE_BATCH)
        { rv = getc((FILE *) FileCommandData(theEnv)->BatchSource); } 
      else
        { rv = EnvGetcRouter(theEnv,(char *) FileCommandData(theEnv)->BatchSource); }

      if (rv == EOF)
        {
         if (FileCommandData(theEnv)->BatchCurrentPosition > 0) EnvPrintRouter(theEnv,"stdout",(char *) FileCommandData(theEnv)->BatchBuffer);
         flag = RemoveBatch(theEnv);
        }
     }

   /*=========================================================*/
   /* If the character retrieved is an end-of-file character, */
   /* then there are no batch files with character input      */
   /* remaining. Remove the batch router.                     */
   /*=========================================================*/

   if (rv == EOF)
     {
      if (FileCommandData(theEnv)->BatchCurrentPosition > 0) EnvPrintRouter(theEnv,"stdout",(char *) FileCommandData(theEnv)->BatchBuffer);
      EnvDeleteRouter(theEnv,"batch");
      RemoveBatch(theEnv);
      if (returnOnEOF == TRUE)
        { return (EOF); }
      else
        { return(EnvGetcRouter(theEnv,logicalName)); }
     }

   /*========================================*/
   /* Add the character to the batch buffer. */
   /*========================================*/

   FileCommandData(theEnv)->BatchBuffer = ExpandStringWithChar(theEnv,(char) rv,FileCommandData(theEnv)->BatchBuffer,&FileCommandData(theEnv)->BatchCurrentPosition,
                                      &FileCommandData(theEnv)->BatchMaximumPosition,FileCommandData(theEnv)->BatchMaximumPosition+BUFFER_SIZE);

   /*======================================*/
   /* If a carriage return is encountered, */
   /* then flush the batch buffer.         */
   /*======================================*/

   if ((char) rv == '\n')
     {
      EnvPrintRouter(theEnv,"stdout",(char *) FileCommandData(theEnv)->BatchBuffer);
      FileCommandData(theEnv)->BatchCurrentPosition = 0;
      if ((FileCommandData(theEnv)->BatchBuffer != NULL) && (FileCommandData(theEnv)->BatchMaximumPosition > BUFFER_SIZE))
        {
         rm(theEnv,FileCommandData(theEnv)->BatchBuffer,FileCommandData(theEnv)->BatchMaximumPosition);
         FileCommandData(theEnv)->BatchMaximumPosition = 0;
         FileCommandData(theEnv)->BatchBuffer = NULL;
        }
     }

   /*=====================================================*/
   /* Return the character retrieved from the batch file. */
   /*=====================================================*/

   return(rv);
  }

/*****************************************************/
/* UngetcBatch: Ungetc routine for the batch router. */
/*****************************************************/
#if WIN_BTC
#pragma argsused
#endif
static int UngetcBatch(
  void *theEnv,
  int ch,
  char *logicalName)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(logicalName)
#endif

   if (FileCommandData(theEnv)->BatchCurrentPosition > 0) FileCommandData(theEnv)->BatchCurrentPosition--;
   if (FileCommandData(theEnv)->BatchBuffer != NULL) FileCommandData(theEnv)->BatchBuffer[FileCommandData(theEnv)->BatchCurrentPosition] = EOS;
   if (FileCommandData(theEnv)->BatchType == FILE_BATCH)
     { return(ungetc(ch,(FILE *) FileCommandData(theEnv)->BatchSource)); }

   return(EnvUngetcRouter(theEnv,ch,(char *) FileCommandData(theEnv)->BatchSource));
  }

/*************************************************/
/* ExitBatch: Exit routine for the batch router. */
/*************************************************/
#if WIN_BTC
#pragma argsused
#endif
static int ExitBatch(
  void *theEnv,
  int num)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(num)
#endif
   CloseAllBatchSources(theEnv);
   return(1);
  }

/**************************************/
/* BatchCommand: H/L access routine   */
/*   for the batch command.           */
/**************************************/
globle int BatchCommand(
  void *theEnv)
  {
   char *fileName;

   if (EnvArgCountCheck(theEnv,"batch",EXACTLY,1) == -1) return(FALSE);
   if ((fileName = GetFileName(theEnv,"batch",1)) == NULL) return(FALSE);

   return(OpenBatch(theEnv,fileName,FALSE));
  }

/**************************************************/
/* Batch: C access routine for the batch command. */
/**************************************************/
globle int Batch(
  void *theEnv,
  char *fileName)
  { return(OpenBatch(theEnv,fileName,FALSE)); }

/***********************************************/
/* OpenBatch: Adds a file to the list of files */
/*   opened with the batch command.            */
/***********************************************/
globle int OpenBatch(
  void *theEnv,
  char *fileName,
  int placeAtEnd)
  {
   FILE *theFile;

   /*======================*/
   /* Open the batch file. */
   /*======================*/

   theFile = GenOpen(theEnv,fileName,"r");

   if (theFile == NULL)
     {
      OpenErrorMessage(theEnv,"batch",fileName);
      return(FALSE);
     }

   /*============================*/
   /* Create the batch router if */
   /* it doesn't already exist.  */
   /*============================*/

   if (FileCommandData(theEnv)->TopOfBatchList == NULL)
     {
      EnvAddRouter(theEnv,"batch", 20,
                 FindBatch, NULL,
                 GetcBatch, UngetcBatch,
                 ExitBatch);
     }

   /*====================================*/
   /* Add the newly opened batch file to */
   /* the list of batch files opened.    */
   /*====================================*/

   AddBatch(theEnv,placeAtEnd,(void *) theFile,FILE_BATCH,NULL);

   /*===================================*/
   /* Return TRUE to indicate the batch */
   /* file was successfully opened.     */
   /*===================================*/

   return(TRUE);
  }

/*****************************************************************/
/* OpenStringBatch: Opens a string source for batch processing.  */
/*   The memory allocated for the argument stringName must be    */
/*   deallocated by the user. The memory allocated for theString */
/*   will be deallocated by the batch routines when batch        */
/*   processing for the  string is completed.                    */
/*****************************************************************/
globle int OpenStringBatch(
  void *theEnv,
  char *stringName,
  char *theString,
  int placeAtEnd)
  {
   if (OpenStringSource(theEnv,stringName,theString,0) == 0)
     { return(0); }

   if (FileCommandData(theEnv)->TopOfBatchList == NULL)
     {
      EnvAddRouter(theEnv,"batch", 20,
                 FindBatch, NULL,
                 GetcBatch, UngetcBatch,
                 ExitBatch);
     }

   AddBatch(theEnv,placeAtEnd,(void *) stringName,STRING_BATCH,theString);

   return(1);
  }

/*******************************************************/
/* AddBatch: Creates the batch file data structure and */
/*   adds it to the list of opened batch files.        */
/*******************************************************/
static void AddBatch(
  void *theEnv,
  int placeAtEnd,
  void *theSource,
  int type,
  char *theString)
  {
   struct batchEntry *bptr;

   /*=========================*/
   /* Create the batch entry. */
   /*=========================*/

   bptr = get_struct(theEnv,batchEntry);
   bptr->batchType = type;
   bptr->inputSource = theSource;
   bptr->theString = theString;
   bptr->next = NULL;

   /*============================*/
   /* Add the entry to the list. */
   /*============================*/

   if (FileCommandData(theEnv)->TopOfBatchList == NULL)
     {
      FileCommandData(theEnv)->TopOfBatchList = bptr;
      FileCommandData(theEnv)->BottomOfBatchList = bptr;
      FileCommandData(theEnv)->BatchType = type;
      FileCommandData(theEnv)->BatchSource = theSource;
      FileCommandData(theEnv)->BatchCurrentPosition = 0;
     }
   else if (placeAtEnd == FALSE)
     {
      bptr->next = FileCommandData(theEnv)->TopOfBatchList;
      FileCommandData(theEnv)->TopOfBatchList = bptr;
      FileCommandData(theEnv)->BatchType = type;
      FileCommandData(theEnv)->BatchSource = theSource;
      FileCommandData(theEnv)->BatchCurrentPosition = 0;
     }
   else
     {
      FileCommandData(theEnv)->BottomOfBatchList->next = bptr;
      FileCommandData(theEnv)->BottomOfBatchList = bptr;
     }
  }

/******************************************************************/
/* RemoveBatch: Removes the top entry on the list of batch files. */
/******************************************************************/
globle int RemoveBatch(
  void *theEnv)
  {
   struct batchEntry *bptr;
   int rv;

   if (FileCommandData(theEnv)->TopOfBatchList == NULL) return(FALSE);

   /*==================================================*/
   /* Close the source from which batch input is read. */
   /*==================================================*/

   if (FileCommandData(theEnv)->TopOfBatchList->batchType == FILE_BATCH)
     { GenClose(theEnv,(FILE *) FileCommandData(theEnv)->TopOfBatchList->inputSource); }
   else
     {
      CloseStringSource(theEnv,(char *) FileCommandData(theEnv)->TopOfBatchList->inputSource);
      rm(theEnv,FileCommandData(theEnv)->TopOfBatchList->theString,strlen(FileCommandData(theEnv)->TopOfBatchList->theString) + 1);
     }

   /*=================================*/
   /* Remove the entry from the list. */
   /*=================================*/

   bptr = FileCommandData(theEnv)->TopOfBatchList;
   FileCommandData(theEnv)->TopOfBatchList = FileCommandData(theEnv)->TopOfBatchList->next;

   rtn_struct(theEnv,batchEntry,bptr);

   /*========================================================*/
   /* If there are no batch files remaining to be processed, */
   /* then free the space used by the batch buffer.          */
   /*========================================================*/

   if (FileCommandData(theEnv)->TopOfBatchList == NULL)
     {
      FileCommandData(theEnv)->BottomOfBatchList = NULL;
      FileCommandData(theEnv)->BatchSource = NULL;
      if (FileCommandData(theEnv)->BatchBuffer != NULL)
        {
         rm(theEnv,FileCommandData(theEnv)->BatchBuffer,FileCommandData(theEnv)->BatchMaximumPosition);
         FileCommandData(theEnv)->BatchBuffer = NULL;
        }
      FileCommandData(theEnv)->BatchCurrentPosition = 0;
      FileCommandData(theEnv)->BatchMaximumPosition = 0;
      rv = 0;
     }

   /*===========================================*/
   /* Otherwise move on to the next batch file. */
   /*===========================================*/

   else
     {
      FileCommandData(theEnv)->BatchType = FileCommandData(theEnv)->TopOfBatchList->batchType;
      FileCommandData(theEnv)->BatchSource = FileCommandData(theEnv)->TopOfBatchList->inputSource;
      FileCommandData(theEnv)->BatchCurrentPosition = 0;
      rv = 1;
     }

   /*====================================================*/
   /* Return TRUE if a batch file if there are remaining */
   /* batch files to be processed, otherwise FALSE.      */
   /*====================================================*/

   return(rv);
  }

/****************************************/
/* BatchActive: Returns TRUE if a batch */
/*   file is open, otherwise FALSE.     */
/****************************************/
globle intBool BatchActive(
  void *theEnv)
  {
   if (FileCommandData(theEnv)->TopOfBatchList != NULL) return(TRUE);

   return(FALSE);
  }

/******************************************************/
/* CloseAllBatchSources: Closes all open batch files. */
/******************************************************/
globle void CloseAllBatchSources(
  void *theEnv)
  {   
   /*================================================*/
   /* Free the batch buffer if it contains anything. */
   /*================================================*/

   if (FileCommandData(theEnv)->BatchBuffer != NULL)
     {
      if (FileCommandData(theEnv)->BatchCurrentPosition > 0) EnvPrintRouter(theEnv,"stdout",(char *) FileCommandData(theEnv)->BatchBuffer);
      rm(theEnv,FileCommandData(theEnv)->BatchBuffer,FileCommandData(theEnv)->BatchMaximumPosition);
      FileCommandData(theEnv)->BatchBuffer = NULL;
      FileCommandData(theEnv)->BatchCurrentPosition = 0;
      FileCommandData(theEnv)->BatchMaximumPosition = 0;
     }

   /*==========================*/
   /* Delete the batch router. */
   /*==========================*/

   EnvDeleteRouter(theEnv,"batch");

   /*=====================================*/
   /* Close each of the open batch files. */
   /*=====================================*/

   while (RemoveBatch(theEnv))
     { /* Do Nothing */ }
  }

/******************************************/
/* BatchStarCommand: H/L access routine   */
/*   for the batch* command.              */
/******************************************/
globle int BatchStarCommand(
  void *theEnv)
  {
   char *fileName;

   if (EnvArgCountCheck(theEnv,"batch*",EXACTLY,1) == -1) return(FALSE);
   if ((fileName = GetFileName(theEnv,"batch*",1)) == NULL) return(FALSE);

   return(EnvBatchStar(theEnv,fileName));
  }

#if ! RUN_TIME

/**********************************************************/
/* EnvBatchStar: C access routine for the batch* command. */
/**********************************************************/
globle int EnvBatchStar(
  void *theEnv,
  char *fileName)
  {
   int inchar;
   FILE *theFile;
   char *theString = NULL;
   size_t position = 0;
   size_t maxChars = 0;

   /*======================*/
   /* Open the batch file. */
   /*======================*/

   theFile = GenOpen(theEnv,fileName,"r");

   if (theFile == NULL)
     {
      OpenErrorMessage(theEnv,"batch",fileName);
      return(FALSE);
     }

   /*========================*/
   /* Reset the error state. */
   /*========================*/

   SetHaltExecution(theEnv,FALSE);
   SetEvaluationError(theEnv,FALSE);

   /*=============================================*/
   /* Evaluate commands from the file one by one. */
   /*=============================================*/

   while ((inchar = getc(theFile)) != EOF)
     {
      theString = ExpandStringWithChar(theEnv,inchar,theString,&position,
                                       &maxChars,maxChars+80);

      if (CompleteCommand(theString) != 0)
        {
         FlushPPBuffer(theEnv);
         SetPPBufferStatus(theEnv,OFF);
         RouteCommand(theEnv,theString,FALSE);
         FlushPPBuffer(theEnv);
         SetHaltExecution(theEnv,FALSE);
         SetEvaluationError(theEnv,FALSE);
         FlushBindList(theEnv);      
         genfree(theEnv,theString,(unsigned) maxChars);
         theString = NULL;
         maxChars = 0;
         position = 0;
        }      
     }

   if (theString != NULL)
     { genfree(theEnv,theString,(unsigned) maxChars); }
     
   /*=======================*/
   /* Close the batch file. */
   /*=======================*/

   GenClose(theEnv,theFile);
   return(TRUE);
  }

#else

/**************************************************/
/* EnvBatchStar: This is the non-functional stub  */
/*   provided for use with a run-time version.    */
/**************************************************/
globle int EnvBatchStar(
  void *theEnv,
  char *fileName)
  {
#if (MAC_MCW || WIN_MCW) && RUN_TIME
#pragma unused(fileName)
#endif

   PrintErrorID(theEnv,"FILECOM",1,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Function batch* does not work in run time modules.\n");
   return(FALSE);
  }

#endif

/***********************************************************/
/* LoadCommand: H/L access routine for the load command.   */
/***********************************************************/
globle int LoadCommand(
  void *theEnv)
  {
#if (! BLOAD_ONLY) && (! RUN_TIME)
   char *theFileName;
   int rv;

   if (EnvArgCountCheck(theEnv,"load",EXACTLY,1) == -1) return(FALSE);
   if ((theFileName = GetFileName(theEnv,"load",1)) == NULL) return(FALSE);

   SetPrintWhileLoading(theEnv,TRUE);

   if ((rv = EnvLoad(theEnv,theFileName)) == FALSE)
     {
      SetPrintWhileLoading(theEnv,FALSE);
      OpenErrorMessage(theEnv,"load",theFileName);
      return(FALSE);
     }

   SetPrintWhileLoading(theEnv,FALSE);
   if (rv == -1) return(FALSE);
   return(TRUE);
#else
   EnvPrintRouter(theEnv,WDIALOG,"Load is not available in this environment\n");
   return(FALSE);
#endif
  }

/****************************************************************/
/* LoadStarCommand: H/L access routine for the load* command.   */
/****************************************************************/
globle int LoadStarCommand(
  void *theEnv)
  {
#if (! BLOAD_ONLY) && (! RUN_TIME)
   char *theFileName;
   int rv;

   if (EnvArgCountCheck(theEnv,"load*",EXACTLY,1) == -1) return(FALSE);
   if ((theFileName = GetFileName(theEnv,"load*",1)) == NULL) return(FALSE);

   if ((rv = EnvLoad(theEnv,theFileName)) == FALSE)
     {
      OpenErrorMessage(theEnv,"load*",theFileName);
      return(FALSE);
     }

   if (rv == -1) return(FALSE);
   return(TRUE);
#else
   EnvPrintRouter(theEnv,WDIALOG,"Load* is not available in this environment\n");
   return(FALSE);
#endif
  }

#if DEBUGGING_FUNCTIONS
/***********************************************************/
/* SaveCommand: H/L access routine for the save command.   */
/***********************************************************/
globle int SaveCommand(
  void *theEnv)
  {
#if (! BLOAD_ONLY) && (! RUN_TIME)
   char *theFileName;

   if (EnvArgCountCheck(theEnv,"save",EXACTLY,1) == -1) return(FALSE);
   if ((theFileName = GetFileName(theEnv,"save",1)) == NULL) return(FALSE);

   if (EnvSave(theEnv,theFileName) == FALSE)
     {
      OpenErrorMessage(theEnv,"save",theFileName);
      return(FALSE);
     }

   return(TRUE);
#else
   EnvPrintRouter(theEnv,WDIALOG,"Save is not available in this environment\n");
   return(FALSE);
#endif
  }
#endif



