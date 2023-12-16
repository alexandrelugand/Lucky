@echo off
pushd ..
call Vendors\Binaries\premake\windows\premake5.exe gmake2
popd