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

#include "initsetup.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::BitBtn1Click(TObject *Sender)
{
 if ((Edit1->Text).IsEmpty() || (Edit2->Text).IsEmpty())
  Application->MessageBox("Fill the User name and Password text boxs ", "WinMySQLAdmin 1.4", MB_OK |MB_ICONINFORMATION);
 else
  {
    Form1->get_server_file();
    Form1->create_my_ini_file();
    Form1->create_shortcut();

    Close();
  }        
}
//---------------------------------------------------------------------------
void __fastcall TForm2::BitBtn2Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TForm2::SpeedButton1Click(TObject *Sender)
{
 Application->HelpCommand(HELP_FINDER,0);              
}
//---------------------------------------------------------------------------
