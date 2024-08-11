;Setup for ZBridge instal1.

[Setup]
AppPublisher=EAK
AppName=ZBridge
AppVersion=0.9
WizardStyle=modern
DefaultDirName={autopf}\ZBridge
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\ZBridge.exe
Compression=lzma2
SolidCompression=yes
OutputBaseFilename=ZBridge
;OutputDir=userdocs:Inno Setup Examples Output

[Files]
Source: "*"; Excludes: "zbridge.iss"; DestDir: "{app}"
Source: ".zbridge\*"; DestDir: "{app}\.zbridge\"
Source: "iconengines\*"; DestDir: "{app}\iconengines\"
Source: "imageformats\*"; DestDir: "{app}\imageformats\"
Source: "networkinformation\*"; DestDir: "{app}\networkinformation\"
Source: "platforms\*"; DestDir: "{app}\platforms\"
Source: "sqldrivers\*"; DestDir: "{app}\sqldrivers\"
Source: "styles\*"; DestDir: "{app}\styles\"
Source: "tls\*"; DestDir: "{app}\tls\"
Source: "translations\*"; DestDir: "{app}\translations\"
Source: "generic\*"; DestDir: "{app}\generic\"


[Icons]
;Name: "{autoprograms}\My Program"; Filename: "{app}\ZBridge.exe"
Name: "{autodesktop}\ZBridge"; Filename: "{app}\ZBridge.exe"
