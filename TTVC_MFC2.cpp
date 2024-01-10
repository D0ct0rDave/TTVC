// TTVC_MFC2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TTVC_MFC2.h"

#include "Misc/WinVars.h"
#include "Misc/Variables.h"
#include "Misc/Config.h"

#include "GUIS/Main/MainFrm.h"
#include "GUIS/BRep/BRepChildFrm.h"
#include "GUIS/BEd/BEd.h"
#include "GUIS/BRep/BRep.h"
#include "GUIS/BSeq/BSeq.h"

#ifdef MFC_PATTED
#include "GUIS\PattEd\ChildFrm.h"
#else
#include "GUIS\PattEd\PattEd.h"
#endif


#include "CrtError.h"
#include "Misc/LangStrs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDM_WINDOWCHILD 4100


char gszIniFilename[1024];

// Global Variables:
TCHAR szTitle[MAX_LOADSTRING];						// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];				// The title bar text

/////////////////////////////////////////////////////////////////////////////
// CTTVC_MFC2App

BEGIN_MESSAGE_MAP(CTTVC_MFC2App, CWinApp)
	//{{AFX_MSG_MAP(CTTVC_MFC2App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTTVC_MFC2App construction

CTTVC_MFC2App::CTTVC_MFC2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTTVC_MFC2App object

CTTVC_MFC2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CTTVC_MFC2App initialization


// From codeguru (cut&paste)
char gszAppPath[MAX_PATH]      = "";
char gszAppDirectory[MAX_PATH] = "";
char gszIniFile[MAX_PATH] = "";

void RetrieveAppDirectory()
{
	// With standard string
	::GetModuleFileName(0, gszAppPath, sizeof(gszAppPath) - 1);

	// Extract directory
	strncpy(gszAppDirectory, gszAppPath, strrchr(gszAppPath, '\\') - gszAppPath);
	gszAppDirectory[strlen(gszAppDirectory)] = '\0';

	sprintf(gszIniFile, "%s\\data\\tt.ini",gszAppDirectory);
}

