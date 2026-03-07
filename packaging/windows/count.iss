[Setup]
AppName=Counting Tool
AppVersion=1.0
DefaultDirName={pf}\Counting-Tool
DefaultGroupName=Counting Tool
OutputDir=build/windows
OutputBaseFilename=counting-tool-installer
Compression=lzma
SolidCompression=yes
ChangesEnvironment=yes

[Files]
Source: "..\..\build\windows\count.exe"; DestDir: "{app}"
Source: "..\..\docs\*"; DestDir: "{app}\docs"; Flags: recursesubdirs

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; \
ValueType: expandsz; ValueName: "Path"; \
ValueData: "{olddata};{app}"; Flags: preservestringtype; \
Check: NeedsAddPath('{app}')

[Code]
function NeedsAddPath(Dir: string): boolean;
var
  Path: string;
begin
  if not RegQueryStringValue(HKLM,
     'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
     'Path', Path)
  then
    Result := True
  else
    Result := Pos(';' + Dir + ';', ';' + Path + ';') = 0;
end;