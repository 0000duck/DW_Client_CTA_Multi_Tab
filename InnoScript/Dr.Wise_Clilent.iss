; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Dr. Wise_Client"
#define MyAppVersion "1.0.0.2"
#define MyAppPublisher "Deepwise, Inc."
#define MyAppURL "http://www.deepwise.com/"
#define MyAppExeName "BrowserApp.exe"
#define MyDateTimeString GetDateTimeString('yyyymmdd_hhmmss', '', '')
#define AppDesc GetStringFileInfo("..\bin\release_69_3497\BrowserApp.exe", PRODUCT_VERSION)

#define AppSourcePath49 "..\bin\release"
#define AppSourcePath69 "..\bin\release_69_3497"

//系统                           版本号                   其它条件
//-------------------------------------------------------------------------------------------------------
//Windows 10                      10 0
//Windows 8.1                      6 3
//Windows 8                        6 2
//Windows 7                        6 1                 OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
//Windows Server 2008 R2           6 1                 OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
//Windows Server 2008              6 0                 OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
//Windows Vista                    6 0                 OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
//Windows Server 2003 R2           5 2                 GetSystemMetrics(SM_SERVERR2) != 0
//Windows Home Server              5 2                 OSVERSIONINFOEX.wSuiteMask == VER_SUITE_WH_SERVER
//Windows Server 2003              5 2                 GetSystemMetrics(SM_SERVERR2) == 0
//Windows XP x64 Edition           5 2                 (OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION) && (SYSTEM_INFO.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
//Windows XP                       5 1
//Windows 2000                     5 0
//Windows Me                       4.9
//Windows 98                       4.1
//Windows 95                       4.0


// TWindowsVersion.wProductType 类型说明
//代码                            值                             说明
//---------------------------------------------------------------------------------------------------------------------------------------
//VER_NT_DOMAIN_CONTROLLER     0x0000002               装的是个域服务器系统（win2000server,2003server,2008server）
//VER_NT_SERVER                0x0000003               装的是服务器系统（win2000server,2003server,2008server）
//VER_NT_WORKSTATION           0x0000001               非服务器版本（Vista, XP Professional, XP Home Edition, 2000）


