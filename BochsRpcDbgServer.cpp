#include <Windows.h>
#include <Rpc.h>

#include <cstdio>
#include <memory>
#include <stdexcept>

#include "RpcShared.h"
#include "bx_rpc_bridging.h"
#include "../bx_debug/debug.h"

#include "TaskQueue.hpp"
#include "ManagedRes.h"
typedef TaskQueue<WorkerParamPack>::ElementType CmdWorkItem;
typedef TaskQueue<WorkerParamPack>::PElementType PCmdWorkItem;

#pragma comment(lib, "rpcrt4")

#if BX_SUPPORT_SMP
#error "Bochs should be configured in non-SMP mode to allow rpc debug"
#endif

bool bx_rpc_debugging;
static void DispatchWorker(PCmdWorkItem CurrentItem)
{
	CurrentItem->data.Status = 0;
	CurrentItem->data.pWorker(CurrentItem->data.pParam, CurrentItem->data.hCompletionEvent);
}

static void CancelWorker(PCmdWorkItem CurrentItem)
{
	CurrentItem->data.Status = RPC_S_CALL_FAILED;
	SetEvent(CurrentItem->data.hCompletionEvent);
}

class _BochsRpcServer
{
	ManagedHANDLE hRpcListeningThread;
	ManagedHANDLE WorkingItemReadyEvent;
	TaskQueue<WorkerParamPack> WorkerList;

	static DWORD WINAPI RpcListeningThread(LPVOID)
	{
		return RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, 0);
	}

	HANDLE Start(const char* name)
	{
		char AddressBuilder[MAX_PATH];
		sprintf(AddressBuilder, "Bochs_Debug_Port_%s", name /*_PID_%08I32X" */ /*, GetCurrentProcessId()*/);
		printf("\n----------\nBochs Rpc Debugger is listening on %s\n----------\n", AddressBuilder);
		RPC_STATUS ret;
		ret = RpcServerUseProtseqEp((unsigned char*)"ncalrpc", RPC_C_LISTEN_MAX_CALLS_DEFAULT, (unsigned char*)AddressBuilder, NULL);
		if (ret != RPC_S_OK)
		{
			throw ::std::runtime_error("RpcServerUseProtseqEp failed");
		}
		ret = RpcServerRegisterIf(BochsDebug_v1_0_s_ifspec, NULL, NULL);
		if (ret != RPC_S_OK)
		{
			throw ::std::runtime_error("RpcServerRegisterIf failed");
		}
		return CreateThread(NULL, 0, RpcListeningThread, NULL, 0, NULL);
	}

public:
	_BochsRpcServer() = delete;
	_BochsRpcServer(const char* name)
		:
		hRpcListeningThread(Start(name)),
		WorkingItemReadyEvent(CreateEvent(NULL, FALSE, FALSE, NULL))
	{
		bx_rpc_debugging = true;
	}
	~_BochsRpcServer()
	{
		printf("wait for rpc server to shutdown\n");
		WaitForSingleObject(hRpcListeningThread.get(), INFINITE);
	}
	bool Enqueue(PCmdWorkItem item)
	{
		if (!WorkerList.Enqueue(item))
		{
			return false;
		}
		SetEvent(WorkingItemReadyEvent.get());
		return true;
	}
	void Dequeue()
	{
		PCmdWorkItem NewItem;
		if (WorkerList.IsShutDown())
		{
			while ((NewItem = WorkerList.Dequeue()) != NULL){
				CancelWorker(NewItem);
			}
			bx_dbg_quit_command();
		}
		NewItem = WorkerList.Dequeue();
		if (NewItem != NULL)
		{
			DispatchWorker(NewItem);
		}
		else
		{
			WaitForSingleObject(WorkingItemReadyEvent.get(), INFINITE);
		}
	}
	void Stop()
	{
		WorkerList.Shutdown();
		RpcMgmtStopServerListening(NULL);
		RpcServerUnregisterIf(NULL, NULL, FALSE);
	}
};

static struct _BochsRpcInit
{
	_BochsRpcServer* pBochsRpcServer;
	_BochsRpcInit()
		:pBochsRpcServer(nullptr)
	{
		static const char rpc_para_prefix[] = "-rpc:";
		const char* rpc_name;
		int i;
		for (i = 0; i < __argc; ++i)
		{
			if (!strncmp(__argv[i], rpc_para_prefix, sizeof(rpc_para_prefix) - 1))
			{
				rpc_name = __argv[i] + sizeof(rpc_para_prefix) - 1;
				printf("rpc enabled : servername[%s]\n", rpc_name);
				break;
			}
		}
		if (i != __argc)
		{
			do{
				__argv[i] = __argv[i + 1];
			} while (++i != __argc);
			//we make use the fact that argv[argc] == null
			--__argc;
			try{
				pBochsRpcServer = new _BochsRpcServer(rpc_name);
			}
			catch (::std::exception& e){
				printf("RpcServer startup failed, err=%s\n", e.what());
			}
		}
	}
	~_BochsRpcInit()
	{
		delete pBochsRpcServer;
	}
}BochsRpcInit;

