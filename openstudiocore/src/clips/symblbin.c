   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*                 SYMBOL BSAVE MODULE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the binary save/load feature for      */
/*    atomic data values.                                    */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#define _BSAVE_SOURCE_

#include "setup.h"

#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE || BLOAD_INSTANCES || BSAVE_INSTANCES

#include "argacces.h"
#include "bload.h"
#include "bsave.h"
#include "cstrnbin.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "memalloc.h"
#include "moduldef.h"
#include "router.h"

#include "symblbin.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                        ReadNeededBitMaps(void *);
#if BLOAD_AND_BSAVE || BSAVE_INSTANCES
   static void                        WriteNeededBitMaps(void *,FILE *);
#endif

#if BLOAD_AND_BSAVE || BSAVE_INSTANCES

/**********************************************/
/* WriteNeededAtomicValues: Save all symbols, */
/*   floats, integers, and bitmaps needed by  */
/*   this binary image to the binary file.    */
/**********************************************/
globle void WriteNeededAtomicValues(
  void *theEnv,
  FILE *fp)
  {
   WriteNeededSymbols(theEnv,fp);
   WriteNeededFloats(theEnv,fp);
   WriteNeededIntegers(theEnv,fp);
   WriteNeededBitMaps(theEnv,fp);
  }

/********************************************************/
/* InitAtomicValueNeededFlags: Initializes all symbols, */
/*   floats, integers, and bitmaps as being unneeded by */
/*   the binary image being saved.                      */
/********************************************************/
globle void InitAtomicValueNeededFlags(
  void *theEnv)
  {
   unsigned long i;
   SYMBOL_HN *symbolPtr, **symbolArray;
   FLOAT_HN *floatPtr, **floatArray;
   INTEGER_HN *integerPtr, **integerArray;
   BITMAP_HN *bitMapPtr, **bitMapArray;

   /*===============*/
   /* Mark symbols. */
   /*===============*/

   symbolArray = GetSymbolTable(theEnv);

   for (i = 0; i < SYMBOL_HASH_SIZE; i++)
     {
      symbolPtr = symbolArray[i];
      while (symbolPtr != NULL)
        {
         symbolPtr->neededSymbol = FALSE;
         symbolPtr = symbolPtr->next;
        }
     }

   /*==============*/
   /* Mark floats. */
   /*==============*/

   floatArray = GetFloatTable(theEnv);

   for (i = 0; i < FLOAT_HASH_SIZE; i++)
     {
      floatPtr = floatArray[i];
      while (floatPtr != NULL)
        {
         floatPtr->neededFloat = FALSE;
         floatPtr = floatPtr->next;
        }
     }

   /*================*/
   /* Mark integers. */
   /*================*/

   integerArray = GetIntegerTable(theEnv);

   for (i = 0; i < INTEGER_HASH_SIZE; i++)
     {
      integerPtr = integerArray[i];
      while (integerPtr != NULL)
        {
         integerPtr->neededInteger = FALSE;
         integerPtr = integerPtr->next;
        }
     }

   /*===============*/
   /* Mark bitmaps. */
   /*===============*/

   bitMapArray = GetBitMapTable(theEnv);

   for (i = 0; i < BITMAP_HASH_SIZE; i++)
     {
      bitMapPtr = bitMapArray[i];
      while (bitMapPtr != NULL)
        {
         bitMapPtr->neededBitMap = FALSE;
         bitMapPtr = bitMapPtr->next;
        }
     }
  }

