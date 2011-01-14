@echo off
SETLOCAL
call "SetEnvironment_Win32.bat"
call "C:\WINDOWS\Microsoft.NET\Framework\v3.5\msbuild.exe" %*
SET ERR_LEVEL=%errorlevel%
exit /b %ERR_LEVEL%
