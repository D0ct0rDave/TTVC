// BSeq.cpp : implementation file
//

#include "stdafx.h"
#include "TTVC_MFC2.h"

#include "BSeq.h"
#include "CrtError.h"
#include "SongStructs/tipos.h"

#include "Misc/Variables.h"
#include "Misc/LangStrs.h"
#include "Misc/WinVars.h"

#include "Controls/BMPResource.h"

#include "GUIS/PattEd/PattEd.h"
#include "GUIS/InputBox/InputBox.h"


CBMPResource	oShape;
HINSTANCE		hInst;
int				giMaxVisItems;
int				giFirstElem;
HBRUSH			hBckBrush;
CBrush			hBlueBrush,hRedBrush;
/////////////////////////////////////////////////////////////////////////////

CBSeq	*gpoBSeq;
/////////////////////////////////////////////////////////////////////////////

LRESULT CMyEditDblCl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDBLCLK)
	{
		// Editar autor de la canción
		if ( bInputBox(LSTR_APP_TITLE,LSTR_SONGAUTOR_IB_CAPTION,m_szStr,80) )
			SetWindowText(m_szStr);
		
		return 0;
	}
	else
		return DefWindowProc(message,wParam,lParam);
}

/////////////////////////////////////////////////////////////////////////////
static bool bCtrlPressed = false;

LRESULT CSeqListBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// Process messages
	 switch (message)
	 {
		case WM_COMMAND:
		{			
			HWND hwndCtl = (HWND) lParam;      // handle of control 			
			if (hwndCtl == NULL)
			{
				return ::SendMessage(ghMainWnd,message,wParam,lParam);
			}
		}
		break;

		case WM_KEYUP:
		case WM_KEYDOWN:
		{

			unsigned int Key = (int)wParam;


			/*
			if (Key == VK_CONTROL)
			{
				if (message == WM_KEYDOWN)
					guiKeyFlags |= KEY_CONTROL;
				else
					guiKeyFlags &= ~KEY_CONTROL;
			}


			if (! (guiKeyFlags & KEY_CONTROL))
			{
			*/
				switch (Key)
				{
					case VK_INSERT:
					case VK_DELETE:
					case VK_BACK:
					case 109:
					case 189:
					case 107:
					case 187:
					case VK_UP:
					case VK_DOWN:
					case VK_PRIOR:
					case VK_NEXT:
					case VK_HOME:
					case VK_END:
					{
						return CListBox::WindowProc(message,wParam,lParam);
					};
					break;
					
					default:
					
					// This allows accelerators to be processed
					MSG msg;	
					msg.hwnd    = m_hWnd;
					msg.lParam  = lParam;
					msg.wParam  = wParam;
					msg.message = message;
				
					TranslateAccelerator(ghMainWnd,ghAccel,&msg);				

					/*
						// return CListBox::WindowProc(message,wParam,lParam);
					*/
					
					// This allows keydown events directed to PatternEditor to be processed
					::SendMessage(ghMainWnd,message,wParam,lParam);
				}
			/*
			}
			else
			{
				// Mandamos los mensajes de teclado al gestor general de mensajes de teclado				
				if (Key != VK_CONTROL)
				{
					MSG msg;	
					msg.hwnd    = m_hWnd;
					msg.lParam  = lParam;
					msg.wParam  = wParam;
					msg.message = message;
					
					TranslateAccelerator(ghMainWnd,ghAccel,&msg);					
				}
				
				::SendMessage(ghMainWnd,message,wParam,lParam);
			}
			*/
		}
		break;

		default:
			return CListBox::WindowProc(message,wParam,lParam);
	}
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBSeq::CBSeq(CWnd* pParent /*=NULL*/)
	 : CDialog(CBSeq::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBSeq)
	//}}AFX_DATA_INIT
}

void CBSeq::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CBSeq)
	DDX_Control(pDX, IDC_ST_AUTOR, m_oSt_Autor);
	DDX_Control(pDX, IDC_ST_TITLE, m_oSt_Title);
	DDX_Control(pDX, IDC_LIST_SEQ, m_oLst_Seq);
	//}}AFX_DATA_MAP
}

// BEGIN_MESSAGE_MAP(CBSeq, CDialog)
BEGIN_MESSAGE_MAP(CBSeq, CWnd)
	//{{AFX_MSG_MAP(CBSeq)
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_SEQ, OnSelchangeListSeq)
	ON_LBN_DBLCLK(IDC_LIST_SEQ, OnDblclkListSeq)
	ON_STN_DBLCLK(IDC_ST_TITLE, OnStTitle)
	ON_STN_DBLCLK(IDC_ST_AUTOR, OnStAutor)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_SEQ, OnSelchangeListSeq)
	ON_LBN_DBLCLK(IDC_LIST_SEQ, OnDblclkListSeq)
	ON_WM_VKEYTOITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


