// TrackProperties.cpp : implementation file
//

// DMC: No se usan precompiled headers
// #include "stdafx.h"

// #include "ttvc.h"
#include <Afxdlgs.h>


#include "TrackProperties.h"
#include "Player/OMIDIOut.h"
#include "Misc/StrList.h"
#include "Misc/MiscIns.h"

#include "Misc/WinVars.h"
#include "Misc/Variables.h"

#include "Player/tt98Player.h"

#include "Misc/LangStrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// --------------------------------------------------------
// Global Variables
// --------------------------------------------------------
static TrackProperties*		gpoTrackPropGUI = NULL;

TInfoTrack*					gpoIT;
TTrackNum					gT;

TInfoTrack					goAux;
// --------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// TrackProperties dialog


TrackProperties::TrackProperties(CWnd* pParent /*=NULL*/)
	: CDialog(TrackProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(TrackProperties)n
	//}}AFX_DATA_INIT

	gpoTrackPropGUI = this;
}


void TrackProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TrackProperties)
	DDX_Control(pDX, IDC_CHECK2, m_oCKB_Ctrl32);
	DDX_Control(pDX, IDC_CHECK1, m_oCKB_Ctrl0);
	DDX_Control(pDX, IDC_EDIT1, m_oEB_TrackName);
	DDX_Control(pDX, IDC_COMBO4, m_oCB_Channel);
	DDX_Control(pDX, IDC_COMBO3, m_oCB_Inst);
	DDX_Control(pDX, IDC_COMBO2, m_oCB_Bank);
	DDX_Control(pDX, IDC_COMBO1, m_oCB_MIDIDsp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrackProperties, CDialog)
	//{{AFX_MSG_MAP(TrackProperties)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelChangeMIDIDev)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeBank)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelChangeInst)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnSelChangeChannel)
	ON_BN_CLICKED(IDC_CHECK1, OnCheckCtrl0)
	ON_BN_CLICKED(IDC_CHECK2, OnCheckCtrl32)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonInicializar)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonCargarLista)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TrackProperties message handlers


