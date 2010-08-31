@echo on
SETX /S %COMPUTERNAME% GURTLE_SCRIPT "%~dp0scripts\updissue.cmd"
SET ERR_LEVEL=%errorlevel%
if %ERR_LEVEL% NEQ 0 exit /b %ERR_LEVEL%

SETX /S %COMPUTERNAME% GURTLE_ISSUE_UPDATE_CMD "%GURTLE_SCRIPT% --username {username} --password {password} --project {project} --issue {issue.id} --status {status} --comment {comment} --debug"
SET ERR_LEVEL=%errorlevel%
if %ERR_LEVEL% NEQ 0 exit /b %ERR_LEVEL%

exit /b 0
