[Setup]
AppName=GNOME Papers
AppVersion=50.0
DefaultDirName={autopf}\GNOME Papers
DefaultGroupName=GNOME Papers
OutputDir=build-installer
OutputBaseFilename=Papers-Setup-x64
Compression=lzma2
SolidCompression=yes
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
ChangesAssociations=yes
AppPublisher=GNOME Papers Contributors
AppPublisherURL=https://github.com/harshmishrahm01/papers-for-windows
AppCopyright=Copyright (C) 2004-2026 GNOME Papers Contributors

[Files]
Source: "dist\*"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs

[Icons]
Name: "{group}\GNOME Papers"; Filename: "{app}\bin\papers.exe"; WorkingDir: "{app}\bin"
Name: "{autodesktop}\GNOME Papers"; Filename: "{app}\bin\papers.exe"; WorkingDir: "{app}\bin"

[Registry]
; Associate .pdf
Root: HKA; Subkey: "Software\Classes\.pdf"; ValueType: string; ValueName: "Content Type"; ValueData: "application/pdf"; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\.pdf\OpenWithProgids"; ValueType: string; ValueName: "Papers.AssocFile.pdf"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.pdf"; ValueType: string; ValueName: ""; ValueData: "PDF Document"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.pdf\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\papers.exe,0"
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.pdf\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\bin\papers.exe"" ""%1"""

; Associate .djvu
Root: HKA; Subkey: "Software\Classes\.djvu"; ValueType: string; ValueName: "Content Type"; ValueData: "image/vnd.djvu"; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\.djvu\OpenWithProgids"; ValueType: string; ValueName: "Papers.AssocFile.djvu"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.djvu"; ValueType: string; ValueName: ""; ValueData: "DjVu Document"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.djvu\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\papers.exe,0"
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.djvu\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\bin\papers.exe"" ""%1"""

; Associate .tiff
Root: HKA; Subkey: "Software\Classes\.tiff"; ValueType: string; ValueName: "Content Type"; ValueData: "image/tiff"; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\.tiff\OpenWithProgids"; ValueType: string; ValueName: "Papers.AssocFile.tiff"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.tiff"; ValueType: string; ValueName: ""; ValueData: "TIFF Image"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.tiff\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\papers.exe,0"
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.tiff\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\bin\papers.exe"" ""%1"""

; Associate .cbr
Root: HKA; Subkey: "Software\Classes\.cbr"; ValueType: string; ValueName: "Content Type"; ValueData: "application/vnd.comicbook-rar"; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\.cbr\OpenWithProgids"; ValueType: string; ValueName: "Papers.AssocFile.cbr"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.cbr"; ValueType: string; ValueName: ""; ValueData: "CBR Comic Book Archive"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.cbr\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\papers.exe,0"
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.cbr\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\bin\papers.exe"" ""%1"""

; Associate .cbz
Root: HKA; Subkey: "Software\Classes\.cbz"; ValueType: string; ValueName: "Content Type"; ValueData: "application/vnd.comicbook+zip"; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\.cbz\OpenWithProgids"; ValueType: string; ValueName: "Papers.AssocFile.cbz"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.cbz"; ValueType: string; ValueName: ""; ValueData: "CBZ Comic Book Archive"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.cbz\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\papers.exe,0"
Root: HKA; Subkey: "Software\Classes\Papers.AssocFile.cbz\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\bin\papers.exe"" ""%1"""

; Default Programs capabilities
Root: HKA; Subkey: "Software\Papers\Capabilities"; ValueType: string; ValueName: "ApplicationDescription"; ValueData: "GNOME Papers Document Viewer"
Root: HKA; Subkey: "Software\Papers\Capabilities"; ValueType: string; ValueName: "ApplicationName"; ValueData: "GNOME Papers"
Root: HKA; Subkey: "Software\Papers\Capabilities\FileAssociations"; ValueType: string; ValueName: ".pdf"; ValueData: "Papers.AssocFile.pdf"
Root: HKA; Subkey: "Software\Papers\Capabilities\FileAssociations"; ValueType: string; ValueName: ".djvu"; ValueData: "Papers.AssocFile.djvu"
Root: HKA; Subkey: "Software\Papers\Capabilities\FileAssociations"; ValueType: string; ValueName: ".tiff"; ValueData: "Papers.AssocFile.tiff"
Root: HKA; Subkey: "Software\Papers\Capabilities\FileAssociations"; ValueType: string; ValueName: ".cbr"; ValueData: "Papers.AssocFile.cbr"
Root: HKA; Subkey: "Software\Papers\Capabilities\FileAssociations"; ValueType: string; ValueName: ".cbz"; ValueData: "Papers.AssocFile.cbz"
Root: HKA; Subkey: "Software\RegisteredApplications"; ValueType: string; ValueName: "Papers"; ValueData: "Software\Papers\Capabilities"; Flags: uninsdeletevalue

[Run]
Filename: "{app}\bin\papers.exe"; Description: "Launch GNOME Papers"; Flags: postinstall nowait