[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{DE4A9C7B-8B91-4407-9AF1-90E943A53545}
AppName={#MyAppName}
AppVersion={#AppDesc}
;AppVerName={#MyAppName} {#AppDesc}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
OutputBaseFilename={#MyAppName}_{#MyDateTimeString}_{#AppDesc}
SetupIconFile=..\resource\images\logo256x256.ico
Compression=lzma
OutputDir=..\Output2
SolidCompression=yes

Uninstallable=yes
UninstallDisplayName={#MyAppName} {#AppDesc}
UninstallDisplayIcon=..\resource\images\logo256x256.ico

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Languages\ChineseSimp.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "startupicon"; Description: "开机启动"; GroupDescription: "{cm:AdditionalIcons}"; 

[Types]
Name: "low_core_type";  Description: "比较适合Windows XP操作系统"
Name: "high_core_type"; Description: "比较适合Win7和Win10操作系统"

[Components]
Name: "low_core_components"; Description: "XP所需"; Types: low_core_type; Flags: exclusive
Name: "high_core_components"; Description: "Win7和Win10所需"; Types: high_core_type; Flags: exclusive

[Files]
; 69内核 依赖文件
Source: "{#AppSourcePath69}\BrowserApp.exe"; DestDir: "{app}"; Flags: ignoreversion  ; Components: high_core_components
Source: "{#AppSourcePath69}\cef.pak"; DestDir: "{app}"; Flags: ignoreversion     ; Components: high_core_components
Source: "{#AppSourcePath69}\cef_100_percent.pak"; DestDir: "{app}"; Flags: ignoreversion ; Components: high_core_components
Source: "{#AppSourcePath69}\cef_200_percent.pak"; DestDir: "{app}"; Flags: ignoreversion  ; Components: high_core_components
Source: "{#AppSourcePath69}\cef_extensions.pak"; DestDir: "{app}"; Flags: ignoreversion  ; Components: high_core_components
Source: "{#AppSourcePath69}\chrome_elf.dll"; DestDir: "{app}"; Flags: ignoreversion ; Components: high_core_components
Source: "{#AppSourcePath69}\config.ini"; DestDir: "{app}"; Flags: ignoreversion ; Components: high_core_components
Source: "{#AppSourcePath69}\d3dcompiler_43.dll"; DestDir: "{app}"; Flags: ignoreversion ; Components: high_core_components
Source: "{#AppSourcePath69}\d3dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion  ; Components: high_core_components
Source: "{#AppSourcePath69}\icudtl.dat"; DestDir: "{app}"; Flags: ignoreversion   ; Components: high_core_components
Source: "{#AppSourcePath69}\libcef.dll"; DestDir: "{app}"; Flags: ignoreversion   ; Components: high_core_components
Source: "{#AppSourcePath69}\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion   ; Components: high_core_components
Source: "{#AppSourcePath69}\libGLESv2.dll"; DestDir: "{app}"; Flags: ignoreversion ; Components: high_core_components
Source: "{#AppSourcePath69}\natives_blob.bin"; DestDir: "{app}"; Flags: ignoreversion  ; Components: high_core_components
Source: "{#AppSourcePath69}\protocol.reg"; DestDir: "{app}"; Flags: ignoreversion   ; Components: high_core_components
Source: "{#AppSourcePath69}\snapshot_blob.bin"; DestDir: "{app}"; Flags: ignoreversion   ; Components: high_core_components
Source: "{#AppSourcePath69}\v8_context_snapshot.bin"; DestDir: "{app}"; Flags: ignoreversion   ; Components: high_core_components
Source: "{#AppSourcePath69}\widevinecdmadapter.dll"; DestDir: "{app}"; Flags: ignoreversion  ; Components: high_core_components
Source: "{#AppSourcePath69}\locales\*"; DestDir: "{app}\locales\"; Flags: ignoreversion recursesubdirs createallsubdirs   ; Components: high_core_components
Source: "{#AppSourcePath69}\ISTask.dll"; DestDir: "{app}"; Flags: ignoreversion    ; Components: high_core_components
Source: "{#AppSourcePath69}\swiftshader\*"; DestDir: "{app}\swiftshader\"; Flags: ignoreversion recursesubdirs createallsubdirs   ; Components: high_core_components

; 49内核 依赖文件
Source: "{#AppSourcePath49}\BrowserApp.exe"; DestDir: "{app}"; Flags: ignoreversion  ; Components: low_core_components
Source: "{#AppSourcePath49}\cef.pak"; DestDir: "{app}"; Flags: ignoreversion     ; Components: low_core_components
Source: "{#AppSourcePath49}\cef_100_percent.pak"; DestDir: "{app}"; Flags: ignoreversion ; Components: low_core_components
Source: "{#AppSourcePath49}\cef_200_percent.pak"; DestDir: "{app}"; Flags: ignoreversion  ; Components: low_core_components
Source: "{#AppSourcePath49}\cef_extensions.pak"; DestDir: "{app}"; Flags: ignoreversion  ; Components: low_core_components
Source: "{#AppSourcePath49}\config.ini"; DestDir: "{app}"; Flags: ignoreversion ; Components: low_core_components
Source: "{#AppSourcePath49}\d3dcompiler_43.dll"; DestDir: "{app}"; Flags: ignoreversion ; Components: low_core_components
Source: "{#AppSourcePath49}\d3dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion  ; Components: low_core_components
Source: "{#AppSourcePath49}\icudtl.dat"; DestDir: "{app}"; Flags: ignoreversion   ; Components: low_core_components
Source: "{#AppSourcePath49}\libcef.dll"; DestDir: "{app}"; Flags: ignoreversion   ; Components: low_core_components
Source: "{#AppSourcePath49}\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion   ; Components: low_core_components
Source: "{#AppSourcePath49}\libGLESv2.dll"; DestDir: "{app}"; Flags: ignoreversion ; Components: low_core_components
Source: "{#AppSourcePath49}\natives_blob.bin"; DestDir: "{app}"; Flags: ignoreversion  ; Components: low_core_components
Source: "{#AppSourcePath49}\protocol.reg"; DestDir: "{app}"; Flags: ignoreversion   ; Components: low_core_components
Source: "{#AppSourcePath49}\snapshot_blob.bin"; DestDir: "{app}"; Flags: ignoreversion   ; Components: low_core_components
Source: "{#AppSourcePath49}\v8_context_snapshot.bin"; DestDir: "{app}"; Flags: ignoreversion   ; Components: low_core_components
Source: "{#AppSourcePath49}\widevinecdmadapter.dll"; DestDir: "{app}"; Flags: ignoreversion  ; Components: low_core_components
Source: "{#AppSourcePath49}\locales\*"; DestDir: "{app}\locales\"; Flags: ignoreversion recursesubdirs createallsubdirs   ; Components: low_core_components
Source: "{#AppSourcePath49}\ISTask.dll"; DestDir: "{app}"; Flags: ignoreversion    ; Components: low_core_components

; 共有的资源文件
Source: "{#AppSourcePath49}\install\*"; DestDir: "{app}\install\"; Flags: ignoreversion recursesubdirs createallsubdirs  ; Components: low_core_components high_core_components
Source: "{#AppSourcePath49}\resources\*"; DestDir: "{app}\resources\"; Flags: ignoreversion recursesubdirs createallsubdirs  ; Components: low_core_components high_core_components
Source: "{#AppSourcePath49}\Skin\*"; DestDir: "{app}\Skin\"; Flags: ignoreversion recursesubdirs createallsubdirs   ; Components: low_core_components high_core_components

;; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userstartup}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Registry] 
Root: HKCR; Subkey: "dpclient"; ValueType: string; ValueName: ""; ValueData: "URL:dpclient Protocol Handler"; Flags:uninsdeletekey
Root: HKCR; Subkey: "dpclient"; ValueType: string; ValueName: "URL Protocol"; ValueData: ""; Flags:uninsdeletekey
Root: HKCR; Subkey: "dpclient"; ValueType: string; ValueName: "DP Version"; ValueData: "{#AppDesc}"; Flags:uninsdeletekey
Root: HKCR; Subkey: "dpclient\shell\open\command"; ValueType: string; ValueName: ""; ValueData: "{app}\BrowserApp.exe -risurl=%1"; Flags:uninsdeletekey
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "Dr. Wise Client"; ValueData: "{app}\{#MyAppExeName}"; Flags:uninsdeletevalue


[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent



[code]
var
Page: TWizardPage;

// 控件y方向间隔
interval_distance: Integer;
// 控件高度
control_height: Integer;

RadioButton1, RadioButton2: TRadioButton;
Lbl1, Lbl2, LbFolder, Lbl3: TNewStaticText;
FolderTreeView: TFolderTreeView;
FormButton, AboutButton : TNewButton;
pathEdit: TNewEdit;
components: TComponent;

window_version: Integer;
Version: TWindowsVersion;

procedure AboutButtonOnClick(Sender: TObject); 
  begin 
	MsgBox('当前客户端版本号为 ' + ' {#AppDesc}', mbInformation, MB_OK);//显示对话框 
  end; 

//安装前判断是否有进程正在运行，istask.dll文件放到innosetup安装目录
function RunTask(FileName: string; bFullpath: Boolean): Boolean;
external 'RunTask@files:ISTask.dll stdcall delayload';
function KillTask(ExeFileName: string): Integer;
external 'KillTask@files:ISTask.dll stdcall delayload';

function InitializeSetup(): Boolean;
begin
  Result:= true;
  if RunTask('{#MyAppExeName}', false) then
      begin
        if MsgBox('软件正在运行中，是否关闭软件并安装！', mbConfirmation, MB_YESNO) = IDYES then
          begin
            KillTask('{#MyAppExeName}');
            Result:= true; 
          end
        else
          begin
            Result :=false; //安装程序退出  
          end           
      end
  else
  Result:= true;
end;

// 卸载前判断进程是否在运行，istask.dll文件与打包的exe文件一起
function RunTaskU(FileName: string; bFullpath: Boolean): Boolean;
external 'RunTask@{app}/ISTask.dll stdcall delayload uninstallonly';
function KillTaskU(ExeFileName: string): Integer;
external 'KillTask@{app}/ISTask.dll stdcall delayload uninstallonly';

function InitializeUninstall(): Boolean;
  begin
    Result:= true;
    if RunTaskU('{#MyAppExeName}', false) then
      begin
        if MsgBox('软件正在运行中，是否关闭软件并卸载！', mbConfirmation, MB_YESNO) = IDYES then
          begin
            KillTaskU('{#MyAppExeName}');
            Result:= true;
          end
        else
          begin
            Result :=false; //安装程序退出  
          end
      end
    else
    Result:= true;
    UnloadDll(ExpandConstant('{app}/ISTask.dll'));
end;

procedure ButtonOnClick1(Sender: TObject);
begin
  WizardForm.TypesCombo.ItemIndex := 0;
  WizardForm.ComponentsList.Checked[0] := True;
  WizardForm.ComponentsList.Checked[1] := False;
end;

procedure ButtonOnClick2(Sender: TObject);
begin
  WizardForm.TypesCombo.ItemIndex := 1;
  //MsgBox(WizardForm.Components[2].Name, mbConfirmation, MB_YESNO)
  WizardForm.ComponentsList.Checked[0] := False;
  WizardForm.ComponentsList.Checked[1] := True;
end;

procedure btn_Browserclick(Sender: TObject);
begin
  WizardForm.DirBrowseButton.OnClick(WizardForm);
  pathEdit.text := WizardForm.DirEdit.text;
end;


procedure DirEditChange(Sender: TObject);
begin
  WizardForm.DirEdit.text := pathEdit.text; 
end;

procedure CreateAddonPage;
  begin
    Page := CreateCustomPage(wpInfoBefore, '选择安装浏览器内核版本', '请根据您的需要选择安装的内核版本');

    interval_distance := 15;
    control_height := 20;

    RadioButton1 := TRadioButton.Create(Page);
    RadioButton1.Left := ScaleX(80);
    RadioButton1.Top := ScaleY(5);
    RadioButton1.Width := Page.SurfaceWidth;
    RadioButton1.Height := ScaleY(control_height);
    RadioButton1.Caption := '低版本';
    //RadioButton1.Checked := True;
    RadioButton1.Parent := Page.Surface;
    RadioButton1.OnClick := @ButtonOnClick1;

    Lbl1 := TNewStaticText.Create(Page);
    Lbl1.Left := ScaleX(95);
    Lbl1.Top := RadioButton1.Top + ScaleY(interval_distance) + ScaleY(control_height);
    Lbl1.Width := ScaleX(250);
    Lbl1.Height := ScaleY(control_height);
    Lbl1.Caption := '比较适合Windows XP操作系统';
    Lbl1.Parent := Page.Surface;

    RadioButton2 := TRadioButton.Create(Page);
    RadioButton2.Left := ScaleX(80);
    RadioButton2.Top := Lbl1.Top + ScaleY(interval_distance) + ScaleY(control_height);
    RadioButton2.Width := Page.SurfaceWidth;
    RadioButton2.Height := ScaleY(control_height);
    RadioButton2.Caption := '高版本';
    //RadioButton2.Checked := false;
    RadioButton2.Parent := Page.Surface;
    RadioButton2.OnClick := @ButtonOnClick2;

    Lbl2 := TNewStaticText.Create(Page);
    Lbl2.Left := ScaleX(95);
    Lbl2.Top := RadioButton2.Top + ScaleY(interval_distance) + ScaleY(control_height);
    Lbl2.Width := ScaleX(250);
    Lbl2.Height := ScaleY(control_height);
    Lbl2.Caption := '比较适合Win7和Win10操作系统';
    Lbl2.Parent := Page.Surface;

    pathEdit := TNewEdit.Create(Page);
    pathEdit.Left := ScaleX(80);
    pathEdit.Top := Lbl2.Top + ScaleY(interval_distance) + ScaleY(control_height);
    pathEdit.Width := Page.SurfaceWidth div 2  + ScaleX(50);
    pathEdit.Height := ScaleY(control_height);
    pathEdit.Text :=WizardForm.DirEdit.text;
    pathEdit.Parent := Page.Surface;
    pathEdit.OnChange := @DirEditChange;

    FormButton := TNewButton.Create(Page);
    FormButton.Left := pathEdit.Left + pathEdit.Width + ScaleX(5);
    FormButton.Top := pathEdit.Top;
    FormButton.Width := Page.SurfaceWidth - FormButton.Left;
    FormButton.Height := ScaleY(control_height);
    FormButton.Caption := '浏览';
    FormButton.OnClick := @btn_Browserclick;
    FormButton.Parent := Page.Surface;
 
    // About 按钮
    AboutButton := TNewButton.Create(WizardForm);
    AboutButton.Width := ScaleX(80);;
    AboutButton.Height := ScaleY(control_height);
    AboutButton.Left := ScaleX(30);
    AboutButton.Top := WizardForm.ClientHeight - ScaleY(12) - AboutButton.Height;
    AboutButton.Caption := '关于';
    AboutButton.OnClick := @AboutButtonOnClick;
    AboutButton.Parent := WizardForm;

    // 获取Window操作系统的版本号
    GetWindowsVersionEx(Version); 
    window_version := 1;
    // win7 和win10
    if Version.NTPlatform then
      begin
        if (Version.Major >= 10) and (Version.Minor >= 0) then       //win10
          begin
          window_version := 2;
          end
        else if (Version.Major =   6) and (Version.Minor = 3) then   //win 8.1
          begin
          window_version := 2;
          end
        else if (Version.Major =   6) and (Version.Minor = 2) then   //win 8
          begin
          window_version := 2;
          end
        else if (Version.Major =   6) and (Version.Minor = 1) and (Version.ProductType = VER_NT_WORKSTATION) then   // win 7
          begin
          window_version := 2;
          end
      end;

     
    if window_version = 1 then
      begin
        // xp
        WizardForm.TypesCombo.ItemIndex := 0;
        WizardForm.ComponentsList.Checked[0] := True;
        WizardForm.ComponentsList.Checked[1] := False;
        RadioButton1.Checked := True;
        RadioButton2.Checked := False;
        RadioButton2.Enabled := False;
      end
    else if window_version = 2 then
      begin
        // win7 win10
        WizardForm.TypesCombo.ItemIndex := 1;
        WizardForm.ComponentsList.Checked[0] := False;
        WizardForm.ComponentsList.Checked[1] := True;
        RadioButton1.Checked := False;
        RadioButton2.Checked := True;
      end;

    
  end;

procedure InitializeWizard();
begin
  CreateAddonPage;
end;


function ShouldSkipPage(PageID: Integer): Boolean;
  begin
    // 跳过欢迎页
    if PageID = wpWelcome then
    result := true;
    if (PageID= wpSelectTasks) then
    result := true;
    //if (PageID= wpReady) then
    //result := true;
    if (PageID= wpSelectDir) then
    result := true;
    if (PageID= wpSelectComponents) then
    result := true;
  end;
