param(
    [switch]$Test,
    [string]$BuildDir = 'cpp\build-windows',
    [string]$Config = 'Release'
)

$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $PSScriptRoot
$python = if (Get-Command python -ErrorAction SilentlyContinue) { 'python' } else { 'py' }
$arguments = @('tools\uimd_dev.py', '--build-dir', $BuildDir, '--config', $Config, 'rebuild-all')
if ($Test) {
    $arguments += '--test'
}

Push-Location $root
try {
    & $python @arguments
    exit $LASTEXITCODE
}
finally {
    Pop-Location
}
