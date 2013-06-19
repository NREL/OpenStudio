   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  07/01/05          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Gary D. Riley                                        */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Added fact-set queries.                        */
/*                                                           */
/*      6.24: Corrected errors when compiling as a C++ file. */
/*            DR0868                                         */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_factqury
#define _H_factqury

#if FACT_SET_QUERIES

#ifndef _H_factmngr
#include "factmngr.h"
#endif

typedef struct query_template
  {
   struct deftemplate *templatePtr;
   struct query_template *chain, *nxt;
  } QUERY_TEMPLATE;

typedef struct query_soln
  {
   struct fact **soln;
   struct query_soln *nxt;
  } QUERY_SOLN;

typedef struct query_core
  {
   struct fact **solns;
   EXPRESSION *query,*action;
   QUERY_SOLN *soln_set,*soln_bottom;
   unsigned soln_size,soln_cnt;
   DATA_OBJECT *result;
  } QUERY_CORE;

typedef struct query_stack
  {
   QUERY_CORE *core;
   struct query_stack *nxt;
  } QUERY_STACK;

#define FACT_QUERY_DATA 63

struct factQueryData
  { 
   SYMBOL_HN *QUERY_DELIMETER_SYMBOL;
   QUERY_CORE *QueryCore;
   QUERY_STACK *QueryCoreStack;
   int AbortQuery;
  };

#define FactQueryData(theEnv) ((struct factQueryData *) GetEnvironmentData(theEnv,FACT_QUERY_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FACTQURY_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define QUERY_DELIMETER_STRING     "(QDS)"

LOCALE void SetupFactQuery(void *);
LOCALE void GetQueryFact(void *,DATA_OBJECT *);
LOCALE void GetQueryFactSlot(void *,DATA_OBJECT *);
LOCALE intBool AnyFacts(void *);
LOCALE void QueryFindFact(void *,DATA_OBJECT *);
LOCALE void QueryFindAllFacts(void *,DATA_OBJECT *);
LOCALE void QueryDoForFact(void *,DATA_OBJECT *);
LOCALE void QueryDoForAllFacts(void *,DATA_OBJECT *);
LOCALE void DelayedQueryDoForAllFacts(void *,DATA_OBJECT *);

#endif

#endif





