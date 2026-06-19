param(
    [string]$BuildDir = 'cpp\build-windows',
    [string]$Config = 'Release',
    [string]$CompareAppSize = '90x35',
    [switch]$NoMcpFast,
    [switch]$NoRebuild
)

$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $PSScriptRoot
$python = if (Get-Command python -ErrorAction SilentlyContinue) { 'python' } else { 'py' }
$arguments = @(
    'tools\uimd_dev.py',
    '--build-dir',
    $BuildDir,
    '--config',
    $Config,
    'test-all',
    '--compare-app-size',
    $CompareAppSize
)
if ($NoMcpFast) {
    $arguments += '--no-mcp-fast'
}
if ($NoRebuild) {
    $arguments += '--no-rebuild'
}

Push-Location $root
try {
    & $python @arguments
    exit $LASTEXITCODE
}
finally {
    Pop-Location
}
