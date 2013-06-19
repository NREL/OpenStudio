   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  04/21/06          */
   /*                                                     */
   /*                USER FUNCTIONS MODULE                */
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
/*      6.24: Created file to seperate UserFunctions and     */
/*            EnvUserFunctions from main.c.                  */
/*                                                           */
/*************************************************************/

/***************************************************************************/
/*                                                                         */
/* Permission is hereby granted, free of charge, to any person obtaining   */
/* a copy of this software and associated documentation files (the         */
/* "Software"), to deal in the Software without restriction, including     */
/* without limitation the rights to use, copy, modify, merge, publish,     */
/* distribute, and/or sell copies of the Software, and to permit persons   */
/* to whom the Software is furnished to do so.                             */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT   */
/* OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY  */
/* CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES */
/* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN   */
/* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF */
/* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.          */
/*                                                                         */
/***************************************************************************/

#include "clips.h"

// #include <rulesengine/CLIPSStandardsInterfaceWrapper.hpp>

void UserFunctions(void);
void EnvUserFunctions(void *);

/*********************************************************/
/* UserFunctions: Informs the expert system environment  */
/*   of any user defined functions. In the default case, */
/*   there are no user defined functions. To define      */
/*   functions, either this function must be replaced by */
/*   a function with the same name within this file, or  */
/*   this function can be deleted from this file and     */
/*   included in another file.                           */
/*********************************************************/
void EnvUserFunctions(void *t_env)
{
  extern void addTextToReport(void *);
  extern void setOverallResultFlag(void *);
  extern void *getOverallResultFlag(void *);

  extern void getObjects(void *, DATA_OBJECT_PTR returnValuePtr);
  extern void *getObjectName(void *);

  extern void *cloneModel(void *);

  extern void *checkAttribute(void *);
  extern void *getBooleanAttribute(void *);
  extern double getDoubleAttribute(void *);
  extern void *getEnumAttribute(void *);
  extern int getIntegerAttribute(void *);

  EnvDefineFunction2(t_env, "addTextToReport",'v',PTIEF addTextToReport,"addTextToReport","2*us");
  EnvDefineFunction2(t_env, "setOverallResultFlag",'v',PTIEF setOverallResultFlag,"setOverallResultFlag","11s");
  EnvDefineFunction2(t_env, "getOverallResultFlag",'w',PTIEF getOverallResultFlag,"getOverallResultFlag","11s");

  EnvDefineFunction2(t_env, "getObjects",'m',PTIEF getObjects,"getObjects","22s");
  EnvDefineFunction2(t_env, "getObjectName",'s',PTIEF getObjectName,"getObjectName","22s");

  EnvDefineFunction2(t_env, "cloneModel",'s',PTIEF cloneModel,"cloneModel","33k");

  EnvDefineFunction2(t_env, "checkAttribute",'s',PTIEF checkAttribute,"checkAttribute","55k");
  EnvDefineFunction2(t_env, "getBooleanAttribute",'w',PTIEF getBooleanAttribute,"getBooleanAttribute","44s");
  EnvDefineFunction2(t_env, "getDoubleAttribute",'d',PTIEF getDoubleAttribute,"getDoubleAttribute","44s");
  EnvDefineFunction2(t_env, "getEnumAttribute",'s',PTIEF getEnumAttribute,"getEnumAttribute","44s");
  EnvDefineFunction2(t_env, "getIntegerAttribute",'i',PTIEF getIntegerAttribute,"getIntegerAttribute","44s");
}
  
/***********************************************************/
/* EnvUserFunctions: Informs the expert system environment */
/*   of any user defined functions. In the default case,   */
/*   there are no user defined functions. To define        */
/*   functions, either this function must be replaced by   */
/*   a function with the same name within this file, or    */
/*   this function can be deleted from this file and       */
/*   included in another file.                             */
/***********************************************************/
#if WIN_BTC
#pragma argsused
#endif
void UserFunctions()
  {
  }

