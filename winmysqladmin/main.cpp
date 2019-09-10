/* Copyright (C) 2000 MySQL AB & MySQL Finland AB

   This software is distributed with NO WARRANTY OF ANY KIND.  No author or
   distributor accepts any responsibility for the consequences of using it, or
   for whether it serves any particular purpose or works at all, unless he or
   she says so in writing.  Refer to the LICENSE file for details.

   Every copy of this file must include a copy of the License, normally in a
   plain ASCII text file named LICENSE.  The License grants you the right to
   copy, modify and redistribute this file, but only under certain conditions
   described in the License.  Among other things, the License requires that
   the copyright notice and this notice be preserved on all copies */

 /* Written by G. Miguel A. Solórzano */


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "initsetup.h"
#include "db.h"
#include "qtime.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#include <shellapi.h>
#include <registry.hpp>
#include <winsvc.h>
#include <winsock.h>
#include <shlobj.h>
#include <IniFiles.hpp>
#include <dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque.h>
#include <vector.h>
#include <fstream.h>
#include <iostream.h>
#include <iterator.h>
#include <sstream.h>
#include "mysql.h"
#include <Printers.hpp>
#include <Filectrl.hpp>

TForm1 *Form1;
bool b_start, b_NT, b_force = false, b_variables = false;
bool b_process  = false, b_DB = false, b_new_line = false;
bool b_extended_server_status = true, b_refresh_status = true, b_grid_init = false;
bool b_preport = false, b_treport = false, b_ereport = false;
bool b_MySQLNode = false;
AnsiString vpath, vip, mainroot;
static unsigned long questions = 0;
MYSQL_RES *res_1;
MYSQL *MySQL;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
  b_start = true;
  b_connect = false;
  QtimeE = 0;
  if (ParamCount() > 0)
  {
   if (ParamStr(1) == "-h" || ParamStr(1) == "h" )
   {
     show_usage();
     Application->Terminate();
   }
   else if (ParamStr(1) == "-w" || ParamStr(1) == "w")
   {
     b_start = false;
     init_tool_start();
   }
  }
  else
  {
    Memo6->Clear();
    init_tool_start();
    Hide();
    server_options();
    set_query_interval();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawItem(TMessage& Msg)
{
   IconDrawItem((LPDRAWITEMSTRUCT)Msg.LParam);
   TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MyNotify(TMessage& Msg)
{
 POINT MousePos;

 switch(Msg.LParam) {
  case WM_RBUTTONUP:
    if (GetCursorPos(&MousePos))
    {
      PopupMenu1->PopupComponent = Form1;
      SetForegroundWindow(Handle);
      PopupMenu1->Popup(MousePos.x, MousePos.y);
    }
    else
     Show();
    break;
  case WM_LBUTTONUP:
    if (GetCursorPos(&MousePos))
    {
      PopupMenu1->PopupComponent = Form1;
      SetForegroundWindow(Handle);
      PopupMenu1->Popup(MousePos.x, MousePos.y);
    }
    ToggleState();
    break;
  default:
     break; }

 TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::TrayMessage(DWORD dwMessage)
{
 NOTIFYICONDATA tnd;
 PSTR pszTip;

 pszTip = TipText();

 tnd.cbSize             = sizeof(NOTIFYICONDATA);
 tnd.hWnd               = Handle;
 tnd.uID                = IDC_MYICON;
 tnd.uFlags             = NIF_MESSAGE | NIF_ICON | NIF_TIP;
 tnd.uCallbackMessage	= MYWM_NOTIFY;

 if (dwMessage == NIM_MODIFY)
 {
   tnd.hIcon	= IconHandle();
   if (pszTip)
    lstrcpyn(tnd.szTip, pszTip, sizeof(tnd.szTip));
   else
    tnd.szTip[0] = '\0';
 }
 else
 {
  tnd.hIcon = NULL;
  tnd.szTip[0] = '\0';
 }
 return (Shell_NotifyIcon(dwMessage, &tnd));
}
//---------------------------------------------------------------------------
HANDLE __fastcall TForm1::IconHandle(void)
{
 if (!b_NT)
 {
   if (is_running())
   {
     Image3->Visible = false;
     Image4->Visible = false;
     Image2->Visible = true;
     return (Image2->Picture->Icon->Handle);
   }
   else
   {
     Image2->Visible = false;
     Image4->Visible = false;
     Image3->Visible = true;
     return (Image3->Picture->Icon->Handle);
   }
 }
 else
 {
   if (service_is_running())
   {
     Image3->Visible = false;
     Image4->Visible = false;
     Image2->Visible = true;
     return (Image2->Picture->Icon->Handle);
   }
   if (service_is_stopping())
   {
     Image3->Visible = false;
     Image4->Visible = true;
     Image2->Visible = false;
     return (Image4->Picture->Icon->Handle);
   }
   else if (is_running())
   {
     Image3->Visible = false;
     Image4->Visible = false;
     Image2->Visible = true;
     return (Image2->Picture->Icon->Handle);
   }
   else
   {
     Image2->Visible = false;
     Image4->Visible = false;
     Image3->Visible = true;
     return (Image3->Picture->Icon->Handle);
   }
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ToggleState(void)
{

 TrayMessage(NIM_MODIFY);
 if (!b_NT){
   if (is_running()){SSW9->Caption = "Shut down the Server";
                     Image3->Visible = false; Image4->Visible = false;Image2->Visible = true; }
   else {            SSW9->Caption = "Start the Server";
                     Image2->Visible = false; Image4->Visible = false;Image3->Visible = true; }
 }
 else {
   if (service_exist()) {
      Standa->Enabled = false;
      if (service_is_stopping()) {RService->Enabled = false;
                     StopS->Enabled = false;
                     StopS->Caption = "Wait: Service Stop Pending";
                     Image3->Visible = false; Image4->Visible = true;
                     Image2->Visible = false; }
      else if (service_is_running()) {RService->Enabled = false;
                     StopS->Enabled = true;
                     StopS->Caption = "Stop the Service";
                     Image3->Visible = false; Image4->Visible = false;
                     Image2->Visible = true; }
      else if (is_running()) {
                     Standa->Enabled = true;
                     StopS->Enabled = false;
                     RService->Enabled = false;
                     Standa->Caption = "Shut down the Server Standalone";
                     Image3->Visible = false; Image4->Visible = false;
                     Image2->Visible = true; }
      else {         RService->Enabled = true;
                     StopS->Enabled = true;
                     RService->Caption = "Remove the Service";
                     StopS->Caption = "Start the Service";
                     Image2->Visible = false; Image4->Visible = false;
                     Image3->Visible = true; }
                          }
    else {
       Standa->Enabled = true;
       StopS->Enabled = false;
       if (is_running()) {
                     RService->Enabled = false;
                     Standa->Caption = "Shut down the Server Standalone";
                     Image3->Visible = false; Image4->Visible = false;
                     Image2->Visible = true; }

       else {
                     RService->Enabled = true;
                     RService->Caption = "Install the Service";
                     Standa->Caption = "Start the Server Standalone";
                     Image2->Visible = false; Image4->Visible = false;
                     Image3->Visible = true; }

     }
   }

}
//---------------------------------------------------------------------------
PSTR __fastcall TForm1::TipText(void)
{
  char* status = StatusLine->SimpleText.c_str();
  return status;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::WMQueryEndSession(TWMQueryEndSession &msg)
{
  if (!b_NT) {

    if (is_running()){
                        StatusLine->SimpleText = "Shut down in progress.....";
                        Show(); standalone_shutdown();  msg.Result = 1; }
    else{
                        StatusLine->SimpleText = "The Server already is down......";
                        Show();  msg.Result = 1;  Close(); }
  }

  else  {

    if(!service_exist()) {
      if (is_running()) { standalone_shutdown();
        msg.Result = 1;
        Close();
      }
    }
    else {
     TrayMessage(NIM_DELETE);
     msg.Result = 1L; Application->Terminate(); //Close();
  }

 }
}

//---------------------------------------------------------------------------
LRESULT IconDrawItem(LPDRAWITEMSTRUCT lpdi)
{
   HICON hIcon;

   hIcon = (HICON)LoadImage(g_hinst, MAKEINTRESOURCE(lpdi->CtlID), IMAGE_ICON,
		16, 16, 0);
   if (!hIcon)
    return(false);

   DrawIconEx(lpdi->hDC, lpdi->rcItem.left, lpdi->rcItem.top, hIcon,
		16, 16, 0, NULL, DI_NORMAL);

   return(true);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TForm1::computer_name()
{
  AnsiString s_name;
  DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
  char szBuf[MAX_COMPUTERNAME_LENGTH + 1];
  szBuf[0] = '\0';

  GetComputerName(szBuf, &dwSize);
  s_name= (AnsiString) szBuf;
  delete [] szBuf;
  return s_name;

}
//---------------------------------------------------------------------------
AnsiString __fastcall TForm1::os_version()
{
  AnsiString OS, UPD;
  OSVERSIONINFO info;
  info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&info);

  switch (info.dwPlatformId)   {
    case VER_PLATFORM_WIN32s:
      b_NT = false;
      OS = "Win32s detected";
      break;
    case VER_PLATFORM_WIN32_WINDOWS:
      b_NT = false;
      if (info.dwMinorVersion == 0)
         OS = "Windows 95";
        else if (info.dwMinorVersion == 10)
          OS = "Windows 98";
        else if (info.dwMinorVersion == 90)
          OS = "Windows ME";
        break;
      case VER_PLATFORM_WIN32_NT:
        b_NT = true;
        if (info.dwMajorVersion == 4)
         OS = "Windows NT 4.0";
        else if (info.dwMajorVersion == 5)
        {
          if (info.dwMinorVersion == 0)
          {
            OS = "Windows 2000";
            UPD = info.szCSDVersion;
            if (!UPD.IsEmpty())
            {
              OS += " ";
              OS += info.szCSDVersion ;
            }
           }
          else if (info.dwMinorVersion == 1)
           OS = "Whistler";
         }
        break;
     }

     return OS;
}
///---------------------------------------------------------------------------
AnsiString __fastcall TForm1::user_name()
{
  AnsiString ret_string;
  DWORD dwSize = 0;

  GetUserName(NULL, &dwSize);

  char *szBuf = new char[dwSize];
  szBuf[0] = '\0';

  GetUserName(szBuf, &dwSize);
  ret_string = (AnsiString) szBuf;
  delete [] szBuf;
  return ret_string;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::computer_ip(void)
{
  WORD wVersionRequested;
  WSADATA WSAData;
  wVersionRequested = MAKEWORD(1,1);
  WSAStartup(wVersionRequested,&WSAData);

  hostent *P;
  char s[128];
  in_addr in;
  char *P2;
  gethostname(s, 128);
  P = gethostbyname(s);

  Memo2->Lines->Clear();
  Memo2->Lines->Add((AnsiString)P->h_name);
  mainroot = P->h_name;
  in.S_un.S_un_b.s_b1 = P->h_addr_list[0][0];
  in.S_un.S_un_b.s_b2 = P->h_addr_list[0][1];
  in.S_un.S_un_b.s_b3 = P->h_addr_list[0][2];
  in.S_un.S_un_b.s_b4 = P->h_addr_list[0][3];
  P2 = inet_ntoa(in);
  vip = P2;
  mainroot += " ( " + (AnsiString)P2 + " )";
  Memo2->Lines->Add(P2);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::computer_memory(void)
{
  MEMORYSTATUS ms;
  ms.dwLength = sizeof(MEMORYSTATUS);
  GlobalMemoryStatus(&ms);
  long int v_mem =   (ms.dwTotalPhys / 1024.0) ;
  Edit2->Text = AnsiString( v_mem) + " KB RAM";
}

//---------------------------------------------------------------------------
void __fastcall TForm1::show_usage(void)
{
   Application->MessageBox("Usage: WinMySQLadmin.EXE [OPTIONS]\n\n-w    Run the tool without start the Server.\n-h        Shows this message and exit ", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::init_tool_start(void)
{
 OS->Text = os_version();
 Localhost->Text = computer_name();
 Localuser->Text = user_name();
 computer_memory();
 clear_box();
 computer_ip();
 is_myodbc();
 is_myini();

 if (!b_NT) { WinNT->Enabled = false; NtVer->Enabled = false;
            Win9->Enabled  = true; NtVerMax->Enabled = false; }
 else     { WinNT->Enabled = true;   Win9->Enabled  = false;   }

 check_start();
 if (b_start)
 {
   if ((!b_NT) && (!is_running()))
    standalone_start();
   else if (b_NT)
    NT_force();

   TrayMessage(NIM_MODIFY);
   err_file();

 }
  Hide();

}

//---------------------------------------------------------------------------
void __fastcall TForm1::is_myodbc(void)
{

  TRegistry *Registry = new TRegistry();
  Memo3->Lines->Clear();

  try
   {
     Registry->RootKey = HKEY_LOCAL_MACHINE;
     // the basic data of myodbc
     if (Registry->OpenKey("Software\\ODBC\\ODBCINST.INI\\MySQL", false))
       {
         Memo3->Lines->Add("Driver Version\t" + Registry->ReadString("DriverODBCVer"));
         Memo3->Lines->Add("Driver\t\t" + Registry->ReadString("Driver"));
         Memo3->Lines->Add("API Level\t\t" + Registry->ReadString("APILevel"));
         Memo3->Lines->Add("Setup\t\t" + Registry->ReadString("Setup"));
         Memo3->Lines->Add("SQL Level\t" + Registry->ReadString("SQLLevel"));
       }
      else
         Memo3->Lines->Add("Not Found");

   }
  catch (...)
   {
    delete Registry;
   }
   Memo3->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::is_myini(void)
{
 AnsiString asFileName = FileSearch("my.ini", win_dir());
 if (asFileName.IsEmpty())
   {
     Memo6->Lines->Add("There is a my.ini file.......................no");
     b_force = true;
     base_dir();
   }
 else
   {
     Memo6->Lines->Add("There is a my.ini file.......................yes");
     Memo1->Enabled = true;
     Memo1->Lines->Clear();
     my_ini_load();
     check_server();
   }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::check_start(void)
{
 char file[_MAX_FNAME];
 char ext[_MAX_EXT];
 char drive[_MAX_DRIVE];
 char dir[_MAX_DIR];
 bool forceserver = false;
 Memo6->Lines->Add("Verifying the my.ini keys.");

 TIniFile *pIniFile = new
 TIniFile(win_dir() + "\\my.ini");

 AnsiString lx = pIniFile->ReadString("WinMySQLadmin","Server","")  ;
 AnsiString drx = pIniFile->ReadString("mysqld","datadir","")  ;
 AnsiString br = pIniFile->ReadString("mysqld","basedir","")  ;
 delete pIniFile;

 if (FileExists("c:/my.cnf"))
 {
   if (RenameFile("c:/my.cnf", "c:/my_cnf.bak"))
   Memo6->Lines->Add("The my.cnf file was renamed to my_cnf.bak....yes");
 }

 if (!lx.IsEmpty())
 {
   if (FileExists(lx))
    Memo6->Lines->Add("MySQL server path key........................OK");
   else
   {
    Memo6->Lines->Add("MySQL server path key........................no");
    Memo6->Lines->Add(" Key value in my.ini: " + lx);
    Memo6->Lines->Add(" Trying to fix");
    _splitpath((lx).c_str(),drive,dir,file,ext);
    AnsiString lw = (AnsiString) file + ext;
    AnsiString xpath = BaseDir->Text + "/bin/" + lw;
    if (FileExists(xpath))
    {
     Memo6->Lines->Add(" Found: " + xpath + "  Fixed.");
     WritePrivateProfileString("WinMySQLAdmin", "Server", xpath.c_str() , "my.ini");
     my_ini_load();
     forceserver = true;
    }
    else
    {
      try_server();
      my_ini_load();
      forceserver = true;
    }
   }
 }
 else
 {
  Memo6->Lines->Add("MySQL path's key is empty....................yes");
  Memo6->Lines->Add(" Trying to fix");
  try_server();
  my_ini_load();
 }

 if (!drx.IsEmpty())
 {

   if (DirectoryExists(drx))
    Memo6->Lines->Add("datadir......................................OK");
   else
   {
    Memo6->Lines->Add(" Datadir value in my.ini: " + drx);
    Memo6->Lines->Add(" Trying to fix the datadir value");
    if (DirectoryExists(BaseDir->Text + "/data"))
    {
     Memo6->Lines->Add(" Found: " + BaseDir->Text + "/data" + "  Fixed.");
     WritePrivateProfileString("mysqld", "datadir", (BaseDir->Text + "/data").c_str() , "my.ini");
     my_ini_load();
     forceserver = true;
    }

   }
 }

 if (!br.IsEmpty())
 {
   if (DirectoryExists(br))
    Memo6->Lines->Add("basedir......................................OK");
   else
   {
    Memo6->Lines->Add(" Basedir value in my.ini: " + br);
    Memo6->Lines->Add(" Trying to fix the basedir value");
    if (DirectoryExists(BaseDir->Text ))
    {
     Memo6->Lines->Add(" Found: " + BaseDir->Text +  " Fixed.");
     WritePrivateProfileString("mysqld", "basedir", (BaseDir->Text).c_str() , "my.ini");
     my_ini_load();
     forceserver = true;
    }

   }
 }

 my_ini_load();
 if (forceserver)
 service_on_start();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::try_server(void)
{
 if (b_NT)
 {
  if (FileExists(BaseDir->Text + "/bin/mysqld-nt.exe"))
  {
   Memo6->Lines->Add(" Found: " + BaseDir->Text + "/bin/mysqld-nt.exe");
   WritePrivateProfileString("WinMySQLAdmin", "Server", (BaseDir->Text + "/bin/mysqld-nt.exe").c_str() , "my.ini");
  }
  else if (FileExists(BaseDir->Text + "/bin/mysqld-max-nt.exe"))
  {
   Memo6->Lines->Add(" Found: " + BaseDir->Text + "/bin/mysqld-max-nt.exe");
   WritePrivateProfileString("WinMySQLAdmin", "Server", (BaseDir->Text + "/bin/mysqld-max-nt.exe").c_str() , "my.ini");
  }
  else if (FileExists(BaseDir->Text + "/bin/mysqld-shareware.exe"))
  {
   Memo6->Lines->Add(" Found: " + BaseDir->Text + "/bin/mysqld-shareware.exe");
   WritePrivateProfileString("WinMySQLAdmin", "Server", (BaseDir->Text + "/bin/mysqld-shareware.exe").c_str() , "my.ini");
  }

  else
  Memo6->Lines->Add(" Fatal error: not found any MySQL server for NT OS ");

 }
 else
 {
  if (FileExists(BaseDir->Text + "/bin/mysqld-opt.exe"))
  {
   Memo6->Lines->Add(" Found: " + BaseDir->Text + "/bin/mysqld-opt.exe");
   WritePrivateProfileString("WinMySQLAdmin", "Server", (BaseDir->Text + "/bin/mysqld-opt.exe").c_str() , "my.ini");
  }
  else if (FileExists(BaseDir->Text + "/bin/mysqld.exe"))
  {
   Memo6->Lines->Add(" Found: " + BaseDir->Text + "/bin/mysqld.exe");
   WritePrivateProfileString("WinMySQLAdmin", "Server", (BaseDir->Text + "/bin/mysqld.exe").c_str() , "my.ini");
  }
  else if (FileExists(BaseDir->Text + "/bin/mysqld-shareware.exe"))
  {
    Memo6->Lines->Add(" Found: " + BaseDir->Text + "/bin/mysqld-shareware.exe");
    WritePrivateProfileString("WinMySQLAdmin", "Server", (BaseDir->Text + "/bin/mysqld-shareware.exe").c_str() , "my.ini");
  }
  else if (FileExists(BaseDir->Text + "/bin/mysqld-max.exe"))
  {
    Memo6->Lines->Add(" Found: " + BaseDir->Text + "/bin/mysqld-max.exe");
    WritePrivateProfileString("WinMySQLAdmin", "Server", (BaseDir->Text + "/bin/mysqld-max.exe").c_str() , "my.ini");
  }
  else
  Memo6->Lines->Add(" Fatal error: not found any server file.");
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NT_force(void)
{
 server_path();
 if (!service_exist())
 {
  if (service_create())
   service_start();
 }
 else
 {
  if (!service_is_running())
  service_start();
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::service_on_start(void)
{
 if (b_NT)
 {
   if (service_exist())
   {
     Memo6->Lines->Add("The Service is already installed.............yes");
     if (!service_is_running())
     {
        Memo6->Lines->Add("The Service is running.......................no");
        if (service_remove())
        {
          Memo6->Lines->Add("The Service was removed......................yes");
          server_path();
          if (service_create())
          {
            if  (service_start())
             Memo6->Lines->Add("The Service was installed and started........yes");
            else
             Memo6->Lines->Add("Problems to start the Service................yes");
          }
          else
          Memo6->Lines->Add("Problems to create the Service...............yes");
        }
        else
        Memo6->Lines->Add("Problems to remove the Service...............yes");
     }
  }
  else
  {
    Memo6->Lines->Add("The Service is already installed.............no");
    server_path();
    if (service_create())
    {
      if  (service_start())
      Memo6->Lines->Add("The Service was installed and started........yes");
      else
      Memo6->Lines->Add("Problems to start the Service................yes");
    }
    else
    Memo6->Lines->Add("Problems to create the Service...............yes");
  }
 }
 else
 my_ini_load();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::base_dir(void)
{
 String xBaseDir;
 int x ;
 xBaseDir = ExtractFilePath(Application->ExeName);
 x = StrLen(xBaseDir.c_str()) - 5 ;
 xBaseDir.SetLength(x);
 BaseDir->Text = xBaseDir;
 xBaseDir = convert_dir();
 BaseDir->Text = xBaseDir;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TForm1::win_dir()
{
  AnsiString WinDir;
  UINT       BufferSize = GetWindowsDirectory(NULL,0);
  WinDir.SetLength(BufferSize+1);
  GetWindowsDirectory(WinDir.c_str(),BufferSize);
  char* dirw = WinDir.c_str();
  return dirw ;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::my_ini_load(void)
{
 AnsiString asFileName = FileSearch("my.ini", win_dir());
 if (!asFileName.IsEmpty())
 Memo1->Lines->LoadFromFile(win_dir() + "\\my.ini");

}
//---------------------------------------------------------------------------
void __fastcall TForm1::check_server(void)
{

 char drive[_MAX_DRIVE];
 char dir[_MAX_DIR];
 char file[_MAX_FNAME];
 char ext[_MAX_EXT];


 TIniFile *pIniFile = new
 TIniFile(win_dir() + "\\my.ini");

 base_dir();
 AnsiString get_file = pIniFile->ReadString("WinMySQLadmin","Server","")  ;
 _splitpath((get_file).c_str(),drive,dir,file,ext);
 AnsiString file_name = (AnsiString) file + ext;

 if ( file_name == "mysqld-shareware.exe") {ShareVer->Checked = true;}
 if ( file_name == "mysqld.exe") {MysqldVer->Checked = true;}
 if ( file_name == "mysqld-max.exe") {MysqldVerMax->Checked = true;}
 if ( file_name == "mysqld-opt.exe") {OptVer->Checked = true;}
 if ( file_name == "mysqld-nt.exe") {NtVer->Checked = true;}
 if ( file_name == "mysqld-max-nt.exe") {NtVerMax->Checked = true;}

 delete pIniFile;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Showme1Click(TObject *Sender)
{
 if(Showme1->Caption == "Show me")  {  TrayMessage(NIM_DELETE);
       Showme1->Caption = "Hide me";  Show();  }
  else  { TrayMessage(NIM_ADD);  TrayMessage(NIM_MODIFY);
       Showme1->Caption = "Show me";  Hide();  }
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::is_running()
{
  HANDLE hEventShutdown;
  bool ret_code = false;
  hEventShutdown=OpenEvent(EVENT_MODIFY_STATE, 0, "MySqlShutdown");

  if(hEventShutdown)
   ret_code = true;

  CloseHandle(hEventShutdown);

  return ret_code;
}

//---------------------------------------------------------------------------
bool __fastcall TForm1::standalone_start()
{
    memset(&pi, 0, sizeof(pi));
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow |= SW_SHOWNORMAL;


    TIniFile *pIniFile = new
    TIniFile(win_dir() + "\\my.ini");
     vpath =   pIniFile->ReadString("WinMySQLadmin","Server","");
    if (b_NT || vpath == "mysqld-max.exe")
     vpath += " --standalone\0" ;
    else
      vpath =   pIniFile->ReadString("WinMySQLadmin","Server","") + "\0" ;

    if ( ! CreateProcess(0,vpath.c_str(), 0, 0, 0, 0, 0, 0, &si,&pi))
     {
       TrayMessage(NIM_MODIFY);
       return false;
     }
    else
      {
        TrayMessage(NIM_MODIFY);
        return true;

      }

}

//---------------------------------------------------------------------------
bool __fastcall TForm1::err_file()
{
  Memo4->Enabled = true;
  Memo4->Lines->Clear();
  AnsiString asFileName = FileSearch("mysql.err", BaseDir->Text + "/data");
  if (!asFileName.IsEmpty())
   {
     FName = BaseDir->Text + "/data/mysql.err";
     ifstream in((FName).c_str());
     in.seekg(0, ios::end);
     string s, line;
     deque<string> v;
     deque<string> lines;
     streampos sp = in.tellg();
     if (sp <= 1000)
      in.seekg(0, ios::beg);
    else
     {
      in.seekg(0, ios::beg);
      in.seekg((sp - 1000));
     }

     do {
          lines.push_back(line);
     }while (getline(in, line));


     if( lines.size() <= 15)
      {
        deque<string>::reverse_iterator r;
        for(r = lines.rbegin(); r != lines.rend() ; r++)
         {
          if (b_ereport)
           Memo5->Lines->Add((*r).c_str());
           Memo4->Lines->Add((*r).c_str());

         }
      }
     else
      {
        int k = 0;
        deque<string>::reverse_iterator r;
        for(r = lines.rbegin(); r != lines.rend(); r++)
         {
           if (b_ereport)
           Memo5->Lines->Add((*r).c_str());
           Memo4->Lines->Add((*r).c_str());
           if (++k >= 15) { break;}
         }
      }
      in.close();
      return true;
   }
  else
    return false;

}

//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
 Showme1->Caption = "Show me";
  TrayMessage(NIM_ADD);
  TrayMessage(NIM_MODIFY);
  Hide();
  if (b_force) {Form2->Show();}
  Timer1->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::get_server_file()
{

  AnsiString FileName;

 if(!b_NT) {
   FileName =  FileSearch("mysqld-opt.exe", ExtractFilePath(Application->ExeName));
   if (FileName.IsEmpty())  FileName =  FileSearch("mysqld.exe", ExtractFilePath(Application->ExeName));
   if (FileName.IsEmpty())  FileName =  FileSearch("mysqld-max.exe", ExtractFilePath(Application->ExeName));
   if (FileName.IsEmpty())    FileName =  FileSearch("mysqld-shareware.exe", ExtractFilePath(Application->ExeName));

   if (!FileName.IsEmpty()){
      if ( FileName == "mysqld-opt.exe") {OptVer->Checked = true;}
      if ( FileName == "mysqld.exe") {MysqldVer->Checked= true;}
      if ( FileName == "mysqld-max.exe") {MysqldVerMax->Checked= true;}
      if ( FileName == "mysqld-shareware.exe") {ShareVer->Checked= true;} }

  }
 else {

   FileName =  FileSearch("mysqld-nt.exe", ExtractFilePath(Application->ExeName));
   if (FileName.IsEmpty())  FileName =  FileSearch("mysqld-opt.exe", ExtractFilePath(Application->ExeName));
   if (FileName.IsEmpty())  FileName =  FileSearch("mysqld.exe", ExtractFilePath(Application->ExeName));
   if (FileName.IsEmpty())  FileName =  FileSearch("mysqld-shareware.exe", ExtractFilePath(Application->ExeName));
   if (FileName.IsEmpty())  FileName =  FileSearch("mysqld-max-nt.exe", ExtractFilePath(Application->ExeName));
   if (FileName.IsEmpty())  FileName =  FileSearch("mysqld-max.exe", ExtractFilePath(Application->ExeName));


   if (!FileName.IsEmpty()) {
      if ( FileName == "mysqld-nt.exe") {NtVer->Checked = true;}
      if ( FileName == "mysqld-max.exe") {MysqldVerMax->Checked= true;}
      if ( FileName == "mysqld-max-nt.exe") {NtVerMax->Checked = true;}
      if ( FileName == "mysqld-opt.exe") {OptVer->Checked = true;}
      if ( FileName == "mysqld.exe") {MysqldVer->Checked= true;}
      if ( FileName == "mysqld-shareware.exe") {ShareVer->Checked= true;} }

     }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::create_my_ini_file(void)
{
 String xBaseDir;
 String DataDir;
 String mycnf = "c:/my.cnf";
 String mybak = "c:/my_cnf.bak";
 char szFileName[6];
 int x ;

 xBaseDir = ExtractFilePath(Application->ExeName);
 x = StrLen(xBaseDir.c_str()) - 5 ;

 xBaseDir.SetLength(x);

 BaseDir->Text = xBaseDir;
 xBaseDir = convert_dir();
 BaseDir->Text = xBaseDir;
 String fName;
 AnsiString jk;
 bool bak = false;
 strcpy(szFileName,"/my.ini");

 DataDir = xBaseDir + "/data";

 fName = win_dir() + szFileName;

 /* first if my.cnf exist */
 Memo6->Lines->Add("Creating my.ini file.");
 if (FileExists(mycnf))
 {
   Memo6->Lines->Add("There is a my.cnf file.......................yes");
   if (RenameFile(mycnf, mybak))
    {
      bak = true;
      Memo6->Lines->Add("The my.cnf file was renamed to my_cnf.bak....yes");
    }
   else
   {
    Memo6->Lines->Add("The my.cnf file was renamed to my_cnf.bak....no");
    Memo6->Lines->Add("WARNING: You need to rename the my.cnf file");
   }
    if (bak)
    {
      if (cnf_copy_file((mybak).c_str(), (fName).c_str()))
      {
       Memo6->Lines->Add("The my.cnf file was copied to my.ini file....yes");
       WritePrivateProfileString("mysqld", "basedir", xBaseDir.c_str() , "my.ini");
       WritePrivateProfileString("mysqld", "datadir", DataDir.c_str() , "my.ini");
       if (ShareVer->Checked) { jk = xBaseDir + "/bin/mysqld-shareware.exe";}
       if (MysqldVer->Checked) {jk = xBaseDir + "bin/mysqld.exe";}
       if (OptVer->Checked) {jk = xBaseDir + "/bin/mysqld-opt.exe";}
       if (NtVer->Checked) {jk = xBaseDir + "/bin/mysqld-nt.exe";}
       if (MysqldVerMax->Checked) {jk = xBaseDir + "bin/mysqld-max.exe";}
       if (NtVerMax->Checked) {jk = xBaseDir + "/bin/mysqld-max-nt.exe";}
       WritePrivateProfileString("WinMySQLAdmin", "Server", jk.c_str() , "my.ini");
       WritePrivateProfileString("WinMySQLAdmin", "user", Form2->Edit1->Text.c_str() , "my.ini");
       WritePrivateProfileString("WinMySQLAdmin", "password", Form2->Edit2->Text.c_str() , "my.ini");
       my_ini_load();
      }
    }
    else
    {

      if (cnf_copy_file((mycnf).c_str(), (fName).c_str()))
      {
       Memo6->Lines->Add("The my.cnf file was copied to my.ini file....yes");
       WritePrivateProfileString("mysqld", "basedir", xBaseDir.c_str() , "my.ini");
       WritePrivateProfileString("mysqld", "datadir", DataDir.c_str() , "my.ini");
       if (ShareVer->Checked) { jk = xBaseDir + "/bin/mysqld-shareware.exe";}
       if (MysqldVer->Checked) {jk = xBaseDir + "bin/mysqld.exe";}
       if (OptVer->Checked) {jk = xBaseDir + "/bin/mysqld-opt.exe";}
       if (NtVer->Checked) {jk = xBaseDir + "/bin/mysqld-nt.exe";}
       if (MysqldVerMax->Checked) {jk = xBaseDir + "bin/mysqld-max.exe";}
       if (NtVerMax->Checked) {jk = xBaseDir + "/bin/mysqld-max-nt.exe";}
       WritePrivateProfileString("WinMySQLAdmin", "Server", jk.c_str() , "my.ini");
       WritePrivateProfileString("WinMySQLAdmin", "user", Form2->Edit1->Text.c_str() , "my.ini");
       WritePrivateProfileString("WinMySQLAdmin", "password", Form2->Edit2->Text.c_str() , "my.ini");
       my_ini_load();
      }
    }
 }

 else
 {
  Memo6->Lines->Add("There is a my.cnf file.......................no");
  Memo1->Enabled = true;
  Memo1->Lines->Clear();
  strcpy(szFileName,"\\my.ini");
  int iFileHandle;
  iFileHandle = FileCreate(win_dir() + szFileName );

  jk = "#This File was made using the WinMySQLAdmin 1.4 Tool\n" ;
  FileWrite(iFileHandle, (jk).c_str(), (jk).Length());

  jk = "#" + Now() + "\n\n" ;
  FileWrite(iFileHandle, (jk).c_str(), (jk).Length());

  jk = "#Uncomment or Add only the keys that you know how works.\n" ;
  FileWrite(iFileHandle, (jk).c_str(), (jk).Length());

  jk = "#Read the MySQL Manual for instructions\n\n" ;
  FileWrite(iFileHandle, (jk).c_str(), (jk).Length());

  FileClose(iFileHandle);

  WritePrivateProfileString("mysqld", "basedir", convert_dir().c_str() , "my.ini");
  jk = "#bind-address" ;
  WritePrivateProfileString("mysqld",(jk).c_str(), (vip).c_str() , "my.ini");
  jk = "datadir"  ;
  WritePrivateProfileString("mysqld",(jk).c_str(), (xBaseDir + "/data").c_str() , "my.ini");
  jk = "#language" ;
  WritePrivateProfileString("mysqld",(jk).c_str(), (convert_dir() + "/share/your language directory").c_str() , "my.ini");
   jk = "#slow query log#" ;
  WritePrivateProfileString("mysqld",(jk).c_str(),"" , "my.ini");
  jk = "#tmpdir#" ;
  WritePrivateProfileString("mysqld",(jk).c_str(),"" , "my.ini");
  jk = "#port" ;
  WritePrivateProfileString("mysqld",(jk).c_str(),"3306" , "my.ini");
  jk = "#set-variable" ;
  WritePrivateProfileString("mysqld",(jk).c_str(),"key_buffer=16M" , "my.ini");

  if (ShareVer->Checked) { jk =  convert_dir() + "/bin/mysqld-shareware.exe";}
  if (MysqldVer->Checked) {jk =  convert_dir() + "/bin/mysqld.exe";}
  if (OptVer->Checked) {jk =  convert_dir() + "/bin/mysqld-opt.exe";}
  if (NtVer->Checked) {jk =  convert_dir() + "/bin/mysqld-nt.exe";}
  if (MysqldVerMax->Checked) {jk =  convert_dir() + "/bin/mysqld-max.exe";}
  if (NtVerMax->Checked) {jk =  convert_dir() + "/bin/mysqld-max-nt.exe";}

  WritePrivateProfileString("WinMySQLadmin","Server",(jk).c_str(), "my.ini");

  jk =  Form2->Edit1->Text ;
  WritePrivateProfileString("WinMySQLadmin","user",(jk).c_str(), "my.ini");

  jk = Form2->Edit2->Text ;
  WritePrivateProfileString("WinMySQLadmin","password",(jk).c_str(), "my.ini");


  my_ini_load();
 }

 if (b_NT)
 {
  if (service_exist())
  {
    Memo6->Lines->Add("The Service is already installed.............yes");
    if (!service_is_running())
    {
      Memo6->Lines->Add("The Service is running.......................no");
      if (service_remove())
      {
        Memo6->Lines->Add("The Service was removed......................yes");
        server_path();
        if (service_create())
        {
         if  (service_start())
         Memo6->Lines->Add("The Service was installed and started........yes");
         else
         Memo6->Lines->Add("Problems to start the Service................yes");
        }
        else
        Memo6->Lines->Add("Problems to create the Service...............yes");
      }
      else
      Memo6->Lines->Add("Problems to remove the Service...............yes");
    }
  }
  else
  {
   Memo6->Lines->Add("The Service is already installed.............no");
   server_path();
   if (service_create())
   {
     if  (service_start())
     Memo6->Lines->Add("The Service was installed and started........yes");
     else
     Memo6->Lines->Add("Problems to start the Service................yes");
   }
   else
   Memo6->Lines->Add("Problems to create the Service...............yes");
  }
 }

}

//---------------------------------------------------------------------------
bool __fastcall TForm1::cnf_copy_file(char *cnffile, char *inifile)
{
 FILE *mycnf, *myini;
 int a;

 if ( ( mycnf = fopen( cnffile, "rb" ) ) == NULL )
  return false;

 if ( ( myini = fopen( inifile, "wb" ) ) == NULL  )
 {
   fclose ( mycnf );
   return false ;
 }


 while (true)
 {
  a = fgetc( mycnf );

  if ( !feof( mycnf ) )
     fputc( a, myini );
  else
    break;
 }

 fclose ( myini );
 fclose ( mycnf );
 return true;

}
//---------------------------------------------------------------------------
bool __fastcall TForm1::create_shortcut()
{
  // Where is The Start Menu in this Machine ?
  LPITEMIDLIST  pidl;
  LPMALLOC      pShellMalloc;
  char          szDir[MAX_PATH + 16];
  AnsiString file;
  AnsiString jk = "\\WinMySQLadmin.lnk" ;

  if(SUCCEEDED(SHGetMalloc(&pShellMalloc)))
   {
     if(SUCCEEDED(SHGetSpecialFolderLocation(NULL,
          CSIDL_STARTUP, &pidl)))
      {
        if(!SHGetPathFromIDList(pidl, szDir))
          {
            pShellMalloc->Release();
            pShellMalloc->Free(pidl);
            return false;
           }

          pShellMalloc->Free(pidl);
       }

        pShellMalloc->Release();
        StrCat(szDir, jk.c_str());
    }

 // the create

 IShellLink* pLink;
 IPersistFile* pPersistFile;

 if(SUCCEEDED(CoInitialize(NULL)))
   {
     if(SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL,
                     CLSCTX_INPROC_SERVER,
                     IID_IShellLink, (void **) &pLink)))
       {

         pLink->SetPath((ExtractFilePath(Application->ExeName) + "WinMySQLadmin.exe").c_str());
         pLink->SetDescription("WinMySQLadmin Tool");
         pLink->SetShowCmd(SW_SHOW);

            if(SUCCEEDED(pLink->QueryInterface(IID_IPersistFile,
                                               (void **)&pPersistFile)))
            {

                WideString strShortCutLocation(szDir);
                pPersistFile->Save(strShortCutLocation.c_bstr(), TRUE);
                pPersistFile->Release();
            }
               pLink->Release();
        }

        CoUninitialize();
    }


  return true;
}

//---------------------------------------------------------------------------
AnsiString __fastcall TForm1::convert_dir()
{
 AnsiString buffer;
 char s[_MAX_PATH + 1];

 StrCopy(s, ( BaseDir->Text).c_str()) ;

 for (int i = 0; s[i] != NULL; i++)
 {
  if (s[i] != '\\')
    buffer += s[i];
  else
    buffer += "/";
 }
 return buffer;

}

//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
 Application->HelpCommand(HELP_FINDER,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
 ToggleState();

}
//---------------------------------------------------------------------------
bool __fastcall TForm1::service_exist()
{
  bool ret_code = false;
  char *SERVICE_NAME = "MySql";
  SC_HANDLE myService, scm;
  scm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS | GENERIC_WRITE);
  if (scm)
  {
    myService = OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
    if (myService)
     ret_code = true;
    CloseServiceHandle(myService);
  }
  CloseServiceHandle(scm);
  return ret_code;
}

//---------------------------------------------------------------------------
bool __fastcall TForm1::service_stop()
{
  bool ret_code = false;
  char *SERVICE_NAME = "MySql";
  SC_HANDLE myService, scm;
  scm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

  if (scm)
  {
    myService = OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
    if (myService)
    {
      if (b_connect)
      {
        mysql_kill(MySQL,mysql_thread_id(MySQL));
        StatusLine->SimpleText = "";
        questions = 0;
      }
      SERVICE_STATUS ss;
      if(ControlService(myService,SERVICE_CONTROL_STOP,&ss))
       ret_code = true;
    }
    CloseServiceHandle(myService);
  }
  CloseServiceHandle(scm);
  return ret_code;
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::service_start()
{
  bool ret_code = false;
  char *SERVICE_NAME = "MySql";
  SC_HANDLE myService, scm;
  scm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

  if (scm)
  {
    myService = OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
    if (myService)
    {
      /* start the service */
      if(StartService(myService, 0, NULL))
       ret_code = true;
    }
    CloseServiceHandle(myService);
  }
  CloseServiceHandle(scm);
  return ret_code;
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::service_is_running()
{
  bool query_status, ret_code = false;
  char *SERVICE_NAME = "MySql";
  SC_HANDLE myService, scm;
  SERVICE_STATUS ss;
  DWORD dwState = 0xFFFFFFFF;
  scm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
  if (scm)
  {
    myService = OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
    if (myService)
    {
      memset(&ss, 0, sizeof(ss));
      query_status = QueryServiceStatus(myService,&ss);
      if (query_status)
      {
        dwState = ss.dwCurrentState;
        if (dwState == SERVICE_RUNNING)
        ret_code = true;
      }
    }
    CloseServiceHandle(myService);
  }
  CloseServiceHandle(scm);
  return ret_code;
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::service_is_stopping()
{
  bool query_status, ret_code = false;
  char *SERVICE_NAME = "MySql";
  SC_HANDLE myService, scm;
  SERVICE_STATUS ss;
  DWORD dwState = 0xFFFFFFFF;
  scm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
  if (scm)
  {
    myService = OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
    if (myService)
    {
      memset(&ss, 0, sizeof(ss));
      query_status = QueryServiceStatus(myService,&ss);
      if (query_status)
      {
        dwState = ss.dwCurrentState;
        if (dwState == SERVICE_STOP_PENDING)
         ret_code = true;
      }
    }
    CloseServiceHandle(myService);
  }
  CloseServiceHandle(scm);
  return ret_code;
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::service_create()

{
  bool ret_code = false;
  char *SERVICE_NAME = "MySql";
  char *szFullPath = vpath.c_str();
  SC_HANDLE myService, scm;
  scm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

  if (scm)
  {
     myService = CreateService(
     scm,
     SERVICE_NAME,
     SERVICE_NAME,
     SERVICE_ALL_ACCESS,
     SERVICE_WIN32_OWN_PROCESS,
     SERVICE_AUTO_START	,
     SERVICE_ERROR_NORMAL,
     szFullPath,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL);
     if (myService)
      ret_code = true;
     CloseServiceHandle(myService);
   }
   CloseServiceHandle(scm);
   return ret_code;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Swin9Click(TObject *Sender)
{
 if(Application->MessageBox("Shutdown this tool", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
 {
  TrayMessage(NIM_DELETE);
  Close();
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SSW9Click(TObject *Sender)
{
 if (is_running())
    {
      if(Application->MessageBox("Shutdown the MySQL Server ", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
        {

          if (standalone_shutdown())
            {
              b_connect = false;
              b_variables = false;
              b_process = false;
              b_DB = false;
              b_extended_server_status = false;
              clear_box();
              Sleep(500);
            }
          else
              Application->MessageBox("Fails to Shutdown the Server", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
        }
    }
   else
     {
       if(Application->MessageBox("Start the MySQL Server ", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
        {
          if (standalone_start())
            {
              TrayMessage(NIM_MODIFY);
              b_extended_server_status = true;
            }
           else
             Application->MessageBox("Fails to Start the Server", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);

        }
     }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ShutDownBoth1Click(TObject *Sender)
{
  if (is_running())
  {
    if(Application->MessageBox("Shutdown the MySQL Server and this tool ", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
     {

       if (standalone_shutdown())
       {
          TrayMessage(NIM_DELETE);
          Close();
       }
       else
         {
           Application->MessageBox("Fails to Shutdown the Server", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);

         }
     }
   }
 else
    if(Application->MessageBox("Shutdown this tool ", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ShutDownthisTool1Click(TObject *Sender)
{
 if(Application->MessageBox("Shutdown this tool ", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
  {
   TrayMessage(NIM_DELETE);
   Close();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StopSClick(TObject *Sender)
{
  AnsiString s_warning;
  if (!Edit3->Text.IsEmpty())
   s_warning = "Are you sure you want to stop the service for MySQL " + Edit3->Text + "\nIf you choose yes, all open connections will be closed!" ;
  else
   s_warning = "Are you sure you want to stop the service for MySQL Server\nIf you choose yes, all open connections will be closed!" ;
  if (service_is_running())
  {
    if(Application->MessageBox(s_warning.c_str(), "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
    {
      if (service_stop())
      {
        TrayMessage(NIM_MODIFY);
        b_connect = false;
        b_variables = false;
        b_process = false;
        b_DB = false;
        b_extended_server_status = false;
        clear_box();
      }
      else
       Application->MessageBox("Fails to stop the Service.", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
    }
  }
  else
  {
    if(Application->MessageBox("Start the Service Manager for the MySQL Server ", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
    {
      if (service_start())
      {
        b_extended_server_status = true;
        TrayMessage(NIM_MODIFY);
      }
      else
        Application->MessageBox("Fails to start the Service.\nPlease wait 30 seconds to try again.", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RServiceClick(TObject *Sender)
{
 if (service_exist())
 {
   if(Application->MessageBox("Remove the MySQL Server service ", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
   {
     if (!service_remove())
      Application->MessageBox("Fails to Remove The MySQL Server Service", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
   }
 }
 else
 {
   if(Application->MessageBox("Install the MySQL Server service ", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
   {
     server_path();
     if (!service_create())
      Application->MessageBox("Fails to Install The MySQL Server Service", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
   }
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StandaClick(TObject *Sender)
{
  if (is_running())
    {
      if(Application->MessageBox("Shutdown the MySQL Server ", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
        {
          if (standalone_shutdown())
           {
             b_connect = false;
             b_variables = false;
             b_process = false;
             b_DB = false;
             b_extended_server_status = false;
             clear_box();
             Sleep(500);
             TrayMessage(NIM_MODIFY);

            }
          else
            Application->MessageBox("Fails to Shutdown the Server", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
        }
    }
   else
     {
       if(Application->MessageBox("Start the MySQL Server ", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
        {
          if (standalone_start())
            {
              StatusLine->SimpleText = "";
              TrayMessage(NIM_MODIFY);
              b_extended_server_status = true;
            }
          else
          Application->MessageBox("Fails to Start the Server", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
        }
     }
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::standalone_shutdown()
{
  bool ret_code = false;
  HANDLE hEventShutdown;
  hEventShutdown=OpenEvent(EVENT_MODIFY_STATE, 0, "MySqlShutdown");

  if (b_connect)
  {
    mysql_kill(MySQL,mysql_thread_id(MySQL));
    mysql_shutdown(MySQL);
    StatusLine->SimpleText = "";
  }

   questions = 0;
  if(hEventShutdown)
  {
    SetEvent(hEventShutdown);
    CloseHandle(hEventShutdown);
    b_connect = false;
    ret_code = true;
  }

  TrayMessage(NIM_MODIFY);
  return ret_code;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::clear_box(void)
{
 
 st22->Text = "";
 st23->Text = "";
 st24->Text = "";
 st25->Text = "";
 st26->Text = "";
 st27->Text = "";
 st28->Text = "";
 st29->Text = "";
 Edit3->Text = "";
 Edit4->Text = "";
 Edit5->Text = "";
 Edit6->Text = "";

}
//---------------------------------------------------------------------------
bool __fastcall TForm1::service_remove()
{
  bool ret_code = false;
  char *SERVICE_NAME = "MySql";
  SC_HANDLE myService, scm;
  scm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
  if (scm)
  {
   myService = OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
   if (myService)
   {
     if(DeleteService(myService))
      ret_code = true;
   }
   CloseServiceHandle(myService);
  }
  CloseServiceHandle(scm);
  return ret_code;
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::server_path()
{
 bool ret_code = true;
 TIniFile *pIniFile = new
 TIniFile(win_dir() + "\\my.ini");

 vpath = pIniFile->ReadString("WinMySQLadmin","Server","")  ;
 delete pIniFile;
 if (vpath.IsEmpty())
  ret_code = false;

 return ret_code;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::tool_connect(void)
{
  AnsiString sql_cmd;
  char	*host = NULL,*password=0,*user=0 ;
  TIniFile *pIniFile = new
  TIniFile(win_dir() + "\\my.ini");

  AnsiString MyUser = pIniFile->ReadString("WinMySQLadmin","user","")  ;
  AnsiString MyPass = pIniFile->ReadString("WinMySQLadmin","password","")  ;

  delete pIniFile;


 if (!MyUser.IsEmpty() && MyUser.Length() && !MyPass.IsEmpty() && MyPass.Length())
   {
     if (!b_connect)
      {

       MySQL = mysql_init(MySQL);
       if (mysql_real_connect(MySQL, "localhost",(MyUser).c_str(), (MyPass).c_str() , 0, 0, NULL, 0))
        b_connect = true;
       else
        {
         if(mysql_real_connect(MySQL,host,user,password , 0, 0, NULL, 0))
          {
            b_connect = true;
            sql_cmd = "GRANT ALL PRIVILEGES ON *.* TO ";
            sql_cmd +=  "'" + MyUser + "' @localhost IDENTIFIED BY ";
            sql_cmd +=  "'" + MyPass + "' with GRANT OPTION";
            char* fsql_cmd = sql_cmd.c_str();
            if(!mysql_query(MySQL, fsql_cmd ))
             StatusLine->SimpleText = " ";
          }

        }

      }

  }
 else
  {
   if (!b_connect)
    {
      MySQL = mysql_init(MySQL);
      if(mysql_real_connect(MySQL,host,user,password , 0, 0, NULL, 0))
      b_connect = true;
    }
  }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Timer3Timer(TObject *Sender)
{
  if ((b_NT) && service_is_running())
   tool_connect();
  if ((b_NT) && !service_is_running() && is_running())
   tool_connect();
  if (!(b_NT) && is_running())
   tool_connect();

  if (b_connect)
   {
     /*get_server_status(); */
     if (!b_MySQLNode)
     tree_root();
     Extended->Enabled = true;
     if (!b_process && !get_process())
      StatusLine->SimpleText = "";
     if (!b_variables && !get_variables())
        StatusLine->SimpleText = "";
     if (QtimeE)
     Timer3->Interval = (QtimeE * 1000);
     else
      Timer3->Interval = 10000;
   }
  else
    Extended->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::get_server_status(void)
{

  get_server_extended_status();
  Edit3->Text = mysql_get_server_info(MySQL);
  Edit4->Text = mysql_get_host_info(MySQL);
  Edit5->Text = mysql_get_client_info();
  Edit6->Text = mysql_get_proto_info(MySQL);

}

//---------------------------------------------------------------------------
bool __fastcall TForm1::get_process()
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  unsigned int i;
  int k = 0;
  int therow = 1;
  b_new_line= true;

   StringGrid2->RowCount= 2;

   if (!(res=mysql_list_processes(MySQL)))
      {
       	return false;
      }

      while ((row=mysql_fetch_row(res)) != 0)
       {
         mysql_field_seek(res,0);
         StringGrid2->Cells[0][0] = "PID";
         StringGrid2->Cells[1][0] = "User";
         StringGrid2->Cells[2][0] = "Host";
         StringGrid2->Cells[3][0] = "DB";
         StringGrid2->Cells[4][0] = "Command";
         StringGrid2->Cells[5][0] = "Time";
         StringGrid2->Cells[6][0] = "State";
         StringGrid2->Cells[7][0] = "Info";
         for (i=0 ; i < mysql_num_fields(res); i++)
          {

             if (k <= 6 )
              {
               StringGrid2->Cells[k][therow] = row[i];
               k++;
              }
             else
               {

                 StringGrid2->Cells[(k)][therow] = row[i];
                 k = 0;
                 therow++ ;
                 StringGrid2->RowCount++;

               }

          }

       }

      StringGrid2->RowCount--;
      mysql_free_result(res);
      StringGrid5->RowCount--;
      b_process = true;
      return true;

}
//---------------------------------------------------------------------------
bool __fastcall TForm1::get_variables()
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  unsigned int i;
  int k = 1;
  b_new_line= true;
  bool left = true;
  AnsiString report;
  StringGrid1->RowCount = 2;

   if (mysql_query(MySQL,"show variables") ||
	  !(res=mysql_store_result(MySQL)))
      {
        return false;
      }

      while ((row=mysql_fetch_row(res)) != 0)
       {
         mysql_field_seek(res,0);

         StringGrid1->Cells[0][0] = "Variable Name";
         StringGrid1->Cells[1][0] = "Value";


         for (i=0 ; i < mysql_num_fields(res); i++)
          {

             if (left)
              {
                 if (b_treport)
                 report = set_row(row[i]);
                 StringGrid1->Cells[0][k++] = row[i];
                 left = false;
              }
             else
               {
                 if (b_treport)
                  Memo5->Lines->Add(report + row[i]);
                 StringGrid1->RowCount++;
                 StringGrid1->Cells[1][--k] = row[i];
                 k++;
                 left = true;
               }

          }

       }

    StringGrid1->RowCount--;
    mysql_free_result(res);
    b_variables = true;
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::nice_time(AnsiString buff)
{

  unsigned long sec;
  unsigned long tmp;
  AnsiString  mytime;

  try {
        sec = StrToFloat(buff);
    }
    catch(...){
        st29->Text = buff;
        return false;

    }


  if (sec >= 3600L*24)
  {
    tmp=sec/(3600L*24);
    sec-=3600L*24*tmp;
    try {

        mytime = FloatToStr(tmp);
    }
    catch(...){
        st29->Text = buff;

        return false;

    }

    if (tmp > 1)
      mytime+= " days ";
    else
      mytime+= " day ";

  }

  if (sec >= 3600L)
  {
    tmp=sec/3600L;
    sec-=3600L*tmp;
     try {
        mytime += FloatToStr(tmp);
    }
    catch(...){
        st29->Text = buff;
        return false;

    }

    if (tmp > 1)
      mytime+= " hours ";
    else
      mytime+= " hour ";
  }
  if (sec >= 60)
  {
    tmp=sec/60;
    sec-=60*tmp;
     try {
        mytime += FloatToStr(tmp);
    }
    catch(...){
        st29->Text = buff;
        return false;

    }

    mytime+= " min ";

  }
   try {
        mytime += FloatToStr(sec);
    }
    catch(...){
        st29->Text = buff;
        return false;

    }

  mytime+= " sec ";
  st29->Text = mytime ;
 return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button11Click(TObject *Sender)
{
  if (b_connect)
  {
    if (get_variables())
     StatusLine->SimpleText = "";
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
 if (b_connect)
  {
    if (get_process())
     StatusLine->SimpleText = "";
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button7Click(TObject *Sender)
{
  if (b_connect)
  {
    if (mysql_refresh(MySQL,REFRESH_LOG))
     StatusLine->SimpleText = "";
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
 Memo1->Enabled = true;
 Memo1->Lines->Clear();
 AnsiString asFileName = FileSearch("my.ini", win_dir());
 if (asFileName.IsEmpty())
  Application->MessageBox("Don't found my.ini file on the Win Directory", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
 else
  my_ini_load();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
 TIniFile *pIniFile = new
 TIniFile(win_dir() + "\\my.ini");

 if (!Memo1->GetTextLen())
   Application->MessageBox("The Memo Box is Empty", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
 else
  {
   if(Application->MessageBox("Are you sure to write the modifications into My.ini file.", "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
    {
      Memo1->Lines->SaveToFile(win_dir() + "\\my.ini");

      Memo1->Lines->Clear();
      Memo1->Enabled = true;
      Memo1->Lines->Clear();
      if (NtVer->Checked)
       pIniFile->WriteString("WinMySQLadmin","Server",convert_dir() + "/bin/mysqld-nt.exe");
      if (MysqldVer->Checked == true)
       pIniFile->WriteString("WinMySQLadmin","Server", convert_dir() + "/bin/mysqld.exe");
      if (ShareVer->Checked)
       pIniFile->WriteString("WinMySQLadmin","Server",convert_dir() + "/bin/mysqld-shareware.exe");
      if (OptVer->Checked)
       pIniFile->WriteString("WinMySQLadmin","Server", convert_dir() + "/bin/mysqld-opt.exe");
      if (NtVerMax->Checked)
       pIniFile->WriteString("WinMySQLadmin","Server",convert_dir() + "/bin/mysqld-max-nt.exe");
      if (MysqldVerMax->Checked == true)
       pIniFile->WriteString("WinMySQLadmin","Server", convert_dir() + "/bin/mysqld-max.exe");
       my_ini_load();
      Application->MessageBox("My.ini was modified", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
    }

  }
  delete pIniFile;
  Memo1->Lines->Clear();
  my_ini_load();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  if(create_shortcut())
   Application->MessageBox("The ShortCut on Start Menu was created", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
 else
   Application->MessageBox("Fails the Operation of Create the ShortCut", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
 BROWSEINFO    info;
  char          szDir[MAX_PATH];
  char          szDisplayName[MAX_PATH];
  LPITEMIDLIST  pidl;
  LPMALLOC      pShellMalloc;


 if(SHGetMalloc(&pShellMalloc) == NO_ERROR)
    {

      memset(&info, 0x00,sizeof(info));
      info.hwndOwner = Handle;
      info.pidlRoot  = 0;
      info.pszDisplayName = szDisplayName;
      info.lpszTitle = "Search MySQL Base Directory";
      info.ulFlags   = BIF_RETURNONLYFSDIRS;
      info.lpfn = 0;

      pidl = SHBrowseForFolder(&info);

      if(pidl)
        {

          if(SHGetPathFromIDList(pidl, szDir)) {BaseDir->Text = szDir; }

            pShellMalloc->Free(pidl);
        }
        pShellMalloc->Release();
    }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::SpeedButton3Click(TObject *Sender)
{
  if(Showme1->Caption == "Show me")  {  TrayMessage(NIM_DELETE);
       Showme1->Caption = "Hide me";  Show();  }
  else  { TrayMessage(NIM_ADD);  TrayMessage(NIM_MODIFY);
       Showme1->Caption = "Show me";  Hide();  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ExtendedClick(TObject *Sender)
{
  if (b_connect)
    get_server_status();
 /*
 if (b_extended_server_status)
 {
   Extended->Caption = "Start Extended Server Status";
   b_extended_server_status = false;
   clear_box();
 }
 else
 {
   Extended->Caption = "Stop Extended Server Status";
   b_extended_server_status = true;
 }    */
}
//---------------------------------------------------------------------------
void __fastcall TForm1::server_options(void)
{
AnsiString FileName;
FileName =  FileSearch("mysqld-opt.exe", ExtractFilePath(Application->ExeName));
if (FileName.IsEmpty()) {OptVer->Enabled = false; }

FileName =  FileSearch("mysqld-shareware.exe", ExtractFilePath(Application->ExeName));
if (FileName.IsEmpty()) {ShareVer->Enabled = false; }

FileName =  FileSearch("mysqld.exe", ExtractFilePath(Application->ExeName));
if (FileName.IsEmpty()) {MysqldVer->Enabled = false; }

FileName =  FileSearch("mysqld-nt.exe", ExtractFilePath(Application->ExeName));
if (FileName.IsEmpty()) {NtVer->Enabled = false; }

FileName =  FileSearch("mysqld-max.exe", ExtractFilePath(Application->ExeName));
if (FileName.IsEmpty()) {MysqldVerMax->Enabled = false; }

FileName =  FileSearch("mysqld-max-nt.exe", ExtractFilePath(Application->ExeName));
if (FileName.IsEmpty()) {NtVerMax->Enabled = false; }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::server_report(void)
{

  AnsiString strspace;
  Memo5->Lines->Clear();
  Memo5->Lines->Add("This Report was made using the WinMySQLAdmin 1.4 Tool");
  Memo5->Lines->Add("");
  Memo5->Lines->Add(Now());
  Memo5->Lines->Add("");

  Memo5->Lines->Add("Description        :");
  Memo5->Lines->Add("How-To-Repeat      :");
  Memo5->Lines->Add("Fix                :");
  Memo5->Lines->Add("Synopsis           :");
  Memo5->Lines->Add("Submitter-Id       :");
  Memo5->Lines->Add("Originator         :");
  Memo5->Lines->Add("Organization       :");
  Memo5->Lines->Add("MySQL support      : [none | licence | email support | extended email support ]");
  Memo5->Lines->Add("Severity           : [non-critical | serious | critical ]");
  Memo5->Lines->Add("Priority           : [low | medium | high ]");
  Memo5->Lines->Add("Category           : [mysqld, mysql client, C, PHP, PERL,  ...]");
  Memo5->Lines->Add("Class              : [ sw-bug | doc-bug | change-request | support ]");
  Memo5->Lines->Add("OS Platform        : " + OS->Text);
  Memo5->Lines->Add("Machine description:");
  Memo5->Lines->Add("Compiler           : VC++ 6.0");
  Memo5->Lines->Add("Architecture       : i686");
  Memo5->Lines->Add("Total Memory       : " + Edit2->Text);
  Memo5->Lines->Add("");


   Memo5->Lines->Add("My.ini File");
   Memo5->Lines->Add("");
   Memo5->Lines->Add("[mysqld]");
   static char  ServiceSection[3000];
	LPSTR MySectionPtr;
	AnsiString h;
	GetPrivateProfileSection("mysqld",
                             ServiceSection,
                             3000,
                             "my.ini");

	MySectionPtr = ServiceSection;

	while (*MySectionPtr)
    {

          h = TrimRight(TrimLeft(MySectionPtr));
          h.SetLength(1);
          if (h != "#" )
          {
	   Memo5->Lines->Add(MySectionPtr);

          }
	  MySectionPtr += lstrlen(MySectionPtr) + 1;
	}



   b_preport = true;
   Memo5->Lines->Add("");
   Memo5->Lines->Add("Server Status Values");
   Memo5->Lines->Add("");
   Memo5->Lines->Add(set_row("Server Info") + mysql_get_server_info(MySQL));
   Memo5->Lines->Add(set_row("Host Info") + mysql_get_host_info(MySQL));
   Memo5->Lines->Add(set_row("Client Info") + mysql_get_client_info());
   Memo5->Lines->Add(set_row("Proto Info") + mysql_get_proto_info(MySQL));
   get_server_extended_status();
   b_preport = false;
   b_treport = true;
   Memo5->Lines->Add("");
   Memo5->Lines->Add("Variables Values");
   Memo5->Lines->Add("");
   get_variables();
   b_treport = false;
   b_ereport = true;
   Memo5->Lines->Add("");
   Memo5->Lines->Add("Last Lines from Err File");
   Memo5->Lines->Add("");
   err_file();
   b_ereport = false;

}

void __fastcall TForm1::SpeedButton4Click(TObject *Sender)
{
 if(b_connect)
  server_report();
 else
  Application->MessageBox("The Server must be connected", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton5Click(TObject *Sender)
{
  AnsiString PathName;
  SaveFileDialog->FileName = PathName;
  if (SaveFileDialog->Execute() ){
    PathName= SaveFileDialog->FileName;
    Caption = ExtractFileName(PathName);
    Memo5->Lines->SaveToFile(PathName);
    Memo5->Modified = false;
  }
}
//---------------------------------------------------------------------------
String  __fastcall TForm1::set_row(String s_row)
{
  int i = 35 - s_row.Length();
  for (int y = 1 ; y <= i ;y++ )
     s_row+= " ";
   return s_row ;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton6Click(TObject *Sender)
{
 PrinterSetupDialog1->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton7Click(TObject *Sender)
{
  AnsiString PathName;
  if (PrintDialog1->Execute()){
    try {
        Memo5->Print(PathName);
    }
    catch(...){
        Printer()->EndDoc();
        throw;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton8Click(TObject *Sender)
{
 Memo5->CutToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton9Click(TObject *Sender)
{
  Memo5->CopyToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton10Click(TObject *Sender)
{

 Memo5->PasteFromClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton11Click(TObject *Sender)
{
 Memo5->ClearSelection();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton12Click(TObject *Sender)
{
 Memo5->SelectAll();
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::tree_root()
{
 MYSQL_RES *res;
 MYSQL_ROW row;
 unsigned int i;
 AnsiString command;

 table_grid_clean();
 CleanGridI();
 computer_ip();

 MySQLNode = DBView->Items->Add(NULL, mainroot.UpperCase());
 MySQLNode->ImageIndex = 0;

 if (!(res=mysql_list_dbs(MySQL,"%")))
  return false;
  while ((row=mysql_fetch_row(res)) != 0)
  {
    mysql_field_seek(res,0);
    for (i=0 ; i < mysql_num_fields(res); i++)
    {
      MySQLDbs = DBView->Items->AddChild(MySQLNode, row[i]);
      MySQLDbs->ImageIndex = 1;
      MySQLDbs->SelectedIndex = 1;
    }
  }
  mysql_free_result(res);
  MySQLNode->Expanded = true;
  b_MySQLNode = true;
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteDatabaseSClick(TObject *Sender)
{
 AnsiString alert;
 if (b_connect)
  {
   if(DBView->Selected == MySQLNode )
    Application->MessageBox("Invalid database row selected.", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
   else if ( DBView->Selected == NULL )
    Application->MessageBox("Invalid database row selected.", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
   else
   {
     if (DBView->Selected->Text.UpperCase() == "MYSQL")
      Application->MessageBox("You cann't use this tool to drop the MySQL Database.", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
     else {
       alert = "Are you sure to drop the < ";
       alert+= DBView->Selected->Text.c_str();
       alert+= " > database.";
      if(Application->MessageBox(alert.c_str(), "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
       {
         char* lese = DBView->Selected->Text.c_str();
        if (!mysql_drop_db(MySQL, lese ))
          {
            DBView->Items->Clear();
            tree_root();
           }
         else
           Application->MessageBox("Fails to drop the Database.", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
        }
      }
   }
  }
 else
   Application->MessageBox("The Server must be connected", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
}
//---------------------------------------------------------------------------
 bool __fastcall TForm1::is_database(String Name)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  unsigned int i;
  AnsiString s_cmd;

  tree_clean();
  s_cmd = "use ";
  s_cmd+= Name.c_str();
  char* c_cmd = s_cmd.c_str();
  char* c_node = Name.c_str();
  if (mysql_query(MySQL, c_cmd ) ||
  !(res=mysql_list_tables(MySQL,"%")))
  return false;

  MySQLNodeT = TableView->Items->Add(NULL, c_node);
  MySQLNodeT->ImageIndex = 1;
  MySQLNodeT->SelectedIndex = 1;
  while ((row=mysql_fetch_row(res)) != 0) {
  mysql_field_seek(res,0);

  for (i=0 ; i < mysql_num_fields(res); i++)
   {

     MySQLTbs = TableView->Items->AddChild(MySQLNodeT, row[i]);
     MySQLTbs->ImageIndex = 2;
     MySQLTbs->SelectedIndex = 2;
   }
   MySQLNodeT->Expanded = true;
  }
  mysql_free_result(res);
  return true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::DBViewClick(TObject *Sender)
{
 if (b_connect)
 {
   if (DBView->Selected != MySQLNode && DBView->Selected != NULL  )
    is_database(DBView->Selected->Text);
   else
    tree_clean();
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TableViewClick(TObject *Sender)
{
 if (b_connect)
 {
   if (DBView->Selected != MySQLNode && DBView->Selected != NULL  )
   {
     if (DBView->Selected != MySQLNodeT )
     {
       is_table(TableView->Selected->Text);
       is_index(TableView->Selected->Text);
     }
     else
     {
      table_grid_clean();
      CleanGridI();
     }
   }
 }
}
//---------------------------------------------------------------------------
 bool __fastcall TForm1::is_table(String Name)
{
 MYSQL_RES *res, *res1;
 MYSQL_ROW row;
 unsigned int i;
 int k = 0;
 int therow = 1;
 b_new_line= true;
 AnsiString command;
 AnsiString commandt;
 AnsiString commandto;

 table_grid_clean();
 CleanGridI();
 int thecounter;

 command = "use ";
 command+= DBView->Selected->Text.c_str();
 char* las = command.c_str();
  commandto = "desc ";
  commandt = "show full columns from ";

 commandt+= Name.c_str();
 commandto+= Name.c_str();
 char* les = commandt.c_str();
 char* leso = commandto.c_str();

 if (mysql_query(MySQL, las ))
 return false;

 if (mysql_query(MySQL, les ) ||
 !(res1=mysql_store_result(MySQL)))
 {
   if (mysql_query(MySQL, leso ) ||
   !(res1=mysql_store_result(MySQL)))
     return false ;
 }
  mysql_fetch_row(res1);
  mysql_field_seek(res1,0);
  thecounter = (mysql_num_fields(res1) - 2);
  mysql_free_result(res1);

 if (mysql_query(MySQL, les ) ||
 !(res=mysql_store_result(MySQL)))
 {
   if (mysql_query(MySQL, leso ) ||
   !(res=mysql_store_result(MySQL)))
     return false ;
 }
 StringGrid4->Cells[0][0] = "Field";
 StringGrid4->Cells[1][0] = "Type";
 StringGrid4->Cells[2][0] = "Null";
 StringGrid4->Cells[3][0] = "Key";
 StringGrid4->Cells[4][0] = "Default";
 StringGrid4->Cells[5][0] = "Extra";
 StringGrid4->Cells[6][0] = "Previleges";
 
 while ((row=mysql_fetch_row(res)) != 0)
 {
   mysql_field_seek(res,0);

   for (i=0 ; i < mysql_num_fields(res); i++)
    {
     if (k <= thecounter )
     {
       StringGrid4->Cells[k][therow] = row[i];
       k++;
     }
     else
     {
       StringGrid4->Cells[(k)][therow] = row[i];
       k = 0;
       therow++ ;
       StringGrid4->RowCount++;
      }
    }

 }
 StringGrid4->RowCount--;
 mysql_free_result(res);
 return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TableViewChange(TObject *Sender, TTreeNode *Node)
{
  if (b_connect)
  {
    if (DBView->Selected != MySQLNodeT )
    {
      is_table(TableView->Selected->Text);
      is_index(TableView->Selected->Text);
    }
    else
    {
      table_grid_clean();
      CleanGridI();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DBViewChange(TObject *Sender, TTreeNode *Node)
{
 if (b_connect)
 {
   if (DBView->Selected != MySQLNode )
    is_database(DBView->Selected->Text);
   else
    tree_clean();
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RefreshSClick(TObject *Sender)
{
 MYSQL_RES *res;
 MYSQL_ROW row;
 unsigned int i;
 AnsiString command;

 if (b_connect)
  {
   b_MySQLNode = false;
   tree_clean();
   DBView->Items->Clear();

   computer_ip();

   MySQLNode = DBView->Items->Add(NULL, mainroot.UpperCase());
   MySQLNode->ImageIndex = 0;

   if (!(res=mysql_list_dbs(MySQL,"%")))  { /*do nothing;*/ }
    while ((row=mysql_fetch_row(res)) != 0) {
    mysql_field_seek(res,0);

     for (i=0 ; i < mysql_num_fields(res); i++)
      {
        MySQLDbs = DBView->Items->AddChild(MySQLNode, row[i]);
        MySQLDbs->ImageIndex = 1;
        MySQLDbs->SelectedIndex = 1;

      }

   }

   mysql_free_result(res);

 b_MySQLNode = true;

 MySQLNode->Expanded = true;

  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CreateDatabaseSClick(TObject *Sender)
{
 if (b_connect)
  {
   dbfrm->Show();

  }
 else
  Application->MessageBox("The Server must be connected", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::tree_clean(void)
{
  StringGrid4->RowCount= 2;
  StringGrid4->Cells[0][1] = "";
  StringGrid4->Cells[1][1] = "";
  StringGrid4->Cells[2][1] = "";
  StringGrid4->Cells[3][1] = "";
  StringGrid4->Cells[4][1] = "";
  StringGrid4->Cells[5][1]  = "";
  TableView->Items->Clear();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::table_grid_clean(void)
{
  StringGrid4->RowCount= 2;
  StringGrid4->Cells[0][1] = "";
  StringGrid4->Cells[1][1] = "";
  StringGrid4->Cells[2][1] = "";
  StringGrid4->Cells[3][1] = "";
  StringGrid4->Cells[4][1] = "";
  StringGrid4->Cells[5][1]  = "";
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::create_db()
{

 if (mysql_create_db(MySQL, dbfrm->Edit1->Text.c_str()))
  return true;
 else
  return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::out_refresh(void)
{
 RefreshSClick(dbfrm->SpeedButton1);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FlushHosts1Click(TObject *Sender)
{
   if (b_connect)
   {
    if (mysql_refresh(MySQL,REFRESH_HOSTS))
       StatusLine->SimpleText = "";
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FlushLogs1Click(TObject *Sender)
{
   if (b_connect)
  {
    if (mysql_refresh(MySQL,REFRESH_LOG))
     StatusLine->SimpleText = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FlushTables1Click(TObject *Sender)
{
  if (b_connect)
  {
    if (mysql_refresh(MySQL,REFRESH_TABLES))
      StatusLine->SimpleText = "";
   }
}
//---------------------------------------------------------------------------
 bool __fastcall TForm1::is_index(String Name)
{
 MYSQL_RES *res;
 MYSQL_ROW row;
 unsigned int n_field;
 int n_cell = 0;
 int n_row = 1;
 b_new_line= true;
 AnsiString s_cmd ;
 AnsiString s_cmdt;
 n_field = 0;
 CleanGridI();
 s_cmd = "use ";
 s_cmd+= DBView->Selected->Text.c_str();
 char* f_sql = s_cmd.c_str();

 s_cmdt = "show index from ";
 s_cmdt+= Name.c_str();
 char* f_sqlt = s_cmdt.c_str();

 if (mysql_query(MySQL, f_sql ))
 return false;

 if (mysql_query(MySQL, f_sqlt ) ||
 !(res=mysql_store_result(MySQL)))
 return false ;

 StringGrid3->RowCount= 2;
 StringGrid3->Cells[0][0] = "Table";
 StringGrid3->Cells[1][0] = "Non_unique";
 StringGrid3->Cells[2][0] = "Key_name";
 StringGrid3->Cells[3][0] = "Seq_in_index";
 StringGrid3->Cells[4][0] = "Col_name";
 StringGrid3->Cells[5][0] = "Collation";
 StringGrid3->Cells[6][0] = "Card.";
 StringGrid3->Cells[7][0] = "Sub_part";
 StringGrid3->Cells[8][0] = "Packed";
 StringGrid3->Cells[9][0] = "Comment";

 int n_counter;
 String s_info = server_info();
 s_info.SetLength(4);

 if ( s_info == "3.22")
  {
   StringGrid3->ColCount = 8;
   n_counter = 6;
  }
 else
  n_counter = 8;
 while ((row=mysql_fetch_row(res)) != 0)
 {
   mysql_field_seek(res,0);

   for (n_field=0 ; n_field < mysql_num_fields(res); n_field++)
    {
     if (n_cell <= n_counter )
     {
       StringGrid3->Cells[n_cell][n_row] = row[n_field];
       n_cell++;
     }
     else
     {
       StringGrid3->Cells[(n_cell)][n_row] = row[n_field];
       n_cell = 0;
       n_row++ ;
       StringGrid3->RowCount++;
      }
    }

 }
 if (n_field)
 StringGrid3->RowCount--;
 mysql_free_result(res);
 return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CleanGridI(void)
{
  StringGrid3->RowCount= 2;
  StringGrid3->Cells[0][1] = "";
  StringGrid3->Cells[1][1] = "";
  StringGrid3->Cells[2][1] = "";
  StringGrid3->Cells[3][1] = "";
  StringGrid3->Cells[4][1] = "";
  StringGrid3->Cells[5][1]  = "";
  StringGrid3->Cells[6][1]  = "";
  StringGrid3->Cells[7][1]  = "";
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::create_table(String table_name)
{

 if (!mysql_query(MySQL, table_name.c_str()))
  return true;
 else
  return false;
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::get_server_extended_status()
{
  if (!b_extended_server_status && !b_preport)
   return true;

  MYSQL_RES *res;
  MYSQL_ROW row;
  unsigned int i;
  int k = 1;
  b_new_line= true;
  bool left = true;
  bool open_tables = false;
  bool open_files = false;
  bool uptime = false;
  bool running_threads = false;
  bool open_streams = false;
  bool slow_queries = false;
  bool opened_tables = false;
  bool questions = false;

  AnsiString report;
  if (b_refresh_status)
  StringGrid5->RowCount = 2;

  if (mysql_query(MySQL,"show status") ||
	  !(res=mysql_store_result(MySQL)))
   return false;
  while ((row=mysql_fetch_row(res)) != 0)
  {
    mysql_field_seek(res,0);
    StringGrid5->Cells[0][0] = "Variable Name";
    StringGrid5->Cells[1][0] = "Value";
    for (i=0 ; i < mysql_num_fields(res); i++)
    {
      if (left)
      {
        if (b_preport)
         report = set_row(row[i]);
        if ( (String) row[i] ==  "Open_tables")
          open_tables = true;
        else
          open_tables = false;
        if ( (String) row[i] ==  "Open_files")
          open_files = true;
        else
          open_files = false;
        if ((String) row[i] == "Uptime")
         uptime = true;
        else
         uptime = false;

        if ( (String) row[i] == "Opened_tables")
          opened_tables = true;
        else
          opened_tables = false;

        if ( (String) row[i] == "Threads_running" || (String) row[i] == "Running_threads")
           running_threads = true;
        else
           running_threads = false;

        if ( (String) row[i] == "Open_streams")
           open_streams = true;
        else
           open_streams = false;
        if ( (String) row[i] == "Slow_queries")
                    slow_queries = true;
                 else
                    slow_queries = false;

                 if ( (String) row[i] == "Questions")
                    questions = true;
                 else
                    questions = false;

                 if (b_refresh_status)
                 StringGrid5->Cells[0][k++] = row[i];

                 left = false;
              }
             else
               {
                 if (b_preport)
                  Memo5->Lines->Add(report + row[i]);
                 if (open_tables)
                  st22->Text = row[i];
                 if (open_files)
                  st23->Text = row[i];
                 if (uptime)
                  nice_time(row[i]);
                 if (running_threads)
                  st27->Text = row[i];
                 if (open_streams)
                  st24->Text = row[i];
                 if (slow_queries)
                  st28->Text = row[i];
                 if (opened_tables)
                  st25->Text = row[i];
                 if (questions){
                   questions++;
                   st26->Text = StrToInt64(row[i]) - questions; }

                 if (b_refresh_status){
                 StringGrid5->RowCount++;
                 StringGrid5->Cells[1][--k] = row[i];
                 k++; }

                 left = true;
               }

          }

       }



    if (b_grid_init)
     StringGrid5->RowCount--;
    mysql_free_result(res);
    b_refresh_status = false;
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton13Click(TObject *Sender)
{
  b_refresh_status = true;
 
}
//---------------------------------------------------------------------------
String  __fastcall TForm1::server_info()
{
 String s_version  =  mysql_get_server_info(MySQL) ;
 return s_version;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::KillProcess1Click(TObject *Sender)
{

  if (b_connect)
    KillPID();
  else
    Application->MessageBox("The Server must be connected", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::KillPID()
{
  String s_warning  = "Are you sure to kill the process PID no. ";
  s_warning+= StringGrid2->Cells[0][StringGrid2->Row];
  s_warning+= " of the USER ";
  s_warning+= StringGrid2->Cells[1][StringGrid2->Row];
  unsigned long  xx = mysql_thread_id(MySQL);
  unsigned long  yy = StrToInt(StringGrid2->Cells[0][StringGrid2->Row]);
  if ( xx != yy)
   {
     if(Application->MessageBox(s_warning.c_str(), "WinMySQLAdmin 1.4", MB_YESNOCANCEL | MB_ICONQUESTION	) == IDYES)
      {
        if (!mysql_kill(MySQL,yy))
         {
          get_process();
          return true;
         }
      }
    }
  else
   {
     Application->MessageBox("From here you can't kill the PID of this tool", "WinMySQLAdmin 1.4", MB_OK | MB_ICONEXCLAMATION);
     return true;
   }
 return true;
}
void __fastcall TForm1::FlushThreads1Click(TObject *Sender)
{
 if (b_connect)
  {
    if (mysql_refresh(MySQL,REFRESH_THREADS))
      StatusLine->SimpleText = "";
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)
{
    QueryTime->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::set_query_interval(void)
{

 TIniFile *pIniFile = new
 TIniFile(win_dir() + "\\my.ini");

 AnsiString lx = pIniFile->ReadString("WinMySQLadmin","QueryInterval","")  ;
 delete pIniFile;
 if (lx.IsEmpty())
  QtimeE = 10;
 else
 {
   try
   {
     QtimeE = StrToInt(lx);
   }
   catch (EConvertError &e)
   {
    QtimeE = 10;

   }
 }
}
