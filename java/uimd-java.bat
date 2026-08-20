@echo off
setlocal EnableExtensions EnableDelayedExpansion

@rem Resolve the Java 17 JDK used by the UIMD Java target. This file is
@rem shipped with the SDK target and copied into application distributions.

set "UIMD_REQUIRED_JAVA_MAJOR=17"
set "UIMD_JAVA_MODE=check"
set "UIMD_JAVA_OUTPUT_PATH="
set "UIMD_JAVA_QUIET=0"
set "UIMD_RESOLVED_JAVA_HOME="

:parseArguments
if "%~1"=="" goto resolveJava
if "%~1"=="--check" (
    set "UIMD_JAVA_MODE=check"
    shift
    goto parseArguments
)
if "%~1"=="--java-home" (
    set "UIMD_JAVA_MODE=home"
    shift
    goto parseArguments
)
if "%~1"=="--java-command" (
    set "UIMD_JAVA_MODE=command"
    shift
    goto parseArguments
)
if "%~1"=="--set-env" (
    set "UIMD_JAVA_MODE=setenv"
    shift
    goto parseArguments
)
if "%~1"=="--write-home" (
    if "%~2"=="" (
        call :error "uimd-java: --write-home requires a path"
        exit /b 2
    )
    set "UIMD_JAVA_MODE=write"
    set "UIMD_JAVA_OUTPUT_PATH=%~2"
    shift
    shift
    goto parseArguments
)
if "%~1"=="--quiet" (
    set "UIMD_JAVA_QUIET=1"
    shift
    goto parseArguments
)
if "%~1"=="--help" goto usage
if "%~1"=="-h" goto usage
call :error "uimd-java: unknown option: %~1"
exit /b 2

:resolveJava
if defined UIMD_JAVA_HOME (
    call :acceptHome "%UIMD_JAVA_HOME%"
    if defined UIMD_RESOLVED_JAVA_HOME goto output
    call :error "uimd-java: UIMD_JAVA_HOME does not point to a Java 17 JDK: %UIMD_JAVA_HOME%"
    exit /b 1
)

if defined JAVA_HOME call :acceptHome "%JAVA_HOME%"
if defined UIMD_RESOLVED_JAVA_HOME goto output

for /f "delims=" %%J in ('where java.exe 2^>NUL') do (
    if not defined UIMD_RESOLVED_JAVA_HOME (
        for %%H in ("%%~dpJ..") do call :acceptHome "%%~fH"
    )
)
if defined UIMD_RESOLVED_JAVA_HOME goto output

if defined ProgramFiles (
    for /d %%H in (
        "%ProgramFiles%\Java\jdk-17*"
        "%ProgramFiles%\Eclipse Adoptium\jdk-17*"
        "%ProgramFiles%\Microsoft\jdk-17*"
        "%ProgramFiles%\Amazon Corretto\jdk17*"
        "%ProgramFiles%\Zulu\zulu-17*"
        "%ProgramFiles%\Zulu\zulu17*"
    ) do (
        if not defined UIMD_RESOLVED_JAVA_HOME call :acceptHome "%%~fH"
    )
)
if defined UIMD_RESOLVED_JAVA_HOME goto output

call :error "uimd-java: Java 17 JDK was not found."
call :error "Install a Java 17 JDK once; UIMD then discovers JAVA_HOME, PATH, and standard installation directories automatically."
call :error "Use UIMD_JAVA_HOME only for a non-standard location."
exit /b 1

:acceptHome
set "UIMD_CANDIDATE_HOME=%~1"
if not exist "!UIMD_CANDIDATE_HOME!\bin\java.exe" exit /b 0
if not exist "!UIMD_CANDIDATE_HOME!\bin\javac.exe" exit /b 0
set "UIMD_JAVA_VERSION_FILE=%TEMP%\uimd-java-version-!RANDOM!-!RANDOM!.txt"
"!UIMD_CANDIDATE_HOME!\bin\javac.exe" -version >"!UIMD_JAVA_VERSION_FILE!" 2>&1
if errorlevel 1 (
    del /q "!UIMD_JAVA_VERSION_FILE!" >NUL 2>&1
    exit /b 0
)
set "UIMD_JAVA_VERSION_LINE="
set /p UIMD_JAVA_VERSION_LINE=<"!UIMD_JAVA_VERSION_FILE!"
del /q "!UIMD_JAVA_VERSION_FILE!" >NUL 2>&1
set "UIMD_JAVA_VERSION="
for /f "tokens=2" %%V in ("!UIMD_JAVA_VERSION_LINE!") do set "UIMD_JAVA_VERSION=%%V"
if "!UIMD_JAVA_VERSION!"=="17" set "UIMD_RESOLVED_JAVA_HOME=!UIMD_CANDIDATE_HOME!"
if "!UIMD_JAVA_VERSION:~0,3!"=="17." set "UIMD_RESOLVED_JAVA_HOME=!UIMD_CANDIDATE_HOME!"
if "!UIMD_JAVA_VERSION:~0,3!"=="17-" set "UIMD_RESOLVED_JAVA_HOME=!UIMD_CANDIDATE_HOME!"
exit /b 0

:output
if "%UIMD_JAVA_MODE%"=="home" (
    echo !UIMD_RESOLVED_JAVA_HOME!
    exit /b 0
)
if "%UIMD_JAVA_MODE%"=="command" (
    echo !UIMD_RESOLVED_JAVA_HOME!\bin\java.exe
    exit /b 0
)
if "%UIMD_JAVA_MODE%"=="write" (
    >"%UIMD_JAVA_OUTPUT_PATH%" echo !UIMD_RESOLVED_JAVA_HOME!
    exit /b 0
)
if "%UIMD_JAVA_MODE%"=="setenv" (
    for %%H in ("!UIMD_RESOLVED_JAVA_HOME!") do endlocal & set "JAVA_HOME=%%~fH" & exit /b 0
)
echo Java 17 JDK: !UIMD_RESOLVED_JAVA_HOME!
exit /b 0

:error
if "%UIMD_JAVA_QUIET%"=="0" 1>&2 echo %~1
exit /b 0

:usage
echo usage: uimd-java [--check ^| --java-home ^| --java-command ^| --set-env]
echo                  [--write-home path] [--quiet]
exit /b 0
