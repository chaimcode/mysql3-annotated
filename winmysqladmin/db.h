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
#ifndef dbH
#define dbH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class Tdbfrm : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *Edit1;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
                bool __fastcall VerDBName();
public:		// User declarations
        __fastcall Tdbfrm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tdbfrm *dbfrm;
//---------------------------------------------------------------------------
#endif
