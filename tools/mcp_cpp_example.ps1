param(
    [Parameter(Mandatory = $true)]
    [string]$Name,
    [Parameter(Mandatory = $true)]
    [string]$Yaml,
    [string]$BuildDir = 'cpp\build-windows',
    [string]$Config = 'Release',
    [string]$CompareAppSize,
    [switch]$McpFast
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$python = if (Get-Command python -ErrorAction SilentlyContinue) { 'python' } else { 'py' }
$arguments = @('tools\uimd_dev.py', '--build-dir', $BuildDir, '--config', $Config, 'mcp-cpp-example', $Name, $Yaml)
if ($CompareAppSize) {
    $arguments += @('--compare-app-size', $CompareAppSize)
}
if ($McpFast) {
    $arguments += '--mcp-fast'
}

Push-Location $root
try {
    & $python @arguments
    exit $LASTEXITCODE
}
finally {
    Pop-Location
}
