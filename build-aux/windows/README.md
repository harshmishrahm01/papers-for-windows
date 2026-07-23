# Building on Windows (MSYS2 UCRT64)

This guide describes how to configure, compile, and run GNOME Papers natively on Windows using the MSYS2 UCRT64 environment.

---

## 1. Prerequisites

### Install MSYS2
1. Download and install [MSYS2](https://www.msys2.org/).
2. Run the **MSYS2 UCRT64** terminal.

### Install Required Packages
In the UCRT64 terminal, run the following command to install the required system libraries, compilers, build systems, and dependencies:

```bash
pacman -S --noconfirm \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-meson \
  mingw-w64-ucrt-x86_64-ninja \
  mingw-w64-ucrt-x86_64-rust \
  mingw-w64-ucrt-x86_64-gtk4 \
  mingw-w64-ucrt-x86_64-libadwaita \
  mingw-w64-ucrt-x86_64-poppler \
  mingw-w64-ucrt-x86_64-libspelling \
  mingw-w64-ucrt-x86_64-djvulibre \
  mingw-w64-ucrt-x86_64-libarchive \
  mingw-w64-ucrt-x86_64-libtiff \
  mingw-w64-ucrt-x86_64-pkg-config \
  diffutils
```

---

## 2. Configuration & Compilation

In your UCRT64 shell, navigate to the project directory and run the compilation commands:

```bash
# Ensure UCRT64 binaries are in the path
export PATH=/ucrt64/bin:$PATH

# Setup and configure the build directory in release mode with stripped symbols
meson setup build --buildtype=release --strip --reconfigure

# Compile all targets (C dependencies, Rust bindings, shell, and thumbnailer)
meson compile -C build
```

This will produce the compiled binaries in `build/`:
- `build/shell/src/papers.exe`
- `build/thumbnailer/release/papers-thumbnailer.exe`
- Backend DLLs in `build/libdocument/backend/`

---

## 3. Running the Application

To run the application directly from the build directory, you need to expose the local GSettings schemas, backends directory, and library dependency DLLs:

```bash
# Add DLL dependencies to PATH
export PATH=/ucrt64/bin:build/libdocument:build/libview:$PATH

# Specify GSettings schema path
export GSETTINGS_SCHEMA_DIR=build/data

# Specify Papers Document Backends path
export PPS_BACKENDS_DIR=build/libdocument/backend

# Launch the app
build/shell/src/papers.exe
```
