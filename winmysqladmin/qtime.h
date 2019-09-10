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
#ifndef qtimeH
#define qtimeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TQueryTime : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TButton *OKButton;
        TButton *CancelButton;
        TEdit *Qtime;
        void __fastcall CancelButtonClick(TObject *Sender);
        void __fastcall OKButtonClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TQueryTime(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TQueryTime *QueryTime;
//---------------------------------------------------------------------------
#endif
