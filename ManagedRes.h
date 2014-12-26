#pragma once
#include <Windows.h>
#include <memory>

class _HANDLE_Deleter{
public:
	inline void operator() (void* p)
	{
		CloseHandle(p);
	}
};

typedef ::std::unique_ptr<void, _HANDLE_Deleter> ManagedHANDLE;