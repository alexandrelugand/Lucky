@echo off
pushd ..
start /I /B /WAIT cmd /C "make clean"
start /I /B /WAIT cmd /C "make -j8 config=debug all"
popd
