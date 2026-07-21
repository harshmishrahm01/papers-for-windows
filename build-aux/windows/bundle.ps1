# GNOME Papers Windows Bundle Script
# Collects papers.exe, dlls, backends, GSettings schemas, and assets into dist/ folder.

$ErrorActionPreference = "Stop"

# 1. Setup paths
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
if (Test-Path (Join-Path $scriptDir "..\..\meson.build")) {
    $projectRoot = (Get-Item (Join-Path $scriptDir "..\..")).FullName
} else {
    $projectRoot = (Get-Item .).FullName
}
Set-Location $projectRoot

$distDir = Join-Path $projectRoot "dist"
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
        throw "MSYS2 was not found at C:\msys64."
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
if (Test-Path "build\thumbnailer\papers-thumbnailer.exe") {
    Copy-Item "build\thumbnailer\papers-thumbnailer.exe" $binDir -Force
} elseif (Test-Path "build\thumbnailer\release\papers-thumbnailer.exe") {
    Copy-Item "build\thumbnailer\release\papers-thumbnailer.exe" $binDir -Force
}
Copy-Item "build\previewer\papers-previewer.exe" $binDir -Force
Copy-Item "build\libdocument\libppsdocument-4.0-6.dll" $binDir -Force
Copy-Item "build\libview\libppsview-4.0-5.dll" $binDir -Force
Copy-Item "build\libdocument\backend\*.dll" $backendsDir -Force
Copy-Item "build\libdocument\backend\*.papers-backend" $backendsDir -Force
Copy-Item "build\data\gschemas.compiled" $schemasDir -Force

Write-Host "Resolving and copying DLL dependencies via ldd..." -ForegroundColor Cyan
$buildPathPosix = ($projectRoot -replace '\\', '/' -replace '^([A-Za-z]):', '/$1')
$lddCmd = "export PATH=/ucrt64/bin:$buildPathPosix/build/libdocument:$buildPathPosix/build/libview:`$PATH && ldd $buildPathPosix/build/shell/src/papers.exe $buildPathPosix/build/previewer/papers-previewer.exe $buildPathPosix/build/libdocument/backend/*.dll"
$lddOutput = & $bashExe -lc $lddCmd

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
if (Test-Path (Join-Path $ucrtShare "icons\Adwaita")) {
    Copy-Item -Path (Join-Path $ucrtShare "icons\Adwaita") -Destination $iconsDir -Recurse -Container -Force
}
if (Test-Path (Join-Path $ucrtShare "icons\hicolor")) {
    Copy-Item -Path (Join-Path $ucrtShare "icons\hicolor") -Destination $iconsDir -Recurse -Container -Force
}
if (Test-Path (Join-Path $ucrtShare "mime")) {
    Copy-Item -Path (Join-Path $ucrtShare "mime") -Destination (Join-Path $distDir "share") -Recurse -Container -Force
}
if (Test-Path (Join-Path $ucrtLib "gdk-pixbuf-2.0")) {
    Copy-Item -Path (Join-Path $ucrtLib "gdk-pixbuf-2.0\*") -Destination $pixbufDir -Recurse -Container -Force
}

Write-Host ""
Write-Host "==========================================================" -ForegroundColor Green
Write-Host "   Bundling complete! Staged application is in:" -ForegroundColor Green
Write-Host "   $distDir" -ForegroundColor Yellow
Write-Host "==========================================================" -ForegroundColor Green
