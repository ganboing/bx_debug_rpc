//MUST!! be compiled with /EHa

#include "BochsRpcShared.h"
#include "bxversion.h"

typedef void(*pNoWaitRpcJob)();

template<pNoWaitRpcJob pJob>
struct BochsNoWaitJobs{
	struct _Job : _BochsRpcServer::BochsRpcJobItem
	{
		virtual void Run() const
		{
			pJob();
		}
		virtual void Cancel() const{}
		//cancel the job has no effect
	};
	static const _Job Job;
};

template <pNoWaitRpcJob pJob>
const typename BochsNoWaitJobs<pJob>::_Job BochsNoWaitJobs<pJob>::Job;

template<pNoWaitRpcJob pJob>
void AddBochsJobNoWait()
{
	if (!bx_rpc_server->Enqueue(&BochsNoWaitJobs<pJob>::Job))
	{
		RaiseException(RPC_S_SERVER_UNAVAILABLE, EXCEPTION_NONCONTINUABLE, NULL, NULL);
	}
}

template<typename F>
void AddBochsJobWait(const F& job)
{
	struct _NewItem : _BochsRpcServer::BochsRpcJobWaitItem
	{
		const F& j;
		virtual void Run() const{
			j();
			SetEvent(Event.get());
		}
		_NewItem(HANDLE _Event, const F& _j) 
			:BochsRpcJobWaitItem(_Event), j(_j)
		{}
	};
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!hEvent)
	{
		RaiseException(RPC_S_OUT_OF_RESOURCES, EXCEPTION_NONCONTINUABLE, NULL, NULL);
	}
	const _NewItem NewItem(hEvent, job);
	if (!bx_rpc_server->Enqueue(&NewItem))
	{
		RaiseException(RPC_S_SERVER_UNAVAILABLE, EXCEPTION_NONCONTINUABLE, NULL, NULL);
	}
	//wait for the operation to complete
	WaitForSingleObject(NewItem.Event.get(), INFINITE);
	if (NewItem.cancelled)
	{
		//raise if cancelled
		RaiseException(RPC_S_CALL_CANCELLED, EXCEPTION_NONCONTINUABLE, NULL, NULL);
	}
}

extern "C"{

	char* Bochs_GetVersion(IN RPC_BINDING_HANDLE h1)
	{
		char* buf = (char*)midl_user_allocate(sizeof(VER_STRING));
		strcpy(buf, VER_STRING);
		return buf;
	}

	void Bochs_QuitSimulation(IN RPC_BINDING_HANDLE h1)
	{
		bx_rpc_server->Stop();
		bx_debug_break();
		//break the main loop
	}

	void Bochs_PauseSimulation(IN RPC_BINDING_HANDLE h1)
	{
		bx_debug_break();
	}

	void Bochs_ResumeSimulation(IN RPC_BINDING_HANDLE h1)
	{
		AddBochsJobNoWait<bx_dbg_continue_command>();
	}

	void Bochs_PrintRegs(IN RPC_BINDING_HANDLE h1)
	{
		AddBochsJobNoWait<bx_dbg_rpc_print_reg>();
	}

	void Bochs_WaitForIdle(IN RPC_BINDING_HANDLE h1)
	{
		AddBochsJobWait(
			[]{}
		);
	}

	boolean Bochs_ReadLinearMemory(IN RPC_BINDING_HANDLE h1, unsigned __int64 StartAddr, unsigned __int32 Length, byte Buffer[]){
		boolean ret;
		AddBochsJobWait(
			[=, &ret]{ret = bx_dbg_rpc_read_linear(StartAddr, Length, Buffer); }
		);
		return ret;
	}

	boolean Bochs_ReadPhysicalMemory(IN RPC_BINDING_HANDLE h1, unsigned __int64 StartAddr, unsigned __int32 Length, byte Buffer[]){
		boolean ret;
		AddBochsJobWait(
			[=, &ret]{ret = bx_dbg_rpc_read_physical(StartAddr, Length, Buffer); }
		);
		return ret;
	}

	void Bochs_GetGPRs(IN RPC_BINDING_HANDLE h1, PBochsGPRsContext context){
		AddBochsJobWait(
			[=]{bx_dbg_rpc_read_gprs(context); }
		);
	}
}