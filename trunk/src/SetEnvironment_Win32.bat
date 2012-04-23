@ECHO OFF
SET CURRENT_DIR=%~dp0
SET PATH=%CURRENT_DIR%..\vendor\BuildTool\strawberry-perl-5.12.1.0-portable;%CURRENT_DIR%..\vendor\BuildTool\strawberry-perl-5.12.1.0-portable\perl\bin;%CURRENT_DIR%..\vendor\BuildTool\SubversionClient\v1.7.0\CollabNet\Subversion Client;%PATH%

CALL "%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
SET ERR_LEVEL=%errorlevel%
EXIT /b %ERR_LEVEL%
