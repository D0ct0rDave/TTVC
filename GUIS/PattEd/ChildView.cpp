#ifdef MFC_PATTED

// ChildView.cpp : implementation of the PattEd:: class
//

#include "stdafx.h"
#include "TTVC_MFC2.h"
#include "ChildView.h"

#include "Variables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

HINSTANCE		PattEd::hInst;
HWND			PattEd::hWnd;
HDC				PattEd::hDC;

RECT			PattEd::oUpdRect;
TScrollBar*		PattEd::sbh;
TScrollBar*		PattEd::sbv;
CBMPResource	PattEd::oShape;

int				PattEd::iWidth;
int				PattEd::iHeight;
// TCanvas     *Canvas;

// Fuentes para las distintas zonas del display
HFONT			PattEd::Font;
			
// Determinan el ancho y alto en pixels de los beats
TRectangulo		PattEd::ZMalla;
TRectangulo		PattEd::ZTracks;
TRectangulo		PattEd::ZNums;

//Brushes y Pens para los distintos tipos de beats
HBRUSH			PattEd::Brush;					//
						
HPEN			PattEd::Pen[3];
HPEN			PattEd::SFPen;					// Selected field in beat pen

unsigned short	PattEd::AltoTTrack;
unsigned short	PattEd::AnchoNumB;
unsigned short	PattEd::AnchoBeat;
unsigned short	PattEd::AltoBeat;
unsigned short	PattEd::AnchoFuente;

unsigned short	PattEd::AnchoNumBeats;
unsigned short	PattEd::AltoNumTracks;
unsigned short	PattEd::TXCtes;
unsigned short	PattEd::TYCtes;

TShiftState		PattEd::Shiftado;

unsigned short	PattEd::DifXP12;
unsigned short	PattEd::DifYP12;

bool			PattEd::bCanResize = false;

unsigned short	PattEd::usFirstTX;
unsigned short	PattEd::usFirstTY;
unsigned short	PattEd::NumTracks;
unsigned short	PattEd::NumBeats;
unsigned short	PattEd::PrimerTrack;
unsigned short	PattEd::PrimerBeat;
unsigned short	PattEd::BeatActual;
unsigned short	PattEd::TrackActual;
unsigned short	PattEd::CampoActual;

unsigned short	PattEd::EstadoSel;
unsigned short	PattEd::BIniSel;
unsigned short	PattEd::BFinSel;
unsigned short	PattEd::TIniSel;
unsigned short	PattEd::TFinSel;


/////////////////////////////////////////////////////////////////////////////
// PattEd::

PattEd::PattEd()
{
}

PattEd::~PattEd()
{
}


BEGIN_MESSAGE_MAP(PattEd,CWnd )
	//{{AFX_MSG_MAP(PattEd)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PattEd:: message handlers

BOOL PattEd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	
	// To clear using correct background color
	HBRUSH hBCKBrush = CreateSolidBrush(VG.PDP.PattEdBk);

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), hBCKBrush, NULL);

	return TRUE;
}

//---------------------------------------------------------------------------
#include "consts2.h"                                // Constantes de caracter global
#include "variables.h"                              // Variables globales
#include "inter.h"                                  // Estructura de datos cancion
#include "SongStructs/Ficheros.h"					//
#include "SongStructs/CrtError.h"

// <WARNING PASAR A VC>
// #include "VinstTrack.h"                              //Establecer propiedades instrumentos
unsigned char DigitosCampos[9] = {2,1,1,1,1,8,1,1,1};
unsigned char OffsetCampos[9]  = {0,2,4,5,6,8,17,18,19};

//---------------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//---------------------------------------------------------------------------------------
#include "player/omidiin.h"
#include "player/omidiout.h"
#include "player/tt98Player.h"

// void CovertirEffectoTCKaEventoMidi(unsigned char Eff,unsigned char Val,TTrackNum T,PBufferSysEx EvEff);

// Si hubieran varios objetos PatternDisplay, lo logico seria que
// todos mantuvieran los mismos canales muteados
bool Mutes[MaxTracks];
//--------------------------------------------------------------------------------
// Miscelanea
//--------------------------------------------------------------------------------
void PattEd::BeginDraw()
{
	hDC = ::GetDC(hWnd);
	::SelectObject(hDC,Font);
}

void PattEd::EndDraw()
{
	::ReleaseDC(hWnd,hDC);
}
//--------------------------------------------------------------------------------
bool PattEd::Coords(short ix,short iy,short fx,short fy,short cx,short cy)
{
     return ( (cx>=ix) && (cx<=fx) && (cy>=iy) && (cy<=fy) );
};

TColor PattEd::GetRGB(unsigned char R,unsigned char G,unsigned char B)
{
     return ( (int(B) << 16) | (int(G) << 8) | int(R) );
};

void PattEd::Line(unsigned short cx1,unsigned short cy1,unsigned short cx2,unsigned short cy2)
{	
	POINT p = {cx1,cy1};
	MoveToEx(hDC,cx1,cy1,&p);
	LineTo(hDC,cx2,cy2);
};

void PattEd::LineaH(unsigned short cx1,unsigned short cy1,unsigned short cx2,unsigned short cy2)
{
	Line(cx1,cy1,cx2+1,cy2);
};

