//---------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------
#include "BEd.h"
#include "Resource.h"
#include "SongStructs/Inter.h"
#include "Misc/Pistas22.h"
#include "Misc/Variables.h"
#include "GUIS/PattEd/PattEd.h"
#include "Misc/WinVars.h"

// ------------------------------------------------------------------------
// Class members
// ------------------------------------------------------------------------
HINSTANCE				BEd::hInst;
HWND					BEd::hWnd;
int						BEd::iWidth;
int						BEd::iHeight;
CBMPResource			BEd::oShape;

CBtnMgr					BEd::oBtnMgr;
CBMPResource			BEd::oButShape;
UINT					BEd::iTimerID;

HDC						BEd::hDC;
RECT					BEd::UpdRect;
CLabel					BEd::oStepLabel;
CLabel					BEd::oAcLabel;
CLabel					BEd::oArLabel;
CLabel					BEd::oTILabel;

bool					bCtrl = false;
// ------------------------------------------------------------------------
int BEd::iInit(HWND _hWnd,HINSTANCE _hInst)
{
	hWnd  = _hWnd;
	hInst = _hInst;

	oShape.Load("data/BEdShape.bmp",hWnd,hInst);
	oButShape.Load("data/BEdButShape.bmp",hWnd,hInst);
	
	oBtnMgr.Init(32);

	// Botones de salto,acorde, arpegio, transposicion de entrada
	oBtnMgr..iAddButton( 20, 50,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BStepInc,NULL,hWnd);
	oBtnMgr.iAddButton(124, 50,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BStepDec,NULL,hWnd);
	
	oBtnMgr.iAddButton( 20,114,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BAcInc,NULL,hWnd);
	oBtnMgr.iAddButton(124,114,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BAcDec,NULL,hWnd);
	
	oBtnMgr.iAddButton( 20,179,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BArInc,NULL,hWnd);
	oBtnMgr.iAddButton(124,179,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BArDec,NULL,hWnd);
	
	oBtnMgr.iAddButton( 20,243,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BTIInc,NULL,hWnd);
	oBtnMgr.iAddButton(124,243,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BTIDec,NULL,hWnd);


	// Botones de transposicion de edicion
	oBtnMgr.iAddButton(196, 50,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BTEdPActOcInc,NULL,hWnd);
	oBtnMgr.iAddButton(196, 85,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BTEdPActOcDec,NULL,hWnd);
	
	oBtnMgr.iAddButton(256, 50,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BTEdPActNtInc,NULL,hWnd);
	oBtnMgr.iAddButton(256, 85,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BTEdPActNtDec,NULL,hWnd);
	
	oBtnMgr.iAddButton(312, 50,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BTEdSngOcInc,NULL,hWnd);	
	oBtnMgr.iAddButton(312, 85,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BTEdSngOcDec,NULL,hWnd);
	
	oBtnMgr.iAddButton(372, 50,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BTEdSngNtInc,NULL,hWnd);
	oBtnMgr.iAddButton(372, 85,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BTEdSngNtDec,NULL,hWnd);


	// Botones de volumen de edicion
	oBtnMgr.iAddButton(196,173,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BVEdPActInc,NULL,hWnd);
	oBtnMgr.iAddButton(256,173,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BVEdPActDec,NULL,hWnd);
	
	oBtnMgr.iAddButton(312,173,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BVEdSngInc,NULL,hWnd);
	oBtnMgr.iAddButton(372,173,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BVEdSngDec,NULL,hWnd);

	// Activate Edition
	oBtnMgr.iAddButton(186,237,42,28,&oButShape,43,60,&oButShape,0,60,NULL,0,0,NULL,0,0,eBTSwitch,eBSReleased,0,BEdit,BEdit,hWnd);

	iTimerID = SetTimer(hWnd, 0,1000/4,NULL);

	oStepLabel.Init(hWnd,74,49,"LCDMono2",16);
	oStepLabel.SetColor(0x002A301E);
	oStepLabel.SetValue(VG.SaltoLinea);

	oAcLabel.Init(hWnd,74,113,"LCDMono2",16);
	oAcLabel.SetColor(0x002A301E);
	oAcLabel.SetValue(VG.Acorde);

	oArLabel.Init(hWnd,74,177,"LCDMono2",16);
	oArLabel.SetColor(0x002A301E);
	oArLabel.SetValue(VG.Arpegio);

	oTILabel.Init(hWnd,74,241,"LCDMono2",16);
	oTILabel.SetColor(0x002A301E);
	oTILabel.SetValue(VG.Transponer);


	/*
	RECT Area;
	Area.left  = 0;
	Area.right = oShape.iGetTX();
	Area.top   = 0;
	Area.bottom= oShape.iGetTY();
	AdjustWindowRect(&Area,WS_CHILD | WS_VISIBLE | WS_CAPTION,false);
	MoveWindow (hWnd,0,0,Area.right - Area.left,Area.bottom - Area.top,true);
	*/

	return(0);
}

