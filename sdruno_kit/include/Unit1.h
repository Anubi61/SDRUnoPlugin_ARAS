//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
private:	// User declarations
//bool returnValue;
public:		// User declarations
   virtual	__fastcall TForm1(TComponent* Owner);
   //bool __fastcall GetReturnValue();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------

extern "C" __declspec(dllexport) bool InvokeTForm1();
extern TForm1 *Form1;


#endif
