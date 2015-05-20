#define _SCL_SECURE_NO_WARNINGS

#include <newstd.h>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <sstream>
#include "BochsRpcShared.h"

GEN_WINAPI_EH_STATUS(RPC_S_OK, RpcServerUseProtseqEp);
GEN_WINAPI_EH_STATUS(RPC_S_OK, RpcServerRegisterIf);
GEN_WINAPI_EH_RESULT(NULL, CreateThread);
GEN_WINAPI_EH_RESULT(NULL, HeapCreate);
GEN_WINAPI_EH_RESULT(NULL, CreateEvent);
GEN_WINAPI_EH_RESULT(0, SetEvent);

bool bx_rpc_debugging;

DWORD WINAPI _BochsRpcServer::RpcListeningThreadProc(LPVOID pthis)
{
	_BochsRpcServer* _this = reinterpret_cast<_BochsRpcServer*>(pthis);
	RPC_STATUS ret = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, 0);
	_this->CheckHeap();
	//check if the heap is in a valid status and all memory allocated are freed
	return ret;
}
ManagedHANDLE _BochsRpcServer::StartRpcListenThread(const char* name)
{
	auto& addr_format = "Bochs_Debug_Port_%s";
	auto len = _snprintf(nullptr, 0, addr_format, name);
	auto addr_buf = (char*)_alloca(len + 1);

	auto ret_len = _snprintf(addr_buf, len + 1, addr_format, name);
	assert(ret_len == len);

	printf("Bochs Rpc Debugger tries to listen on %s\n", addr_buf);
	
	EH_RpcServerUseProtseqEp((unsigned char*)"ncalrpc", RPC_C_LISTEN_MAX_CALLS_DEFAULT, (unsigned char*)addr_buf, NULL);
	printf("\t RpcServerUseProtseqEp: OK\n");
	
	EH_RpcServerRegisterIf(BochsDebug_v1_0_s_ifspec, NULL, NULL);
	printf("\t RpcServerRegisterIf: OK\n");

	ManagedHANDLE hthread(EH_CreateThread(NULL, 0, RpcListeningThreadProc, this, CREATE_SUSPENDED, NULL));
	printf("\t Rpc Listening thread created\n");
	return hthread;
}
_BochsRpcServer::_BochsRpcServer(const char* name, bool _debug)
	:
	debug_heap(_debug),
	server_shutdown(false),
	RpcDataHeap(EH_HeapCreate(_debug ? HEAP_NO_SERIALIZE:0, 0, 0)),
	WorkingItemReadyEvent(EH_CreateEvent(NULL, FALSE, FALSE, NULL)),
	hRpcListeningThread(StartRpcListenThread(name))
{
	printf("bochds-rpc: heap created [%p]\n", RpcDataHeap.get());
	bx_rpc_debugging = true;
}
_BochsRpcServer::~_BochsRpcServer()
{
	printf("wait for rpc server to shutdown\n");
	WaitForSingleObject(hRpcListeningThread.get(), INFINITE);
}

bool _BochsRpcServer::Enqueue(const BochsRpcJobItem* item)
{
	if (server_shutdown || !WorkerList.Enqueue(item))
	{
		return false;
	}
	SetEvent(WorkingItemReadyEvent.get());
	return true;
}
void _BochsRpcServer::Dequeue()
{
	const BochsRpcJobItem* NewItem;
	if (server_shutdown)
	{
		while ((NewItem = WorkerList.Dequeue()) != NULL){
			NewItem->Cancel();
		}
		bx_dbg_quit_command();
	}
	NewItem = WorkerList.Dequeue();
	if (NewItem != NULL)
	{
		NewItem->Run();
	}
	else
	{
		WaitForSingleObject(WorkingItemReadyEvent.get(), INFINITE);
	}
}
void _BochsRpcServer::Stop()
{
	if (!server_shutdown.exchange(true))
	{
		RpcMgmtStopServerListening(NULL);
		RpcServerUnregisterIf(NULL, NULL, FALSE);
		SetEvent(WorkingItemReadyEvent.get());
		//break the request processing wait
	}
}
void _BochsRpcServer::CheckHeap()
{
	if (debug_heap)
	{
		::std::lock_guard<::std::mutex> l(heap_lock);
		if (!HeapValidate(RpcDataHeap.get(), 0, NULL))
		{
			OutputDebugStringA("bochs-rpc: RpcHeap is in a invalid state\n");
			return;
		}
		if (heap_alloc_set.size() != 0)
		{
			OutputDebugStringA("bochs-rpc: RpcHeap is not completely freed\n");
			::std::stringstream buf;
			for (auto i = heap_alloc_set.begin(), j = heap_alloc_set.end(); i != j; ++i)
			{
				buf << "\t heap block: [" << *i << "]\n";
			}
			OutputDebugStringA(buf.str().c_str());
		}
	}
}
void _BochsRpcServer::BochsRpcJobWaitItem::Cancel() const
{
	cancelled = true;
	SetEvent(Event.get());
}
void _BochsRpcServer::BochsRpcJobItem::Cancel() const
{}

