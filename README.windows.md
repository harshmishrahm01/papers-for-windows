# GNOME Papers Windows Port

Welcome to the Windows port of **GNOME Papers** (originally Evince), a modern document viewer capable of displaying multiple and single page document formats like PDF, Comic Book Archives, DjVu, and TIFF.

This port maintains the core design aesthetics and features of GNOME Papers while enabling compilation and execution natively on Windows systems.

---

## Documentation Index

- **Build Instructions**: Detailed instructions for setting up the environment, installing dependencies, and compiling the project are available in [build-aux/windows/README.md](file:///c:/Users/Admin/Documents/proj/linapp/papers/build-aux/windows/README.md).
- **Architectural Port Context**: Information about the Platform Abstraction Layer (PAL) and design choices made to decouple the application from Unix-specific systems is available in [docs/windows-port.md](file:///c:/Users/Admin/Documents/proj/linapp/papers/docs/windows-port.md).

---

## Quick Build & Run Checklist (MSYS2 UCRT64)

1. Open the **MSYS2 UCRT64** terminal.
2. Install dependencies:
   ```bash
   pacman -S --noconfirm mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-meson mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-rust mingw-w64-ucrt-x86_64-gtk4 mingw-w64-ucrt-x86_64-libadwaita mingw-w64-ucrt-x86_64-poppler-glib mingw-w64-ucrt-x86_64-libspelling mingw-w64-ucrt-x86_64-djvulibre mingw-w64-ucrt-x86_64-libarchive mingw-w64-ucrt-x86_64-libtiff mingw-w64-ucrt-x86_64-pkg-config diffutils
   ```
3. Compile the project:
   ```bash
   export PATH=/ucrt64/bin:$PATH
   meson setup build --reconfigure
   meson compile -C build
   ```
4. Run the application:
   ```bash
   export PATH=/ucrt64/bin:build/libdocument:build/libview:$PATH
   export GSETTINGS_SCHEMA_DIR=build/data
   export PPS_BACKENDS_DIR=build/libdocument/backend
   build/shell/src/papers.exe
   ```
