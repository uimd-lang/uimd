@echo off

call "%~dp0uimd_dev.cmd" mcp-compare-example %*
if errorlevel 1 exit /b %ERRORLEVEL%
