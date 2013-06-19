   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*            SORT FUNCTIONS HEADER MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for sorting functions.         */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_sortfun

#define _H_sortfun

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _SORTFUN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           SortFunctionDefinitions(void *);
   LOCALE void                           MergeSort(void *,unsigned long,DATA_OBJECT *,
                                                   int (*)(void *,DATA_OBJECT *,DATA_OBJECT *));
   LOCALE void                           SortFunction(void *,DATA_OBJECT *);

#endif



