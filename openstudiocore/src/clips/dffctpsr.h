   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*             DEFFACTS PARSER HEADER FILE             */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_dffctpsr
#define _H_dffctpsr

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _DFFCTPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE int                            ParseDeffacts(void *,char *);

#endif

