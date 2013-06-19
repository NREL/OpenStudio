   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  03/04/08            */
   /*                                                     */
   /*          DEFTEMPLATE FUNCTION HEADER FILE           */
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
/*      6.24: Added deftemplate-slot-names,                  */
/*            deftemplate-slot-default-value,                */
/*            deftemplate-slot-cardinality,                  */
/*            deftemplate-slot-allowed-values,               */
/*            deftemplate-slot-range,                        */
/*            deftemplate-slot-types,                        */
/*            deftemplate-slot-multip,                       */
/*            deftemplate-slot-singlep,                      */
/*            deftemplate-slot-existp, and                   */
/*            deftemplate-slot-defaultp functions.           */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Moved default type constants (NO_DEFAULT,      */
/*            STATIC_DEFAULT, and DYNAMIC_DEFAULT) to        */
/*            constant.h                                     */
/*                                                           */
/*************************************************************/

#ifndef _H_tmpltfun

#define _H_tmpltfun

#ifndef _H_symbol
#include "symbol.h"
#endif
#ifndef _H_scanner
#include "scanner.h"
#endif
#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_factmngr
#include "factmngr.h"
#endif
#ifndef _H_tmpltdef
#include "tmpltdef.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _TMPLTFUN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define DeftemplateSlotNames(a,b) EnvDeftemplateSlotNames(GetCurrentEnvironment(),a,b)
#define DeftemplateSlotDefaultValue(a,b,c) EnvDeftemplateSlotDefaultValue(GetCurrentEnvironment(),a,b,c)
#define DeftemplateSlotCardinality(a,b,c) EnvDeftemplateSlotCardinality(GetCurrentEnvironment(),a,b,c)
#define DeftemplateSlotAllowedValues(a,b,c) EnvDeftemplateSlotAllowedValues(GetCurrentEnvironment(),a,b,c)
#define DeftemplateSlotRange(a,b,c) EnvDeftemplateSlotRange(GetCurrentEnvironment(),a,b,c)
#define DeftemplateSlotTypes(a,b,c) EnvDeftemplateSlotTypes(GetCurrentEnvironment(),a,b,c)
#define DeftemplateSlotMultiP(a,b) EnvDeftemplateSlotMultiP(GetCurrentEnvironment(),a,b)
#define DeftemplateSlotSingleP(a,b) EnvDeftemplateSlotSingleP(GetCurrentEnvironment(),a,b)
#define DeftemplateSlotExistP(a,b) EnvDeftemplateSlotExistP(GetCurrentEnvironment(),a,b)
#define DeftemplateSlotDefaultP(a,b) EnvDeftemplateSlotDefaultP(GetCurrentEnvironment(),a,b)

   LOCALE intBool                        UpdateModifyDuplicate(void *,struct expr *,char *,void *);
   LOCALE struct expr                   *ModifyParse(void *,struct expr *,char *);
   LOCALE struct expr                   *DuplicateParse(void *,struct expr *,char *);
   LOCALE void                           DeftemplateFunctions( void *);
   LOCALE void                           ModifyCommand(void *,DATA_OBJECT_PTR);
   LOCALE void                           DuplicateCommand(void *,DATA_OBJECT_PTR);
   LOCALE void                           DeftemplateSlotNamesFunction(void *,DATA_OBJECT *);
   LOCALE void                           EnvDeftemplateSlotNames(void *,void *,DATA_OBJECT *);
   LOCALE void                           DeftemplateSlotDefaultValueFunction(void *,DATA_OBJECT *);
   LOCALE intBool                        EnvDeftemplateSlotDefaultValue(void *,void *,char *,DATA_OBJECT *);
   LOCALE void                           DeftemplateSlotCardinalityFunction(void *,DATA_OBJECT *);
   LOCALE void                           EnvDeftemplateSlotCardinality(void *,void *,char *,DATA_OBJECT *);
   LOCALE void                           DeftemplateSlotAllowedValuesFunction(void *,DATA_OBJECT *);
   LOCALE void                           EnvDeftemplateSlotAllowedValues(void *,void *,char *,DATA_OBJECT *);
   LOCALE void                           DeftemplateSlotRangeFunction(void *,DATA_OBJECT *);
   LOCALE void                           EnvDeftemplateSlotRange(void *,void *,char *,DATA_OBJECT *);
   LOCALE void                           DeftemplateSlotTypesFunction(void *,DATA_OBJECT *);
   LOCALE void                           EnvDeftemplateSlotTypes(void *,void *,char *,DATA_OBJECT *);
   LOCALE int                            DeftemplateSlotMultiPFunction(void *);
   LOCALE int                            EnvDeftemplateSlotMultiP(void *,void *,char *);
   LOCALE int                            DeftemplateSlotSinglePFunction(void *);
   LOCALE int                            EnvDeftemplateSlotSingleP(void *,void *,char *);
   LOCALE int                            DeftemplateSlotExistPFunction(void *);
   LOCALE int                            EnvDeftemplateSlotExistP(void *,void *,char *);
   LOCALE void                          *DeftemplateSlotDefaultPFunction(void *);
   LOCALE int                            EnvDeftemplateSlotDefaultP(void *,void *,char *);
   LOCALE int                            DeftemplateSlotFacetExistPFunction(void *);
   LOCALE int                            EnvDeftemplateSlotFacetExistP(void *,void *,char *,char *);
   LOCALE void                           DeftemplateSlotFacetValueFunction(void *,DATA_OBJECT *);
   LOCALE int                            EnvDeftemplateSlotFacetValue(void *,void *,char *,char *,DATA_OBJECT *);

#endif




