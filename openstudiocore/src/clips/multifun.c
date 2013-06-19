   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*             MULTIFIELD FUNCTIONS MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for several multifield         */
/*   functions including first$, rest$, subseq$, delete$,    */
/*   delete-member$, replace-member$                         */
/*   replace$, insert$, explode$, implode$, nth$, member$,   */
/*   subsetp, progn$, str-implode, str-explode, subset, nth, */
/*   mv-replace, member, mv-subseq, and mv-delete.           */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian Dantes                                         */
/*      Barry Cameron                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*            Changed name of variable exp to theExp         */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Moved ImplodeMultifield to multifld.c.         */
/*                                                           */
/*      6.30: Added foreach function.                        */
/*                                                           */
/*************************************************************/

#define _MULTIFUN_SOURCE_

#include "setup.h"

#if MULTIFIELD_FUNCTIONS || OBJECT_SYSTEM

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <string.h>

#include "argacces.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "memalloc.h"
#include "multifld.h"
#include "multifun.h"
#include "prcdrpsr.h"
#include "prcdrfun.h"
#include "router.h"
#if (! BLOAD_ONLY) && (! RUN_TIME)
#include "scanner.h"
#endif
#include "utility.h"

#if OBJECT_SYSTEM
#include "object.h"
#endif

/**************/
/* STRUCTURES */
/**************/

typedef struct fieldVarStack
  {
   unsigned short type;
   void *value;
   long index;
   struct fieldVarStack *nxt;
  } FIELD_VAR_STACK;

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if MULTIFIELD_FUNCTIONS
   static intBool                 MVRangeCheck(long,long,long *,int);
#if (! BLOAD_ONLY) && (! RUN_TIME)
   static struct expr            *MultifieldPrognParser(void *,struct expr *,char *);
   static struct expr            *ForeachParser(void *,struct expr *,char *);
   static void                    ReplaceMvPrognFieldVars(void *,SYMBOL_HN *,struct expr *,int);
#endif
#endif
   static void                    MVRangeError(void *,long,long,long,char *);
   static void                    MultifieldPrognDriver(void *,DATA_OBJECT_PTR,char *);
#endif

/***************************************/
/* LOCAL INTERNAL VARIABLE DEFINITIONS */
/***************************************/

#if MULTIFIELD_FUNCTIONS

#define MULTIFUN_DATA 10

struct multiFunctionData
  { 
   FIELD_VAR_STACK *FieldVarStack;
  };

#define MultiFunctionData(theEnv) ((struct multiFunctionData *) GetEnvironmentData(theEnv,MULTIFUN_DATA))

/**********************************************/
/* MultifieldFunctionDefinitions: Initializes */
/*   the multifield functions.                */
/**********************************************/
globle void MultifieldFunctionDefinitions(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,MULTIFUN_DATA,sizeof(struct multiFunctionData),NULL);

#if ! RUN_TIME
   EnvDefineFunction2(theEnv,"first$", 'm', PTIEF FirstFunction, "FirstFunction", "11m");
   EnvDefineFunction2(theEnv,"rest$", 'm', PTIEF RestFunction, "RestFunction", "11m");
   EnvDefineFunction2(theEnv,"subseq$", 'm', PTIEF SubseqFunction, "SubseqFunction", "33im");
   EnvDefineFunction2(theEnv,"delete-member$", 'm', PTIEF DeleteMemberFunction, "DeleteMemberFunction", "2*um");
   EnvDefineFunction2(theEnv,"replace-member$", 'm', PTIEF ReplaceMemberFunction, "ReplaceMemberFunction","3*um");
   EnvDefineFunction2(theEnv,"delete$", 'm', PTIEF DeleteFunction, "DeleteFunction", "33im");
   EnvDefineFunction2(theEnv,"replace$", 'm', PTIEF ReplaceFunction, "ReplaceFunction","4**mii");
   EnvDefineFunction2(theEnv,"insert$", 'm', PTIEF InsertFunction, "InsertFunction", "3**mi");
   EnvDefineFunction2(theEnv,"explode$", 'm', PTIEF ExplodeFunction, "ExplodeFunction", "11s");
   EnvDefineFunction2(theEnv,"implode$", 's', PTIEF ImplodeFunction, "ImplodeFunction", "11m");
   EnvDefineFunction2(theEnv,"nth$", 'u', PTIEF NthFunction, "NthFunction", "22*im");
   EnvDefineFunction2(theEnv,"member$", 'u', PTIEF MemberFunction, "MemberFunction", "22*um");
   EnvDefineFunction2(theEnv,"subsetp", 'b', PTIEF SubsetpFunction, "SubsetpFunction", "22*mm");
   EnvDefineFunction2(theEnv,"progn$", 'u', PTIEF MultifieldPrognFunction, "MultifieldPrognFunction", NULL);
   EnvDefineFunction2(theEnv,"foreach", 'u', PTIEF ForeachFunction, "ForeachFunction", NULL);
   EnvDefineFunction2(theEnv,"str-implode", 's', PTIEF ImplodeFunction, "ImplodeFunction", "11m");
   EnvDefineFunction2(theEnv,"str-explode", 'm', PTIEF ExplodeFunction, "ExplodeFunction", "11s");
   EnvDefineFunction2(theEnv,"subset", 'b', PTIEF SubsetpFunction, "SubsetpFunction", "22*mm");
   EnvDefineFunction2(theEnv,"nth", 'u', PTIEF NthFunction, "NthFunction", "22*im");
   EnvDefineFunction2(theEnv,"mv-replace", 'm', PTIEF MVReplaceFunction, "MVReplaceFunction","33*im");
   EnvDefineFunction2(theEnv,"member", 'u', PTIEF MemberFunction, "MemberFunction", "22*um");
   EnvDefineFunction2(theEnv,"mv-subseq", 'm', PTIEF MVSubseqFunction, "MVSubseqFunction", "33*iim");
   EnvDefineFunction2(theEnv,"mv-delete", 'm', PTIEF MVDeleteFunction,"MVDeleteFunction", "22*im");
#if ! BLOAD_ONLY
   AddFunctionParser(theEnv,"progn$",MultifieldPrognParser);
   AddFunctionParser(theEnv,"foreach",ForeachParser);
#endif
   FuncSeqOvlFlags(theEnv,"progn$",FALSE,FALSE);
   EnvDefineFunction2(theEnv,"(get-progn$-field)", 'u', PTIEF GetMvPrognField, "GetMvPrognField", "00");
   EnvDefineFunction2(theEnv,"(get-progn$-index)", 'l', PTIEF GetMvPrognIndex, "GetMvPrognIndex", "00");
#endif
  }