/*****************************************************************/
/* WriteNeededSymbols: Stores all of the symbols in the symbol   */
/*   table needed for this binary image in the binary save file. */
/*****************************************************************/
globle void WriteNeededSymbols(
  void *theEnv,
  FILE *fp)
  {
   unsigned long i;
   size_t length;
   SYMBOL_HN **symbolArray;
   SYMBOL_HN *symbolPtr;
   unsigned long int numberOfUsedSymbols = 0;
   size_t size = 0;

   /*=================================*/
   /* Get a copy of the symbol table. */
   /*=================================*/

   symbolArray = GetSymbolTable(theEnv);

   /*======================================================*/
   /* Get the number of symbols and the total string size. */
   /*======================================================*/

   for (i = 0; i < SYMBOL_HASH_SIZE; i++)
     {
      for (symbolPtr = symbolArray[i];
           symbolPtr != NULL;
           symbolPtr = symbolPtr->next)
        {
         if (symbolPtr->neededSymbol)
           {
            numberOfUsedSymbols++;
            size += strlen(symbolPtr->contents) + 1;
           }
        }
     }

   /*=============================================*/
   /* Write out the symbols and the string sizes. */
   /*=============================================*/

   GenWrite((void *) &numberOfUsedSymbols,(unsigned long) sizeof(unsigned long int),fp);
   GenWrite((void *) &size,(unsigned long) sizeof(unsigned long int),fp);

   for (i = 0; i < SYMBOL_HASH_SIZE; i++)
     {
      for (symbolPtr = symbolArray[i];
           symbolPtr != NULL;
           symbolPtr = symbolPtr->next)
        {
         if (symbolPtr->neededSymbol)
           {
            length = strlen(symbolPtr->contents) + 1;
            GenWrite((void *) symbolPtr->contents,(unsigned long) length,fp);
           }
        }
     }
  }

/*****************************************************************/
/* WriteNeededFloats: Stores all of the floats in the float      */
/*   table needed for this binary image in the binary save file. */
/*****************************************************************/
globle void WriteNeededFloats(
  void *theEnv,
  FILE *fp)
  {
   int i;
   FLOAT_HN **floatArray;
   FLOAT_HN *floatPtr;
   unsigned long int numberOfUsedFloats = 0;

   /*================================*/
   /* Get a copy of the float table. */
   /*================================*/

   floatArray = GetFloatTable(theEnv);

   /*===========================*/
   /* Get the number of floats. */
   /*===========================*/

   for (i = 0; i < FLOAT_HASH_SIZE; i++)
     {
      for (floatPtr = floatArray[i];
           floatPtr != NULL;
           floatPtr = floatPtr->next)
        { if (floatPtr->neededFloat) numberOfUsedFloats++; }
     }

   /*======================================================*/
   /* Write out the number of floats and the float values. */
   /*======================================================*/

   GenWrite(&numberOfUsedFloats,(unsigned long) sizeof(unsigned long int),fp);

   for (i = 0 ; i < FLOAT_HASH_SIZE; i++)
     {
      for (floatPtr = floatArray[i];
           floatPtr != NULL;
           floatPtr = floatPtr->next)
        {
         if (floatPtr->neededFloat)
           { GenWrite(&floatPtr->contents,
                      (unsigned long) sizeof(floatPtr->contents),fp); }
        }
     }
  }

/******************************************************************/
/* WriteNeededIntegers: Stores all of the integers in the integer */
/*   table needed for this binary image in the binary save file.  */
/******************************************************************/
globle void WriteNeededIntegers(
  void *theEnv,
  FILE *fp)
  {
   int i;
   INTEGER_HN **integerArray;
   INTEGER_HN *integerPtr;
   unsigned long int numberOfUsedIntegers = 0;

   /*==================================*/
   /* Get a copy of the integer table. */
   /*==================================*/

   integerArray = GetIntegerTable(theEnv);

   /*=============================*/
   /* Get the number of integers. */
   /*=============================*/

   for (i = 0 ; i < INTEGER_HASH_SIZE; i++)
     {
      for (integerPtr = integerArray[i];
           integerPtr != NULL;
           integerPtr = integerPtr->next)
        {
         if (integerPtr->neededInteger) numberOfUsedIntegers++;
        }
     }

   /*==========================================================*/
   /* Write out the number of integers and the integer values. */
   /*==========================================================*/

   GenWrite(&numberOfUsedIntegers,(unsigned long) sizeof(unsigned long int),fp);

   for (i = 0 ; i < INTEGER_HASH_SIZE; i++)
     {
      for (integerPtr = integerArray[i];
           integerPtr != NULL;
           integerPtr = integerPtr->next)
        {
         if (integerPtr->neededInteger)
           {
            GenWrite(&integerPtr->contents,
                     (unsigned long) sizeof(integerPtr->contents),fp);
           }
        }
     }
  }

