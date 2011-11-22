@ECHO On
SETLOCAL

SET CURRENT_DIR=%~dp0
SET SHFBROOT=%CURRENT_DIR%EWSoftware\Sandcastle Help File Builder
SET DXROOT=%CURRENT_DIR%Sandcastle

"%SHFBROOT%\SandcastleBuilderGUI.exe"
