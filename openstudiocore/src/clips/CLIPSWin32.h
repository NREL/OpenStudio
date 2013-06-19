#ifndef CLIPSWin32_H
#define CLIPSWin32_H

#include "CLIPSWin32defs.h"

void __declspec(dllimport) * CALL_CONV __CreateEnvironment(void);
void __declspec(dllimport) CALL_CONV __DestroyEnvironment(void *);
void __declspec(dllimport) CALL_CONV __EnvClear(void *);
void __declspec(dllimport) CALL_CONV __EnvReset(void *);
int __declspec(dllimport) CALL_CONV __EnvLoad(void *,char *);
long long __declspec(dllimport) CALL_CONV __EnvRun(void *,long long);
int __declspec(dllimport) CALL_CONV __EnvBuild(void *,char *);
int __declspec(dllimport) CALL_CONV __EnvEval(void *,char *,void *);  
void __declspec(dllimport) CALL_CONV __EnvIncrementFactCount(void *,void *);
void __declspec(dllimport) CALL_CONV __EnvDecrementFactCount(void *,void *);
void __declspec(dllimport) CALL_CONV __EnvIncrementInstanceCount(void *,void *);
void __declspec(dllimport) CALL_CONV __EnvDecrementInstanceCount(void *,void *);
long long __declspec(dllimport) CALL_CONV __EnvFactIndex(void *,void *);
int __declspec(dllimport) CALL_CONV __EnvGetFactSlot(void *,void *,char *,void *);  
char __declspec(dllimport) * CALL_CONV __EnvGetInstanceName(void *,void *); 

#endif