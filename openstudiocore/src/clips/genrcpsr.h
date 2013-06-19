   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
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
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_genrcpsr
#define _H_genrcpsr

#if DEFGENERIC_CONSTRUCT && (! BLOAD_ONLY) && (! RUN_TIME)

#include "genrcfun.h"

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _GENRCPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE intBool ParseDefgeneric(void *,char *);
LOCALE intBool ParseDefmethod(void *,char *);
LOCALE DEFMETHOD *AddMethod(void *,DEFGENERIC *,DEFMETHOD *,int,short,EXPRESSION *,
                            int,int,SYMBOL_HN *,EXPRESSION *,char *,int);
LOCALE void PackRestrictionTypes(void *,RESTRICTION *,EXPRESSION *);
LOCALE void DeleteTempRestricts(void *,EXPRESSION *);
LOCALE DEFMETHOD *FindMethodByRestrictions(DEFGENERIC *,EXPRESSION *,int,
                                           SYMBOL_HN *,int *);

#ifndef _GENRCPSR_SOURCE_
#endif

#endif

#endif