/****************************************/
/* DeleteFunction: H/L access routine   */
/*   for the delete$ function.          */
/****************************************/
globle void DeleteFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT value1, value2, value3;

   /*=======================================*/
   /* Check for the correct argument types. */
   /*=======================================*/

   if ((EnvArgTypeCheck(theEnv,"delete$",1,MULTIFIELD,&value1) == FALSE) ||
       (EnvArgTypeCheck(theEnv,"delete$",2,INTEGER,&value2) == FALSE) ||
       (EnvArgTypeCheck(theEnv,"delete$",3,INTEGER,&value3) == FALSE))
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*=================================================*/
   /* Delete the section out of the multifield value. */
   /*=================================================*/

   if (DeleteMultiValueField(theEnv,returnValue,&value1,
            (long) DOToLong(value2),(long) DOToLong(value3),"delete$") == FALSE)/* TBD */
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
     }
  }

/******************************************/
/* MVDeleteFunction: H/L access routine   */
/*   for the mv-delete function.          */
/******************************************/
globle void MVDeleteFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT value1, value2;

   /*=======================================*/
   /* Check for the correct argument types. */
   /*=======================================*/

   if ((EnvArgTypeCheck(theEnv,"mv-delete",1,INTEGER,&value1) == FALSE) ||
       (EnvArgTypeCheck(theEnv,"mv-delete",2,MULTIFIELD,&value2) == FALSE))
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*=================================================*/
   /* Delete the section out of the multifield value. */
   /*=================================================*/

   if (DeleteMultiValueField(theEnv,returnValue,&value2,
            (long) DOToLong(value1),(long) DOToLong(value1),"mv-delete") == FALSE) /* TBD */
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
     }
  }

/*****************************************/
/* ReplaceFunction: H/L access routine   */
/*   for the replace$ function.          */
/*****************************************/
globle void ReplaceFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT value1, value2, value3, value4;
   EXPRESSION *fieldarg;

   /*=======================================*/
   /* Check for the correct argument types. */
   /*=======================================*/

   if ((EnvArgTypeCheck(theEnv,"replace$",1,MULTIFIELD,&value1) == FALSE) ||
       (EnvArgTypeCheck(theEnv,"replace$",2,INTEGER,&value2) == FALSE) ||
       (EnvArgTypeCheck(theEnv,"replace$",3,INTEGER,&value3) == FALSE))
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*===============================*/
   /* Create the replacement value. */
   /*===============================*/

   fieldarg = GetFirstArgument()->nextArg->nextArg->nextArg;
   if (fieldarg->nextArg != NULL)
     { StoreInMultifield(theEnv,&value4,fieldarg,TRUE); }
   else
     { EvaluateExpression(theEnv,fieldarg,&value4); }

   /*==============================================*/
   /* Replace the section in the multifield value. */
   /*==============================================*/

   if (ReplaceMultiValueField(theEnv,returnValue,&value1,(long) DOToLong(value2),
                   (long) DOToLong(value3),&value4,"replace$") == FALSE) /* TBD */
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
     }
  }

/*******************************************/
/* MVReplaceFunction: H/L access routine   */
/*   for the mv-replace function.          */
/*******************************************/
globle void MVReplaceFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT value1, value2, value3;

   /*=======================================*/
   /* Check for the correct argument types. */
   /*=======================================*/

   if ((EnvArgTypeCheck(theEnv,"mv-replace",1,INTEGER,&value1) == FALSE) ||
       (EnvArgTypeCheck(theEnv,"mv-replace",2,MULTIFIELD,&value2) == FALSE))
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*===============================*/
   /* Create the replacement value. */
   /*===============================*/

   EvaluateExpression(theEnv,GetFirstArgument()->nextArg->nextArg,&value3);

   /*==============================================*/
   /* Replace the section in the multifield value. */
   /*==============================================*/

   if (ReplaceMultiValueField(theEnv,returnValue,&value2,(long) DOToLong(value1),
                   (long) DOToLong(value1),&value3,"mv-replace") == FALSE) /* TBD */
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
     }
  }

/**********************************************/
/* DeleteMemberFunction: H/L access routine   */
/*   for the delete-member$ function.         */
/**********************************************/
globle void DeleteMemberFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT resultValue,*delVals,tmpVal;
   int i,argCnt;
   unsigned delSize;
   long j,k;

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/

   argCnt = EnvArgCountCheck(theEnv,"delete-member$",AT_LEAST,2);
   if (argCnt == -1)
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*=======================================*/
   /* Check for the correct argument types. */
   /*=======================================*/
   if (EnvArgTypeCheck(theEnv,"delete-member$",1,MULTIFIELD,&resultValue) == FALSE)
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*=================================================
     For every value specified, delete all occurrences
     of those values from the multifield
     ================================================= */
   delSize = (sizeof(DATA_OBJECT) * (argCnt-1));
   delVals = (DATA_OBJECT_PTR) gm2(theEnv,delSize);
   for (i = 2 ; i <= argCnt ; i++)
     {
      if (!EnvRtnUnknown(theEnv,i,&delVals[i-2]))
        {
         rm(theEnv,(void *) delVals,delSize);
         SetEvaluationError(theEnv,TRUE);
         EnvSetMultifieldErrorValue(theEnv,returnValue);
         return;
        }
     }

   while (FindDOsInSegment(delVals,argCnt-1,&resultValue,&j,&k,NULL,0))
     {
      if (DeleteMultiValueField(theEnv,&tmpVal,&resultValue,
                                j,k,"delete-member$") == FALSE)
        {
         rm(theEnv,(void *) delVals,delSize);
         SetEvaluationError(theEnv,TRUE);
         EnvSetMultifieldErrorValue(theEnv,returnValue);
         return;
        }
      GenCopyMemory(DATA_OBJECT,1,&resultValue,&tmpVal);
     }
   rm(theEnv,(void *) delVals,delSize);
   GenCopyMemory(DATA_OBJECT,1,returnValue,&resultValue);
  }

