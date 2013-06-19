   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  07/01/05            */
   /*                                                     */
   /*                SORT FUNCTIONS MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for sorting functions.         */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: The sort function leaks memory when called     */
/*            with a multifield value of length zero.        */
/*            DR0864                                         */
/*                                                           */
/*************************************************************/

#define _SORTFUN_SOURCE_

#include "setup.h"

#include "argacces.h"
#include "dffnxfun.h"
#include "envrnmnt.h"
#include "evaluatn.h"
#include "extnfunc.h"
#include "memalloc.h"
#include "multifld.h"
#include "sysdep.h"

#include "sortfun.h"

#define SORTFUN_DATA 7

struct sortFunctionData
  { 
   struct expr *SortComparisonFunction;
  };

#define SortFunctionData(theEnv) ((struct sortFunctionData *) GetEnvironmentData(theEnv,SORTFUN_DATA))

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                    DoMergeSort(void *,DATA_OBJECT *,DATA_OBJECT *,unsigned long,
                                              unsigned long,unsigned long,unsigned long,
                                              int (*)(void *,DATA_OBJECT *,DATA_OBJECT *));
   static int                     DefaultCompareSwapFunction(void *,DATA_OBJECT *,DATA_OBJECT *);
   static void                    DeallocateSortFunctionData(void *);
   
/****************************************/
/* SortFunctionDefinitions: Initializes */
/*   the sorting functions.             */
/****************************************/
globle void SortFunctionDefinitions(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,SORTFUN_DATA,sizeof(struct sortFunctionData),DeallocateSortFunctionData);
#if ! RUN_TIME
   EnvDefineFunction2(theEnv,"sort",'u', PTIEF SortFunction,"SortFunction","1**w");
#endif
  }

/*******************************************************/
/* DeallocateSortFunctionData: Deallocates environment */
/*    data for the sort function.                      */
/*******************************************************/
static void DeallocateSortFunctionData(
  void *theEnv)
  {
   ReturnExpression(theEnv,SortFunctionData(theEnv)->SortComparisonFunction);
  }

/**************************************/
/* DefaultCompareSwapFunction:  */
/**************************************/
static int DefaultCompareSwapFunction(
  void *theEnv,
  DATA_OBJECT *item1,
  DATA_OBJECT *item2)
  {
   DATA_OBJECT returnValue;

   SortFunctionData(theEnv)->SortComparisonFunction->argList = GenConstant(theEnv,item1->type,item1->value);
   SortFunctionData(theEnv)->SortComparisonFunction->argList->nextArg = GenConstant(theEnv,item2->type,item2->value);
   ExpressionInstall(theEnv,SortFunctionData(theEnv)->SortComparisonFunction);
   EvaluateExpression(theEnv,SortFunctionData(theEnv)->SortComparisonFunction,&returnValue);
   ExpressionDeinstall(theEnv,SortFunctionData(theEnv)->SortComparisonFunction);
   ReturnExpression(theEnv,SortFunctionData(theEnv)->SortComparisonFunction->argList);
   SortFunctionData(theEnv)->SortComparisonFunction->argList = NULL;

   if ((GetType(returnValue) == SYMBOL) &&
       (GetValue(returnValue) == EnvFalseSymbol(theEnv)))
     { return(FALSE); }

   return(TRUE);
  }

/**************************************/
/* SortFunction: H/L access routine   */
/*   for the rest$ function.          */
/**************************************/
globle void SortFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   long argumentCount, i, j, k = 0;
   DATA_OBJECT *theArguments, *theArguments2;
   DATA_OBJECT theArg;
   struct multifield *theMultifield, *tempMultifield;
   char *functionName;
   struct expr *functionReference;
   int argumentSize = 0;
   struct FunctionDefinition *fptr;
#if DEFFUNCTION_CONSTRUCT
   DEFFUNCTION *dptr;
