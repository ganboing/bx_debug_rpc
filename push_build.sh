#!/bin/sh
dir=$(dirname "$0")
remote=ganboing2@frs.sourceforge.net:/home/pfs/project/bochsrpcdebugger/build
echo "build project"
pushd $dir/../vs2013
MSBuild.exe bochs.vcxproj -p:Configuration=Debug -t:Build
MSBuild.exe bochs.vcxproj -p:Configuration=Release -t:Build
popd
echo "pushing to sf"
scp $dir/../obj-debug/bochs.exe $remote/Debug/bochs.exe
scp $dir/../obj-debug/bochs.pdb $remote/Debug/bochs.pdb
scp $dir/../obj-release/bochs.exe $remote/Release/bochs.exe
scp $dir/../obj-release/bochs.pdb $remote/Release/bochs.pdb