/***********************************************/
/* ReplaceMemberFunction: H/L access routine   */
/*   for the replace-member$ function.         */
/***********************************************/
globle void ReplaceMemberFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT resultValue,replVal,*delVals,tmpVal;
   int i,argCnt;
   unsigned delSize;
   long j,k,mink[2],*minkp;
   long replLen = 1L;

   /*============================================*/
   /* Check for the correct number of arguments. */
   /*============================================*/
   argCnt = EnvArgCountCheck(theEnv,"replace-member$",AT_LEAST,3);
   if (argCnt == -1)
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*=======================================*/
   /* Check for the correct argument types. */
   /*=======================================*/
   if (EnvArgTypeCheck(theEnv,"replace-member$",1,MULTIFIELD,&resultValue) == FALSE)
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   if (!EnvRtnUnknown(theEnv,2,&replVal))
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }
   if (GetType(replVal) == MULTIFIELD)
     replLen = GetDOLength(replVal);

   /*=====================================================
     For the value (or values from multifield ) specified,
     replace all occurrences of those values with all
     values specified
     ===================================================== */
   delSize = (sizeof(DATA_OBJECT) * (argCnt-2));
   delVals = (DATA_OBJECT_PTR) gm2(theEnv,delSize);
   for (i = 3 ; i <= argCnt ; i++)
     {
      if (!EnvRtnUnknown(theEnv,i,&delVals[i-3]))
        {
         rm(theEnv,(void *) delVals,delSize);
         SetEvaluationError(theEnv,TRUE);
         EnvSetMultifieldErrorValue(theEnv,returnValue);
         return;
        }
     }
   minkp = NULL;
   while (FindDOsInSegment(delVals,argCnt-2,&resultValue,&j,&k,minkp,minkp ? 1 : 0))
     {
      if (ReplaceMultiValueField(theEnv,&tmpVal,&resultValue,j,k,
                                 &replVal,"replace-member$") == FALSE)
        {
         rm(theEnv,(void *) delVals,delSize);
         SetEvaluationError(theEnv,TRUE);
         EnvSetMultifieldErrorValue(theEnv,returnValue);
         return;
        }
      GenCopyMemory(DATA_OBJECT,1,&resultValue,&tmpVal);
      mink[0] = 1L;
      mink[1] = j + replLen - 1L;
      minkp = mink;
     }
   rm(theEnv,(void *) delVals,delSize);
   GenCopyMemory(DATA_OBJECT,1,returnValue,&resultValue);
  }

/****************************************/
/* InsertFunction: H/L access routine   */
/*   for the insert$ function.          */
/****************************************/
globle void InsertFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT value1, value2, value3;
   EXPRESSION *fieldarg;

   /*=======================================*/
   /* Check for the correct argument types. */
   /*=======================================*/

   if ((EnvArgTypeCheck(theEnv,"insert$",1,MULTIFIELD,&value1) == FALSE) ||
       (EnvArgTypeCheck(theEnv,"insert$",2,INTEGER,&value2) == FALSE))
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*=============================*/
   /* Create the insertion value. */
   /*=============================*/

   fieldarg = GetFirstArgument()->nextArg->nextArg;
   if (fieldarg->nextArg != NULL)
     StoreInMultifield(theEnv,&value3,fieldarg,TRUE);
   else
     EvaluateExpression(theEnv,fieldarg,&value3);

   /*===========================================*/
   /* Insert the value in the multifield value. */
   /*===========================================*/

   if (InsertMultiValueField(theEnv,returnValue,&value1,(long) DOToLong(value2), /* TBD */
                             &value3,"insert$") == FALSE)
     {
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
     }
  }

/*****************************************/
/* ExplodeFunction: H/L access routine   */
/*   for the explode$ function.          */
/*****************************************/
globle void ExplodeFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT value;
   struct multifield *theMultifield;
   unsigned long end;

   /*=====================================*/
   /* Explode$ expects a single argument. */
   /*=====================================*/

   if (EnvArgCountCheck(theEnv,"explode$",EXACTLY,1) == -1)
     {
      SetHaltExecution(theEnv,TRUE);
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*==================================*/
   /* The argument should be a string. */
   /*==================================*/

   if (EnvArgTypeCheck(theEnv,"explode$",1,STRING,&value) == FALSE)
     {
      SetHaltExecution(theEnv,TRUE);
      SetEvaluationError(theEnv,TRUE);
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   /*=====================================*/
   /* Convert the string to a multifield. */
   /*=====================================*/

   theMultifield = StringToMultifield(theEnv,DOToString(value));
   if (theMultifield == NULL)
     {
      theMultifield = (struct multifield *) EnvCreateMultifield(theEnv,0L);
      end = 0;
     }
   else
     { end = GetMFLength(theMultifield); }

   /*========================*/
   /* Return the multifield. */
   /*========================*/

   SetpType(returnValue,MULTIFIELD);
   SetpDOBegin(returnValue,1);
   SetpDOEnd(returnValue,end);
   SetpValue(returnValue,(void *) theMultifield);
   return;
  }

/*****************************************/
/* ImplodeFunction: H/L access routine   */
/*   for the implode$ function.          */
/*****************************************/
globle void *ImplodeFunction(
  void *theEnv)
  {
   DATA_OBJECT value;

   /*=====================================*/
   /* Implode$ expects a single argument. */
   /*=====================================*/

   if (EnvArgCountCheck(theEnv,"implode$",EXACTLY,1) == -1)
     { return(EnvAddSymbol(theEnv,"")); }

   /*======================================*/
   /* The argument should be a multifield. */
   /*======================================*/

   if (EnvArgTypeCheck(theEnv,"implode$",1,MULTIFIELD,&value) == FALSE)
     { return(EnvAddSymbol(theEnv,"")); }

   /*====================*/
   /* Return the string. */
   /*====================*/

   return(ImplodeMultifield(theEnv,&value));
  }

/****************************************/
/* SubseqFunction: H/L access routine   */
/*   for the subseq$ function.          */
/****************************************/
globle void SubseqFunction(
  void *theEnv,
  DATA_OBJECT_PTR sub_value)
  {
   DATA_OBJECT value;
   struct multifield *theList;
   long long offset, start, end, length; /* 6.04 Bug Fix */

   /*===================================*/
   /* Get the segment to be subdivided. */
   /*===================================*/

   if (EnvArgTypeCheck(theEnv,"subseq$",1,MULTIFIELD,&value) == FALSE)
     {
      EnvSetMultifieldErrorValue(theEnv,sub_value);
      return;
     }
   theList = (struct multifield *) DOToPointer(value);
   offset = GetDOBegin(value);
   length = GetDOLength(value);

   /*=============================================*/
   /* Get range arguments. If they are not within */
   /* appropriate ranges, return a null segment.  */
   /*=============================================*/

   if (EnvArgTypeCheck(theEnv,"subseq$",2,INTEGER,&value) == FALSE)
     {
      EnvSetMultifieldErrorValue(theEnv,sub_value);
      return;
     }
   start = DOToLong(value);

   if (EnvArgTypeCheck(theEnv,"subseq$",3,INTEGER,&value) == FALSE)
     {
      EnvSetMultifieldErrorValue(theEnv,sub_value);
      return;
     }
   end = DOToLong(value);

   if ((end < 1) || (end < start))
     {
      EnvSetMultifieldErrorValue(theEnv,sub_value);
      return;
     }

   /*===================================================*/
   /* Adjust lengths  to conform to segment boundaries. */
   /*===================================================*/

   if (start > length)
     {
      EnvSetMultifieldErrorValue(theEnv,sub_value);
      return;
     }
   if (end > length) end = length;
   if (start < 1) start = 1;

   /*=========================*/
   /* Return the new segment. */
   /*=========================*/

   SetpType(sub_value,MULTIFIELD);
   SetpValue(sub_value,theList);
   SetpDOEnd(sub_value,offset + end - 1);
   SetpDOBegin(sub_value,offset + start - 1);
  }

/******************************************/
/* MVSubseqFunction: H/L access routine   */
/*   for the mv-subseq function.          */
/******************************************/
globle void MVSubseqFunction(
  void *theEnv,
  DATA_OBJECT_PTR sub_value)
  {
   DATA_OBJECT value;
   struct multifield *theList;
   long long offset, start, end, length; /* 6.04 Bug Fix */

   /*=============================================*/
   /* Get range arguments. If they are not within */
   /* appropriate ranges, return a null segment.  */
   /*=============================================*/

   if (EnvArgTypeCheck(theEnv,"mv-subseq",1,INTEGER,&value) == FALSE)
     {
      EnvSetMultifieldErrorValue(theEnv,sub_value);
      return;
     }
   start = DOToLong(value);

   if (EnvArgTypeCheck(theEnv,"mv-subseq",2,INTEGER,&value) == FALSE)
     {
      EnvSetMultifieldErrorValue(theEnv,sub_value);
      return;
     }
   end = DOToLong(value);

   if ((end < 1) || (end < start))
     {
      EnvSetMultifieldErrorValue(theEnv,sub_value);
      return;
     }

   /*===================================*/
   /* Get the segment to be subdivided. */
   /*===================================*/

   if (EnvArgTypeCheck(theEnv,"mv-subseq",3,MULTIFIELD,&value) == FALSE)
     {
      EnvSetMultifieldErrorValue(theEnv,sub_value);
      return;
     }
   theList = (struct multifield *) DOToPointer(value);
   offset = GetDOBegin(value);

   /*===================================================*/
   /* Adjust lengths  to conform to segment boundaries. */
   /*===================================================*/

   length = GetDOLength(value);
   if (start > length)
     {
      EnvSetMultifieldErrorValue(theEnv,sub_value);
      return;
     }
   if (end > length) end = length;
   if (start < 1) start = 1;

   /*=========================*/
   /* Return the new segment. */
   /*=========================*/

   SetpType(sub_value,MULTIFIELD);
   SetpValue(sub_value,theList);
   SetpDOEnd(sub_value,offset + end - 1);
   SetpDOBegin(sub_value,offset + start - 1);
  }

/***************************************/
/* FirstFunction: H/L access routine   */
/*   for the first$ function.          */
/***************************************/
globle void FirstFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT theValue;
   struct multifield *theList;

   /*===================================*/
   /* Get the segment to be subdivided. */
   /*===================================*/

   if (EnvArgTypeCheck(theEnv,"first$",1,MULTIFIELD,&theValue) == FALSE)
     {
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   theList = (struct multifield *) DOToPointer(theValue);

   /*=========================*/
   /* Return the new segment. */
   /*=========================*/

   SetpType(returnValue,MULTIFIELD);
   SetpValue(returnValue,theList);
   if (GetDOEnd(theValue) >= GetDOBegin(theValue))
     { SetpDOEnd(returnValue,GetDOBegin(theValue)); }
   else
     { SetpDOEnd(returnValue,GetDOEnd(theValue)); }
   SetpDOBegin(returnValue,GetDOBegin(theValue));
  }

/**************************************/
/* RestFunction: H/L access routine   */
/*   for the rest$ function.          */
/**************************************/
globle void RestFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   DATA_OBJECT theValue;
   struct multifield *theList;

   /*===================================*/
   /* Get the segment to be subdivided. */
   /*===================================*/

   if (EnvArgTypeCheck(theEnv,"rest$",1,MULTIFIELD,&theValue) == FALSE)
     {
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      return;
     }

   theList = (struct multifield *) DOToPointer(theValue);

   /*=========================*/
   /* Return the new segment. */
   /*=========================*/

   SetpType(returnValue,MULTIFIELD);
   SetpValue(returnValue,theList);
   if (GetDOBegin(theValue) > GetDOEnd(theValue))
     { SetpDOBegin(returnValue,GetDOBegin(theValue)); }
   else
     { SetpDOBegin(returnValue,GetDOBegin(theValue) + 1); }
   SetpDOEnd(returnValue,GetDOEnd(theValue));
  }

