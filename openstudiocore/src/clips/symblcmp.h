   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*        SYMBOL CONSTRUCT COMPILER HEADER FILE        */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the constructs-to-c feature for       */
/*   atomic data values: symbols, integers, floats, and      */
/*   bit maps.                                               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_symblcmp
#define _H_symblcmp

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

#ifndef _H_symbol
#include "symbol.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _SYMBLCMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                     PrintSymbolReference(void *,FILE *,SYMBOL_HN *);
   LOCALE void                     PrintFloatReference(void *,FILE *,FLOAT_HN *);
   LOCALE void                     PrintIntegerReference(void *,FILE *,INTEGER_HN *);
   LOCALE void                     PrintBitMapReference(void *,FILE *,BITMAP_HN *);
   LOCALE void                     AtomicValuesToCode(void *,char *,char *,char *);

#endif


