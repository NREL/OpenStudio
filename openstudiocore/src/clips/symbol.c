   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*                    SYMBOL MODULE                    */
   /*******************************************************/

/*************************************************************/
/* Purpose: Manages the atomic data value hash tables for    */
/*   storing symbols, integers, floats, and bit maps.        */
/*   Contains routines for adding entries, examining the     */
/*   hash tables, and performing garbage collection to       */
/*   remove entries no longer in use.                        */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: CLIPS crashing on AMD64 processor in the       */
/*            function used to generate a hash value for     */
/*            integers. DR0871                               */
/*                                                           */
/*            Support for run-time programs directly passing */
/*            the hash tables for initialization.            */
/*                                                           */
/*            Corrected code generating compilation          */
/*            warnings.                                      */
/*                                                           */
/*************************************************************/

#define _SYMBOL_SOURCE_
                                                                                
#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>
#include <string.h>

#include "setup.h"

#include "constant.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "router.h"
#include "utility.h"
#include "argacces.h"
#include "sysdep.h"

#include "symbol.h"

/***************/
/* DEFINITIONS */
/***************/

#define FALSE_STRING "FALSE"
#define TRUE_STRING  "TRUE"
#define POSITIVE_INFINITY_STRING "+oo"
#define NEGATIVE_INFINITY_STRING "-oo"

#define AVERAGE_STRING_SIZE 10
#define AVERAGE_BITMAP_SIZE sizeof(long)
#define NUMBER_OF_LONGS_FOR_HASH 25

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                    RemoveHashNode(void *,GENERIC_HN *,GENERIC_HN **,int,int);
   static void                    AddEphemeralHashNode(void *,GENERIC_HN *,struct ephemeron **,
                                                       int,int);
   static void                    RemoveEphemeralHashNodes(void *,struct ephemeron **,
                                                           GENERIC_HN **,
                                                           int,int,int);
   static char                   *StringWithinString(char *,char *);
   static size_t                  CommonPrefixLength(char *,char *);
   static void                    DeallocateSymbolData(void *);

/*******************************************************/
/* InitializeAtomTables: Initializes the SymbolTable,  */
/*   IntegerTable, and FloatTable. It also initializes */
/*   the TrueSymbol and FalseSymbol.                   */
/*******************************************************/
#if WIN_BTC && (! RUN_TIME)
#pragma argsused
#endif
globle void InitializeAtomTables(
  void *theEnv,
  struct symbolHashNode **symbolTable,
  struct floatHashNode **floatTable,
  struct integerHashNode **integerTable,
  struct bitMapHashNode **bitmapTable,
  struct externalAddressHashNode **externalAddressTable)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(symbolTable)
#pragma unused(floatTable)
#pragma unused(integerTable)
#pragma unused(bitmapTable)
#pragma unused(externalAddressTable)
#endif
   unsigned long i;
   
   AllocateEnvironmentData(theEnv,SYMBOL_DATA,sizeof(struct symbolData),DeallocateSymbolData);

#if ! RUN_TIME
   /*=========================*/
   /* Create the hash tables. */
   /*=========================*/

   SymbolData(theEnv)->SymbolTable = (SYMBOL_HN **)
                  gm3(theEnv,sizeof (SYMBOL_HN *) * SYMBOL_HASH_SIZE);

   SymbolData(theEnv)->FloatTable = (FLOAT_HN **)
                  gm2(theEnv,(int) sizeof (FLOAT_HN *) * FLOAT_HASH_SIZE);

   SymbolData(theEnv)->IntegerTable = (INTEGER_HN **)
                   gm2(theEnv,(int) sizeof (INTEGER_HN *) * INTEGER_HASH_SIZE);

   SymbolData(theEnv)->BitMapTable = (BITMAP_HN **)
                   gm2(theEnv,(int) sizeof (BITMAP_HN *) * BITMAP_HASH_SIZE);

   SymbolData(theEnv)->ExternalAddressTable = (EXTERNAL_ADDRESS_HN **)
                   gm2(theEnv,(int) sizeof (EXTERNAL_ADDRESS_HN *) * EXTERNAL_ADDRESS_HASH_SIZE);

   /*===================================================*/
   /* Initialize all of the hash table entries to NULL. */
   /*===================================================*/

   for (i = 0; i < SYMBOL_HASH_SIZE; i++) SymbolData(theEnv)->SymbolTable[i] = NULL;
   for (i = 0; i < FLOAT_HASH_SIZE; i++) SymbolData(theEnv)->FloatTable[i] = NULL;
   for (i = 0; i < INTEGER_HASH_SIZE; i++) SymbolData(theEnv)->IntegerTable[i] = NULL;
   for (i = 0; i < BITMAP_HASH_SIZE; i++) SymbolData(theEnv)->BitMapTable[i] = NULL;
   for (i = 0; i < EXTERNAL_ADDRESS_HASH_SIZE; i++) SymbolData(theEnv)->ExternalAddressTable[i] = NULL;

   /*========================*/
   /* Predefine some values. */
   /*========================*/

   SymbolData(theEnv)->TrueSymbolHN = EnvAddSymbol(theEnv,TRUE_STRING);
   IncrementSymbolCount(SymbolData(theEnv)->TrueSymbolHN);
   SymbolData(theEnv)->FalseSymbolHN = EnvAddSymbol(theEnv,FALSE_STRING);
   IncrementSymbolCount(SymbolData(theEnv)->FalseSymbolHN);
   SymbolData(theEnv)->PositiveInfinity = EnvAddSymbol(theEnv,POSITIVE_INFINITY_STRING);
   IncrementSymbolCount(SymbolData(theEnv)->PositiveInfinity);
   SymbolData(theEnv)->NegativeInfinity = EnvAddSymbol(theEnv,NEGATIVE_INFINITY_STRING);
   IncrementSymbolCount(SymbolData(theEnv)->NegativeInfinity);
   SymbolData(theEnv)->Zero = EnvAddLong(theEnv,0LL);
   IncrementIntegerCount(SymbolData(theEnv)->Zero);
#else
   SetSymbolTable(theEnv,symbolTable);
   SetFloatTable(theEnv,floatTable);
   SetIntegerTable(theEnv,integerTable);
   SetBitMapTable(theEnv,bitmapTable);
   
   SymbolData(theEnv)->ExternalAddressTable = (EXTERNAL_ADDRESS_HN **)
                gm2(theEnv,(int) sizeof (EXTERNAL_ADDRESS_HN *) * EXTERNAL_ADDRESS_HASH_SIZE);

   for (i = 0; i < EXTERNAL_ADDRESS_HASH_SIZE; i++) SymbolData(theEnv)->ExternalAddressTable[i] = NULL;
#endif
  }

