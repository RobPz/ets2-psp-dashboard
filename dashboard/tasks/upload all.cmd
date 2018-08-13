@echo off

call "upload binaries.cmd" nopause
echo.
call "upload resources.cmd" nopause

call "pause.cmd" %1