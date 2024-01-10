// BRep.cpp : implementation file
//

#include "stdafx.h"
#include "TTVC_MFC2.h"
#include "BRepChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------
#include "Misc/Variables.h"
#include "Player/tt98Player.h"
#include "Player/OMidiIn.h"
#include "Player/OMidiOut.h"
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CBRepChildView

CBRepChildView::CBRepChildView()
{
   if ( IsWindow( m_hWnd ) )
      Invalidate();
}

CBRepChildView::~CBRepChildView()
{
}


void CBRepChildView::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd::PreSubclassWindow();
}

BEGIN_MESSAGE_MAP(CBRepChildView, CWnd)
	//{{AFX_MSG_MAP(CBRepChildView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// ------------------------------------------------------------------------
// Button actions
// ------------------------------------------------------------------------
void BTAC_ResetMIDISystem();
void BTAC_PlayCancion();
void BTAC_StopCancion();

/////////////////////////////////////////////////////////////////////////////
// CBRepChildView message handlers

int CBRepChildView::iInit(HWND _hWnd,HINSTANCE _hInst)
{
	hWnd  = _hWnd;
	hInst = _hInst;

	oBMPRes.Load("data\\plantt99.bmp",hWnd,hInst);
	oBtnMgr.Init(32);
	oBtnMgr.iAddButton(540,22,50,70,&oBMPRes,350+ 0,96+  0,&oBMPRes,350+50,96+  0,NULL,350+ 0,96+  0,NULL,0,0,eBTOneShot,eBSReleased,0,BTAC_ResetMIDISystem ,NULL,hWnd);  // Reset
    oBtnMgr.iAddButton(135,75,75,30,&oBMPRes,199+ 0,60+  0,&oBMPRes,199+75,60+  0,NULL,199+ 0,60+  0,NULL,0,0,eBTSwitch ,eBSReleased,1,BTAC_PlayCancion		,NULL,hWnd);  // Play
    oBtnMgr.iAddButton( 60,75,75,30,&oBMPRes,199+ 0,60+ 30,&oBMPRes,199+75,60+ 30,NULL,199+ 0,60+ 30,NULL,0,0,eBTOneShot,eBSReleased,1,BTAC_StopCancion		,NULL,hWnd);  // Stop
    oBtnMgr.iAddButton(210,75,75,30,&oBMPRes,199+ 0,60+ 60,&oBMPRes,199+75,60+ 60,NULL,199+ 0,60+ 60,NULL,0,0,eBTSwitch ,eBSReleased,1,NULL					,NULL,hWnd);  // Pause
    oBtnMgr.iAddButton(100, 8,75,30,&oBMPRes,199+ 0,60+ 90,&oBMPRes,199+75,60+ 90,NULL,199+ 0,60+ 90,NULL,0,0,eBTRepeat ,eBSReleased,0,NULL					,NULL,hWnd);  // PlayTrack
    oBtnMgr.iAddButton(175, 8,75,30,&oBMPRes,199+ 0,60+120,&oBMPRes,199+75,60+120,NULL,199+ 0,60+120,NULL,0,0,eBTSwitch ,eBSReleased,0,NULL					,NULL,hWnd);  // PlayPattern
	
	return(1);
}
// ------------------------------------------------------------------------
// Button actions
// ------------------------------------------------------------------------
void BTAC_ResetMIDISystem()
{
	ResetSistemaMIDI();
}

void BTAC_PlayCancion()
{
	PlayCancion(VG.Sng);
}

void BTAC_StopCancion()
{
	StopCancion();
}
// ------------------------------------------------------------------------
int CBRepChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	iInit(m_hWnd,lpCreateStruct->hInstance);

	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code her
}