/*************************************************/
/* DeallocateSymbolData: Deallocates environment */
/*    data for symbols.                          */
/*************************************************/
static void DeallocateSymbolData(
  void *theEnv)
  {
   int i;
   SYMBOL_HN *shPtr, *nextSHPtr;
   INTEGER_HN *ihPtr, *nextIHPtr;
   FLOAT_HN *fhPtr, *nextFHPtr;
   BITMAP_HN *bmhPtr, *nextBMHPtr;
   EXTERNAL_ADDRESS_HN *eahPtr, *nextEAHPtr;
   struct ephemeron *edPtr, *nextEDPtr;

   if ((SymbolData(theEnv)->SymbolTable == NULL) ||
       (SymbolData(theEnv)->FloatTable == NULL) ||
       (SymbolData(theEnv)->IntegerTable == NULL) ||
       (SymbolData(theEnv)->BitMapTable == NULL) ||
       (SymbolData(theEnv)->ExternalAddressTable == NULL))
     { return; }
     
   for (i = 0; i < SYMBOL_HASH_SIZE; i++) 
     {
      shPtr = SymbolData(theEnv)->SymbolTable[i];
      
      while (shPtr != NULL)
        {
         nextSHPtr = shPtr->next;
         if (! shPtr->permanent)
           {
            rm(theEnv,shPtr->contents,strlen(shPtr->contents)+1);
            rtn_struct(theEnv,symbolHashNode,shPtr);
           }  
         shPtr = nextSHPtr;
        } 
     }
      
   for (i = 0; i < FLOAT_HASH_SIZE; i++) 
     {
      fhPtr = SymbolData(theEnv)->FloatTable[i];

      while (fhPtr != NULL)
        {
         nextFHPtr = fhPtr->next;
         if (! fhPtr->permanent)
           { rtn_struct(theEnv,floatHashNode,fhPtr); }
         fhPtr = nextFHPtr;
        }
     }
     
   for (i = 0; i < INTEGER_HASH_SIZE; i++) 
     {
      ihPtr = SymbolData(theEnv)->IntegerTable[i];

      while (ihPtr != NULL)
        {
         nextIHPtr = ihPtr->next;
         if (! ihPtr->permanent)
           { rtn_struct(theEnv,integerHashNode,ihPtr); }
         ihPtr = nextIHPtr;
        }
     }
     
   for (i = 0; i < BITMAP_HASH_SIZE; i++) 
     {
      bmhPtr = SymbolData(theEnv)->BitMapTable[i];

      while (bmhPtr != NULL)
        {
         nextBMHPtr = bmhPtr->next;
         if (! bmhPtr->permanent)
           {
            rm(theEnv,bmhPtr->contents,bmhPtr->size);
            rtn_struct(theEnv,bitMapHashNode,bmhPtr); 
           } 
         bmhPtr = nextBMHPtr;
        }
     }

   for (i = 0; i < EXTERNAL_ADDRESS_HASH_SIZE; i++) 
     {
      eahPtr = SymbolData(theEnv)->ExternalAddressTable[i];

      while (eahPtr != NULL)
        {
         nextEAHPtr = eahPtr->next;
         if (! eahPtr->permanent)
           {
            rtn_struct(theEnv,externalAddressHashNode,eahPtr); 
           } 
         eahPtr = nextEAHPtr;
        }
     }

   /*=========================================*/
   /* Remove the ephemeral symbol structures. */
   /*=========================================*/
   
   edPtr = SymbolData(theEnv)->EphemeralSymbolList;

   while (edPtr != NULL)
     {
      nextEDPtr = edPtr->next;
      rtn_struct(theEnv,ephemeron,edPtr);
      edPtr = nextEDPtr;
     }

   edPtr = SymbolData(theEnv)->EphemeralFloatList;

   while (edPtr != NULL)
     {
      nextEDPtr = edPtr->next;
      rtn_struct(theEnv,ephemeron,edPtr);
      edPtr = nextEDPtr;
     }

   edPtr = SymbolData(theEnv)->EphemeralIntegerList;

   while (edPtr != NULL)
     {
      nextEDPtr = edPtr->next;
      rtn_struct(theEnv,ephemeron,edPtr);
      edPtr = nextEDPtr;
     }

   edPtr = SymbolData(theEnv)->EphemeralBitMapList;

   while (edPtr != NULL)
     {
      nextEDPtr = edPtr->next;
      rtn_struct(theEnv,ephemeron,edPtr);
      edPtr = nextEDPtr;
     }

   edPtr = SymbolData(theEnv)->EphemeralExternalAddressList;

   while (edPtr != NULL)
     {
      nextEDPtr = edPtr->next;
      rtn_struct(theEnv,ephemeron,edPtr);
      edPtr = nextEDPtr;
     }

   /*================================*/
   /* Remove the symbol hash tables. */
   /*================================*/
   
 #if ! RUN_TIME  
   rm3(theEnv,SymbolData(theEnv)->SymbolTable,sizeof (SYMBOL_HN *) * SYMBOL_HASH_SIZE);

   genfree(theEnv,SymbolData(theEnv)->FloatTable,(int) sizeof (FLOAT_HN *) * FLOAT_HASH_SIZE);

   genfree(theEnv,SymbolData(theEnv)->IntegerTable,(int) sizeof (INTEGER_HN *) * INTEGER_HASH_SIZE);

   genfree(theEnv,SymbolData(theEnv)->BitMapTable,(int) sizeof (BITMAP_HN *) * BITMAP_HASH_SIZE);
#endif
   
   genfree(theEnv,SymbolData(theEnv)->ExternalAddressTable,(int) sizeof (EXTERNAL_ADDRESS_HN *) * EXTERNAL_ADDRESS_HASH_SIZE);

   /*==============================*/
   /* Remove binary symbol tables. */
   /*==============================*/
   
#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE || BLOAD_INSTANCES || BSAVE_INSTANCES
   if (SymbolData(theEnv)->SymbolArray != NULL)
     rm3(theEnv,(void *) SymbolData(theEnv)->SymbolArray,(long) sizeof(SYMBOL_HN *) * SymbolData(theEnv)->NumberOfSymbols);
   if (SymbolData(theEnv)->FloatArray != NULL)
     rm3(theEnv,(void *) SymbolData(theEnv)->FloatArray,(long) sizeof(FLOAT_HN *) * SymbolData(theEnv)->NumberOfFloats);
   if (SymbolData(theEnv)->IntegerArray != NULL)
     rm3(theEnv,(void *) SymbolData(theEnv)->IntegerArray,(long) sizeof(INTEGER_HN *) * SymbolData(theEnv)->NumberOfIntegers);
   if (SymbolData(theEnv)->BitMapArray != NULL)
     rm3(theEnv,(void *) SymbolData(theEnv)->BitMapArray,(long) sizeof(BITMAP_HN *) * SymbolData(theEnv)->NumberOfBitMaps);
#endif
  }

