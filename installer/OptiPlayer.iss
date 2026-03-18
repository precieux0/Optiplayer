; OptiPlayer Installer Script
#define MyAppName "OptiPlayer"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "OKITAKOY Inc."
#define MyAppURL "https://optiplayer.onrender.com"
#define MyAppExeName "OptiPlayer.exe"

[Setup]
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}/support
AppUpdatesURL={#MyAppURL}/download
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
OutputDir=.
OutputBaseFilename=OptiPlayer-Setup-{#MyAppVersion}
Compression=lzma2/ultra64
SolidCompression=yes
SetupIconFile=..\assets\icons\optiplayer.ico
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=6.2

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "Create desktop icon"; GroupDescription: "Additional icons:"
Name: "fileassociation"; Description: "Associate media files with OptiPlayer"; GroupDescription: "File associations:"

[Files]
Source: "..\build\OptiPlayer-Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\OptiPlayer-Release\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\assets\icons\*"; DestDir: "{app}\assets\icons"; Flags: ignoreversion
Source: "..\LICENSE"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\README.md"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Registry]
Root: HKA; Subkey: "Software\Classes\.mp4\OpenWithProgids"; ValueType: string; ValueName: "OptiPlayerFile"; ValueData: ""; Tasks: fileassociation
Root: HKA; Subkey: "Software\Classes\.mkv\OpenWithProgids"; ValueType: string; ValueName: "OptiPlayerFile"; ValueData: ""; Tasks: fileassociation
Root: HKA; Subkey: "Software\Classes\OptiPlayerFile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Launch OptiPlayer"; Flags: nowait postinstall skipifsilent

[Code]
function InitializeSetup(): Boolean;
begin
  Result := True;
  MsgBox('Welcome to OptiPlayer!' + #13#10 +
         'Developed with ❤️ by OKITAKOY Inc.' + #13#10 +
         'Founder: Précieux Okitakoy', mbInformation, MB_OK);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    CreateDir(ExpandConstant('{userappdata}\OptiPlayer'));
    CreateDir(ExpandConstant('{userappdata}\OptiPlayer\cache'));
  end;
end;
