<#
.SYNOPSIS
    GNOME Papers Dependency Auto-Setup Script for Windows

.DESCRIPTION
    Automates environment setup on Windows by detecting or installing MSYS2,
    updating package databases and core runtime, and installing all required
    UCRT64 toolchain and library dependencies for building GNOME Papers.

.PARAMETER InstallDir
    Custom installation path for MSYS2. Defaults to C:\nbin (or existing installation at C:\msys64).

.PARAMETER NonInteractive
    Runs the setup non-interactively without user prompts.

.PARAMETER SkipUpdate
    Skips the full system upgrade step (pacman -Syu / pacman -Su).

.EXAMPLE
    .\build-aux\windows\auto-setup.ps1
    .\build-aux\windows\auto-setup.ps1 -NonInteractive
    .\build-aux\windows\auto-setup.ps1 -InstallDir "D:\MSYS2" -NonInteractive
#>

[CmdletBinding()]
param (
    [string]$InstallDir = "",
    [switch]$NonInteractive,
    [switch]$SkipUpdate
)

$ErrorActionPreference = "Stop"

$repoRoot = (Get-Item $PSScriptRoot).Parent.Parent.FullName

Write-Host "==========================================================" -ForegroundColor Cyan
Write-Host "        GNOME Papers Windows Dependency Setup             " -ForegroundColor Cyan
Write-Host "==========================================================" -ForegroundColor Cyan
Write-Host ""

# 1. Determine MSYS2 Installation Path
$defaultInstallDir = "C:\nbin"
$msysPath = ""
$bashExe = ""

# Search standard locations first
$standardLocations = @(
    "C:\msys64",
    "C:\nbin\msys64",
    "$env:LOCALAPPDATA\Programs\msys64"
)

if (![string]::IsNullOrWhitespace($InstallDir)) {
    $resolvedDir = [System.IO.Path]::GetFullPath($InstallDir)
    if (Test-Path (Join-Path $resolvedDir "usr\bin\bash.exe")) {
        $msysPath = $resolvedDir
    } elseif (Test-Path (Join-Path $resolvedDir "msys64\usr\bin\bash.exe")) {
        $msysPath = Join-Path $resolvedDir "msys64"
    } else {
        $msysPath = Join-Path $resolvedDir "msys64"
    }
} else {
    foreach ($loc in $standardLocations) {
        if (Test-Path (Join-Path $loc "usr\bin\bash.exe")) {
            $msysPath = $loc
            break
        }
    }
}

$needMsysInstall = $true

if (![string]::IsNullOrWhitespace($msysPath) -and (Test-Path (Join-Path $msysPath "usr\bin\bash.exe"))) {
    $bashExe = Join-Path $msysPath "usr\bin\bash.exe"
    Write-Host "Found existing MSYS2 installation at: $msysPath" -ForegroundColor Green
    
    if (-not $NonInteractive -and [string]::IsNullOrWhitespace($InstallDir)) {
        $confirm = Read-Host "Use this existing MSYS2 installation? (Y/N) [Default: Y]"
        if ([string]::IsNullOrWhitespace($confirm) -or $confirm.Trim().ToUpper() -eq "Y") {
            $needMsysInstall = $false
        }
    } else {
        $needMsysInstall = $false
    }
}

