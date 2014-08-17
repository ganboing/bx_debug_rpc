Bochs RPC Debugger

A modified bochs internal debugger to support debugging through RPC.

Windows only (ver >= 5.1)
The project is synced with bochs REL_2_6_6. 

Prerequsite:

Source code of Bochs REL_2_6_6. (Checked out from Bochs SVN)
A working bash shell on Windows. (See Bochs notes of "Compiling on Win32").

Setup the build environment:

First configure Bochs via `sh .conf.win32-vcpp`. The internal debugger must be enabled. ( "--enable-debugger" flag should be added to .conf.win32-vcpp). 
Caution: following the steps in the notes under "Compiling on Win32 with Microsoft VC++" may lead to unexpected behavior, since the configure script deduces lots of parameters and modifies multiple header files (namely "bxversion.h" "bxversion.rc" "config.h" "ltdlconf.h") based on the environment it runs. Hence transfer the sources configured on a different platform might be fatal. To simplify the configure procedure, we have already provide the sample configured header and resource files required for Visual Studio to build Bochs. If you decide to utilize the files we provided, the configure step can be skipped. Just extract the vs2008 directory compressed in "build\win32\vs2008ex-workspace.zip" to the Bochs source directory.

Second, clone the git repo to "bx_debug_rpc" directory under bochs source directory. Make sure the name is the same as the default ("bx_debug_rpc"). Invoke `make` in bx_debug_rpc directory to patch the VC project files and copy the configured header files if necessary. 

Finally, open the bochs VS solution file ("vs2008\bochs.sln") and build the project "bochs".
