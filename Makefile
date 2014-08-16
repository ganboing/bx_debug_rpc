../vs2008/bochs.vcproj: bochs.vcproj.patch
	patch ../vs2008/bochs.vcproj < bochs.vcproj.patch

../vs2008/bx_debug.vcproj: bx_debug.vcproj.patch
	patch ../vs2008/bx_debug.vcproj < bx_debug.vcproj.patch

../bx_debug/dbg_main.cc: dbg_main.patch
	patch ../bx_debug/dbg_main.cc < dbg_main.patch

BochsDbg_s.c: BochsDbg.idl
	midl /client none BochsDbg.idl

../bxversion.h:
	cp bxversion.h ../bxversion.h

../bxversion.rc:
	cp bxversion.rc ../bxversion.rc

../config.h:
	cp config.h ../config.h

../bxversion.rc:
	cp ltdlconf.h ../ltdlconf.h