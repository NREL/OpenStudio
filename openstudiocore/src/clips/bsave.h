   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
   /*                                                     */
   /*                 BSAVE HEADER FILE                   */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_bsave
#define _H_bsave

struct BinaryItem;

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

#ifndef _H_expressn
#include "expressn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _BSAVE_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

struct BinaryItem
  {
   char *name;
   void (*findFunction)(void *);
   void (*bloadStorageFunction)(void *);
   void (*bloadFunction)(void *);
   void (*clearFunction)(void *);
   void (*expressionFunction)(void *,FILE *);
   void (*bsaveStorageFunction)(void *,FILE *);
   void (*bsaveFunction)(void *,FILE *);
   int priority;
   struct BinaryItem *next;
  };

#if BLOAD_AND_BSAVE
typedef struct bloadcntsv
  {
   long val;
   struct bloadcntsv *nxt;
  } BLOADCNTSV;
#endif

typedef struct bsave_expr
  {
   unsigned short type;
   long value,arg_list,next_arg;
  } BSAVE_EXPRESSION;

#define CONSTRUCT_HEADER_SIZE 20

#define BSAVE_DATA 39

struct bsaveData
  { 
   struct BinaryItem *ListOfBinaryItems;
#if BLOAD_AND_BSAVE
   BLOADCNTSV *BloadCountSaveTop;
#endif
  };

#define BsaveData(theEnv) ((struct bsaveData *) GetEnvironmentData(theEnv,BSAVE_DATA))

#define Bsave(a) EnvBsave(GetCurrentEnvironment(),a)

   LOCALE void                    InitializeBsaveData(void *);
   LOCALE int                     BsaveCommand(void *);
#if BLOAD_AND_BSAVE
   LOCALE intBool                 EnvBsave(void *,char *);
   LOCALE void                    MarkNeededItems(void *,struct expr *);
   LOCALE void                    SaveBloadCount(void *,long);
   LOCALE void                    RestoreBloadCount(void *,long *);
#endif
   LOCALE intBool                 AddBinaryItem(void *,char *,int,
                                                void (*)(void *),
                                                void (*)(void *,FILE *),
                                                void (*)(void *,FILE *),
                                                void (*)(void *,FILE *),
                                                void (*)(void *),
                                                void (*)(void *),
                                                void (*)(void *));

#ifndef _BSAVE_SOURCE_
   extern struct BinaryItem      *ListOfBinaryItems;
#endif

#endif







