//---------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------
#include "WinVars.h"

#include "BRep.h"
#include "Resource.h"
#include "Inter.h"
//#include "Pistas22.h"
#include "Variables.h"
//#include "Unit3.h"

//---------------------------------------------------------------------------
#include "consts2.h"                                // Constantes de caracter global
#include "variables.h"                              // Variables globales
#include "inter.h"                                  // Estructura de datos cancion
#include "SongStructs/Ficheros.h"					//
#include "SongStructs/CrtError.h"
//---------------------------------------------------------------------------
#include "Misc/Variables.h"
#include "Player/tt98Player.h"
#include "Player/OMidiIn.h"
#include "Player/OMidiOut.h"
#include "SongStructs/Inter.h"
#include "Misc/Msgs.h"
// ------------------------------------------------------------------------
#include "Guis/PattEd/PattEd.h"
#include "Guis/BSeq/BSeq.h"
// ------------------------------------------------------------------------
// Class members
// ------------------------------------------------------------------------
HINSTANCE				BRep::hInst;
HWND					BRep::hWnd;
int						BRep::iWidth;
int						BRep::iHeight;
CBMPResource			BRep::oShape;

CBtnMgr					BRep::oBtnMgr;
CBMPResource			BRep::oButShape;
UINT					BRep::iTimerID;

HDC						BRep::hDC;
RECT					BRep::UpdRect;
CLabel					BRep::oBPMLabel;

int						BR_PLAY;
int						BR_STOP;
int						BR_PAUSE;
int						BR_PATTERN;
int						BR_TRACK;
int						BR_LOOP;
int						BR_RESET;
//---------------------------------------------------------------------------
typedef enum eEstadoTimer {	
	ETInactivo				= 0,
	ETReproduciendoCancion  = 1,
	ETReproduccionEnPausa   = 2,
	ETReproduciendoPista    = 3,
	ETReproduciendoPatron   = 4,
};

