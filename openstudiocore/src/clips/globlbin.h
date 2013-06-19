   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*             DEFGLOBAL BINARY HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_globlbin

#define _H_globlbin

#include "modulbin.h"
#include "cstrcbin.h"
#include "globldef.h"

struct bsaveDefglobal
  {
   struct bsaveConstructHeader header;
   long initial;
  };

struct bsaveDefglobalModule
  {
   struct bsaveDefmoduleItemHeader header;
  };

#define GLOBLBIN_DATA 60

struct defglobalBinaryData
  { 
   struct defglobal *DefglobalArray;
   long NumberOfDefglobals;
   struct defglobalModule *ModuleArray;
   long NumberOfDefglobalModules;
  };
  
#define DefglobalBinaryData(theEnv) ((struct defglobalBinaryData *) GetEnvironmentData(theEnv,GLOBLBIN_DATA))

#define DefglobalPointer(i) ((struct defglobal *) (&DefglobalBinaryData(theEnv)->DefglobalArray[i]))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _GLOBLBIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           DefglobalBinarySetup(void *);
   LOCALE void                          *BloadDefglobalModuleReference(void *,int);

#ifndef _GLOBLBIN_SOURCE_
   extern struct defglobal *DefglobalArray;
#endif
#endif



