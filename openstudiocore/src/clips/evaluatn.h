   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*               EVALUATION HEADER FILE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for evaluating expressions.    */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Added EvaluateAndStoreInDataObject function.   */
/*                                                           */
/*************************************************************/

#ifndef _H_evaluatn

#define _H_evaluatn

struct entityRecord;
struct dataObject;

#ifndef _H_constant
#include "constant.h"
#endif
#ifndef _H_symbol
#include "symbol.h"
#endif
#ifndef _H_expressn
#include "expressn.h"
#endif

struct dataObject
  {
   void *supplementalInfo;
   unsigned short type;
   void *value;
   long begin;
   long end;
   struct dataObject *next;
  };

typedef struct dataObject DATA_OBJECT;
typedef struct dataObject * DATA_OBJECT_PTR;

typedef struct expr FUNCTION_REFERENCE;

#define DATA_OBJECT_PTR_ARG DATA_OBJECT_PTR

#include "userdata.h"

struct entityRecord
  {
   char *name;
   unsigned int type : 13;
   unsigned int copyToEvaluate : 1;
   unsigned int bitMap : 1;
   unsigned int addsToRuleComplexity : 1;
   void (*shortPrintFunction)(void *,char *,void *);
   void (*longPrintFunction)(void *,char *,void *);
   intBool (*deleteFunction)(void *,void *);
   intBool (*evaluateFunction)(void *,void *,DATA_OBJECT *);
   void *(*getNextFunction)(void *,void *);
   void (*decrementBusyCount)(void *,void *);
   void (*incrementBusyCount)(void *,void *);
   void (*propagateDepth)(void *,void *);
   void (*markNeeded)(void *,void *);
   void (*install)(void *,void *);
   void (*deinstall)(void *,void *);
   struct userData *usrData;
  };

struct externalAddressType
  {
   char *name;
   void (*shortPrintFunction)(void *,char *,void *);
   void (*longPrintFunction)(void *,char *,void *);
   intBool (*discardFunction)(void *,void *);
   void (*newFunction)(void *,DATA_OBJECT *);
   intBool (*callFunction)(void *,DATA_OBJECT *,DATA_OBJECT *);
  };

typedef struct entityRecord ENTITY_RECORD;
typedef struct entityRecord * ENTITY_RECORD_PTR;

#define GetDOLength(target)       (((target).end - (target).begin) + 1)
#define GetpDOLength(target)      (((target)->end - (target)->begin) + 1)
#define GetDOBegin(target)        ((target).begin + 1)
#define GetDOEnd(target)          ((target).end + 1)
#define GetpDOBegin(target)       ((target)->begin + 1)
#define GetpDOEnd(target)         ((target)->end + 1)
#define SetDOBegin(target,val)   ((target).begin = (long) ((val) - 1))
#define SetDOEnd(target,val)     ((target).end = (long) ((val) - 1))
#define SetpDOBegin(target,val)   ((target)->begin = (long) ((val) - 1))
#define SetpDOEnd(target,val)     ((target)->end = (long) ((val) - 1))

#define EnvGetDOLength(theEnv,target)       (((target).end - (target).begin) + 1)
#define EnvGetpDOLength(theEnv,target)      (((target)->end - (target)->begin) + 1)
#define EnvGetDOBegin(theEnv,target)        ((target).begin + 1)
#define EnvGetDOEnd(theEnv,target)          ((target).end + 1)
#define EnvGetpDOBegin(theEnv,target)       ((target)->begin + 1)
#define EnvGetpDOEnd(theEnv,target)         ((target)->end + 1)
#define EnvSetDOBegin(theEnv,target,val)   ((target).begin = (long) ((val) - 1))
#define EnvSetDOEnd(theEnv,target,val)     ((target).end = (long) ((val) - 1))
#define EnvSetpDOBegin(theEnv,target,val)   ((target)->begin = (long) ((val) - 1))
#define EnvSetpDOEnd(theEnv,target,val)     ((target)->end = (long) ((val) - 1))

#define DOPToString(target) (((struct symbolHashNode *) ((target)->value))->contents)
#define DOPToDouble(target) (((struct floatHashNode *) ((target)->value))->contents)
#define DOPToFloat(target) ((float) (((struct floatHashNode *) ((target)->value))->contents))
#define DOPToLong(target) (((struct integerHashNode *) ((target)->value))->contents)
#define DOPToInteger(target) ((int) (((struct integerHashNode *) ((target)->value))->contents))
#define DOPToPointer(target)       ((target)->value)
#define DOPToExternalAddress(target) (((struct externalAddressHashNode *) ((target)->value))->externalAddress)

#define EnvDOPToString(theEnv,target) (((struct symbolHashNode *) ((target)->value))->contents)
#define EnvDOPToDouble(theEnv,target) (((struct floatHashNode *) ((target)->value))->contents)
#define EnvDOPToFloat(theEnv,target) ((float) (((struct floatHashNode *) ((target)->value))->contents))
#define EnvDOPToLong(theEnv,target) (((struct integerHashNode *) ((target)->value))->contents)
#define EnvDOPToInteger(theEnv,target) ((int) (((struct integerHashNode *) ((target)->value))->contents))
#define EnvDOPToPointer(theEnv,target)       ((target)->value)
#define EnvDOPToExternalAddress(target) (((struct externalAddressHashNode *) ((target)->value))->externalAddress)

