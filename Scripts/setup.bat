@echo off
pushd ..
call Vendors\Binaries\premake\windows\premake5.exe gmake2
call Vendors\Binaries\premake\windows\premake5.exe vs2022
popd