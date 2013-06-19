   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*            CONSTRAINT UTILITY HEADER FILE           */
   /*******************************************************/

/*************************************************************/
/* Purpose: Utility routines for manipulating, initializing, */
/*   creating, copying, and comparing constraint records.    */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian Dantes                                         */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_cstrnutl
#define _H_cstrnutl

#ifndef _H_constrnt
#include "constrnt.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _CSTRNUTL_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

   LOCALE struct constraintRecord       *GetConstraintRecord(void *);
   LOCALE int                            CompareNumbers(void *,int,void *,int,void *);
   LOCALE struct constraintRecord       *CopyConstraintRecord(void *,CONSTRAINT_RECORD *);
   LOCALE int                            SetConstraintType(int,CONSTRAINT_RECORD *);
   LOCALE void                           SetAnyAllowedFlags(CONSTRAINT_RECORD *,int);
   LOCALE void                           SetAnyRestrictionFlags(CONSTRAINT_RECORD *,int);
   LOCALE CONSTRAINT_RECORD             *ArgumentTypeToConstraintRecord(void *,int);
   LOCALE CONSTRAINT_RECORD             *FunctionCallToConstraintRecord(void *,void *);
   LOCALE CONSTRAINT_RECORD             *ExpressionToConstraintRecord(void *,struct expr *);

#endif