static _BochsRpcServer*& bx_rpc_server = BochsRpcInit.pBochsRpcServer;
void bx_rpc_get_command(void)
{
	return bx_rpc_server->Dequeue();
}

/*#if BX_SUPPORT_X86_64
#error "Bochs should be configured without x86_64 support"
#endif*/

void Bochs_PauseSimulationImpl()
{
	bx_debug_break();
}

void Bochs_ResumeSimulationImpl(PVOID, HANDLE hEvent)
{
	SetEvent(hEvent);
	bx_dbg_continue_command();
}

void Bochs_ReadLinearMemoryImpl(PVOID _para, HANDLE hEvent)
{
	PReadMemoryParas para = (PReadMemoryParas)_para;
	para->Status = bx_dbg_read_linear(dbg_cpu, para->StartAddr, para->Length, para->Buffer);
	SetEvent(hEvent);
}

void Bochs_ReadPhysicalMemoryImpl(PVOID _para, HANDLE hEvent)
{
	/*PReadMemoryParas para = (PReadMemoryParas)_para;
	para->Status = BX_MEM(0)->dbg_fetch_mem(BX_CPU(dbg_cpu), (bx_phy_address)para->StartAddr, para->Length, para->Buffer);
	*/
	// 0 - error, 1 - success
	SetEvent(hEvent);
}

void Bochs_GetSegRegImpl(PVOID _para, HANDLE hEvent)
{

}

void Bochs_GetGPRsImpl(PVOID _para, HANDLE hEvent)
{
	/*PReadGPRsParas para = (PReadGPRsParas)_para;
	PBochsGPRsContext pContext = para->pContext;
	pContext->EAX = BX_CPU(dbg_cpu)->get_reg32(BX_32BIT_REG_EAX);
	pContext->ECX = BX_CPU(dbg_cpu)->get_reg32(BX_32BIT_REG_ECX);
	pContext->EDX = BX_CPU(dbg_cpu)->get_reg32(BX_32BIT_REG_EDX);
	pContext->EBX = BX_CPU(dbg_cpu)->get_reg32(BX_32BIT_REG_EBX);
	pContext->ESP = BX_CPU(dbg_cpu)->get_reg32(BX_32BIT_REG_ESP);
	pContext->EBP = BX_CPU(dbg_cpu)->get_reg32(BX_32BIT_REG_EBP);
	pContext->ESI = BX_CPU(dbg_cpu)->get_reg32(BX_32BIT_REG_ESI);
	pContext->EDI = BX_CPU(dbg_cpu)->get_reg32(BX_32BIT_REG_EDI);
	pContext->EIP = bx_dbg_get_instruction_pointer();
	pContext->EFLAGS = BX_CPU(dbg_cpu)->read_eflags();
	para->Status = 0;*/
	SetEvent(hEvent);
}

void Bochs_WaitForIdleImpl(PVOID, HANDLE hEvent)
{
	SetEvent(hEvent);
}

void Bochs_QuitSimulationImpl(PVOID, HANDLE hEvent)
{
	bx_rpc_server->Stop();
	SetEvent(hEvent);
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
		if(!bx_rpc_server->Enqueue(&NewItem))
		{
			RaiseException(RPC_S_CALL_FAILED, EXCEPTION_NONCONTINUABLE , NULL, NULL);
		}
		//wait for the operation to complete
		WaitForSingleObject(hEvent, INFINITE);
		if(NewItem.data.Status)
		{
			//raise if any error
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

	error_status_t Bochs_ReadLinearMemory(IN RPC_BINDING_HANDLE h1, unsigned __int32 StartAddr, unsigned __int32 Length, byte Buffer[])
	{
		ReadMemoryParas para = {StartAddr, Length, Buffer};
		CollectWorker(Bochs_ReadLinearMemoryImpl, &para);
		return para.Status;
	}

	error_status_t Bochs_ReadPhysicalMemory(IN RPC_BINDING_HANDLE h1, unsigned __int32 StartAddr, unsigned __int32 Length, byte Buffer[])
	{
		ReadMemoryParas para = { StartAddr, Length, Buffer };
		CollectWorker(Bochs_ReadPhysicalMemoryImpl, &para);
		return para.Status;
	}

	error_status_t Bochs_GetGPRs(IN RPC_BINDING_HANDLE h1, PBochsGPRsContext context)
	{
		ReadGPRsParas para = { context };
		CollectWorker(Bochs_GetGPRsImpl, &para);
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