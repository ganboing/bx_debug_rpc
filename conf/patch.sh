#!/bin/sh
dir=$(dirname "$0")
echo "patching vc project configuration"
patch -d $dir/../../vs2013 --binary -i ../bx_debug_rpc/conf/vcproj.diff