#define DOToString(target) (((struct symbolHashNode *) ((target).value))->contents)
#define DOToDouble(target) (((struct floatHashNode *) ((target).value))->contents)
#define DOToFloat(target) ((float) (((struct floatHashNode *) ((target).value))->contents))
#define DOToLong(target) (((struct integerHashNode *) ((target).value))->contents)
#define DOToInteger(target) ((int) (((struct integerHashNode *) ((target).value))->contents))
#define DOToPointer(target)        ((target).value)
#define DOToExternalAddress(target) (((struct externalAddressHashNode *) ((target).value))->externalAddress))

#define EnvDOToString(theEnv,target) (((struct symbolHashNode *) ((target).value))->contents)
#define EnvDOToDouble(theEnv,target) (((struct floatHashNode *) ((target).value))->contents)
#define EnvDOToFloat(theEnv,target) ((float) (((struct floatHashNode *) ((target).value))->contents))
#define EnvDOToLong(theEnv,target) (((struct integerHashNode *) ((target).value))->contents)
#define EnvDOToInteger(theEnv,target) ((int) (((struct integerHashNode *) ((target).value))->contents))
#define EnvDOToPointer(theEnv,target)        ((target).value)
#define EnvDOToExternalAddress(target) (((struct externalAddressHashNode *) ((target).value))->externalAddress))

#define CoerceToLongInteger(t,v) ((t == INTEGER) ? ValueToLong(v) : (long int) ValueToDouble(v))
#define CoerceToInteger(t,v) ((t == INTEGER) ? (int) ValueToLong(v) : (int) ValueToDouble(v))
#define CoerceToDouble(t,v) ((t == INTEGER) ? (double) ValueToLong(v) : ValueToDouble(v))

#define GetFirstArgument()           (EvaluationData(theEnv)->CurrentExpression->argList)
#define GetNextArgument(ep)          (ep->nextArg)

#define MAXIMUM_PRIMITIVES 150
#define MAXIMUM_EXTERNAL_ADDRESS_TYPES 10

#define BITS_PER_BYTE    8

#define BitwiseTest(n,b)   ((n) & (char) (1 << (b)))
#define BitwiseSet(n,b)    (n |= (char) (1 << (b)))
#define BitwiseClear(n,b)  (n &= (char) ~(1 << (b)))

#define TestBitMap(map,id)  BitwiseTest(map[(id) / BITS_PER_BYTE],(id) % BITS_PER_BYTE)
#define SetBitMap(map,id)   BitwiseSet(map[(id) / BITS_PER_BYTE],(id) % BITS_PER_BYTE)
#define ClearBitMap(map,id) BitwiseClear(map[(id) / BITS_PER_BYTE],(id) % BITS_PER_BYTE)

#define EVALUATION_DATA 44

struct evaluationData
  { 
   struct expr *CurrentExpression;
   int EvaluationError;
   int HaltExecution;
   int CurrentEvaluationDepth;
   int numberOfAddressTypes;
   struct entityRecord *PrimitivesArray[MAXIMUM_PRIMITIVES];
   struct externalAddressType *ExternalAddressTypes[MAXIMUM_EXTERNAL_ADDRESS_TYPES];
  };

#define EvaluationData(theEnv) ((struct evaluationData *) GetEnvironmentData(theEnv,EVALUATION_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _EVALUATN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define SetMultifieldErrorValue(a) EnvSetMultifieldErrorValue(GetCurrentEnvironment(),a)
#define FunctionCall(a,b,c) EnvFunctionCall(GetCurrentEnvironment(),a,b,c)

   LOCALE void                           InitializeEvaluationData(void *);
   LOCALE int                            EvaluateExpression(void *,struct expr *,struct dataObject *);
   LOCALE void                           SetEvaluationError(void *,intBool);
   LOCALE int                            GetEvaluationError(void *);
   LOCALE void                           SetHaltExecution(void *,int);
   LOCALE int                            GetHaltExecution(void *);
   LOCALE void                           ReturnValues(void *,struct dataObject *,intBool);
   LOCALE void                           PrintDataObject(void *,char *,struct dataObject *);
   LOCALE void                           EnvSetMultifieldErrorValue(void *,struct dataObject *);
   LOCALE void                           ValueInstall(void *,struct dataObject *);
   LOCALE void                           ValueDeinstall(void *,struct dataObject *);
   LOCALE void                           PropagateReturnValue(void *,struct dataObject *);
#if DEFFUNCTION_CONSTRUCT || DEFGENERIC_CONSTRUCT
   LOCALE int                            EnvFunctionCall(void *,char *,char *,DATA_OBJECT *);
   LOCALE int                            FunctionCall2(void *,FUNCTION_REFERENCE *,char *,DATA_OBJECT *);
#endif
   LOCALE void                           CopyDataObject(void *,DATA_OBJECT *,DATA_OBJECT *,int);
   LOCALE void                           AtomInstall(void *,int,void *);
   LOCALE void                           AtomDeinstall(void *,int,void *);
   LOCALE struct expr                   *ConvertValueToExpression(void *,DATA_OBJECT *);
   LOCALE unsigned long                  GetAtomicHashValue(unsigned short,void *,int);
   LOCALE void                           InstallPrimitive(void *,struct entityRecord *,int);
   LOCALE int                            InstallExternalAddressType(void *,struct externalAddressType *);
   LOCALE void                           TransferDataObjectValues(DATA_OBJECT *,DATA_OBJECT *);
   LOCALE struct expr                   *FunctionReferenceExpression(void *,char *);
   LOCALE intBool                        GetFunctionReference(void *,char *,FUNCTION_REFERENCE *);
   LOCALE intBool                        DOsEqual(DATA_OBJECT_PTR,DATA_OBJECT_PTR);
   LOCALE int                            EvaluateAndStoreInDataObject(void *,int,EXPRESSION *,DATA_OBJECT *,int);

#endif






