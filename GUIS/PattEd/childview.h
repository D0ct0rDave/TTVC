#ifdef MFC_PATTED

// ChildView.h : interface of the PattEd class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__4100FCA0_CD03_46F4_AFAA_5F9DD9876F89__INCLUDED_)
#define AFX_CHILDVIEW_H__4100FCA0_CD03_46F4_AFAA_5F9DD9876F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// PattEd window

// #include <windows.h>
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




class PattEd : public CWnd
{
// Construction
public:
	PattEd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PattEd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PattEd();

	// Generated message map functions
			int iInit(HWND _hWnd,HINSTANCE _hInst);
			void Finish();
			LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			static void ActualizaPantalla();
			static void PintaTrack    (unsigned short T);
			static void PintaBeat     (unsigned short T,unsigned short B,unsigned short Tipo);
			static void PintaLineaBeat(unsigned short Beat,bool Actual);


protected:
	//{{AFX_MSG(PattEd)
	afx_msg void OnFileOpen();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
			


			void Resize(short tx,short ty);
			
			void BeginDraw();
			void Draw();
			void EndDraw();

			// Eventos que seran simulados por quien los reciba
			void KeyDownHandler(bool Shift,unsigned short key);
			bool KeyDown_ViewerHandler (bool Shift,unsigned short key);
			bool KeyDown_EditionHandler(bool Shift,unsigned short key);
			bool KeyDown_PianoHandler  (bool Shift,unsigned short key);
			
			void KeyUpHandler(bool Shift,unsigned short key);
			bool KeyUp_PianoHandler(bool Shift,unsigned short key);

			bool MouseDown(TMouseButton Button,short X,short Y);
			void sbvScroll(TScrollCode ScrollCode);
			void sbhScroll(TScrollCode ScrollCode);
			void MostrarPartitura();

			void PintaTracks();
			void PintaNumeracionBeats();
			void PintaNumeracionTracks();
			void DrawFrame();

			AnsiString ObtenerDatosBeat(unsigned short T,unsigned short B);
			AnsiString ObtenerNombreTrack(unsigned short T);

			void BeatClick(unsigned short Track,unsigned short Beat);
			void TituloTrackClick(unsigned short Track,TMouseButton B);

			bool Coords(short ix,short iy,short fx,short fy,short cx,short cy);
			TColor GetRGB(unsigned char R,unsigned char G,unsigned char B);

			void Line  (unsigned short cx1,unsigned short cy1,unsigned short cx2,unsigned short cy2);
			void LineaH(unsigned short cx1,unsigned short cy1,unsigned short cx2,unsigned short cy2);
			void LineaV(unsigned short cx1,unsigned short cy1,unsigned short cx2,unsigned short cy2);

			void ZTracksMouseDown(TMouseButton Button,short X, short Y);
			void ZMallaMouseDown (TMouseButton Button,short X, short Y);
			
			// ------------------------------
			void __stdcall RSIMidiIn(HMIDIIN ldDisp,unsigned short Msj,DWORD dir,DWORD Mensaje,DWORD Tiempo);

		private:
			static HINSTANCE		hInst;
			static HWND				hWnd;

			static HDC				hDC;			
			
			static RECT				oUpdRect;
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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__4100FCA0_CD03_46F4_AFAA_5F9DD9876F89__INCLUDED_)

#endif
