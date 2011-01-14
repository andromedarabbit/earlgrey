@echo off
SET PATH=%PATH%;%~dp0..\vendor\strawberry-perl-5.12.1.0-portable;%~dp0..\vendor\strawberry-perl-5.12.1.0-portable\perl\bin

call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" x64
SET ERR_LEVEL=%errorlevel%
exit /b %ERR_LEVEL%
