@echo off
SETLOCAL
call "SetEnvironment_x64.bat"
call "C:\WINDOWS\Microsoft.NET\Framework64\v3.5\msbuild.exe" %*
SET ERR_LEVEL=%errorlevel%
exit /b %ERR_LEVEL%
