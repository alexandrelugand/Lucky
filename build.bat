@echo off
start /I /B /WAIT cmd /C "make clean"
start /I /B /WAIT cmd /C "make -j"
