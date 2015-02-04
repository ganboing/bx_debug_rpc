Bochs RPC Debugger

A modified bochs internal debugger to support debugging through RPC (Remote Procedure Call) on win32.
Feature request & bugfix: ganboing@gmail.com. 

Windows only (ver >= 5.1)
The project is synced with bochs trunk. 

Prerequsite:

1. Source code of Bochs trunk. (Checked out from Bochs SVN)
2. Visual C++ of version >=12.0 (Visual C++ 2013 or higher)
3. A working bash shell on Windows. (See Bochs notes of "Compiling on Win32").
4. The following utilities:
	unzip
	svn
	patch

Build environment setup:

Clone the git repository to "bx_debug_rpc" directory under bochs source directory. Please make sure the directory layout is identical to:

	[Bochs Source Directory]/bx_debug_rpc/
											BochsRpcDbgInterface.cpp
											BochsRpcDbgServer.cpp
											bx_debug_extra.cpp
											......

The All-in-one configure script is located in "bx_debug_rpc/conf/configure.win32.rpc.sh". It will prepare all the necessary files for compilation. After all works are done, simply open the solution file "vs2013/bochs.sln" to build.

In general, it should be fine to execute the script as long as your bochs workspace is clean, e.g., a fresh checkout from the SVN. It is highly recommended to use minGW or Cygwin shells to execute the script. Remember to "login" to the bash shell via "Visual Studio Command Prompt", otherwise the configure script will not able to detect the Visual C++ compiler.

In case of anything goes wrong, it is also possible to control different stage of the configuration process by utilizing the following scripts:

	conf/fetch.sh : Extract the VC project files to bochs workspace
	conf/patch.sh : Patch the source codes along with the svn:ignore property.
	conf/config.sh : Configure the bochs workspace.

Note: The All-in-one script invoke these helper scripts above accordingly.

Usage:

The commandline of the RPC debugger version of bochs enables the following options to bochs
	
	-rpc:[%s] (Required): Specifies the server name string.
	-rpc-debug (Optional): Enable debug messages.
	-rpc-event=[%p] (Optional): Specifies the synchronization Event.

A sample client program is included in bx_debug_rpc/bx_sample_client.

TODO: design docs.
