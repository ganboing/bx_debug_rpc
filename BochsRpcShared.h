#include <atomic>
#include <mutex>
#include <set>
#include <Windows.h>
#include "ManagedRes.h"
#include "TaskQueue.hpp"

#pragma comment(lib, "rpcrt4")

class _BochsRpcServer
{
public:
	struct BochsRpcJobItem{
		virtual void Run() const = 0;
		virtual void Cancel() const;
	};

	struct BochsRpcJobWaitItem : BochsRpcJobItem{
		mutable bool cancelled;
		const ManagedHANDLE<> Event;
		virtual void Cancel() const;
		inline BochsRpcJobWaitItem(HANDLE _Event)
			: Event(_Event), cancelled(false)
		{}
	};
private:
	static DWORD WINAPI RpcListeningThread(LPVOID);
	HANDLE Start(const char* name);
	void CheckHeap();
private:
	/*
		The Order of our data member is IMPORTANT
		Rpc listening thread must be the last one to initialize
	*/
	const bool debug_heap;
	::std::set<void*> heap_alloc_set;
	::std::mutex heap_lock;
	::std::atomic<bool> server_shutdown;
	TaskQueue<const BochsRpcJobItem, 0x10> WorkerList;
	ManagedHANDLE<HeapDestroy> RpcDataHeap;
	ManagedHANDLE<> WorkingItemReadyEvent;
	ManagedHANDLE<> hRpcListeningThread;
public:
	_BochsRpcServer() = delete;
	_BochsRpcServer(const char* name, bool _debug_heap);
	~_BochsRpcServer();
	void* midl_malloc(size_t);
	void midl_free(void*);
	bool Enqueue(const BochsRpcJobItem* item);
	void Dequeue();
	void Stop();
};

extern _BochsRpcServer*& bx_rpc_server;

#include "rpc_gen/BochsRpcDbg.h"
#include "bx_rpc_bridging.h"