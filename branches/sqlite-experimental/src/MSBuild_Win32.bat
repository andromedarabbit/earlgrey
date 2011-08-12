@ECHO OFF
SETLOCAL
SET CURRENT_DIR=%~dp0
CALL "%CURRENT_DIR%SetEnvironment_Win32.bat"
CALL "%WINDIR%\Microsoft.NET\Framework\v4.0.30319\msbuild.exe" %*
SET ERR_LEVEL=%errorlevel%
EXIT /b %ERR_LEVEL%
