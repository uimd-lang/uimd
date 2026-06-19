param(
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$Arguments
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$candidates = @(
    "$root\cpp\build-windows\tools\uimd\Release\uimd.exe",
    "$root\cpp\build-windows\tools\uimd\Debug\uimd.exe",
    "$root\cpp\build-windows\tools\uimd\uimd.exe",
    "$root\cpp\build\tools\uimd\Release\uimd.exe",
    "$root\cpp\build\tools\uimd\Debug\uimd.exe",
    "$root\cpp\build\tools\uimd\uimd.exe"
)

foreach ($candidate in $candidates) {
    if (Test-Path -LiteralPath $candidate -PathType Leaf) {
        & $candidate @Arguments
        exit $LASTEXITCODE
    }
}

Write-Error "native UIMD launcher is not built. Run: .\tools\rebuild_all.ps1"
exit 127