LRESULT CALLBACK BEd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int				wmId, wmEvent;
	PAINTSTRUCT		ps;
	int				x,y;

	/*
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	*/

	switch (message) 
	{
		case WM_COMMAND:
		break;
			
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			UpdRect = ps.rcPaint;

			// ----------------------------------------------------
			// TODO: Add any drawing code here...
			// ----------------------------------------------------
			
			Draw();

			// ----------------------------------------------------


			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;


		/*
		#define WM_MOUSEMOVE                    0x0200
		#define WM_LBUTTONDOWN                  0x0201
		#define WM_LBUTTONUP                    0x0202
		#define WM_LBUTTONDBLCLK                0x0203
		#define WM_RBUTTONDOWN                  0x0204
		#define WM_RBUTTONUP                    0x0205
		#define WM_RBUTTONDBLCLK                0x0206
		#define WM_MBUTTONDOWN                  0x0207
		#define WM_MBUTTONUP                    0x0208
		#define WM_MBUTTONDBLCLK                0x0209
		*/

		case WM_MOUSEMOVE:
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		oBtnMgr.MouseMove(x,y);
		break;

		case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		oBtnMgr.MouseDown(x,y);
		break;

		case WM_LBUTTONUP:
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		oBtnMgr.MouseUp(x,y);
		break;

		case WM_TIMER:
		oBtnMgr.Timer();
		break;

		case WM_CLOSE:
		// Avoid closing double clicking on system menu
		break;


		// Mandamos los mensajes de teclado al gestor general de mensajes de teclado
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			SendMessage(ghMainWnd,message,wParam,lParam);
		}
		break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
//---------------------------------------------------------------------------
bool BEd::KeyDownHandler(unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode)
{
	switch (key)
	{
		// -------------------------------
		// Simulate an edit button press
		case 'E':
		case 'e':
		{
			// De momento no se me ocurre nada mejor
			if (_uiKeyFlags & KEY_CONTROL)
			{
				oBtnMgr.MouseDown(186,237);			
				return(true);
			}
		}
		break;
	}

	return(false);
}

bool BEd::KeyUpHandler  (unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode)
{
	switch (key)
	{		
		// -------------------------------
		case 'E':
		case 'e':		
		{
			if (_uiKeyFlags & KEY_CONTROL)
			{
				return(true);
			}
		}
		break;
	}
	
	return(false);
}
//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------
void __fastcall BEd::formCreate(TObject *Sender)
{
     Fondo=Image1->Picture->Bitmap;
     PatronActivado=true;
};
//------------------------------------------------------------------------------
void __fastcall BEd::LimpiarTrackAuxClick(TObject *Sender)
{
};

void __fastcall BEd::LimpiarPatternAuxClick(TObject *Sender)
{
};

void __fastcall BEd::vvt1dClick(TObject *Sender)
{
     // Reloj de arena
     Cursor=crHourGlass;
     r=CalcularVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,-1,0);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)

         {
              ModificarVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,-1,0);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
         ModificarVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,-1,0);
         PattEd::PintaTrack(PattEd::TrackActual);
     };

     Cursor=crDefault;
};

void __fastcall BEd::vvt1uClick(TObject *Sender)
{
     // Reloj de arena
     Cursor=crHourGlass;
     r=CalcularVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,1,0);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              ModificarVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,1,0);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
         ModificarVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,1,0);
         PattEd::PintaTrack(PattEd::TrackActual);;
     };

     Cursor=crDefault;
};

void __fastcall BEd::vvp1uClick(TObject *Sender)
{
     // Reloj de arena
     Cursor=crHourGlass;
     r=CalcularVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,1,0);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              ModificarVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,1,0);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
         ModificarVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,1,0);
         PattEd::PintaTrack(PattEd::TrackActual);
     };

     Cursor=crDefault;
};

void __fastcall BEd::vvp1dClick(TObject *Sender)
{
     // Reloj de arena
     Cursor=crHourGlass;
     r=CalcularVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,-1,0);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              ModificarVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,-1,0);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
         ModificarVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,-1,0);
         PattEd::PintaTrack(PattEd::TrackActual);
     };
     Cursor=crDefault;
};
//------------------------------------------------------------------------------
// Trasponer                                                                    
//------------------------------------------------------------------------------
void __fastcall BEd::TP1uClick(TObject *Sender)
{
     Cursor=crHourGlass;
     r=CalcularTrasposicionPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,1);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,1);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
          TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,1);
          PattEd::PintaTrack(PattEd::TrackActual);
     };

     Cursor=crDefault;
};

