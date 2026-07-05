# ![papers-logo] Document Viewer

Papers is a document viewer capable of displaying multiple and single
page document formats like PDF and DejaVu.  For more general
information about Papers and how to get started, please visit
[https://welcome.gnome.org/app/Papers](https://welcome.gnome.org/app/Papers)

## Windows Native Port

GNOME Papers is now compatible with Windows! For setup, build instructions, and architectural details specific to Windows, please refer to:
* **[README.windows.md](README.windows.md)**

## Installation

Papers is licensed under the [GPLv2][license], get it on Flathub!

[![flatpak]](https://flathub.org/apps/details/org.gnome.Papers)

## Reporting and Development

If you experience issues with Papers, check out the [reporting tips](TESTING.md).
Developers should make sure to read the [contributing](CONTRIBUTING.md)
guidelines, before starting to work on any changes.

### Papers Requirements

* [GNOME Platform libraries][gnome]
* [Poppler for PDF viewing][poppler]

### Papers Optional Backend Libraries

* [DjVuLibre for DjVu viewing][djvulibre]
* [Archive library for Comic Book Resources (CBR) viewing][comics]
* [LibTiff for Multipage TIFF viewing][tiff]

[gnome]: https://www.gnome.org/
[poppler]: https://poppler.freedesktop.org/
[djvulibre]: https://djvu.sourceforge.net/
[comics]: https://libarchive.org/
[tiff]: https://libtiff.gitlab.io/libtiff/
[license]: COPYING
[papers-logo]: data/icons/scalable/apps/org.gnome.Papers.svg
[flatpak]: https://flathub.org/api/badge?svg&locale=en

## Documentation

The documentation for the libraries can be found online.

* [libppsview](https://gnome.pages.gitlab.gnome.org/papers/view/)
* [libppsdocument](https://gnome.pages.gitlab.gnome.org/papers/document/)

## Code of Conduct

When interacting with the project, the [GNOME Code Of Conduct](https://conduct.gnome.org/) applies.
