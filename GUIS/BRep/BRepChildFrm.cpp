// ChildFrm.cpp : implementation of the CBRepChildFrame class
//

#include "stdafx.h"
#include "TTVC_MFC2.h"

#include "BRepChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBRepChildFrame

IMPLEMENT_DYNCREATE(CBRepChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CBRepChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CBRepChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
		ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
		ON_WM_SETFOCUS()
		ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBRepChildFrame construction/destruction

CBRepChildFrame::CBRepChildFrame()
{
	// TODO: add member initialization code here
	
}

CBRepChildFrame::~CBRepChildFrame()
{
}

BOOL CBRepChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBRepChildFrame diagnostics

#ifdef _DEBUG
void CBRepChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CBRepChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBRepChildFrame message handlers
void CBRepChildFrame::OnFileClose() 
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.

	SendMessage(WM_CLOSE);
}

int CBRepChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	return 0;
}

void CBRepChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

BOOL CBRepChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	// otherwise, do default handling
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