/*********************************************************************/
/* EnvAddSymbol: Searches for the string in the symbol table. If the */
/*   string is already in the symbol table, then the address of the  */
/*   string's location in the symbol table is returned. Otherwise,   */
/*   the string is added to the symbol table and then the address    */
/*   of the string's location in the symbol table is returned.       */
/*********************************************************************/
globle void *EnvAddSymbol(
  void *theEnv,
  char *str)
  {
   unsigned long tally;
   size_t length;
   SYMBOL_HN *past = NULL, *peek;

    /*====================================*/
    /* Get the hash value for the string. */
    /*====================================*/

    if (str == NULL)
      {
       SystemError(theEnv,"SYMBOL",1);
       EnvExitRouter(theEnv,EXIT_FAILURE);
      }

    tally = HashSymbol(str,SYMBOL_HASH_SIZE);
    peek = SymbolData(theEnv)->SymbolTable[tally];

    /*==================================================*/
    /* Search for the string in the list of entries for */
    /* this symbol table location.  If the string is    */
    /* found, then return the address of the string.    */
    /*==================================================*/

    while (peek != NULL)
      {
       if (strcmp(str,peek->contents) == 0)
         { return((void *) peek); }
       past = peek;
       peek = peek->next;
      }

    /*==================================================*/
    /* Add the string at the end of the list of entries */
    /* for this symbol table location.                  */
    /*==================================================*/

    peek = get_struct(theEnv,symbolHashNode);

    if (past == NULL) SymbolData(theEnv)->SymbolTable[tally] = peek;
    else past->next = peek;

    length = strlen(str) + 1;
    peek->contents = (char *) gm2(theEnv,length);
    peek->next = NULL;
    peek->bucket = tally;
    peek->count = 0;
    peek->permanent = FALSE;
    genstrcpy(peek->contents,str);
      
    /*================================================*/
    /* Add the string to the list of ephemeral items. */
    /*================================================*/

    AddEphemeralHashNode(theEnv,(GENERIC_HN *) peek,&SymbolData(theEnv)->EphemeralSymbolList,
                         sizeof(SYMBOL_HN),AVERAGE_STRING_SIZE);
    peek->depth = EvaluationData(theEnv)->CurrentEvaluationDepth;

    /*===================================*/
    /* Return the address of the symbol. */
    /*===================================*/

    return((void *) peek);
   }

/*****************************************************************/
/* FindSymbolHN: Searches for the string in the symbol table and */
/*   returns a pointer to it if found, otherwise returns NULL.   */
/*****************************************************************/
globle SYMBOL_HN *FindSymbolHN(
  void *theEnv,
  char *str)
  {
   unsigned long tally;
   SYMBOL_HN *peek;

    tally = HashSymbol(str,SYMBOL_HASH_SIZE);

    for (peek = SymbolData(theEnv)->SymbolTable[tally];
         peek != NULL;
         peek = peek->next)
      { 
       if (strcmp(str,peek->contents) == 0) 
         { return(peek); }
      }

    return(NULL);
   }

/*******************************************************************/
/* EnvAddDouble: Searches for the double in the hash table. If the */
/*   double is already in the hash table, then the address of the  */
/*   double is returned. Otherwise, the double is hashed into the  */
/*   table and the address of the double is also returned.         */
/*******************************************************************/
globle void *EnvAddDouble(
  void *theEnv,
  double number)
  {
   unsigned long tally;
   FLOAT_HN *past = NULL, *peek;

    /*====================================*/
    /* Get the hash value for the double. */
    /*====================================*/

    tally = HashFloat(number,FLOAT_HASH_SIZE);
    peek = SymbolData(theEnv)->FloatTable[tally];

    /*==================================================*/
    /* Search for the double in the list of entries for */
    /* this hash location.  If the double is found,     */
    /* then return the address of the double.           */
    /*==================================================*/

    while (peek != NULL)
      {
       if (number == peek->contents)
         { return((void *) peek); }
       past = peek;
       peek = peek->next;
      }

    /*=================================================*/
    /* Add the float at the end of the list of entries */
    /* for this hash location.                         */
    /*=================================================*/

    peek = get_struct(theEnv,floatHashNode);

    if (past == NULL) SymbolData(theEnv)->FloatTable[tally] = peek;
    else past->next = peek;

    peek->contents = number;
    peek->next = NULL;
    peek->bucket = tally;
    peek->count = 0;
    peek->permanent = FALSE;

    /*===============================================*/
    /* Add the float to the list of ephemeral items. */
    /*===============================================*/

    AddEphemeralHashNode(theEnv,(GENERIC_HN *) peek,&SymbolData(theEnv)->EphemeralFloatList,
                         sizeof(FLOAT_HN),0);
    peek->depth = EvaluationData(theEnv)->CurrentEvaluationDepth;

    /*==================================*/
    /* Return the address of the float. */
    /*==================================*/

    return((void *) peek);
   }

/***************************************************************/
/* EnvAddLong: Searches for the long in the hash table. If the */
/*   long is already in the hash table, then the address of    */
/*   the long is returned. Otherwise, the long is hashed into  */
/*   the table and the address of the long is also returned.   */
/***************************************************************/
globle void *EnvAddLong(
  void *theEnv,
  long long number)
  {
   unsigned long tally;
   INTEGER_HN *past = NULL, *peek;

    /*==================================*/
    /* Get the hash value for the long. */
    /*==================================*/

    tally = HashInteger(number,INTEGER_HASH_SIZE);
    peek = SymbolData(theEnv)->IntegerTable[tally];

    /*================================================*/
    /* Search for the long in the list of entries for */
    /* this hash location. If the long is found, then */
    /* return the address of the long.                */
    /*================================================*/

    while (peek != NULL)
      {
       if (number == peek->contents)
         { return((void *) peek); }
       past = peek;
       peek = peek->next;
      }

    /*================================================*/
    /* Add the long at the end of the list of entries */
    /* for this hash location.                        */
    /*================================================*/

    peek = get_struct(theEnv,integerHashNode);
    if (past == NULL) SymbolData(theEnv)->IntegerTable[tally] = peek;
    else past->next = peek;

    peek->contents = number;
    peek->next = NULL;
    peek->bucket = tally;
    peek->count = 0;
    peek->permanent = FALSE;

    /*=================================================*/
    /* Add the integer to the list of ephemeral items. */
    /*=================================================*/

    AddEphemeralHashNode(theEnv,(GENERIC_HN *) peek,&SymbolData(theEnv)->EphemeralIntegerList,
                         sizeof(INTEGER_HN),0);
    peek->depth = EvaluationData(theEnv)->CurrentEvaluationDepth;

    /*====================================*/
    /* Return the address of the integer. */
    /*====================================*/

    return((void *) peek);
   }

/*****************************************************************/
/* FindLongHN: Searches for the integer in the integer table and */
/*   returns a pointer to it if found, otherwise returns NULL.   */
/*****************************************************************/
globle INTEGER_HN *FindLongHN(
  void *theEnv,
  long long theLong)
  {
   unsigned long tally;
   INTEGER_HN *peek;

   tally = HashInteger(theLong,INTEGER_HASH_SIZE);

   for (peek = SymbolData(theEnv)->IntegerTable[tally];
        peek != NULL;
        peek = peek->next)
     { if (peek->contents == theLong) return(peek); }

   return(NULL);
  }

