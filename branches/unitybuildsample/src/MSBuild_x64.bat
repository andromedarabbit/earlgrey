@echo off
SETLOCAL
call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" x64 >> _temp.txt
call "C:\WINDOWS\Microsoft.NET\Framework64\v3.5\msbuild.exe" %*
SET ERR_LEVEL=%errorlevel%
exit /b %ERR_LEVEL%
