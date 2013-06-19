   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.23  01/31/05          */
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
/*************************************************************/

#ifndef _H_factqpsr
#define _H_factqpsr

#if FACT_SET_QUERIES && (! RUN_TIME)

#ifndef _H_expressn
#include "expressn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FACTQPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE EXPRESSION *FactParseQueryNoAction(void *,EXPRESSION *,char *);
LOCALE EXPRESSION *FactParseQueryAction(void *,EXPRESSION *,char *);

#ifndef _FACTQPSR_SOURCE_
#endif

#endif

#endif



