#!/bin/sh
dir=$(dirname "$0")
echo "diffing vc project configuration"
unzip.exe $dir/../../build/win32/vs2013ex-workspace.zip -d $dir/vs2013_wk_tmp
diff -ur $dir/vs2013_wk_tmp/vs2013/ $dir/../../vs2013/ | grep --binary -v "^Only" > $dir/vcproj.patch
rm -rf $dir/vs2013_wk_tmp