LRESULT CBSeq::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch (message)
	{
		case WM_CLOSE:
		// Avoid closing double clicking on system menu
		break;

		case WM_COMMAND:
		{
			HWND hwndCtl = (HWND) lParam;      // handle of control 
			
			if (hwndCtl == NULL)
			{
				/*
				switch (wParam)
				{
					case 
				*/
					return ::SendMessage(ghMainWnd,message,wParam,lParam);
				/*
				}
				*/
			}
		}
		break;

		// Mandamos los mensajes de teclado al gestor general de mensajes de teclado
		case WM_KEYUP:
		case WM_KEYDOWN:
			::SendMessage(ghMainWnd,message,wParam,lParam);
		return(0);		
		break;

		default:
			return CWnd::DefWindowProc(message, wParam, lParam);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBSeq message handlers

BOOL CBSeq::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	hInst = cs.hInstance;	
//	cs.style = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	int iWidth  = GetSystemMetrics(SM_CXSCREEN);
	int iHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT oRect;
	GetWindowRect(&oRect);
	cs.x = iWidth  - giBSeqWTX;
	cs.y = iHeight - giBSeqWTY - giBRepWTX;

	return CWnd::PreCreateWindow(cs);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CBSeq::OnInitDialog() 
{
	CDialog::OnInitDialog();

	oShape.Load("data/BSeqShape.bmp",m_hWnd,hInst);
	
	// TODO: Add extra initialization here
	SetWindowText(LSTR_SEQBAR_NAME);
	
	// 
	m_oSt_Autor.SetWindowText( VG.Sng->Autor.CString() );
	m_oSt_Title.SetWindowText( VG.Sng->Autor.CString() );

	// Retrieve max visible items
	int iItH = m_oLst_Seq.GetItemHeight(0);	
	RECT oRect;
	m_oLst_Seq.GetClientRect(&oRect);
	
	giMaxVisItems = 1 + (oRect.bottom - oRect.top) / iItH;

	// 0x007B8C5A

	hBckBrush = CreateSolidBrush(VG.PDP.PattEdBk);

	UpdateContents();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
HBRUSH CBSeq::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr;	
	
	// TODO: Change any attributes of the DC here
	switch (nCtlColor)	
	{
		case CTLCOLOR_EDIT:
		case CTLCOLOR_MSGBOX:
		case CTLCOLOR_STATIC:
		case CTLCOLOR_LISTBOX:
		{
			switch (pWnd->GetDlgCtrlID())
			{
				case IDC_LIST_SEQ:
				case IDC_EDIT_TITLE:
				case IDC_EDIT_AUTOR:
				case IDC_ST_TITLE:
				case IDC_ST_AUTOR:
				  pDC->SetBkColor(VG.PDP.PattEdBk);
				  pDC->SetTextColor(VG.PDP.CurFnColor);
				  hbr = (HBRUSH) hBckBrush;
				break;

				
				default:
					hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
			}
		}
		break;

		default:
			hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
/////////////////////////////////////////////////////////////////////////////
void CBSeq::UpdateContents()
{
	// TODO: Add extra initialization here
	ObtenerSecuencia();
	// m_oLst_Seq.SendMessage(WM_PAINT);
	// m_oLst_Seq.SetCurSel(VG.Sng->Seq->Num-1);

	/*
	m_oLst_Seq.Invalidate();

	// 
		m_oLst_Seq.RedrawWindow();
	m_oLst_Seq.UpdateWindow();
	for (i=0;i<m_oLst_Seq.GetCount();i++)
	{
	}
	*/
}
/////////////////////////////////////////////////////////////////////////////
void CBSeq::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) 
{
	if (nIDCtl != IDC_LIST1) return;
	if ((lpDIS->itemID > VG.Sng->Seq->Num) || (lpDIS->itemID<0)) return;

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	COLORREF cr = (COLORREF)lpDIS->itemData; // RGB in item data

	// Get item text
	char szText[256];
	int iPat  = VG.Sng->Seq->S[ lpDIS->itemID ];
	TT98Pattern *poPat = VG.Sng->Pats[iPat];

	if (poPat)
		sprintf(szText,"%d %d %s",lpDIS->itemID, iPat , poPat->Nombre.CString());
	else
		sprintf(szText,"",lpDIS->itemID, iPat);
	
	// ---------------------------------
	if (
		(lpDIS->itemAction & ODA_DRAWENTIRE) ||
		(lpDIS->itemAction & ODA_SELECT) ||
		(lpDIS->itemAction & ODA_FOCUS)
		)
	{
		COLORREF BakColor;
		COLORREF TxtColor;

		// Paint the color item in the color requested
		if (lpDIS->itemState & ODS_SELECTED)
		{
			BakColor = VG.PDP.CurBkColor;
			TxtColor = VG.PDP.CurFnColor;
		}
		else
		{
			BakColor = VG.PDP.PattEdBk;
			TxtColor = VG.PDP.NrmFnColor;
		}

		// Draw items
		CBrush  br(BakColor);
		CPen	pn(PS_SOLID,1,VG.PDP.NrmFnColor);

		SelectObject(lpDIS->hDC,pn);
		
		pDC->SetTextColor(TxtColor);
		pDC->SetBkColor(BakColor);
		pDC->FillRect(&lpDIS->rcItem, &br);
		
		// Draw first column
		pDC->MoveTo(lpDIS->rcItem.left,lpDIS->rcItem.top);
		pDC->LineTo(lpDIS->rcItem.left,lpDIS->rcItem.bottom);

		lpDIS->rcItem.left += 2;		
		sprintf(szText,"%d",lpDIS->itemID);		
		pDC->DrawText(szText,-1,&lpDIS->rcItem,DT_LEFT);

		// Draw second column
		lpDIS->rcItem.left += 20;
		pDC->MoveTo(lpDIS->rcItem.left,lpDIS->rcItem.top);
		pDC->LineTo(lpDIS->rcItem.left,lpDIS->rcItem.bottom);

		lpDIS->rcItem.left += 2;
		sprintf(szText,"%d",iPat);
		pDC->DrawText(szText,-1,&lpDIS->rcItem,DT_LEFT);

		// Draw third column
		lpDIS->rcItem.left += 22;
		pDC->MoveTo(lpDIS->rcItem.left + 0,lpDIS->rcItem.top);
		pDC->LineTo(lpDIS->rcItem.left + 0,lpDIS->rcItem.bottom);
		
		lpDIS->rcItem.left += 2;
		if (poPat) sprintf(szText,"%s",poPat->Nombre.CString()); else sprintf(szText,"<NULL>");
		pDC->DrawText(szText,-1,&lpDIS->rcItem,DT_LEFT);
		
		// Draw Last column
		pDC->MoveTo(lpDIS->rcItem.right-1,lpDIS->rcItem.top);
		pDC->LineTo(lpDIS->rcItem.right-1,lpDIS->rcItem.bottom);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CBSeq::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	BitBlt(dc.m_hDC,0,0,oShape.iGetTX(),oShape.iGetTY(),oShape.GetDC(),0,0,SRCCOPY);
		
	// Do not call CDialog::OnPaint() for painting messages
}
/////////////////////////////////////////////////////////////////////////////
int CBSeq::iCurSeqPos()
{
	return( m_oLst_Seq.GetCurSel() );
}
/////////////////////////////////////////////////////////////////////////////
void CBSeq::SetCurSeqPos(int _iIdx)
{
	m_oLst_Seq.SetCurSel(_iIdx);
}
/////////////////////////////////////////////////////////////////////////////
void CBSeq::ObtenerSecuencia()
{
	// Clear list
	int i;
	m_oLst_Seq.ResetContent();

	// Fill list
	CString str;
	for(i=0;i<VG.Sng->Seq->Num; i++)
	{
		str.Format(_T("item string %d"), i);
		m_oLst_Seq.AddString( str );
	}

	m_oLst_Seq.SetCurSel(VG.IndSecActual);
    VG.PatternActual    = VG.Sng->Seq->S[VG.IndSecActual];
    VG.PatronActual     = VG.Sng->Pats[VG.PatternActual];
    VG.PistaActual      = &(VG.PatronActual->Tracks[VG.AbsTrackActual]);

	m_oSt_Title.SetWindowText(VG.Sng->Titulo.CString()); // GetWindowText(szString,256);
	m_oSt_Autor.SetWindowText(VG.Sng->Autor .CString()); 
	
	/*
    Tit->Text = VG.Sng->Titulo.CString();
    Aut->Text = VG.Sng->Autor.CString();
	*/
}
/////////////////////////////////////////////////////////////////////////////
// Obtener el titulo del pattern
/////////////////////////////////////////////////////////////////////////////
void CBSeq::OnDblclkListSeq() 
{
	// TODO: Add your control notification handler code here
	//

	// Editar nombre del patrón
	TPatternNum Pat;
	Pat = LeerIndicePatronEnSecuencia(VG.Sng->Seq,VG.IndSecActual);

	if ( bInputBox(LSTR_APP_TITLE,LSTR_PATNAME_IB_CAPTION,VG.Sng->Pats[Pat]->Nombre.CString(),80))
		gpoBSeq->ObtenerSecuencia();
}

void CBSeq::OnSelchangeListSeq() 
{
	// TODO: Add your control notification handler code here
	// Control list box cursor changes
	TPatternNum Pat;

	Pat = LeerIndicePatronEnSecuencia(VG.Sng->Seq,VG.IndSecActual);
	VG.IndSecActual = m_oLst_Seq.GetCurSel();

	if (Pat != LeerIndicePatronEnSecuencia(VG.Sng->Seq,VG.IndSecActual))
	{
		VG.PatternActual= VG.Sng->Seq->S[VG.IndSecActual];
		VG.PatronActual = VG.Sng->Pats[VG.PatternActual];
		VG.PistaActual  = &(VG.PatronActual->Tracks[VG.AbsTrackActual]);
		PattEd::UpdateContents();
	};
}

// Poner flag Want Key Input en las propiedades de la lsit box en el class wizard
int CBSeq::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
	// TODO: Add your message handler code here and/or call default
	// Process messages
			
	unsigned int Key = (int)nKey;
	TPatternNum Pat;

	switch (Key)
	{
		case VK_INSERT:
		{
			if (InsertarPatronEnSecuencia(VG.Sng->Seq,VG.IndSecActual))
				gpoBSeq->ObtenerSecuencia();
			else						
				ShowMessage(DescripcionDeError().CString());
			
			return CWnd::OnVKeyToItem(nKey, pListBox, nIndex);
		};
		break;
		
		case VK_DELETE:
		{
			if (BorrarPatronDeSecuencia(VG.Sng->Seq,VG.IndSecActual))
				gpoBSeq->ObtenerSecuencia();
			else
				ShowMessage(DescripcionDeError().CString());
			
			return CWnd::OnVKeyToItem(nKey, pListBox, nIndex);
		};
		break;
		
		case VK_BACK:
		{
			if (VG.IndSecActual>0)
			{
				VG.IndSecActual=VG.IndSecActual-1;
				BorrarPatronDeSecuencia(VG.Sng->Seq,VG.IndSecActual);
				gpoBSeq->ObtenerSecuencia();
			};

			return CWnd::OnVKeyToItem(nKey, pListBox, nIndex);
		};
		break;
		
		case 109:
		case 189:
		{
			// Tecla -
			Pat=LeerIndicePatronEnSecuencia(VG.Sng->Seq,VG.IndSecActual);

			if (Pat>0)
			{
				EscribirIndicePatronDeSecuencia(VG.Sng->Seq,VG.IndSecActual,Pat-1);
				gpoBSeq->ObtenerSecuencia();
				PattEd::UpdateContents();
			};

			return CWnd::OnVKeyToItem(nKey, pListBox, nIndex);
		};

		break;
		case 107:
		case 187:
		{
			// Tecla +
			Pat=LeerIndicePatronEnSecuencia(VG.Sng->Seq,VG.IndSecActual);

			if (Pat<MaxPatterns)
			{
				EscribirIndicePatronDeSecuencia(VG.Sng->Seq,VG.IndSecActual,Pat+1);
				if (VG.Sng->Pats[Pat+1]==NULL)  CrearPattern(VG.Sng->Pats[Pat+1]);

				gpoBSeq->ObtenerSecuencia();
				PattEd::UpdateContents();
			};
			
			return CWnd::OnVKeyToItem(nKey, pListBox, nIndex);
		};
		break;

		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
		{
			// Pat=Sng.Seq.LeerIndice;
			// Sng.Seq.PActual=Secuenciador->Row;
			// if (Pat!=Sng.Seq.LeerIndice)  PatternDisplay.UpdateContents;
			return CWnd::OnVKeyToItem(nKey, pListBox, nIndex);
		};
		break;

		default:
			return ::SendMessage(ghMainWnd,WM_KEYDOWN,Key,0);
	}	
}

/////////////////////////////////////////////////////////////////////////////
// Obtener el titulo del pattern
/////////////////////////////////////////////////////////////////////////////
bool CBSeq::KeyDownHandler(unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode)
{
	return(false);
}

bool CBSeq::KeyUpHandler  (unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode)
{
	return(false);
}

void CBSeq::OnStTitle() 
{
	// TODO: Add your control notification handler code here

	// Editar autor de la canción
	if ( bInputBox(LSTR_APP_TITLE,LSTR_SONGAUTOR_IB_CAPTION,VG.Sng->Titulo.CString(),80) )
		m_oSt_Title.SetWindowText( VG.Sng->Titulo.CString() );
		// SetDlgItemText(IDC_ST_TITLE,VG.Sng->Titulo.CString());
}

void CBSeq::OnStAutor()
{
	// TODO: Add your control notification handler code here
	
	// Editar autor de la canción
	if ( bInputBox(LSTR_APP_TITLE,LSTR_SONGAUTOR_IB_CAPTION,VG.Sng->Autor.CString(),80) )
		m_oSt_Autor.SetWindowText( VG.Sng->Autor.CString() );
		// SetDlgItemText(IDC_ST_TITLE,VG.Sng->Titulo.CString());
}
