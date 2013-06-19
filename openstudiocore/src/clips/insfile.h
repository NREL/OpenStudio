   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#ifndef _H_insfile
#define _H_insfile

#ifndef _H_expressn
#include "expressn.h"
#endif

#define INSTANCE_FILE_DATA 30

#if BLOAD_INSTANCES || BSAVE_INSTANCES
struct instanceFileData
  { 
#if BLOAD_INSTANCES || BSAVE_INSTANCES
   char *InstanceBinaryPrefixID;
   char *InstanceBinaryVersionID;
   unsigned long BinaryInstanceFileSize;

#if BLOAD_INSTANCES
   unsigned long BinaryInstanceFileOffset;
   char *CurrentReadBuffer;
   unsigned long CurrentReadBufferSize;
   unsigned long CurrentReadBufferOffset;
#endif

#endif
  };

#define InstanceFileData(theEnv) ((struct instanceFileData *) GetEnvironmentData(theEnv,INSTANCE_FILE_DATA))

#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _INSFILE_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define BinaryLoadInstances(a) EnvBinaryLoadInstances(GetCurrentEnvironment(),a)
#define BinarySaveInstances(a,b,c,d) EnvBinarySaveInstances(GetCurrentEnvironment(),a,b,c,d)
#define LoadInstances(a) EnvLoadInstances(GetCurrentEnvironment(),a)
#define LoadInstancesFromString(a,b) EnvLoadInstancesFromString(GetCurrentEnvironment(),a,b)
#define RestoreInstances(a) EnvRestoreInstances(GetCurrentEnvironment(),a)
#define RestoreInstancesFromString(a,b) EnvRestoreInstancesFromString(GetCurrentEnvironment(),a,b)
#define SaveInstances(a,b,c,d) EnvSaveInstances(GetCurrentEnvironment(),a,b,c,d)

LOCALE void SetupInstanceFileCommands(void *);

LOCALE long SaveInstancesCommand(void *);
LOCALE long LoadInstancesCommand(void *);
LOCALE long RestoreInstancesCommand(void *);
LOCALE long EnvSaveInstances(void *,char *,int,EXPRESSION *,intBool);

#if BSAVE_INSTANCES
LOCALE long BinarySaveInstancesCommand(void *);
LOCALE long EnvBinarySaveInstances(void *,char *,int,EXPRESSION *,intBool);
#endif

#if BLOAD_INSTANCES
LOCALE long BinaryLoadInstancesCommand(void *);
LOCALE long EnvBinaryLoadInstances(void *,char *);
#endif

LOCALE long EnvLoadInstances(void *,char *);
LOCALE long EnvLoadInstancesFromString(void *,char *,int);
LOCALE long EnvRestoreInstances(void *,char *);
LOCALE long EnvRestoreInstancesFromString(void *,char *,int);

#ifndef _INSFILE_SOURCE_
#endif

#endif