void __fastcall BEd::tp1dClick(TObject *Sender)
{
     Cursor=crHourGlass;
     r=CalcularTrasposicionPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-1);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-1);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
          TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-1);
          PattEd::PintaTrack(PattEd::TrackActual);
     };

     Cursor=crDefault;
};

void __fastcall BEd::tp12uClick(TObject *Sender)
{
     Cursor=crHourGlass;
     r=CalcularTrasposicionPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,12);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,12);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
         TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,12);
         PattEd::PintaTrack(PattEd::TrackActual);
     };

     Cursor=crDefault;
};

void __fastcall BEd::tp12dClick(TObject *Sender)
{
     Cursor=crHourGlass;
     r=CalcularTrasposicionPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-12);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-12);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
          TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-12);
          PattEd::PintaTrack(PattEd::TrackActual);
     };

     Cursor=crDefault;
};

void __fastcall BEd::tt1uClick(TObject *Sender)
{
     Cursor=crHourGlass;
     r=CalcularTrasposicionPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,1);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,1);
              PattEd::PintaTrack(PattEd::TrackActual);;
         };
     }
     else
     {
         TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,1);
         PattEd::PintaTrack(PattEd::TrackActual);
     };

     Cursor=crDefault;
};

void __fastcall BEd::ttudClick(TObject *Sender)
{
     Cursor=crHourGlass;
     r=CalcularTrasposicionPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-1);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-1);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
          TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-1);
          PattEd::PintaTrack(PattEd::TrackActual);
     };

     Cursor=crDefault;
};

void __fastcall BEd::tt12uClick(TObject *Sender)
{
     Cursor=crHourGlass;
     r=CalcularTrasposicionPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,12);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,12);
              PattEd::PintaTrack(PattEd::TrackActual);;
         };
     }
     else
     {
         TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,12);
         PattEd::PintaTrack(PattEd::TrackActual);
     };

     Cursor=crDefault;
};

void __fastcall BEd::tt12dClick(TObject *Sender)
{
     Cursor=crHourGlass;
     r=CalcularTrasposicionPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-12);

     if (r>0)
     {
         if (MessageDlg("Se van a truncar "+IntToStr(r)+" posiciones: \n"+
                        "¿Estás de acuerdo?",mtWarning,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
         {
              TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-12);
              PattEd::PintaTrack(PattEd::TrackActual);;
         };
     }
     else
     {
         TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-12);
         PattEd::PintaTrack(PattEd::TrackActual);;
     };

     Cursor=crDefault;
};

void __fastcall BEd::formShow(TObject *Sender)
{
     formPaint(Sender);
};

