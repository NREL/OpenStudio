   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*                GENERATE HEADER FILE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for converting field           */
/*   constraints to expressions which can be used            */
/*   in the pattern and join networks.                       */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_generate

#define _H_generate

#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_reorder
#include "reorder.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _GENERATE_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           FieldConversion(void *,struct lhsParseNode *,struct lhsParseNode *);
   LOCALE struct expr                   *GetvarReplace(void *,struct lhsParseNode *,int);
   LOCALE intBool                        IsNandTest(struct lhsParseNode *);

#endif



