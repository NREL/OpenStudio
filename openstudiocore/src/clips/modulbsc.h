   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*         DEFMODULE BASIC COMMANDS HEADER FILE        */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements core commands for the deffacts        */
/*   construct such as clear, reset, save, undeffacts,       */
/*   ppdeffacts, list-deffacts, and get-deffacts-list.       */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_modulbsc
#define _H_modulbsc

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _MODULBSC_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define GetDefmoduleList(a) EnvGetDefmoduleList(GetCurrentEnvironment(),a)
#define ListDefmodules(a) EnvListDefmodules(GetCurrentEnvironment(),a)

   LOCALE void                           DefmoduleBasicCommands(void *);
   LOCALE void                           EnvGetDefmoduleList(void *,DATA_OBJECT_PTR);
   LOCALE void                           PPDefmoduleCommand(void *);
   LOCALE int                            PPDefmodule(void *,char *,char *);
   LOCALE void                           ListDefmodulesCommand(void *);
   LOCALE void                           EnvListDefmodules(void *,char *);

#endif

