   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  05/17/06            */
   /*                                                     */
   /*                 FACT HASHING MODULE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for maintaining a fact hash    */
/*   table so that duplication of facts can quickly be       */
/*   determined.                                             */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Removed LOGICAL_DEPENDENCIES compilation flag. */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#define _FACTHSH_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT

#include "constant.h"
#include "memalloc.h"
#include "router.h"
#include "sysdep.h"
#include "envrnmnt.h"

#if DEFRULE_CONSTRUCT
#include "lgcldpnd.h"
#endif

#include "facthsh.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static struct fact            *FactExists(void *,struct fact *,unsigned long);
   static struct factHashEntry  **CreateFactHashTable(void *,unsigned long);
   static void                    ResizeFactHashTable(void *);
   static void                    ResetFactHashTable(void *);
   
/************************************************/
/* HashFact: Returns the hash value for a fact. */
/************************************************/
unsigned long HashFact(
  struct fact *theFact)
  {
   unsigned long count = 0;

   /*============================================*/
   /* Get a hash value for the deftemplate name. */
   /*============================================*/

   count += (unsigned long) theFact->whichDeftemplate->header.name->bucket * 73981;

   /*=================================================*/
   /* Add in the hash value for the rest of the fact. */
   /*=================================================*/

   count += HashMultifield(&theFact->theProposition,0);

   /*================================*/
   /* Make sure the hash value falls */
   /* in the appropriate range.      */
   /*================================*/

   theFact->hashValue = count;

   /*========================*/
   /* Return the hash value. */
   /*========================*/

   return(count);
  }

/**********************************************/
/* FactExists: Determines if a specified fact */
/*   already exists in the fact hash table.   */
/**********************************************/
static struct fact *FactExists(
  void *theEnv,
  struct fact *theFact,
  unsigned long hashValue)
  {
   struct factHashEntry *theFactHash;

   hashValue = (hashValue % FactData(theEnv)->FactHashTableSize);

   for (theFactHash = FactData(theEnv)->FactHashTable[hashValue];
        theFactHash != NULL;
        theFactHash = theFactHash->next)
     {
      if (theFact->hashValue != theFactHash->theFact->hashValue)
        { continue; }

      if ((theFact->whichDeftemplate == theFactHash->theFact->whichDeftemplate) ?
          MultifieldsEqual(&theFact->theProposition,
                           &theFactHash->theFact->theProposition) : FALSE)
        { return(theFactHash->theFact); }
     }

   return(NULL);
  }

/************************************************************/
/* AddHashedFact: Adds a fact entry to the fact hash table. */
/************************************************************/
globle void AddHashedFact(
  void *theEnv,
  struct fact *theFact,
  unsigned long hashValue)
  {
   struct factHashEntry *newhash, *temp;

   if (FactData(theEnv)->NumberOfFacts > FactData(theEnv)->FactHashTableSize)
     { ResizeFactHashTable(theEnv); }

   newhash = get_struct(theEnv,factHashEntry);
   newhash->theFact = theFact;

   hashValue = (hashValue % FactData(theEnv)->FactHashTableSize);
   
   temp = FactData(theEnv)->FactHashTable[hashValue];
   FactData(theEnv)->FactHashTable[hashValue] = newhash;
   newhash->next = temp;
  }

/******************************************/
/* RemoveHashedFact: Removes a fact entry */
/*   from the fact hash table.            */
/******************************************/
globle intBool RemoveHashedFact(
  void *theEnv,
  struct fact *theFact)
  {
   int hashValue;
   struct factHashEntry *hptr, *prev;

   hashValue = HashFact(theFact);
   hashValue = (hashValue % FactData(theEnv)->FactHashTableSize);

   for (hptr = FactData(theEnv)->FactHashTable[hashValue], prev = NULL;
        hptr != NULL;
        hptr = hptr->next)
     {
      if (hptr->theFact == theFact)
        {
         if (prev == NULL)
           {
            FactData(theEnv)->FactHashTable[hashValue] = hptr->next;
            rtn_struct(theEnv,factHashEntry,hptr);
            if (FactData(theEnv)->NumberOfFacts == 1)
              { ResetFactHashTable(theEnv); }
            return(1);
           }
         else
           {
            prev->next = hptr->next;
            rtn_struct(theEnv,factHashEntry,hptr);
            if (FactData(theEnv)->NumberOfFacts == 1)
              { ResetFactHashTable(theEnv); }
            return(1);
           }
        }
      prev = hptr;
     }

   return(0);
  }

/*****************************************************/
/* HandleFactDuplication: Determines if a fact to be */
/*   added to the fact-list is a duplicate entry and */
/*   takes appropriate action based on the current   */
/*   setting of the fact-duplication flag.           */
/*****************************************************/
globle unsigned long HandleFactDuplication(
  void *theEnv,
  void *theFact,
  intBool *duplicate)
  {
   struct fact *tempPtr;
   unsigned long hashValue;
   *duplicate = FALSE;
   
   hashValue = HashFact((struct fact *) theFact);

   if (FactData(theEnv)->FactDuplication) return(hashValue);

   tempPtr = FactExists(theEnv,(struct fact *) theFact,hashValue);
   if (tempPtr == NULL) return(hashValue);

   ReturnFact(theEnv,(struct fact *) theFact);
#if DEFRULE_CONSTRUCT
   AddLogicalDependencies(theEnv,(struct patternEntity *) tempPtr,TRUE);
#endif
   *duplicate = TRUE;
   return(0);
  }

