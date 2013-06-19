   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*        DEFRULE CONSTRUCT COMPILER HEADER FILE       */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the constructs-to-c feature for the   */
/*    defrule construct.                                     */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_rulecmp
#define _H_rulecmp

#include "conscomp.h"
#ifndef _H_extnfunc
#include "extnfunc.h"
#endif

#define JoinPrefix() ArbitraryPrefix(DefruleData(theEnv)->DefruleCodeItem,2)
#define LinkPrefix() ArbitraryPrefix(DefruleData(theEnv)->DefruleCodeItem,3)

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _RULECMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                     DefruleCompilerSetup(void *);
   LOCALE void                     DefruleCModuleReference(void *,FILE *,int,int,int);

#ifndef _RULECMP_SOURCE_
extern struct CodeGeneratorItem *DefruleCodeItem;
#endif

#endif




