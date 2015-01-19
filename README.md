Bochs RPC Debugger

A modified bochs internal debugger to support debugging through RPC.

Windows only (ver >= 5.1)
The project is synced with bochs trunk. 

Prerequsite:

1. Source code of Bochs trunk. (Checked out from Bochs SVN)
2. A working bash shell on Windows. (See Bochs notes of "Compiling on Win32").

Build environment setup:

1. Configure Bochs via the configure script. Note that the "--enable-debugger" flag is appended in addition to the default win32 configure script.

	./configure --target=pentium-windows \
            --enable-sb16 \
            --enable-ne2000 \
            --enable-all-optimizations \
            --enable-cpu-level=6 \
            --enable-x86-64 \
            --enable-vmx=2 \
            --enable-pci \
            --enable-clgd54xx \
            --enable-voodoo \
            --enable-usb \
            --enable-usb-ohci \
            --enable-usb-xhci \
            --enable-es1370 \
            --enable-e1000 \
            --enable-show-ips \
            --disable-readline \
            --without-x \
            --with-win32 \
            --with-rfb 
            --with-nogui \
            --enable-debugger

	Caution: following the steps in the bochs release notes under "Compiling on Win32 with Microsoft VC++" may lead to unexpected behavior, since the configure script deduces lots of parameters and modifies multiple header files (namely "bxversion.h" "bxversion.rc" "config.h" "ltdlconf.h") based on the environment it runs. Hence transfer the sources and configure on a different platform might be fatal. It is highly recommended to use minGW or Cygwin shells to execute configure script. Remember to "login" to the bash shell via "Visual Studio Command Prompt", otherwise the configure script will not able to detect the Visual C++ compiler.

2. After the configure is done, extract the vs2013 project configuration files from "build\win32\vs2013ex-workspace.zip" to Bochs source directory. Please make sure the vs2013 solution file is located in the following path. 

	[Bochs Source Directory]/vs2013/bochs.sln 

3. Clone the git repository to "bx_debug_rpc" directory under bochs source directory. Please make sure the directory layout is identical to:

	[Bochs Source Directory]/bx_debug_rpc/
											BochsRpcDbgInterface.cpp
											BochsRpcDbgServer.cpp
											bx_debug_extra.cpp
											......

4. Invoke the `patch.sh` in bx_debug_rpc directory to patch the source files along with other VC project files. 

	[Bochs Source Directory]/bx_debug_rpc/patch.sh

	Please note that the patch script will also modify the svn:ignore attribute of the top level source directory, eg [Bochs Source Directory]. Additional ignored directories are:
		
		obj-debug
		obj-release
		vs2013
		bx_debug_rpc

5. Finally, open the bochs VS solution file and build the project "bochs".

Usage:

//TODO