if ($needMsysInstall) {
    if ([string]::IsNullOrWhitespace($InstallDir)) {
        if ($NonInteractive) {
            $InstallDir = $defaultInstallDir
        } else {
            $userPath = Read-Host "Enter dependency installation directory [Default: $defaultInstallDir]"
            $InstallDir = if ([string]::IsNullOrWhitespace($userPath)) { $defaultInstallDir } else { $userPath }
        }
    }
    
    $installPath = [System.IO.Path]::GetFullPath($InstallDir)
    $msysPath = Join-Path $installPath "msys64"
    $bashExe = Join-Path $msysPath "usr\bin\bash.exe"

    if (!(Test-Path $installPath)) {
        New-Item -ItemType Directory -Force -Path $installPath | Out-Null
    }

    $tempInstaller = Join-Path $env:TEMP "msys2-installer.exe"
    Write-Host "Downloading MSYS2 installer..." -ForegroundColor Cyan
    $downloadUrl = "https://github.com/msys2/msys2-installer/releases/download/nightly-x86_64/msys2-x86_64-latest.exe"
    
    Invoke-WebRequest -Uri $downloadUrl -OutFile $tempInstaller
    
    Write-Host "Installing MSYS2 silently to $msysPath..." -ForegroundColor Cyan
    $installArgs = @(
        "--mode", "script",
        "--unattendedmodeui", "none",
        "default_installation_directory=$msysPath"
    )
    $process = Start-Process -FilePath $tempInstaller -ArgumentList $installArgs -Wait -NoNewWindow -PassThru
    
    if ($process.ExitCode -ne 0) {
        throw "MSYS2 installation failed with exit code $($process.ExitCode)."
    }
    
    if (Test-Path $tempInstaller) {
        Remove-Item $tempInstaller -Force
    }
    Write-Host "MSYS2 installed successfully!" -ForegroundColor Green
}

# 2. System Update & Sync
if (-not $SkipUpdate) {
    Write-Host "Updating MSYS2 package databases and system core..." -ForegroundColor Cyan
    
    # Sync databases
    $process = Start-Process -FilePath $bashExe -ArgumentList "-lc", '"pacman -Sy --noconfirm"' -Wait -NoNewWindow -PassThru
    if ($process.ExitCode -ne 0) {
        Write-Warning "pacman -Sy returned exit code $($process.ExitCode). Proceeding with package updates..."
    }

    # Full system upgrade
    $process = Start-Process -FilePath $bashExe -ArgumentList "-lc", '"pacman -Su --noconfirm"' -Wait -NoNewWindow -PassThru
    if ($process.ExitCode -ne 0) {
        Write-Warning "pacman -Su returned exit code $($process.ExitCode). Continuing setup..."
    }
}

# 3. Install Required Dependencies
Write-Host "Installing required UCRT64 toolchain and libraries..." -ForegroundColor Cyan

$packages = @(
    "mingw-w64-ucrt-x86_64-gcc",
    "mingw-w64-ucrt-x86_64-meson",
    "mingw-w64-ucrt-x86_64-ninja",
    "mingw-w64-ucrt-x86_64-rust",
    "mingw-w64-ucrt-x86_64-gtk4",
    "mingw-w64-ucrt-x86_64-libadwaita",
    "mingw-w64-ucrt-x86_64-poppler",
    "mingw-w64-ucrt-x86_64-libspelling",
    "mingw-w64-ucrt-x86_64-djvulibre",
    "mingw-w64-ucrt-x86_64-libarchive",
    "mingw-w64-ucrt-x86_64-libtiff",
    "mingw-w64-ucrt-x86_64-pkgconf",
    "mingw-w64-ucrt-x86_64-blueprint-compiler",
    "diffutils"
)

$packageString = $packages -join " "
$bashCmd = "pacman -S --noconfirm --needed $packageString"
$process = Start-Process -FilePath $bashExe -ArgumentList "-lc", "`"$bashCmd`"" -Wait -NoNewWindow -PassThru

if ($process.ExitCode -ne 0) {
    throw "Failed to install dependencies via MSYS2 pacman (exit code: $($process.ExitCode))."
}

Write-Host ""
Write-Host "==========================================================" -ForegroundColor Green
Write-Host "   Setup complete! All dependencies are installed." -ForegroundColor Green
Write-Host "   MSYS2 Path: $bashExe" -ForegroundColor Yellow
Write-Host ""
Write-Host "   To build GNOME Papers, execute:" -ForegroundColor Yellow
Write-Host "     1. export PATH=/ucrt64/bin:`$PATH" -ForegroundColor Yellow
Write-Host "     2. meson setup build --buildtype=release --strip -Ddocumentation=false -Duser_doc=false --reconfigure" -ForegroundColor Yellow
Write-Host "     3. meson compile -C build" -ForegroundColor Yellow
Write-Host "==========================================================" -ForegroundColor Green
