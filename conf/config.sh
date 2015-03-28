#!/bin/sh
dir=$(dirname "$0")
cd $dir/../../
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
--with-win32 --with-rfb --with-nogui --enable-debugger
