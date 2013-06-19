   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*            MEMORY ALLOCATION HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose: Memory allocation routines.                      */
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
/*      6.30: Added get_mem and rtn_mem macros.              */
/*                                                           */
/*************************************************************/

#ifndef _H_memalloc

#include <string.h>

#define _H_memalloc

struct chunkInfo;
struct blockInfo;
struct memoryPtr;

#define MEM_TABLE_SIZE 500

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _MEMORY_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

struct chunkInfo
  {
   struct chunkInfo *prevChunk;
   struct chunkInfo *nextFree;
   struct chunkInfo *lastFree;
   long int size;
  };

struct blockInfo
  {
   struct blockInfo *nextBlock;
   struct blockInfo *prevBlock;
   struct chunkInfo *nextFree;
   long int size;
  };

struct memoryPtr
  {
   struct memoryPtr *next;
  };

#define get_struct(theEnv,type) \
  ((MemoryData(theEnv)->MemoryTable[sizeof(struct type)] == NULL) ? \
   ((struct type *) genalloc(theEnv,sizeof(struct type))) :\
   ((MemoryData(theEnv)->TempMemoryPtr = MemoryData(theEnv)->MemoryTable[sizeof(struct type)]),\
    MemoryData(theEnv)->MemoryTable[sizeof(struct type)] = MemoryData(theEnv)->TempMemoryPtr->next,\
    ((struct type *) MemoryData(theEnv)->TempMemoryPtr)))

#define rtn_struct(theEnv,type,struct_ptr) \
  (MemoryData(theEnv)->TempMemoryPtr = (struct memoryPtr *) struct_ptr,\
   MemoryData(theEnv)->TempMemoryPtr->next = MemoryData(theEnv)->MemoryTable[sizeof(struct type)], \
   MemoryData(theEnv)->MemoryTable[sizeof(struct type)] = MemoryData(theEnv)->TempMemoryPtr)

#define rtn_sized_struct(theEnv,size,struct_ptr) \
  (MemoryData(theEnv)->TempMemoryPtr = (struct memoryPtr *) struct_ptr,\
   MemoryData(theEnv)->TempMemoryPtr->next = MemoryData(theEnv)->MemoryTable[size], \
   MemoryData(theEnv)->MemoryTable[size] = MemoryData(theEnv)->TempMemoryPtr)

#define get_var_struct(theEnv,type,vsize) \
  ((((sizeof(struct type) + vsize) <  MEM_TABLE_SIZE) ? \
    (MemoryData(theEnv)->MemoryTable[sizeof(struct type) + vsize] == NULL) : 1) ? \
   ((struct type *) genalloc(theEnv,(sizeof(struct type) + vsize))) :\
   ((MemoryData(theEnv)->TempMemoryPtr = MemoryData(theEnv)->MemoryTable[sizeof(struct type) + vsize]),\
    MemoryData(theEnv)->MemoryTable[sizeof(struct type) + vsize] = MemoryData(theEnv)->TempMemoryPtr->next,\
    ((struct type *) MemoryData(theEnv)->TempMemoryPtr)))

#define rtn_var_struct(theEnv,type,vsize,struct_ptr) \
  (MemoryData(theEnv)->TempSize = sizeof(struct type) + vsize, \
   ((MemoryData(theEnv)->TempSize < MEM_TABLE_SIZE) ? \
    (MemoryData(theEnv)->TempMemoryPtr = (struct memoryPtr *) struct_ptr,\
     MemoryData(theEnv)->TempMemoryPtr->next = MemoryData(theEnv)->MemoryTable[MemoryData(theEnv)->TempSize], \
     MemoryData(theEnv)->MemoryTable[MemoryData(theEnv)->TempSize] =  MemoryData(theEnv)->TempMemoryPtr) : \
    (genfree(theEnv,(void *) struct_ptr,MemoryData(theEnv)->TempSize),(struct memoryPtr *) struct_ptr)))

