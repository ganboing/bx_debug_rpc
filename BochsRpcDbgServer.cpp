#include "BochsRpcShared.h"
#include "rpc_gen/BochsRpcDbg.h"
#include "bx_rpc_bridging.h"
#include <stdexcept>

#pragma comment(lib, "rpcrt4")

bool bx_rpc_debugging;

DWORD WINAPI _BochsRpcServer::RpcListeningThread(LPVOID)
{
	return RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, 0);
}
HANDLE _BochsRpcServer::Start(const char* name)
{
	char AddressBuilder[MAX_PATH];
	sprintf(AddressBuilder, "Bochs_Debug_Port_%s", name);
	printf("Bochs Rpc Debugger tries to listen on %s\n", AddressBuilder);
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
	HANDLE hthread;
	if (!(hthread = CreateThread(NULL, 0, RpcListeningThread, NULL, 0, NULL)))
	{
		throw ::std::runtime_error("create Rpc thread failed");
	}
	printf("Bochs Rpc Debugger Init succeed\n");
	return hthread;
}
_BochsRpcServer::_BochsRpcServer(const char* name)
	:
	hRpcListeningThread(Start(name)),
	WorkingItemReadyEvent(CreateEvent(NULL, FALSE, FALSE, NULL))
{
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
	server_shutdown.store(true);
	RpcMgmtStopServerListening(NULL);
	RpcServerUnregisterIf(NULL, NULL, FALSE);
}
_BochsRpcServer::BochsRpcJobWaitItem::BochsRpcJobWaitItem()
	: Event(CreateEvent(NULL, FALSE, FALSE, NULL)), cancelled(false)
{}

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

_BochsRpcServer*& bx_rpc_server = BochsRpcInit.pBochsRpcServer;

void bx_rpc_get_command(void)
{
	return bx_rpc_server->Dequeue();
}

extern "C"
{
	void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
	{
		return(malloc(len));
	}

	void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
	{
		free(ptr);
	}
}
