@echo off

call "target directory.cmd"

if not exist %TARGETDIR% mkdir %TARGETDIR%
if not exist %TARGETDIR%\resources mkdir %TARGETDIR%\resources

cd ..\source
echo EBOOT.PBP
copy EBOOT.PBP %TARGETDIR%\EBOOT.PBP
echo PARAM.SFO
copy PARAM.SFO %TARGETDIR%\PARAM.SFO

cd ..\tasks

call "pause.cmd" %1