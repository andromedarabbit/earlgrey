@ECHO On
SETLOCAL

SET CURRENT_DIR=%~dp0
SET SHFBROOT=%CURRENT_DIR%EWSoftware\Sandcastle Help File Builder
SET DXROOT=%CURRENT_DIR%Sandcastle

IF NOT EXIST "%DXROOT%" CALL "%CURRENT_DIR%..\..\src\Msbuild_Win32.bat" "%CURRENT_DIR%..\..\src\BuildTools\msbuild-initialize.xml" /t:Initialize

"%SHFBROOT%\SandcastleBuilderGUI.exe" "%CURRENT_DIR%..\..\src\BuildTools\MSBuild\UserGuide\UserGuideProject.shfbproj"