/*****************************************************************/
/* WriteNeededBitMaps: Stores all of the bitmaps in the bitmap   */
/*   table needed for this binary image in the binary save file. */
/*****************************************************************/
static void WriteNeededBitMaps(
  void *theEnv,
  FILE *fp)
  {
   int i;
   BITMAP_HN **bitMapArray;
   BITMAP_HN *bitMapPtr;
   unsigned long int numberOfUsedBitMaps = 0, size = 0;
   char tempSize;

   /*=================================*/
   /* Get a copy of the bitmap table. */
   /*=================================*/

   bitMapArray = GetBitMapTable(theEnv);

   /*======================================================*/
   /* Get the number of bitmaps and the total bitmap size. */
   /*======================================================*/

   for (i = 0; i < BITMAP_HASH_SIZE; i++)
     {
      for (bitMapPtr = bitMapArray[i];
           bitMapPtr != NULL;
           bitMapPtr = bitMapPtr->next)
        {
         if (bitMapPtr->neededBitMap)
           {
            numberOfUsedBitMaps++;
            size += (unsigned long) (bitMapPtr->size + 1);
           }
        }
     }

   /*========================================*/
   /* Write out the bitmaps and their sizes. */
   /*========================================*/

   GenWrite((void *) &numberOfUsedBitMaps,(unsigned long) sizeof(unsigned long int),fp);
   GenWrite((void *) &size,(unsigned long) sizeof(unsigned long int),fp);

   for (i = 0; i < BITMAP_HASH_SIZE; i++)
     {
      for (bitMapPtr = bitMapArray[i];
           bitMapPtr != NULL;
           bitMapPtr = bitMapPtr->next)
        {
         if (bitMapPtr->neededBitMap)
           {
            tempSize = (char) bitMapPtr->size;
            GenWrite((void *) &tempSize,(unsigned long) sizeof(char),fp);
            GenWrite((void *) bitMapPtr->contents,(unsigned long) bitMapPtr->size,fp);
           }
        }
     }
  }

#endif /* BLOAD_AND_BSAVE || BSAVE_INSTANCES */

/*********************************************/
/* ReadNeededAtomicValues: Read all symbols, */
/*   floats, integers, and bitmaps needed by */
/*   this binary image from the binary file. */
/*********************************************/
globle void ReadNeededAtomicValues(
  void *theEnv)
  {
   ReadNeededSymbols(theEnv);
   ReadNeededFloats(theEnv);
   ReadNeededIntegers(theEnv);
   ReadNeededBitMaps(theEnv);
  }

/*******************************************/
/* ReadNeededSymbols: Reads in the symbols */
/*   used by the binary image.             */
/*******************************************/
globle void ReadNeededSymbols(
  void *theEnv)
  {
   char *symbolNames, *namePtr;
   unsigned long space;
   long i;

   /*=================================================*/
   /* Determine the number of symbol names to be read */
   /* and space required for them.                    */
   /*=================================================*/

   GenReadBinary(theEnv,(void *) &SymbolData(theEnv)->NumberOfSymbols,(unsigned long) sizeof(long int));
   GenReadBinary(theEnv,&space,(unsigned long) sizeof(unsigned long int));
   if (SymbolData(theEnv)->NumberOfSymbols == 0)
     {
      SymbolData(theEnv)->SymbolArray = NULL;
      return;
     }

   /*=======================================*/
   /* Allocate area for strings to be read. */
   /*=======================================*/

   symbolNames = (char *) gm3(theEnv,(long) space);
   GenReadBinary(theEnv,(void *) symbolNames,space);

   /*================================================*/
   /* Store the symbol pointers in the symbol array. */
   /*================================================*/

   SymbolData(theEnv)->SymbolArray = (SYMBOL_HN **)
                 gm3(theEnv,(long) sizeof(SYMBOL_HN *) *  SymbolData(theEnv)->NumberOfSymbols);
   namePtr = symbolNames;
   for (i = 0; i < SymbolData(theEnv)->NumberOfSymbols; i++)
     {
      SymbolData(theEnv)->SymbolArray[i] = (SYMBOL_HN *) EnvAddSymbol(theEnv,namePtr);
      namePtr += strlen(namePtr) + 1;
     }

   /*=======================*/
   /* Free the name buffer. */
   /*=======================*/

   rm3(theEnv,(void *) symbolNames,(long) space);
  }

