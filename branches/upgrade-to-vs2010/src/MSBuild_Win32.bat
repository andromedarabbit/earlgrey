@ECHO OFF
SETLOCAL
SET CURRENT_DIR=%~dp0
CALL "%CURRENT_DIR%SetEnvironment_Win32.bat"
SET ERR_LEVEL=%errorlevel%
IF "%ERR_LEVEL%" NEQ "0" EXIT /B %ERR_LEVEL%

IF NOT EXIST "%WINDIR%\Microsoft.NET\Framework\v4.0.30319\msbuild.exe" GOTO ERROR_NOT_FOUND

CALL "%WINDIR%\Microsoft.NET\Framework\v4.0.30319\msbuild.exe" %*
SET ERR_LEVEL=%errorlevel%
EXIT /B %ERR_LEVEL%


:ERROR_NOT_FOUND
ECHO You should first install .NET Framework 4.0 SDK!
EXIT /B 999