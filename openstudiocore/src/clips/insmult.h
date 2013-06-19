   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
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
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_insmult
#define _H_insmult

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _INSMULT_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#if (! RUN_TIME)
LOCALE void SetupInstanceMultifieldCommands(void *);
#endif

LOCALE void MVSlotReplaceCommand(void *,DATA_OBJECT *);
LOCALE void MVSlotInsertCommand(void *,DATA_OBJECT *);
LOCALE void MVSlotDeleteCommand(void *,DATA_OBJECT *);
LOCALE intBool DirectMVReplaceCommand(void *);
LOCALE intBool DirectMVInsertCommand(void *);
LOCALE intBool DirectMVDeleteCommand(void *);

#ifndef _INSMULT_SOURCE_
#endif

#endif



