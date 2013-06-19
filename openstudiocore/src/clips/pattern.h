   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  05/17/06            */
   /*                                                     */
   /*                PATTERN HEADER FILE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides the mechanism for recognizing and       */
/*   parsing the various types of patterns that can be used  */
/*   in the LHS of a rule. In version 6.0, the only pattern  */
/*   types provided are for deftemplate and instance         */
/*   patterns.                                               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Removed LOGICAL_DEPENDENCIES compilation flag. */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_pattern

#define _H_pattern

#ifndef _STDIO_INCLUDED_
#include <stdio.h>
#define _STDIO_INCLUDED_
#endif

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

struct patternEntityRecord
  {
   struct entityRecord base;
   void (*decrementBasisCount)(void *,void *);
   void (*incrementBasisCount)(void *,void *);
   void (*matchFunction)(void *,void *);
   intBool (*synchronized)(void *,void *);
  };

typedef struct patternEntityRecord PTRN_ENTITY_RECORD;
typedef struct patternEntityRecord *PTRN_ENTITY_RECORD_PTR;

struct patternEntity
  {
   struct patternEntityRecord *theInfo;
   void *dependents;
   unsigned busyCount;
   unsigned long long timeTag;
  };

typedef struct patternEntity PATTERN_ENTITY;
typedef struct patternEntity * PATTERN_ENTITY_PTR;

struct patternParser;

#ifndef _H_symbol
#include "symbol.h"
#endif
#ifndef _H_scanner
#include "scanner.h"
#endif
#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_match
#include "match.h"
#endif
#ifndef _H_reorder
#include "reorder.h"
#endif
#ifndef _H_constrnt
#include "constrnt.h"
#endif

#define MAXIMUM_NUMBER_OF_PATTERNS 128

struct patternParser
  {
   char *name;
   struct patternEntityRecord *entityType;
   int positionInArray;
   int (*recognizeFunction)(SYMBOL_HN *);
   struct lhsParseNode *(*parseFunction)(void *,char *,struct token *);
   int (*postAnalysisFunction)(void *,struct lhsParseNode *);
   struct patternNodeHeader *(*addPatternFunction)(void *,struct lhsParseNode *);
   void (*removePatternFunction)(void *,struct patternNodeHeader *);
   struct expr *(*genJNConstantFunction)(void *,struct lhsParseNode *,int);
   void (*replaceGetJNValueFunction)(void *,struct expr *,struct lhsParseNode *,int);
   struct expr *(*genGetJNValueFunction)(void *,struct lhsParseNode *,int);
   struct expr *(*genCompareJNValuesFunction)(void *,struct lhsParseNode *,struct lhsParseNode *,int);
   struct expr *(*genPNConstantFunction)(void *,struct lhsParseNode *);
   void (*replaceGetPNValueFunction)(void *,struct expr *,struct lhsParseNode *);
   struct expr *(*genGetPNValueFunction)(void *,struct lhsParseNode *);
   struct expr *(*genComparePNValuesFunction)(void *,struct lhsParseNode *,struct lhsParseNode *);
   void (*returnUserDataFunction)(void *,void *);
   void *(*copyUserDataFunction)(void *,void *);
   void (*markIRPatternFunction)(void *,struct patternNodeHeader *,int);
   void (*incrementalResetFunction)(void *);
   struct lhsParseNode *(*initialPatternFunction)(void *);
   void (*codeReferenceFunction)(void *,void *,FILE *,int,int);
   int priority;
   struct patternParser *next;
  };

struct reservedSymbol
  {
   char *theSymbol;
   char *reservedBy;
   struct reservedSymbol *next;
  };

#define MAX_POSITIONS 8

#define PATTERN_DATA 19

struct patternData
  { 
   struct patternParser *ListOfPatternParsers;
   struct patternParser *PatternParserArray[MAX_POSITIONS];
   int NextPosition;
   struct reservedSymbol *ListOfReservedPatternSymbols;
   int WithinNotCE;
   int  GlobalSalience;
   int GlobalAutoFocus;
   struct expr *SalienceExpression;
   struct patternNodeHashEntry **PatternHashTable;
   unsigned long PatternHashTableSize;
  };

#define PatternData(theEnv) ((struct patternData *) GetEnvironmentData(theEnv,PATTERN_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _PATTERN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           InitializePatterns(void *);
   LOCALE int                            AddPatternParser(void *,struct patternParser *);
   LOCALE struct patternParser          *FindPatternParser(void *,char *);
   LOCALE void                           DetachPattern(void *,int,struct patternNodeHeader *);
   LOCALE void                           GetNextPatternEntity(void *,
                                                              struct patternParser **,
                                                              struct patternEntity **);
   LOCALE struct patternParser          *GetPatternParser(void *,int);
   LOCALE struct lhsParseNode           *RestrictionParse(void *,char *,struct token *,int,
                                                       struct symbolHashNode *,short,
                                                       struct constraintRecord *,short);
   LOCALE int                            PostPatternAnalysis(void *,struct lhsParseNode *);
   LOCALE void                           PatternNodeHeaderToCode(void *,FILE *,struct patternNodeHeader *,int,int);
   LOCALE void                           AddReservedPatternSymbol(void *,char *,char *);
   LOCALE intBool                        ReservedPatternSymbol(void *,char *,char *);
   LOCALE void                           ReservedPatternSymbolErrorMsg(void *,char *,char *);
   LOCALE void                           AddHashedPatternNode(void *,void *,void *,unsigned short,void *);
   LOCALE intBool                        RemoveHashedPatternNode(void *,void *,void *,unsigned short,void *);
   LOCALE void                          *FindHashedPatternNode(void *,void *,unsigned short,void *);

#endif