#endif

   /*==================================*/
   /* Set up the default return value. */
   /*==================================*/

   SetpType(returnValue,SYMBOL);
   SetpValue(returnValue,EnvFalseSymbol(theEnv));

   /*=============================================*/
   /* The function expects at least one argument. */
   /*=============================================*/

   if ((argumentCount = EnvArgCountCheck(theEnv,"sort",AT_LEAST,1)) == -1)
     { return; }

   /*=============================================*/
   /* Verify that the comparison function exists. */
   /*=============================================*/

   if (EnvArgTypeCheck(theEnv,"sort",1,SYMBOL,&theArg) == FALSE)
     { return; }

   functionName = DOToString(theArg);
   functionReference = FunctionReferenceExpression(theEnv,functionName);
   if (functionReference == NULL)
     {
      ExpectedTypeError1(theEnv,"sort",1,"function name, deffunction name, or defgeneric name");
      return;
     }

   /*======================================*/
   /* For an external function, verify the */
   /* correct number of arguments.         */
   /*======================================*/
   
   if (functionReference->type == FCALL)
     {
      fptr = (struct FunctionDefinition *) functionReference->value;
      if ((GetMinimumArgs(fptr) > 2) ||
          (GetMaximumArgs(fptr) == 0) ||
          (GetMaximumArgs(fptr) == 1))
        {
         ExpectedTypeError1(theEnv,"sort",1,"function name expecting two arguments");
         ReturnExpression(theEnv,functionReference);
         return;
        }
     }
     
   /*=======================================*/
   /* For a deffunction, verify the correct */
   /* number of arguments.                  */
   /*=======================================*/
  
#if DEFFUNCTION_CONSTRUCT
   if (functionReference->type == PCALL)
     {
      dptr = (DEFFUNCTION *) functionReference->value;
      if ((dptr->minNumberOfParameters > 2) ||
          (dptr->maxNumberOfParameters == 0) ||
          (dptr->maxNumberOfParameters == 1))
        {
         ExpectedTypeError1(theEnv,"sort",1,"deffunction name expecting two arguments");
         ReturnExpression(theEnv,functionReference);
         return;
        }
     }
#endif

   /*=====================================*/
   /* If there are no items to be sorted, */
   /* then return an empty multifield.    */
   /*=====================================*/

   if (argumentCount == 1)
     {
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      ReturnExpression(theEnv,functionReference);
      return;
     }
     
   /*=====================================*/
   /* Retrieve the arguments to be sorted */
   /* and determine how many there are.   */
   /*=====================================*/

   theArguments = (DATA_OBJECT *) genalloc(theEnv,(argumentCount - 1) * sizeof(DATA_OBJECT));

   for (i = 2; i <= argumentCount; i++)
     {
      EnvRtnUnknown(theEnv,i,&theArguments[i-2]);
      if (GetType(theArguments[i-2]) == MULTIFIELD)
        { argumentSize += GetpDOLength(&theArguments[i-2]); }
      else
        { argumentSize++; }
     }
     
   if (argumentSize == 0)
     {   
      genfree(theEnv,theArguments,(argumentCount - 1) * sizeof(DATA_OBJECT)); /* Bug Fix */
      EnvSetMultifieldErrorValue(theEnv,returnValue);
      ReturnExpression(theEnv,functionReference);
      return;
     }
   
   /*====================================*/
   /* Pack all of the items to be sorted */
   /* into a data object array.          */
   /*====================================*/
   
   theArguments2 = (DATA_OBJECT *) genalloc(theEnv,argumentSize * sizeof(DATA_OBJECT));

   for (i = 2; i <= argumentCount; i++)
     {
      if (GetType(theArguments[i-2]) == MULTIFIELD)
        {
         tempMultifield = (struct multifield *) GetValue(theArguments[i-2]);
         for (j = GetDOBegin(theArguments[i-2]); j <= GetDOEnd(theArguments[i-2]); j++, k++)
           {
            SetType(theArguments2[k],GetMFType(tempMultifield,j));
            SetValue(theArguments2[k],GetMFValue(tempMultifield,j));
           }
        }
      else
        {
         SetType(theArguments2[k],GetType(theArguments[i-2]));
         SetValue(theArguments2[k],GetValue(theArguments[i-2]));
         k++;
        }
     }
     
   genfree(theEnv,theArguments,(argumentCount - 1) * sizeof(DATA_OBJECT));

   functionReference->nextArg = SortFunctionData(theEnv)->SortComparisonFunction;
   SortFunctionData(theEnv)->SortComparisonFunction = functionReference;

   for (i = 0; i < argumentSize; i++)
     { ValueInstall(theEnv,&theArguments2[i]); }

   MergeSort(theEnv,(unsigned long) argumentSize,theArguments2,DefaultCompareSwapFunction);
  
   for (i = 0; i < argumentSize; i++)
     { ValueDeinstall(theEnv,&theArguments2[i]); }

   SortFunctionData(theEnv)->SortComparisonFunction = SortFunctionData(theEnv)->SortComparisonFunction->nextArg;
   functionReference->nextArg = NULL;
   ReturnExpression(theEnv,functionReference);

   theMultifield = (struct multifield *) EnvCreateMultifield(theEnv,(unsigned long) argumentSize);

   for (i = 0; i < argumentSize; i++)
     {
      SetMFType(theMultifield,i+1,GetType(theArguments2[i]));
      SetMFValue(theMultifield,i+1,GetValue(theArguments2[i]));
     }
     
   genfree(theEnv,theArguments2,argumentSize * sizeof(DATA_OBJECT));

   SetpType(returnValue,MULTIFIELD);
   SetpDOBegin(returnValue,1);
   SetpDOEnd(returnValue,argumentSize);
   SetpValue(returnValue,(void *) theMultifield);
  }


