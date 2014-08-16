#pragma once
#include <Windows.h>
#include <cstdlib>
#include <stdint.h>

class SpinLock{
private:
	volatile mutable LONG lock;
public:
	inline SpinLock(){
		lock = 0;
	}
	inline void Acquire() const
	{
		while(InterlockedCompareExchange(&lock, 1, 0)){}
	}
	inline void Release() const
	{
		lock = 0;
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
	TaskQueue()
	{
		head = NULL;
		tail = &head;
		shutdown = 0;
	}
	void Shutdown()
	{
		shutdown = 1;
	}
	bool IsShutDown() const
	{
		return shutdown;
	}
	bool Push(PElementType pElement)
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
	PElementType Pop()
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