/*************************************/
/* NthFunction: H/L access routine   */
/*   for the nth$ function.          */
/*************************************/
globle void NthFunction(
  void *theEnv,
  DATA_OBJECT_PTR nth_value)
  {
   DATA_OBJECT value1, value2;
   struct multifield *elm_ptr;
   long long n; /* 6.04 Bug Fix */

   if (EnvArgCountCheck(theEnv,"nth$",EXACTLY,2) == -1)
	 {
	  SetpType(nth_value,SYMBOL);
	  SetpValue(nth_value,(void *) EnvAddSymbol(theEnv,"nil"));
	  return;
	 }

   if ((EnvArgTypeCheck(theEnv,"nth$",1,INTEGER,&value1) == FALSE) ||
	   (EnvArgTypeCheck(theEnv,"nth$",2,MULTIFIELD,&value2) == FALSE))
	 {
	  SetpType(nth_value,SYMBOL);
	  SetpValue(nth_value,(void *) EnvAddSymbol(theEnv,"nil"));
	  return;
	 }

   n = DOToLong(value1); /* 6.04 Bug Fix */
   if ((n > GetDOLength(value2)) || (n < 1))
	 {
	  SetpType(nth_value,SYMBOL);
	  SetpValue(nth_value,(void *) EnvAddSymbol(theEnv,"nil"));
	  return;
	 }

   elm_ptr = (struct multifield *) GetValue(value2);
   SetpType(nth_value,GetMFType(elm_ptr,((long) n) + GetDOBegin(value2) - 1));
   SetpValue(nth_value,GetMFValue(elm_ptr,((long) n) + GetDOBegin(value2) - 1));
  }

/* ------------------------------------------------------------------
 *    SubsetFunction:
 *               This function compares two multi-field variables
 *               to see if the first is a subset of the second. It
 *               does not consider order.
 *
 *    INPUTS:    Two arguments via argument stack. First is the sublist
 *               multi-field variable, the second is the list to be
 *               compared to. Both should be of type MULTIFIELD.
 *
 *    OUTPUTS:   TRUE if the first list is a subset of the
 *               second, else FALSE
 *
 *    NOTES:     This function is called from H/L with the subset
 *               command. Repeated values in the sublist must also
 *               be repeated in the main list.
 * ------------------------------------------------------------------
 */

globle intBool SubsetpFunction(
  void *theEnv)
  {
   DATA_OBJECT item1, item2, tmpItem;
   long i,j,k; 

   if (EnvArgCountCheck(theEnv,"subsetp",EXACTLY,2) == -1)
     return(FALSE);

   if (EnvArgTypeCheck(theEnv,"subsetp",1,MULTIFIELD,&item1) == FALSE)
     return(FALSE);

   if (EnvArgTypeCheck(theEnv,"subsetp",2,MULTIFIELD,&item2) == FALSE)
     return(FALSE);

   if (GetDOLength(item1) == 0) return(TRUE);
   if (GetDOLength(item2) == 0) return(FALSE);

   for (i = GetDOBegin(item1) ; i <= GetDOEnd(item1) ; i++)
     {
      SetType(tmpItem,GetMFType((struct multifield *) GetValue(item1),i));
      SetValue(tmpItem,GetMFValue((struct multifield *) GetValue(item1),i));


      if (! FindDOsInSegment(&tmpItem,1,&item2,&j,&k,NULL,0))
        { return(FALSE); }
     }

   return(TRUE);
  }

