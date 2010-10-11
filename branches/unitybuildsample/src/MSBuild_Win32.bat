@echo off
SETLOCAL
call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
call "C:\WINDOWS\Microsoft.NET\Framework\v3.5\msbuild.exe" %*
SET ERR_LEVEL=%errorlevel%
exit /b %ERR_LEVEL%