#define get_mem(theEnv,size) \
  (((size <  MEM_TABLE_SIZE) ? \
    (MemoryData(theEnv)->MemoryTable[size] == NULL) : 1) ? \
   ((struct type *) genalloc(theEnv,(size_t) (size))) :\
   ((MemoryData(theEnv)->TempMemoryPtr = MemoryData(theEnv)->MemoryTable[size]),\
    MemoryData(theEnv)->MemoryTable[size] = MemoryData(theEnv)->TempMemoryPtr->next,\
    ((struct type *) MemoryData(theEnv)->TempMemoryPtr)))

#define rtn_mem(theEnv,size,ptr) \
  (MemoryData(theEnv)->TempSize = size, \
   ((MemoryData(theEnv)->TempSize < MEM_TABLE_SIZE) ? \
    (MemoryData(theEnv)->TempMemoryPtr = (struct memoryPtr *) ptr,\
     MemoryData(theEnv)->TempMemoryPtr->next = MemoryData(theEnv)->MemoryTable[MemoryData(theEnv)->TempSize], \
     MemoryData(theEnv)->MemoryTable[MemoryData(theEnv)->TempSize] =  MemoryData(theEnv)->TempMemoryPtr) : \
    (genfree(theEnv,(void *) ptr,MemoryData(theEnv)->TempSize),(struct memoryPtr *) ptr)))

#define GenCopyMemory(type,cnt,dst,src) \
   memcpy((void *) (dst),(void *) (src),sizeof(type) * (size_t) (cnt))

#define MEMORY_DATA 59

struct memoryData
  { 
   long int MemoryAmount;
   long int MemoryCalls;
   intBool ConserveMemory;
   int (*OutOfMemoryFunction)(void *,size_t);
#if BLOCK_MEMORY
   struct blockInfo *TopMemoryBlock;
   int BlockInfoSize;
   int ChunkInfoSize;
   int BlockMemoryInitialized;
#endif
   struct memoryPtr *TempMemoryPtr;
   struct memoryPtr **MemoryTable;
   size_t TempSize;
  };

#define MemoryData(theEnv) ((struct memoryData *) GetEnvironmentData(theEnv,MEMORY_DATA))

#define GetConserveMemory() EnvGetConserveMemory(GetCurrentEnvironment())
#define MemRequests() EnvMemRequests(GetCurrentEnvironment())
#define MemUsed() EnvMemUsed(GetCurrentEnvironment())
#define ReleaseMem(a,b) EnvReleaseMem(GetCurrentEnvironment(),a,b)
#define SetConserveMemory(a) EnvSetConserveMemory(GetCurrentEnvironment(),a)
#define SetOutOfMemoryFunction(a) EnvSetOutOfMemoryFunction(GetCurrentEnvironment(),a)

   LOCALE void                           InitializeMemory(void *);
   LOCALE void                          *genalloc(void *,size_t);
   LOCALE int                            DefaultOutOfMemoryFunction(void *,size_t);
   LOCALE int                          (*EnvSetOutOfMemoryFunction(void *,int (*)(void *,size_t)))(void *,size_t);
   LOCALE int                            genfree(void *,void *,size_t);
   LOCALE void                          *genrealloc(void *,void *,size_t,size_t);
   LOCALE long                           EnvMemUsed(void *);
   LOCALE long                           EnvMemRequests(void *);
   LOCALE long                           UpdateMemoryUsed(void *,long int);
   LOCALE long                           UpdateMemoryRequests(void *,long int);
   LOCALE long                           EnvReleaseMem(void *,long,int);
   LOCALE void                          *gm1(void *,size_t);
   LOCALE void                          *gm2(void *,size_t);
   LOCALE void                          *gm3(void *,size_t);
   LOCALE int                            rm(void *,void *,size_t);
   LOCALE int                            rm3(void *,void *,size_t);
   LOCALE unsigned long                  PoolSize(void *);
   LOCALE unsigned long                  ActualPoolSize(void *);
   LOCALE void                          *RequestChunk(void *,size_t);
   LOCALE int                            ReturnChunk(void *,void *,size_t);
   LOCALE intBool                        EnvSetConserveMemory(void *,intBool);
   LOCALE intBool                        EnvGetConserveMemory(void *);
   LOCALE void                           genmemcpy(char *,char *,unsigned long);
   LOCALE void                           ReturnAllBlocks(void *);

#endif






