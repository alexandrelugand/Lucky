@echo off
pushd ..
del /S /Q bin\windows\x86_64\Debug\Lucky.lib
del /S /Q bin\browser\Debug\Lucky.a

del /S /Q bin\windows\x86_64\Debug\Sandbox.exe
del /S /Q bin\browser\Debug\index.data
del /S /Q bin\browser\Debug\index.html
del /S /Q bin\browser\Debug\index.js
del /S /Q bin\browser\Debug\index.wasm

rmdir /S /Q bin-int\windows\x86_64\Debug\Lucky
rmdir /S /Q bin-int\browser\Debug\Lucky
rmdir /S /Q bin-int\windows\x86_64\Debug\Sandbox
rmdir /S /Q bin-int\browser\Debug\Sandbox
popd
