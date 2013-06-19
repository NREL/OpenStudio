   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*             DEFMODULE PARSER HEADER FILE            */
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
/*************************************************************/

#ifndef _H_modulpsr
#define _H_modulpsr

struct portConstructItem
  {
   char *constructName;
   int typeExpected;
   struct portConstructItem *next;
  };

#ifndef _H_symbol
#include "symbol.h"
#endif
#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _MODULPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE long                           GetNumberOfDefmodules(void *);
   LOCALE void                           SetNumberOfDefmodules(void *,long);
   LOCALE void                           AddAfterModuleDefinedFunction(void *,char *,void (*)(void *),int);
   LOCALE int                            ParseDefmodule(void *,char *);
   LOCALE void                           AddPortConstructItem(void *,char *,int);
   LOCALE struct portConstructItem      *ValidPortConstructItem(void *,char *);
   LOCALE int                            FindImportExportConflict(void *,char *,struct defmodule *,char *);

#endif