//---------------------------------------------------------------------------
BOOL TrackProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	oShape.Load("data/TPropsShape.bmp",m_hWnd,ghInst);
	oButShape.Load("data/BEdButShape.bmp",m_hWnd,ghInst);

	// Add buttons
	oBtnMgr.Init(2);
	
	// Default volume
	oBtnMgr.iAddButton( 21,290,18,15,&oButShape,54, 5,&oButShape,11, 5,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BVDefInc,NULL,m_hWnd);
	oBtnMgr.iAddButton(124,290,18,15,&oButShape,54,32,&oButShape,11,32,NULL,0,0,NULL,0,0,eBTRepeat,eBSReleased,0,BVDefDec,NULL,m_hWnd);
	oVDefLabel.Init(m_hWnd,66,287,"LCDMono2",16);
	oVDefLabel.SetColor(0x002A301E);
	oVDefLabel.SetValue(64);

	iTimerID = SetTimer(0,1000/8,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//---------------------------------------------------------------------------
bool RellenarDatosInstrumentoTrack(TInfoTrack &IT,TTrackNum T)
{
     // Salvar valores
     goAux = IT;         //Copiar valores actuales sobre extructura auxiliar
     gpoIT = &IT;        //Apuntar a estructura original
     gT    = T;
	

	if (!gpoTrackPropGUI)
	{
		gpoTrackPropGUI = new TrackProperties;
		gpoTrackPropGUI->Create(IDD_TRACKPROPS,NULL);	
	}
	
	int Res = gpoTrackPropGUI->ShowWindow(SW_SHOWNORMAL);
	return(true);

	if (Res)
		// En cualquier caso, hemos de activar la configuarcion actual
		ActivarPropiedadesMIDI(&IT);
	else
		IT = goAux;
	
	// delete poDialog;	
    return(Res);
};
//---------------------------------------------------------------------------
int TrackProperties::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

//---------------------------------------------------------------------------
// Pasar los valores de las listas, estructuras y variables a los controles
//---------------------------------------------------------------------------
void TrackProperties::GetBankList(int _iMIDIDev)
{
	// Obtener lista bancos para el dispositivo MIDI
	TStrList oBankLst;
	ObtenerListaBancos(_iMIDIDev,&oBankLst);

	m_oCB_Bank.ResetContent();
	for(int i=0;i<oBankLst.Count(); i++)
		m_oCB_Bank.AddString( oBankLst[i].c_str() );
}

void TrackProperties::GetInstList(int _iMIDIDev,int _iBank)
{
	TStrList oInstLst;
	ObtenerListaInstrumentos(_iMIDIDev,_iBank,&oInstLst);
	
	m_oCB_Inst.ResetContent();

	m_oCB_Inst.AddString(LSTR_DEF_FOR_CHAN);
	for (int i=0;i<oInstLst.Count();i++)
		m_oCB_Inst.AddString(oInstLst[i].c_str());
}
//---------------------------------------------------------------------------
// Pasar los valores de los controles a las estructuras y las variables
//---------------------------------------------------------------------------
void TrackProperties::OnOK() 
{
	// TODO: Add extra cleanup here
	ActivarPropiedadesMIDI(gpoIT);

	m_oEB_TrackName.GetWindowText(gpoIT->Nombre.CString(),35);	
	CDialog::OnOK();
}

void TrackProperties::OnCancel() 
{
	// TODO: Add extra cleanup here
	*gpoIT = goAux;
	ActivarPropiedadesMIDI(gpoIT);

	CDialog::OnCancel();
}


// ------------------------------------------------------------------------------------------
// MIDI Dev has changed
// ------------------------------------------------------------------------------------------
void TrackProperties::OnSelChangeMIDIDev() 
{
	// TODO: Add your control notification handler code here
	// Al cambiar el dispositivo, cambian los bancos y los instrumentos
	
	if (m_oCB_MIDIDsp.GetCurSel() > -1)
		gpoIT->Disp = m_oCB_MIDIDsp.GetCurSel();
    ActivarPropiedadesMIDI(gpoIT);

    
	GetInstList(gpoIT->Disp,gpoIT->Banco);
	m_oCB_Inst.SetCurSel(gpoIT->Inst+1);


	GetBankList(gpoIT->Disp);
	m_oCB_Bank.SetCurSel(gpoIT->Banco);
}
// ------------------------------------------------------------------------------------------
// Bank has changed
// ------------------------------------------------------------------------------------------
void TrackProperties::OnSelchangeBank() 
{
	// TODO: Add your control notification handler code here
	if (m_oCB_Bank.GetCurSel() > -1)
		gpoIT->Banco = m_oCB_Bank.GetCurSel();

    ActivarPropiedadesMIDI(gpoIT);

	GetInstList(gpoIT->Disp,gpoIT->Banco);
	m_oCB_Inst.SetCurSel(gpoIT->Inst+1);
}
// ------------------------------------------------------------------------------------------
// Inst has changed
// ------------------------------------------------------------------------------------------
void TrackProperties::OnSelChangeInst() 
{
	// TODO: Add your control notification handler code here
	if (m_oCB_Inst.GetCurSel() > -1)
		gpoIT->Inst = m_oCB_Inst.GetCurSel() - 1;

    ActivarPropiedadesMIDI(gpoIT);	
}
// ------------------------------------------------------------------------------------------
// Channel has changed
// ------------------------------------------------------------------------------------------
void TrackProperties::OnSelChangeChannel() 
{
	// TODO: Add your control notification handler code here
	if (m_oCB_Channel.GetCurSel() > -1)
		gpoIT->Canal = m_oCB_Channel.GetCurSel();

    ActivarPropiedadesMIDI(gpoIT);	
}
// ------------------------------------------------------------------------------------------
// Ctrl 0 Check box changed
// ------------------------------------------------------------------------------------------
void TrackProperties::OnCheckCtrl0() 
{
	// TODO: Add your control notification handler code here
	gpoIT->Ctrl0 = m_oCKB_Ctrl0.GetCheck();
    ActivarPropiedadesMIDI(gpoIT);		
}

// ------------------------------------------------------------------------------------------
// Ctrl 32 Check box changed
// ------------------------------------------------------------------------------------------
void TrackProperties::OnCheckCtrl32() 
{
	// TODO: Add your control notification handler code here
	gpoIT->Ctrl32 = m_oCKB_Ctrl32.GetCheck();
    ActivarPropiedadesMIDI(gpoIT);	
}
// ------------------------------------------------------------------------------------------
// Boton Inicializar Lista Instrumentos para el banco actual
// ------------------------------------------------------------------------------------------
void TrackProperties::OnButtonInicializar() 
{
	// TODO: Add your control notification handler code here
	// Inicializa los nombres de los instrumentos
	InicializarLista(gpoIT->Disp,gpoIT->Banco);

	// ListaBancos->Items->Strings[I->Banco] = TomarNombreBanco(I->Disp,I->Banco);
    // ListaBancos->ItemIndex=I->Banco;
	GetBankList(gpoIT->Disp);
	m_oCB_Bank.SetCurSel(gpoIT->Banco);

	GetInstList(gpoIT->Disp,gpoIT->Banco);
	m_oCB_Inst.SetCurSel(gpoIT->Inst+1);

	VG.FicherosListasInstrumentos[gpoIT->Disp][gpoIT->Banco] = "";
}
// ------------------------------------------------------------------------------------------
// Boton Cargar Lista Instrumentos para el banco actual
// ------------------------------------------------------------------------------------------
void TrackProperties::OnButtonCargarLista() 
{
	// TODO: Add your control notification handler code here
	// extern char *szOpenFile(HWND hWnd,char *_szDescFile,char *_szExt);

	CFileDialog oLoadFile(TRUE, "*.LST", NULL, NULL, "Lista de intrumentos (*.LST)|*.lst|All the files(*.*)|*.*||"); 

	if (oLoadFile.DoModal()!= IDOK)
		return;

	char szFileName[256];
	strcpy(szFileName, oLoadFile.GetPathName());

	CargarLista(gpoIT->Disp,gpoIT->Banco,szFileName);

	// ListaBancos->Items->Strings[I->Banco] = TomarNombreBanco(I->Disp,I->Banco);
    // ListaBancos->ItemIndex=I->Banco;
	GetBankList(gpoIT->Disp);
	m_oCB_Bank.SetCurSel(gpoIT->Banco);

	GetInstList(gpoIT->Disp,gpoIT->Banco);
	m_oCB_Inst.SetCurSel(gpoIT->Inst+1);

    // Actualizar lista de ficheros de configuración
	VG.FicherosListasInstrumentos[gpoIT->Disp][gpoIT->Banco] = szFileName;

	UpdateData(1);
}
// ------------------------------------------------------------------------------------------

void TrackProperties::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	int i;
	     
	// Change window Caption
	char szString[256]; sprintf(szString,"Propiedades de la pista: %d",gT);
	SetWindowText(szString);

	// Add MIDI output devices
	m_oCB_MIDIDsp.ResetContent();
	int iNumMIDIOutDevs = NumeroDispositivosMIDIOut();
    for (i=0;i<iNumMIDIOutDevs;i++)
        m_oCB_MIDIDsp.AddString( ObtenerNombreDispositivoMidiOut(i) );

	m_oCB_MIDIDsp.SetCurSel(gpoIT->Disp);

	// Add midi channels to the list
	m_oCB_Channel.ResetContent();
	for(i=0;i<16; i++)
	{
		char szStr[16];
		sprintf(szStr,"%d",i);
		m_oCB_Channel.AddString(szStr);
	}
	m_oCB_Channel.SetCurSel(gpoIT->Canal);

	// Add bank list
	GetBankList(gpoIT->Disp);
	m_oCB_Bank.SetCurSel(gpoIT->Banco);

	// Add inst list
	GetInstList(gpoIT->Disp,gpoIT->Banco);
	m_oCB_Inst.SetCurSel(gpoIT->Inst+1);


     // Copiar valores de la estructura InfoInst a los sus controles respectivos     
	m_oEB_TrackName.SetWindowText(gpoIT->Nombre.CString());
	
	// Controladores	
	m_oCKB_Ctrl0.SetCheck ( gpoIT->Ctrl0  );
	m_oCKB_Ctrl32.SetCheck( gpoIT->Ctrl32 );
     
	// Default volume
	// m_oSLD_DefVol.SetRange(0,127);
	// m_oSLD_DefVol.SetPos(gpoIT->VolDef);
	
	// Label
	// char szStr[4];	sprintf(szStr,"%d",gpoIT->VolDef);
	// m_oEB_DefVol.SetWindowText(szStr);
	
	oVDefLabel.SetValue(gpoIT->VolDef);	
}

