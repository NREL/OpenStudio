   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                 NETWORK HEADER FILE                 */
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
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*************************************************************/

#ifndef _H_network

#define _H_network

struct patternNodeHeader;
struct joinNode;
struct alphaMemoryHash;

#ifndef _H_match
#include "match.h"
#endif

#ifndef _H_expressn
#include "expressn.h"
#endif

struct patternNodeHeader
  {
   struct alphaMemoryHash *firstHash;
   struct alphaMemoryHash *lastHash;
   struct joinNode *entryJoin;
   struct expr *rightHash;
   unsigned int singlefieldNode : 1;
   unsigned int multifieldNode : 1;
   unsigned int stopNode : 1;
   unsigned int initialize : 1;
   unsigned int marked : 1;
   unsigned int beginSlot : 1;
   unsigned int endSlot : 1;
   unsigned int selector : 1;
  };

struct patternNodeHashEntry
  {
   void *parent;
   void *child;
   int type;
   void *value;
   struct patternNodeHashEntry *next;
  };

#define SIZE_PATTERN_HASH 16231

struct alphaMemoryHash
  {
   unsigned long bucket;
   struct patternNodeHeader *owner;
   struct partialMatch *alphaMemory;
   struct partialMatch *endOfQueue;
   struct alphaMemoryHash *nextHash;
   struct alphaMemoryHash *prevHash;
   struct alphaMemoryHash *next;
   struct alphaMemoryHash *prev;
  };

typedef struct alphaMemoryHash ALPHA_MEMORY_HASH;

#ifndef _H_ruledef
#include "ruledef.h"
#endif

#define INITIAL_BETA_HASH_SIZE 17

struct betaMemory
  {
   unsigned long size;
   unsigned long count;
   struct partialMatch **beta;
   struct partialMatch **last;
  };

struct joinLink
  {
   char enterDirection;
   struct joinNode *join;
   struct joinLink *next;
   long bsaveID;
  };
    
struct joinNode
  {
   unsigned int firstJoin : 1;
   unsigned int logicalJoin : 1;
   unsigned int joinFromTheRight : 1;
   unsigned int patternIsNegated : 1;
   unsigned int patternIsExists : 1;
   unsigned int initialize : 1;
   unsigned int marked : 1;
   unsigned int rhsType : 3;
   unsigned int depth : 16;
   long bsaveID;
   long long memoryAdds;
   long long memoryDeletes;
   long long memoryCompares;
   struct betaMemory *leftMemory;
   struct betaMemory *rightMemory;
   struct expr *networkTest;
   struct expr *secondaryNetworkTest;
   struct expr *leftHash;
   struct expr *rightHash;
   void *rightSideEntryStructure;
   struct joinLink *nextLinks;
   struct joinNode *lastLevel;
   struct joinNode *rightMatchNode;
   struct defrule *ruleToActivate;
  };

#endif




