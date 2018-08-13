@echo off

call "build.cmd" nopause
if not defined ABORT (
  echo.
  call "upload binaries.cmd" nopause
)

call "pause.cmd" %1