   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  03/04/08            */
   /*                                                     */
   /*                   API HEADER FILE                   */
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
/*      6.24: Added filertr.h and tmpltfun.h to include      */
/*            list.                                          */
/*                                                           */
/*      6.30: Added classpsr.h to include list.              */
/*                                                           */
/*************************************************************/

#ifndef _H_API
#define _H_API

#include "setup.h"
#ifndef _H_argacces
#include "argacces.h"
#endif
#include "constant.h"
#include "memalloc.h"
#include "cstrcpsr.h"
#include "filecom.h"
#include "strngfun.h"
#include "envrnmnt.h"
#include "commline.h"
#ifndef _H_symbol
#include "symbol.h"
#endif

#include "router.h"
#include "filertr.h"
#include "strngrtr.h"

#include "iofun.h"

#include "sysdep.h"
#include "bmathfun.h"
#ifndef _H_expressn
#include "expressn.h"
#endif
#include "exprnpsr.h"
#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_constrct
#include "constrct.h"
#endif
#include "utility.h"
#include "watch.h"
#include "modulbsc.h"

#if BLOAD_ONLY || BLOAD || BLOAD_AND_BSAVE
#include "bload.h"
#endif

#if BLOAD_AND_BSAVE
#include "bsave.h"
#endif

#if DEFRULE_CONSTRUCT
#ifndef _H_ruledef
#include "ruledef.h"
#endif
#include "rulebsc.h"
#include "engine.h"
#include "drive.h"
#include "incrrset.h"
#include "rulecom.h"
#include "crstrtgy.h"
#endif

#if DEFFACTS_CONSTRUCT
#include "dffctdef.h"
#include "dffctbsc.h"
#endif

#if DEFTEMPLATE_CONSTRUCT
#include "tmpltdef.h"
#include "tmpltbsc.h"
#include "tmpltfun.h"
#include "factcom.h"
#include "factfun.h"
#ifndef _H_factmngr
#include "factmngr.h"
#endif
#include "facthsh.h"
#endif

#if DEFGLOBAL_CONSTRUCT
#include "globldef.h"
#include "globlbsc.h"
#include "globlcom.h"
#endif

#if DEFFUNCTION_CONSTRUCT
#include "dffnxfun.h"
#endif

#if DEFGENERIC_CONSTRUCT
#include "genrccom.h"
#include "genrcfun.h"
#endif

#if OBJECT_SYSTEM
#include "classcom.h"
#include "classexm.h"
#include "classinf.h"
#include "classini.h"
#include "classpsr.h"
#include "defins.h"
#include "inscom.h"
#include "insfile.h"
#include "insfun.h"
#include "msgcom.h"
#include "msgpass.h"
#include "objrtmch.h"
#endif


#endif



