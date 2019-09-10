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
HINSTANCE g_hinst;
USERES("winmysqladmin.res");
USEFORM("main.cpp", Form1);
USEFORM("initsetup.cpp", Form2);
USEFORM("db.cpp", dbfrm);
USEFORM("qtime.cpp", QueryTime);
USELIB("libmysql.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->HelpFile = "C:\\mysql\\bin\\WINMYSQLADMIN.HLP";
                 Application->Title = "WinMySQLAdmin 1.3";
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TForm2), &Form2);
                 Application->CreateForm(__classid(Tdbfrm), &dbfrm);
                 Application->CreateForm(__classid(TQueryTime), &QueryTime);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
