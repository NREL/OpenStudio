   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*                 SCANNER HEADER FILE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Routines for scanning lexical tokens from an     */
/*   input source.                                           */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_scanner
#define _H_scanner

struct token;

#ifndef _H_pprint
#include "pprint.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _SCANNER_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

struct token
  {
   unsigned short type;
   void *value;
   char *printForm;
  };

#define SCANNER_DATA 57

struct scannerData
  { 
   char *GlobalString;
   size_t GlobalMax;
   size_t GlobalPos;
   long LineCount;
   int IgnoreCompletionErrors;
  };

#define ScannerData(theEnv) ((struct scannerData *) GetEnvironmentData(theEnv,SCANNER_DATA))

   LOCALE void                           InitializeScannerData(void *);
   LOCALE void                           GetToken(void *,char *,struct token *);
   LOCALE void                           CopyToken(struct token *,struct token *);
   LOCALE void                           ResetLineCount(void *);
   LOCALE long                           GetLineCount(void *);
   LOCALE void                           IncrementLineCount(void *);
   LOCALE void                           DecrementLineCount(void *);

#endif