/****************************************/
/* MemberFunction: H/L access routine   */
/*   for the member$ function.          */
/****************************************/
globle void MemberFunction(
  void *theEnv,
  DATA_OBJECT_PTR result)
  {
   DATA_OBJECT item1, item2;
   long j, k;

   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);

   if (EnvArgCountCheck(theEnv,"member$",EXACTLY,2) == -1) return;

   EnvRtnUnknown(theEnv,1,&item1);

   if (EnvArgTypeCheck(theEnv,"member$",2,MULTIFIELD,&item2) == FALSE) return;

   if (FindDOsInSegment(&item1,1,&item2,&j,&k,NULL,0))
     {
      if (j == k)
        {
         result->type = INTEGER;
         result->value = (void *) EnvAddLong(theEnv,j);
        }
      else
        {
         result->type = MULTIFIELD;
         result->value = EnvCreateMultifield(theEnv,2);
         SetMFType(result->value,1,INTEGER);
         SetMFValue(result->value,1,EnvAddLong(theEnv,j));
         SetMFType(result->value,2,INTEGER);
         SetMFValue(result->value,2,EnvAddLong(theEnv,k));
         SetpDOBegin(result,1);
         SetpDOEnd(result,2);
        }
     }
  }

/***************************************/
/* FindDOsInSegment:                  */
/***************************************/
/* 6.05 Bug Fix */
intBool FindDOsInSegment(
  DATA_OBJECT_PTR searchDOs,
  int scnt,
  DATA_OBJECT_PTR value,
  long *si,
  long *ei,
  long *excludes,
  int epaircnt)
  {
   long mul_length,slen,i,k; /* 6.04 Bug Fix */
   int j;

   mul_length = GetpDOLength(value);
   for (i = 0 ; i < mul_length ; i++)
     {
      for (j = 0 ; j < scnt ; j++)
        {
         if (GetType(searchDOs[j]) == MULTIFIELD)
           {
            slen = GetDOLength(searchDOs[j]);
            if (MVRangeCheck(i+1L,i+slen,excludes,epaircnt))
              {
               for (k = 0L ; (k < slen) && ((k + i) < mul_length) ; k++)
                 if ((GetMFType(GetValue(searchDOs[j]),k+GetDOBegin(searchDOs[j])) !=
                      GetMFType(GetpValue(value),k+i+GetpDOBegin(value))) ||
                     (GetMFValue(GetValue(searchDOs[j]),k+GetDOBegin(searchDOs[j])) !=
                      GetMFValue(GetpValue(value),k+i+GetpDOBegin(value))))
                   break;
               if (k >= slen)
                 {
                  *si = i + 1L;
                  *ei = i + slen;
                  return(TRUE);
                 }
              }
           }
         else if ((GetValue(searchDOs[j]) == GetMFValue(GetpValue(value),i + GetpDOBegin(value))) &&
                  (GetType(searchDOs[j]) == GetMFType(GetpValue(value),i + GetpDOBegin(value))) &&
                  MVRangeCheck(i+1L,i+1L,excludes,epaircnt))
           {
            *si = *ei = i+1L;
            return(TRUE);
           }
        }
     }

   return(FALSE);
  }

/******************************************************/
/* MVRangeCheck:  */
/******************************************************/
static intBool MVRangeCheck(
  long si,
  long ei,
  long *elist,
  int epaircnt)
{
  int i;

  if (!elist || !epaircnt)
    return(TRUE);
  for (i = 0 ; i < epaircnt ; i++)
    if (((si >= elist[i*2]) && (si <= elist[i*2+1])) ||
        ((ei >= elist[i*2]) && (ei <= elist[i*2+1])))
    return(FALSE);

  return(TRUE);
}

#if (! BLOAD_ONLY) && (! RUN_TIME)

/******************************************************/
/* MultifieldPrognParser: Parses the progn$ function. */
/******************************************************/
static struct expr *MultifieldPrognParser(
  void *theEnv,
  struct expr *top,
  char *infile)
  {
   struct BindInfo *oldBindList,*newBindList,*prev;
   struct token tkn;
   struct expr *tmp;
   SYMBOL_HN *fieldVar = NULL;

   SavePPBuffer(theEnv," ");
   GetToken(theEnv,infile,&tkn);

   /* ================================
      Simple form: progn$ <mf-exp> ...
      ================================ */
   if (tkn.type != LPAREN)
     {
      top->argList = ParseAtomOrExpression(theEnv,infile,&tkn);
      if (top->argList == NULL)
        {
         ReturnExpression(theEnv,top);
         return(NULL);
        }
     }
   else
     {
      GetToken(theEnv,infile,&tkn);
      if (tkn.type != SF_VARIABLE)
        {
         if (tkn.type != SYMBOL)
           goto MvPrognParseError;
         top->argList = Function2Parse(theEnv,infile,ValueToString(tkn.value));
         if (top->argList == NULL)
           {
            ReturnExpression(theEnv,top);
            return(NULL);
           }
        }

      /* =========================================
         Complex form: progn$ (<var> <mf-exp>) ...
         ========================================= */
      else
        {
         fieldVar = (SYMBOL_HN *) tkn.value;
         SavePPBuffer(theEnv," ");
         top->argList = ParseAtomOrExpression(theEnv,infile,NULL);
         if (top->argList == NULL)
           {
            ReturnExpression(theEnv,top);
            return(NULL);
           }
         GetToken(theEnv,infile,&tkn);
         if (tkn.type != RPAREN)
           goto MvPrognParseError;
         PPBackup(theEnv);
         /* PPBackup(theEnv); */
         SavePPBuffer(theEnv,tkn.printForm);
         SavePPBuffer(theEnv," ");
        }
     }

   if (CheckArgumentAgainstRestriction(theEnv,top->argList,(int) 'm'))
     goto MvPrognParseError;
   oldBindList = GetParsedBindNames(theEnv);
   SetParsedBindNames(theEnv,NULL);
   IncrementIndentDepth(theEnv,3);
   ExpressionData(theEnv)->BreakContext = TRUE;
   ExpressionData(theEnv)->ReturnContext = ExpressionData(theEnv)->svContexts->rtn;
   PPCRAndIndent(theEnv);
   top->argList->nextArg = GroupActions(theEnv,infile,&tkn,TRUE,NULL,FALSE);
   DecrementIndentDepth(theEnv,3);
   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,tkn.printForm);
   if (top->argList->nextArg == NULL)
     {
      ClearParsedBindNames(theEnv);
      SetParsedBindNames(theEnv,oldBindList);
      ReturnExpression(theEnv,top);
      return(NULL);
     }
   tmp = top->argList->nextArg;
   top->argList->nextArg = tmp->argList;
   tmp->argList = NULL;
   ReturnExpression(theEnv,tmp);
   newBindList = GetParsedBindNames(theEnv);
   prev = NULL;
   while (newBindList != NULL)
     {
      if ((fieldVar == NULL) ? FALSE :
          (strcmp(ValueToString(newBindList->name),ValueToString(fieldVar)) == 0))
        {
         ClearParsedBindNames(theEnv);
         SetParsedBindNames(theEnv,oldBindList);
         PrintErrorID(theEnv,"MULTIFUN",2,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Cannot rebind field variable in function progn$.\n");
         ReturnExpression(theEnv,top);
         return(NULL);
        }
      prev = newBindList;
      newBindList = newBindList->next;
     }
   if (prev == NULL)
     SetParsedBindNames(theEnv,oldBindList);
   else
     prev->next = oldBindList;
   if (fieldVar != NULL)
     ReplaceMvPrognFieldVars(theEnv,fieldVar,top->argList->nextArg,0);
   return(top);

MvPrognParseError:
   SyntaxErrorMessage(theEnv,"progn$");
   ReturnExpression(theEnv,top);
   return(NULL);
  }

