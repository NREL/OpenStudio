   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*             DEFRULE COMMANDS HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides the matches command. Also provides the  */
/*   the developer commands show-joins and rule-complexity.  */
/*   Also provides the initialization routine which          */
/*   registers rule commands found in other modules.         */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*            Added matches-count function.                  */
/*                                                           */
/*            Added get-join-hashing and set-join-hashing    */
/*            functions.                                     */
/*                                                           */
/*************************************************************/

#ifndef _H_rulecom
#define _H_rulecom

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _RULECOM_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define Matches(a) EnvMatches(GetCurrentEnvironment(),a)
#define JoinActivity(a,b) EnvJoinActivity(GetCurrentEnvironment(),a,b)
#define MatchesCount(a) EnvMatchesCount(GetCurrentEnvironment(),a)
#define GetBetaMemoryResizing() EnvGetBetaMemoryResizing(GetCurrentEnvironment())
#define SetBetaMemoryResizing(a) EnvSetBetaMemoryResizing(GetCurrentEnvironment(),a)

   LOCALE intBool                        EnvGetBetaMemoryResizing(void *);
   LOCALE intBool                        EnvSetBetaMemoryResizing(void *,intBool);
   LOCALE int                            GetBetaMemoryResizingCommand(void *);
   LOCALE int                            SetBetaMemoryResizingCommand(void *);

   LOCALE intBool                        EnvMatches(void *,void *);
   LOCALE long long                      EnvJoinActivity(void *,void *,int);
   LOCALE intBool                        EnvMatchesCount(void *,void *);
   LOCALE void                           DefruleCommands(void *);
   LOCALE void                           MatchesCommand(void *);
   LOCALE long long                      JoinActivityCommand(void *);
   LOCALE void                           MatchesCountCommand(void *);
   LOCALE long long                      TimetagFunction(void *);
#if DEVELOPER
   LOCALE void                           ShowJoinsCommand(void *);
   LOCALE long                           RuleComplexityCommand(void *);
   LOCALE void                           ShowAlphaHashTable(void *);
#endif

#endif


