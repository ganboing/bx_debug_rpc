#pragma once
#include <Windows.h>
#include <memory>
#include <stdexcept>

template<decltype(CloseHandle) D>
class _HANDLE_Deleter{
public:
	inline void operator() (HANDLE p)
	{
		D(p);
	}
};

//The default dtor is CloseHandle
template<decltype(CloseHandle) D = CloseHandle>
using ManagedHANDLE = ::std::unique_ptr < void, _HANDLE_Deleter<D> > ;

inline HANDLE CheckHandle(HANDLE h)
{
	if (h == NULL)
	{
		throw ::std::runtime_error("handle is null");
	}
	return h;
}