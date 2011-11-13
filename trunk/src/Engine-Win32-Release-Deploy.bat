@ECHO OFF
SETLOCAL
CALL MSBuild_Win32.bat msbuild-deploy.xml /p:Configuration=RELEASE