void TrackProperties::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	BitBlt(dc.m_hDC,0,0,oShape.iGetTX(),oShape.iGetTY(),oShape.GetDC(),0,0,SRCCOPY);
	oBtnMgr.Draw();
	oVDefLabel.Draw();

	// Do not call CDialog::OnPaint() for painting messages
}
// ------------------------------------------------------------------------------------------
// Button Manager events
// ------------------------------------------------------------------------------------------
void TrackProperties::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	oBtnMgr.MouseDown(point.x,point.y);
	CDialog::OnLButtonDown(nFlags, point);
}

void TrackProperties::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	oBtnMgr.MouseUp(point.x,point.y);
	CDialog::OnLButtonUp(nFlags, point);
}

void TrackProperties::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	oBtnMgr.MouseMove(point.x,point.y);	
	CDialog::OnMouseMove(nFlags, point);
}

void TrackProperties::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	oBtnMgr.Timer();
	CDialog::OnTimer(nIDEvent);
}
// ------------------------------------------------------------------------------------------
// Button events
// ------------------------------------------------------------------------------------------
void TrackProperties::BVDefInc()
{
	if (gpoIT->VolDef<127) gpoIT->VolDef++;
	gpoTrackPropGUI->oVDefLabel.SetValue(gpoIT->VolDef);
}

void TrackProperties::BVDefDec()
{
	if (gpoIT->VolDef>0) gpoIT->VolDef--;
	gpoTrackPropGUI->oVDefLabel.SetValue(gpoIT->VolDef);
}
// ------------------------------------------------------------------------------------------
LRESULT TrackProperties::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	// Process messages
	switch (message)
	{
		case WM_KEYUP:
		case WM_KEYDOWN:
		{
			// This allows accelerators to be processed
			MSG msg;	
			msg.hwnd    = m_hWnd;
			msg.lParam  = lParam;
			msg.wParam  = wParam;
			msg.message = message;
		
			TranslateAccelerator(ghMainWnd,ghAccel,&msg);				

			/*
				// return CListBox::WindowProc(message,wParam,lParam);
			*/
			
			// This allows keydown events directed to PatternEditor to be processed
			::SendMessage(ghMainWnd,message,wParam,lParam);

		}
		
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
// ------------------------------------------------------------------------------------------
