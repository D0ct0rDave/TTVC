#ifndef MFC_PATTED

#ifndef PattEdH
#define PattEdH

#ifdef MFCS
	#include <Afxwin.h>
	#include <afxcmn.h>
#else
	#include <windows.h>
#endif


#include <mmSystem.h>

#include "PascalCnv.h"
#include "Controls/BMPResource.h"

    typedef struct{
        short   cx,cy,
                fx,fy,
                tx,ty;
    }TRectangulo;
	
	typedef unsigned int TColor;

    typedef void (* TMuteProc  )(unsigned short Track,bool Estado);
    typedef void (* TUpdVarProc)(unsigned short PB,unsigned short BA,unsigned short PT,unsigned short TA);

	typedef enum TMouseButton {mbLeft,mbRight,mbMiddle};

	typedef int TScrollBar;
	// typedef int TMouseButton;
	typedef int TScrollCode;
	typedef int TShiftState;

	class PattEd
	{
		public:
			 PattEd() {}
			~PattEd() {};

			// Public declarations
			static int iInit(HWND _hWnd,HINSTANCE _hInst);			
			static LRESULT CALLBACK WndProc(HWND _hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			static void Finish();

		public:
			static void UpdateContents();
			static void PintaTrack    (unsigned short T);
			static void PintaBeat     (unsigned short T,unsigned short B,unsigned short Tipo);
			static void PintaLineaBeat(unsigned short Beat,bool Actual);

			static bool KeyDownHandler(unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode);
			static bool KeyUpHandler  (unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode);

		private:
			
			static void Resize(short tx,short ty);
			
			static void BeginDraw();
			static void Draw();
			static void EndDraw();

			// Eventos que seran simulados por quien los reciba			
			static bool KeyDown_ViewerHandler (bool Shift,unsigned short key);
			static bool KeyDown_EditionHandler(bool Shift,unsigned short key);
			static bool KeyDown_PianoHandler  (unsigned int _uiKeyFlags,unsigned short key);
			
			static bool KeyUpHandler(bool Shift,unsigned short key);
			static bool KeyUp_PianoHandler(unsigned int _uiKeyFlags,unsigned short key);

			static bool MouseDlbClick(TMouseButton Button,short X,short Y);
			static bool MouseDown(TMouseButton Button,short X,short Y);
			static void sbvScroll(TScrollCode ScrollCode);
			static void sbhScroll(TScrollCode ScrollCode);
			static void MostrarPartitura();

			static void PintaTracks();
			static void PintaNumeracionBeats();
			static void PintaNumeracionTracks();
			static void DrawFrame();

			static AnsiString ObtenerDatosBeat(unsigned short T,unsigned short B);
			static AnsiString ObtenerNombreTrack(unsigned short T);

			static void BeatClick(unsigned short Track,unsigned short Beat);
			static void TituloTrackClick(unsigned short Track,TMouseButton B);

			static bool Coords(short ix,short iy,short fx,short fy,short cx,short cy);
			static TColor GetRGB(unsigned char R,unsigned char G,unsigned char B);

			static void Line  (unsigned short cx1,unsigned short cy1,unsigned short cx2,unsigned short cy2);
			static void LineaH(unsigned short cx1,unsigned short cy1,unsigned short cx2,unsigned short cy2);
			static void LineaV(unsigned short cx1,unsigned short cy1,unsigned short cx2,unsigned short cy2);

			static void ZTracksMouseDown(TMouseButton Button,short X, short Y);
			static void ZMallaMouseDown (TMouseButton Button,short X, short Y);
			
			// ------------------------------
		public:
			static void __stdcall RSIMidiIn(HMIDIIN ldDisp,unsigned short Msj,DWORD dir,DWORD Mensaje,DWORD Tiempo);

		private:
			static HINSTANCE		hInst;
			static HWND				hWnd;

			static HDC				hDC;			
			
			static RECT				UpdRect;
			static CBMPResource		oShape; 
			
			static int				iWidth;
			static int				iHeight;			   

			static TScrollBar		*sbh;
			static TScrollBar		*sbv;

			// Fuentes para las distintas zonas del display
			static HFONT			Font;
						
			// Determinan el ancho y alto en pixels de los beats
			static TRectangulo		ZMalla;
			static TRectangulo		ZTracks;
			static TRectangulo		ZNums;

			//Brushes y Pens para los distintos tipos de beats
			static HBRUSH			Brush;					//
									
			static HPEN				Pen[3];
			static HPEN				SFPen;					// Selected field in beat pen
						
			static unsigned short	AnchoNumB;
			static unsigned short	AnchoBeat;
			static unsigned short	AltoBeat;
			static unsigned short	AltoTTrack;
			static unsigned short	AnchoFuente;
			static unsigned short	AnchoNumBeats;
			static unsigned short	AltoNumTracks;

			static unsigned short	TXCtes;
			static unsigned short	TYCtes;

			static TShiftState		Shiftado;

			static unsigned short	DifXP12;
			static unsigned short	DifYP12;
			
			static bool				bCanResize;
			static bool				bMaximized;
			
			static unsigned short	usFirstTX;
			static unsigned short	usFirstTY;
		
		public:
			static unsigned short	NumTracks;
			static unsigned short	NumBeats;
			static unsigned short	PrimerTrack;
			static unsigned short	PrimerBeat;
			static unsigned short	BeatActual;
			static unsigned short	TrackActual;
			static unsigned short	CampoActual;

			static unsigned short	EstadoSel;
			static unsigned short	BIniSel;
			static unsigned short	BFinSel;
			static unsigned short	TIniSel;
			static unsigned short	TFinSel;
	};

#endif

#endif