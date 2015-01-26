#!/bin/sh
dir=$(dirname "$0")
remote=ganboing2@frs.sourceforge.net:/home/pfs/project/bochsrpcdebugger/build
echo "build project using "$NUMBER_OF_PROCESSORS" cpus"
pushd $dir/../vs2013
MSBuild.exe bochs.vcxproj -p:Configuration=Debug -t:Build -maxcpucount:$NUMBER_OF_PROCESSORS
MSBuild.exe bochs.vcxproj -p:Configuration=Release -t:Build -maxcpucount:$NUMBER_OF_PROCESSORS
popd
echo "pushing to sf"
scp $dir/../obj-debug/bochs.exe $remote/Debug/bochs.exe
scp $dir/../obj-debug/bochs.pdb $remote/Debug/bochs.pdb
scp $dir/../obj-release/bochs.exe $remote/Release/bochs.exe
scp $dir/../obj-release/bochs.pdb $remote/Release/bochs.pdb

