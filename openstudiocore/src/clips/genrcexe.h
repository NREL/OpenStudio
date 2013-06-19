   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  05/17/06          */
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
/*      6.24: Removed IMPERATIVE_METHODS compilation flag.   */
/*                                                           */
/*************************************************************/

#ifndef _H_genrcexe
#define _H_genrcexe

#if DEFGENERIC_CONSTRUCT

#include "genrcfun.h"
#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _GENRCEXE_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void GenericDispatch(void *,DEFGENERIC *,DEFMETHOD *,DEFMETHOD *,EXPRESSION *,DATA_OBJECT *);
LOCALE void UnboundMethodErr(void *);
LOCALE intBool IsMethodApplicable(void *,DEFMETHOD *);

LOCALE int NextMethodP(void *);
LOCALE void CallNextMethod(void *,DATA_OBJECT *);
LOCALE void CallSpecificMethod(void *,DATA_OBJECT *);
LOCALE void OverrideNextMethod(void *,DATA_OBJECT *);

LOCALE void GetGenericCurrentArgument(void *,DATA_OBJECT *);

#ifndef _GENRCEXE_SOURCE_
#endif

#endif

#endif




