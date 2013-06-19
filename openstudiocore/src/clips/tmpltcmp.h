   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*      DEFTEMPLATE CONSTRUCT COMPILER HEADER FILE     */
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

#ifndef _H_tmpltcmp

#define _H_tmpltcmp

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _TMPLTCMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           DeftemplateCompilerSetup(void *);
   LOCALE void                           DeftemplateCModuleReference(void *,FILE *,int,int,int);
   LOCALE void                           DeftemplateCConstructReference(void *,FILE *,void *,int,int);

#endif
