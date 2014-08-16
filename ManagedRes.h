#pragma once
#include <Windows.h>

class ManagedHANDLE{
private:
	HANDLE h;
public:
	inline ManagedHANDLE():h(NULL)
	{
	}
	inline ManagedHANDLE(HANDLE p):h(p)
	{
	}
	inline HANDLE get() const 
	{
		return h;
	}
	inline HANDLE release() 
	{
		HANDLE ret = h;
		h=NULL;
		return ret;
	}
	inline ~ManagedHANDLE()
	{
		if(h)
		{
			CloseHandle(h);
		}
	}
};