void PattEd::LineaV(unsigned short cx1,unsigned short cy1,unsigned short cx2,unsigned short cy2)
{
	Line(cx1,cy1,cx2,cy2+1);
};
//--------------------------------------------------------------------------------
// Valores persistentes desde la creacion del objeto hasta su destruccion
//--------------------------------------------------------------------------------
int PattEd::iInit(HWND _hWnd,HINSTANCE _hInst)
{
	unsigned char I;
	short         h;
	
	hWnd   = _hWnd;
	hInst  = _hInst;


	hDC = ::GetDC(hWnd);


	// Inicializacion de las barras de scroll
	/*
	sbh->Kind        = sbHorizontal;
	sbh->Width       = 12;
	sbh->Height      = 12;

	sbv->Kind        = sbVertical;
	sbv->Width       = 12;
	*/

	// Load Bitmap resrouce
	oShape.Load("data/PattEdShape.bmp",hWnd,hInst);

	// Creacion e inicializacion de las fuentes
	Font = CreateFont(VG.PDP.FontSize,0,
					  0,0,
					  FW_DONTCARE,	// FW_DONTCARE,
					  false,		// italic
					  false,		// underline
					  false,		// strike out
					  DEFAULT_CHARSET,
					  OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS,
					  PROOF_QUALITY, // DEFAULT_QUALITY,
					  FF_MODERN | FIXED_PITCH,
					  VG.PDP.Font.c_str());	
	
	SelectObject(hDC,Font);
	
	// FuenteTTracks->Color= VG.PDP.ColorFuenteTitulosT;
	// VG.PDP.Canvas->Font  = FuenteMalla;
	char *szDefBeatStr = "___ ...          ...";
	int  iDefBeatStrLen = strlen(szDefBeatStr);
	SIZE Size;

	if (! GetTextExtentPoint32(hDC,szDefBeatStr,iDefBeatStrLen,&Size))
	{
		Size.cx = VG.PDP.FontSize*iDefBeatStrLen;
		Size.cy = VG.PDP.FontSize;
	}

	AnchoFuente			= Size.cx / iDefBeatStrLen;
	AnchoBeat			= Size.cx + 1;
	AnchoNumB			= AnchoFuente*5 + 1;
	AltoBeat			= Size.cy;
	AltoTTrack			= AltoBeat + 2;

	TXCtes = AnchoNumB  + 32+32;	// 32+32 del marco tipo bitmap
	TYCtes = AltoTTrack + 32+32;

	// Creacion e inicializacion de los brushes y pens
	// Brush = CreateSolidBrush(VG.PDP.PattEdBk);
	SFPen = CreatePen(0,1,VG.PDP.NrmFnColor);

	/*
	for (int i=0;i<3;i++)	
	Pen[i] = CreatePen(0,1, GetRGB(VG.PDP.CRojo >> i,VG.PDP.CVerde >> i,VG.PDP.CAzul >> i) );
	*/
	/*
	VG.PDP.Canvas->Pen->Width = 1;
	VG.PDP.Canvas->Pen->Color = clRed;
	VG.PDP.Canvas->Pen->Style = psSolid;
	*/

	//--------------------------------------
	PrimerTrack= 0;
	PrimerBeat = 0;
	BeatActual = 0;
	TrackActual= 0;
	CampoActual= 0;

	if (VG.PDP.ActVar!=NULL)
	VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);

	EstadoSel  =0;
	BIniSel    =0;
	BFinSel    =0;
	TIniSel    =0;
	TFinSel    =0;

	// Valores de las propiedades 'no fisicas' de los controles
	// InicializarPropiedadesControles;

	// Tamaño y posicion fisica de los controles en pantalla
	// InicializarValores;


	bCanResize = true;
	Resize(usFirstTX,usFirstTY);

	/*
	if (! CargarCancion("Songs/TheFeverSea.TCK"))
		ShowMessage("No se ha podido cargar la cancion");
	*/

	::ReleaseDC(hWnd,hDC);
	return(1);
};
//--------------------------------------------------------------------------------
void PattEd::Finish()
{
};
//--------------------------------------------------------------------------------
// Eventos simulados
//--------------------------------------------------------------------------------
LRESULT PattEd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
		case WM_CREATE:
			LPCREATESTRUCT pCSt;
			pCSt = (LPCREATESTRUCT)lParam;
			iInit(m_hWnd,pCSt->hInstance);			
		break;
		
		case WM_COMMAND:
		
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case SC_CLOSE:
				break;

				default:
				   return ::DefWindowProc(hWnd, message, wParam, lParam);
			}
		break;

		case WM_SIZE:					
			::GetClientRect(hWnd,&oUpdRect);
			::InvalidateRect(hWnd,&oUpdRect,true);
			Resize(LOWORD(lParam),HIWORD(lParam));
		break;

		case WM_PAINT:
			
			// Pequeño bug que no mostraba partitura hasta el primer evento resize
			if (! NumBeats) 
			{
				::GetClientRect(hWnd,&oUpdRect);
				Resize(oUpdRect.right - oUpdRect.left,oUpdRect.top - oUpdRect.bottom);
			}

			hDC     = ::BeginPaint(hWnd, &ps);
			oUpdRect = ps.rcPaint;

			// ----------------------------------------------------
			// TODO: Add any drawing code here...
			// ----------------------------------------------------
			
			Draw();

			// ----------------------------------------------------


			::EndPaint(hWnd, &ps);
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
		break;

		case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		MouseDown(mbLeft,x,y);
		break;
	
		case WM_LBUTTONUP:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		break;

		case WM_RBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		MouseDown(mbRight,x,y);
		break;

		case WM_RBUTTONUP:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		break;

		case WM_TIMER:
		break;

		// ------------------------------------------------
		static bool bShiftPressed;
		case WM_KEYDOWN:
			if ((int) wParam == VK_SHIFT) bShiftPressed = true;
			KeyDownHandler(bShiftPressed,(int) wParam);
		break;

		case WM_KEYUP:
			if ((int) wParam == VK_SHIFT) bShiftPressed = false;
			KeyUpHandler(bShiftPressed,(int) wParam);
		break;
		// ------------------------------------------------

		default:
			return CWnd::WindowProc(message, wParam, lParam);
	}

	return(0);
}
//---------------------------------------------------------------------------------------
void PattEd::KeyDownHandler(bool Shift,unsigned short key)
{
    if (KeyDown_ViewerHandler(Shift,key)) return;
	if (KeyDown_PianoHandler (Shift,key)) return;
	if (KeyDown_EditionHandler(Shift,key)) return;
}

void PattEd::KeyUpHandler(bool Shift,unsigned short key)
{
    // if (KeyDown_ViewerHandler(Shift,key)) return;
	if (KeyUp_PianoHandler (Shift,key)) return;
	// if (KeyDown_EditionHandler(Shift,key)) return;	
}
//---------------------------------------------------------------------------------------
/*
    RSIMidiIn(  ldDisp:HMIDIIN;
                Msj:unsigned short;
                dir:Dunsigned short;
                Mensaje:Dunsigned short;
                Tiempo:Dunsigned short);

    EventoMidi.Cmd   = Lounsigned char(Lounsigned short(Mensaje));
                        $80:Note off
                        $90:Note on

    EventoMidi.Data1 = Hiunsigned char(Lounsigned short(Mensaje));
    EventoMidi.Data2 = Lounsigned char(Hiunsigned short(Mensaje));
    EventoMidi.Sysex = NULL;
    EventoMidi.TamSysex = 0;
*/

bool ActiveKey[100];
//---------------------------------------------------------------------------------------
unsigned char TECLA2NOTA[256] =
{
// ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' '
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	// 0..15

// ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' '
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	// 16..31

// ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' '
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	// 32..47

// '0' '1' '2' '3' '4' '5' '6' '7' '8' '9' 
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	// 48..63

// ' ' 'A' 'B' 'C' 'D' 'E' 'F' 'G' 'H' 'I' 'J' 'K' 'L' 'M' 'N' 'O'
	0,  0, 43, 40, 39,  0,  0, 42, 44,  0, 46,  0,  0, 47, 45,  0,	// 64..79

// 'P' 'Q' 'R' 'S' 'T' 'U' 'V' 'W' 'X' 'Y' 'Z' ' ' ' ' ' ' ' ' ' '
	0,  0,  0, 37,  0,  0, 41,  0, 38,  0, 36,  0,  0,  0,  0,  0,	// 80..95

// ' ' 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o'
	0,  0, 43, 40, 39,  0,  0, 42, 44,  0, 46,  0,  0, 47, 45,  0,	// 96..111

// 'p' 'q' 'r' 's' 't' 'u' 'v' 'w' 'x' 'y' 'z' ' ' ' ' ' ' ' ' ' '
	0,  0,  0, 37,  0,  0, 41,  0, 38,  0, 36,  0,  0,  0,  0,  0,	// 112..127
};

