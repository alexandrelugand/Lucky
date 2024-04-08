@echo off
cd "..\Browser\BlazorLuckyEditor\wwwroot"
@echo Create 'assets' symbolic link...
@mklink /J "assets" "..\..\..\LuckyEditor\assets"
@echo Create 'resources' symbolic link...
@mklink /J "resources" "..\..\..\LuckyEditor\resources"
@echo Done
pause
exit
