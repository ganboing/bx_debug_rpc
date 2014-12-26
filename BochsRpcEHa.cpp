//MUST!! be compiled with /EHa

#include "BochsRpcShared.h"
#include "rpc_gen/BochsRpcDbg.h"
#include "bx_rpc_bridging.h"

typedef void(*pNoWaitRpcJob)();

/*void Bochs_QuitSimulationImpl(PVOID, HANDLE hEvent)
{
bx_rpc_server->Stop();
SetEvent(hEvent);
}*/

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
		RaiseException(RPC_S_CALL_FAILED, EXCEPTION_NONCONTINUABLE, NULL, NULL);
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
		virtual void Cancel() const{
			cancelled = true;
			SetEvent(Event.get());
		}
		_NewItem(const F& _j) : j(_j)
		{}
	};
	const _NewItem NewItem(job);
	if (!bx_rpc_server->Enqueue(&NewItem))
	{
		RaiseException(RPC_S_CALL_FAILED, EXCEPTION_NONCONTINUABLE, NULL, NULL);
	}
	//wait for the operation to complete
	WaitForSingleObject(NewItem.Event.get(), INFINITE);
	if (NewItem.cancelled)
	{
		//raise if canceled
		RaiseException(RPC_S_CALL_CANCELLED, EXCEPTION_NONCONTINUABLE, NULL, NULL);
	}
}

extern "C"{

	void Bochs_QuitSimulation(IN RPC_BINDING_HANDLE h1)
	{

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