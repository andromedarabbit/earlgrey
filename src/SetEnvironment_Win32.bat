@echo off
SET PATH=%~dp0..\vendor\strawberry-perl-5.12.1.0-portable;%~dp0..\vendor\strawberry-perl-5.12.1.0-portable\perl\bin;%~dp0..\vendor\BuildTool\SubversionClient\v1.6.13\CollabNet\Subversion Client;%PATH%

call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
SET ERR_LEVEL=%errorlevel%
exit /b %ERR_LEVEL%
