   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*            DEFMODULE UTILITY HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for parsing module/construct   */
/*   names and searching through modules for specific        */
/*   constructs.                                             */
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

#ifndef _H_modulutl
#define _H_modulutl

#ifndef _H_symbol
#include "symbol.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _MODULUTL_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE unsigned                       FindModuleSeparator(char *);
   LOCALE SYMBOL_HN                     *ExtractModuleName(void *,unsigned,char *);
   LOCALE SYMBOL_HN                     *ExtractConstructName(void *,unsigned,char *);
   LOCALE char                          *ExtractModuleAndConstructName(void *,char *);
   LOCALE void                          *FindImportedConstruct(void *,char *,struct defmodule *,
                                                               char *,int *,int,struct defmodule *);
   LOCALE void                           AmbiguousReferenceErrorMessage(void *,char *,char *);
   LOCALE void                           MarkModulesAsUnvisited(void *);
   LOCALE void                           ListItemsDriver(void *,
                                                         char *,struct defmodule *,
                                                         char *,char *,
                                                          void *(*)(void *,void *),
                                                          char *(*)(void *),
                                                          void (*)(void *,char *,void *),
                                                          int (*)(void *,void *));
   LOCALE long                           DoForAllModules(void *,
                                                         void (*)(struct defmodule *,void *),
                                                         int,void *);
   LOCALE intBool                        ConstructExported(void *,char *,struct symbolHashNode *,struct symbolHashNode *);
   
#endif



