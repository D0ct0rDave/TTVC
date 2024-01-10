// InputBox.cpp : implementation file
//

#include "stdafx.h"
#include "TTVC_MFC2.h"
#include "InputBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char *gszTitle;
char *gszText;
char *gszCaption;
int  giTextLen;
/////////////////////////////////////////////////////////////////////////////
// CInputBox dialog


CInputBox::CInputBox(CWnd* pParent /*=NULL*/)
	: CDialog(CInputBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBox)
	//}}AFX_DATA_INIT
}


void CInputBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBox)
	DDX_Control(pDX, IDC_EDIT2, m_oEd_Caption);
	DDX_Control(pDX, IDC_EDIT1, m_oEd_Text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBox, CDialog)
	//{{AFX_MSG_MAP(CInputBox)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

bool bInputBox(char *_szTitle,char *_szCaption,char *_szText,int _iTextLen)
{
	CInputBox oIBox;

	gszTitle   = _szTitle;
	gszCaption = _szCaption;
	gszText    = _szText;
	giTextLen  = _iTextLen;

	return(oIBox.DoModal() == IDOK);
}
/////////////////////////////////////////////////////////////////////////////
// CInputBox message handlers

BOOL CInputBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	SetWindowText(gszTitle);
	::SetWindowText(m_oEd_Caption.m_hWnd,gszCaption);
	::SetWindowText(m_oEd_Text.m_hWnd,gszText);
	
	::SetFocus(m_oEd_Text.m_hWnd);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputBox::OnOK() 
{
	// TODO: Add extra validation here
	::GetWindowText(m_oEd_Text.m_hWnd,gszText,giTextLen);
	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
void CInputBox::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	int iLen = strlen(gszText);
	::SetFocus(m_oEd_Text.m_hWnd);	
	m_oEd_Text.SetSel(iLen,iLen);
}