/*******************************************************************/
/* EnvAddBitMap: Searches for the bitmap in the hash table. If the */
/*   bitmap is already in the hash table, then the address of the  */
/*   bitmap is returned. Otherwise, the bitmap is hashed into the  */
/*   table and the address of the bitmap is also returned.         */
/*******************************************************************/
globle void *EnvAddBitMap(
  void *theEnv,
  void *vTheBitMap,
  unsigned size)
  {
   char *theBitMap = (char *) vTheBitMap;
   unsigned long tally;
   unsigned i;
   BITMAP_HN *past = NULL, *peek;

    /*====================================*/
    /* Get the hash value for the bitmap. */
    /*====================================*/

    if (theBitMap == NULL)
      {
       SystemError(theEnv,"SYMBOL",2);
       EnvExitRouter(theEnv,EXIT_FAILURE);
      }

    tally = HashBitMap(theBitMap,BITMAP_HASH_SIZE,size);
    peek = SymbolData(theEnv)->BitMapTable[tally];

    /*==================================================*/
    /* Search for the bitmap in the list of entries for */
    /* this hash table location.  If the bitmap is      */
    /* found, then return the address of the bitmap.    */
    /*==================================================*/

    while (peek != NULL)
      {
	   if (peek->size == (unsigned short) size)
         {
          for (i = 0; i < size ; i++)
            { if (peek->contents[i] != theBitMap[i]) break; }

          if (i == size) return((void *) peek);
         }

       past = peek;
       peek = peek->next;
      }

    /*==================================================*/
    /* Add the bitmap at the end of the list of entries */
    /* for this hash table location.  Return the        */
    /*==================================================*/

    peek = get_struct(theEnv,bitMapHashNode);
    if (past == NULL) SymbolData(theEnv)->BitMapTable[tally] = peek;
    else past->next = peek;

    peek->contents = (char *) gm2(theEnv,size);
    peek->next = NULL;
    peek->bucket = tally;
    peek->count = 0;
    peek->permanent = FALSE;
    peek->size = (unsigned short) size;

    for (i = 0; i < size ; i++) peek->contents[i] = theBitMap[i];

    /*================================================*/
    /* Add the bitmap to the list of ephemeral items. */
    /*================================================*/

    AddEphemeralHashNode(theEnv,(GENERIC_HN *) peek,&SymbolData(theEnv)->EphemeralBitMapList,
                         sizeof(BITMAP_HN),sizeof(long));
    peek->depth = EvaluationData(theEnv)->CurrentEvaluationDepth;

    /*===================================*/
    /* Return the address of the bitmap. */
    /*===================================*/

    return((void *) peek);
   }

/*******************************************************************/
/* EnvAddExternalAddress: Searches for the external address in the */
/*   hash table. If the external address is already in the hash    */
/*   table, then the address of the external address is returned.  */
/*   Otherwise, the external address is hashed into the table and  */
/*   the address of the external address is also returned.         */
/*******************************************************************/
globle void *EnvAddExternalAddress(
  void *theEnv,
  void *theExternalAddress,
  unsigned theType)
  {
   unsigned long tally;
   EXTERNAL_ADDRESS_HN *past = NULL, *peek;

    /*====================================*/
    /* Get the hash value for the bitmap. */
    /*====================================*/

    tally = HashExternalAddress(theExternalAddress,EXTERNAL_ADDRESS_HASH_SIZE);

    peek = SymbolData(theEnv)->ExternalAddressTable[tally];

    /*=============================================================*/
    /* Search for the external address in the list of entries for  */
    /* this hash table location.  If the external addressis found, */
    /* then return the address of the external address.            */
    /*=============================================================*/

    while (peek != NULL)
      {
       if ((peek->type == (unsigned short) theType) &&
           (peek->externalAddress == theExternalAddress))
         { return((void *) peek); }

       past = peek;
       peek = peek->next;
      }

    /*=================================================*/
    /* Add the external address at the end of the list */
    /* of entries for this hash table location.        */
    /*=================================================*/

    peek = get_struct(theEnv,externalAddressHashNode);
    if (past == NULL) SymbolData(theEnv)->ExternalAddressTable[tally] = peek;
    else past->next = peek;

    peek->externalAddress = theExternalAddress;
    peek->type = (unsigned short) theType;
    peek->next = NULL;
    peek->bucket = tally;
    peek->count = 0;
    peek->permanent = FALSE;

    /*================================================*/
    /* Add the bitmap to the list of ephemeral items. */
    /*================================================*/

    AddEphemeralHashNode(theEnv,(GENERIC_HN *) peek,&SymbolData(theEnv)->EphemeralExternalAddressList,
                         sizeof(EXTERNAL_ADDRESS_HN),sizeof(long));
    peek->depth = EvaluationData(theEnv)->CurrentEvaluationDepth;

    /*=============================================*/
    /* Return the address of the external address. */
    /*=============================================*/

    return((void *) peek);
   }

/***************************************************/
/* HashSymbol: Computes a hash value for a symbol. */
/***************************************************/
globle unsigned long HashSymbol(
  char *word,
  unsigned long range)
  {
   register int i;
   unsigned long tally = 0;

   for (i = 0; word[i]; i++)
     { tally = tally * 127 + word[i]; }

   if (range == 0)
     { return tally; }
     
   return(tally % range);
  }

/*************************************************/
/* HashFloat: Computes a hash value for a float. */
/*************************************************/
globle unsigned long HashFloat(
  double number,
  unsigned long range)
  {
   unsigned long tally = 0;
   char *word;
   unsigned i;
   
   word = (char *) &number;
   
   for (i = 0; i < sizeof(double); i++)
     { tally = tally * 127 + word[i]; }
     
   if (range == 0)
     { return tally; }
       
   return(tally % range);
  }

/******************************************************/
/* HashInteger: Computes a hash value for an integer. */
/******************************************************/
globle unsigned long HashInteger(
  long long number,
  unsigned long range)
  {
   unsigned long tally;

#if WIN_MVC
   if (number < 0)
     { number = - number; }
   tally = (((unsigned) number) % range);
#else
   tally = (((unsigned) llabs(number)) % range);
#endif

   if (range == 0)
     { return tally; }
     
   return(tally);
  }

/****************************************/
/* HashExternalAddress: Computes a hash */
/*   value for an external address.     */
/****************************************/
globle unsigned long HashExternalAddress(
  void *theExternalAddress,
  unsigned long range)
  {
   unsigned long tally;
   union
     {
      void *vv;
      unsigned uv;
     } fis;
 
   fis.vv = theExternalAddress;
   tally = (fis.uv / 256);
   
   if (range == 0)
     { return tally; }
     
   return(tally % range);
  }

/***************************************************/
/* HashBitMap: Computes a hash value for a bitmap. */
/***************************************************/
globle unsigned long HashBitMap(
  char *word,
  unsigned long range,
  unsigned length)
  {
   register unsigned k,j,i;
   unsigned long tally;
   unsigned longLength;
   unsigned long count = 0L,tmpLong;
   char *tmpPtr;

   tmpPtr = (char *) &tmpLong;

   /*================================================================ */
   /* Add up the first part of the word as unsigned long int values.  */
   /*================================================================ */

   longLength = length / sizeof(unsigned long);
   for (i = 0 , j = 0 ; i < longLength; i++)
     {
      for (k = 0 ; k < sizeof(unsigned long) ; k++ , j++)
        tmpPtr[k] = word[j];
      count += tmpLong;
     }

   /*============================================*/
   /* Add the remaining characters to the count. */
   /*============================================*/

   for (; j < length; j++) count += (unsigned long) word[j];

   /*========================*/
   /* Return the hash value. */
   /*========================*/

   if (range == 0)
     { return count; }
     
   tally = (count % range);

   return(tally);
  }

