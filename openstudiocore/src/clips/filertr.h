   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*             FILE I/O ROUTER HEADER FILE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: I/O Router routines which allow files to be used */
/*   as input and output sources.                            */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_filertr
#define _H_filertr

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

#define FILE_ROUTER_DATA 47
   
struct fileRouter
  {
   char *logicalName;
   FILE *stream;
   struct fileRouter *next;
  };

struct fileRouterData
  { 
   struct fileRouter *ListOfFileRouters;
  };

#define FileRouterData(theEnv) ((struct fileRouterData *) GetEnvironmentData(theEnv,FILE_ROUTER_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FILERTR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           InitializeFileRouter(void *);
   LOCALE FILE                          *FindFptr(void *,char *);
   LOCALE int                            OpenAFile(void *,char *,char *,char *);
   LOCALE int                            CloseAllFiles(void *);
   LOCALE int                            CloseFile(void *,char *);
   LOCALE int                            FindFile(void *,char *);

#endif






