# GNOME Papers Windows Bundle Script
# Collects papers.exe, dlls, backends, GSettings schemas, and assets into dist/ folder.

$ErrorActionPreference = "Stop"

# 1. Setup paths relative to repository root
$repoRoot = (Get-Item $PSScriptRoot).Parent.Parent.FullName
Set-Location $repoRoot

$distDir = Join-Path $repoRoot "dist"
$binDir = Join-Path $distDir "bin"
$backendsDir = Join-Path $distDir "lib\papers\6\backends"
$schemasDir = Join-Path $distDir "share\glib-2.0\schemas"
$iconsDir = Join-Path $distDir "share\icons"
$pixbufDir = Join-Path $distDir "lib\gdk-pixbuf-2.0"

# Find MSYS2
$msysPath = "C:\msys64"
if (!(Test-Path "$msysPath\usr\bin\bash.exe")) {
    if (Test-Path "C:\nbin\msys64\usr\bin\bash.exe") {
        $msysPath = "C:\nbin\msys64"
    } else {
        $msysPath = Read-Host "MSYS2 was not found at C:\msys64. Please enter your MSYS2 install path"
    }
}

$bashExe = "$msysPath\usr\bin\bash.exe"
$ucrtBin = "$msysPath\ucrt64\bin"
$ucrtShare = "$msysPath\ucrt64\share"
$ucrtLib = "$msysPath\ucrt64\lib"

Write-Host "Creating staging directories in $distDir..." -ForegroundColor Cyan
New-Item -ItemType Directory -Force -Path $binDir | Out-Null
New-Item -ItemType Directory -Force -Path $backendsDir | Out-Null
New-Item -ItemType Directory -Force -Path $schemasDir | Out-Null
New-Item -ItemType Directory -Force -Path $iconsDir | Out-Null
New-Item -ItemType Directory -Force -Path $pixbufDir | Out-Null

Write-Host "Copying compiled binaries and backends..." -ForegroundColor Cyan
Copy-Item "build\shell\src\papers.exe" $binDir -Force
Copy-Item "build\thumbnailer\release\papers-thumbnailer.exe" $binDir -Force
Copy-Item "build\previewer\papers-previewer.exe" $binDir -Force
Copy-Item "build\libdocument\libppsdocument-4.0-6.dll" $binDir -Force
Copy-Item "build\libview\libppsview-4.0-5.dll" $binDir -Force
Copy-Item "build\libdocument\backend\*.dll" $backendsDir -Force
Copy-Item "build\libdocument\backend\*.papers-backend" $backendsDir -Force
Copy-Item "build\data\gschemas.compiled" $schemasDir -Force

Write-Host "Resolving and copying DLL dependencies via ldd..." -ForegroundColor Cyan
$posixRepo = "/" + $repoRoot.Substring(0,1).ToLower() + ($repoRoot.Substring(2) -replace '\\', '/')
$lddOutput = & $bashExe -lc "export PATH=/ucrt64/bin:$posixRepo/build/libdocument:$posixRepo/build/libview:`$PATH && ldd $posixRepo/build/shell/src/papers.exe $posixRepo/build/thumbnailer/release/papers-thumbnailer.exe $posixRepo/build/previewer/papers-previewer.exe $posixRepo/build/libdocument/backend/*.dll"

foreach ($line in $lddOutput) {
    if ($line -match '/ucrt64/bin/([^ ]+)') {
        $dllName = $Matches[1]
        $srcPath = Join-Path $ucrtBin $dllName
        if (Test-Path $srcPath) {
            Copy-Item $srcPath $binDir -Force
        }
    }
}

Write-Host "Copying UI assets, MIME database, and GdkPixbuf loaders..." -ForegroundColor Cyan
$adwaitaPath = Join-Path $ucrtShare "icons\Adwaita"
if (Test-Path $adwaitaPath) {
    Copy-Item -Path $adwaitaPath -Destination $iconsDir -Recurse -Container -Force
}
$hicolorPath = Join-Path $ucrtShare "icons\hicolor"
if (Test-Path $hicolorPath) {
    Copy-Item -Path $hicolorPath -Destination $iconsDir -Recurse -Container -Force
}
$mimePath = Join-Path $ucrtShare "mime"
if (Test-Path $mimePath) {
    Copy-Item -Path $mimePath -Destination (Join-Path $distDir "share") -Recurse -Container -Force
}
$pixbufPath = Join-Path $ucrtLib "gdk-pixbuf-2.0"
if (Test-Path $pixbufPath) {
    Copy-Item -Path "$pixbufPath\*" -Destination $pixbufDir -Recurse -Container -Force
}

Write-Host ""
Write-Host "==========================================================" -ForegroundColor Green
Write-Host "   Bundling complete! Staged application is in:" -ForegroundColor Green
Write-Host "   $distDir" -ForegroundColor Yellow
Write-Host "==========================================================" -ForegroundColor Green
