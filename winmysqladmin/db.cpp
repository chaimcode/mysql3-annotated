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

#include "db.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tdbfrm *dbfrm;
//---------------------------------------------------------------------------
__fastcall Tdbfrm::Tdbfrm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tdbfrm::SpeedButton2Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall Tdbfrm::SpeedButton1Click(TObject *Sender)
{
 if (VerDBName())
  {
    if (!Form1->create_db())
     {
      Form1->out_refresh();
      Edit1->Text = "";
      Application->MessageBox("The database was created", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
     } 
  }





}
//---------------------------------------------------------------------------
bool __fastcall Tdbfrm::VerDBName()
{
  String temp = Edit1->Text;
 if (Edit1->Text.IsEmpty())
  {
   Application->MessageBox("The name of the Database is Empty", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
   return false;
  }

 if (temp.Length() > 64)
  {
   Application->MessageBox("The name of the Database can't have more than 64 characters ", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
   return false;
  }

 for (int j = 1; j <= temp.Length(); j++)
  {
    if (temp[j] == ' ')
     {
      Application->MessageBox("The name of the Database can't have blank spaces ", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
      return false;
     }
    else if (temp[j] == '/')
     {
      Application->MessageBox("The name of the Database can't have frontslash (/)", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
      return false;
     }
    else if (temp[j] == '\\')
     {
      Application->MessageBox("The name of the Database can't have backslash (\\)", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
      return false;
     }
    else if (temp[j] == '.')
     {
      Application->MessageBox("The name of the Database can't have periods", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
      return false;
     }
  }
 return true;
}
//---------------------------------------------------------------------------
