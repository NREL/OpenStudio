   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*                    MEMORY MODULE                    */
   /*******************************************************/

/*************************************************************/
/* Purpose: Memory allocation routines.                      */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Removed HaltExecution check from the           */
/*            EnvReleaseMem function. DR0863                 */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Corrected code to remove compiler warnings.    */
/*                                                           */
/*************************************************************/

#define _MEMORY_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "setup.h"

#include "constant.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "router.h"
#include "utility.h"

#include <stdlib.h>

#if WIN_BTC
#include <alloc.h>
#endif
#if WIN_MVC
#include <malloc.h>
#endif

#define STRICT_ALIGN_SIZE sizeof(double)

#define SpecialMalloc(sz) malloc((STD_SIZE) sz)
#define SpecialFree(ptr) free(ptr)

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if BLOCK_MEMORY
   static int                     InitializeBlockMemory(void *,unsigned int);
   static int                     AllocateBlock(void *,struct blockInfo *,unsigned int);
   static void                    AllocateChunk(void *,struct blockInfo *,struct chunkInfo *,size_t);
#endif

/********************************************/
/* InitializeMemory: Sets up memory tables. */
/********************************************/
globle void InitializeMemory(
  void *theEnv)
  {
   int i;

   AllocateEnvironmentData(theEnv,MEMORY_DATA,sizeof(struct memoryData),NULL);

   MemoryData(theEnv)->OutOfMemoryFunction = DefaultOutOfMemoryFunction;
   
   MemoryData(theEnv)->MemoryTable = (struct memoryPtr **)
                 malloc((STD_SIZE) (sizeof(struct memoryPtr *) * MEM_TABLE_SIZE));

   if (MemoryData(theEnv)->MemoryTable == NULL)
     {
      PrintErrorID(theEnv,"MEMORY",1,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Out of memory.\n");
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   for (i = 0; i < MEM_TABLE_SIZE; i++) MemoryData(theEnv)->MemoryTable[i] = NULL;
  }

/***************************************************/
/* genalloc: A generic memory allocation function. */
/***************************************************/
globle void *genalloc(
  void *theEnv,
  size_t size)
  {
   char *memPtr;
               
#if   BLOCK_MEMORY
   memPtr = (char *) RequestChunk(theEnv,size);
   if (memPtr == NULL)
     {
      EnvReleaseMem(theEnv,(long) ((size * 5 > 4096) ? size * 5 : 4096),FALSE);
      memPtr = (char *) RequestChunk(theEnv,size);
      if (memPtr == NULL)
        {
         EnvReleaseMem(theEnv,-1L,TRUE);
         memPtr = (char *) RequestChunk(theEnv,size);
         while (memPtr == NULL)
           {
            if ((*MemoryData(theEnv)->OutOfMemoryFunction)(theEnv,(unsigned long) size))
              return(NULL);
            memPtr = (char *) RequestChunk(theEnv,size);
           }
        }
     }
#else
   memPtr = (char *) malloc(size);
        
   if (memPtr == NULL)
     {
      EnvReleaseMem(theEnv,(long) ((size * 5 > 4096) ? size * 5 : 4096),FALSE);
      memPtr = (char *) malloc(size);
      if (memPtr == NULL)
        {
         EnvReleaseMem(theEnv,-1L,TRUE);
         memPtr = (char *) malloc(size);
         while (memPtr == NULL)
           {
            if ((*MemoryData(theEnv)->OutOfMemoryFunction)(theEnv,size))
              return(NULL);
            memPtr = (char *) malloc(size);
           }
        }
     }
#endif

   MemoryData(theEnv)->MemoryAmount += (long) size;
   MemoryData(theEnv)->MemoryCalls++;

   return((void *) memPtr);
  }

/***********************************************/
/* DefaultOutOfMemoryFunction: Function called */
/*   when the KB runs out of memory.           */
/***********************************************/
#if WIN_BTC
#pragma argsused
#endif
globle int DefaultOutOfMemoryFunction(
  void *theEnv,
  size_t size)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(size)
#endif

   PrintErrorID(theEnv,"MEMORY",1,TRUE);
   EnvPrintRouter(theEnv,WERROR,"Out of memory.\n");
   EnvExitRouter(theEnv,EXIT_FAILURE);
   return(TRUE);
  }

/***********************************************************/
/* EnvSetOutOfMemoryFunction: Allows the function which is */
/*   called when the KB runs out of memory to be changed.  */
/***********************************************************/
globle int (*EnvSetOutOfMemoryFunction(void *theEnv,int (*functionPtr)(void *,size_t)))(void *,size_t)
  {
   int (*tmpPtr)(void *,size_t);

   tmpPtr = MemoryData(theEnv)->OutOfMemoryFunction;
   MemoryData(theEnv)->OutOfMemoryFunction = functionPtr;
   return(tmpPtr);
  }

/****************************************************/
/* genfree: A generic memory deallocation function. */
/****************************************************/
globle int genfree(
  void *theEnv,
  void *waste,
  size_t size)
  {   
#if BLOCK_MEMORY
   if (ReturnChunk(theEnv,waste,size) == FALSE)
     {
      PrintErrorID(theEnv,"MEMORY",2,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Release error in genfree.\n");
      return(-1);
     }
#else
   free(waste);
#endif

   MemoryData(theEnv)->MemoryAmount -= (long) size;
   MemoryData(theEnv)->MemoryCalls--;

   return(0);
  }

/******************************************************/
/* genrealloc: Simple (i.e. dumb) version of realloc. */
/******************************************************/
globle void *genrealloc(
  void *theEnv,
  void *oldaddr,
  size_t oldsz,
  size_t newsz)
  {
   char *newaddr;
   unsigned i;
   size_t limit;

   newaddr = ((newsz != 0) ? (char *) gm2(theEnv,newsz) : NULL);

   if (oldaddr != NULL)
     {
      limit = (oldsz < newsz) ? oldsz : newsz;
      for (i = 0 ; i < limit ; i++)
        { newaddr[i] = ((char *) oldaddr)[i]; }
      for ( ; i < newsz; i++)
        { newaddr[i] = '\0'; }
      rm(theEnv,(void *) oldaddr,oldsz);
     }

   return((void *) newaddr);
  }

/********************************/
/* EnvMemUsed: C access routine */
/*   for the mem-used command.  */
/********************************/
globle long int EnvMemUsed(
  void *theEnv)
  {
   return(MemoryData(theEnv)->MemoryAmount);
  }

/************************************/
/* EnvMemRequests: C access routine */
/*   for the mem-requests command.  */
/************************************/
globle long int EnvMemRequests(
  void *theEnv)
  {
   return(MemoryData(theEnv)->MemoryCalls);
  }

/***************************************/
/* UpdateMemoryUsed: Allows the amount */
/*   of memory used to be updated.     */
/***************************************/
globle long int UpdateMemoryUsed(
  void *theEnv,
  long int value)
  {
   MemoryData(theEnv)->MemoryAmount += value;
   return(MemoryData(theEnv)->MemoryAmount);
  }

/*******************************************/
/* UpdateMemoryRequests: Allows the number */
/*   of memory requests to be updated.     */
/*******************************************/
globle long int UpdateMemoryRequests(
  void *theEnv,
  long int value)
  {
   MemoryData(theEnv)->MemoryCalls += value;
   return(MemoryData(theEnv)->MemoryCalls);
  }

/***********************************/
/* EnvReleaseMem: C access routine */
/*   for the release-mem command.  */
/***********************************/
globle long int EnvReleaseMem(
  void *theEnv,
  long int maximum,
  int printMessage)
  {
   struct memoryPtr *tmpPtr, *memPtr;
   int i;
   long int returns = 0;
   long int amount = 0;

   if (printMessage == TRUE)
     { EnvPrintRouter(theEnv,WDIALOG,"\n*** DEALLOCATING MEMORY ***\n"); }

   for (i = (MEM_TABLE_SIZE - 1) ; i >= (int) sizeof(char *) ; i--)
     {
      YieldTime(theEnv);
      memPtr = MemoryData(theEnv)->MemoryTable[i];
      while (memPtr != NULL)
        {
         tmpPtr = memPtr->next;
         genfree(theEnv,(void *) memPtr,(unsigned) i);
         memPtr = tmpPtr;
         amount += i;
         returns++;
         if ((returns % 100) == 0)
           { YieldTime(theEnv); }
        }
      MemoryData(theEnv)->MemoryTable[i] = NULL;
      if ((amount > maximum) && (maximum > 0))
        {
         if (printMessage == TRUE)
           { EnvPrintRouter(theEnv,WDIALOG,"*** MEMORY  DEALLOCATED ***\n"); }
         return(amount);
        }
     }

   if (printMessage == TRUE)
     { EnvPrintRouter(theEnv,WDIALOG,"*** MEMORY  DEALLOCATED ***\n"); }

   return(amount);
  }

/*****************************************************/
/* gm1: Allocates memory and sets all bytes to zero. */
/*****************************************************/
globle void *gm1(
  void *theEnv,
  size_t size)
  {
   struct memoryPtr *memPtr;
   char *tmpPtr;
   size_t i;

   if (size < (long) sizeof(char *)) size = sizeof(char *);

   if (size >= MEM_TABLE_SIZE)
     {
      tmpPtr = (char *) genalloc(theEnv,(unsigned) size);
      for (i = 0 ; i < size ; i++)
        { tmpPtr[i] = '\0'; }
      return((void *) tmpPtr);
     }

   memPtr = (struct memoryPtr *) MemoryData(theEnv)->MemoryTable[size];
   if (memPtr == NULL)
     {
      tmpPtr = (char *) genalloc(theEnv,(unsigned) size);
      for (i = 0 ; i < size ; i++)
        { tmpPtr[i] = '\0'; }
      return((void *) tmpPtr);
     }

   MemoryData(theEnv)->MemoryTable[size] = memPtr->next;

   tmpPtr = (char *) memPtr;
   for (i = 0 ; i < size ; i++)
     { tmpPtr[i] = '\0'; }

   return ((void *) tmpPtr);
  }

/*****************************************************/
/* gm2: Allocates memory and does not initialize it. */
/*****************************************************/
globle void *gm2(
  void *theEnv,
  size_t size)
  {
   struct memoryPtr *memPtr;
   
   if (size < sizeof(char *)) size = sizeof(char *);

   if (size >= MEM_TABLE_SIZE) return(genalloc(theEnv,(unsigned) size));

   memPtr = (struct memoryPtr *) MemoryData(theEnv)->MemoryTable[size];
   if (memPtr == NULL)
     {
      return(genalloc(theEnv,(unsigned) size));
     }

   MemoryData(theEnv)->MemoryTable[size] = memPtr->next;

   return ((void *) memPtr);
  }

/*****************************************************/
/* gm3: Allocates memory and does not initialize it. */
/*****************************************************/
globle void *gm3(
  void *theEnv,
  size_t size)
  {
   struct memoryPtr *memPtr;

   if (size < (long) sizeof(char *)) size = sizeof(char *);

   if (size >= MEM_TABLE_SIZE) return(genalloc(theEnv,size));

   memPtr = (struct memoryPtr *) MemoryData(theEnv)->MemoryTable[(int) size];
   if (memPtr == NULL)
     { return(genalloc(theEnv,size)); }

   MemoryData(theEnv)->MemoryTable[(int) size] = memPtr->next;

   return ((void *) memPtr);
  }

/****************************************/
/* rm: Returns a block of memory to the */
/*   maintained pool of free memory.    */
/****************************************/
globle int rm(
  void *theEnv,
  void *str,
  size_t size)
  {
   struct memoryPtr *memPtr;

   if (size == 0)
     {
      SystemError(theEnv,"MEMORY",1);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   if (size < sizeof(char *)) size = sizeof(char *);

   if (size >= MEM_TABLE_SIZE) return(genfree(theEnv,(void *) str,(unsigned) size));

   memPtr = (struct memoryPtr *) str;
   memPtr->next = MemoryData(theEnv)->MemoryTable[size];
   MemoryData(theEnv)->MemoryTable[size] = memPtr;
   return(1);
  }

/********************************************/
/* rm3: Returns a block of memory to the    */
/*   maintained pool of free memory that's  */
/*   size is indicated with a long integer. */
/********************************************/
globle int rm3(
  void *theEnv,
  void *str,
  size_t size)
  {
   struct memoryPtr *memPtr;

   if (size == 0)
     {
      SystemError(theEnv,"MEMORY",1);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   if (size < (long) sizeof(char *)) size = sizeof(char *);

   if (size >= MEM_TABLE_SIZE) return(genfree(theEnv,(void *) str,(unsigned long) size));

   memPtr = (struct memoryPtr *) str;
   memPtr->next = MemoryData(theEnv)->MemoryTable[(int) size];
   MemoryData(theEnv)->MemoryTable[(int) size] = memPtr;
   return(1);
  }

/***************************************************/
/* PoolSize: Returns number of bytes in free pool. */
/***************************************************/
globle unsigned long PoolSize(
  void *theEnv)
  {
   register int i;
   struct memoryPtr *memPtr;
   unsigned long cnt = 0;

   for (i = sizeof(char *) ; i < MEM_TABLE_SIZE ; i++)
     {
      memPtr = MemoryData(theEnv)->MemoryTable[i];
      while (memPtr != NULL)
        {
         cnt += (unsigned long) i;
         memPtr = memPtr->next;
        }
     }
   return(cnt);
  }

/***************************************************************/
/* ActualPoolSize : Returns number of bytes DOS requires to    */
/*   store the free pool.  This routine is functionally        */
/*   equivalent to pool_size on anything other than the IBM-PC */
/***************************************************************/
globle unsigned long ActualPoolSize(
  void *theEnv)
  {
#if WIN_BTC
   register int i;
   struct memoryPtr *memPtr;
   unsigned long cnt = 0;

   for (i = sizeof(char *) ; i < MEM_TABLE_SIZE ; i++)
     {
      memPtr = MemoryData(theEnv)->MemoryTable[i];
      while (memPtr != NULL)
        {
         /*==============================================================*/
         /* For a block of size n, the Turbo-C Library routines require  */
         /* a header of size 8 bytes and further require that all memory */
         /* allotments be paragraph (16-bytes) aligned.                  */
         /*==============================================================*/

         cnt += (((unsigned long) i) + 19L) & 0xfffffff0L;
         memPtr = memPtr->next;
        }
     }
   return(cnt);
#else
   return(PoolSize(theEnv));
#endif
  }

/********************************************/
/* EnvSetConserveMemory: Allows the setting */
/*    of the memory conservation flag.      */
/********************************************/
globle intBool EnvSetConserveMemory(
  void *theEnv,
  intBool value)
  {
   int ov;

   ov = MemoryData(theEnv)->ConserveMemory;
   MemoryData(theEnv)->ConserveMemory = value;
   return(ov);
  }

/*******************************************/
/* EnvGetConserveMemory: Returns the value */
/*    of the memory conservation flag.     */
/*******************************************/
globle intBool EnvGetConserveMemory(
  void *theEnv)
  {
   return(MemoryData(theEnv)->ConserveMemory);
  }

/**************************/
/* genmemcpy:             */
/**************************/
globle void genmemcpy(
  char *dst,
  char *src,
  unsigned long size)
  {
   unsigned long i;

   for (i = 0L ; i < size ; i++)
     dst[i] = src[i];
  }

/**************************/
/* BLOCK MEMORY FUNCTIONS */
/**************************/

#if BLOCK_MEMORY

/***************************************************/
/* InitializeBlockMemory: Initializes block memory */
/*   management and allocates the first block.     */
/***************************************************/
static int InitializeBlockMemory(
  void *theEnv,
  unsigned int requestSize)
  {
   struct chunkInfo *chunkPtr;
   unsigned int initialBlockSize, usableBlockSize;

   /*===========================================*/
   /* The block memory routines depend upon the */
   /* size of a character being 1 byte.         */
   /*===========================================*/

   if (sizeof(char) != 1)
     {
      fprintf(stdout, "Size of character data is not 1\n");
      fprintf(stdout, "Memory allocation functions may not work\n");
      return(0);
     }

   MemoryData(theEnv)->ChunkInfoSize = sizeof(struct chunkInfo);
   MemoryData(theEnv)->ChunkInfoSize = (int) ((((MemoryData(theEnv)->ChunkInfoSize - 1) / STRICT_ALIGN_SIZE) + 1) * STRICT_ALIGN_SIZE);

   MemoryData(theEnv)->BlockInfoSize = sizeof(struct blockInfo);
   MemoryData(theEnv)->BlockInfoSize = (int) ((((MemoryData(theEnv)->BlockInfoSize - 1) / STRICT_ALIGN_SIZE) + 1) * STRICT_ALIGN_SIZE);

   initialBlockSize = (INITBLOCKSIZE > requestSize ? INITBLOCKSIZE : requestSize);
   initialBlockSize += MemoryData(theEnv)->ChunkInfoSize * 2 + MemoryData(theEnv)->BlockInfoSize;
   initialBlockSize = (((initialBlockSize - 1) / STRICT_ALIGN_SIZE) + 1) * STRICT_ALIGN_SIZE;

   usableBlockSize = initialBlockSize - (2 * MemoryData(theEnv)->ChunkInfoSize) - MemoryData(theEnv)->BlockInfoSize;

   /* make sure we get a buffer big enough to be usable */
   if ((requestSize < INITBLOCKSIZE) &&
       (usableBlockSize <= requestSize + MemoryData(theEnv)->ChunkInfoSize))
     {
      initialBlockSize = requestSize + MemoryData(theEnv)->ChunkInfoSize * 2 + MemoryData(theEnv)->BlockInfoSize;
      initialBlockSize = (((initialBlockSize - 1) / STRICT_ALIGN_SIZE) + 1) * STRICT_ALIGN_SIZE;
      usableBlockSize = initialBlockSize - (2 * MemoryData(theEnv)->ChunkInfoSize) - MemoryData(theEnv)->BlockInfoSize;
     }

   MemoryData(theEnv)->TopMemoryBlock = (struct blockInfo *) malloc((STD_SIZE) initialBlockSize);

   if (MemoryData(theEnv)->TopMemoryBlock == NULL)
     {
      fprintf(stdout, "Unable to allocate initial memory pool\n");
      return(0);
     }

   MemoryData(theEnv)->TopMemoryBlock->nextBlock = NULL;
   MemoryData(theEnv)->TopMemoryBlock->prevBlock = NULL;
   MemoryData(theEnv)->TopMemoryBlock->nextFree = (struct chunkInfo *) (((char *) MemoryData(theEnv)->TopMemoryBlock) + MemoryData(theEnv)->BlockInfoSize);
   MemoryData(theEnv)->TopMemoryBlock->size = (long) usableBlockSize;

   chunkPtr = (struct chunkInfo *) (((char *) MemoryData(theEnv)->TopMemoryBlock) + MemoryData(theEnv)->BlockInfoSize + MemoryData(theEnv)->ChunkInfoSize + usableBlockSize);
   chunkPtr->nextFree = NULL;
   chunkPtr->lastFree = NULL;
   chunkPtr->prevChunk = MemoryData(theEnv)->TopMemoryBlock->nextFree;
   chunkPtr->size = 0;

   MemoryData(theEnv)->TopMemoryBlock->nextFree->nextFree = NULL;
   MemoryData(theEnv)->TopMemoryBlock->nextFree->lastFree = NULL;
   MemoryData(theEnv)->TopMemoryBlock->nextFree->prevChunk = NULL;
   MemoryData(theEnv)->TopMemoryBlock->nextFree->size = (long) usableBlockSize;

   MemoryData(theEnv)->BlockMemoryInitialized = TRUE;
   return(1);
  }

/***************************************************************************/
/* AllocateBlock: Adds a new block of memory to the list of memory blocks. */
/***************************************************************************/
static int AllocateBlock(
  void *theEnv,
  struct blockInfo *blockPtr,
  unsigned int requestSize)
  {
   unsigned int blockSize, usableBlockSize;
   struct blockInfo *newBlock;
   struct chunkInfo *newTopChunk;

   /*============================================================*/
   /* Determine the size of the block that needs to be allocated */
   /* to satisfy the request. Normally, a default block size is  */
   /* used, but if the requested size is larger than the default */
   /* size, then the requested size is used for the block size.  */
   /*============================================================*/

   blockSize = (BLOCKSIZE > requestSize ? BLOCKSIZE : requestSize);
   blockSize += MemoryData(theEnv)->BlockInfoSize + MemoryData(theEnv)->ChunkInfoSize * 2;
   blockSize = (((blockSize - 1) / STRICT_ALIGN_SIZE) + 1) * STRICT_ALIGN_SIZE;

   usableBlockSize = blockSize - MemoryData(theEnv)->BlockInfoSize - (2 * MemoryData(theEnv)->ChunkInfoSize);

   /*=========================*/
   /* Allocate the new block. */
   /*=========================*/

   newBlock = (struct blockInfo *) malloc((STD_SIZE) blockSize);
   if (newBlock == NULL) return(0);

   /*======================================*/
   /* Initialize the block data structure. */
   /*======================================*/

   newBlock->nextBlock = NULL;
   newBlock->prevBlock = blockPtr;
   newBlock->nextFree = (struct chunkInfo *) (((char *) newBlock) + MemoryData(theEnv)->BlockInfoSize);
   newBlock->size = (long) usableBlockSize;
   blockPtr->nextBlock = newBlock;

   newTopChunk = (struct chunkInfo *) (((char *) newBlock) + MemoryData(theEnv)->BlockInfoSize + MemoryData(theEnv)->ChunkInfoSize + usableBlockSize);
   newTopChunk->nextFree = NULL;
   newTopChunk->lastFree = NULL;
   newTopChunk->size = 0;
   newTopChunk->prevChunk = newBlock->nextFree;

   newBlock->nextFree->nextFree = NULL;
   newBlock->nextFree->lastFree = NULL;
   newBlock->nextFree->prevChunk = NULL;
   newBlock->nextFree->size = (long) usableBlockSize;

   return(1);
  }

/*******************************************************/
/* RequestChunk: Allocates memory by returning a chunk */
/*   of memory from a larger block of memory.          */
/*******************************************************/
globle void *RequestChunk(
  void *theEnv,
  size_t requestSize)
  {
   struct chunkInfo *chunkPtr;
   struct blockInfo *blockPtr;

   /*==================================================*/
   /* Allocate initial memory pool block if it has not */
   /* already been allocated.                          */
   /*==================================================*/

   if (MemoryData(theEnv)->BlockMemoryInitialized == FALSE)
      {
       if (InitializeBlockMemory(theEnv,requestSize) == 0) return(NULL);
      }

   /*====================================================*/
   /* Make sure that the amount of memory requested will */
   /* fall on a boundary of strictest alignment          */
   /*====================================================*/

   requestSize = (((requestSize - 1) / STRICT_ALIGN_SIZE) + 1) * STRICT_ALIGN_SIZE;

   /*=====================================================*/
   /* Search through the list of free memory for a block  */
   /* of the appropriate size.  If a block is found, then */
   /* allocate and return a pointer to it.                */
   /*=====================================================*/

   blockPtr = MemoryData(theEnv)->TopMemoryBlock;

   while (blockPtr != NULL)
     {
      chunkPtr = blockPtr->nextFree;

      while (chunkPtr != NULL)
        {
         if ((chunkPtr->size == requestSize) ||
             (chunkPtr->size > (requestSize + MemoryData(theEnv)->ChunkInfoSize)))
           {
            AllocateChunk(theEnv,blockPtr,chunkPtr,requestSize);

            return((void *) (((char *) chunkPtr) + MemoryData(theEnv)->ChunkInfoSize));
           }
         chunkPtr = chunkPtr->nextFree;
        }

      if (blockPtr->nextBlock == NULL)
        {
         if (AllocateBlock(theEnv,blockPtr,requestSize) == 0)  /* get another block */
           { return(NULL); }
        }
      blockPtr = blockPtr->nextBlock;
     }

   SystemError(theEnv,"MEMORY",2);
   EnvExitRouter(theEnv,EXIT_FAILURE);
   return(NULL); /* Unreachable, but prevents warning. */
  }

/********************************************/
/* AllocateChunk: Allocates a chunk from an */
/*   existing chunk in a block of memory.   */
/********************************************/
static void AllocateChunk(
  void *theEnv,
  struct blockInfo *parentBlock,
  struct chunkInfo *chunkPtr,
  size_t requestSize)
  {
   struct chunkInfo *splitChunk, *nextChunk;

   /*=============================================================*/
   /* If the size of the memory chunk is an exact match for the   */
   /* requested amount of memory, then the chunk can be allocated */
   /* without splitting it.                                       */
   /*=============================================================*/

   if (requestSize == chunkPtr->size)
     {
      chunkPtr->size = - (long int) requestSize;
      if (chunkPtr->lastFree == NULL)
        {
         if (chunkPtr->nextFree != NULL)
           { parentBlock->nextFree = chunkPtr->nextFree; }
         else
           { parentBlock->nextFree = NULL; }
        }
      else
        { chunkPtr->lastFree->nextFree = chunkPtr->nextFree; }

      if (chunkPtr->nextFree != NULL)
        { chunkPtr->nextFree->lastFree = chunkPtr->lastFree; }

      chunkPtr->lastFree = NULL;
      chunkPtr->nextFree = NULL;
      return;
     }

   /*===========================================================*/
   /* If the size of the memory chunk is larger than the memory */
   /* request, then split the chunk into two pieces.            */
   /*===========================================================*/

   nextChunk = (struct chunkInfo *)
              (((char *) chunkPtr) + MemoryData(theEnv)->ChunkInfoSize + chunkPtr->size);

   splitChunk = (struct chunkInfo *)
                  (((char *) chunkPtr) + (MemoryData(theEnv)->ChunkInfoSize + requestSize));

   splitChunk->size = (long) (chunkPtr->size - (requestSize + MemoryData(theEnv)->ChunkInfoSize));
   splitChunk->prevChunk = chunkPtr;

   splitChunk->nextFree = chunkPtr->nextFree;
   splitChunk->lastFree = chunkPtr->lastFree;

   nextChunk->prevChunk = splitChunk;

   if (splitChunk->lastFree == NULL)
     { parentBlock->nextFree = splitChunk; }
   else
     { splitChunk->lastFree->nextFree = splitChunk; }

   if (splitChunk->nextFree != NULL)
     { splitChunk->nextFree->lastFree = splitChunk; }

   chunkPtr->size = - (long int) requestSize;
   chunkPtr->lastFree = NULL;
   chunkPtr->nextFree = NULL;

   return;
  }

/***********************************************************/
/* ReturnChunk: Frees memory allocated using RequestChunk. */
/***********************************************************/
globle int ReturnChunk(
  void *theEnv,
  void *memPtr,
  size_t size)
  {
   struct chunkInfo *chunkPtr, *lastChunk, *nextChunk, *topChunk;
   struct blockInfo *blockPtr;

   /*=====================================================*/
   /* Determine if the expected size of the chunk matches */
   /* the size stored in the chunk's information record.  */
   /*=====================================================*/

   size = (((size - 1) / STRICT_ALIGN_SIZE) + 1) * STRICT_ALIGN_SIZE;

   chunkPtr = (struct chunkInfo *) (((char *) memPtr) - MemoryData(theEnv)->ChunkInfoSize);

   if (chunkPtr == NULL)
     { return(FALSE); }

   if (chunkPtr->size >= 0)
     { return(FALSE); }

   if (chunkPtr->size != - (long int) size)
     { return(FALSE); }

   chunkPtr->size = - chunkPtr->size;

   /*=============================================*/
   /* Determine in which block the chunk resides. */
   /*=============================================*/

   topChunk = chunkPtr;
   while (topChunk->prevChunk != NULL)
     { topChunk = topChunk->prevChunk; }
   blockPtr = (struct blockInfo *) (((char *) topChunk) - MemoryData(theEnv)->BlockInfoSize);

   /*===========================================*/
   /* Determine the chunks physically preceding */
   /* and following the returned chunk.         */
   /*===========================================*/

   lastChunk = chunkPtr->prevChunk;
   nextChunk = (struct chunkInfo *) (((char *) memPtr) + size);

   /*=========================================================*/
   /* Add the chunk to the list of free chunks for the block. */
   /*=========================================================*/

   if (blockPtr->nextFree != NULL)
     { blockPtr->nextFree->lastFree = chunkPtr; }

   chunkPtr->nextFree = blockPtr->nextFree;
   chunkPtr->lastFree = NULL;

   blockPtr->nextFree = chunkPtr;

   /*=====================================================*/
   /* Combine this chunk with previous chunk if possible. */
   /*=====================================================*/

   if (lastChunk != NULL)
     {
      if (lastChunk->size > 0)
        {
         lastChunk->size += (MemoryData(theEnv)->ChunkInfoSize + chunkPtr->size);

         if (nextChunk != NULL)
           { nextChunk->prevChunk = lastChunk; }
         else
           { return(FALSE); }

         if (lastChunk->lastFree != NULL)
           { lastChunk->lastFree->nextFree = lastChunk->nextFree; }

         if (lastChunk->nextFree != NULL)
           { lastChunk->nextFree->lastFree = lastChunk->lastFree; }

         lastChunk->nextFree = chunkPtr->nextFree;
         if (chunkPtr->nextFree != NULL)
           { chunkPtr->nextFree->lastFree = lastChunk; }
         lastChunk->lastFree = NULL;

         blockPtr->nextFree = lastChunk;
         chunkPtr->lastFree = NULL;
         chunkPtr->nextFree = NULL;
         chunkPtr = lastChunk;
        }
     }

   /*=====================================================*/
   /* Combine this chunk with the next chunk if possible. */
   /*=====================================================*/

   if (nextChunk == NULL) return(FALSE);
   if (chunkPtr == NULL) return(FALSE);

   if (nextChunk->size > 0)
     {
      chunkPtr->size += (MemoryData(theEnv)->ChunkInfoSize + nextChunk->size);

      topChunk = (struct chunkInfo *) (((char *) nextChunk) + nextChunk->size + MemoryData(theEnv)->ChunkInfoSize);
      if (topChunk != NULL)
        { topChunk->prevChunk = chunkPtr; }
      else
        { return(FALSE); }

      if (nextChunk->lastFree != NULL)
        { nextChunk->lastFree->nextFree = nextChunk->nextFree; }

      if (nextChunk->nextFree != NULL)
        { nextChunk->nextFree->lastFree = nextChunk->lastFree; }

     }

   /*===========================================*/
   /* Free the buffer if we can, but don't free */
   /* the first buffer if it's the only one.    */
   /*===========================================*/

   if ((chunkPtr->prevChunk == NULL) &&
       (chunkPtr->size == blockPtr->size))
     {
      if (blockPtr->prevBlock != NULL)
        {
         blockPtr->prevBlock->nextBlock = blockPtr->nextBlock;
         if (blockPtr->nextBlock != NULL)
           { blockPtr->nextBlock->prevBlock = blockPtr->prevBlock; }
         free((char *) blockPtr);
        }
      else
        {
         if (blockPtr->nextBlock != NULL)
           {
            blockPtr->nextBlock->prevBlock = NULL;
            MemoryData(theEnv)->TopMemoryBlock = blockPtr->nextBlock;
            free((char *) blockPtr);
           }
        }
     }

   return(TRUE);
  }

/***********************************************/
/* ReturnAllBlocks: Frees all allocated blocks */
/*   back to the operating system.             */
/***********************************************/
globle void ReturnAllBlocks(
  void *theEnv)
  {
   struct blockInfo *theBlock, *nextBlock;

   /*======================================*/
   /* Free up int based memory allocation. */
   /*======================================*/

   theBlock = MemoryData(theEnv)->TopMemoryBlock;
   while (theBlock != NULL)
     {
      nextBlock = theBlock->nextBlock;
      free((char *) theBlock);
      theBlock = nextBlock;
     }

   MemoryData(theEnv)->TopMemoryBlock = NULL;
  }
#endif
