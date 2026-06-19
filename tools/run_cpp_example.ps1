param(
    [Parameter(Mandatory = $true)]
    [string]$Name,
    [string]$BuildDir = 'cpp\build-windows',
    [string]$Config = 'Release',
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$AppArgs
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$python = if (Get-Command python -ErrorAction SilentlyContinue) { 'python' } else { 'py' }
$arguments = @('tools\uimd_dev.py', '--build-dir', $BuildDir, '--config', $Config, 'run-cpp-example', $Name)
if ($AppArgs) {
    $arguments += $AppArgs
}

Push-Location $root
try {
    & $python @arguments
    exit $LASTEXITCODE
}
finally {
    Pop-Location
}