/*****************************************/
/* ReadNeededFloats: Reads in the floats */
/*   used by the binary image.           */
/*****************************************/
globle void ReadNeededFloats(
  void *theEnv)
  {
   double *floatValues;
   long i;

   /*============================================*/
   /* Determine the number of floats to be read. */
   /*============================================*/

   GenReadBinary(theEnv,&SymbolData(theEnv)->NumberOfFloats,(unsigned long) sizeof(long int));
   if (SymbolData(theEnv)->NumberOfFloats == 0)
     {
      SymbolData(theEnv)->FloatArray = NULL;
      return;
     }

   /*===============================*/
   /* Allocate area for the floats. */
   /*===============================*/

   floatValues = (double *) gm3(theEnv,(long) sizeof(double) * SymbolData(theEnv)->NumberOfFloats);
   GenReadBinary(theEnv,(void *) floatValues,(unsigned long) (sizeof(double) * SymbolData(theEnv)->NumberOfFloats));

   /*======================================*/
   /* Store the floats in the float array. */
   /*======================================*/

   SymbolData(theEnv)->FloatArray = (FLOAT_HN **)
               gm3(theEnv,(long) sizeof(FLOAT_HN *) * SymbolData(theEnv)->NumberOfFloats);
   for (i = 0; i < SymbolData(theEnv)->NumberOfFloats; i++)
     { SymbolData(theEnv)->FloatArray[i] = (FLOAT_HN *) EnvAddDouble(theEnv,floatValues[i]); }

   /*========================*/
   /* Free the float buffer. */
   /*========================*/

   rm3(theEnv,(void *) floatValues,(long) (sizeof(double) * SymbolData(theEnv)->NumberOfFloats));
  }

/*********************************************/
/* ReadNeededIntegers: Reads in the integers */
/*   used by the binary image.               */
/*********************************************/
globle void ReadNeededIntegers(
  void *theEnv)
  {
   long long *integerValues;
   long i;

   /*==============================================*/
   /* Determine the number of integers to be read. */
   /*==============================================*/

   GenReadBinary(theEnv,&SymbolData(theEnv)->NumberOfIntegers,(unsigned long) sizeof(unsigned long int));
   if (SymbolData(theEnv)->NumberOfIntegers == 0)
     {
      SymbolData(theEnv)->IntegerArray = NULL;
      return;
     }

   /*=================================*/
   /* Allocate area for the integers. */
   /*=================================*/

   integerValues = (long long *) gm3(theEnv,(long) (sizeof(long long) * SymbolData(theEnv)->NumberOfIntegers));
   GenReadBinary(theEnv,(void *) integerValues,(unsigned long) (sizeof(long long) * SymbolData(theEnv)->NumberOfIntegers));

   /*==========================================*/
   /* Store the integers in the integer array. */
   /*==========================================*/

   SymbolData(theEnv)->IntegerArray = (INTEGER_HN **)
           gm3(theEnv,(long) (sizeof(INTEGER_HN *) * SymbolData(theEnv)->NumberOfIntegers));
   for (i = 0; i < SymbolData(theEnv)->NumberOfIntegers; i++)
     { SymbolData(theEnv)->IntegerArray[i] = (INTEGER_HN *) EnvAddLong(theEnv,integerValues[i]); }

   /*==========================*/
   /* Free the integer buffer. */
   /*==========================*/

   rm3(theEnv,(void *) integerValues,(long) (sizeof(long long) * SymbolData(theEnv)->NumberOfIntegers));
  }

