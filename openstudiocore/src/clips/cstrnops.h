   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*           CONSTRAINT OPERATIONS HEADER FILE         */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides functions for performing operations on  */
/*   constraint records including computing the intersection */
/*   and union of constraint records.                        */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_cstrnops
#define _H_cstrnops

#if (! RUN_TIME)

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_constrnt
#include "constrnt.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _CSTRNOPS_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE struct constraintRecord       *IntersectConstraints(void *,struct constraintRecord *,struct constraintRecord *);
#if (! BLOAD_ONLY)
   LOCALE struct constraintRecord       *UnionConstraints(void *,struct constraintRecord *,struct constraintRecord *);
   LOCALE void                           RemoveConstantFromConstraint(void *,int,void *,CONSTRAINT_RECORD *);
#endif

#endif

#endif
