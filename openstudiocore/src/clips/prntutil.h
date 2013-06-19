   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  07/01/05            */
   /*                                                     */
   /*              PRINT UTILITY HEADER FILE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Utility routines for printing various items      */
/*   and messages.                                           */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Link error occurs for the SlotExistError       */
/*            function when OBJECT_SYSTEM is set to 0 in     */
/*            setup.h. DR0865                                */
/*                                                           */
/*            Added DataObjectToString function.             */
/*                                                           */
/*            Added SlotExistError function.                 */
/*                                                           */
/*************************************************************/

#ifndef _H_prntutil
#define _H_prntutil

#ifndef _H_moduldef
#include "moduldef.h"
#endif

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

#define PRINT_UTILITY_DATA 53

struct printUtilityData
  { 
   intBool PreserveEscapedCharacters;
   intBool AddressesToStrings;
   intBool InstanceAddressesToNames;
  };

#define PrintUtilityData(theEnv) ((struct printUtilityData *) GetEnvironmentData(theEnv,PRINT_UTILITY_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _PRNTUTIL_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif
   LOCALE void                           InitializePrintUtilityData(void *);
   LOCALE void                           PrintInChunks(void *,char *,char *);
   LOCALE void                           PrintFloat(void *,char *,double);
   LOCALE void                           PrintLongInteger(void *,char *,long long);
   LOCALE void                           PrintAtom(void *,char *,int,void *);
   LOCALE void                           PrintTally(void *,char *,long long,char *,char *);
   LOCALE char                          *FloatToString(void *,double);
   LOCALE char                          *LongIntegerToString(void *,long long);
   LOCALE char                          *DataObjectToString(void *,DATA_OBJECT *);
   LOCALE void                           SyntaxErrorMessage(void *,char *);
   LOCALE void                           SystemError(void *,char *,int);
   LOCALE void                           PrintErrorID(void *,char *,int,int);
   LOCALE void                           PrintWarningID(void *,char *,int,int);
   LOCALE void                           CantFindItemErrorMessage(void *,char *,char *);
   LOCALE void                           CantDeleteItemErrorMessage(void *,char *,char *);
   LOCALE void                           AlreadyParsedErrorMessage(void *,char *,char *);
   LOCALE void                           LocalVariableErrorMessage(void *,char *);
   LOCALE void                           DivideByZeroErrorMessage(void *,char *);
   LOCALE void                           SalienceInformationError(void *,char *,char *);
   LOCALE void                           SalienceRangeError(void *,int,int);
   LOCALE void                           SalienceNonIntegerError(void *);
   LOCALE void                           CantFindItemInFunctionErrorMessage(void *,char *,char *,char *);
   LOCALE void                           SlotExistError(void *,char *,char *);

#endif






