@echo off
SETX /S "%COMPUTERNAME%" GURTLE_ISSUE_UPDATE_CMD "\"%~dp0scripts\updissue.cmd\" --username {username} --password {password} --project {project} --issue {issue.id} --status {status} --comment {comment} --debug"
SET ERR_LEVEL=%errorlevel%
if %ERR_LEVEL% NEQ 0 exit /b %ERR_LEVEL%

exit /b 0
