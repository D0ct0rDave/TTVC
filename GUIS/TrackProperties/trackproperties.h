#if !defined(AFX_TrackProperties_H__2587F4B4_BC7C_47EC_BD5B_E08E7559ECED__INCLUDED_)
#define AFX_TrackProperties_H__2587F4B4_BC7C_47EC_BD5B_E08E7559ECED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrackProperties.h : header file
//
#include <Afxwin.h>
#include <afxcmn.h>
#include "tipos.h"
#include "resource.h"

#include "Controls/BMPResource.h"
#include "Controls/BtnMgr.h"
#include "Controls/Label.h"

/////////////////////////////////////////////////////////////////////////////
// TrackProperties dialog

class TrackProperties : public CDialog
{
// Construction
public:
	TrackProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TrackProperties)
	enum { IDD = IDD_TRACKPROPS };
	CButton	m_oCKB_Ctrl32;
	CButton	m_oCKB_Ctrl0;
	CEdit	m_oEB_TrackName;
	CComboBox	m_oCB_Channel;
	CComboBox	m_oCB_Inst;
	CComboBox	m_oCB_Bank;
	CComboBox	m_oCB_MIDIDsp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TrackProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TrackProperties)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelChangeMIDIDev();
	afx_msg void OnSelchangeBank();
	afx_msg void OnSelChangeInst();
	afx_msg void OnSelChangeChannel();
	afx_msg void OnCheckCtrl0();
	afx_msg void OnCheckCtrl32();
	afx_msg void OnButtonInicializar();
	afx_msg void OnButtonCargarLista();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	public:
		void GetBankList(int _iMIDIDev);
		void GetInstList(int _iMIDIDev,int _iBank);
	
	protected:
		CBMPResource	oShape;
		CBMPResource	oButShape;
		
		UINT			iTimerID;
		CBtnMgr			oBtnMgr;

		// Button events
		CLabel		oVDefLabel;
		static void BVDefInc();
		static void BVDefDec();
};

/// -----------------------------------------------------------------
bool RellenarDatosInstrumentoTrack(TInfoTrack &IT,TTrackNum T);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_TrackProperties_H__2587F4B4_BC7C_47EC_BD5B_E08E7559ECED__INCLUDED_)
