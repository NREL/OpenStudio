   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Message-passing support functions                */
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

#ifndef _H_msgfun
#define _H_msgfun

typedef struct handlerSlotReference
  {
   long classID;
   long slotID;
  } HANDLER_SLOT_REFERENCE;

#ifndef _H_object
#include "object.h"
#endif
#include "msgpass.h"

#define BEGIN_TRACE ">>"
#define END_TRACE   "<<"

/* =================================================================================
   Message-handler types - don't change these values: a string array depends on them
   ================================================================================= */
#define MAROUND        0
#define MBEFORE        1
#define MPRIMARY       2
#define MAFTER         3
#define MERROR         4

#define LOOKUP_HANDLER_INDEX   0
#define LOOKUP_HANDLER_ADDRESS 1

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _MSGFUN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void             UnboundHandlerErr(void *);
   LOCALE void             PrintNoHandlerError(void *,char *);
   LOCALE int              CheckHandlerArgCount(void *);
   LOCALE void             SlotAccessViolationError(void *,char *,intBool,void *);
   LOCALE void             SlotVisibilityViolationError(void *,SLOT_DESC *,DEFCLASS *);

#if ! RUN_TIME
   LOCALE void             NewSystemHandler(void *,char *,char *,char *,int);
   LOCALE HANDLER         *InsertHandlerHeader(void *,DEFCLASS *,SYMBOL_HN *,int);
#endif

#if (! BLOAD_ONLY) && (! RUN_TIME)
   LOCALE HANDLER         *NewHandler(void);
   LOCALE int              HandlersExecuting(DEFCLASS *);
   LOCALE int              DeleteHandler(void *,DEFCLASS *,SYMBOL_HN *,int,int);
   LOCALE void             DeallocateMarkedHandlers(void *,DEFCLASS *);
#endif
   LOCALE unsigned         HandlerType(void *,char *,char *);
   LOCALE int              CheckCurrentMessage(void *,char *,int);
   LOCALE void             PrintHandler(void *,char *,HANDLER *,int);
   LOCALE HANDLER         *FindHandlerByAddress(DEFCLASS *,SYMBOL_HN *,unsigned);
   LOCALE int              FindHandlerByIndex(DEFCLASS *,SYMBOL_HN *,unsigned);
   LOCALE int              FindHandlerNameGroup(DEFCLASS *,SYMBOL_HN *);
   LOCALE void             HandlerDeleteError(void *,char *);

#if DEBUGGING_FUNCTIONS
   LOCALE void             DisplayCore(void *,char *,HANDLER_LINK *,int);
   LOCALE HANDLER_LINK    *FindPreviewApplicableHandlers(void *,DEFCLASS *,SYMBOL_HN *);
   LOCALE void             WatchMessage(void *,char *,char *);
   LOCALE void             WatchHandler(void *,char *,HANDLER_LINK *,char *);
#endif

#endif







