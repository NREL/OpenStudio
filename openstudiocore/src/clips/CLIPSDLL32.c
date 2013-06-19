#include <windows.h>

#include "CLIPSWin32defs.h"

#include "clips.h"

BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,
  DWORD fwdreason,
  LPVOID lpvReserved)
  {
   return 1;
  }

void __declspec(dllexport) * CALL_CONV __CreateEnvironment()
  {
   return CreateEnvironment();
  }

void __declspec(dllexport) CALL_CONV __DestroyEnvironment(
  void *theEnv)
  {
   DestroyEnvironment(theEnv);
  }

void __declspec(dllexport) CALL_CONV __EnvClear(
  void *theEnv)
  {
   EnvClear(theEnv);
  }

void __declspec(dllexport) CALL_CONV __EnvReset(
  void *theEnv)
  {
   EnvReset(theEnv);
  }

int __declspec(dllexport) CALL_CONV __EnvLoad(
  void *theEnv,
  char *theFile)
  {
   return EnvLoad(theEnv,theFile);
  }

long long __declspec(dllexport) CALL_CONV __EnvRun(
  void *theEnv,
  long long runLimit)
  {
   return EnvRun(theEnv,runLimit);
  }
  
int __declspec(dllexport) CALL_CONV __EnvBuild(
  void *theEnv,
  char *buildString)
  {
   return EnvBuild(theEnv,buildString);
  }
  
int __declspec(dllexport) CALL_CONV __EnvEval(
  void *theEnv,
  char *evalString,
  DATA_OBJECT *rv)
  {
   return EnvEval(theEnv,evalString,rv);
  }  

void __declspec(dllexport) CALL_CONV __EnvIncrementFactCount(
  void *theEnv,
  void *theFact)
  {
   EnvIncrementFactCount(theEnv,theFact);
  }

void __declspec(dllexport) CALL_CONV __EnvDecrementFactCount(
  void *theEnv,
  void *theFact)
  {
   EnvDecrementFactCount(theEnv,theFact);
  }

void __declspec(dllexport) CALL_CONV __EnvIncrementInstanceCount(
  void *theEnv,
  void *theInstance)
  {
   EnvIncrementInstanceCount(theEnv,theInstance);
  }

void __declspec(dllexport) CALL_CONV __EnvDecrementInstanceCount(
  void *theEnv,
  void *theInstance)
  {
   EnvDecrementFactCount(theEnv,theInstance);
  }

long long __declspec(dllexport) CALL_CONV __EnvFactIndex(
  void *theEnv,
  void *theFact)
  {
   return EnvFactIndex(theEnv,theFact);
  }

int __declspec(dllexport) CALL_CONV __EnvGetFactSlot(
  void *theEnv,
  void *theFact,
  char *slotName,
  void *returnValue)
  {
   return EnvGetFactSlot(theEnv,theFact,slotName,returnValue);  
  }  

char __declspec(dllexport) * CALL_CONV __EnvGetInstanceName(
  void *theEnv,
  void *theInstance)
  {
   return EnvGetInstanceName(theEnv,theInstance);  
  } 