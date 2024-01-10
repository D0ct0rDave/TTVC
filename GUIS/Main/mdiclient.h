#if !defined(AFX_MDICLIENT_H__5F7DD178_D560_45FE_942C_C6D36642A544__INCLUDED_)
#define AFX_MDICLIENT_H__5F7DD178_D560_45FE_942C_C6D36642A544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MDIClient.h : header file
//



#include "Controls/BMPResource.h"
#include "Controls/BtnMgr.h"
#include "Misc/RList.h"


/////////////////////////////////////////////////////////////////////////////
// CMDIClient window

class CMDIClient : public CWnd
{
		// Construction
	public:
			CMDIClient();
	
		// Attributes
	public:
	
		// Operations
	public:
		int	iInit(HWND _hWnd,HINSTANCE _hInst);

		void Draw();			
		void FillBackground();

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMDIClient)
	protected:
	virtual void PreSubclassWindow();
	public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
		
		// Implementation
	public:
		virtual ~CMDIClient();		

	// Generated message map functions
	protected:
		//{{AFX_MSG(CMDIClient)
		afx_msg void OnPaint();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()


public:
		AnsiString GetConfigDir();
		void LeerConfig();
		void EscribirConfig();
		void InitSongStructs();

public:
		CBMPResource			oBck;
		CBMPResource			oBMPRes;
		HDC						hDC;
		HWND					hWnd;
		HINSTANCE				hInst;
	
		int						iWidth;
		int						iHeight;
	

		RECT					oUpdRect;
	
		CBtnMgr					oBtnMgr;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICLIENT_H__5F7DD178_D560_45FE_942C_C6D36642A544__INCLUDED_)
