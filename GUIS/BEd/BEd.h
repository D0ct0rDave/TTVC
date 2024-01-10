//---------------------------------------------------------------------------
#ifndef TBEdH
#define TBEdH
//---------------------------------------------------------------------------

/*
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "CSPIN.h"
#include "LEDScreen.hpp"
*/


#ifdef MFCS
	#include <Afxwin.h>
	#include <afxcmn.h>
#else
	#include <windows.h>
#endif

#include "controls/BMPResource.h"
#include "controls/BtnMgr.h"
#include "controls/Label.h"
//---------------------------------------------------------------------------
class BEd
{
  public:
			BEd() {};
			~BEd(){};
			static int iInit(HWND _hWnd,HINSTANCE _hInst);			
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			static void Finish();
			static bool KeyDownHandler(unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode);
			static bool KeyUpHandler  (unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode);

  private:
			static void Draw();

			// Private declarations
			static HWND				hWnd;
			static HDC				hDC;
			static HINSTANCE		hInst;

			static int				iWidth;
			static int				iHeight;

			static RECT				UpdRect;
			static CBMPResource		oShape;    
	public:
			static CBtnMgr			oBtnMgr;
	private:
			static CBMPResource		oButShape;
			static UINT				iTimerID;
			static HFONT			Font;
			
			static CLabel			oStepLabel;
			static CLabel			oAcLabel;
			static CLabel			oArLabel;
			static CLabel			oTILabel;

	private:
			// Button events
			static void BStepInc();
			static void BStepDec();

			static void BAcInc();
			static void BAcDec();

			static void BArInc();
			static void BArDec();

			static void BTIInc();
			static void BTIDec();
			
			static void BTEdPActOcInc();
			static void BTEdPActOcDec();
			static void BTEdPActNtInc();
			static void BTEdPActNtDec();
			
			static void BTEdSngOcInc();
			static void BTEdSngOcDec();
			static void BTEdSngNtInc();
			static void BTEdSngNtDec();

			static void BVEdPActInc();
			static void BVEdPActDec();
			static void BVEdSngInc();
			static void BVEdSngDec();

			static void BEdit();

    /*
	short   r;

    HDC     ScreenDC;
    short   OldX,
            OldY,
            OldLeft,
            OldTop;

    TRect   MoveRect;
    bool    Moving;
	*/

/*
  public:		// User declarations
        __fastcall TBEd(TComponent* Owner);

  public:
    // Public declarations
    bool PatronActivado;
*/
};

//---------------------------------------------------------------------------
#endif

