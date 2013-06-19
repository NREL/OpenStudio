   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*           CONSTRUCT COMPILER HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
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
/*************************************************************/

#ifndef _H_conscomp
#define _H_conscomp

#define ArbitraryPrefix(codeItem,i)    (codeItem)->arrayNames[(i)]

#define ModulePrefix(codeItem)         (codeItem)->arrayNames[0]
#define ConstructPrefix(codeItem)      (codeItem)->arrayNames[1]

#ifndef _H_constrct
#include "constrct.h"
#endif
#ifndef _H_extnfunc
#include "extnfunc.h"
#endif
#ifndef _H_symblcmp
#include "symblcmp.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif

#define CONSTRUCT_COMPILER_DATA 41

struct CodeGeneratorItem
  {
   char *name;
   void (*beforeFunction)(void *);
   void (*initFunction)(void *,FILE *,int,int);
   int (*generateFunction)(void *,char *,char *,char *,int,FILE *,int,int);
   int priority;
   char **arrayNames;
   int arrayCount;
   struct CodeGeneratorItem *next;
  };

struct constructCompilerData
  { 
   int ImageID;
   FILE *HeaderFP;
   int MaxIndices;
   FILE *ExpressionFP;
   FILE *FixupFP;
   char *FilePrefix;
   char *PathName;
   char *FileNameBuffer;
   intBool ExpressionHeader;
   long ExpressionCount;
   int ExpressionVersion;
   int CodeGeneratorCount;
   struct CodeGeneratorItem *ListOfCodeGeneratorItems;
  };

#define ConstructCompilerData(theEnv) ((struct constructCompilerData *) GetEnvironmentData(theEnv,CONSTRUCT_COMPILER_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

struct CodeGeneratorFile
 {
  char *filePrefix;
  char *pathName;
  char *fileNameBuffer;
  int id,version;
 };

#ifdef _CONSCOMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                      InitializeConstructCompilerData(void *);
   LOCALE void                      ConstructsToCCommandDefinition(void *);
   LOCALE FILE                     *NewCFile(void *,char *,char *,char *,int,int,int);
   LOCALE int                       ExpressionToCode(void *,FILE *,struct expr *);
   LOCALE void                      PrintFunctionReference(void *,FILE *,struct FunctionDefinition *);
   LOCALE struct CodeGeneratorItem *AddCodeGeneratorItem(void *,char *,int,
                                                         void (*)(void *),
                                                         void (*)(void *,FILE *,int,int),
                                                         int (*)(void *,char *,char *,char *,int,FILE *,int,int),int);
   LOCALE FILE                     *CloseFileIfNeeded(void *,FILE *,int *,int *,int,int *,struct CodeGeneratorFile *);
   LOCALE FILE                     *OpenFileIfNeeded(void *,FILE *,char *,char *,char *,int,int,int *,int,FILE *,
                                                     char *,char *,int,struct CodeGeneratorFile *);
   LOCALE void                      MarkConstructBsaveIDs(void *,int);
   LOCALE void                      ConstructHeaderToCode(void *,FILE *,struct constructHeader *,int,int,
                                                         int,char *,char *);
   LOCALE void                      ConstructModuleToCode(void *,FILE *,struct defmodule *,int,int,
                                                         int,char *);
   LOCALE void                      PrintHashedExpressionReference(void *,FILE *,struct expr *,int,int);

#endif