/*******************************************/
/* MergeSort: Sorts a list of fields       */
/*   according to user specified criteria. */
/*******************************************/
void MergeSort(
  void *theEnv,
  unsigned long listSize,
  DATA_OBJECT *theList,
  int (*swapFunction)(void *,DATA_OBJECT *,DATA_OBJECT  *))
  {
   DATA_OBJECT *tempList;
   unsigned long middle;

   if (listSize <= 1) return;

   /*==============================*/
   /* Create the temporary storage */
   /* needed for the merge sort.   */
   /*==============================*/

   tempList = (DATA_OBJECT *) genalloc(theEnv,listSize * sizeof(DATA_OBJECT));

   /*=====================================*/
   /* Call the merge sort driver routine. */
   /*=====================================*/

   middle = (listSize + 1) / 2;
   DoMergeSort(theEnv,theList,tempList,0,middle-1,middle,listSize - 1,swapFunction);

   /*==================================*/
   /* Deallocate the temporary storage */
   /* needed by the merge sort.        */
   /*==================================*/

   genfree(theEnv,tempList,listSize * sizeof(DATA_OBJECT));
  }


/******************************************************/
/* DoMergeSort: Driver routine for performing a merge */
/*   sort on an array of DATA_OBJECT structures.      */
/******************************************************/
static void DoMergeSort(
  void *theEnv,
  DATA_OBJECT *theList,
  DATA_OBJECT *tempList,
  unsigned long s1,
  unsigned long e1,
  unsigned long s2,
  unsigned long e2,
  int (*swapFunction)(void *,DATA_OBJECT *,DATA_OBJECT *))
  {
   DATA_OBJECT temp;
   unsigned long middle, size;
   unsigned long c1, c2, mergePoint;

   /* Sort the two subareas before merging them. */

   if (s1 == e1)
     { /* List doesn't need to be merged. */ }
   else if ((s1 + 1) == e1)
     {
      if ((*swapFunction)(theEnv,&theList[s1],&theList[e1]))
        {
         TransferDataObjectValues(&temp,&theList[s1]);
         TransferDataObjectValues(&theList[s1],&theList[e1]);
         TransferDataObjectValues(&theList[e1],&temp);
        }
     }
   else
     {
      size = ((e1 - s1) + 1);
      middle = s1 + ((size + 1) / 2);
      DoMergeSort(theEnv,theList,tempList,s1,middle-1,middle,e1,swapFunction);
     }

   if (s2 == e2)
     { /* List doesn't need to be merged. */ }
   else if ((s2 + 1) == e2)
     {
      if ((*swapFunction)(theEnv,&theList[s2],&theList[e2]))
        {
         TransferDataObjectValues(&temp,&theList[s2]);
         TransferDataObjectValues(&theList[s2],&theList[e2]);
         TransferDataObjectValues(&theList[e2],&temp);
        }
     }
   else
     {
      size = ((e2 - s2) + 1);
      middle = s2 + ((size + 1) / 2);
      DoMergeSort(theEnv,theList,tempList,s2,middle-1,middle,e2,swapFunction);
     }

   /*======================*/
   /* Merge the two areas. */
   /*======================*/

   mergePoint = s1;
   c1 = s1;
   c2 = s2;

   while (mergePoint <= e2)
     {
      if (c1 > e1)
        {
         TransferDataObjectValues(&tempList[mergePoint],&theList[c2]);
         c2++;
         mergePoint++;
        }
      else if (c2 > e2)
        {
         TransferDataObjectValues(&tempList[mergePoint],&theList[c1]);
         c1++;
         mergePoint++;
        }
      else if ((*swapFunction)(theEnv,&theList[c1],&theList[c2]))
        {
         TransferDataObjectValues(&tempList[mergePoint],&theList[c2]);
         c2++;
         mergePoint++;
        }
      else
        {
         TransferDataObjectValues(&tempList[mergePoint],&theList[c1]);
         c1++;
         mergePoint++;
        }
     }

   /*=======================================*/
   /* Copy them back to the original array. */
   /*=======================================*/

   for (c1 = s1; c1 <= e2; c1++)
     { TransferDataObjectValues(&theList[c1],&tempList[c1]); }
  }



