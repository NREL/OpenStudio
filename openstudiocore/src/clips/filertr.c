   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*               FILE I/O ROUTER MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: I/O Router routines which allow files to be used */
/*   as input and output sources.                            */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Added environment parameter to GenClose.       */
/*            Added environment parameter to GenOpen.        */
/*                                                           */
/*            Added pragmas to remove compilation warnings.  */
/*                                                           */
/*************************************************************/

#define _FILERTR_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <string.h>

#include "setup.h"

#include "constant.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "router.h"
#include "sysdep.h"

#include "filertr.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static int                     ExitFile(void *,int);
   static int                     PrintFile(void *,char *,char *);
   static int                     GetcFile(void *,char *);
   static int                     UngetcFile(void *,int,char *);
   static void                    DeallocateFileRouterData(void *);

/***************************************************************/
/* InitializeFileRouter: Initializes file input/output router. */
/***************************************************************/
globle void InitializeFileRouter(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,FILE_ROUTER_DATA,sizeof(struct fileRouterData),DeallocateFileRouterData);

   EnvAddRouter(theEnv,"fileio",0,FindFile,
             PrintFile,GetcFile,
             UngetcFile,ExitFile);
  }

/*****************************************/
/* DeallocateFileRouterData: Deallocates */
/*    environment data for file routers. */
/*****************************************/
static void DeallocateFileRouterData(
  void *theEnv)
  {
   struct fileRouter *tmpPtr, *nextPtr;

   tmpPtr = FileRouterData(theEnv)->ListOfFileRouters;
   while (tmpPtr != NULL)
     {
      nextPtr = tmpPtr->next;
      GenClose(theEnv,tmpPtr->stream);
      rm(theEnv,tmpPtr->logicalName,strlen(tmpPtr->logicalName) + 1);
      rtn_struct(theEnv,fileRouter,tmpPtr);
      tmpPtr = nextPtr;
     }
  }

/*****************************************/
/* FindFptr: Returns a pointer to a file */
/*   stream for a given logical name.    */
/*****************************************/
globle FILE *FindFptr(
  void *theEnv,
  char *logicalName)
  {
   struct fileRouter *fptr;

   /*========================================================*/
   /* Check to see if standard input or output is requested. */
   /*========================================================*/

   if (strcmp(logicalName,"stdout") == 0)
     { return(stdout); }
   else if (strcmp(logicalName,"stdin") == 0)
     { return(stdin);  }
   else if (strcmp(logicalName,WTRACE) == 0)
     { return(stdout); }
   else if (strcmp(logicalName,WDIALOG) == 0)
     { return(stdout); }
   else if (strcmp(logicalName,WPROMPT) == 0)
     { return(stdout); }
   else if (strcmp(logicalName,WDISPLAY) == 0)
     { return(stdout); }
   else if (strcmp(logicalName,WERROR) == 0)
     { return(stdout); }
   else if (strcmp(logicalName,WWARNING) == 0)
     { return(stdout); }

   /*==============================================================*/
   /* Otherwise, look up the logical name on the global file list. */
   /*==============================================================*/

   fptr = FileRouterData(theEnv)->ListOfFileRouters;
   while ((fptr != NULL) ? (strcmp(logicalName,fptr->logicalName) != 0) : FALSE)
     { fptr = fptr->next; }

   if (fptr != NULL) return(fptr->stream);

   return(NULL);
  }

/*****************************************************/
/* FindFile: Find routine for file router logical    */
/*   names. Returns TRUE if the specified logical    */
/*   name has an associated file stream (which means */
/*   that the logical name can be handled by the     */
/*   file router). Otherwise, FALSE is returned.     */
/*****************************************************/
globle int FindFile(
  void *theEnv,
  char *logicalName)
  {
   if (FindFptr(theEnv,logicalName) != NULL) return(TRUE);

   return(FALSE);
  }

/********************************************/
/* ExitFile:  Exit routine for file router. */
/********************************************/
#if WIN_BTC
#pragma argsused
#endif
static int ExitFile(
  void *theEnv,
  int num)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(num)
#endif
#if IO_FUNCTIONS
   CloseAllFiles(theEnv);
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
#endif
   return(1);
  }

/*********************************************/
/* PrintFile: Print routine for file router. */
/*********************************************/
static int PrintFile(
  void *theEnv,
  char *logicalName,
  char *str)
  {
   FILE *fptr;

   fptr = FindFptr(theEnv,logicalName);
   
   genprintfile(theEnv,fptr,str);
   
   return(1);
  }

