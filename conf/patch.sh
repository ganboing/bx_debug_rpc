#!/bin/sh
dir=$(dirname "$0")
echo "patching vc project configuration"
patch --binary $dir/../../vs2013/bx_debug.vcxproj $dir/bx_debug.vcxproj.diff
