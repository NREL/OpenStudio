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

