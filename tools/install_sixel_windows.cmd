@echo off
setlocal EnableExtensions

set "REPO_ROOT=%~dp0.."
for %%I in ("%REPO_ROOT%") do set "REPO_ROOT=%%~fI"

if not defined UIMD_MSYS2_ROOT set "UIMD_MSYS2_ROOT=C:\msys64"
set "MSYS2_ROOT=%UIMD_MSYS2_ROOT%"
set "MSYS2_BASH=%MSYS2_ROOT%\usr\bin\bash.exe"
set "MSYS2_ENV=%MSYS2_ROOT%\usr\bin\env.exe"
set "UIMD_SIXEL_INSTALL_DIR=%MSYS2_ROOT%\ucrt64\bin"
set "UIMD_SIXEL_DLL=%UIMD_SIXEL_INSTALL_DIR%\libsixel-1.dll"
set "UIMD_SIXEL_MSYS2_SCRIPT=%REPO_ROOT%\tools\install_sixel_windows_msys2.sh"

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
    exit /b 1
)

if not exist "%MSYS2_BASH%" (
    where winget >nul 2>nul
    if errorlevel 1 (
        echo error: MSYS2 is missing and winget was not found.
        echo install MSYS2 first, or set UIMD_MSYS2_ROOT to an existing MSYS2 root.
        exit /b 1
    )

    echo Installing MSYS2 with winget...
    winget install --id MSYS2.MSYS2 -e --accept-package-agreements --accept-source-agreements
    if errorlevel 1 exit /b %ERRORLEVEL%
)

if not exist "%MSYS2_BASH%" (
    echo error: MSYS2 bash was not found at %MSYS2_BASH%.
    echo set UIMD_MSYS2_ROOT to the MSYS2 install root if it is not C:\msys64.
    exit /b 1
)

if not exist "%MSYS2_ENV%" (
    echo error: MSYS2 env was not found at %MSYS2_ENV%.
    exit /b 1
)

echo Installing Python Sixel binding...
%PYTHON_CMD% -m pip install libsixel-python
if errorlevel 1 exit /b %ERRORLEVEL%

echo Ensuring native libsixel is installed...
"%MSYS2_ENV%" MSYSTEM=UCRT64 CHERE_INVOKING=1 /usr/bin/bash "%UIMD_SIXEL_MSYS2_SCRIPT%"
if errorlevel 1 exit /b %ERRORLEVEL%

if not exist "%UIMD_SIXEL_DLL%" (
    echo error: libsixel build finished but %UIMD_SIXEL_DLL% was not found.
    exit /b 1
)

set "UIMD_LIBSIXEL_DIR=%UIMD_SIXEL_INSTALL_DIR%"
setx UIMD_LIBSIXEL_DIR "%UIMD_SIXEL_INSTALL_DIR%" >nul
if errorlevel 1 exit /b %ERRORLEVEL%

echo Verifying UIMD can load libsixel...
set "PYTHONPATH=%REPO_ROOT%\src;%REPO_ROOT%\python;%PYTHONPATH%"
%PYTHON_CMD% -c "from uimd.runtime import image; ok = image._load_libsixel(); print('uimd libsixel load:', ok); print('detail:', image._LIBSIXEL_ERROR); raise SystemExit(0 if ok else 1)"
if errorlevel 1 exit /b %ERRORLEVEL%

set "FINAL_UIMD_LIBSIXEL_DIR=%UIMD_SIXEL_INSTALL_DIR%"
endlocal & set "UIMD_LIBSIXEL_DIR=%FINAL_UIMD_LIBSIXEL_DIR%"

echo.
echo Sixel is ready.
echo Current shell: UIMD_LIBSIXEL_DIR=%UIMD_LIBSIXEL_DIR%
echo Future shells: UIMD_LIBSIXEL_DIR was saved with setx.
echo.
goto :eof
