// ChildFrm.h : interface of the CBRepChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BREPCHILDFRM_H__1FF147C2_FFCF_435A_9EB5_4ED0A53D1A8A__INCLUDED_)
#define AFX_BREPCHILDFRM_H__1FF147C2_FFCF_435A_9EB5_4ED0A53D1A8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BRepChildView.h"

class CBRepChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CBRepChildFrame)
public:
	CBRepChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBRepChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	// view for the client area of the frame.
	CBRepChildView m_wndView;
	virtual ~CBRepChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CBRepChildFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__1FF147C2_FFCF_435A_9EB5_4ED0A53D1A8A__INCLUDED_)
