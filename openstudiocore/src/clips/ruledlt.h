   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*          RULE DELETION MODULE HEADER FILE           */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for deleting a rule including  */
/*   freeing the defrule data structures and removing the    */
/*   appropriate joins from the join network.                */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_ruledlt

#define _H_ruledlt

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _RULEDLT_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           ReturnDefrule(void *,void *);
   LOCALE void                           DestroyDefrule(void *,void *);

#endif






