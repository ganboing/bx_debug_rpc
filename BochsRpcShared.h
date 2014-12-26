#include <atomic>
#include <Windows.h>
#include "ManagedRes.h"
#include "TaskQueue.hpp"

class _BochsRpcServer
{
public:
	struct BochsRpcJobItem{
		virtual void Run() const = 0;
		virtual void Cancel() const = 0;
	};

	struct BochsRpcJobWaitItem : BochsRpcJobItem{
		mutable bool cancelled;
		const ManagedHANDLE Event;
		BochsRpcJobWaitItem();
	};
private:
	static DWORD WINAPI RpcListeningThread(LPVOID);
	::std::atomic_bool server_shutdown;
	ManagedHANDLE hRpcListeningThread;
	ManagedHANDLE WorkingItemReadyEvent;
	TaskQueue<const BochsRpcJobItem, 0x10> WorkerList;
	HANDLE Start(const char* name);
public:
	_BochsRpcServer() = delete;
	_BochsRpcServer(const char* name);
	~_BochsRpcServer();
	bool Enqueue(const BochsRpcJobItem* item);
	void Dequeue();
	void Stop();
};

extern _BochsRpcServer*& bx_rpc_server;