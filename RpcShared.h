#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "../rpc_gen/BochsRpcDbg.h"
#ifdef __cplusplus
}
#endif

typedef VOID (*PWorkerProc)(PVOID pParam, HANDLE hCompletionEvent);

typedef struct{
	unsigned __int32 StartAddr;
	unsigned __int32 Length;
	PBYTE Buffer;
	error_status_t Status;
}ReadMemoryParas, *PReadMemoryParas;

typedef struct{
	PBochsGPRsContext pContext;
	error_status_t Status;
}ReadGPRsParas, *PReadGPRsParas;

void Bochs_PauseSimulationImpl();
void Bochs_ResumeSimulationImpl(PVOID, HANDLE);
void Bochs_QuitSimulationImpl(PVOID, HANDLE);
void Bochs_ReadLinearMemoryImpl(PVOID, HANDLE);
void Bochs_ReadPhysicalMemoryImpl(PVOID, HANDLE);
void Bochs_GetGPRsImpl(PVOID, HANDLE);
void Bochs_WaitForIdleImpl(PVOID, HANDLE);

struct WorkerParamPack{
	PWorkerProc pWorker;
	UINT32 Status;
	PVOID pParam;
	HANDLE hCompletionEvent;
};
