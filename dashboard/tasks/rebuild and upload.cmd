@echo off

call "rebuild.cmd" nopause
if not defined ABORT (
  echo.
  call "upload binaries.cmd" nopause
)

call "pause.cmd" %1