   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*           DEFMODULE BSAVE/BLOAD HEADER FILE         */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
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

#ifndef _H_modulbin

#define _H_modulbin

#ifndef _H_moduldef
#include "moduldef.h"
#endif

struct bsaveDefmodule
  {
   unsigned long name;
   long importList;
   long exportList;
   long next;
   long bsaveID;
  };

struct bsaveDefmoduleItemHeader
  {
   long theModule;
   long firstItem;
   long lastItem;
  };

struct bsavePortItem
  {
   long moduleName;
   long constructType;
   long constructName;
   long next;
  };

#define ModulePointer(i) ((struct defmodule *) (&DefmoduleData(theEnv)->DefmoduleArray[i]))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _MODULBIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           DefmoduleBinarySetup(void *);
   LOCALE void                           UpdateDefmoduleItemHeader
                                                 (void *,struct bsaveDefmoduleItemHeader *,
                                                  struct defmoduleItemHeader *,int,void *);

#if BLOAD_AND_BSAVE
   LOCALE void                           AssignBsaveDefmdlItemHdrVals
                                                 (struct bsaveDefmoduleItemHeader *,
                                                  struct defmoduleItemHeader *);
#endif

#endif



