@echo off

call "%~dp0uimd_dev.cmd" run-cpp-example %*
if errorlevel 1 exit /b %ERRORLEVEL%
