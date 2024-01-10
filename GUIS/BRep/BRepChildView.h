#if !defined(AFX_BREPCHILDVIEW_H__53D2B90F_3E64_4D28_B4B9_AA5695D2C491__INCLUDED_)
#define AFX_BREPCHILDVIEW_H__53D2B90F_3E64_4D28_B4B9_AA5695D2C491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BRep.h : header file
//

#include "Controls/BMPResource.h"
#include "Controls/BtnMgr.h"


/////////////////////////////////////////////////////////////////////////////
// CBRepChildView window

class CBRepChildView : public CWnd
{
// Construction
public:
	CBRepChildView();
	int iInit(HWND _hWnd,HINSTANCE _hInst);


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBRepChildView)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual ~CBRepChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBRepChildView)
		// NOTE - the ClassWizard will add and remove member functions here.
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



	CBtnMgr					oBtnMgr;
	CBMPResource			oBMPRes;
	HDC						hDC;
	HWND					hWnd;
	HINSTANCE				hInst;
	RECT					oUpdRect;


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BREP_H__53D2B90F_3E64_4D28_B4B9_AA5695D2C491__INCLUDED_)
