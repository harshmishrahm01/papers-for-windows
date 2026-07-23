# Project Porting Context: GNOME Papers to Windows

## 1. Project Overview
The goal is to port the GNOME Papers document viewer to Windows while preserving core features and design aesthetics. The project has successfully completed the **Abstraction and Porting Phase**.

## 2. Architectural Strategy
The porting strategy relies on a **Platform Abstraction Layer (PAL)**. All GTK/GNOME dependencies are isolated into abstract interfaces.

### The PAL Pattern:
- **Interface:** `libview/pps-platform.h` defines the abstract functions.
- **Linux Implementation:** `libview/pps-platform-gtk.c` implements these using GTK4.
- **Windows Implementation:** `libview/pps-platform-win32.c` implements these using Win32/GTK4.

## 3. Current Implementation Status

### ✅ Abstracted Components:
- **Rendering Pipeline:** `pps_platform_snapshot_*` functions now handle saves, restores, translations, and texture/color appends.
- **Input System:** `pps_platform_get_event_state`, `pps_platform_get_event_time`, and `pps_platform_gesture_set_state` decouple the viewer from GdkEvents and GtkGestures.
- **UI Metrics:** `pps_platform_get_widget_dpi` and `pps_platform_get_text_direction` are abstracted.
- **Navigation:** `pps_platform_get_adjustment_*` and `pps_platform_adjustment_set_value` handle scroll logic.
- **Widget Management:** `pps_platform_get_first_child`, `pps_platform_get_next_sibling`, and `pps_platform_unparent` abstract the widget tree.
- **Focus & Feedback:** `pps_platform_grab_focus` and `pps_platform_error_bell` are abstracted.
- **Styling:** `pps_platform_add_css_class` and `pps_platform_remove_css_class` decouple the visual identity from GTK CSS.

### 🛠️ Refactored Files:
- `libview/pps-view.c`: Core viewing logic now uses the PAL.
- `libview/pps-annotation-overlay.c`: Annotation interactions are decoupled.
- `libview/pps-annotation-layer-ink.c`: Ink drawing is decoupled.
- `libview/pps-annotation-layer-objects.c`: Object manipulation is decoupled.
- `libview/pps-annotation-window.c`: Window styling is decoupled.
- `libview/pps-view-page.c`: Page-level gestures and styling are decoupled.

## 4. Completed Next Steps
1.  **Implemented `libview/pps-platform-win32.c`**: Created the Windows implementation of the PAL.
2.  **Windows File & Process Management**: Standardized path handling and process spawning using standard library tools on Windows.
3.  **Build System Update**: Modified `meson.build` files and cargo commands to compile and link dependencies cleanly under the MSYS2 UCRT64 toolchain.
4.  **Running & Verification**: Confirmed execution success of the final binary `papers.exe` with localized GSettings and backends directory exports.
