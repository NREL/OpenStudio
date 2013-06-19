   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*             RULE CONSTRAINTS HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for detecting constraint       */
/*   conflicts in the LHS and RHS of rules.                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_rulecstr

#define _H_rulecstr

#ifdef LOCALE
#undef LOCALE
#endif
#ifdef _RULECSTR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE struct lhsParseNode           *GetExpressionVarConstraints(void *,struct lhsParseNode *);
   LOCALE struct lhsParseNode           *DeriveVariableConstraints(void *,struct lhsParseNode *);
   LOCALE intBool                        ProcessConnectedConstraints(void *,struct lhsParseNode *,struct lhsParseNode *,struct lhsParseNode *);
   LOCALE void                           ConstraintReferenceErrorMessage(void *,
                                                                struct symbolHashNode *,
                                                                struct lhsParseNode *,
                                                                int,int,
                                                                struct symbolHashNode *,
                                                                int);
   LOCALE intBool                        CheckRHSForConstraintErrors(void *,struct expr *,struct lhsParseNode *);

#endif

