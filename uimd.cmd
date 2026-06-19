@echo off
setlocal

set "ROOT=%~dp0"
set "UIMD_EXE="

if exist "%ROOT%cpp\build-windows\tools\uimd\Release\uimd.exe" set "UIMD_EXE=%ROOT%cpp\build-windows\tools\uimd\Release\uimd.exe"
if not defined UIMD_EXE if exist "%ROOT%cpp\build-windows\tools\uimd\Debug\uimd.exe" set "UIMD_EXE=%ROOT%cpp\build-windows\tools\uimd\Debug\uimd.exe"
if not defined UIMD_EXE if exist "%ROOT%cpp\build-windows\tools\uimd\uimd.exe" set "UIMD_EXE=%ROOT%cpp\build-windows\tools\uimd\uimd.exe"
if not defined UIMD_EXE if exist "%ROOT%cpp\build\tools\uimd\Release\uimd.exe" set "UIMD_EXE=%ROOT%cpp\build\tools\uimd\Release\uimd.exe"
if not defined UIMD_EXE if exist "%ROOT%cpp\build\tools\uimd\Debug\uimd.exe" set "UIMD_EXE=%ROOT%cpp\build\tools\uimd\Debug\uimd.exe"
if not defined UIMD_EXE if exist "%ROOT%cpp\build\tools\uimd\uimd.exe" set "UIMD_EXE=%ROOT%cpp\build\tools\uimd\uimd.exe"

if not defined UIMD_EXE (
    echo error: repo-local uimd.exe was not found.
    echo Run .\tools\rebuild_all.cmd first.
    exit /b 1
)

"%UIMD_EXE%" %*
if errorlevel 1 exit /b %ERRORLEVEL%
