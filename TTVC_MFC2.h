// TTVC_MFC2.h : main header file for the TTVC_MFC2 application
//

#if !defined(AFX_TTVC_MFC2_H__563DF841_3FD2_4995_B4E7_D9BADCB5F6D0__INCLUDED_)
#define AFX_TTVC_MFC2_H__563DF841_3FD2_4995_B4E7_D9BADCB5F6D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTTVC_MFC2App:
// See TTVC_MFC2.cpp for the implementation of this class
//

class CTTVC_MFC2App : public CWinApp
{
public:
	CTTVC_MFC2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTTVC_MFC2App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	bool bCreatePatternEditorWindow();
	bool bCreateBarraEdicionWindow();
	bool bCreateBRepWindow();
	bool bCreateBSeqWindow();
	void LoadLocalStrings();

// Implementation
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;

public:
	//{{AFX_MSG(CTTVC_MFC2App)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTVC_MFC2_H__563DF841_3FD2_4995_B4E7_D9BADCB5F6D0__INCLUDED_)
