@echo off

call "target directory.cmd"

if not exist %TARGETDIR% mkdir %TARGETDIR%
if not exist %TARGETDIR%\resources mkdir %TARGETDIR%\resources

cd ..\resources
copy * %TARGETDIR%\resources\

cd ..\tasks

call "pause.cmd" %1