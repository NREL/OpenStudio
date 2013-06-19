   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*             TEXT PROCESSING HEADER FILE             */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Added get-region function.                     */
/*                                                           */
/*************************************************************/

#ifndef _H_textpro

#define _H_textpro

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _TEXTPRO_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#if TEXTPRO_FUNCTIONS
   LOCALE void                           FetchCommand(void *,DATA_OBJECT *);
   LOCALE int                            PrintRegionCommand(void *);
   LOCALE void                          *GetRegionCommand(void *);
   int                                   TossCommand(void *);
#endif

#if HELP_FUNCTIONS
   LOCALE void                           HelpFunction(void *);
   LOCALE void                           HelpPathFunction(void *);
#endif

   LOCALE void                           HelpFunctionDefinitions(void *);
#endif