BOOL CTTVC_MFC2App::InitInstance()
{
	// Get app instance first of all
	ghInst		= AfxGetResourceHandle();
	gszAppTitle = LSTR_APP_TITLE;
	
	giScrTX = GetSystemMetrics(SM_CXSCREEN);
	giScrTY = GetSystemMetrics(SM_CYSCREEN);

	AfxEnableControlContainer();

	// Retrieve ini filename
	RetrieveAppDirectory();

	ReadConfig(gszIniFile);
	LoadLocalStrings();
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// Child and parent frame uses the same menu resource. Callbacks should be placed in the same
	// file.

	// Asi hacemos que los eventos de menu sean iguales tanto si es la ventana principal
	// la que tiene el foco como si es la ventana "hija"
	m_hMDIMenu  = ::LoadMenu(ghInst, MAKEINTRESOURCE(IDR_MAINFRAME_ACCEL)); // IDR_TTVC_MTYPE));
	m_hMDIAccel = ::LoadAccelerators(ghInst, MAKEINTRESOURCE(IDR_MAINFRAME_MENU)); // IDR_TTVC_MTYPE));
	ghAccel = m_hMDIAccel;


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	gpMainFrame= new CMainFrame;
	m_pMainWnd = gpMainFrame;

	// create main MDI frame window
		
	if (!gpMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	ghMainWnd  = gpMainFrame->m_hWnd;
	
	gpMainFrame->SetIcon(LoadIcon(IDR_MAINFRAME),true);
	gpMainFrame->SetIcon(LoadIcon(IDR_MAINFRAME),false);
/*
	if (! gpMainFrame->Create(RUNTIME_CLASS(CMainFrame),"The Tracker",
						WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
						gpMainFrame->rectDefault,NULL)
						)
		return FALSE;
*/

	// try to load shared MDI menus and accelerator table
	// TODO: add additional member variables and load calls for
	//	additional menu types your application may need. 


	// The main window has been initialized, so show and update it.
	gpMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	gpMainFrame->UpdateWindow();

	gpMainFrame->SetupRecentFiles();

	// -------------------------------------------------------------------
	// Create NON MFC Windows
	// -------------------------------------------------------------------
	ghMDIClWnd= gpMainFrame->m_oMDIClient.m_hWnd;

	bCreateBRepWindow();
	bCreateBarraEdicionWindow();

	// Create child window (Pattern Editor)
	#ifndef MFC_PATTED
	bCreatePatternEditorWindow();
	#else
	gpMainFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_TTVC_MTYPE, m_hMDIMenu);	
	#endif

	// -------------------------------------------------------------------
	// Create Windows
	// -------------------------------------------------------------------
	bCreateBSeqWindow();

	// -------------------------------------------------------------------
	gpMainFrame->GetMenu()->CheckMenuItem(ID_VER_BARRADEEDICIN		,gbBEdWndVis ?MF_CHECKED:MF_UNCHECKED);
	gpMainFrame->GetMenu()->CheckMenuItem(ID_VER_BARRADEREPRODUCCION,gbBRepWndVis?MF_CHECKED:MF_UNCHECKED);
	gpMainFrame->GetMenu()->CheckMenuItem(ID_VER_BARRADESECUENCIA	,gbBSeqWndVis?MF_CHECKED:MF_UNCHECKED);
	// -------------------------------------------------------------------
	
	// Load any file?
	if (__argc == 2)
	{
		char szFilename[256];
		// Warning, "open with..." command passes argument with coutes!!!

		// Process input filename
		char *szInFilename  = m_lpCmdLine;
		char *szOutFilename = szFilename;
		
		do{

			if (*szInFilename != '"')
			{
				*szOutFilename = *szInFilename;
				szOutFilename++;
			}

			szInFilename++;

		}while (*szInFilename);

		// Terninate string
		*szOutFilename = 0;

		// Load song
		gpMainFrame->CargarCancionAux(szFilename);
	}

	// Set focus on main window
	SetFocus (ghPattEdWnd);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTTVC_MFC2App message handlers

int CTTVC_MFC2App::ExitInstance() 
{
	//TODO: handle additional resources you may have added
	WriteConfig(gszIniFile);


	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTTVC_MFC2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTTVC_MFC2App message handlers

// ------------------------------------------------------------------------
bool CTTVC_MFC2App::bCreateBarraEdicionWindow()
{
	WNDCLASSEX	wcex;
	TCHAR		szWndClassName[MAX_LOADSTRING];
	TCHAR		szWndTitle[MAX_LOADSTRING];

	// -------------------------------------------------------------------
	// Register Edition Bar
	// -------------------------------------------------------------------
	LoadString(ghInst, IDS_BEDWND_CLASSNAME, szWndClassName, MAX_LOADSTRING);
	
	memset(&wcex,0,sizeof(WNDCLASSEX));
	wcex.lpfnWndProc	= (WNDPROC)BEd::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.hIcon			= LoadIcon(IDI_ICON1);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWndClassName;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.hIconSm		= LoadIcon(IDI_ICON1);

	if (! RegisterClassEx(&wcex))
		return FALSE;



	// -------------------------------------------------------------------
	// Creat+e Edition Bar
	// -------------------------------------------------------------------
	// Create the MDI client filling the client area
	CLIENTCREATESTRUCT ccs;

	// Find window menu where children will be listed
	LoadString(ghInst, IDS_BEDWND_CLASSNAME, szWndClassName, MAX_LOADSTRING);
	LoadString(ghInst, IDS_BEDWND_TITLE    , szWndTitle    , MAX_LOADSTRING);
	
	// Obtener dimensiones del modo de video (para poner la ventana en la otra esquina)
	DWORD		dwWndFlags;	
	dwWndFlags = WS_POPUP | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
				// WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_DLGFRAME | WS_CAPTION,
				// WS_DLGFRAME | WS_CAPTION | WS_OVERLAPPEDWINDOW | WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_OVERLAPPED,
	if (gbBEdWndVis) dwWndFlags |= WS_VISIBLE;

	ghBEdWnd = CreateWindow(szWndClassName,
							szWndTitle,
							dwWndFlags,
							giScrTX  - giBEdWTX,
							0,
							giBEdWTX,
							giBEdWTY,
							ghMDIClWnd,
							NULL,
							ghInst,
							NULL);
	// -------------------------------------------------------------------
	// Init Edition Bar
	// -------------------------------------------------------------------
	BEd::iInit(ghBEdWnd,ghInst);

	// -------------------------------------------------------------------
	// Show Edition Bar
	// -------------------------------------------------------------------
	UpdateWindow(ghBEdWnd);
	SetFocus (ghBEdWnd);

	HMENU hMenu = GetSystemMenu(ghBEdWnd,FALSE);
	DeleteMenu(hMenu,SC_CLOSE, MF_BYCOMMAND);
	// EnableMenuItem(hMenu,SC_CLOSE,		MF_BYCOMMAND | /*MF_GRAYED | */MF_DISABLED);

	return(TRUE);
}

// ------------------------------------------------------------------------
bool CTTVC_MFC2App::bCreatePatternEditorWindow()
{
	#ifndef MFC_PATTED
	WNDCLASSEX	wcex;
	TCHAR		szWndClassName[MAX_LOADSTRING];
	TCHAR		szWndTitle[MAX_LOADSTRING];
	
	LoadString(ghInst, IDS_PATTEDWND_CLASSNAME, szWndClassName, MAX_LOADSTRING);
	LoadString(ghInst, IDS_PATTEDWND_TITLE	  , szWndTitle    , MAX_LOADSTRING);

	// -------------------------------------------------------------------
	// Register main window
	// -------------------------------------------------------------------
	HBRUSH Brush		= CreateSolidBrush(VG.PDP.PattEdBk);
	wcex.cbSize			= sizeof(WNDCLASSEX);

	wcex.style			= 0; // CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)PattEd::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 8;
	wcex.hInstance		= ghInst;
	wcex.hIcon			= LoadIcon((LPCTSTR)IDI_ICON1);
	wcex.hCursor		= LoadCursor(IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(CreateSolidBrush(VG.PDP.PattEdBk)); 
	// wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWndClassName;
	wcex.hIconSm		= LoadIcon(IDI_ICON1);

	if (! RegisterClassEx(&wcex))
		return FALSE;

	// -------------------------------------------------------------------
	// Create PatEd Bar
	// -------------------------------------------------------------------
	DWORD		dwWndFlags;
	dwWndFlags = WS_BORDER | WS_CAPTION | WS_OVERLAPPEDWINDOW | WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_OVERLAPPED;
	if (gbPattEdWndVis) dwWndFlags |= WS_VISIBLE;

	ghPattEdWnd= CreateWindow(szWndClassName,
							szWndTitle,
							dwWndFlags,
							0,
							0,
							640,
							480,
							ghMDIClWnd,
							NULL,
							ghInst,
							NULL);	
	// -------------------------------------------------------------------
	// Init Pattern Editor
	// -------------------------------------------------------------------
	PattEd::iInit(ghPattEdWnd,ghInst);

	// -------------------------------------------------------------------
	// Show Edition Bar
	// -------------------------------------------------------------------
	ShowWindow(ghPattEdWnd,SW_SHOWMAXIMIZED);
	UpdateWindow(ghPattEdWnd);
	SetFocus (ghPattEdWnd);
	#endif

	HMENU hMenu = GetSystemMenu(ghPattEdWnd,FALSE);
	DeleteMenu(hMenu,SC_CLOSE, MF_BYCOMMAND);
	// EnableMenuItem(hMenu,SC_CLOSE,		MF_BYCOMMAND | /*MF_GRAYED | */MF_DISABLED);

	return(TRUE);
}

// ------------------------------------------------------------------------
bool CTTVC_MFC2App::bCreateBRepWindow()
{
	WNDCLASSEX	wcex;
	TCHAR		szWndClassName[MAX_LOADSTRING];
	TCHAR		szWndTitle[MAX_LOADSTRING];

	// -------------------------------------------------------------------
	// Register Edition Bar
	// -------------------------------------------------------------------
	LoadString(ghInst, IDS_BREPWND_CLASSNAME, szWndClassName, MAX_LOADSTRING);
	
	memset(&wcex,0,sizeof(WNDCLASSEX));
	wcex.lpfnWndProc	= (WNDPROC)BRep::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.hIcon			= LoadIcon(IDI_ICON1);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWndClassName;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.hIconSm		= LoadIcon(IDI_ICON1);

	if (! RegisterClassEx(&wcex))
		return FALSE;

	// -------------------------------------------------------------------
	// Create Edition Bar
	// -------------------------------------------------------------------
	// Create the MDI client filling the client area
	CLIENTCREATESTRUCT ccs;

	// Find window menu where children will be listed
	LoadString(ghInst, IDS_BREPWND_CLASSNAME, szWndClassName, MAX_LOADSTRING);
	LoadString(ghInst, IDS_BREPWND_TITLE    , szWndTitle    , MAX_LOADSTRING);

	DWORD		dwWndFlags;
	dwWndFlags = WS_POPUP | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	if (gbBRepWndVis) dwWndFlags |= WS_VISIBLE;

	ghBRepWnd= CreateWindow(szWndClassName,
							szWndTitle,
							dwWndFlags,
							giScrTX - giBRepWTX,
							giScrTY - giBRepWTY,
							giBRepWTX,
							giBRepWTY,
							ghMDIClWnd,
							NULL,
							ghInst,
							NULL);

	// -------------------------------------------------------------------
	// Init Edition Bar
	// -------------------------------------------------------------------
	BRep::iInit(ghBRepWnd,ghInst);

	// -------------------------------------------------------------------
	// Show Edition Bar
	// -------------------------------------------------------------------
	UpdateWindow(ghBRepWnd);
	SetFocus (ghBRepWnd);


	HMENU hMenu = GetSystemMenu(ghBRepWnd,FALSE);
	DeleteMenu(hMenu,SC_CLOSE, MF_BYCOMMAND);

	//disable close window button
	// EnableMenuItem(hMenu,SC_MINIMIZE,	MF_BYCOMMAND | MF_ENABLED);
	// EnableMenuItem(hMenu,SC_CLOSE,		MF_BYCOMMAND | /*MF_GRAYED | */MF_DISABLED);

	return(TRUE);
}
// ------------------------------------------------------------------------
bool CTTVC_MFC2App::bCreateBSeqWindow()
{
/*
	DWORD dwWndFlags = WS_POPUP | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	
	RECT oRect;
	oRect.left	= giScrTX - giBRepWTX;
	oRect.right	= iWidth;
	oRect.top	= giScrTY - giBRepWTY;
	oRect.bottom= iHeight;
	// ((CWnd*)gpoBSeq)->Create("BarraSeq","CacaSeq",dwWndFlags,oRect,NULL,0,NULL);
	
*/
	gpoBSeq = new CBSeq;
	gpoBSeq->Create((UINT)IDD_BSEQ,NULL);
	gpoBSeq->ShowWindow(gbBSeqWndVis);
	ghBSeqWnd = gpoBSeq->m_hWnd;
	SetWindowPos(ghBSeqWnd,
				HWND_NOTOPMOST,
				giScrTX - giBSeqWTX,
				giScrTY - giBSeqWTY - giBRepWTY,
				giBSeqWTX,
				giBSeqWTY,0);

	HMENU hMenu = GetSystemMenu(ghBSeqWnd,FALSE);	
	DeleteMenu(hMenu,SC_CLOSE, MF_BYCOMMAND);
	
	gpoBSeq->SetIcon(LoadIcon(IDI_ICON1),true);
	gpoBSeq->SetIcon(LoadIcon(IDI_ICON1),false);

	// gpMainFrame->CreateNewChild(RUNTIME_CLASS(CSeqEd), IDR_TTVC_MTYPE, m_hMDIMenu);	
	return(TRUE);
}
// ------------------------------------------------------------------------
void CTTVC_MFC2App::LoadLocalStrings()
{
	// 
	LoadString(ghInst, AFX_IDS_APP_TITLE,		gsz_APP_NAME,			MAX_LOADSTRING);
	LoadString(ghInst, AFX_SURE_TO_INIT_SONG,	gsz_SURE_TO_INIT_SONG,	MAX_LOADSTRING);	
}
// ------------------------------------------------------------------------
