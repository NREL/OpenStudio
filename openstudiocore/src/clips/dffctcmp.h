   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*        DEFFACTS CONSTRUCT COMPILER HEADER FILE      */
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
/*************************************************************/

#ifndef _H_dffctcmp

#define _H_dffctcmp

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _DFFCTCMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           DeffactsCompilerSetup(void *);
   LOCALE void                           DeffactsCModuleReference(void *,FILE *,int,int,int);

#endif
