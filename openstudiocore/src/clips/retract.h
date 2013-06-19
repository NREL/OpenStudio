   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                RETRACT HEADER FILE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose:  Handles join network activity associated with   */
/*   with the removal of a data entity such as a fact or     */
/*   instance.                                               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Rule with exists CE has incorrect activation.  */
/*            DR0867                                         */
/*                                                           */
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*            Added additional developer statistics to help  */
/*            analyze join network performance.              */
/*                                                           */
/*************************************************************/

#ifndef _H_retract
#define _H_retract

#ifndef _H_match
#include "match.h"
#endif
#ifndef _H_network
#include "network.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _RETRACT_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

struct rdriveinfo
  {
   struct partialMatch *link;
   struct joinNode *jlist;
   struct rdriveinfo *next;
  };

LOCALE void                           NetworkRetract(void *,struct patternMatch *);
LOCALE void                           ReturnPartialMatch(void *,struct partialMatch *);
LOCALE void                           DestroyPartialMatch(void *,struct partialMatch *);
LOCALE void                           FlushGarbagePartialMatches(void *);
LOCALE void                           DeletePartialMatches(void *,struct partialMatch *);
LOCALE void                           PosEntryRetractBeta(void *,struct partialMatch *,struct partialMatch *);
LOCALE void                           PosEntryRetractAlpha(void *,struct partialMatch *);

#endif



