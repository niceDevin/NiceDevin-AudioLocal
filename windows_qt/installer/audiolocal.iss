; NiceDevin AudioLocal - 本地音频转换器安装程序
; 仅支持 中文 / English 两种语言

[Setup]
AppId={{A1B2C3D4-1111-2222-3333-444455556666}
AppName=NiceDevin AudioLocal
AppVersion=1.0.0
AppVerName=NiceDevin AudioLocal 1.0.0
AppPublisher=NiceDevin
AppPublisherURL=https://nicedevin.top
AppSupportURL=https://nicedevin.top
DefaultDirName={autopf}\NiceDevin AudioLocal
DefaultGroupName=NiceDevin AudioLocal
DisableProgramGroupPage=yes
SetupIconFile=audiolocal.ico
WizardImageFile=wizard.bmp
WizardSmallImageFile=small.bmp
UninstallDisplayIcon={app}\audiolocal.ico
OutputDir=.
OutputBaseFilename=AudioLocalSetup
Compression=lzma2/ultra
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=admin
UsePreviousAppDir=yes
; 安装/覆盖时若主程序正在运行，也让 Inno 提示关闭（卸载由下方 [Code] 处理强制结束）
CloseApplications=yes
CloseApplicationsFilter=AudioLocalQt.exe

[Languages]
Name: "chinese"; MessagesFile: "C:\Users\wangc\Desktop\audio\windows_qt\installer\ChineseSimplified.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[CustomMessages]
chinese.ForceCloseTitle=强制结束应用
chinese.ForceClosePrompt=检测到 NiceDevin AudioLocal 正在运行。%n是否强制结束它并继续卸载？
english.ForceCloseTitle=Force close application
english.ForceClosePrompt=NiceDevin AudioLocal is currently running.%nForce terminate it and continue uninstalling?

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "audiolocal.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{autoprograms}\NiceDevin AudioLocal"; Filename: "{app}\AudioLocalQt.exe"; IconFilename: "{app}\audiolocal.ico"
Name: "{autodesktop}\NiceDevin AudioLocal"; Filename: "{app}\AudioLocalQt.exe"; IconFilename: "{app}\audiolocal.ico"; Tasks: desktopicon

[Code]
var
  SweepTrack: TPanel;
  SweepBlock: TPanel;
  IntroDone: Boolean;

// 跑一圈：高亮块从左侧扫到右侧
procedure SweepRound;
var
  trackW, blockW, steps, i: Integer;
begin
  trackW := SweepTrack.ClientWidth;
  blockW := SweepBlock.Width;
  steps := 22;
  for i := 0 to steps do
  begin
    SweepBlock.Left := -blockW + (i * (trackW + blockW)) div steps;
    SweepTrack.Repaint;
    Sleep(12);
  end;
end;

// 品牌进入动画：欢迎页打开时扫 3 圈
procedure PlayIntroSweep;
var
  trackW, trackH, r: Integer;
begin
  trackW := 240;
  trackH := 10;
  SweepTrack.Left := (WizardForm.ClientWidth - trackW) div 2;
  SweepTrack.Top := WizardForm.ClientHeight - 96;
  SweepTrack.Visible := True;
  for r := 1 to 3 do
    SweepRound;
  SweepTrack.Visible := False;
end;

procedure CreateSweep;
var
  trackW, trackH: Integer;
begin
  trackW := 240;
  trackH := 10;
  SweepTrack := TPanel.Create(WizardForm);
  SweepTrack.Parent := WizardForm;
  SweepTrack.Width := trackW;
  SweepTrack.Height := trackH;
  SweepTrack.BevelOuter := bvNone;
  SweepTrack.Color := $00ACC1C9;   // 暖灰轨道 RGB(201,193,172)
  SweepTrack.Visible := False;

  SweepBlock := TPanel.Create(SweepTrack);
  SweepBlock.Parent := SweepTrack;
  SweepBlock.BevelOuter := bvNone;
  SweepBlock.Color := $001A57C2;   // 铜橙高亮 RGB(194,87,26)
  SweepBlock.Width := trackW div 4;
  SweepBlock.Height := trackH;
  SweepBlock.Left := -SweepBlock.Width;
  SweepBlock.Top := 0;
end;

procedure InitializeWizard;
begin
  CreateSweep;
  IntroDone := False;
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if (CurPageID = wpWelcome) and (not IntroDone) then
  begin
    IntroDone := True;
    PlayIntroSweep;
  end;
end;

// ---- 卸载：检测主程序是否在运行，询问是否强制结束 ----
// tasklist 找不到进程时退出码仍为 0，必须靠 findstr 的命中退出码判断
// （findstr 找到进程名 -> 0，没找到 -> 1，与系统语言无关）。
function IsAppRunning: Boolean;
var
  RC: Integer;
begin
  Result := False;
  if Exec('cmd.exe',
          '/c tasklist /FI "IMAGENAME eq AudioLocalQt.exe" /NH | findstr /C:"AudioLocalQt.exe"',
          '', SW_HIDE, ewWaitUntilTerminated, RC) then
    Result := (RC = 0);
end;

function InitializeUninstall: Boolean;
var
  ResultCode: Integer;
begin
  Result := True;
  if IsAppRunning then
  begin
    if MsgBox(CustomMessage('ForceClosePrompt'), mbConfirmation,
              MB_YESNO or MB_DEFBUTTON2) = IDYES then
    begin
      Exec('taskkill.exe', '/IM AudioLocalQt.exe /F /T', '', SW_HIDE,
           ewWaitUntilTerminated, ResultCode);
      Sleep(300);
    end
    else
      Result := False;   // 用户选择否：取消卸载
  end;
end;
