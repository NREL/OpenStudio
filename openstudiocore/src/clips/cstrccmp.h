   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.20  01/31/02          */
   /*                                                     */
   /*           CONSTRUCT CONSTRUCTS-TO-C HEADER          */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*      Support functions for the constructs-to-c of         */
/*      construct headers and related items.                 */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_cstrccmp
#define _H_cstrccmp

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _CSTRCCMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

#if ANSI_COMPILER  
   LOCALE void                           MarkConstructHeaders(int);
#else
   LOCALE void                           MarkConstructHeaders();
#endif 

#endif




