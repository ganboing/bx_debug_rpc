#!/bin/sh
dir=$(dirname "$0")
svnrevision=$(svn info "$dir"/../ | grep Revision | awk '{print $2}')
remotedir=/home/pfs/project/bochsrpcdebugger/build
remote=ganboing2@frs.sourceforge.net
echo "build project using "$NUMBER_OF_PROCESSORS" cpus"
pushd $dir/../vs2013
MSBuild.exe bochs.vcxproj -p:Configuration=Debug -t:Build -maxcpucount:$NUMBER_OF_PROCESSORS
MSBuild.exe bochs.vcxproj -p:Configuration=Release -t:Build -maxcpucount:$NUMBER_OF_PROCESSORS
popd
echo "pushing to sf"
bgsign.sh $dir/../obj-debug/bochs.exe
bgsign.sh $dir/../obj-release/bochs.exe
scp $dir/../obj-debug/bochs.exe $remote:$remotedir/Debug/bochs.svn-$svnrevision.exe
scp $dir/../obj-debug/bochs.pdb $remote:$remotedir/Debug/bochs.svn-$svnrevision.pdb
scp $dir/../obj-release/bochs.exe $remote:$remotedir/Release/bochs.svn-$svnrevision.exe
scp $dir/../obj-release/bochs.pdb $remote:$remotedir/Release/bochs.svn-$svnrevision.pdb
