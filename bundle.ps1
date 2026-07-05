# GNOME Papers Windows Bundle Script
# Collects papers.exe, dlls, backends, GSettings schemas, and assets into dist/ folder.

$ErrorActionPreference = "Stop"

# 1. Setup paths
$projectRoot = Get-Item .
$distDir = Join-Path $projectRoot "dist"
$binDir = Join-Path $distDir "bin"
$backendsDir = Join-Path $distDir "lib\papers\6\backends"
$schemasDir = Join-Path $distDir "share\glib-2.0\schemas"
$iconsDir = Join-Path $distDir "share\icons"
$pixbufDir = Join-Path $distDir "lib\gdk-pixbuf-2.0"

# Find MSYS2
$msysPath = "C:\msys64"
if (!(Test-Path "$msysPath\usr\bin\bash.exe")) {
    $msysPath = Read-Host "MSYS2 was not found at C:\msys64. Please enter your MSYS2 install path (e.g. C:\nbin\msys64)"
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
# Run ldd on main executables AND all backend DLLs using MSYS2 bash and capture output
$lddOutput = & $bashExe -lc "export PATH=/ucrt64/bin:/c/Users/Admin/Documents/proj/linapp/papers/build/libdocument:/c/Users/Admin/Documents/proj/linapp/papers/build/libview:`$PATH && ldd /c/Users/Admin/Documents/proj/linapp/papers/build/shell/src/papers.exe /c/Users/Admin/Documents/proj/linapp/papers/build/thumbnailer/release/papers-thumbnailer.exe /c/Users/Admin/Documents/proj/linapp/papers/build/previewer/papers-previewer.exe /c/Users/Admin/Documents/proj/linapp/papers/build/libdocument/backend/*.dll"

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
Copy-Item -Path (Join-Path $ucrtShare "icons\Adwaita") -Destination $iconsDir -Recurse -Container -Force
Copy-Item -Path (Join-Path $ucrtShare "icons\hicolor") -Destination $iconsDir -Recurse -Container -Force
Copy-Item -Path (Join-Path $ucrtShare "mime") -Destination (Join-Path $distDir "share") -Recurse -Container -Force
Copy-Item -Path (Join-Path $ucrtLib "gdk-pixbuf-2.0\*") -Destination $pixbufDir -Recurse -Container -Force

Write-Host ""
Write-Host "==========================================================" -ForegroundColor Green
Write-Host "   Bundling complete! Staged application is in:" -ForegroundColor Green
Write-Host "   $distDir" -ForegroundColor Yellow
Write-Host "==========================================================" -ForegroundColor Green