//---------------------------------------------------------------------------------------
unsigned int MIDIMessage(unsigned char Event,unsigned char Data0,unsigned char Data1)
{
    return( int(Event) | (int(Data0) << 8) | (int(Data1) << 16) );
}
//---------------------------------------------------------------------------------------
bool PattEd::KeyDown_PianoHandler  (bool Shift,unsigned short key)
{
	unsigned char Nota = TECLA2NOTA[ key & 0x00ff ];
    if (Nota==0) return(false);

    if (! ActiveKey[Nota])
    {
		ActiveKey[Nota] = true;
        RSIMidiIn(0,MIM_DATA,0,MIDIMessage(0x90,Nota,VG.Sng->TIns->T[VG.AbsTrackActual].VolDef),0);
    }

	return(true);
}
//---------------------------------------------------------------------------------------
bool PattEd::KeyUp_PianoHandler(bool Shift,unsigned short key)
{
    unsigned char Nota = TECLA2NOTA[ key & 0x00ff ];
	if (Nota==0) return(false);

    if (ActiveKey[Nota])
    {
        RSIMidiIn(0,MIM_DATA,0,MIDIMessage(0x80,Nota,0),0);
        ActiveKey[Nota] = false;
    }
}
//---------------------------------------------------------------------------------------
bool PattEd::KeyDown_ViewerHandler(bool Shift,unsigned short key)
{
    switch (key)
    {
	    case VK_UP:
        {
			if (BeatActual>0)
			{
			   PintaLineaBeat(BeatActual,false);
			   BeatActual--;
			   if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
			   PintaLineaBeat(BeatActual,true);
			}
			else
			if (PrimerBeat>0)
			{
			   PrimerBeat=PrimerBeat-1;
			   if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
			   MostrarPartitura();
			};

			// <WARNING: VC>
			// sbv->Position = PrimerBeat;
			return(true);
        };
        break;
        case VK_DOWN:
        {
			if (BeatActual<NumBeats-1)
			{
				PintaLineaBeat(BeatActual,false);
				BeatActual++;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				PintaLineaBeat(BeatActual,true);
			}
			else
			if (PrimerBeat+NumBeats<MaxBeats)
			{
				PrimerBeat++;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				MostrarPartitura();
			};
			// <WARNING: VC>
			// sbv->Position=PrimerBeat;
			return(true);
        };
        break;
        case VK_LEFT:
        {
			// Moverse por los campos del beat
			if (CampoActual>0)
			{
				CampoActual--;
				PintaBeat(TrackActual,BeatActual,1);
			};
			return(true);
        };
        break;
        case VK_RIGHT:
        {
			// Moverse por los campos del beat
			if (CampoActual<8)
			{
				CampoActual++;
				PintaBeat(TrackActual,BeatActual,1);
			};
			return(true);
        };
        break;

        case VK_HOME:
        {
			if (BeatActual>0)
			{
				PintaLineaBeat(BeatActual,false);
				BeatActual = 0;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				PintaLineaBeat(BeatActual,true);
			};
			return(true);
        };
        break;

        // WARNING -> Is this correct ???
        case '}':
        {
			if (BeatActual<NumBeats-1)
			{
				PintaLineaBeat(BeatActual,false);
				BeatActual = NumBeats-1;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				PintaLineaBeat(BeatActual,true);
			};
			return(true);
		};
        break;

        case VK_TAB:
        case 220:
        {   // º \ ª key
			// Mirar el estado del shift
			if (! Shift)
			{
				if (TrackActual<NumTracks-1)
				{
					 TrackActual++;
					 MostrarPartitura();
				}
				else
				if (PrimerTrack+NumTracks<MaxTracks)
				{
					 PrimerTrack++;
					 MostrarPartitura();
				};
			}
			else
			{
				if (TrackActual>0)
				{
					 TrackActual--;
					 MostrarPartitura();
				}
				else
				if (PrimerTrack>0)
				{
					 PrimerTrack--;
					 MostrarPartitura();
				};
			};

			if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
			// <WARNING: VC>
			// sbh->Position   = PrimerTrack;
			return(true);
        };
        break;

        case VK_NEXT:
        {
			//    Ultimo beat visible + NumBeats
			if ((PrimerBeat+NumBeats-1)+NumBeats<MaxBeats)
			{
				PrimerBeat=PrimerBeat+NumBeats;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				MostrarPartitura();
			}
			else
			{
				if (PrimerBeat+NumBeats==MaxBeats)
				{
					// El tope está alcanzado y solo cabe
					// la posibilidad de desplazar el beat actual
					if (BeatActual<NumBeats-1)
					{
						PintaLineaBeat(BeatActual,false);
						BeatActual=NumBeats-1;
						if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
						PintaLineaBeat(BeatActual,true);
					};
				}
				else
				{
					// No se ha alcanzado el tope
					PrimerBeat=MaxBeats-NumBeats;
					if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
					MostrarPartitura();
				};
			};

			// <WARNING: VC>
			// sbv->Position=PrimerBeat;
			return(true);
        };
        break;
        case VK_PRIOR:
        {
			if ((int(PrimerBeat)-int(NumBeats)) > 0)
			{
				PrimerBeat=PrimerBeat-NumBeats;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				MostrarPartitura();
			}
			else
			{
				if (PrimerBeat>0)
				{
					PrimerBeat=0;
					if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
					MostrarPartitura();
				}
				else
				if (BeatActual>0)
				{
					PintaLineaBeat(BeatActual,false);
					BeatActual=0;
					if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
					PintaLineaBeat(BeatActual,true);
				};
			};
			// <WARNING: VC> 
			// sbv->Position=PrimerBeat;
			return(true);
        };
        break;
               // Otras teclas con funciones para el pattern
        case 73:
        case 105:
        {
			// I
			// Posicionar el cursor al inicio del patrón
			if (PrimerBeat>0)
			{
				PrimerBeat = 0;
				BeatActual = 0;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				MostrarPartitura();
			}
			else
			if (BeatActual>0)
			{
				PintaLineaBeat(BeatActual,false);
				BeatActual=0;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				PintaLineaBeat(BeatActual,true);
			};
			return(true);
        };
        break;

        case 70:
        case 102:
        {
			// F
			// Posicionar el cursor al final del patrón
			if (PrimerBeat+NumBeats<MaxBeats)
			{
				PrimerBeat=MaxBeats-NumBeats;
				BeatActual=0;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				MostrarPartitura();
			}
			else
			if (BeatActual>0)
			{
				PintaLineaBeat(BeatActual,false);
				BeatActual=0;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				PintaLineaBeat(BeatActual,true);
			};
			return(true);
        };
        break;

        default:
			return(false);
    };
};
//---------------------------------------------------------------------------------------
bool PattEd::KeyDown_EditionHandler(bool Shift,unsigned short key)
{
    unsigned short A;
    unsigned char d,Aux;
    bool Ac;
    unsigned char Track,Beat,Campo;

    Track = VG.AbsTrackActual;
    Beat  = VG.AbsBeatActual;
    Campo = CampoActual;

    /*
    __ _ _ _ _ _______ _ _ _
    c# 8 1 2 7 Efecto1 1 2 7
    0  1 2 3 4    5    6 7 8
    */

    Ac=false;  // Ac-tualizar puntero
    switch (key)
    {
        case VK_F1:
        {
            /*
			VG.EfectoActual=LeerEffBeat(VG.PistaActual,Beat);
            if (BarradeEdicin1->Checked)
                // BarradeEdicin1->LedScreen4.Cadena=ListaEfectos[Sng.Tins.T[Track].Disp].TomarNombreEfecto(EfectoActual);
                BEd->LEDScreen4->Cadena=ListaDeEfectos[VG.EfectoActual];
			*/
        }
        break;

        case VK_F2:
        {
            EscribirEffBeat(VG.PistaActual,Beat,VG.EfectoActual);
            Ac=true;
        }
        break;

        // case 189:
        case 109:
        {
            //Tecla -
            switch (Campo)
            {
                case 0:
                {
                    Aux=LeerNotaBeat(VG.PistaActual,Beat);
                    if (Aux!=255)
                    {
                        if (Aux>0)  Aux=Aux-1;
                        EscribirNotaBeat(VG.PistaActual,Beat,Aux);
                    }
                }
                break;

                case 1:
                {
                    Aux=LeerNotaBeat(VG.PistaActual,Beat);
                    if (Aux!=255)
                    {
                        if (Aux>11)  Aux=Aux-12;
                        EscribirNotaBeat(VG.PistaActual,Beat,Aux);
                    }
                }
                break;

                case 2:
                {
                    Aux=LeerNotaBeat(VG.PistaActual,Beat);
                    if (Aux!=255)
                    {
                         Aux=LeerVolBeat(VG.PistaActual,Beat);
                         if (Aux>99)  Aux=Aux-100;
                         EscribirVolBeat(VG.PistaActual,Beat,Aux);
                    }
                }
                break;

                case 3:
                {
                    Aux=LeerNotaBeat(VG.PistaActual,Beat);
                    if (Aux!=255)
                    {
                        Aux=LeerVolBeat(VG.PistaActual,Beat);
                        if (Aux>9)  Aux=Aux-10;
                        EscribirVolBeat(VG.PistaActual,Beat,Aux);
                    }
                }
                break;

                case 4:
                {
                    Aux=LeerNotaBeat(VG.PistaActual,Beat);
                    if (Aux!=255)
                    {
                        Aux=LeerVolBeat(VG.PistaActual,Beat);
                        if (Aux>0)  Aux=Aux-1;
                            EscribirVolBeat(VG.PistaActual,Beat,Aux);
                    }
                }
                break;
       case 5:{ //Efecto
                                 Aux=LeerEffBeat(VG.PistaActual,Beat);
                                 if (Aux>0)  Aux=Aux-1;

                                 //Si llega a 0 puede que se deba eliminar
                                 if (Aux=0)
                                    if (LeerNotaBeat(VG.PistaActual,Beat)==255)
                                       EliminarEventoTrack(VG.PistaActual,Beat);

                                 EscribirEffBeat(VG.PistaActual,Beat,Aux);
                           }
                         break;
       case 6:{
                                Aux=LeerEffBeat(VG.PistaActual,Beat);
                                if (Aux!=0)
                                {
                                     Aux=LeerValBeat(VG.PistaActual,Beat);
                                     if (Aux>99)  Aux=Aux-100;
                                     EscribirValBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         break;
       case 7:{
                                Aux=LeerEffBeat(VG.PistaActual,Beat);
                                if (Aux!=0)
                                {
                                     Aux=LeerValBeat(VG.PistaActual,Beat);
                                     if (Aux>9)  Aux=Aux-10;
                                     EscribirValBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         break;
       case 8:{
                                Aux=LeerEffBeat(VG.PistaActual,Beat);
                                if (Aux!=0)
                                {
                                     Aux=LeerValBeat(VG.PistaActual,Beat);
                                     if (Aux>0)  Aux=Aux-1;
                                     EscribirValBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         }

                         PintaBeat(TrackActual,BeatActual,1);
                    }
            break;
       // case 187:
       case 107:{
                         //Tecla +
                         switch (Campo)
                         {

       case 0:{
                                Aux=LeerNotaBeat(VG.PistaActual,Beat);
                                if (Aux!=255)
                                {
                                     if (Aux<94)  Aux=Aux+1;
                                     EscribirNotaBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         break;
       case 1:{
                                Aux=LeerNotaBeat(VG.PistaActual,Beat);
                                if (Aux!=255)
                                {
                                     if (Aux<94-12)  Aux=Aux+12;
                                     EscribirNotaBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         break;
       case 2:{
                                Aux=LeerNotaBeat(VG.PistaActual,Beat);
                                if (Aux!=255)
                                {
                                     Aux=LeerVolBeat(VG.PistaActual,Beat);
                                     if (Aux<28)  Aux=Aux+100;
                                     EscribirVolBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         break;
       case 3:{
                                Aux=LeerNotaBeat(VG.PistaActual,Beat);
                                if (Aux!=255)
                                {

                                     Aux=LeerVolBeat(VG.PistaActual,Beat);
                                     if (Aux<118)  Aux=Aux+10;
                                     EscribirVolBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         break;
       case 4:{
                                Aux=LeerNotaBeat(VG.PistaActual,Beat);
                                if (Aux!=255)
                                {
                                     Aux=LeerVolBeat(VG.PistaActual,Beat);
                                     if (Aux<127)  Aux=Aux+1;
                                     EscribirVolBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         break;
       case 5:{//Campo Efecto
                                Aux=LeerEffBeat(VG.PistaActual,Beat);
                                if (Aux<127)  Aux=Aux+1;
                                EscribirEffBeat(VG.PistaActual,Beat,Aux);
                           }
                         break;
       case 6:{
                                Aux=LeerEffBeat(VG.PistaActual,Beat);
                                if (Aux!=0)
                                {
                                     Aux=LeerValBeat(VG.PistaActual,Beat);
                                     if (Aux<28)  Aux=Aux+100;
                                     EscribirValBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         break;
       case 7:{
                                Aux=LeerEffBeat(VG.PistaActual,Beat);
                                if (Aux!=0)
                                {
                                     Aux=LeerValBeat(VG.PistaActual,Beat);
                                     if (Aux<118)  Aux=Aux+10;
                                     EscribirValBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         break;
       case 8:{
                                Aux=LeerEffBeat(VG.PistaActual,Beat);
                                if (Aux!=0)
                                {
                                     Aux=LeerValBeat(VG.PistaActual,Beat);
                                     if (Aux<127)  Aux=Aux+1;
                                     EscribirValBeat(VG.PistaActual,Beat,Aux);
                                }
                           }
                         }
                         PintaBeat(TrackActual,BeatActual,1);
                    }

            break;
       case '0':
       case '1':
       case '2':
       case '3':
       case '4':
       case '5':
       case '6':
       case '7':
       case '8':
       case '9':
       case VK_NUMPAD0:
       case VK_NUMPAD1:
       case VK_NUMPAD2:
       case VK_NUMPAD3:
       case VK_NUMPAD4:
       case VK_NUMPAD5:
       case VK_NUMPAD6:
       case VK_NUMPAD7:
       case VK_NUMPAD8:
       case VK_NUMPAD9:
       {
                         // Obtencion del digito pulsado
                         if (key<58)
                          d = key-'0';
                         else
                          d = key-VK_NUMPAD0;

                         switch (Campo)
                         {
       case   1:{
                                Aux=LeerNotaBeat(VG.PistaActual,Beat);
                                if (Aux!=255)
                                {
                                     if (d<8)
                                     {
                                          Aux=LeerNotaBeat(VG.PistaActual,Beat);
                                          Aux=(Aux % 12)+(d*12);
                                          EscribirNotaBeat(VG.PistaActual,Beat,Aux);
                                          Ac=true;
                                     }
                                }
                           }
                         break;
       case 2:{
                                Aux=LeerNotaBeat(VG.PistaActual,Beat);
                                if (Aux!=255)
                                {
                                     if (d<2)
                                     {
                                          Aux=LeerVolBeat(VG.PistaActual,Beat);
                                          Aux=(Aux % 100)+(d*100);
                                          if (Aux>127)  Aux=127;
                                          EscribirVolBeat(VG.PistaActual,Beat,Aux);
                                          Ac=true;
                                     }
                                }
                           }
                         break;
       case 3:{
                                Aux=LeerNotaBeat(VG.PistaActual,Beat);
                                if (Aux!=255)
                                {
                                     A=LeerVolBeat(VG.PistaActual,Beat);
                                     A=(A / 100)*100 + d*10+ (A % 10);
                                     if (A>127)  Aux=127; else Aux=A;
                                     EscribirVolBeat(VG.PistaActual,Beat,Aux);
                                     Ac=true;
                                }
                           }
                         break;
       case 4:{
                                Aux=LeerNotaBeat(VG.PistaActual,Beat);
                                if (Aux!=255)
                                {
                                     Aux=LeerVolBeat(VG.PistaActual,Beat);
                                     Aux=Aux - (Aux % 10)+d;
                                     if (Aux>127)  Aux=127;
                                     EscribirVolBeat(VG.PistaActual,Beat,Aux);
                                     Ac=true;
                                }
                           }
                         break;
       case 6:{
                                Aux=LeerEffBeat(VG.PistaActual,Beat);
                                if (Aux!=0)
                                {
                                     if (d<2)
                                     {
                                          Aux=LeerValBeat(VG.PistaActual,Beat);
                                          Aux=(Aux % 100)+(d*100);
                                          if (Aux>127)  Aux=127;
                                          EscribirValBeat(VG.PistaActual,Beat,Aux);
                                          Ac=true;
                                     }
                                }
                           }
                         break;
       case 7:{
                                Aux=LeerEffBeat(VG.PistaActual,Beat);
                                if (Aux!=0)
                                {
                                     A=LeerValBeat(VG.PistaActual,Beat);
                                     A=(A / 100)*100 + d*10+ (A % 10);
                                     if (A>127)  Aux=127; else Aux=A;
                                     EscribirValBeat(VG.PistaActual,Beat,Aux);
                                     Ac=true;
                                }
                           }
                         break;
       case 8:{
                                Aux=LeerEffBeat(VG.PistaActual,Beat);
                                if (Aux!=0)
                                {
                                     Aux=LeerValBeat(VG.PistaActual,Beat);
                                     Aux=Aux - (Aux % 10)+d;
                                     if (Aux>127)  Aux=127;
                                     EscribirValBeat(VG.PistaActual,Beat,Aux);
                                     Ac=true;
                                }
                           }
                         }
                     }
                         //Hay que actualizar el puntero de posicion
            break;
       case VK_SPACE :{
                           if (EstaEventoTrack(VG.PistaActual,Beat))
                           {
                                switch (Campo)
                                {
                                  case 0:
                                  case 1:
                                  case 2:
                                  case 3:
                                  case 4:
                                          if (LeerEffBeat(VG.PistaActual,Beat)==0)
                                             EliminarEventoTrack(VG.PistaActual,Beat);
                                          else
                                          {
                                               EscribirNotaBeat(VG.PistaActual,Beat,255);
                                               EscribirVolBeat(VG.PistaActual,Beat,0);
                                          }
                                          break;
                                  default:
                                  {
                                          if (LeerNotaBeat(VG.PistaActual,Beat)==255)
                                             EliminarEventoTrack(VG.PistaActual,Beat);
                                          else
                                          {
                                               EscribirEffBeat(VG.PistaActual,Beat,0);
                                               EscribirValBeat(VG.PistaActual,Beat,0);
                                          }
                                  }
                                }
                           }

                           Ac=true;
                      }
            break;
       case VK_DELETE:{
                           EliminarEventoTrack(VG.PistaActual,Beat);
                           Ac=true;
                      }
            break;
       case VK_BACK:{
                         if (BeatActual>0)
                         {
                              BorrarPosicionTrack(VG.PistaActual,Beat-1);

                              PintaLineaBeat(BeatActual,false);//Limpia linea actual
                              BeatActual=BeatActual-1;
                              VG.AbsBeatActual = PrimerBeat+BeatActual;
                              PintaTrack(TrackActual);         //Muestra el track
                              PintaLineaBeat(BeatActual,true); //Pinta la linea actual
                         }
                         else
                         {
                              if (PrimerBeat>0)
                              {
                                   BorrarPosicionTrack(VG.PistaActual,Beat-1);

                                   PrimerBeat=PrimerBeat-1;
                                   VG.AbsBeatActual = PrimerBeat+BeatActual;
                                   ActualizaPantalla();
                              }
                         }
                    }
            break;
       case VK_INSERT:
       {
                           InsertarPosicionTrack(VG.PistaActual,Beat);
                           PintaTrack(TrackActual);
       }
       break;

       case VK_RETURN:
       {
            Ac=true;
       }
       break;

	   default:
	   {
	   /*
            switch (key)
            {
                case 65:
                case 97:{	// A
                                  //Boton des-activar edicion
                                  Botones[NBActivaED]->Estado = ! Botones[NBActivaED]->Estado;Botones[NBActivaED]->Mostrar();
                                  AccionBoton9();
                }
                break;

                case 76:
                case 108:{//L
                                  //Botón de Loop
                                  Botones[NBLoop]->Estado = ! Botones[NBLoop]->Estado;Botones[NBLoop]->Mostrar();
                                  AccionBoton8();
                             }
                break;

                case 82:
                case 114:
                {           // R
                                  //Reset
                                  AccionBoton0();
                }
                break;

                case 80:
                case 112:{//P
                                  //Play/Stop Pattern
                                  Botones[NBPlayPatt]->Estado = ! Botones[NBPlayPatt]->Estado;Botones[NBPlayPatt]->Mostrar();
                                  AccionBoton5();
                             }
                break;

                case 84:
                case 116:{//T
                                  //Play/Stop Track
                                  Botones[NBPlayTrack]->Estado = ! Botones[NBPlayTrack]->Estado;Botones[NBPlayTrack]->Mostrar();
                                  AccionBoton4();
                             }
				*/

                /*
                                  case 83:
                                  case 115:
                                  {   // S
                                      // Boton Stop
                                      AccionBoton2;
                                  }
                */

				/*
                break;

                case 89:
                case 121:
                {   // Y
                    // Boton Play
                    Botones[NBPlay]->Estado = ! Botones[NBPlay]->Estado;Botones[NBPlay]->Mostrar();
                    AccionBoton1();
                }
                break;

                case 69:
                case 101:
                {
                    // E
                    // Boton Pause
                    if (Botones[NBPause]->Activo)
                    {
                        Botones[NBPause]->Estado = ! Botones[NBPause]->Estado;Botones[NBPause]->Mostrar();
                        AccionBoton3();
                    }
                }
            }   // END switch
			*/

       }	// END default
    };	// END switch

    if (Ac)
    {
        BeatActual=BeatActual+VG.SaltoLinea;
        VG.AbsBeatActual = PrimerBeat+BeatActual;

        if (BeatActual>=NumBeats)
        {
             BeatActual=BeatActual-VG.SaltoLinea;
             VG.AbsBeatActual = PrimerBeat+BeatActual;

             if ((PrimerBeat+NumBeats-1)+VG.SaltoLinea<MaxBeats)
             {
                  PrimerBeat=PrimerBeat+VG.SaltoLinea;
                  VG.AbsBeatActual = PrimerBeat+BeatActual;
                  ActualizaPantalla();
             }
             else
             {
                  if (PrimerBeat+NumBeats<MaxBeats)
                  {
                       /*No se ha alcanzado el tope*/
                       Aux=PrimerBeat+BeatActual;
                       PrimerBeat=MaxBeats-NumBeats;
                       BeatActual=Aux-PrimerBeat;
                       VG.AbsBeatActual = PrimerBeat+BeatActual;
                       ActualizaPantalla();
                  }
                  else
                  {
                       PintaLineaBeat(BeatActual,true);
                  }
             }
        }
        else
        {
             PintaLineaBeat(BeatActual-VG.SaltoLinea,false);
             PintaLineaBeat(BeatActual,true);
        }
    }

	return(true);
}
//--------------------------------------------------------------------------------
bool PattEd::MouseDown(TMouseButton Button,short X,short Y)
{
     if (Coords(ZMalla.cx,ZMalla.cy,ZMalla.fx,ZMalla.fy,X,Y))
     {
          ZMallaMouseDown(Button,X-ZMalla.cx,Y-ZMalla.cy);
          return(true);
     }
     else
     if (Coords(ZTracks.cx,ZTracks.cy,ZTracks.fx,ZTracks.fy,X,Y))
     {
          ZTracksMouseDown(Button,X-ZTracks.cx,Y-ZTracks.cy);
          return(true);
     }
     else
         return(false);
};
//------------------------------------------------------------------------------
void PattEd::ZTracksMouseDown(TMouseButton Button,short X, short Y)
{
   unsigned short Track;

     // Responde a eventos cuando se ha pulsado sobre un titulo de Track
     Track = X / AnchoBeat;

     if (Button == mbLeft)
     {
          // Poner una marca para que se vea que se esta editando algo en ese track
          
		  // <WARNING: VC>
		  // RellenarDatosInstrumentoTrack(VG.Sng->TIns->T[PrimerTrack+Track],PrimerTrack+Track);
		  // AbrirDialogo();
     }
     else
         if (&(VG.PDP.ProcMute)!=NULL) 
         {
              Mutes[PrimerTrack+Track]=! Mutes[PrimerTrack+Track];
              VG.PDP.ProcMute(PrimerTrack+Track,Mutes[PrimerTrack+Track]);

              //Mostrar el track muteado (o desmuteado)
              PintaNumeracionTracks();
         };
};
//------------------------------------------------------------------------------
void PattEd::ZMallaMouseDown(TMouseButton Button,short X, short Y)
{
	unsigned short NT;
	unsigned short NB;

	//Calcular coordenadas
	NT=(X / AnchoBeat);
	NB=(Y / AltoBeat);

	//Primero vemos cual es el boton apretado
	if (Button==mbLeft) 
	{
		if (NT==TrackActual) 
		{
			if (NB!=BeatActual) 
			{
				PintaLineaBeat(BeatActual,false);
				BeatActual  = NB;
				if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
				PintaLineaBeat(BeatActual,true);
			};
		}
		else
		{
			// Asi eliminamos el beat seleccionado
			PintaLineaBeat(BeatActual,false);
			TrackActual = NT;
			BeatActual  = NB;
			if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
			PintaLineaBeat(BeatActual,true);
		};

		BeatClick(TrackActual,BeatActual);
		if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
	}
	else
	{
		// Comenzar la seleccion
		NT = PrimerTrack + NT;
		NB = PrimerBeat + NB;

		switch (EstadoSel)
		{
			case 0:
			{
				TIniSel  = NT;
				BIniSel  = NB;
				EstadoSel= 1;
			};
			break;

			case 1:
			{
				if (NT<TIniSel)
				{
					TFinSel=TIniSel;
					TIniSel=NT;
				}
				else
					TFinSel=NT;

				if (NB<BIniSel)
				{
					BFinSel=BIniSel;
					BIniSel=NB;
				}
				else
					BFinSel=NB;

				EstadoSel=2;
				MostrarPartitura();
			};
			break;

			case 2:
			{
				EstadoSel= 1;
				TIniSel  = NT;
				BIniSel  = NB;
				MostrarPartitura();
			};
			break;
		};
	};
};

void PattEd::sbvScroll(TScrollCode ScrollCode)
{
	// <WARNING: VC>
     /*
	 if ((ScrollCode == scEndScroll) && (sbv->Position!=PrimerBeat))
     {
          PrimerBeat       = sbv->Position;
          if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
          MostrarPartitura();
     };
	 */
};

void PattEd::sbhScroll(TScrollCode ScrollCode)
{
	// <WARNING: VC>
	/*
     if ((ScrollCode == scEndScroll) && (PrimerTrack!=sbh->Position)) 
     {
          PrimerTrack = sbh->Position;
          if (VG.PDP.ActVar!=NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
          MostrarPartitura();
     };
	 */
};

void PattEd::BeatClick(unsigned short Track,unsigned short Beat)
{
     /*Responde a eventos generados al pulsar sobre una celda*/
     NumBeats=NumBeats;
};

void PattEd::TituloTrackClick(unsigned short Track,TMouseButton B)
{
	/*Responde a eventos cuando se ha pulsado sobre un titulo de Track*/
	if (B==mbLeft) 
	{
		// <WARNING: VC>
		// RellenarDatosInstrumentoTrack(VG.Sng->TIns->T[PrimerTrack+Track],PrimerTrack+Track);
		// AbrirDialogo();		
	}
	else
	if (&(VG.PDP.ProcMute)!=NULL) 
	{
		Mutes[PrimerTrack+Track]=! Mutes[PrimerTrack+Track];
		VG.PDP.ProcMute(PrimerTrack+Track,Mutes[PrimerTrack+Track]);

		/*Mostrar el track muteado*/
		PintaNumeracionTracks();
	};
};
//--------------------------------------------------------------------------------
void PattEd::Resize(short tx,short ty)
{
	RECT CR;

	::GetClientRect(hWnd,&CR);
	iWidth  = CR.right - CR.left;
	iHeight = CR.bottom - CR.top;
	
	if (! bCanResize)
	{
		usFirstTX = iWidth;
		usFirstTY = iHeight;
		return;
	}
	
	if ((tx==0) || (ty==0)) return;
	unsigned short MargenH,MargenV,AnchoV,AltoV;

	// Valores que se pueden reinicializar varias veces
	// ClientWidth<150)  ClientWidth=150;
	// if (ClientHeight<150)  ClientHeight=150;
	NumTracks  = (iWidth - TXCtes) / AnchoBeat;
	NumBeats   = (iHeight- TYCtes) / AltoBeat;

	// if (NumTracks<2)  { NumTracks=2;ClientWidth =(NumTracks*AnchoBeat)+TXCtes;};
	// if (NumBeats <8)  { NumBeats=8; ClientHeight=(NumBeats*AltoBeat) +TYCtes;};

	MargenH = (iWidth -  ((NumTracks*AnchoBeat)+TXCtes)) / 2;
	MargenV = (iHeight - ((NumBeats*AltoBeat)  +TYCtes)) / 2;

	// ------------------------------------------------------------------
	ZNums.cx   = 32 + MargenH;
	ZNums.cy   = 32 + MargenV + AltoTTrack;
	ZNums.tx   = AnchoNumB;
	ZNums.ty   = NumBeats*AltoBeat;
	ZNums.fx   = ZNums.cx + ZNums.tx;
	ZNums.fy   = ZNums.cy + ZNums.ty;

	ZMalla.cx  = 32 + MargenH + AnchoNumB;
	ZMalla.cy  = 32 + MargenV + AltoTTrack;
	ZMalla.tx  = NumTracks*AnchoBeat;
	ZMalla.ty  = NumBeats*AltoBeat;
	ZMalla.fx  = ZMalla.cx+ZMalla.tx;
	ZMalla.fy  = ZMalla.cy+ZMalla.ty;

	ZTracks.cx = 32 + MargenH + AnchoNumB;
	ZTracks.cy = 32 + MargenV;
	ZTracks.tx = NumTracks*AnchoBeat;
	ZTracks.ty = AltoTTrack;
	ZTracks.fx = ZTracks.cx + ZTracks.tx;
	ZTracks.fy = ZTracks.cy + ZTracks.ty;
	// ------------------------------------------------------------------
	// <WARNING: VC>
	/*
	sbv->Left   = ZMalla.fx+1;
	sbv->Top    = ZMalla.cy;
	sbv->Height = ZMalla.ty;
	sbv->SmallChange = 1;
	sbv->LargeChange = NumBeats-1;

	sbh->Left   = ZMalla.cx;
	sbh->Top    = ZMalla.fy+1;
	sbh->Width  = ZMalla.tx;
	sbh->SmallChange =1;
	sbh->LargeChange = NumTracks-1;

	sbv->Position =0;
	sbh->Position =0;
	sbh->Min      =0;
	sbv->Min      =0;
	sbv->Max      =MaxBeats-NumBeats;
	sbh->Max      =MaxTracks-NumTracks;
	*/
	if (BeatActual >=NumBeats)   BeatActual=NumBeats-1;
	if (TrackActual>=NumTracks)  TrackActual=NumTracks-1;
	if (VG.PDP.ActVar != NULL)  VG.PDP.ActVar(PrimerBeat,BeatActual,PrimerTrack,TrackActual);
};
//------------------------------------------------------------------------------
void PattEd::PintaBeat(unsigned short T,unsigned short B,unsigned short Tipo)
{
	BeginDraw();

		AnsiString BeatStr;

		/*
		T = Track Físico
		B = Beat Fisico
		Tipo = 0 : Normal
		1 : Beat Actual
		2 : Track Actual
		3 : Pertenece a seleccion
		4 : Borrar el Actual
		5 : Linea actual
		*/

		BeatStr = ObtenerDatosBeat(PrimerTrack+T,PrimerBeat+B);

		switch (Tipo)
		{
			case 4:
			case 2:
			case 0:
					if ((PrimerBeat+B & 0x07) == 0)
					{
						SetBkColor(hDC,VG.PDP.Nrm8BMBkColor);
						SetTextColor(hDC,VG.PDP.Nrm8BMFnColor);
					}
					else
					{
						SetBkColor(hDC,VG.PDP.NrmBkColor);
						SetTextColor(hDC,VG.PDP.NrmFnColor);
					}
			break;

			case 1 :SetBkColor(hDC,VG.PDP.CurBkColor); 
					SetTextColor(hDC,VG.PDP.CurFnColor);
			break;

			case 3 :SetBkColor(hDC,VG.PDP.SelBkColor); 
					SetTextColor(hDC,VG.PDP.SelFnColor);
			break;

			case 5 :SetBkColor(hDC,VG.PDP.MrkBtBkColor); 
					SetTextColor(hDC,VG.PDP.MrkBtFnColor);
			break;
		};

		TextOut(hDC,ZMalla.cx + (T*AnchoBeat)+1,ZMalla.cy + B*AltoBeat,BeatStr.c_str(),BeatStr.Length());

		if (Tipo == 1) 
		{
			int iIX  = ZMalla.cx + (T*AnchoBeat)+(OffsetCampos[CampoActual]*AnchoFuente);
			int iIY  = ZMalla.cy + (B    )*AltoBeat;

			int iFX  = ZMalla.cx + (T*AnchoBeat)+(OffsetCampos[CampoActual]*AnchoFuente) + (DigitosCampos[CampoActual]*AnchoFuente) - 1;
			int iFY  = ZMalla.cy + (B + 1)*AltoBeat-1;

			// Draw rectagle arround field				
			LineaH(iIX+1,iIY,iFX-1,iIY);
			LineaH(iIX+1,iFY,iFX-1,iFY);
			LineaV(iIX,iIY+1,iIX,iFY-1);
			LineaV(iFX,iIY+1,iFX,iFY-1);
		};

	EndDraw();
};
//------------------------------------------------------------------------------
void PattEd::PintaTrack(unsigned short T)
{
	for (unsigned short i=0;i<NumBeats;i++)
	{
		// Por defecto
		unsigned short tipo=0;

		// Posible seleccion
		if (EstadoSel==2)
		{
			if (Coords(BIniSel,TIniSel,BFinSel,TFinSel,PrimerBeat+i,PrimerTrack+T)) 
			tipo=3;
		};

		// Si es en la linea actual ???
		if (i==BeatActual)
		{
			if (T==TrackActual)	tipo=1; else tipo=5;
		};

		// Pintar Beat
		PintaBeat(T,i,tipo);
	};

	BeginDraw();
	
		// Pintar columna de la derecha de cada Track
		LineaV(ZTracks.cx + (T*AnchoBeat),ZTracks.cy,ZTracks.cx + (T*AnchoBeat), ZMalla.fy);	
	
	EndDraw();
};
//------------------------------------------------------------------------------
void PattEd::PintaTracks()
{
     for (int I=0; I<= NumTracks-1;I++)
        PintaTrack(I);
};
//------------------------------------------------------------------------------
void PattEd::PintaLineaBeat(unsigned short Beat,bool Actual)
{
	unsigned char tipo,i;

	// SelectObject(hDC,BMalla);
	// SelectObject(hDC,Font);

	// Pinta la linea normal
	if (! Actual) 
	{
		for (i=0;i <= NumTracks-1;i++)
		{
			// Por defecto
			tipo=4;

			// Posible seleccion
			if (EstadoSel==2)
			{
				if (Coords(BIniSel,TIniSel,BFinSel,TFinSel,PrimerBeat+Beat,PrimerTrack+i))
					tipo=3;
			};

			PintaBeat(i,Beat,tipo);
		};
	}
	else
	{
		for (i=0;i <= NumTracks-1;i++)
		{
			if (i==TrackActual)   tipo=1; else tipo=5;
			PintaBeat(i,Beat,tipo);
		};
	};     
};
//------------------------------------------------------------------------------
void PattEd::PintaNumeracionBeats()
{
	BeginDraw();

		AnsiString s;
		unsigned short j,i;
		char szStr[10];

		for (i=0;i<NumBeats;i++)
		{
			j = (i+PrimerBeat) % 8;
			
			if (j == 0)
			{
				SetBkColor(hDC,VG.PDP.Nrm8BMBkColor);
				SetTextColor(hDC,VG.PDP.Nrm8BMFnColor);
			}
			else
			{
				SetBkColor(hDC,VG.PDP.NrmBkColor);
				SetTextColor(hDC,VG.PDP.NrmFnColor);
			}

			switch (j)
			{
				case 0: 
					sprintf(szStr,"%5d",(i+PrimerBeat) / 8);
					s = szStr;
				break;

				case 2:
				case 6:
					s="  ·  ";
				break;

				case 4:
					s="  -  ";
				break;

				default:
					s="     ";
			};

			TextOut(hDC,ZNums.cx+1,ZNums.cy + (i*AltoBeat),s.c_str(),s.Length());
			/*
			LineaH(ZNums.cx,
				   ZNums.cy + (i+1)*AltoBeat,
				   ZNums.fx,
				   ZNums.cy + (i+1)*AltoBeat
				   );
			*/
		};

	EndDraw();
};
//------------------------------------------------------------------------------
void PattEd::PintaNumeracionTracks()
{
	BeginDraw();

		for (unsigned char i=0;i<NumTracks;i++)
		{
			AnsiString S = ObtenerNombreTrack(PrimerTrack+i);

			if (S=="")
			{
				char szStr[32];				
				sprintf(szStr,"Pista %d",i+PrimerTrack);
				S = szStr;
			}
			
			char szStr2[32];
			sprintf(szStr2,"%20s",S.c_str());
			S = szStr2;

			if (! Mutes[PrimerTrack+i])
			{
				SetBkColor(hDC,VG.PDP.NumBkColor);
				SetTextColor(hDC,VG.PDP.NumFnColor);

				TextOut(hDC,ZTracks.cx + (i*AnchoBeat)+1,ZTracks.cy + 1,S.c_str(),S.Length());
			}
			else
			{
				SetBkColor(hDC,VG.PDP.NumBkColor);
				SetTextColor(hDC,VG.PDP.NumFnColor);

				TextOut(hDC,ZTracks.cx + (i*AnchoBeat)+1,ZTracks.cy + 1,"MUTED",5);
			};
		};

	EndDraw();
};
//------------------------------------------------------------------------------
void PattEd::MostrarPartitura()
{	
	// Mostrar marco
	DrawFrame();

	// Mostrar numeracion de beats
	PintaNumeracionBeats();

	//Mostrar Numeracion de Tracks
	PintaNumeracionTracks();

	//Pintar Tracks
	PintaTracks();

	//Pintar Estructura
	BeginDraw();
		
		LineaH(ZTracks.cx  ,ZTracks.cy  ,ZTracks.fx ,ZTracks.cy);
		LineaH(ZNums.cx    ,ZNums.cy  -1,ZTracks.fx ,ZNums.cy-1);
		LineaH(ZNums.cx    ,ZNums.fy    ,ZTracks.fx ,ZNums.fy  );

		LineaV(ZNums.cx  ,ZNums.cy  ,ZNums.cx  ,ZNums.fy);
		LineaV(ZTracks.fx,ZTracks.cy,ZTracks.fx,ZNums.fy);
	
	EndDraw();
};
//------------------------------------------------------------------------------
#define		TILEW	32
#define		TILEH	32

void PattEd::DrawFrame()
{
	BeginDraw();

		// -------------------------
		// Dibujar el marco exterior
		// -------------------------		
		int iXTiles = ((iWidth + (TILEW-1)) / TILEW);
		for (int i=1;i<iXTiles-1;i++)	// Skip corners
		{
			BitBlt(hDC,i*TILEW,0            ,TILEW,TILEH,oShape.GetDC(),TILEW,      0,SRCCOPY);
			BitBlt(hDC,i*TILEW,iHeight-TILEH,TILEW,TILEH,oShape.GetDC(),TILEW,TILEH*2,SRCCOPY);
		}

		int iYTiles = ((iHeight + (TILEH-1)) / TILEH);
		for (int j=1;j<iYTiles-1;j++)	// Skip corners
		{
			BitBlt(hDC,0           ,j*TILEH,TILEW,TILEH,oShape.GetDC(),      0,TILEH,SRCCOPY);
			BitBlt(hDC,iWidth-TILEW,j*TILEH,TILEW,TILEH,oShape.GetDC(),TILEW*2,TILEH,SRCCOPY);
		}

		// Draw 4 corners
		BitBlt(hDC,0           ,0            ,TILEW,TILEH,oShape.GetDC(),      0,      0,SRCCOPY);
		BitBlt(hDC,iWidth-TILEW,0            ,TILEW,TILEH,oShape.GetDC(),2*TILEW,      0,SRCCOPY);
		BitBlt(hDC,0           ,iHeight-TILEH,TILEW,TILEH,oShape.GetDC(),      0,2*TILEH,SRCCOPY);
		BitBlt(hDC,iWidth-TILEW,iHeight-TILEH,TILEW,TILEH,oShape.GetDC(),2*TILEW,2*TILEH,SRCCOPY);

	EndDraw();
}
//------------------------------------------------------------------------------
void PattEd::Draw()
{
	if (PrimerBeat + NumBeats > MaxBeats)
        PrimerBeat = MaxBeats-NumBeats;

    MostrarPartitura();

	::ValidateRect(hWnd,&oUpdRect);
};
//------------------------------------------------------------------------------
AnsiString PattEd::ObtenerDatosBeat(unsigned short T,unsigned short B)
{
	AnsiString Valor,Ef,aux;
	unsigned char Efecto;

	aux = LeerBeatTrack(LeerTrackDePattern(VG.PatronActual,T),B).CString();

	if ( aux.Pos("_") >= 0)
	{
		// Obtener nombre de los datos que nos ofrece la lista de efectos
		Efecto = LeerEffBeat(LeerTrackDePattern(VG.PatronActual,T),B);

		char szStr[10];
		sprintf(szStr,"%3d", LeerValBeat(LeerTrackDePattern(VG.PatronActual,T),B) );
		Valor = szStr;

		Ef = VG.ListaEfectos[VG.Sng->TIns->T[T].Disp].TomarNombreEfecto(Efecto).CString();

		if (Ef=="        ")
		{
			sprintf(szStr,"%4d", Efecto );
			Ef = "Eff:";
			Ef = Ef + szStr;
		};

		// WARNING: DEBUG
		// Aux = Copy(Aux,1,Pos('-',Aux)-1)+Ef+" "+Valor;
	};

	return( aux);
};
//------------------------------------------------------------------------------
AnsiString PattEd::ObtenerNombreTrack(unsigned short T)
{
	if ( AnchoFuente * strlen( VG.Sng->TIns->T[T].Nombre.CString() ) > AnchoBeat )
	{
		char szString[256];.
		strncpy(szString,VG.Sng->TIns->T[T].Nombre.CString(),AnchoBeat / AnchoFuente );
		return(szString);
	}
	else
		return ( VG.Sng->TIns->T[T].Nombre.CString() );
};
//------------------------------------------------------------------------------
void PattEd::ActualizaPantalla()
{	
     MostrarPartitura();

	 // <WARNING: VC>
     /*
	 sbv->Position=PrimerBeat;
     sbh->Position=PrimerTrack;
	 */
};
//------------------------------------------------------------------------------
void __stdcall PattEd::RSIMidiIn(HMIDIIN ldDisp,unsigned short Msj,DWORD dir,DWORD Mensaje,DWORD Tiempo)
{
	TEventoMidiIn    EventoMIDI;
	unsigned char    MidiOutPort;
	unsigned char    TAct;
	unsigned char    Ba;

    // IMPORTANTE:
    //            Esta función solo estaba pensada para eventos tipo midi_in_data,
    //            por lo que se hace necesario el uso de filtros

    if (Msj==MIM_DATA)     // Miramos si el evento es de datos tipo short
    {
		// <WARNING: VC>
        // if ((Screen->ActiveForm != Main) && (Main->Ready)) Main->SetFocus();

        TAct = VG.AbsTrackActual;

        // Transformar el evento en algo legible para los controles del formulario
        EventoMIDI.Cmd   =  Mensaje & 0x000000ff;
        EventoMIDI.Data1 = (Mensaje & 0x0000ff00) >> 8;
        EventoMIDI.Data2 = (Mensaje & 0x00ff0000) >> 16;
        EventoMIDI.Sysex = NULL;
        EventoMIDI.TamSysex = 0;

        // Averiguar el evento
        EventoMIDI.Cmd = (EventoMIDI.Cmd & 0xf0);

        if ((EventoMIDI.Cmd == 0x90) & (EventoMIDI.Data2==0))
            EventoMIDI.Cmd=0x80;

        switch (EventoMIDI.Cmd)
        {
            // Si es note on ...
            case 0x90:
            {
                 // Mirar si se debe modificar el patron o no
                 if (VG.PatronActivo)
                 {
                      // Activar Timer de actualizacion de pantalla
                      // Main->T1->Enabled = true;

                      EventoMIDI.Data2 = VG.Sng->TIns->T[TAct].VolDef;

                      // Escribir evento en la página
                      EscribirBeatTrack(LeerTrackDePattern(VG.PatronActual,(VG.AbsTrackActual+VG.ContArpegios) % MaxTracks),
                                                           VG.AbsBeatActual,
                                                           EventoMIDI.Data1 + VG.Transponer,
                                                           VG.Sng->TIns->T[TAct].VolDef,
                                                           0,
                                                           0);
                      // Actualiza variables de salto
                      VG.ContAcordes = (VG.ContAcordes+1)  % VG.Acorde;
                      VG.ContArpegios= (VG.ContArpegios+1) % VG.Arpegio;

                      if (VG.ContAcordes==0)
                      {
                           BeatActual= BeatActual+VG.SaltoLinea;
                           VG.AbsBeatActual = PrimerBeat + BeatActual;
                      }

                      if (BeatActual>=NumBeats)
                      {
                           if (VG.AbsBeatActual<MaxBeats)
                           {
                                Ba=VG.AbsBeatActual;

                                if ((PrimerBeat+NumBeats-1)+VG.SaltoLinea<MaxBeats)
                                {
                                     BeatActual=BeatActual-VG.SaltoLinea;
                                     PrimerBeat=PrimerBeat+VG.SaltoLinea;
                                     VG.AbsBeatActual =PrimerBeat+BeatActual;
                                }
                                else
                                {
                                     PrimerBeat =MaxBeats-NumBeats;
                                     BeatActual =Ba-PrimerBeat;
                                     VG.AbsBeatActual = PrimerBeat+BeatActual;
                                }
                           }
                           else
                           {
                                BeatActual=BeatActual-VG.SaltoLinea;
                                VG.AbsBeatActual = PrimerBeat+BeatActual;
                           }
                      }
                 } // END If (PatronActivo)

                 // Si es noteoff, volumen =0;
                 PlayNoteOn(VG.Sng->TIns->T[TAct].Disp,
                 VG.Sng->TIns->T[TAct].Canal,
                 EventoMIDI.Data1 + VG.Transponer,
                 VG.Sng->TIns->T[TAct].VolDef);
            }
            break;

            case 0x80:
            {
                PlayNoteOff(VG.Sng->TIns->T[TAct].Disp,
                            VG.Sng->TIns->T[TAct].Canal,
                            EventoMIDI.Data1 + VG.Transponer);
            }
            break;

            default:
            {
                // Del resto de eventos pasamos como si nada...
            }
        } // END Switch
    } // END If
}
//------------------------------------------------------------------------------
void PattEd::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CFileDialog oLoadFile(TRUE, NULL, NULL, NULL, "TCK song files(*.TCK)|*.tck|All the files(*.*)|*.*||"); 

	if (oLoadFile.DoModal()!= IDOK)
		return;

	char szFileName[256];
	strcpy(szFileName, oLoadFile.GetPathName());

}
//------------------------------------------------------------------------------

int PattEd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

#endif
