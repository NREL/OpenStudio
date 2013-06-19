   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*               PRETTY PRINT HEADER FILE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Routines for processing the pretty print         */
/*   representation of constructs.                           */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_pprint
#define _H_pprint

#define PRETTY_PRINT_DATA 52

struct prettyPrintData
  { 
   int PPBufferStatus;
   int PPBufferEnabled;
   int IndentationDepth;
   size_t PPBufferPos;
   size_t PPBufferMax;
   size_t PPBackupOnce;
   size_t PPBackupTwice;
   char *PrettyPrintBuffer;
  };

#define PrettyPrintData(theEnv) ((struct prettyPrintData *) GetEnvironmentData(theEnv,PRETTY_PRINT_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _PPRINT_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           InitializePrettyPrintData(void *);
   LOCALE void                           FlushPPBuffer(void *);
   LOCALE void                           DestroyPPBuffer(void *);
   LOCALE void                           SavePPBuffer(void *,char *);
   LOCALE void                           PPBackup(void *);
   LOCALE char                          *CopyPPBuffer(void *);
   LOCALE char                          *GetPPBuffer(void *);
   LOCALE void                           PPCRAndIndent(void *);
   LOCALE void                           IncrementIndentDepth(void *,int);
   LOCALE void                           DecrementIndentDepth(void *,int);
   LOCALE void                           SetIndentDepth(void *,int);
   LOCALE void                           SetPPBufferStatus(void *,int);
   LOCALE int                            GetPPBufferStatus(void *);
   LOCALE int                            SetPPBufferEnabled(void *,int);
   LOCALE int                            GetPPBufferEnabled(void *);

#endif



