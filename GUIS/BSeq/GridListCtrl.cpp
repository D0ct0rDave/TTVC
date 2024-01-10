// SeqEd.cpp : implementation file
//

#include "stdafx.h"
#include "TTVC_MFC2.h"
#include "BSeq.h"
#include "Misc/Variables.h"
#include "SongStructs/tipos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeqEd

IMPLEMENT_DYNCREATE(CSeqEd, CDialog)

CSeqEd::CSeqEd()
{
}

CSeqEd::~CSeqEd()
{
}


BEGIN_MESSAGE_MAP(CSeqEd, CDialog)
	//{{AFX_MSG_MAP(CSeqEd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeqEd message handlers

BOOL CSeqEd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateContents();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
void CSeqEd::UpdateContents()
{
	// TODO: Add extra initialization here
	LV_ITEM		item;	
	int			i;
	char		szText[256];

	item.pszText = szText;
	
	
	// TODO: Add extra initialization here
	m_oSeqLst.InsertColumn(0,"#"	  ,LVCFMT_LEFT, 25, 0);
	m_oSeqLst.InsertColumn(1,"Pattern",LVCFMT_LEFT, 100, 1);

	for(i=0;i<VG.Sng->Seq->Num; i++)
	{

		item.iItem		= i;
		item.mask		= LVIF_TEXT;

		// First field
		item.lParam		= (LPARAM)i;
		item.iSubItem	= 0;
		sprintf(item.pszText,"%d",i);
		
		m_oSeqLst.InsertItem(&item);
		

		// Second field
		int iPat  = VG.Sng->Seq->S[i];
		TT98Pattern *poPat = VG.Sng->Pats[iPat];
		
		if (poPat)
		{
			sprintf(item.pszText,"%s",poPat->Nombre);
			item.lParam		= (LPARAM)iPat;
			item.iSubItem	= 1;
			m_oSeqLst.SetItem(&item);

			
			m_oSeqLst.SetItemData(i,(DWORD)iPat);
		}
	}
	
	m_oSeqLst.SetSelectionMark(0);

}
/////////////////////////////////////////////////////////////////////////////
