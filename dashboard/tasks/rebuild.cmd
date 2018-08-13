@echo off

call "clean.cmd" nopause
echo.
call "build.cmd" nopause

call "pause.cmd" %1