/*******************************************/
/* ReadNeededBitMaps: Reads in the bitmaps */
/*   used by the binary image.             */
/*******************************************/
static void ReadNeededBitMaps(
  void *theEnv)
  {
   char *bitMapStorage, *bitMapPtr;
   unsigned long space;
   long i;

   /*=======================================*/
   /* Determine the number of bitmaps to be */
   /* read and space required for them.     */
   /*=======================================*/

   GenReadBinary(theEnv,(void *) &SymbolData(theEnv)->NumberOfBitMaps,(unsigned long) sizeof(long int));
   GenReadBinary(theEnv,&space,(unsigned long) sizeof(unsigned long int));
   if (SymbolData(theEnv)->NumberOfBitMaps == 0)
     {
      SymbolData(theEnv)->BitMapArray = NULL;
      return;
     }

   /*=======================================*/
   /* Allocate area for bitmaps to be read. */
   /*=======================================*/

   bitMapStorage = (char *) gm3(theEnv,(long) space);
   GenReadBinary(theEnv,(void *) bitMapStorage,space);

   /*================================================*/
   /* Store the bitMap pointers in the bitmap array. */
   /*================================================*/

   SymbolData(theEnv)->BitMapArray = (BITMAP_HN **)
                 gm3(theEnv,(long) sizeof(BITMAP_HN *) *  SymbolData(theEnv)->NumberOfBitMaps);
   bitMapPtr = bitMapStorage;
   for (i = 0; i < SymbolData(theEnv)->NumberOfBitMaps; i++)
     {
      SymbolData(theEnv)->BitMapArray[i] = (BITMAP_HN *) EnvAddBitMap(theEnv,bitMapPtr+1,*bitMapPtr);
      bitMapPtr += *bitMapPtr + 1;
     }

   /*=========================*/
   /* Free the bitmap buffer. */
   /*=========================*/

   rm3(theEnv,(void *) bitMapStorage,(long) space);
  }

/**********************************************************/
/* FreeAtomicValueStorage: Returns the memory allocated   */
/*   for storing the pointers to atomic data values used  */
/*   in refreshing expressions and other data structures. */
/**********************************************************/
globle void FreeAtomicValueStorage(
  void *theEnv)
  {
   if (SymbolData(theEnv)->SymbolArray != NULL)
     rm3(theEnv,(void *) SymbolData(theEnv)->SymbolArray,(long) sizeof(SYMBOL_HN *) * SymbolData(theEnv)->NumberOfSymbols);
   if (SymbolData(theEnv)->FloatArray != NULL)
     rm3(theEnv,(void *) SymbolData(theEnv)->FloatArray,(long) sizeof(FLOAT_HN *) * SymbolData(theEnv)->NumberOfFloats);
   if (SymbolData(theEnv)->IntegerArray != NULL)
     rm3(theEnv,(void *) SymbolData(theEnv)->IntegerArray,(long) sizeof(INTEGER_HN *) * SymbolData(theEnv)->NumberOfIntegers);
   if (SymbolData(theEnv)->BitMapArray != NULL)
     rm3(theEnv,(void *) SymbolData(theEnv)->BitMapArray,(long) sizeof(BITMAP_HN *) * SymbolData(theEnv)->NumberOfBitMaps);
     
   SymbolData(theEnv)->SymbolArray = NULL;
   SymbolData(theEnv)->FloatArray = NULL;
   SymbolData(theEnv)->IntegerArray = NULL;
   SymbolData(theEnv)->BitMapArray = NULL;
   SymbolData(theEnv)->NumberOfSymbols = 0;
   SymbolData(theEnv)->NumberOfFloats = 0;
   SymbolData(theEnv)->NumberOfIntegers = 0;
   SymbolData(theEnv)->NumberOfBitMaps = 0;
  }

#endif /* BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE || BLOAD_INSTANCES || BSAVE_INSTANCES */
