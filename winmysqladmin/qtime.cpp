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
#include "qtime.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TQueryTime *QueryTime;
//---------------------------------------------------------------------------
__fastcall TQueryTime::TQueryTime(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TQueryTime::CancelButtonClick(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TQueryTime::OKButtonClick(TObject *Sender)
{

  AnsiString str = Qtime->Text;
  try
  {
    Form1->QtimeE = StrToInt(str);
  }
  catch (EConvertError &e)
  {
    Application->MessageBox("The interval value wasn't a number." , "Error", MB_OK);
    Form1->QtimeE = 10;
    return;
  }
  WritePrivateProfileString("WinMySQLAdmin", "QueryInterval", Qtime->Text.c_str() , "my.ini");
  Form1->my_ini_load();
  Form1->Timer3->Interval = StrToInt(str) * 1000;
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TQueryTime::FormCreate(TObject *Sender)
{
 TIniFile *pIniFile = new
 TIniFile(Form1->win_dir() + "\\my.ini");

 AnsiString lx = pIniFile->ReadString("WinMySQLadmin","QueryInterval","")  ;
 delete pIniFile;
 if (!lx.IsEmpty())
 Qtime->Text = lx;
}
//---------------------------------------------------------------------------