/******************************************************/
/* ForeachParser: Parses the foreach function. */
/******************************************************/
static struct expr *ForeachParser(
  void *theEnv,
  struct expr *top,
  char *infile)
  {
   struct BindInfo *oldBindList,*newBindList,*prev;
   struct token tkn;
   struct expr *tmp;
   SYMBOL_HN *fieldVar;

   SavePPBuffer(theEnv," ");
   GetToken(theEnv,infile,&tkn);

   if (tkn.type != SF_VARIABLE)
     { goto ForeachParseError; }

   fieldVar = (SYMBOL_HN *) tkn.value;
   SavePPBuffer(theEnv," ");
   top->argList = ParseAtomOrExpression(theEnv,infile,NULL);
   if (top->argList == NULL)
     {
      ReturnExpression(theEnv,top);
      return(NULL);
     }

   if (CheckArgumentAgainstRestriction(theEnv,top->argList,(int) 'm'))
     goto ForeachParseError;
   oldBindList = GetParsedBindNames(theEnv);
   SetParsedBindNames(theEnv,NULL);
   IncrementIndentDepth(theEnv,3);
   ExpressionData(theEnv)->BreakContext = TRUE;
   ExpressionData(theEnv)->ReturnContext = ExpressionData(theEnv)->svContexts->rtn;
   PPCRAndIndent(theEnv);
   top->argList->nextArg = GroupActions(theEnv,infile,&tkn,TRUE,NULL,FALSE);
   DecrementIndentDepth(theEnv,3);
   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,tkn.printForm);
   if (top->argList->nextArg == NULL)
     {
      ClearParsedBindNames(theEnv);
      SetParsedBindNames(theEnv,oldBindList);
      ReturnExpression(theEnv,top);
      return(NULL);
     }
   tmp = top->argList->nextArg;
   top->argList->nextArg = tmp->argList;
   tmp->argList = NULL;
   ReturnExpression(theEnv,tmp);
   newBindList = GetParsedBindNames(theEnv);
   prev = NULL;
   while (newBindList != NULL)
     {
      if ((fieldVar == NULL) ? FALSE :
          (strcmp(ValueToString(newBindList->name),ValueToString(fieldVar)) == 0))
        {
         ClearParsedBindNames(theEnv);
         SetParsedBindNames(theEnv,oldBindList);
         PrintErrorID(theEnv,"MULTIFUN",2,FALSE);
         EnvPrintRouter(theEnv,WERROR,"Cannot rebind field variable in function foreach.\n");
         ReturnExpression(theEnv,top);
         return(NULL);
        }
      prev = newBindList;
      newBindList = newBindList->next;
     }
   if (prev == NULL)
     SetParsedBindNames(theEnv,oldBindList);
   else
     prev->next = oldBindList;
   if (fieldVar != NULL)
     ReplaceMvPrognFieldVars(theEnv,fieldVar,top->argList->nextArg,0);
   return(top);

ForeachParseError:
   SyntaxErrorMessage(theEnv,"foreach");
   ReturnExpression(theEnv,top);
   return(NULL);
  }
  
/**********************************************/
/* ReplaceMvPrognFieldVars: Replaces variable */
/*   references found in the progn$ function. */
/**********************************************/
static void ReplaceMvPrognFieldVars(
  void *theEnv,
  SYMBOL_HN *fieldVar,
  struct expr *theExp,
  int depth)
  {
   size_t flen;

   flen = strlen(ValueToString(fieldVar));
   while (theExp != NULL)
     {
      if ((theExp->type != SF_VARIABLE) ? FALSE :
          (strncmp(ValueToString(theExp->value),ValueToString(fieldVar),
                   (STD_SIZE) flen) == 0))
        {
         if (ValueToString(theExp->value)[flen] == '\0')
           {
            theExp->type = FCALL;
            theExp->value = (void *) FindFunction(theEnv,"(get-progn$-field)");
            theExp->argList = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,(long long) depth));
           }
         else if (strcmp(ValueToString(theExp->value) + flen,"-index") == 0)
           {
            theExp->type = FCALL;
            theExp->value = (void *) FindFunction(theEnv,"(get-progn$-index)");
            theExp->argList = GenConstant(theEnv,INTEGER,EnvAddLong(theEnv,(long long) depth));
           }
        }
      else if (theExp->argList != NULL)
        {
         if ((theExp->type == FCALL) && (theExp->value == (void *) FindFunction(theEnv,"progn$")))
           ReplaceMvPrognFieldVars(theEnv,fieldVar,theExp->argList,depth+1);
         else
           ReplaceMvPrognFieldVars(theEnv,fieldVar,theExp->argList,depth);
        }
      theExp = theExp->nextArg;
     }
  }

#endif

/*****************************************/
/* MultifieldPrognFunction: H/L access   */
/*   routine for the progn$ function.    */
/*****************************************/
globle void MultifieldPrognFunction(
  void *theEnv,
  DATA_OBJECT_PTR result)
  {
   MultifieldPrognDriver(theEnv,result,"progn$");
  }

/***************************************/
/* ForeachFunction: H/L access routine */
/*   for the foreach function.         */
/***************************************/
globle void ForeachFunction(
  void *theEnv,
  DATA_OBJECT_PTR result)
  {
   MultifieldPrognDriver(theEnv,result,"foreach");
  }
    
