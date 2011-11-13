@ECHO OFF
SETLOCAL
CALL MSBuild_x64.bat msbuild-deploy.xml /p:Configuration=RELEASE