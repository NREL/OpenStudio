   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  05/17/06          */
   /*                                                     */
   /*           INSTANCE MODIFY AND DUPLICATE MODULE      */
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

#ifndef _H_insmoddp
#define _H_insmoddp

#define DIRECT_MODIFY_STRING    "direct-modify"
#define MSG_MODIFY_STRING       "message-modify"
#define DIRECT_DUPLICATE_STRING "direct-duplicate"
#define MSG_DUPLICATE_STRING    "message-duplicate"

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _INSMODDP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#if (! RUN_TIME)
LOCALE void SetupInstanceModDupCommands(void *);
#endif

LOCALE void ModifyInstance(void *,DATA_OBJECT *);
LOCALE void MsgModifyInstance(void *,DATA_OBJECT *);
LOCALE void DuplicateInstance(void *,DATA_OBJECT *);
LOCALE void MsgDuplicateInstance(void *,DATA_OBJECT *);

#if DEFRULE_CONSTRUCT && OBJECT_SYSTEM
LOCALE void InactiveModifyInstance(void *,DATA_OBJECT *);
LOCALE void InactiveMsgModifyInstance(void *,DATA_OBJECT *);
LOCALE void InactiveDuplicateInstance(void *,DATA_OBJECT *);
LOCALE void InactiveMsgDuplicateInstance(void *,DATA_OBJECT *);
#endif

LOCALE void DirectModifyMsgHandler(void *,DATA_OBJECT *);
LOCALE void MsgModifyMsgHandler(void *,DATA_OBJECT *);
LOCALE void DirectDuplicateMsgHandler(void *,DATA_OBJECT *);
LOCALE void MsgDuplicateMsgHandler(void *,DATA_OBJECT *);

#ifndef _INSMODDP_SOURCE_
#endif

#endif







