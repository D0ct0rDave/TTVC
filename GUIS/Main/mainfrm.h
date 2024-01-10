// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__EFB3A2A2_4154_4A85_8A26_6A404F17B726__INCLUDED_)
#define AFX_MAINFRM_H__EFB3A2A2_4154_4A85_8A26_6A404F17B726__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MDIClient.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:
	CMDIClient m_oMDIClient;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	protected:
	//}}AFX_VIRTUAL
	
	

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnArchivoAbrir();
	afx_msg void OnArchivoExportaramidi();
	afx_msg void OnArchivoGuardartema();
	afx_msg void OnArchivoGuardartemacomo();
	afx_msg void OnArchivoNuevo();
	afx_msg void OnEdicinBloqueBorrar();
	afx_msg void OnEdicinBloqueCargar();
	afx_msg void OnEdicinBloqueCopiar();
	afx_msg void OnEdicinBloqueCortar();
	afx_msg void OnEdicinBloquePegar();
	afx_msg void OnEdicinBloqueSalvar();
	afx_msg void OnEdicinOpcionesEntradamidi();
	afx_msg void OnEdicinPatrnBorrar();
	afx_msg void OnEdicinPatrnCargar();
	afx_msg void OnEdicinPatrnCopiar();
	afx_msg void OnEdicinPatrnCortar();
	afx_msg void OnEdicinPatrnPegar();
	afx_msg void OnEdicinPatrnSalvar();
	afx_msg void OnEdicinPistaBorrar();
	afx_msg void OnEdicinPistaCargar();
	afx_msg void OnEdicinPistaCopiar();
	afx_msg void OnEdicinPistaCortar();
	afx_msg void OnEdicinPistaPegar();
	afx_msg void OnEdicinPistaProcesarefecto();
	afx_msg void OnEdicinPistaProcessarvolumen();
	afx_msg void OnEdicinPistaSalvar();
	afx_msg void OnEdicinTablasdeefectos();
	afx_msg void OnEdicinTablasdeinstrumentos();
	afx_msg void OnVerBarradeedicin();
	afx_msg void OnVerBarradereproduccion();
	afx_msg void OnVerBarradesecuencia();
	afx_msg void OnRecentfile00();
	afx_msg void OnRecentfile01();
	afx_msg void OnRecentfile02();
	afx_msg void OnRecentfile03();
	afx_msg void OnUpdateArchivoExportaramidi(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdicinTablasdeefectos(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdicinTablasdeinstrumentos(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


		
		void InicializarCancion();
		void InicializarTablaInstrumentos();
	public:
		void CargarCancionAux(AnsiString fichero);
	
	public:
		void SetupRecentFiles();
};

/////////////////////////////////////////////////////////////////////////////
extern CMainFrame*	gpMainFrame;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_MAINFRM_H__EFB3A2A2_4154_4A85_8A26_6A404F17B726__INCLUDED_)
