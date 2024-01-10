//---------------------------------------------------------------------------
#ifndef TBRepH
#define TBRepH
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
class BRep
{
  public:
			BRep() {};
			~BRep(){};
			static int iInit(HWND _hWnd,HINSTANCE _hInst);			
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			static void Finish();

			static void UpdateContents();
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
			static CLabel			oBPMLabel;

	private:
			static void ManageTimer();

			// Button events
			static void BR_Play();
			static void BR_Pause();
			static void BR_Stop();
			static void BR_PlayPattern();
			static void BR_PlayTrack();
			static void BR_Loop();
			static void BR_Reset();
			static void BR_IncBPM();
			static void BR_DecBPM();

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
        __fastcall TBRep(TComponent* Owner);

  public:
    // Public declarations
    bool PatronActivado;
*/
};

//---------------------------------------------------------------------------
#endif

