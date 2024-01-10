// GridListView.cpp : implementation file
//

#include "stdafx.h"
#include "TTVC_MFC2.h"
#include "GridListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridListView

IMPLEMENT_DYNCREATE(CGridListView, CListView)

CGridListView::CGridListView()
{
}

CGridListView::~CGridListView()
{
}


BEGIN_MESSAGE_MAP(CGridListView, CListView)
	//{{AFX_MSG_MAP(CGridListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridListView drawing

void CGridListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	pDC->
}

/////////////////////////////////////////////////////////////////////////////
// CGridListView diagnostics

#ifdef _DEBUG
void CGridListView::AssertValid() const
{
	CListView::AssertValid();
}

void CGridListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridListView message handlers
