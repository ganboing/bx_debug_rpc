#pragma once
#include <Windows.h>
#include <cstdlib>

class SpinLock{
private:
	volatile mutable LONG lock;
public:
	inline SpinLock(){
		lock = 0;
	}
	inline void Acquire() const
	{
		while(InterlockedExchange(&lock, 1)){
			Sleep(0);
		}
	}
	inline void Release() const
	{
		InterlockedExchange(&lock, 1);
	}
};

template<class T>
class TaskQueue{
private:
	SpinLock lock;
	volatile bool shutdown;
	class ListEntry;
	struct EntryHeader{
		ListEntry* volatile next;
	};
	struct ListEntry : EntryHeader{
		T data;
	};
	ListEntry* volatile head;
	ListEntry* volatile* volatile tail;
public:
	typedef ListEntry ElementType, *PElementType;
	TaskQueue() throw ()
	{
		head = NULL;
		tail = &head;
		shutdown = 0;
	}
	void Shutdown() throw()
	{
		shutdown = 1;
		MemoryBarrier();
	}
	bool IsShutDown() const throw()
	{
		return shutdown;
	}
	bool Enqueue(PElementType pElement) throw()
	{
		if(!shutdown)
		{
			lock.Acquire();
			*tail = pElement;
			pElement->next = NULL;
			tail = &(pElement->next);
			lock.Release();
			return true;
		}
		return false;
	}
	PElementType Dequeue() throw()
	{
		lock.Acquire();
		PElementType ret = NULL;
		if(head != NULL)
		{
			ret = head;
			head = head->next;
			if(head == NULL)
			{
				tail = &head;
			}
		}
		lock.Release();
		return ret;
	}
};