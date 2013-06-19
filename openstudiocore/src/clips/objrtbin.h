   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  05/17/06          */
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
/*      6.24: Converted INSTANCE_PATTERN_MATCHING to         */
/*            DEFRULE_CONSTRUCT.                             */
/*                                                           */
/*************************************************************/

#ifndef _H_objrtbin
#define _H_objrtbin

#if DEFRULE_CONSTRUCT && OBJECT_SYSTEM

#define OBJECTRETEBIN_DATA 34

struct objectReteBinaryData
  { 
   long AlphaNodeCount;
   long PatternNodeCount;
   OBJECT_ALPHA_NODE *AlphaArray;
   OBJECT_PATTERN_NODE *PatternArray;
  };

#define ObjectReteBinaryData(theEnv) ((struct objectReteBinaryData *) GetEnvironmentData(theEnv,OBJECTRETEBIN_DATA))


#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _OBJRTBIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void SetupObjectPatternsBload(void *);

#endif

#endif



