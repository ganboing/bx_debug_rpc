#pragma once
#include <Windows.h>

class ManagedHANDLE{
private:
	HANDLE h;
public:
	inline ManagedHANDLE() throw ():h(NULL)
	{
	}
	inline ManagedHANDLE(HANDLE p) throw () :h(p)
	{
	}
	inline HANDLE get() const  throw ()
	{
		return h;
	}
	inline HANDLE release()  throw ()
	{
		HANDLE ret = h;
		h=NULL;
		return ret;
	}
	inline ~ManagedHANDLE() throw ()
	{
		if(h)
		{
			CloseHandle(h);
		}
	}
};