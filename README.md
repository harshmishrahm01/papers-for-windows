# GNOME Papers (Windows Port)

GNOME Papers is a modern document viewer capable of displaying multiple and single-page document formats like PDF, Comic Book Archives, DjVu, and TIFF. 

This repository contains a **Windows Native Port** of the application, branched from the official upstream GNOME GitLab repository.

* **Upstream Source Repository**: [https://gitlab.gnome.org/GNOME/papers](https://gitlab.gnome.org/GNOME/papers)
* **GitHub Port Repository**: [https://github.com/harshmishrahm01/papers-for-windows](https://github.com/harshmishrahm01/papers-for-windows)

---

## 1. Abbreviations

- **PAL**: Platform Abstraction Layer
- **API**: Application Programming Interface
- **DLL**: Dynamic Link Library
- **MSYS2**: Minimal System Utility collection 2 (Windows compiler environment)
- **UCRT64**: Universal C Runtime 64-bit (MSYS2 toolchain)
- **CBR**: Comic Book Resources

---

## 2. Port Changes & Improvements

This port adapts the Linux-centric codebase of GNOME Papers to compile and run natively on Windows with the following modifications:

1. **Platform Abstraction Layer (PAL)**: Added a dynamic platform source selection mechanism. Extracted platform-dependent GUI / rendering wrapper functions in `libview/pps-platform-win32.c` to replace Unix-specific window management and coordinate mapping.
2. **Unix API Gating**:
   - Gated Unix-specific file descriptor duplicate operations (`fcntl`/`F_DUPFD_CLOEXEC`) behind `#ifndef G_OS_WIN32` blocks in the previewer, `pps-jobs.c`, and poppler loader.
   - Conditionally included `<io.h>` and `<unistd.h>` to expose POSIX `write` and `close` functions on Windows.
3. **Cross-Platform Compilation Support**:
   - Disabled Unix-only features such as D-Bus Keyring/Secret Service integration (`oo7` crate) and Nautilus file manager extensions.
   - Replaced platform-specific `cp` commands in Meson configuration scripts with a Python-based copy script to ensure compatibility under standard CMD and PowerShell environments.
4. **Shell Execution Fallback**: Replaced the Unix-only `glib::spawn_command_line_async` in the Rust shell (`shell/src/application.rs`) with a standard Rust `std::process::Command` execution path on Windows.

---

## 3. Setup & Build Instructions

### 3.1 Automated Setup (Recommended)
We provide an automated PowerShell script to set up MSYS2 and install all dependencies:

1. Open a PowerShell console as Administrator (required for writing to system directories if installing to default locations).
2. Execute the setup script in the project root:
   ```powershell
   Set-ExecutionPolicy Bypass -Scope Process
   .\build-aux\windows\auto-setup.ps1
   ```
3. The script will prompt you for an installation path (Defaulting to `C:\nbin\msys64`) and automatically install:
   - GCC compiler toolchains (`mingw-w64-ucrt-x86_64-gcc`)
   - Build systems (`meson`, `ninja`, `pkg-config`)
   - Rust development toolchain (`mingw-w64-ucrt-x86_64-rust`)
   - External library dependencies (`gtk4`, `libadwaita`, `poppler`, `libspelling`, `djvulibre`, `libarchive`, `libtiff`)
   - UNIX compatibility utilities (`diffutils`)

### 3.2 Manual Compilation
Once dependencies are configured, compile the project using MSYS2 UCRT64:

```bash
# Set PATH to UCRT64 binaries
export PATH=/ucrt64/bin:$PATH

# Setup and configure the build directory in release mode with stripped symbols
meson setup build --buildtype=release --strip --reconfigure

# Compile all shell targets
meson compile -C build
```

### 3.3 Running the Application
Launch `papers.exe` with runtime variables pointing to GSettings schemas and backend plugins:

```bash
# Add compiled dependencies and local libraries to path
export PATH=/ucrt64/bin:build/libdocument:build/libview:$PATH

# Expose compiled settings schemas
export GSETTINGS_SCHEMA_DIR=build/data

# Expose backend document readers path
export PPS_BACKENDS_DIR=build/libdocument/backend

# Launch
build/shell/src/papers.exe
```

---

## 4. Credits and References

Papers is originally branched from GNOME Evince. We thank the original authors and maintainers for their foundational work:

### Original Developers & Maintainers
- **Martin Kretzschmar** <m_kretzschmar@gmx.net>
- **Jonathan Blandford** <jrb@gnome.org>
- **Marco Pesenti Gritti** <marco@gnome.org>
- **Nickolay V. Shmyrev** <nshmyrev@yandex.ru>
- **Bryan Clark** <clarkbw@gnome.org>
- **Carlos Garcia Campos** <carlosgc@gnome.org>
- **Wouter Bolsterlee** <wbolster@gnome.org>
- **Christian Persch** <chpe@src.gnome.org>
- **Germán Poo-Caamaño** <gpoo@gnome.org>
- **Qiu Wenbo** <qiuwenbo@gnome.org>
- **Pablo Correa Gómez** <ablocorrea@hotmail.com>
- **Markus Göllnitz** (https://bewares.it/)

### Original Documenters
- **Nickolay V. Shmyrev** <nshmyrev@yandex.ru>
- **Phil Bull** <philbull@gmail.com>
- **Tiffany Antpolski** <tiffany.antopolski@gmail.com>

---

## 5. Licensing

Papers is licensed under the GNU General Public License (GPLv3). See the `COPYING` file for license details.
