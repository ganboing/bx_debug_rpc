#pragma once
#include <atomic>
#include <algorithm>

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
		:
		p_reserve(0), p_commit(0), p_free(0)
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
		T* ret = data[p_f];
		data[p_f] = nullptr;
		p_free.store((p_f + 1) % S);
		return ret;
	}
};