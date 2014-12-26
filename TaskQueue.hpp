#pragma once
#include <atomic>
#include <algorithm>
//#include <mutex>

template<class T, size_t S>
class TaskQueue{
private:
	T* data[S];
	::std::atomic_size_t p_reserve;
	::std::atomic_size_t p_commit;
	::std::atomic_size_t p_free;
	/*
		[--free--|--commit--|--reserved--]
		         |          |            |
				p_f        p_c          p_r
	*/
public:
	TaskQueue()
	{
		::std::fill(data, data + S, nullptr);
	}
	bool Enqueue(T* newdata)
	{
		size_t p_r_old = p_reserve.load();
		size_t p_r_new;
		do{
			p_r_new = (p_r_old + 1) % S;
			if (p_r_new == p_free.load())
			{
				return false;
			}
		} while (!p_reserve.compare_exchange_strong(p_r_old, p_r_new));
		data[p_r_old] = newdata;
		while (p_commit.load() != p_r_old);
		p_commit.store(p_r_new);
		return true;
	}
	T* Dequeue()
	{
		size_t p_f = p_free.load();
		if (p_f == p_commit.load())
		{
			return nullptr;
		}
		p_free.store((p_f + 1) % S);
		return data[p_f];
	}
};

/*template<class T>
class TaskQueue{
private:
	T*  head;
	T** tail;
	::std::atomic_bool shutdown;
	::std::mutex mut;
public:
	typedef T ElementType, *PElementType;
	TaskQueue()
		: head(nullptr), tail(&head)
	{
	}
	void Shutdown()
	{
		shutdown.store(true);
	}
	bool IsShutDown() const
	{
		return shutdown.load();
	}
	bool Enqueue(PElementType pElement)
	{
		if(!shutdown.load())
		{
			::std::lock_guard<::std::mutex> lock(mut);
			*tail = pElement;
			tail = &pElement->next;
			return true;
		}
		return false;
	}
	PElementType Dequeue()
	{
		PElementType ret = nullptr;
		::std::lock_guard<::std::mutex> lock(mut);
		if(&head != tail)
		{
			ret = head;
			if(tail == &head->next)
			{
				tail = &head;
			}
			else
			{
				head = head->next;
			}
		}
		return ret;
	}
};*/