/*******************************************/
/* MultifieldPrognDriver: Driver routine   */
/*   for the progn$ and foreach functions. */
/******************************************/
static void MultifieldPrognDriver(
  void *theEnv,
  DATA_OBJECT_PTR result,
  char *functionName)
  {
   EXPRESSION *theExp;
   DATA_OBJECT argval;
   long i, end; /* 6.04 Bug Fix */
   FIELD_VAR_STACK *tmpField;

   tmpField = get_struct(theEnv,fieldVarStack);
   tmpField->type = SYMBOL;
   tmpField->value = EnvFalseSymbol(theEnv);
   tmpField->nxt = MultiFunctionData(theEnv)->FieldVarStack;
   MultiFunctionData(theEnv)->FieldVarStack = tmpField;
   result->type = SYMBOL;
   result->value = EnvFalseSymbol(theEnv);
   if (EnvArgTypeCheck(theEnv,functionName,1,MULTIFIELD,&argval) == FALSE)
     {
      MultiFunctionData(theEnv)->FieldVarStack = tmpField->nxt;
      rtn_struct(theEnv,fieldVarStack,tmpField);
      return;
     }
   ValueInstall(theEnv,&argval);
   end = GetDOEnd(argval);
   for (i = GetDOBegin(argval) ; i <= end ; i++)
     {
      tmpField->type = GetMFType(argval.value,i);
      tmpField->value = GetMFValue(argval.value,i);
      /* tmpField->index = i; */
      tmpField->index = (i - GetDOBegin(argval)) + 1; 
      for (theExp = GetFirstArgument()->nextArg ; theExp != NULL ; theExp = theExp->nextArg)
        {
         EvaluationData(theEnv)->CurrentEvaluationDepth++;
         EvaluateExpression(theEnv,theExp,result);
         EvaluationData(theEnv)->CurrentEvaluationDepth--;
         if (ProcedureFunctionData(theEnv)->ReturnFlag == TRUE)
           { PropagateReturnValue(theEnv,result); }
         PeriodicCleanup(theEnv,FALSE,TRUE);

         if (EvaluationData(theEnv)->HaltExecution || ProcedureFunctionData(theEnv)->BreakFlag || ProcedureFunctionData(theEnv)->ReturnFlag)
           {
            ValueDeinstall(theEnv,&argval);
            ProcedureFunctionData(theEnv)->BreakFlag = FALSE;
            if (EvaluationData(theEnv)->HaltExecution)
              {
               result->type = SYMBOL;
               result->value = EnvFalseSymbol(theEnv);
              }
            MultiFunctionData(theEnv)->FieldVarStack = tmpField->nxt;
            rtn_struct(theEnv,fieldVarStack,tmpField);
            return;
           }
        }
     }
   ValueDeinstall(theEnv,&argval);
   ProcedureFunctionData(theEnv)->BreakFlag = FALSE;
   MultiFunctionData(theEnv)->FieldVarStack = tmpField->nxt;
   rtn_struct(theEnv,fieldVarStack,tmpField);
  }

/***************************************************/
/* GetMvPrognField                                 */
/***************************************************/
globle void GetMvPrognField(
  void *theEnv,
  DATA_OBJECT_PTR result)
  {
   int depth;
   FIELD_VAR_STACK *tmpField;

   depth = ValueToInteger(GetFirstArgument()->value);
   tmpField = MultiFunctionData(theEnv)->FieldVarStack;
   while (depth > 0)
     {
      tmpField = tmpField->nxt;
      depth--;
     }
   result->type = tmpField->type;
   result->value = tmpField->value;
  }

/***************************************************/
/* GetMvPrognIndex                                 */
/***************************************************/
globle long GetMvPrognIndex(
  void *theEnv)
  {
   int depth;
   FIELD_VAR_STACK *tmpField;

   depth = ValueToInteger(GetFirstArgument()->value);
   tmpField = MultiFunctionData(theEnv)->FieldVarStack;
   while (depth > 0)
     {
      tmpField = tmpField->nxt;
      depth--;
     }
   return(tmpField->index);
  }

#endif

#if OBJECT_SYSTEM || MULTIFIELD_FUNCTIONS

/**************************************************************************
  NAME         : ReplaceMultiValueField
  DESCRIPTION  : Performs a replace on the src multi-field value
                   storing the results in the dst multi-field value
  INPUTS       : 1) The destination value buffer
                 2) The source value (can be NULL)
                 3) Beginning of index range
                 4) End of range
                 5) The new field value
  RETURNS      : TRUE if successful, FALSE otherwise
  SIDE EFFECTS : Allocates and sets a ephemeral segment (even if new
                   number of fields is 0)
                 Src value segment is not changed
  NOTES        : index is NOT guaranteed to be valid
                 src is guaranteed to be a multi-field variable or NULL
 **************************************************************************/
globle int ReplaceMultiValueField(
  void *theEnv,
  DATA_OBJECT *dst,
  DATA_OBJECT *src,
  long rb,
  long re,
  DATA_OBJECT *field,
  char *funcName)
  {
   long i,j,k;
   struct field *deptr;
   struct field *septr;
   long srclen,dstlen;

   srclen = ((src != NULL) ? (src->end - src->begin + 1) : 0);
   if ((re < rb) ||
	   (rb < 1) || (re < 1) ||
	   (rb > srclen) || (re > srclen))
	 {
	  MVRangeError(theEnv,rb,re,srclen,funcName);
	  return(FALSE);
	 }
   rb = src->begin + rb - 1;
   re = src->begin + re - 1;
   if (field->type == MULTIFIELD)
	 dstlen = srclen + GetpDOLength(field) - (re-rb+1);
   else
	 dstlen = srclen + 1 - (re-rb+1);
   dst->type = MULTIFIELD;
   dst->begin = 0;
   dst->value = EnvCreateMultifield(theEnv,dstlen);
   SetpDOEnd(dst,dstlen);
   for (i = 0 , j = src->begin ; j < rb ; i++ , j++)
	 {
	  deptr = &((struct multifield *) dst->value)->theFields[i];
	  septr = &((struct multifield *) src->value)->theFields[j];
	  deptr->type = septr->type;
	  deptr->value = septr->value;
	 }
   if (field->type != MULTIFIELD)
	 {
	  deptr = &((struct multifield *) dst->value)->theFields[i++];
	  deptr->type = field->type;
	  deptr->value = field->value;
	 }
   else
	 {
	  for (k = field->begin ; k <= field->end ; k++ , i++)
		{
		 deptr = &((struct multifield *) dst->value)->theFields[i];
		 septr = &((struct multifield *) field->value)->theFields[k];
		 deptr->type = septr->type;
		 deptr->value = septr->value;
		}
	 }
   while (j < re)
	 j++;
   for (j++ ; i < dstlen ; i++ , j++)
	 {
	  deptr = &((struct multifield *) dst->value)->theFields[i];
	  septr = &((struct multifield *) src->value)->theFields[j];
	  deptr->type = septr->type;
	  deptr->value = septr->value;
	 }
   return(TRUE);
  }

