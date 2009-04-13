# 32ºñÆ®¿ë
@echo off
SET PATHSAVED=%PATH% > _temp.txt
call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" x86 >> _temp.txt
"C:\WINDOWS\Microsoft.NET\Framework\v3.5\msbuild.exe" %*
SET ERR_LEVEL=%errorlevel%
SET PATH=%PATHSAVED% >> _temp.txt
SET PATHSAVED=>> _temp.txt
exit /b %ERR_LEVEL%
