@echo off
setlocal

pushd "%~dp0.." || exit /b 1

set "PYTHON_CMD="
where python >nul 2>nul
if not errorlevel 1 set "PYTHON_CMD=python"

if not defined PYTHON_CMD (
    where py >nul 2>nul
    if not errorlevel 1 set "PYTHON_CMD=py"
)

if not defined PYTHON_CMD (
    where python3 >nul 2>nul
    if not errorlevel 1 set "PYTHON_CMD=python3"
)

if not defined PYTHON_CMD (
    echo error: python, py, or python3 was not found on PATH.
    popd
    exit /b 1
)

%PYTHON_CMD% tools\uimd_dev.py --build-dir cpp\build-windows --config Release %*
set "EXIT_CODE=%ERRORLEVEL%"
popd
if not "%EXIT_CODE%"=="0" exit /b %EXIT_CODE%
