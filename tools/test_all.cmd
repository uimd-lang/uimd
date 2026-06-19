@echo off

call "%~dp0uimd_dev.cmd" test-all %*
if errorlevel 1 exit /b %ERRORLEVEL%
