   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*                FACT BUILD HEADER FILE               */
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

#ifndef _H_factbld

#define _H_factbld

#ifndef _H_pattern
#include "pattern.h"
#endif
#ifndef _H_network
#include "network.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

struct factPatternNode
  {
   struct patternNodeHeader header;
   long bsaveID;
   unsigned short whichField;
   unsigned short whichSlot;
   unsigned short leaveFields;
   struct expr *networkTest;
   struct factPatternNode *nextLevel;
   struct factPatternNode *lastLevel;
   struct factPatternNode *leftNode;
   struct factPatternNode *rightNode;
  };

#ifdef _FACTBUILD_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           InitializeFactPatterns(void *);
   LOCALE void                           DestroyFactPatternNetwork(void *,
                                                                   struct factPatternNode *);

#endif