eEstadoTimer geEstadoTimer;
//---------------------------------------------------------------------------
int BRep::iInit(HWND _hWnd,HINSTANCE _hInst)
{
	hWnd  = _hWnd;
	hInst = _hInst;

	oShape.Load("data/BRepShape.bmp",hWnd,hInst);
	oButShape.Load("data/BRepButShape.bmp",hWnd,hInst);
	
	oBtnMgr.Init(32);

	// Botones play  pause stop
	BR_PLAY		= oBtnMgr.iAddButton( 28, 20, 80, 24,&oButShape, 80,  0,&oButShape,0,  0,NULL,0,0,NULL,0,0,eBTSwitch ,eBSReleased,1,BR_Play	,NULL,hWnd);
	BR_PAUSE	= oBtnMgr.iAddButton(118, 20, 80, 24,&oButShape, 80, 24,&oButShape,0, 24,NULL,0,0,NULL,0,0,eBTSwitch ,eBSReleased,1,BR_Pause	,NULL,hWnd);
	BR_STOP		= oBtnMgr.iAddButton( 76, 54 ,80, 24,&oButShape, 80, 48,&oButShape,0, 48,NULL,0,0,NULL,0,0,eBTOneShot,eBSReleased,1,BR_Stop	,NULL,hWnd);
	
	// Botones pattern track
	BR_PATTERN	= oBtnMgr.iAddButton( 28, 92, 80, 24,&oButShape, 80, 72,&oButShape,0, 72,NULL,0,0,NULL,0,0,eBTSwitch,eBSReleased,1,BR_PlayPattern,NULL,hWnd);
	BR_TRACK	= oBtnMgr.iAddButton(118, 92, 80, 24,&oButShape, 80, 96,&oButShape,0, 96,NULL,0,0,NULL,0,0,eBTSwitch,eBSReleased,1,BR_PlayTrack,NULL,hWnd);
	
	// Loop
	BR_LOOP		= oBtnMgr.iAddButton( 76,192, 80, 24,&oButShape, 80,120,&oButShape,0,120,NULL,0,0,NULL,0,0,eBTSwitch,eBSReleased,0,BR_Loop,NULL,hWnd);
	
	// Bpmp
	oBtnMgr.iAddButton( 36,144, 43, 28,&oButShape, 43,144,&oButShape,0,144,NULL,0,0,NULL,0,0,eBTOneShot,eBSReleased,0,BR_IncBPM,NULL,hWnd);
	oBtnMgr.iAddButton(148,144, 43, 28,&oButShape, 43,172,&oButShape,0,172,NULL,0,0,NULL,0,0,eBTOneShot,eBSReleased,0,BR_DecBPM,NULL,hWnd);

	// Reset
	BR_RESET   = oBtnMgr.iAddButton( 42,230, 16, 40,&oButShape, 16,200,&oButShape,0,200,NULL,0,0,NULL,0,0,eBTOneShot,eBSReleased,0,BR_Reset,NULL,hWnd);

	// VG.Loop	   = false;	
	geEstadoTimer  = ETInactivo;
	iTimerID	   = SetTimer(hWnd, 1,500,NULL);
	
	// Botones de transposicion de edicion
	iTimerID = SetTimer(hWnd, 0,1000/2,NULL);

	oBPMLabel.Init(hWnd,98,148,"LCDMono2",16);
	oBPMLabel.SetColor(0x002A301E);
	oBPMLabel.SetValue(VG.Sng->BPMS);

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
//---------------------------------------------------------------------------
void BRep::Finish()
{
}
//---------------------------------------------------------------------------
LRESULT CALLBACK BRep::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	int	x,y;

	/*
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	*/

	switch (message) 
	{
		
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

		case WM_CLOSE:
		// Avoid closing double clicking on system menu
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
		// oBtnMgr.Timer();
		ManageTimer();
		break;

		// Mandamos los mensajes de teclado al gestor general de mensajes de teclado
		case WM_KEYUP:
		{
			if (! KeyUpHandler(KEY_SHIFT,(int)wParam,(lParam >> 16) & 0x00ff))
				SendMessage(ghMainWnd,message,wParam,lParam);
		}
		break;

		case WM_KEYDOWN:
		{
			if (! KeyDownHandler(KEY_SHIFT,(int)wParam,(lParam >> 16) & 0x00ff))
			{
				SendMessage(ghMainWnd,message,wParam,lParam);
			}
		}
		return(0);		
		break;


		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

//---------------------------------------------------------------------------
bool BRep::KeyDownHandler(unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode)
{
	if (_uiKeyFlags & KEY_CONTROL)
	{
		switch (key)
		{
			// Boton Play
			case 'Y':
			case 'y':
			{
				oBtnMgr.PushButton(BR_PLAY);
			}
			break;

			// Boton stop
			case VK_ESCAPE:
			{
				oBtnMgr.PushButton(BR_STOP);
			}
			break;
			
			// Boton pause
			case 'E':
			case 'e':
			{
				oBtnMgr.PushButton(BR_PAUSE);
			}
			break;

			// Boton Play Pattern
			case 'P':
			case 'p':
			{
				oBtnMgr.PushButton(BR_PATTERN);
			}
			break;

			// Boton Play Track
			case 'T':
			case 't':
			{
				oBtnMgr.PushButton(BR_TRACK);
			}
			break;

			// Boton loop
			case 'O':
			case 'o':
			{
				oBtnMgr.PushButton(BR_LOOP);
			}
			break;

			// Boton reset
			case 'R':
			case 'r':
			{
				oBtnMgr.PushButton(BR_RESET);
			}
			break;
			
			default:
				return(false);
		}

		return(true);
	}

	return(false);
}

bool BRep::KeyUpHandler  (unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode)
{
	if (_uiKeyFlags & KEY_CONTROL)
	{
		switch (key)
		{
			// Boton Play
			case 'Y':
			case 'y':
			case VK_ESCAPE:
			case 'E':
			case 'e':
			case 'P':
			case 'p':
			case 'T':
			case 't':
			case 'O':
			case 'o':
			case 'R':
			case 'r':
			{				
				return(true);
			}
			break;
			
			default:
				return(false);
		}
	}

	return(false);
}
//---------------------------------------------------------------------------
void BRep::ManageTimer()
{
	unsigned short PB,BA;
	unsigned short Pattern;

     if (Reproduciendo())
     {
		  ObtenerEstadoReproduccion(VG.AbsBeatActual,Pattern,VG.IndSecActual);
	 
	      /*Hay que recalcular por cual beat vamos*/
          PB=(VG.AbsBeatActual / PattEd::NumBeats)*PattEd::NumBeats;
          if (PB+PattEd::NumBeats>MaxBeats)  PB=MaxBeats-PattEd::NumBeats;
          BA=(VG.AbsBeatActual - PB);

          if (PB!=PattEd::PrimerBeat)
          {
               PattEd::PrimerBeat=PB;
               PattEd::BeatActual=BA;
               PattEd::UpdateContents();
          }
          else
          if (BA!=PattEd::BeatActual)
          {
               PattEd::PintaLineaBeat(PattEd::BeatActual,false);
               PattEd::BeatActual = BA;
               PattEd::PintaLineaBeat(PattEd::BeatActual,true);
          }
          
		  if (gpoBSeq->iCurSeqPos() != VG.IndSecActual)
          {
				gpoBSeq->SetCurSeqPos(VG.IndSecActual);               

               // Es posible que el patron nuevo sea distinto al actual
               if (Pattern!=VG.PatternActual)
               {
                    VG.PatternActual = Pattern;
                    VG.PatronActual = VG.Sng->Pats[Pattern];
                    VG.PistaActual = &(VG.PatronActual->Tracks[VG.AbsTrackActual]);
               }

				BRep::UpdateContents();
				gpoBSeq->UpdateContents();
				PattEd::UpdateContents();
          }
     }
     else
     {
		switch (geEstadoTimer)
        {
			case ETReproduciendoCancion:
			{
				// Timer->Enabled   =false;
				// Como si se pulsara  STOP
				oBtnMgr.PushButton(BR_STOP);
				geEstadoTimer     =ETInactivo;
				MuestraMensajeScroll("Reproduccion Finalizada");
				ObtenerEstadoReproduccion(VG.AbsBeatActual,Pattern,VG.IndSecActual);
				
				BRep::UpdateContents();
				gpoBSeq->UpdateContents();
				PattEd::UpdateContents();

				ResetSistemaMIDI();
			}
			break;

			case ETReproduciendoPista:
			{
				// Timer->Enabled   =false;
				// BR_PlayTrack();   // Como si se pulsara PLAYTRACK
				oBtnMgr.PushButton(BR_STOP);

				geEstadoTimer     =ETInactivo;
				MuestraMensajeScroll("Pista finalizada.");
				ObtenerEstadoReproduccion(VG.AbsBeatActual,Pattern,VG.IndSecActual);
				
				BRep::UpdateContents();
				gpoBSeq->UpdateContents();
				PattEd::UpdateContents();

				ResetSistemaMIDI();
			}
			break;

			case ETReproduciendoPatron:
			{
				// Timer->Enabled   =false;
				oBtnMgr.PushButton(BR_STOP); 
				// BR_PlayPattern();   // Como si se pulsara PlayPattern
				geEstadoTimer     =ETInactivo;
				MuestraMensajeScroll("Patron finalizado.");
				ObtenerEstadoReproduccion(VG.AbsBeatActual,Pattern,VG.IndSecActual);

				BRep::UpdateContents();
				gpoBSeq->UpdateContents();
				PattEd::UpdateContents();

				ResetSistemaMIDI();
			}

			default:
			{
				// Caso de Seguridad, por si algun otro caso no contemplado
				// Timer->Enabled   = false;
				geEstadoTimer    = ETInactivo;
		    }

			// Cuidado!!!
			// El código que se ponga aqui se ejecuta siempre que la reproduccion esté desactivada!!!
		}
	}
}
/*
//---------------------------------------------------------------------------
void __fastcall BRep::FormKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{
    Main->FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------
void __fastcall BRep::FormKeyUp(TObject *Sender, WORD &Key,TShiftState Shift)
{
    Main->formKeyUp(Sender,Key,Shift);
}
*/
//--------------------------------------------------------------------------
void BRep::Draw()
{
	BitBlt(hDC,0,0,oShape.iGetTX(),oShape.iGetTY(),oShape.GetDC(),0,0,SRCCOPY);
	oBtnMgr.Draw();

	// Draw label
	// SetBackColor(
	oBPMLabel.Draw(hDC);
}
//---------------------------------------------------------------------------
// Button actions
//---------------------------------------------------------------------------
void BRep::BR_Play()
{
     // Accion del Boton Play
    switch (geEstadoTimer)
    {
        case ETInactivo:
        {
			EstablecerEstadoReproduccion(0,VG.IndSecActual);
			PlayCancion(VG.Sng);

			geEstadoTimer = ETReproduciendoCancion;
		}
		break;

		case ETReproduciendoCancion:
		{
			geEstadoTimer = ETInactivo;
			StopCancion();
			ResetSistemaMIDI();
		}
		break;
	}

}
//---------------------------------------------------------------------------
void BRep::BR_Pause()
{
     // Accion del Boton Pause
     switch (geEstadoTimer)
     {
        case ETReproduciendoCancion:
        {
           StopCancion();
           // Timer->Enabled=false;
           geEstadoTimer = ETReproduccionEnPausa;

           MuestraMensajeFijo("Cancion en pausa");
        }
        break;

        case ETReproduccionEnPausa:
        {
           PlayCancion(VG.Sng);
           // Timer->Enabled=true;
           geEstadoTimer = ETReproduciendoCancion;
           MuestraMensajeScroll("Continuando Reproduccion");
        }
        break;
     }
}
//---------------------------------------------------------------------------
void BRep::BR_Stop()
{
	// Accion de Boton Stop
    switch (geEstadoTimer)
    {
        case ETReproduciendoCancion:
        {
              geEstadoTimer  =ETInactivo;
              StopCancion();
              /*
			  Botones[NBReset]->Activo = true;
              Botones[NBPause]->Activo = false; // Pause off
              Botones[NBStop]->Activo  = false;
              Botones[NBPlayTrack]->Activo = true;
              Botones[NBPlayPatt]->Activo  =true;

              Botones[NBPlay]->Activo = true;
              Botones[NBPlay]->Estado = false;
              Botones[NBPlay]->Mostrar();
				
              Timer->Enabled=false;
              DesactivaDeslizamiento();
			  */
              ResetSistemaMIDI();
        }
        break;

        case ETReproduccionEnPausa :
        {
              /*
			  Botones[NBReset]->Activo = true;
              Botones[NBStop]->Activo  = false;
              Botones[NBPlayTrack]->Activo = true;
              Botones[NBPlayPatt]->Activo=true;

              Botones[NBPlay]->Activo = true;
              Botones[NBPlay]->Estado = false;
              Botones[NBPlay]->Mostrar();

              Botones[NBPause]->Activo = false;
              Botones[NBPause]->Estado = false;
              Botones[NBPause]->Mostrar();

              Timer->Enabled=false;
			  */
              geEstadoTimer  =ETInactivo;

              // DesactivaDeslizamiento();
              ResetSistemaMIDI();
        }
        break;

        case ETReproduciendoPista:
        {
            BR_PlayTrack();
        }
        break;

        case ETReproduciendoPatron:
        {
            BR_PlayPattern();
        }
        break;
    }
}
//---------------------------------------------------------------------------
void BRep::BR_PlayPattern()
{
     // Accion del Boton PlayPattern
     switch (geEstadoTimer)
     {
        case ETInactivo:
        {
            /*
			Botones[NBStop]->Activo  = true;
            Botones[NBPlay]->Activo  = false;
            Botones[NBReset]->Activo = false;
            Botones[NBPlayTrack]->Activo = false;
			*/

            EstablecerEstadoReproduccion(VG.AbsBeatActual,VG.IndSecActual);
            PlayPattern(VG.PatronActual,VG.Sng->TIns);

            // Timer->Enabled=true;
            geEstadoTimer=ETReproduciendoPatron;
            MuestraMensajeScroll ( "Reproduciendo patron "+ AnsiString ( VG.PatronActual->Nombre.CString()));
        }
        break;

        case ETReproduciendoPatron:
        {
            /*
			Botones[NBStop]->Activo  = false;
            Botones[NBPlay]->Activo  = true;
            Botones[NBReset]->Activo = true;
            Botones[NBPlayTrack]->Activo = true;
			
            // Si se pulsó STOP, todavia permaneceria encendido
            if (Botones[NBPlayPatt]->Estado)
            {
                Botones[NBPlayPatt]->Estado = false;
                Botones[NBPlayPatt]->Mostrar();
            }			
			*/

            StopPattern();
            // Timer->Enabled = false;
            geEstadoTimer    = ETInactivo;
            // MuestraMensajeFijo("Patron parado.");
        }
        break;
     }
}
//---------------------------------------------------------------------------
void BRep::BR_PlayTrack()
{
    // Accion del Boton PlayTrack
    switch (geEstadoTimer)
    {
        case ETInactivo:
        {
            /*
			Botones[NBStop]->Activo       = true;
            Botones[NBPlay]->Activo       = false;
            Botones[NBReset]->Activo      = false;
            Botones[NBPlayPatt]->Activo   = false;
			*/
            EstablecerEstadoReproduccion(VG.AbsBeatActual,VG.IndSecActual);
            PlayTrack(VG.PistaActual,VG.AbsTrackActual,&(VG.Sng->TIns->T[VG.AbsTrackActual]));

            // Timer->Enabled=true;
            geEstadoTimer = ETReproduciendoPista;
            MuestraMensajeScroll("Reproduciendo pista " + AnsiString( VG.Sng->TIns->T[VG.AbsTrackActual].Nombre.CString()) );
        }
        break;

        case ETReproduciendoPista:
        {
            /*
			Botones[NBStop]->Activo      = false;
            Botones[NBPlay]->Activo      = true;
            Botones[NBReset]->Activo     = true;
            Botones[NBPlayPatt]->Activo  = true;

            // Si se pulsó STOP, todavia permaneceria enc}ido
            if (Botones[NBPlayTrack]->Estado)
            {
                Botones[NBPlayTrack]->Estado = false;
                Botones[NBPlayTrack]->Mostrar();
            }
			*/

            StopTrack();
            // Timer->Enabled   =false;
            geEstadoTimer = ETInactivo;
            // MuestraMensajeFijo("Pista parada.");
        }
        break;
    }
}
//---------------------------------------------------------------------------
void BRep::BR_Loop()
{
	VG.Loop = !VG.Loop;
	EstablecerLooping(VG.Loop);
}
//---------------------------------------------------------------------------
void BRep::BR_Reset()
{
	// Accion del Boton Reset
	if (geEstadoTimer == ETInactivo)
	{
		ResetSistemaMIDI();
		MuestraMensajeScroll("Sistema Reseteado");
	}
}
//---------------------------------------------------------------------------
void BRep::BR_IncBPM()
{
	//Accion del boton + de los BPM'S
	if (VG.Sng->BPMS < MaxBPMS)
	{
		// Aumentamos BPMS
		VG.Sng->BPMS++;
		// Notificamos cambio
		EstablecerBeatsPerMinute(VG.Sng->BPMS);
		// Mostramos cambio en GUI
		oBPMLabel.SetValue(VG.Sng->BPMS);

		// BPMDis->Cadena = IntToStr(VG.Sng->BPMS)+" BPMS";
		MuestraMensajeFijo("Reproduccion a "+BPMDis->Cadena + "...");
	}
}
//---------------------------------------------------------------------------
void BRep::BR_DecBPM()
{
	//Accion del boton - de los BPM'S
	if (VG.Sng->BPMS>MinBPMS)
	{
		// Aumentamos BPMS
		VG.Sng->BPMS--;
		// Notificamos cambio
		EstablecerBeatsPerMinute(VG.Sng->BPMS);
		// Mostramos cambio en GUI
		oBPMLabel.SetValue(VG.Sng->BPMS);

		// BPMDis->Cadena=IntToStr(VG.Sng->BPMS)+" BPMS";
		MuestraMensajeFijo("Reproduccion a "+BPMDis->Cadena + "...");
	}
}
//---------------------------------------------------------------------------
void BRep::UpdateContents()
{
	// Mostramos cambio en GUI
	oBPMLabel.SetValue(VG.Sng->BPMS);
}
//---------------------------------------------------------------------------
