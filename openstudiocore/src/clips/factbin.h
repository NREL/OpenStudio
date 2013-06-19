   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*            FACT BLOAD/BSAVE HEADER FILE             */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_factbin

#define _H_factbin

#include "factbld.h"

#define FACTBIN_DATA 62

struct factBinaryData
  { 
   struct factPatternNode *FactPatternArray;
   long NumberOfPatterns;
  };
  
#define FactBinaryData(theEnv) ((struct factBinaryData *) GetEnvironmentData(theEnv,FACTBIN_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FACTBIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           FactBinarySetup(void *);

#define BsaveFactPatternIndex(patPtr) ((patPtr == NULL) ? -1L : ((struct factPatternNode *) patPtr)->bsaveID)
#define BloadFactPatternPointer(i) ((struct factPatternNode *) ((i == -1L) ? NULL : &FactBinaryData(theEnv)->FactPatternArray[i]))

#endif

