   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*           CONSTRUCT COMMAND HEADER MODULE           */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
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
/*            Added ConstructsDeletable function.            */
/*                                                           */
/*************************************************************/

#ifndef _H_cstrccom

#define _H_cstrccom

#ifndef _H_moduldef
#include "moduldef.h"
#endif
#ifndef _H_constrct
#include "constrct.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _CSTRCCOM_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#if (! RUN_TIME)
   LOCALE void                           AddConstructToModule(struct constructHeader *);
#endif
   LOCALE intBool                        DeleteNamedConstruct(void *,char *,struct construct *);
   LOCALE void                          *FindNamedConstruct(void *,char *,struct construct *);
   LOCALE void                           UndefconstructCommand(void *,char *,struct construct *);
   LOCALE int                            PPConstruct(void *,char *,char *,struct construct *);
   LOCALE SYMBOL_HN                     *GetConstructModuleCommand(void *,char *,struct construct *);
   LOCALE struct defmodule              *GetConstructModule(void *,char *,struct construct *);
   LOCALE intBool                        Undefconstruct(void *,void *,struct construct *);
   LOCALE void                           SaveConstruct(void *,void *,char *,struct construct *);
   LOCALE char                          *GetConstructNameString(struct constructHeader *);
   LOCALE char                          *EnvGetConstructNameString(void *,struct constructHeader *);
   LOCALE char                          *GetConstructModuleName(struct constructHeader *);
   LOCALE SYMBOL_HN                     *GetConstructNamePointer(struct constructHeader *);
   LOCALE void                           GetConstructListFunction(void *,char *,DATA_OBJECT_PTR,
                                                                  struct construct *);
   LOCALE void                           GetConstructList(void *,DATA_OBJECT_PTR,struct construct *,
                                                          struct defmodule *);
   LOCALE void                           ListConstructCommand(void *,char *,struct construct *);
   LOCALE void                           ListConstruct(void *,struct construct *,char *,struct defmodule *);
   LOCALE void                           SetNextConstruct(struct constructHeader *,struct constructHeader *);
   LOCALE struct defmoduleItemHeader    *GetConstructModuleItem(struct constructHeader *);
   LOCALE char                          *GetConstructPPForm(void *,struct constructHeader *);
   LOCALE void                           PPConstructCommand(void *,char *,struct construct *);
   LOCALE struct constructHeader        *GetNextConstructItem(void *,struct constructHeader *,int);
   LOCALE struct defmoduleItemHeader    *GetConstructModuleItemByIndex(void *,struct defmodule *,int);
   LOCALE void                           FreeConstructHeaderModule(void *,struct defmoduleItemHeader *,
                                                                   struct construct *);
   LOCALE long                           DoForAllConstructs(void *,
                                                            void (*)(void *,struct constructHeader *,void *),
                                                            int,int,void *);
   LOCALE void                           DoForAllConstructsInModule(void *,void *,
                                                            void (*)(void *,struct constructHeader *,void *),
                                                            int,int,void *);
   LOCALE void                           InitializeConstructHeader(void *,char *,struct constructHeader *,SYMBOL_HN *);
   LOCALE void                           SetConstructPPForm(void *,struct constructHeader *,char *);
   LOCALE void                          *LookupConstruct(void *,struct construct *,char *,intBool);
#if DEBUGGING_FUNCTIONS
   LOCALE unsigned                       ConstructPrintWatchAccess(void *,struct construct *,char *,
                                            EXPRESSION *,
                                            unsigned (*)(void *,void *),
                                            void (*)(void *,unsigned,void *));
   LOCALE unsigned                       ConstructSetWatchAccess(void *,struct construct *,unsigned,
                                            EXPRESSION *,
                                            unsigned (*)(void *,void *),
                                            void (*)(void *,unsigned,void *));
#endif
   LOCALE intBool                        ConstructsDeletable(void *);

#endif



