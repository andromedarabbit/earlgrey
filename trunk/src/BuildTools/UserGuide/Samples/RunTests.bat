@ECHO OFF
SETLOCAL
SET CURRENT_DIR=%~dp0

CALL "%CURRENT_DIR%MSBuild_Win32.bat" msbuild-run-tests.xml
SET ERR_LEVEL=%errorlevel%
EXIT /B %ERR_LEVEL%