/**************************************************************************
  NAME         : InsertMultiValueField
  DESCRIPTION  : Performs an insert on the src multi-field value
                   storing the results in the dst multi-field value
  INPUTS       : 1) The destination value buffer
                 2) The source value (can be NULL)
                 3) The index for the change
                 4) The new field value
  RETURNS      : TRUE if successful, FALSE otherwise
  SIDE EFFECTS : Allocates and sets a ephemeral segment (even if new
                   number of fields is 0)
                 Src value segment is not changed
  NOTES        : index is NOT guaranteed to be valid
                 src is guaranteed to be a multi-field variable or NULL
 **************************************************************************/
globle int InsertMultiValueField(
  void *theEnv,
  DATA_OBJECT *dst,
  DATA_OBJECT *src,
  long theIndex,
  DATA_OBJECT *field,
  char *funcName)
  {
   long i,j,k;
   register FIELD *deptr, *septr;
   long srclen,dstlen;

   srclen = (long) ((src != NULL) ? (src->end - src->begin + 1) : 0);
   if (theIndex < 1)
     {
      MVRangeError(theEnv,theIndex,theIndex,srclen+1,funcName);
      return(FALSE);
     }
   if (theIndex > (srclen + 1))
     theIndex = (srclen + 1);
   dst->type = MULTIFIELD;
   dst->begin = 0;
   if (src == NULL)
     {
      if (field->type == MULTIFIELD)
        {
         DuplicateMultifield(theEnv,dst,field);
         AddToMultifieldList(theEnv,(struct multifield *) dst->value);
        }
      else
        {
         dst->value = EnvCreateMultifield(theEnv,0L);
         dst->end = 0;
         deptr = &((struct multifield *) dst->value)->theFields[0];
         deptr->type = field->type;
         deptr->value = field->value;
        }
      return(TRUE);
     }
   dstlen = (field->type == MULTIFIELD) ? GetpDOLength(field) + srclen : srclen + 1;
   dst->value = EnvCreateMultifield(theEnv,dstlen);
   SetpDOEnd(dst,dstlen);
   theIndex--;
   for (i = 0 , j = src->begin ; i < theIndex ; i++ , j++)
     {
      deptr = &((struct multifield *) dst->value)->theFields[i];
      septr = &((struct multifield *) src->value)->theFields[j];
      deptr->type = septr->type;
      deptr->value = septr->value;
     }
   if (field->type != MULTIFIELD)
     {
      deptr = &((struct multifield *) dst->value)->theFields[theIndex];
      deptr->type = field->type;
      deptr->value = field->value;
      i++;
     }
   else
     {
      for (k = field->begin ; k <= field->end ; k++ , i++)
        {
         deptr = &((struct multifield *) dst->value)->theFields[i];
         septr = &((struct multifield *) field->value)->theFields[k];
         deptr->type = septr->type;
         deptr->value = septr->value;
        }
     }
   for ( ; j <= src->end ; i++ , j++)
     {
      deptr = &((struct multifield *) dst->value)->theFields[i];
      septr = &((struct multifield *) src->value)->theFields[j];
      deptr->type = septr->type;
      deptr->value = septr->value;
     }
   return(TRUE);
  }

/*******************************************************
  NAME         : MVRangeError
  DESCRIPTION  : Prints out an error messages for index
                   out-of-range errors in multi-field
                   access functions
  INPUTS       : 1) The bad range start
                 2) The bad range end
                 3) The max end of the range (min is
                     assumed to be 1)
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 ******************************************************/
static void MVRangeError(
  void *theEnv,
  long brb,
  long bre,
  long max,
  char *funcName)
  {
   PrintErrorID(theEnv,"MULTIFUN",1,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Multifield index ");
   if (brb == bre)
     PrintLongInteger(theEnv,WERROR,(long long) brb);
   else
     {
      EnvPrintRouter(theEnv,WERROR,"range ");
      PrintLongInteger(theEnv,WERROR,(long long) brb);
      EnvPrintRouter(theEnv,WERROR,"..");
      PrintLongInteger(theEnv,WERROR,(long long) bre);
     }
   EnvPrintRouter(theEnv,WERROR," out of range 1..");
   PrintLongInteger(theEnv,WERROR,(long long) max);
   if (funcName != NULL)
     {
      EnvPrintRouter(theEnv,WERROR," in function ");
      EnvPrintRouter(theEnv,WERROR,funcName);
     }
   EnvPrintRouter(theEnv,WERROR,".\n");
  }

/**************************************************************************
  NAME         : DeleteMultiValueField
  DESCRIPTION  : Performs a modify on the src multi-field value
                   storing the results in the dst multi-field value
  INPUTS       : 1) The destination value buffer
                 2) The source value (can be NULL)
                 3) The beginning index for deletion
                 4) The ending index for deletion
  RETURNS      : TRUE if successful, FALSE otherwise
  SIDE EFFECTS : Allocates and sets a ephemeral segment (even if new
                   number of fields is 0)
                 Src value segment is not changed
  NOTES        : index is NOT guaranteed to be valid
                 src is guaranteed to be a multi-field variable or NULL
 **************************************************************************/
globle int DeleteMultiValueField(
  void *theEnv,
  DATA_OBJECT *dst,
  DATA_OBJECT *src,
  long rb,
  long re,
  char *funcName)
  {
   long i,j;
   register FIELD_PTR deptr,septr;
   long srclen, dstlen;

   srclen = (long) ((src != NULL) ? (src->end - src->begin + 1) : 0);
   if ((re < rb) ||
       (rb < 1) || (re < 1) ||
       (rb > srclen) || (re > srclen))
     {
      MVRangeError(theEnv,rb,re,srclen,funcName);
      return(FALSE);
     }
   dst->type = MULTIFIELD;
   dst->begin = 0;
   if (srclen == 0)
    {
     dst->value = EnvCreateMultifield(theEnv,0L);
     dst->end = -1;
     return(TRUE);
    }
   rb = src->begin + rb -1;
   re = src->begin + re -1;
   dstlen = srclen-(re-rb+1);
   SetpDOEnd(dst,dstlen);
   dst->value = EnvCreateMultifield(theEnv,dstlen);
   for (i = 0 , j = src->begin ; j < rb ; i++ , j++)
     {
      deptr = &((struct multifield *) dst->value)->theFields[i];
      septr = &((struct multifield *) src->value)->theFields[j];
      deptr->type = septr->type;
      deptr->value = septr->value;
     }
   while (j < re)
     j++;
   for (j++ ; i <= dst->end ; j++ , i++)
     {
      deptr = &((struct multifield *) dst->value)->theFields[i];
      septr = &((struct multifield *) src->value)->theFields[j];
      deptr->type = septr->type;
      deptr->value = septr->value;
     }
   return(TRUE);
  }

#endif
