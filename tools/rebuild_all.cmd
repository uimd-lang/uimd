@echo off

if /I "%~1"=="-Test" (
    call "%~dp0uimd_dev.cmd" rebuild-all --test
    if errorlevel 1 exit /b %ERRORLEVEL%
    goto :eof
)

if /I "%~1"=="/Test" (
    call "%~dp0uimd_dev.cmd" rebuild-all --test
    if errorlevel 1 exit /b %ERRORLEVEL%
    goto :eof
)

call "%~dp0uimd_dev.cmd" rebuild-all %*
if errorlevel 1 exit /b %ERRORLEVEL%
