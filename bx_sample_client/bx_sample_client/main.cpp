#include <windows.h>
#include <cstdio>
#include <iostream>
#include <string>
#include "../../rpc_gen/BochsRpcDbg.h"

::std::string command;

int main(int argc, char** argv)
{
	unsigned char* BindingStr;
	printf("server: %s\n", argv[1]);

	RpcStringBindingCompose(NULL,
		(unsigned char*)"ncalrpc",
		NULL,
		(unsigned char*)argv[1],
		NULL,
		&BindingStr);

	if (RpcBindingFromStringBinding(BindingStr, &BochsDebug_v1_0_c_ifspec) != RPC_S_OK)
	{
		printf("rpc bind failed\n");
		exit(1);
	}
	while (true)
	{
		RpcTryExcept
		{
			::std::cin >> command;
			if (command == "e")
			{
				break;
			}
			if (command == "r")
			{
				Bochs_PrintRegs(BochsDebug_v1_0_c_ifspec);
			}
			else if (command == "c")
			{
				Bochs_ResumeSimulation(BochsDebug_v1_0_c_ifspec);
			}
			else if (command == "q")
			{
				Bochs_QuitSimulation(BochsDebug_v1_0_c_ifspec);
			}
			else if (command == "p")
			{
				Bochs_PauseSimulation(BochsDebug_v1_0_c_ifspec);
			}
			else if (command == "w")
			{
				Bochs_WaitForIdle(BochsDebug_v1_0_c_ifspec);
			}
			else if (command == "v")
			{
				char* ret =
					Bochs_GetVersion(BochsDebug_v1_0_c_ifspec);
				printf("bochs version [%s]", ret);
				midl_user_free(ret);
			}
		}
			RpcExcept(1)
		{
			unsigned long ulCode = RpcExceptionCode();
			printf("Runtime reported exception 0x%lx = %ld\n", ulCode, ulCode);
		}
		RpcEndExcept
	}
}

void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
{
	return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
{
	free(ptr);
}