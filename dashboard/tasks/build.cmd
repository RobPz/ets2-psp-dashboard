@echo off

cd ..\source
make
if %errorlevel% neq 0 (
  set ABORT=1
)
cd ..\tasks

call "pause.cmd" %1