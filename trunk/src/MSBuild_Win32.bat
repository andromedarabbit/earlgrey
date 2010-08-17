@echo off
SETLOCAL
call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
call "C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319\msbuild.exe" %*
SET ERR_LEVEL=%errorlevel%
exit /b %ERR_LEVEL%
