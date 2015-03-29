#pragma once
#include <atomic>
#include <array>
#include <algorithm>

//XXX:The VC buggy headers force me to have reinterpret_cast all over the world

template<class T, size_t S>
class TaskQueue{
private:
	std::array<std::atomic<uintptr_t>, S> ptrs;
	std::atomic_size_t p_reserve;
	std::atomic_size_t p_free;
	/*
		[--free--|-----01010101010101----]
		         | <-commit + reserved-> |
				p_f                     p_r
	*/
public:
	TaskQueue()
		:
		ptrs{}, p_reserve(0), p_free(0)
	{}
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
		ptrs[p_r_old].store(reinterpret_cast<uintptr_t>(newdata));
		return true;
	}
	T* Dequeue()
	{
		size_t p_f = p_free.load();
		if (p_f == p_reserve.load())
		{
			return nullptr;
		}
		T* ret = reinterpret_cast<T*>(ptrs[p_f].load());
		if (!ret){
			return nullptr;
		}
		ptrs[p_f].store(reinterpret_cast<uintptr_t>(nullptr));
		p_free.store((p_f + 1) % S);
		return ret;
	}
};