/*******************************************/
/* GetcFile: Getc routine for file router. */
/*******************************************/
static int GetcFile(
  void *theEnv,
  char *logicalName)
  {
   FILE *fptr;
   int theChar;

   fptr = FindFptr(theEnv,logicalName);

   if (fptr == stdin)
     { theChar = gengetchar(theEnv); }
   else
     { theChar = getc(fptr); }

   /*=================================================*/
   /* The following code prevents Control-D on UNIX   */
   /* machines from terminating all input from stdin. */
   /*=================================================*/

   if ((fptr == stdin) && (theChar == EOF)) clearerr(stdin);

   return(theChar);
  }

/***********************************************/
/* UngetcFile: Ungetc routine for file router. */
/***********************************************/
static int UngetcFile(
  void *theEnv,
  int ch,
  char *logicalName)
  {
   FILE *fptr;

   fptr = FindFptr(theEnv,logicalName);
   
   if (fptr == stdin)
     { return(genungetchar(theEnv,ch)); }
   else
     { return(ungetc(ch,fptr)); }
  }

/*********************************************************/
/* OpenFile: Opens a file with the specified access mode */
/*   and stores the opened stream on the list of files   */
/*   associated with logical names Returns TRUE if the   */
/*   file was succesfully opened, otherwise FALSE.       */
/*********************************************************/
globle int OpenAFile(
  void *theEnv,
  char *fileName,
  char *accessMode,
  char *logicalName)
  {
   FILE *newstream;
   struct fileRouter *newRouter;

   /*==================================*/
   /* Make sure the file can be opened */
   /* with the specified access mode.  */
   /*==================================*/

   if ((newstream = GenOpen(theEnv,fileName,accessMode)) == NULL)
     { return(FALSE); }

   /*===========================*/
   /* Create a new file router. */
   /*===========================*/

   newRouter = get_struct(theEnv,fileRouter);
   newRouter->logicalName = (char *) gm2(theEnv,strlen(logicalName) + 1);
   genstrcpy(newRouter->logicalName,logicalName);
   newRouter->stream = newstream;

   /*==========================================*/
   /* Add the newly opened file to the list of */
   /* files associated with logical names.     */
   /*==========================================*/

   newRouter->next = FileRouterData(theEnv)->ListOfFileRouters;
   FileRouterData(theEnv)->ListOfFileRouters = newRouter;

   /*==================================*/
   /* Return TRUE to indicate the file */
   /* was opened successfully.         */
   /*==================================*/

   return(TRUE);
  }

/*************************************************************/
/* CloseFile: Closes the file associated with the specified  */
/*   logical name. Returns TRUE if the file was successfully */
/*   closed, otherwise FALSE.                                */
/*************************************************************/
globle int CloseFile(
  void *theEnv,
  char *fid)
  {
   struct fileRouter *fptr, *prev;

   for (fptr = FileRouterData(theEnv)->ListOfFileRouters, prev = NULL;
        fptr != NULL;
        fptr = fptr->next)
     {
      if (strcmp(fptr->logicalName,fid) == 0)
        {
         GenClose(theEnv,fptr->stream);
         rm(theEnv,fptr->logicalName,strlen(fptr->logicalName) + 1);
         if (prev == NULL)
           { FileRouterData(theEnv)->ListOfFileRouters = fptr->next; }
         else
           { prev->next = fptr->next; }
         rm(theEnv,fptr,(int) sizeof(struct fileRouter));

         return(TRUE);
        }

      prev = fptr;
     }

   return(FALSE);
  }

/**********************************************/
/* CloseAllFiles: Closes all files associated */
/*   with a file I/O router. Returns TRUE if  */
/*   any file was closed, otherwise FALSE.    */
/**********************************************/
globle int CloseAllFiles(
  void *theEnv)
  {
   struct fileRouter *fptr, *prev;

   if (FileRouterData(theEnv)->ListOfFileRouters == NULL) return(FALSE);

   fptr = FileRouterData(theEnv)->ListOfFileRouters;

   while (fptr != NULL)
     {
      GenClose(theEnv,fptr->stream);
      prev = fptr;
      rm(theEnv,fptr->logicalName,strlen(fptr->logicalName) + 1);
      fptr = fptr->next;
      rm(theEnv,prev,(int) sizeof(struct fileRouter));
     }

   FileRouterData(theEnv)->ListOfFileRouters = NULL;

   return(TRUE);
  }



