   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*           DEFFACTS BSAVE/BLOAD HEADER FILE          */
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

#if (! RUN_TIME)
#ifndef _H_dffctbin

#define _H_dffctbin

#include "modulbin.h"
#include "cstrcbin.h"
#ifndef _H_constrct
#include "constrct.h"
#endif

struct bsaveDeffacts
  {
   struct bsaveConstructHeader header;
   long assertList;
  };

struct bsaveDeffactsModule
  {
   struct bsaveDefmoduleItemHeader header;
  };
  
#define DFFCTBIN_DATA 26

struct deffactsBinaryData
  { 
   struct deffacts *DeffactsArray;
   long NumberOfDeffacts;
   struct deffactsModule *ModuleArray;
   long NumberOfDeffactsModules;
  };
  
#define DeffactsBinaryData(theEnv) ((struct deffactsBinaryData *) GetEnvironmentData(theEnv,DFFCTBIN_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _DFFCTBIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           DeffactsBinarySetup(void *);
   LOCALE void                          *BloadDeffactsModuleReference(void *,int);

#endif
#endif



