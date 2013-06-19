   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*               RULE BUILD HEADER FILE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines to ntegrates a set of pattern  */
/*   and join tests associated with a rule into the pattern  */
/*   and join networks. The joins are integrated into the    */
/*   join network by routines in this module. The pattern    */
/*   is integrated by calling the external routine           */
/*   associated with the pattern parser that originally      */
/*   parsed the pattern.                                     */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_rulebld

#define _H_rulebld

#ifndef _H_reorder
#include "reorder.h"
#endif
#ifndef _H_network
#include "network.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _RULEBLD_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE struct joinNode               *ConstructJoins(void *,int,struct lhsParseNode *,int);

#endif



