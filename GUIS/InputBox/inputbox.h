#if !defined(AFX_INPUTBOX_H__0FAA803B_B150_4387_966A_3E89A8413AAF__INCLUDED_)
#define AFX_INPUTBOX_H__0FAA803B_B150_4387_966A_3E89A8413AAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBox.h : header file
//
bool bInputBox(char *_szTitle,char *_szCaption,char *_szText,int _iTextLen);

/////////////////////////////////////////////////////////////////////////////
// CInputBox dialog

class CInputBox : public CDialog
{
// Construction
public:
	CInputBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBox)
	enum { IDD = IDD_INPUTBOX };
	CEdit	m_oEd_Caption;
	CEdit	m_oEd_Text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBox)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBOX_H__0FAA803B_B150_4387_966A_3E89A8413AAF__INCLUDED_)
