#pragma once
#include "BochsDbg.h"
#include "TaskQueue.hpp"
#include "ManagedRes.h"

typedef VOID (*PWorkerProc)(PVOID pParam, HANDLE hCompletionEvent);

typedef struct{
	__int64 StartAddr;
	__int32 Length;
	PBYTE Buffer;
	error_status_t Status;
}ReadMemoryParas, *PReadMemoryParas;

HANDLE StartRpcServer();
void StopRpcServer();

void Bochs_PauseSimulationImpl();
void Bochs_ResumeSimulationImpl(PVOID, HANDLE);
void Bochs_QuitSimulationImpl(PVOID, HANDLE);
void Bochs_ReadLinearMemoryImpl(PVOID, HANDLE);
void Bochs_WaitForIdleImpl(PVOID, HANDLE);

struct WorkerParamPack{
	PWorkerProc pWorker;
	UINT32 Status;
	PVOID pParam;
	HANDLE hCompletionEvent;
};
typedef TaskQueue<WorkerParamPack>::ElementType CmdWorkItem;
typedef TaskQueue<WorkerParamPack>::PElementType PCmdWorkItem;

extern TaskQueue<WorkerParamPack> WorkerList;
extern ManagedHANDLE WorkingItemReadyEvent;