/*******************************************/
/* EnvGetFactDuplication: C access routine */
/*   for the get-fact-duplication command. */
/*******************************************/
globle intBool EnvGetFactDuplication(
  void *theEnv)
  {   
   return(FactData(theEnv)->FactDuplication); 
  }

/*******************************************/
/* EnvSetFactDuplication: C access routine */
/*   for the set-fact-duplication command. */
/*******************************************/
globle intBool EnvSetFactDuplication(
  void *theEnv,
  int value)
  {
   int ov;

   ov = FactData(theEnv)->FactDuplication;
   FactData(theEnv)->FactDuplication = value;
   return(ov);
  }

/**************************************************/
/* InitializeFactHashTable: Initializes the table */
/*   entries in the fact hash table to NULL.      */
/**************************************************/
globle void InitializeFactHashTable(
   void *theEnv)
   {
    FactData(theEnv)->FactHashTable = CreateFactHashTable(theEnv,SIZE_FACT_HASH);
    FactData(theEnv)->FactHashTableSize = SIZE_FACT_HASH;
   }

/*******************************************************************/
/* CreateFactHashTable: Creates and initializes a fact hash table. */
/*******************************************************************/
static struct factHashEntry **CreateFactHashTable(
   void *theEnv,
   unsigned long tableSize)
   {
    unsigned long i;
    struct factHashEntry **theTable;

    theTable = (struct factHashEntry **)
               gm3(theEnv,sizeof (struct factHashEntry *) * tableSize);

    if (theTable == NULL) EnvExitRouter(theEnv,EXIT_FAILURE);
    
    for (i = 0; i < tableSize; i++) theTable[i] = NULL;
    
    return(theTable);
   }
 
/*******************************************************************/
/* ResizeFactHashTable: */
/*******************************************************************/
static void ResizeFactHashTable(
   void *theEnv)
   {
    unsigned long i, newSize, newLocation;
    struct factHashEntry **theTable, **newTable;
    struct factHashEntry *theEntry, *nextEntry;

    theTable = FactData(theEnv)->FactHashTable;
    
    newSize = (FactData(theEnv)->FactHashTableSize * 2) + 1;
    newTable = CreateFactHashTable(theEnv,newSize);

    /*========================================*/
    /* Copy the old entries to the new table. */
    /*========================================*/
    
    for (i = 0; i < FactData(theEnv)->FactHashTableSize; i++)
      {
       theEntry = theTable[i];
       while (theEntry != NULL)
         { 
          nextEntry = theEntry->next;
          
          newLocation = theEntry->theFact->hashValue % newSize;
          theEntry->next = newTable[newLocation];
          newTable[newLocation] = theEntry;
          
          theEntry = nextEntry;
         }
      }
    
    /*=====================================================*/
    /* Replace the old hash table with the new hash table. */
    /*=====================================================*/
    
    rm3(theEnv,theTable,sizeof(struct factHashEntry *) * FactData(theEnv)->FactHashTableSize);
    FactData(theEnv)->FactHashTableSize = newSize;
    FactData(theEnv)->FactHashTable = newTable;
   }

/*******************************************************************/
/* ResetFactHashTable: */
/*******************************************************************/
static void ResetFactHashTable(
   void *theEnv)
   {
    struct factHashEntry **newTable;

    /*=============================================*/
    /* Don't reset the table unless the hash table */
    /* has been expanded from its original size.   */
    /*=============================================*/
    
    if (FactData(theEnv)->FactHashTableSize == SIZE_FACT_HASH)
      { return; }
          
    /*=======================*/
    /* Create the new table. */
    /*=======================*/
    
    newTable = CreateFactHashTable(theEnv,SIZE_FACT_HASH);
    
    /*=====================================================*/
    /* Replace the old hash table with the new hash table. */
    /*=====================================================*/
    
    rm3(theEnv,FactData(theEnv)->FactHashTable,sizeof(struct factHashEntry *) * FactData(theEnv)->FactHashTableSize);
    FactData(theEnv)->FactHashTableSize = SIZE_FACT_HASH;
    FactData(theEnv)->FactHashTable = newTable;
   }
      
#if DEVELOPER

/*****************************************************/
/* ShowFactHashTable: Displays the number of entries */
/*   in each slot of the fact hash table.            */
/*****************************************************/
globle void ShowFactHashTable(
   void *theEnv)
   {
    int i, count;
    struct factHashEntry *theEntry;
    char buffer[20];

    for (i = 0; i < FactData(theEnv)->FactHashTableSize; i++)
      {
       for (theEntry =  FactData(theEnv)->FactHashTable[i], count = 0;
            theEntry != NULL;
            theEntry = theEntry->next)
         { count++; }

       if (count != 0)
         {
          gensprintf(buffer,"%4d: %4d\n",i,count);
          EnvPrintRouter(theEnv,WDISPLAY,buffer);
         }
      }
   }

#endif /* DEVELOPER */

#endif /* DEFTEMPLATE_CONSTRUCT */