void __fastcall BEd::formPaint(TObject *Sender)
{
};
*/
/*
void __fastcall BEd::SLUpClick(TObject *Sender)
{

};

void __fastcall BEd::SLDownClick(TObject *Sender)
{
};

void __fastcall BEd::AcUpClick(TObject *Sender)
{
};

void __fastcall BEd::AcDownClick(TObject *Sender)
{
};

void __fastcall BEd::ArUpClick(TObject *Sender)
{
};

void __fastcall BEd::ArDownClick(TObject *Sender)
{
};


void __fastcall BEd::CSpinButton1DownClick(TObject *Sender)
{
}

void __fastcall BEd::CSpinButton1UpClick(TObject *Sender)
{
}

void __fastcall BEd::formClose(TObject *Sender,TCloseAction &Action)
{
    // WARNING
    // Main->BarradeEdicin1.Checked=false;
};
//---------------------------------------------------------------------------
void __fastcall BEd::FormKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{
    Main->FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------
void __fastcall BEd::FormKeyUp(TObject *Sender, WORD &Key,TShiftState Shift)
{
    Main->formKeyUp(Sender,Key,Shift);
}
*/
//--------------------------------------------------------------------------
void BEd::Draw()
{
	BitBlt(hDC,0,0,oShape.iGetTX(),oShape.iGetTY(),oShape.GetDC(),0,0,SRCCOPY);
	oBtnMgr.Draw();

	// Draw label
	// SetBackColor(
	
	oStepLabel.Draw(hDC);
	oAcLabel.Draw(hDC);
	oArLabel.Draw(hDC);
	oTILabel.Draw(hDC);

/*
     unsigned short       i,j,x,y;
     x = ClientWidth / Fondo->Width;
     y = ClientHeight / Fondo->Height;

     for (j=0;j<=y;j++)
         for (i=0;i<=x;i++)
             Canvas->Draw(i*Fondo->Width,j*Fondo->Height,Fondo);

     LimpiarTrackAux->Enabled   = ! TrackVacio(& VG.TrackAux);
     LimpiarPatternAux->Enabled = ! PatternVacio(VG.PatternAux);
     // LedScreen4.Cadena=ListaEfectos[Sng.Tins.T[PattEd::PTrack+PattEd::TrackActual].Disp].TomarNombreEfecto(EfectoActual);
     LEDScreen4->Cadena=ListaDeEfectos[VG.EfectoActual];
                                                                                            
     // Acordes y arpegio
     ObtenerCadenaSaltoLinea();
     ObtenerCadenaAcorde();
     ObtenerCadenaArpegio();
*/
}
//---------------------------------------------------------------------------
// Button actions
//---------------------------------------------------------------------------
void BEd::BStepInc()
{
	if (VG.SaltoLinea<MaxSaltoLinea) 
	{
		if (VG.SaltoLinea>0) 
			VG.SaltoLinea=VG.SaltoLinea*2;
		else
			VG.SaltoLinea=1;

		oStepLabel.SetValue(VG.SaltoLinea);
	};
}
//---------------------------------------------------------------------------
void BEd::BStepDec()
{
	if (VG.SaltoLinea>=1) 
	{
		if (VG.SaltoLinea==1) 
			VG.SaltoLinea=0;
		else
			VG.SaltoLinea=VG.SaltoLinea / 2;

		oStepLabel.SetValue(VG.SaltoLinea);
	};
}
//---------------------------------------------------------------------------
void BEd::BAcInc()
{
     if (VG.Acorde<MaxAcorde) 
     {
          VG.Acorde++;
          oAcLabel.SetValue(VG.Acorde);
     };
}
//---------------------------------------------------------------------------
void BEd::BAcDec()
{
     if (VG.Acorde>1)  // No debe ser menor que 1 sino daria division por 0
     {
          VG.Acorde--;
          oAcLabel.SetValue(VG.Acorde);
     };
}
//---------------------------------------------------------------------------
void BEd::BArInc()
{
     if (VG.Arpegio<MaxArpegio)
     {
          VG.Arpegio++;
          oArLabel.SetValue(VG.Arpegio);
     };
}
//---------------------------------------------------------------------------
void BEd::BArDec()
{
     if (VG.Arpegio>1)  // No debe ser menor que 1 sino daría division por 0
     {
          VG.Arpegio--;
          oArLabel.SetValue(VG.Arpegio);
     };
}
//---------------------------------------------------------------------------
void BEd::BTIInc()
{
	if (VG.Arpegio < 24)  // No debe ser menor que 1 sino daría division por 0
	{
		VG.Transponer++;
		oTILabel.SetValue(VG.Transponer);
	};
}
//---------------------------------------------------------------------------
void BEd::BTIDec()
{
	if (VG.Transponer > -24)  // No debe ser menor que 1 sino daría division por 0
    {
		VG.Transponer--;
		oTILabel.SetValue(VG.Transponer);
    };
}
//------------------------------------------------------------------------------
// Trasponer                                                                    
//------------------------------------------------------------------------------
void BEd::BTEdPActOcInc()
{
     int r=CalcularTrasposicionPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,12);

     if (r>0)
     {
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
						"¿Estás de acuerdo?",r);
		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
              TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,12);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
         TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,12);
         PattEd::PintaTrack(PattEd::TrackActual);
     };
}
//---------------------------------------------------------------------------
void BEd::BTEdPActOcDec()
{
     int r=CalcularTrasposicionPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-12);

     if (r>0)
     {
				// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
						"¿Estás de acuerdo?",r);
		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
              TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-12);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
          TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-12);
          PattEd::PintaTrack(PattEd::TrackActual);
     };
}
//---------------------------------------------------------------------------
void BEd::BTEdPActNtInc()
{
     int r=CalcularTrasposicionPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,1);

     if (r>0)
     {
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
						"¿Estás de acuerdo?",r);
		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
              TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,1);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
          TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,1);
          PattEd::PintaTrack(PattEd::TrackActual);
     };
}
//---------------------------------------------------------------------------
void BEd::BTEdPActNtDec()
{
     int r=CalcularTrasposicionPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-1);

     if (r>0)
     {
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
						"¿Estás de acuerdo?",r);
		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
              TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-1);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
          TrasponerPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats-1,-1);
          PattEd::PintaTrack(PattEd::TrackActual);
     };
}
//---------------------------------------------------------------------------
void BEd::BTEdSngOcInc()
{
     int r=CalcularTrasposicionPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,12);

     if (r>0)
     {
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
						"¿Estás de acuerdo?",r);
		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
              TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,12);
              PattEd::PintaTrack(PattEd::TrackActual);;
         };
     }
     else
     {
         TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,12);
         PattEd::PintaTrack(PattEd::TrackActual);
     };
}
//---------------------------------------------------------------------------
void BEd::BTEdSngOcDec()
{
     int r=CalcularTrasposicionPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-12);

     if (r>0)
     {
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
						"¿Estás de acuerdo?",r);
		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
              TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-12);
              PattEd::PintaTrack(PattEd::TrackActual);;
         };
     }
     else
     {
         TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-12);
         PattEd::PintaTrack(PattEd::TrackActual);;
     };
}
//---------------------------------------------------------------------------
void BEd::BTEdSngNtInc()
{
     int r=CalcularTrasposicionPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,1);

     if (r>0)
     {
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
						"¿Estás de acuerdo?",r);
		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
              TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,1);
              PattEd::PintaTrack(PattEd::TrackActual);;
         };
     }
     else
     {
         TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,1);
         PattEd::PintaTrack(PattEd::TrackActual);
     };
}
//---------------------------------------------------------------------------
void BEd::BTEdSngNtDec()
{
     int r=CalcularTrasposicionPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-1);

     if (r>0)
     {
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
						"¿Estás de acuerdo?",r);
		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
              TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-1);
              PattEd::PintaTrack(PattEd::TrackActual);
         };
     }
     else
     {
          TrasponerPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats-1,-1);
          PattEd::PintaTrack(PattEd::TrackActual);
     };
}
//------------------------------------------------------------------------------
// Volumen
//------------------------------------------------------------------------------
void BEd::BVEdPActInc()
{
	int r = CalcularVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,1,0);

	if (r>0)
	{
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
		"¿Estás de acuerdo?",r);

		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
			ModificarVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,1,0);
			PattEd::PintaTrack(PattEd::TrackActual);
		};
	}
	else
	{
		ModificarVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,1,0);
		PattEd::PintaTrack(PattEd::TrackActual);
	};
}
//---------------------------------------------------------------------------
void BEd::BVEdPActDec()
{
	int r = CalcularVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,-1,0);

	if (r>0)
	{
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
						"¿Estás de acuerdo?",r);

		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
			ModificarVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,-1,0);
			PattEd::PintaTrack(PattEd::TrackActual);
		};
	}
	else
	{
		ModificarVolumenPista(LeerTrackDePattern(VG.PatronActual,VG.AbsTrackActual),0,MaxBeats,1,-1,0);
		PattEd::PintaTrack(PattEd::TrackActual);
	};
}
//---------------------------------------------------------------------------
void BEd::BVEdSngInc()
{
	int r = CalcularVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,1,0);

	if (r>0)
	{
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,	"Se van a truncar %d posiciones: \n"
						"¿Estás de acuerdo?",r);

		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
		{
			ModificarVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,1,0);
			PattEd::PintaTrack(PattEd::TrackActual);
		};
	}
	else
	{
		ModificarVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,1,0);
		PattEd::PintaTrack(PattEd::TrackActual);
	};
}
//---------------------------------------------------------------------------
void BEd::BVEdSngDec()
{
     int r = CalcularVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,-1,0);

     if (r>0)
     {
		// Avoid bug releasing mouse button when in message box
		oBtnMgr.MouseUp(0,0);

		char szMsg[1024];
		sprintf(szMsg,"Se van a truncar %d posiciones: \n"
                      "¿Estás de acuerdo?",r);
		
		if (MessageBox(hWnd,szMsg,"The Tracker",MB_OKCANCEL) == IDOK)
        {
			ModificarVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,-1,0);
			PattEd::PintaTrack(PattEd::TrackActual);
        };
     }
     else
     {
         ModificarVolumenPistaCancion(VG.Sng,VG.AbsTrackActual,0,MaxBeats,1,-1,0);
         PattEd::PintaTrack(PattEd::TrackActual);
     };     
}
//---------------------------------------------------------------------------
void BEd::BEdit()
{
	// Accion del boton ActivarEdicion
    VG.PatronActivo=! VG.PatronActivo;
}
//---------------------------------------------------------------------------
void BEd::Finish()
{
}
//---------------------------------------------------------------------------