/*****************************************************/
/* DecrementSymbolCount: Decrements the count value  */
/*   for a SymbolTable entry. Adds the symbol to the */
/*   EphemeralSymbolList if the count becomes zero.  */
/*****************************************************/
globle void DecrementSymbolCount(
  void *theEnv,
  SYMBOL_HN *theValue)
  {
   if (theValue->count < 0)
     {
      SystemError(theEnv,"SYMBOL",3);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   if (theValue->count == 0)
     {
      SystemError(theEnv,"SYMBOL",4);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   theValue->count--;

   if (theValue->count != 0) return;

   if (theValue->markedEphemeral == FALSE)
     {
      AddEphemeralHashNode(theEnv,(GENERIC_HN *) theValue,&SymbolData(theEnv)->EphemeralSymbolList,
                           sizeof(SYMBOL_HN),AVERAGE_STRING_SIZE);
     }

   return;
  }

/***************************************************/
/* DecrementFloatCount: Decrements the count value */
/*   for a FloatTable entry. Adds the float to the */
/*   EphemeralFloatList if the count becomes zero. */
/***************************************************/
globle void DecrementFloatCount(
  void *theEnv,
  FLOAT_HN *theValue)
  {
   if (theValue->count <= 0)
     {
      SystemError(theEnv,"SYMBOL",5);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   theValue->count--;

   if (theValue->count != 0) return;

   if (theValue->markedEphemeral == FALSE)
     {
      AddEphemeralHashNode(theEnv,(GENERIC_HN *) theValue,&SymbolData(theEnv)->EphemeralFloatList,
                           sizeof(FLOAT_HN),0);
     }

   return;
  }

/*********************************************************/
/* DecrementIntegerCount: Decrements the count value for */
/*   an IntegerTable entry. Adds the integer to the      */
/*   EphemeralIntegerList if the count becomes zero.     */
/*********************************************************/
globle void DecrementIntegerCount(
  void *theEnv,
  INTEGER_HN *theValue)
  {
   if (theValue->count <= 0)
     {
      SystemError(theEnv,"SYMBOL",6);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   theValue->count--;

   if (theValue->count != 0) return;

   if (theValue->markedEphemeral == FALSE)
     {
      AddEphemeralHashNode(theEnv,(GENERIC_HN *) theValue,&SymbolData(theEnv)->EphemeralIntegerList,
                           sizeof(INTEGER_HN),0);
     }

   return;
  }

/*****************************************************/
/* DecrementBitMapCount: Decrements the count value  */
/*   for a BitmapTable entry. Adds the bitmap to the */
/*   EphemeralBitMapList if the count becomes zero.  */
/*****************************************************/
globle void DecrementBitMapCount(
  void *theEnv,
  BITMAP_HN *theValue)
  {
   if (theValue->count < 0)
     {
      SystemError(theEnv,"SYMBOL",7);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   if (theValue->count == 0)
     {
      SystemError(theEnv,"SYMBOL",8);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   theValue->count--;

   if (theValue->count != 0) return;

   if (theValue->markedEphemeral == FALSE)
     {
      AddEphemeralHashNode(theEnv,(GENERIC_HN *) theValue,&SymbolData(theEnv)->EphemeralBitMapList,
                           sizeof(BITMAP_HN),sizeof(long));
     }

   return;
  }

/*************************************************************/
/* DecrementExternalAddressCount: Decrements the count value */
/*   for an ExternAddressTable entry. Adds the bitmap to the */
/*   EphemeralExternalAddressList if the count becomes zero. */
/*************************************************************/
globle void DecrementExternalAddressCount(
  void *theEnv,
  EXTERNAL_ADDRESS_HN *theValue)
  {
   if (theValue->count < 0)
     {
      SystemError(theEnv,"SYMBOL",9);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   if (theValue->count == 0)
     {
      SystemError(theEnv,"SYMBOL",10);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   theValue->count--;

   if (theValue->count != 0) return;

   if (theValue->markedEphemeral == FALSE)
     {
      AddEphemeralHashNode(theEnv,(GENERIC_HN *) theValue,&SymbolData(theEnv)->EphemeralExternalAddressList,
                           sizeof(EXTERNAL_ADDRESS_HN),sizeof(long));
     }

   return;
  }
  
/************************************************/
/* RemoveHashNode: Removes a hash node from the */
/*   SymbolTable, FloatTable, IntegerTable,     */
/*   BitMapTable, or ExternalAddressTable.      */
/************************************************/
static void RemoveHashNode(
  void *theEnv,
  GENERIC_HN *theValue,
  GENERIC_HN **theTable,
  int size,
  int type)
  {
   GENERIC_HN *previousNode, *currentNode;
   struct externalAddressHashNode *theAddress;

   /*=============================================*/
   /* Find the entry in the specified hash table. */
   /*=============================================*/

   previousNode = NULL;
   currentNode = theTable[theValue->bucket];

   while (currentNode != theValue)
     {
      previousNode = currentNode;
      currentNode = currentNode->next;

      if (currentNode == NULL)
        {
         SystemError(theEnv,"SYMBOL",11);
         EnvExitRouter(theEnv,EXIT_FAILURE);
        }
     }

   /*===========================================*/
   /* Remove the entry from the list of entries */
   /* stored in the hash table bucket.          */
   /*===========================================*/

   if (previousNode == NULL)
     { theTable[theValue->bucket] = theValue->next; }
   else
     { previousNode->next = currentNode->next; }

   /*=================================================*/
   /* Symbol and bit map nodes have additional memory */
   /* use to store the character or bitmap string.    */
   /*=================================================*/

   if (type == SYMBOL)
     {
      rm(theEnv,((SYMBOL_HN *) theValue)->contents,
         strlen(((SYMBOL_HN *) theValue)->contents) + 1);
     }
   else if (type == BITMAPARRAY)
     {
      rm(theEnv,((BITMAP_HN *) theValue)->contents,
         ((BITMAP_HN *) theValue)->size);
     }
   else if (type == EXTERNAL_ADDRESS)
     {       
      theAddress = (struct externalAddressHashNode *) theValue;

      if ((EvaluationData(theEnv)->ExternalAddressTypes[theAddress->type] != NULL) &&
          (EvaluationData(theEnv)->ExternalAddressTypes[theAddress->type]->discardFunction != NULL))
        { (*EvaluationData(theEnv)->ExternalAddressTypes[theAddress->type]->discardFunction)(theEnv,theAddress->externalAddress); }
     }

   /*===========================*/
   /* Return the table entry to */
   /* the pool of free memory.  */
   /*===========================*/

   rtn_sized_struct(theEnv,size,theValue);
  }

/***********************************************************/
/* AddEphemeralHashNode: Adds a symbol, integer, float, or */
/*   bit map table entry to the list of ephemeral atomic   */
/*   values. These entries have a zero count indicating    */
/*   that no structure is using the data value.            */
/***********************************************************/
static void AddEphemeralHashNode(
  void *theEnv,
  GENERIC_HN *theHashNode,
  struct ephemeron **theEphemeralList,
  int hashNodeSize,
  int averageContentsSize)
  {
   struct ephemeron *temp;

   /*===========================================*/
   /* If the count isn't zero then this routine */
   /* should never have been called.            */
   /*===========================================*/

   if (theHashNode->count != 0)
     {
      SystemError(theEnv,"SYMBOL",12);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   /*=====================================*/
   /* Mark the atomic value as ephemeral. */
   /*=====================================*/

   theHashNode->markedEphemeral = TRUE;

   /*=============================*/
   /* Add the atomic value to the */
   /* list of ephemeral values.   */
   /*=============================*/

   temp = get_struct(theEnv,ephemeron);
   temp->associatedValue = theHashNode;
   temp->next = *theEphemeralList;
   *theEphemeralList = temp;

   /*=========================================================*/
   /* Increment the ephemeral count and size variables. These */
   /* variables are used by the garbage collection routines   */
   /* to determine when garbage collection should occur.      */
   /*=========================================================*/

   UtilityData(theEnv)->EphemeralItemCount++;
   UtilityData(theEnv)->EphemeralItemSize += sizeof(struct ephemeron) + hashNodeSize +
                        averageContentsSize;
  }

/***************************************************/
/* RemoveEphemeralAtoms: Causes the removal of all */
/*   ephemeral symbols, integers, floats, and bit  */
/*   maps that still have a count value of zero,   */
/*   from their respective storage tables.         */
/***************************************************/
globle void RemoveEphemeralAtoms(
  void *theEnv)
  {
   RemoveEphemeralHashNodes(theEnv,&SymbolData(theEnv)->EphemeralSymbolList,(GENERIC_HN **) SymbolData(theEnv)->SymbolTable,
                            sizeof(SYMBOL_HN),SYMBOL,AVERAGE_STRING_SIZE);
   RemoveEphemeralHashNodes(theEnv,&SymbolData(theEnv)->EphemeralFloatList,(GENERIC_HN **) SymbolData(theEnv)->FloatTable,
                            sizeof(FLOAT_HN),FLOAT,0);
   RemoveEphemeralHashNodes(theEnv,&SymbolData(theEnv)->EphemeralIntegerList,(GENERIC_HN **) SymbolData(theEnv)->IntegerTable,
                            sizeof(INTEGER_HN),INTEGER,0);
   RemoveEphemeralHashNodes(theEnv,&SymbolData(theEnv)->EphemeralBitMapList,(GENERIC_HN **) SymbolData(theEnv)->BitMapTable,
                            sizeof(BITMAP_HN),BITMAPARRAY,AVERAGE_BITMAP_SIZE);
   RemoveEphemeralHashNodes(theEnv,&SymbolData(theEnv)->EphemeralExternalAddressList,(GENERIC_HN **) SymbolData(theEnv)->ExternalAddressTable,
                            sizeof(EXTERNAL_ADDRESS_HN),EXTERNAL_ADDRESS,0);
  }

/****************************************************************/
/* RemoveEphemeralHashNodes: Removes symbols from the ephemeral */
/*   symbol list that have a count of zero and were placed on   */
/*   the list at a higher level than the current evaluation     */
/*   depth. Since symbols are ordered in the list in descending */
/*   order, the removal process can end when a depth is reached */
/*   less than the current evaluation depth. Because ephemeral  */
/*   symbols can be "pulled" up through an evaluation depth,    */
/*   this routine needs to check through both the previous and  */
/*   current evaluation depth.                                  */
/****************************************************************/
static void RemoveEphemeralHashNodes(
  void *theEnv,
  struct ephemeron **theEphemeralList,
  GENERIC_HN **theTable,
  int hashNodeSize,
  int hashNodeType,
  int averageContentsSize)
  {
   struct ephemeron *edPtr, *lastPtr = NULL, *nextPtr;

   edPtr = *theEphemeralList;

   while (edPtr != NULL)
     {
      /*======================================================*/
      /* Check through previous and current evaluation depth  */
      /* because these symbols can be interspersed, otherwise */
      /* symbols are stored in descending evaluation depth.   */
      /*======================================================*/

      nextPtr = edPtr->next;

      /*==================================================*/
      /* Remove any symbols that have a count of zero and */
      /* were added to the ephemeral list at a higher     */
      /* evaluation depth.                                */
      /*==================================================*/

      if ((edPtr->associatedValue->count == 0) &&
          (edPtr->associatedValue->depth > EvaluationData(theEnv)->CurrentEvaluationDepth))
        {
         RemoveHashNode(theEnv,edPtr->associatedValue,theTable,hashNodeSize,hashNodeType);
         rtn_struct(theEnv,ephemeron,edPtr);
         if (lastPtr == NULL) *theEphemeralList = nextPtr;
         else lastPtr->next = nextPtr;
         UtilityData(theEnv)->EphemeralItemCount--;
         UtilityData(theEnv)->EphemeralItemSize -= sizeof(struct ephemeron) + hashNodeSize +
                              averageContentsSize;
        }

      /*=======================================*/
      /* Remove ephemeral status of any symbol */
      /* with a count greater than zero.       */
      /*=======================================*/

      else if (edPtr->associatedValue->count > 0)
        {
         edPtr->associatedValue->markedEphemeral = FALSE;

         rtn_struct(theEnv,ephemeron,edPtr);

         if (lastPtr == NULL) *theEphemeralList = nextPtr;
         else lastPtr->next = nextPtr;
         UtilityData(theEnv)->EphemeralItemCount--;
         UtilityData(theEnv)->EphemeralItemSize -= sizeof(struct ephemeron) + hashNodeSize +
                              averageContentsSize;
        }

      /*==================================================*/
      /* Otherwise keep the symbol in the ephemeral list. */
      /*==================================================*/

      else
        { lastPtr = edPtr; }

      edPtr = nextPtr;
     }
  }

/*********************************************************/
/* GetSymbolTable: Returns a pointer to the SymbolTable. */
/*********************************************************/
globle SYMBOL_HN **GetSymbolTable(
  void *theEnv)
  {
   return(SymbolData(theEnv)->SymbolTable);
  }

/******************************************************/
/* SetSymbolTable: Sets the value of the SymbolTable. */
/******************************************************/
globle void SetSymbolTable(
  void *theEnv,
  SYMBOL_HN **value)
  {
   SymbolData(theEnv)->SymbolTable = value;
  }

/*******************************************************/
/* GetFloatTable: Returns a pointer to the FloatTable. */
/*******************************************************/
globle FLOAT_HN **GetFloatTable(
  void *theEnv)
  {
   return(SymbolData(theEnv)->FloatTable);
  }

/****************************************************/
/* SetFloatTable: Sets the value of the FloatTable. */
/****************************************************/
globle void SetFloatTable(
  void *theEnv,
  FLOAT_HN **value)
  {
   SymbolData(theEnv)->FloatTable = value;
  }

/***********************************************************/
/* GetIntegerTable: Returns a pointer to the IntegerTable. */
/***********************************************************/
globle INTEGER_HN **GetIntegerTable(
  void *theEnv)
  {
   return(SymbolData(theEnv)->IntegerTable);
  }

/********************************************************/
/* SetIntegerTable: Sets the value of the IntegerTable. */
/********************************************************/
globle void SetIntegerTable(
  void *theEnv,
  INTEGER_HN **value)
  {
   SymbolData(theEnv)->IntegerTable = value;
  }

/*********************************************************/
/* GetBitMapTable: Returns a pointer to the BitMapTable. */
/*********************************************************/
globle BITMAP_HN **GetBitMapTable(
  void *theEnv)
  {
   return(SymbolData(theEnv)->BitMapTable);
  }

/******************************************************/
/* SetBitMapTable: Sets the value of the BitMapTable. */
/******************************************************/
globle void SetBitMapTable(
  void *theEnv,
  BITMAP_HN **value)
  {
   SymbolData(theEnv)->BitMapTable = value;
  }

/***************************************************************************/
/* GetExternalAddressTable: Returns a pointer to the ExternalAddressTable. */
/***************************************************************************/
globle EXTERNAL_ADDRESS_HN **GetExternalAddressTable(
  void *theEnv)
  {
   return(SymbolData(theEnv)->ExternalAddressTable);
  }

/************************************************************************/
/* SetExternalAddressTable: Sets the value of the ExternalAddressTable. */
/************************************************************************/
globle void SetExternalAddressTable(
  void *theEnv,
  EXTERNAL_ADDRESS_HN **value)
  {
   SymbolData(theEnv)->ExternalAddressTable = value;
  }

/******************************************************/
/* RefreshSpecialSymbols: Resets the values of the    */
/*   TrueSymbol, FalseSymbol, Zero, PositiveInfinity, */
/*   and NegativeInfinity symbols.                    */
/******************************************************/
globle void RefreshSpecialSymbols(
  void *theEnv)
  {
   SymbolData(theEnv)->TrueSymbolHN = (void *) FindSymbolHN(theEnv,TRUE_STRING);
   SymbolData(theEnv)->FalseSymbolHN = (void *) FindSymbolHN(theEnv,FALSE_STRING);
   SymbolData(theEnv)->PositiveInfinity = (void *) FindSymbolHN(theEnv,POSITIVE_INFINITY_STRING);
   SymbolData(theEnv)->NegativeInfinity = (void *) FindSymbolHN(theEnv,NEGATIVE_INFINITY_STRING);
   SymbolData(theEnv)->Zero = (void *) FindLongHN(theEnv,0L);
  }

/***********************************************************/
/* FindSymbolMatches: Finds all symbols in the SymbolTable */
/*   which begin with a specified symbol. This function is */
/*   used to implement the command completion feature      */
/*   found in some of the machine specific interfaces.     */
/***********************************************************/
globle struct symbolMatch *FindSymbolMatches(
  void *theEnv,
  char *searchString,
  unsigned *numberOfMatches,
  size_t *commonPrefixLength)
  {
   struct symbolMatch *reply = NULL, *temp;
   struct symbolHashNode *hashPtr = NULL;
   size_t searchLength;

   searchLength = strlen(searchString);
   *numberOfMatches = 0;

   while ((hashPtr = GetNextSymbolMatch(theEnv,searchString,searchLength,hashPtr,
                                        FALSE,commonPrefixLength)) != NULL)
     {
      *numberOfMatches = *numberOfMatches + 1;
      temp = get_struct(theEnv,symbolMatch);
      temp->match = hashPtr;
      temp->next = reply;
      reply = temp;
     }

   return(reply);
  }

/*********************************************************/
/* ReturnSymbolMatches: Returns a set of symbol matches. */
/*********************************************************/
globle void ReturnSymbolMatches(
  void *theEnv,
  struct symbolMatch *listOfMatches)
  {
   struct symbolMatch *temp;

   while (listOfMatches != NULL)
     {
      temp = listOfMatches->next;
      rtn_struct(theEnv,symbolMatch,listOfMatches);
      listOfMatches = temp;
     }
  }

/***************************************************************/
/* ClearBitString: Initializes the values of a bitmap to zero. */
/***************************************************************/
globle void ClearBitString(
  void *vTheBitMap,
  unsigned length)
  {
   char *theBitMap = (char *) vTheBitMap;
   unsigned i;

   for (i = 0; i < length; i++) theBitMap[i] = '\0';
  }

/*****************************************************************/
/* GetNextSymbolMatch: Finds the next symbol in the SymbolTable  */
/*   which begins with a specified symbol. This function is used */
/*   to implement the command completion feature found in some   */
/*   of the machine specific interfaces.                         */
/*****************************************************************/
globle SYMBOL_HN *GetNextSymbolMatch(
  void *theEnv,
  char *searchString,
  size_t searchLength,
  SYMBOL_HN *prevSymbol,
  int anywhere,
  size_t *commonPrefixLength)
  {
   register unsigned long i;
   SYMBOL_HN *hashPtr;
   int flag = TRUE;
   size_t prefixLength;

   /*==========================================*/
   /* If we're looking anywhere in the string, */
   /* then there's no common prefix length.    */
   /*==========================================*/

   if (anywhere && (commonPrefixLength != NULL))
     *commonPrefixLength = 0;

   /*========================================================*/
   /* If we're starting the search from the beginning of the */
   /* symbol table, the previous symbol argument is NULL.    */
   /*========================================================*/

   if (prevSymbol == NULL)
     {
      i = 0;
      hashPtr = SymbolData(theEnv)->SymbolTable[0];
     }

   /*==========================================*/
   /* Otherwise start the search at the symbol */
   /* after the last symbol found.             */
   /*==========================================*/

   else
     {
      i = prevSymbol->bucket;
      hashPtr = prevSymbol->next;
     }

   /*==============================================*/
   /* Search through all the symbol table buckets. */
   /*==============================================*/

   while (flag)
     {
      /*===================================*/
      /* Search through all of the entries */
      /* in the bucket being examined.     */
      /*===================================*/

      for (; hashPtr != NULL; hashPtr = hashPtr->next)
        {
         /*================================================*/
         /* Skip symbols that being with ( since these are */
         /* typically symbols for internal use. Also skip  */
         /* any symbols that are marked ephemeral since    */
         /* these aren't in use.                           */
         /*================================================*/

         if ((hashPtr->contents[0] == '(') ||
             (hashPtr->markedEphemeral))
           { continue; }

         /*==================================================*/
         /* Two types of matching can be performed: the type */
         /* comparing just to the beginning of the string    */
         /* and the type which looks for the substring       */
         /* anywhere within the string being examined.       */
         /*==================================================*/

         if (! anywhere)
           {
            /*=============================================*/
            /* Determine the common prefix length between  */
            /* the previously found match (if available or */
            /* the search string if not) and the symbol    */
            /* table entry.                                */
            /*=============================================*/

            if (prevSymbol != NULL)
              prefixLength = CommonPrefixLength(prevSymbol->contents,hashPtr->contents);
            else
              prefixLength = CommonPrefixLength(searchString,hashPtr->contents);

            /*===================================================*/
            /* If the prefix length is greater than or equal to  */
            /* the length of the search string, then we've found */
            /* a match. If this is the first match, the common   */
            /* prefix length is set to the length of the first   */
            /* match, otherwise the common prefix length is the  */
            /* smallest prefix length found among all matches.   */
            /*===================================================*/

            if (prefixLength >= searchLength)
              {
               if (commonPrefixLength != NULL)
                 {
                  if (prevSymbol == NULL)
                    *commonPrefixLength = strlen(hashPtr->contents);
                  else if (prefixLength < *commonPrefixLength)
                    *commonPrefixLength = prefixLength;
                 }
               return(hashPtr);
              }
           }
         else
           {
            if (StringWithinString(hashPtr->contents,searchString) != NULL)
              { return(hashPtr); }
           }
        }

      /*=================================================*/
      /* Move on to the next bucket in the symbol table. */
      /*=================================================*/

      if (++i >= SYMBOL_HASH_SIZE) flag = FALSE;
      else hashPtr = SymbolData(theEnv)->SymbolTable[i];
     }

   /*=====================================*/
   /* There are no more matching symbols. */
   /*=====================================*/

   return(NULL);
  }

/**********************************************/
/* StringWithinString: Determines if a string */
/*   is contained within another string.      */
/**********************************************/
static char *StringWithinString(
  char *cs,
  char *ct)
  {
   register unsigned i,j,k;

   for (i = 0 ; cs[i] != '\0' ; i++)
     {
      for (j = i , k = 0 ; ct[k] != '\0' && cs[j] == ct[k] ; j++, k++) ;
      if ((ct[k] == '\0') && (k != 0))
        return(cs + i);
     }
   return(NULL);
  }

/************************************************/
/* CommonPrefixLength: Determines the length of */
/*    the maximumcommon prefix of two strings   */
/************************************************/
static size_t CommonPrefixLength(
  char *cs,
  char *ct)
  {
   register unsigned i;

   for (i = 0 ; (cs[i] != '\0') && (ct[i] != '\0') ; i++)
     if (cs[i] != ct[i])
       break;
   return(i);
  }

#if BLOAD_AND_BSAVE || CONSTRUCT_COMPILER || BSAVE_INSTANCES

/****************************************************************/
/* SetAtomicValueIndices: Sets the bucket values for hash table */
/*   entries with an index value that indicates the position of */
/*   the hash table in a hash table traversal (e.g. this is the */
/*   fifth entry in the  hash table.                            */
/****************************************************************/
globle void SetAtomicValueIndices(
  void *theEnv,
  int setAll)
  {
   unsigned long count;
   unsigned long i;
   SYMBOL_HN *symbolPtr, **symbolArray;
   FLOAT_HN *floatPtr, **floatArray;
   INTEGER_HN *integerPtr, **integerArray;
   BITMAP_HN *bitMapPtr, **bitMapArray;

   /*===================================*/
   /* Set indices for the symbol table. */
   /*===================================*/

   count = 0;
   symbolArray = GetSymbolTable(theEnv);

   for (i = 0; i < SYMBOL_HASH_SIZE; i++)
     {
      for (symbolPtr = symbolArray[i];
           symbolPtr != NULL;
           symbolPtr = symbolPtr->next)
        {
         if ((symbolPtr->neededSymbol == TRUE) || setAll)
           {
            symbolPtr->bucket = count++;
            if (symbolPtr->bucket != (count - 1))
              { SystemError(theEnv,"SYMBOL",13); }
           }
        }
     }

   /*==================================*/
   /* Set indices for the float table. */
   /*==================================*/

   count = 0;
   floatArray = GetFloatTable(theEnv);

   for (i = 0; i < FLOAT_HASH_SIZE; i++)
     {
      for (floatPtr = floatArray[i];
           floatPtr != NULL;
           floatPtr = floatPtr->next)
        {
         if ((floatPtr->neededFloat == TRUE) || setAll)
           {
            floatPtr->bucket = count++;
            if (floatPtr->bucket != (count - 1))
              { SystemError(theEnv,"SYMBOL",14); }
           }
        }
     }

   /*====================================*/
   /* Set indices for the integer table. */
   /*====================================*/

   count = 0;
   integerArray = GetIntegerTable(theEnv);

   for (i = 0; i < INTEGER_HASH_SIZE; i++)
     {
      for (integerPtr = integerArray[i];
           integerPtr != NULL;
           integerPtr = integerPtr->next)
        {
         if ((integerPtr->neededInteger == TRUE) || setAll)
           {
            integerPtr->bucket = count++;
            if (integerPtr->bucket != (count - 1))
              { SystemError(theEnv,"SYMBOL",15); }
           }
        }
     }

   /*===================================*/
   /* Set indices for the bitmap table. */
   /*===================================*/

   count = 0;
   bitMapArray = GetBitMapTable(theEnv);

   for (i = 0; i < BITMAP_HASH_SIZE; i++)
     {
      for (bitMapPtr = bitMapArray[i];
           bitMapPtr != NULL;
           bitMapPtr = bitMapPtr->next)
        {
         if ((bitMapPtr->neededBitMap == TRUE) || setAll)
           {
            bitMapPtr->bucket = count++;
            if (bitMapPtr->bucket != (count - 1))
              { SystemError(theEnv,"SYMBOL",16); }
           }
        }
     }
  }

/***********************************************************************/
/* RestoreAtomicValueBuckets: Restores the bucket values of hash table */
/*   entries to the appropriate values. Normally called to undo the    */
/*   effects of a call to the SetAtomicValueIndices function.          */
/***********************************************************************/
globle void RestoreAtomicValueBuckets(
  void *theEnv)
  {
   unsigned long i;
   SYMBOL_HN *symbolPtr, **symbolArray;
   FLOAT_HN *floatPtr, **floatArray;
   INTEGER_HN *integerPtr, **integerArray;
   BITMAP_HN *bitMapPtr, **bitMapArray;

   /*================================================*/
   /* Restore the bucket values in the symbol table. */
   /*================================================*/

   symbolArray = GetSymbolTable(theEnv);

   for (i = 0; i < SYMBOL_HASH_SIZE; i++)
     {
      for (symbolPtr = symbolArray[i];
           symbolPtr != NULL;
           symbolPtr = symbolPtr->next)
        { symbolPtr->bucket = i; }
     }

   /*===============================================*/
   /* Restore the bucket values in the float table. */
   /*===============================================*/

   floatArray = GetFloatTable(theEnv);

   for (i = 0; i < FLOAT_HASH_SIZE; i++)
     {
      for (floatPtr = floatArray[i];
           floatPtr != NULL;
           floatPtr = floatPtr->next)
        { floatPtr->bucket = i; }
     }

   /*=================================================*/
   /* Restore the bucket values in the integer table. */
   /*=================================================*/

   integerArray = GetIntegerTable(theEnv);

   for (i = 0; i < INTEGER_HASH_SIZE; i++)
     {
      for (integerPtr = integerArray[i];
           integerPtr != NULL;
           integerPtr = integerPtr->next)
        { integerPtr->bucket = i; }
     }

   /*================================================*/
   /* Restore the bucket values in the bitmap table. */
   /*================================================*/

   bitMapArray = GetBitMapTable(theEnv);

   for (i = 0; i < BITMAP_HASH_SIZE; i++)
     {
      for (bitMapPtr = bitMapArray[i];
           bitMapPtr != NULL;
           bitMapPtr = bitMapPtr->next)
        { bitMapPtr->bucket = i; }
     }
  }

#endif /* BLOAD_AND_BSAVE || CONSTRUCT_COMPILER || BSAVE_INSTANCES */
