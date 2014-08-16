#include <Windows.h>
#include <cstdio>
#include <Rpc.h>
#include "RpcShared.h"
#include "../bx_debug/debug.h"

static DWORD WINAPI RpcListeningThread(LPVOID)
{
	return RpcServerListen(1,RPC_C_LISTEN_MAX_CALLS_DEFAULT,0);
}

HANDLE StartRpcServer()
{
	char AddressBuilder[MAX_PATH];
	sprintf(AddressBuilder, "Bochs_Debug_Port" /*_PID_%08I32X" */ /*, GetCurrentProcessId()*/);
	dbg_printf("\n----------\nBochs Rpc Debugger is listening on %s\n----------\n", AddressBuilder);
	RpcServerUseProtseqEp((unsigned char*)"ncalrpc",RPC_C_LISTEN_MAX_CALLS_DEFAULT,  (unsigned char*)AddressBuilder, NULL);
	RpcServerRegisterIf(BochsDebug_v1_0_s_ifspec, NULL, NULL);
	return CreateThread(NULL, 0, RpcListeningThread, NULL, 0, NULL);
}

void StopRpcServer()
{
	WorkerList.Shutdown();
	RpcMgmtStopServerListening(NULL);
	RpcServerUnregisterIf(NULL, NULL, FALSE);
}

void CollectWorker(PWorkerProc proc, PVOID para)
{
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	CmdWorkItem NewItem;
	NewItem.data.pWorker = proc;
	NewItem.data.hCompletionEvent = hEvent;
	NewItem.data.pParam = para;
	NewItem.data.Status = 0;
	__try{
		if(!WorkerList.Push(&NewItem))
		{
			RaiseException(RPC_S_CALL_FAILED, EXCEPTION_NONCONTINUABLE , NULL, NULL);
		}
		SetEvent(WorkingItemReadyEvent.get());
		WaitForSingleObject(hEvent, INFINITE);
		if(NewItem.data.Status)
		{
			RaiseException(NewItem.data.Status, EXCEPTION_NONCONTINUABLE , NULL, NULL);
		}
	}
	__finally{
		CloseHandle(hEvent);
	}
}

extern "C"{

	void Bochs_QuitSimulation(IN RPC_BINDING_HANDLE h1)
	{
		CollectWorker(Bochs_QuitSimulationImpl, NULL);
	}

	void Bochs_PauseSimulation(IN RPC_BINDING_HANDLE h1)
	{
		Bochs_PauseSimulationImpl();
	}

	void Bochs_ResumeSimulation(IN RPC_BINDING_HANDLE h1)
	{
		CollectWorker(Bochs_ResumeSimulationImpl, NULL);
	}

	void Bochs_WaitForIdle(IN RPC_BINDING_HANDLE h1)
	{
		CollectWorker(Bochs_WaitForIdleImpl, NULL);
	}

	error_status_t Bochs_ReadLinearMemory(IN RPC_BINDING_HANDLE h1, __int64 StartAddr, __int32 Length, byte Buffer[  ])
	{
		ReadMemoryParas para = {StartAddr, Length, Buffer};
		CollectWorker(Bochs_ReadLinearMemoryImpl, &para);
		return para.Status;
	}

	void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
	{
		return(malloc(len));
	}

	void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
	{
		free(ptr);
	}

}