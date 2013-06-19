   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*                 REORDER HEADER FILE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines necessary for converting the   */
/*   the LHS of a rule into an appropriate form suitable for */
/*   the KB Rete topology. This includes transforming the    */
/*   LHS so there is at most one "or" CE (and this is the    */
/*   first CE of the LHS if it is used), adding initial      */
/*   patterns to the LHS (if no LHS is specified or a "test" */
/*   or "not" CE is the first pattern within an "and" CE),   */
/*   removing redundant CEs, and determining appropriate     */
/*   information on nesting for implementing joins from the  */
/*   right.                                                  */
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

#ifndef _H_reorder
#define _H_reorder

struct lhsParseNode;

#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_ruledef
#include "ruledef.h"
#endif
#ifndef _H_pattern
#include "pattern.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _REORDER_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

/***********************************************************************/
/* lhsParseNode structure: Stores information about the intermediate   */
/*   parsed representation of the lhs of a rule.                       */
/***********************************************************************/
struct lhsParseNode
  {
   unsigned short type;
   void *value;
   unsigned int negated : 1;
   unsigned int exists : 1;
   unsigned int existsNand : 1;
   unsigned int logical : 1;
   unsigned int multifieldSlot : 1;
   unsigned int bindingVariable : 1;
   unsigned int derivedConstraints : 1;
   unsigned int userCE : 1;
   unsigned int whichCE : 7;
   unsigned int marked : 1;
   unsigned int withinMultifieldSlot : 1;
   unsigned short multiFieldsBefore;
   unsigned short multiFieldsAfter;
   unsigned short singleFieldsBefore;
   unsigned short singleFieldsAfter;
   struct constraintRecord *constraints;
   struct lhsParseNode *referringNode;
   struct patternParser *patternType;
   short pattern;
   short index;
   struct symbolHashNode *slot;
   short slotNumber;
   int beginNandDepth;
   int endNandDepth;
   int joinDepth;
   struct expr *networkTest;
   struct expr *externalNetworkTest;
   struct expr *secondaryNetworkTest;
   struct expr *secondaryExternalNetworkTest;
   struct expr *externalLeftHash;
   struct expr *externalRightHash;
   struct expr *constantSelector;
   struct expr *constantValue;
   struct expr *leftHash;
   struct expr *rightHash;
   struct expr *betaHash;
   struct lhsParseNode *expression;
   void *userData;
   struct lhsParseNode *right;
   struct lhsParseNode *bottom;
  };

LOCALE struct lhsParseNode           *ReorderPatterns(void *,struct lhsParseNode *,int *);
LOCALE struct lhsParseNode           *CopyLHSParseNodes(void *,struct lhsParseNode *);
LOCALE void                           CopyLHSParseNode(void *,struct lhsParseNode *,struct lhsParseNode *,int);
LOCALE struct lhsParseNode           *GetLHSParseNode(void *);
LOCALE void                           ReturnLHSParseNodes(void *,struct lhsParseNode *);
LOCALE struct lhsParseNode           *ExpressionToLHSParseNodes(void *,struct expr *);
LOCALE struct expr                   *LHSParseNodesToExpression(void *,struct lhsParseNode *);
LOCALE void                           AddInitialPatterns(void *,struct lhsParseNode *);
LOCALE int                            IsExistsSubjoin(struct lhsParseNode *,int);

#endif





