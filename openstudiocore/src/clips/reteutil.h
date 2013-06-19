   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*              RETE UTILITY HEADER FILE               */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides a set of utility functions useful to    */
/*   other modules.                                          */
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
/*************************************************************/

#ifndef _H_reteutil
#define _H_reteutil

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_match
#include "match.h"
#endif
#ifndef _H_network
#include "network.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _RETEUTIL_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           PrintPartialMatch(void *,char *,struct partialMatch *);
   LOCALE struct partialMatch           *CopyPartialMatch(void *,struct partialMatch *);
   LOCALE struct partialMatch           *MergePartialMatches(void *,struct partialMatch *,struct partialMatch *);
   LOCALE long int                       IncrementPseudoFactIndex(void);
   LOCALE struct partialMatch           *GetAlphaMemory(void *,struct patternNodeHeader *,unsigned long);
   LOCALE struct partialMatch           *GetLeftBetaMemory(struct joinNode *,unsigned long);
   LOCALE struct partialMatch           *GetRightBetaMemory(struct joinNode *,unsigned long);
   LOCALE void                           ReturnLeftMemory(void *,struct joinNode *);
   LOCALE void                           ReturnRightMemory(void *,struct joinNode *);
   LOCALE void                           DestroyBetaMemory(void *,struct joinNode *,int);
   LOCALE void                           FlushBetaMemory(void *,struct joinNode *,int);
   LOCALE intBool                        BetaMemoryNotEmpty(struct joinNode *);
   LOCALE void                           RemoveAlphaMemoryMatches(void *,struct patternNodeHeader *,struct partialMatch *,
                                                                  struct alphaMatch *); 
   LOCALE void                           DestroyAlphaMemory(void *,struct patternNodeHeader *,int);
   LOCALE void                           FlushAlphaMemory(void *,struct patternNodeHeader *);
   LOCALE void                           FlushAlphaBetaMemory(void *,struct partialMatch *);
   LOCALE void                           DestroyAlphaBetaMemory(void *,struct partialMatch *);
   LOCALE int                            GetPatternNumberFromJoin(struct joinNode *);
   LOCALE struct multifieldMarker       *CopyMultifieldMarkers(void *,struct multifieldMarker *);
   LOCALE struct partialMatch           *CreateAlphaMatch(void *,void *,struct multifieldMarker *,
                                                          struct patternNodeHeader *,unsigned long);
   LOCALE void                           TraceErrorToRule(void *,struct joinNode *,char *);
   LOCALE void                           InitializePatternHeader(void *,struct patternNodeHeader *);
   LOCALE void                           MarkRuleNetwork(void *,int);
   LOCALE void                           TagRuleNetwork(void *,long *,long *,long *,long *);
   LOCALE int                            FindEntityInPartialMatch(struct patternEntity *,struct partialMatch *);
   LOCALE unsigned long                  ComputeRightHashValue(void *,struct patternNodeHeader *);
   LOCALE void                           UpdateBetaPMLinks(void *,struct partialMatch *,struct partialMatch *,struct partialMatch *,
                                                       struct joinNode *,unsigned long,int);
   LOCALE void                           UnlinkBetaPMFromNodeAndLineage(void *,struct joinNode *,struct partialMatch *,int);
   LOCALE void                           UnlinkNonLeftLineage(void *,struct joinNode *,struct partialMatch *,int);
   LOCALE struct partialMatch           *CreateEmptyPartialMatch(void *);
   LOCALE void                           MarkRuleJoins(struct joinNode *,int);
   LOCALE void                           AddBlockedLink(struct partialMatch *,struct partialMatch *);
   LOCALE void                           RemoveBlockedLink(struct partialMatch *);
#endif