void* _BochsRpcServer::midl_malloc(size_t len)
{
	if (debug_heap){
		void* ret = nullptr;
		bool exist = false;
		{
			::std::lock_guard<::std::mutex> l(heap_lock);
			ret = HeapAlloc(RpcDataHeap.get(), 0, len); 
			if (ret)
			{
				exist = !heap_alloc_set.insert(ret).second;
			}
		}
		::std::stringstream buf;
		buf << "bochs-rpc: allocating [" << len << "] from debug heap, ret= [" << ret << "]\n";
		if (exist)
		{
			buf << "\t the allocated block [" << ret << "] is already in the set\n";
		}
		OutputDebugStringA(buf.str().c_str());
		return ret;
	}
	else
	{
		return HeapAlloc(RpcDataHeap.get(), 0, len);
	}
}
void _BochsRpcServer::midl_free(void* p)
{
	if (p != nullptr)
	{
		if (debug_heap)
		{
			bool deleted = false;
			{
				::std::lock_guard<::std::mutex> l(heap_lock);
				if (heap_alloc_set.erase(p))
				{
					deleted = true;
					HeapFree(RpcDataHeap.get(), 0, p);
				}
			}
			::std::stringstream buf;
			buf << "bochs-rpc: deleting [" << p << "] from debug heap\n";
			if (!deleted)
			{
				buf << "\t the allocated block " << p << "is not found in the set (Possible double free??)\n";
			}
			OutputDebugStringA(buf.str().c_str());
		}
		else
		{
			HeapFree(RpcDataHeap.get(), 0, p);
		}
	}
}

#define RPC_PARA_PREFIX "-rpc:"
#define RPC_PARA_DEBUG "-rpc-debug"
#define RPC_PARA_EVENT_PREFIX "-rpc-event="

BOOL WINAPI SetAndCloseEvent(HANDLE Event)
{
	BOOL ret = SetEvent(Event) && CloseHandle(Event);
	if (!ret)
	{
		printf("bochs-rpc: rpc notify parent failed\n");
	}
	return ret;
}

static struct _BochsRpcInit
{
	template<typename F>
	static char* FindAndRemoveArgv(F&& f)
	{
		char** p_para = ::std::find_if(__argv, __argv + __argc, std::forward<F>(f));
		char* ret = nullptr;
		if (p_para != __argv + __argc)
		{
			ret = *p_para;
			//make use of the fact that argv[argc] == null
			::std::copy(p_para + 1, __argv + __argc + 1, p_para);
			--__argc;
		}
		return ret;
	}

	std::unique_ptr<_BochsRpcServer> pBochsRpcServer;
	_BochsRpcInit()
	{
		//printf("bochs console HWND=%p\n", GetConsoleWindow());
		char* rpc_name = FindAndRemoveArgv(
			[](char* _para){return !strncmp(_para, RPC_PARA_PREFIX, sizeof(RPC_PARA_PREFIX) - 1); }
			);
		bool rpc_debug = !!FindAndRemoveArgv(
			[](char* _para){return !strcmp(_para, RPC_PARA_DEBUG); }
			);
		HANDLE rpc_parent_ctrl = NULL;
		FindAndRemoveArgv(
			[&rpc_parent_ctrl](char* _para){return sscanf(_para, RPC_PARA_EVENT_PREFIX "%p", &rpc_parent_ctrl) == 1; }
			);
		if (rpc_name){
			rpc_name += sizeof(RPC_PARA_PREFIX) - 1;
			printf("bochs-rpc: rpc enabled servername[%s]\n", rpc_name);
			if (rpc_debug)
			{
				printf("bochs-rpc: rpc heap debugging\n");
			}
			if (rpc_parent_ctrl)
			{
				printf("bochs-rpc: rpc notify parent\n");
			}
			ManagedHANDLE parent_event(rpc_parent_ctrl);
			try{
				pBochsRpcServer.reset(new _BochsRpcServer(rpc_name, rpc_debug));
			}
			catch (::std::exception& e){
				printf("bochs-rpc: RpcServer startup failed, err=%s\n", e.what());
			}
			catch (DWORD& err){
				printf("bochs-rpc: RpcServer startup failed, err=%lX\n", (unsigned long)err);
			}
			EH_SetEvent(rpc_parent_ctrl);
		}
	}
}BochsRpcInit;

_BochsRpcServer* bx_rpc_server = BochsRpcInit.pBochsRpcServer.get();

void bx_rpc_get_command(void)
{
	return bx_rpc_server->Dequeue();
}

extern "C"
{
	void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
	{
		return bx_rpc_server->midl_malloc(len);
	}

	void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
	{
		return bx_rpc_server->midl_free(ptr);
	}
}
