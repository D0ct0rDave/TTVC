#if !defined(AFX_BSEQ_H__6356427B_BB4F_43AE_B5B0_03DC0E1163F2__INCLUDED_)
#define AFX_BSEQ_H__6356427B_BB4F_43AE_B5B0_03DC0E1163F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BSeq.h : header file
//

#include "resource.h"

class CSeqListBox: public CListBox
{
	public:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

class CMyEditDblCl : public CEdit
{
	public:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	public:
		char *m_szStr;
};

/////////////////////////////////////////////////////////////////////////////
// CBSeq dialog
class CBSeq : public CDialog
{
// Construction
public:
	CBSeq(CWnd* pParent = NULL);   // standard constructor

protected:
	// Dialog Data
	//{{AFX_DATA(CBSeq)
	enum { IDD = IDD_BSEQ };
	CStatic		m_oSt_Autor;
	CStatic		m_oSt_Title;
	CSeqListBox	m_oLst_Seq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBSeq)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CBSeq)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeListSeq();
	afx_msg void OnDblclkListSeq();
	afx_msg void OnStTitle();
	afx_msg void OnStAutor();
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void UpdateContents();
	void ObtenerSecuencia();
	int iCurSeqPos();
	void SetCurSeqPos(int _iIdx);
	bool KeyDownHandler(unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode);
	bool KeyUpHandler  (unsigned int _uiKeyFlags,unsigned short key,unsigned char scancode);
};

extern CBSeq *gpoBSeq;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BSEQ_H__6356427B_BB4F_43AE_B5B0_03DC0E1163F2__INCLUDED_)
