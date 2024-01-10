// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TTVC_MFC2.h"
#include "Misc/winvars.h"
#include "Misc/LangStrs.h"
#include "MainFrm.h"

#include "GUIS/PattED/PattEd.h"
#include "GUIS/BRep/BRep.h"
#include "GUIS/BSeq/BSeq.h"
#include "GUIS/BEd/BEd.h"

#include "Misc/Msgs.h"

#include "CrtError.h"
#include "Inter.h"
#include "Variables.h"
#include "Ficheros.h"
#include "Player\tt98Player.h"

#include "Misc/TckAMID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMainFrame*	gpMainFrame;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_ARCHIVO_ABRIR, OnArchivoAbrir)
	ON_COMMAND(ID_ARCHIVO_EXPORTARAMIDI, OnArchivoExportaramidi)
	ON_COMMAND(ID_ARCHIVO_GUARDARTEMA, OnArchivoGuardartema)
	ON_COMMAND(ID_ARCHIVO_GUARDARTEMACOMO, OnArchivoGuardartemacomo)
	ON_COMMAND(ID_ARCHIVO_NUEVO, OnArchivoNuevo)
	ON_COMMAND(ID_EDICIN_BLOQUE_BORRAR, OnEdicinBloqueBorrar)
	ON_COMMAND(ID_EDICIN_BLOQUE_CARGAR, OnEdicinBloqueCargar)
	ON_COMMAND(ID_EDICIN_BLOQUE_COPIAR, OnEdicinBloqueCopiar)
	ON_COMMAND(ID_EDICIN_BLOQUE_CORTAR, OnEdicinBloqueCortar)
	ON_COMMAND(ID_EDICIN_BLOQUE_PEGAR, OnEdicinBloquePegar)
	ON_COMMAND(ID_EDICIN_BLOQUE_SALVAR, OnEdicinBloqueSalvar)
	ON_COMMAND(ID_EDICIN_OPCIONES_ENTRADAMIDI, OnEdicinOpcionesEntradamidi)
	ON_COMMAND(ID_EDICIN_PATRN_BORRAR, OnEdicinPatrnBorrar)
	ON_COMMAND(ID_EDICIN_PATRN_CARGAR, OnEdicinPatrnCargar)
	ON_COMMAND(ID_EDICIN_PATRN_COPIAR, OnEdicinPatrnCopiar)
	ON_COMMAND(ID_EDICIN_PATRN_CORTAR, OnEdicinPatrnCortar)
	ON_COMMAND(ID_EDICIN_PATRN_PEGAR, OnEdicinPatrnPegar)
	ON_COMMAND(ID_EDICIN_PATRN_SALVAR, OnEdicinPatrnSalvar)
	ON_COMMAND(ID_EDICIN_PISTA_BORRAR, OnEdicinPistaBorrar)
	ON_COMMAND(ID_EDICIN_PISTA_CARGAR, OnEdicinPistaCargar)
	ON_COMMAND(ID_EDICIN_PISTA_COPIAR, OnEdicinPistaCopiar)
	ON_COMMAND(ID_EDICIN_PISTA_CORTAR, OnEdicinPistaCortar)
	ON_COMMAND(ID_EDICIN_PISTA_PEGAR, OnEdicinPistaPegar)
	ON_COMMAND(ID_EDICIN_PISTA_PROCESAREFECTO, OnEdicinPistaProcesarefecto)
	ON_COMMAND(ID_EDICIN_PISTA_PROCESSARVOLUMEN, OnEdicinPistaProcessarvolumen)
	ON_COMMAND(ID_EDICIN_PISTA_SALVAR, OnEdicinPistaSalvar)
	ON_COMMAND(ID_EDICIN_TABLASDEEFECTOS, OnEdicinTablasdeefectos)
	ON_COMMAND(ID_EDICIN_TABLASDEINSTRUMENTOS, OnEdicinTablasdeinstrumentos)
	ON_COMMAND(ID_VER_BARRADEEDICIN, OnVerBarradeedicin)
	ON_COMMAND(ID_VER_BARRADEREPRODUCCION, OnVerBarradereproduccion)
	ON_COMMAND(ID_VER_BARRADESECUENCIA, OnVerBarradesecuencia)
	ON_COMMAND(ID_RECENTFILE_00, OnRecentfile00)
	ON_COMMAND(ID_RECENTFILE_01, OnRecentfile01)
	ON_COMMAND(ID_RECENTFILE_02, OnRecentfile02)
	ON_COMMAND(ID_RECENTFILE_03, OnRecentfile03)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVO_EXPORTARAMIDI, OnUpdateArchivoExportaramidi)
	ON_UPDATE_COMMAND_UI(ID_EDICIN_TABLASDEEFECTOS, OnUpdateEdicinTablasdeefectos)
	ON_UPDATE_COMMAND_UI(ID_EDICIN_TABLASDEINSTRUMENTOS, OnUpdateEdicinTablasdeinstrumentos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	
	VERIFY( m_oMDIClient.SubclassWindow( m_hWndMDIClient ) );
	m_oMDIClient.iInit(m_oMDIClient.m_hWnd,lpCreateStruct->hInstance);	
	
	//
	
	/*
	CMenu *poMenu = GetMenu();
	m_oMDIClient.SetMenu( poMenu );
	*/
	// CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	
	// Create reproduction panel
	// CreateNewChild(RUNTIME_CLASS(CBRepChildFrame), IDR_TTVC_MTYPE, lpCreateStruct->hMenu/*m_hMDIMenu*/);	
	


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	// return CMDIFrameWnd::WindowProc(message, wParam, lParam);

	switch (message)
	{
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))		
			{
				// Send messages to the client MDI window
				/*
				case SC_MAXIMIZE:
				case SC_MINIMIZE:
				case SC_RESTORE:
				case SC_CLOSE:				
				case ID_FILE_NEW:
				case ID_FILE_OPEN:
				case ID_FILE_SAVE:

					m_oMDIClient.WindowProc(message,wParam,lParam);
				break;
				*/

				case ID_BPM_INC:
				BRep::oBtnMgr.PushButton(6);
				break;

				case ID_BPM_DEC:
				BRep::oBtnMgr.PushButton(7);
				break;

				case ID_ENABLE_EDITION:
				BEd::oBtnMgr.PushButton(20);
				break;

			default:
				return CMDIFrameWnd::WindowProc(message, wParam, lParam);
			}
		};
		break;

		case WM_CLOSE:
		{
			if (MessageBox(LSTR_ARE_YOU_SURE_MSG,LSTR_APP_TITLE,MB_OKCANCEL) == IDOK)
			{
				DestroyWindow();
			}
		}
		break;

		// Pasamos mensajes primero a MDI client, si no los utiliza
		// los procesamos aqui
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			if (m_oMDIClient.WindowProc(message,wParam,lParam))
				return CMDIFrameWnd::WindowProc(message, wParam, lParam);
		
		}
		break;

		default:
			return CMDIFrameWnd::WindowProc(message, wParam, lParam);
	}

	return(0);
}
//------------------------------------------------------------------------------
void CMainFrame::OnFileNew() 
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnFileOpen() 
{
	// TODO: Add your command handler code here

}
//------------------------------------------------------------------------------
// Procedimiento auxiliar que tambien servirá para cargar archivos de la RecentFilesList
//------------------------------------------------------------------------------
void CMainFrame::CargarCancionAux(AnsiString fichero)
{
     if (CargarCancion(fichero)) 
     {
          VG.NombreFichero = fichero;
          VG.oRcntFiles.AnyadirElemento(fichero);
		  SetupRecentFiles();

          // BS    ->ObtenerSecuencia();
          // BPMDis->Cadena=IntToStr(VG.Sng->BPMS)+" BPMS";          

		  EstablecerBeatsPerMinute(VG.Sng->BPMS);

		  BRep::UpdateContents();
		  gpoBSeq->UpdateContents();
		  PattEd::UpdateContents();

          ResetSistemaMIDI();
          // Caption = VG.Sng->Titulo.CString();
     }
     else
     {
          InicializarCancion();

		  EstablecerBeatsPerMinute(VG.Sng->BPMS);

		  BRep::UpdateContents();
		  gpoBSeq->UpdateContents();
		  PattEd::UpdateContents();

          // BS->ObtenerSecuencia();
          // BPMDis->Cadena=IntToStr(VG.Sng->BPMS)+" BPMS";
          // PattEd::UpdateContents();

          VG.NombreFichero="";
          // Caption      = VG.Sng->Titulo.CString();
     }
}

//------------------------------------------------------------------------------
void CMainFrame::InicializarCancion()
{
     //Inicialización de todas las variables
     //CrearTrack(&TrackAux);        // Track para copiar/pegar ...
     CrearCancion(VG.Sng);           // Variable cancion global
     VG.PatronActual =VG.Sng->Pats[0];
     VG.IndSecActual =0;
     VG.PatternActual=0;

     InicializarTablaInstrumentos();
}
//------------------------------------------------------------------------------
void CMainFrame::InicializarTablaInstrumentos()
{	
	unsigned char I;
    
	for (I=0;I<=MaxTracks-1;I++)
        VG.Sng->TIns->T[I].Disp = VG.DispositivoSalidaMIDI;
}
/******************************************************************************
*******************************************************************************
FILE/SONG MANAGEMENT CODE
*******************************************************************************
******************************************************************************/

// Open TCK file 
void CMainFrame::OnArchivoAbrir() 
{
	// TODO: Add your command handler code here
	CFileDialog oFileDlg(TRUE, NULL, NULL, NULL, "TCK song files(*.TCK)|*.tck|All the files(*.*)|*.*||"); 

	if (oFileDlg.DoModal()!= IDOK)
		return;

	char szFileName[256];
	strcpy(szFileName, oFileDlg.GetPathName());
	CargarCancionAux(szFileName);
}

// Exports MIDI File
void CMainFrame::OnArchivoExportaramidi() 
{
	// TODO: Add your command handler code here
	CFileDialog oFileDlg(FALSE, NULL, NULL, NULL, "MIDI files(*.MID)|*.mid|All the files(*.*)|*.*||"); 
	
	if (oFileDlg.DoModal()!= IDOK)
		return;

	char szFileName[256];
	char szPathName[256];
	char szExtName[256];
	char szCompleteFileName[1024];

	strcpy(szFileName, oFileDlg.GetFileName());
	strcpy(szPathName, oFileDlg.GetPathName());
	strcpy(szExtName,  oFileDlg.GetFileExt());

	// Tiene la extension?
	strlwr(szExtName);
	
	if (strcmp(szExtName,"mid"))
		// Añadir nosotros la extension
		sprintf(szCompleteFileName,"%s.mid",szPathName);
	else
		strcpy(szCompleteFileName,szPathName);	


	MuestraMensajeFijo("Exportando tema...");
	
	
	if (EscribirFicheroMIDI(szCompleteFileName,false))
		MuestraMensajeFijo("Tema exportado");	
	else
		MuestraMensajeFijo("Error al exportar tema");
}

// Saves song contents into TCK file
void CMainFrame::OnArchivoGuardartema() 
{
	// TODO: Add your command handler code here
     if (VG.NombreFichero != "")
     {
          MuestraMensajeFijo("Salvando tema...");

          if (SalvarCancion(VG.NombreFichero,true))
            MuestraMensajeFijo("Tema salvado");
     }
     else
         // Como si hubieramos pulsado esa opcion del menu
         OnArchivoGuardartemacomo();	
}

// Save as function
void CMainFrame::OnArchivoGuardartemacomo() 
{
	// TODO: Add your command handler code here
	CFileDialog oFileDlg(FALSE, NULL, NULL, NULL, "TCK song files(*.TCK)|*.tck|All the files(*.*)|*.*||"); 
	
	if (oFileDlg.DoModal()!= IDOK)
		return;

	char szFileName[256];
	char szPathName[256];
	char szExtName[256];
	char szCompleteFileName[1024];

	strcpy(szFileName, oFileDlg.GetFileName());
	strcpy(szPathName, oFileDlg.GetPathName());
	strcpy(szExtName,  oFileDlg.GetFileExt());

	// Tiene la extension?
	strlwr(szExtName);
	
	if (strcmp(szExtName,"tck"))
		// Añadir nosotros la extension
		sprintf(szCompleteFileName,"%s.tck",szPathName);
	else
		strcpy(szCompleteFileName,szPathName);	



	MuestraMensajeFijo("Salvando tema...");
	if (SalvarCancion(szCompleteFileName,false))
	{
          VG.NombreFichero = AnsiString(szCompleteFileName);
          VG.oRcntFiles.AnyadirElemento(VG.NombreFichero);
		  SetupRecentFiles();

		// FicherosRecientes.AnyadirElemento(VG.NombreFichero);
		// ActualizarFicherosRecientes();

		MuestraMensajeFijo("Tema salvado");
	}
}

// New song
void CMainFrame::OnArchivoNuevo() 
{
	// TODO: Add your command handler code here
    if (MessageBox(gsz_SURE_TO_INIT_SONG,gsz_APP_NAME,MB_YESNO) == IDYES)
    {
          InicializarCancion();

          gpoBSeq->UpdateContents();
          PattEd::UpdateContents();
          VG.NombreFichero="";          
     }	
}

/******************************************************************************
*******************************************************************************
BLOCK MANAGEMENT CODE
*******************************************************************************
******************************************************************************/

// Erases a song block 
void CMainFrame::OnEdicinBloqueBorrar() 
{
	// TODO: Add your command handler code here
     if (PattEd::EstadoSel!=2)
        ShowMessage("No hay un bloque marcado.");
     else
     {
          BorrarBloque(VG.PatronActual,PattEd::BIniSel,PattEd::BFinSel,PattEd::TIniSel,PattEd::TFinSel);
          PattEd::UpdateContents();
     }	
}

// Copies a song block to memory
void CMainFrame::OnEdicinBloqueCopiar() 
{
	// TODO: Add your command handler code here
	 if (PattEd::EstadoSel != 2)
		ShowMessage("No hay un bloque marcado.");
     else
     {
          CopiarBloque(VG.PatronActual,VG.BloqueAux,PattEd::BIniSel,PattEd::BFinSel,PattEd::TIniSel,PattEd::TFinSel);
     }
}

// Copies a song block to memory and takes it out from current song
void CMainFrame::OnEdicinBloqueCortar() 
{
	// TODO: Add your command handler code here
     if (PattEd::EstadoSel!=2)
        ShowMessage("No hay un bloque marcado.");
     else
     {
          CortarBloque(VG.PatronActual,VG.BloqueAux,PattEd::BIniSel,PattEd::BFinSel,PattEd::TIniSel,PattEd::TFinSel);
          PattEd::UpdateContents();
     }
}

// Pastes a song block from memory to the song starting in the current beat/track position
void CMainFrame::OnEdicinBloquePegar() 
{
	// TODO: Add your command handler code here
     if (PattEd::EstadoSel!=2)
        ShowMessage("No hay un bloque marcado.");
     else
     {
          PegarBloque(VG.PatronActual,VG.BloqueAux,VG.AbsBeatActual,VG.AbsTrackActual);
          PattEd::UpdateContents();
     }
	
}

// Loads a song block from disk
void CMainFrame::OnEdicinBloqueCargar() 
{
	// TODO: Add your command handler code here
	CFileDialog oFileDlg(TRUE, NULL, NULL, NULL, "Song block files(*.blk)|*.blk|All the files(*.*)|*.*||"); 

    if (oFileDlg.DoModal()!= IDOK)
		return;

	char szFileName[256];
	strcpy(szFileName, oFileDlg.GetPathName());

	CrearBloque(VG.BloqueAux2);

    if (CargarBloque(szFileName))
    {
		PegarBloque(VG.PatronActual,VG.BloqueAux2,VG.AbsBeatActual,VG.AbsTrackActual);
        PattEd::UpdateContents();
	}

    DestruirBloque(VG.BloqueAux2);
}

// Saves a song block to disk
void CMainFrame::OnEdicinBloqueSalvar() 
{
	// TODO: Add your command handler code here
	
     if (PattEd::EstadoSel!=2) 
        ShowMessage("No hay un bloque marcado.");
     else
     {
		CFileDialog oFileDlg(FALSE, NULL, NULL, NULL, "Song block files(*.blk)|*.blk|All the files(*.*)|*.*||"); 

		if (oFileDlg.DoModal()!= IDOK)
		return;

		char szFileName[256];
		strcpy(szFileName, oFileDlg.GetPathName());


          CrearBloque(VG.BloqueAux2);
		  CopiarBloque(VG.PatronActual,VG.BloqueAux2,PattEd::BIniSel,PattEd::BFinSel,PattEd::TIniSel,PattEd::TFinSel);
          SalvarBloque(szFileName,false);
          DestruirBloque(VG.BloqueAux2);
	 }
}

/******************************************************************************
*******************************************************************************
PATTERN MANAGEMENT CODE
*******************************************************************************
******************************************************************************/

void CMainFrame::OnEdicinPatrnBorrar() 
{
	// TODO: Add your command handler code here
    if (MessageBox("¿Estás seguro de que quieres borrar este patrón?","TT98",MB_YESNO) == IDYES)
        BorrarPattern(VG.PatronActual);
     
	 PattEd::UpdateContents();
}


void CMainFrame::OnEdicinPatrnCopiar() 
{
	// TODO: Add your command handler code here     
     CopiarPattern(VG.PatronActual,VG.PatternAux);
}

void CMainFrame::OnEdicinPatrnCortar() 
{
	// TODO: Add your command handler code here
     CopiarPattern(VG.PatronActual,VG.PatternAux);
     BorrarPattern(VG.PatronActual);
     
	 PattEd::UpdateContents();
}

void CMainFrame::OnEdicinPatrnPegar() 
{
	// TODO: Add your command handler code here
     CopiarPattern(VG.PatternAux,VG.PatronActual);
    PattEd::UpdateContents();
}

// Loads a song pattern from disk
void CMainFrame::OnEdicinPatrnCargar() 
{
	// TODO: Add your command handler code here
	CFileDialog oFileDlg(TRUE, NULL, NULL, NULL, "Pattern files (*.PAT)|*.pat|All the files(*.*)|*.*||"); 
	if (oFileDlg.DoModal()!= IDOK)
		return;

	char szFileName[256];
	strcpy(szFileName, oFileDlg.GetPathName());

	if (CargarPatron(szFileName))
    {
		gpoBSeq->UpdateContents();
        PattEd::UpdateContents();
	}
}
// Saves a song pattern to disk
void CMainFrame::OnEdicinPatrnSalvar() 
{
	// TODO: Add your command handler code here
	CFileDialog oFileDlg(FALSE, NULL, NULL, NULL, "Pattern files (*.PAT)|*.pat|All the files(*.*)|*.*||"); 
	if (oFileDlg.DoModal()!= IDOK)
		return;

	char szFileName[256];
	strcpy(szFileName, oFileDlg.GetPathName());


	SalvarPatron(szFileName,false);
}

/******************************************************************************
*******************************************************************************
TRACK MANAGEMENT CODE
*******************************************************************************
******************************************************************************/
void CMainFrame::OnEdicinPistaBorrar() 
{
	// TODO: Add your command handler code here
     CrearTrack(VG.PistaActual);
     PattEd::PintaTrack(PattEd::TrackActual);	
}


void CMainFrame::OnEdicinPistaCopiar() 
{
	// TODO: Add your command handler code here
     CopiarTrack(VG.PistaActual,&VG.TrackAux);
}

void CMainFrame::OnEdicinPistaCortar() 
{
	// TODO: Add your command handler code here
	
     CopiarTrack(VG.PistaActual,&VG.TrackAux);
     CrearTrack(VG.PistaActual);

     PattEd::PintaTrack(PattEd::TrackActual);
}

void CMainFrame::OnEdicinPistaPegar() 
{
	// TODO: Add your command handler code here
     CopiarTrack(&VG.TrackAux,VG.PistaActual);
     PattEd::PintaTrack(PattEd::TrackActual);
}

void CMainFrame::OnEdicinPistaProcesarefecto() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnEdicinPistaProcessarvolumen() 
{
	// TODO: Add your command handler code here
	
}
// Loads a song track from disk
void CMainFrame::OnEdicinPistaCargar() 
{
	// TODO: Add your command handler code here
	CFileDialog oFileDlg(TRUE, NULL, NULL, NULL, "Track files (*.TRK|*.trk|All the files(*.*)|*.*||"); 

	if (oFileDlg.DoModal()!= IDOK)
		return;

	char szFileName[256];
	strcpy(szFileName, oFileDlg.GetPathName());

	CrearTrack(VG.PistaActual);	
	if (CargarPista(szFileName))
		PattEd::PintaTrack(PattEd::PrimerTrack+PattEd::TrackActual);
}

// Saves the current track to disk
void CMainFrame::OnEdicinPistaSalvar() 
{
	// TODO: Add your command handler code here
	CFileDialog oFileDlg(FALSE, NULL, NULL, NULL, "Track files (*.TRK|*.trk|All the files(*.*)|*.*||"); 	
	
	if (oFileDlg.DoModal()!= IDOK)
		return;

	char szFileName[256];
	strcpy(szFileName, oFileDlg.GetPathName());

	SalvarPista(szFileName,false);
}

/******************************************************************************
*******************************************************************************
WINDOW MANAGEMENT CODE
*******************************************************************************
******************************************************************************/
void CMainFrame::OnVerBarradeedicin() 
{
	// TODO: Add your command handler code here	
	gbBEdWndVis = !gbBEdWndVis;

	if (gbBEdWndVis)
		::ShowWindow(ghBEdWnd, SW_SHOW);
	else
		::ShowWindow(ghBEdWnd, SW_HIDE);

	GetMenu()->CheckMenuItem(ID_VER_BARRADEEDICIN,gbBEdWndVis?MF_CHECKED:MF_UNCHECKED);
}

void CMainFrame::OnVerBarradereproduccion() 
{
	// TODO: Add your command handler code here
	gbBRepWndVis = !gbBRepWndVis;

	if (gbBRepWndVis)
		::ShowWindow(ghBRepWnd, SW_SHOW);
	else
		::ShowWindow(ghBRepWnd, SW_HIDE);

	GetMenu()->CheckMenuItem(ID_VER_BARRADEREPRODUCCION,gbBRepWndVis?MF_CHECKED:MF_UNCHECKED);
}

void CMainFrame::OnVerBarradesecuencia() 
{
	// TODO: Add your command handler code here
	gbBSeqWndVis = !gbBSeqWndVis;

	if (gbBSeqWndVis)
		::ShowWindow(ghBSeqWnd, SW_SHOW);
	else
		::ShowWindow(ghBSeqWnd, SW_HIDE);

	GetMenu()->CheckMenuItem(ID_VER_BARRADESECUENCIA,gbBSeqWndVis?MF_CHECKED:MF_UNCHECKED);
}

/******************************************************************************
*******************************************************************************
WINDOW MANAGEMENT CODE
*******************************************************************************
******************************************************************************/
void CMainFrame::OnEdicinTablasdeefectos() 
{
	// TODO: Add your command handler code here
	/*
	    // ShowMessage("WARNING 10: Not fully implemented");
    VEffs->ShowModal();

*/
}

void CMainFrame::OnEdicinTablasdeinstrumentos() 
{
	// TODO: Add your command handler code here
/*
    // ShowMessage("WARNING 07: Not fully implemented");
    EditorIns->ShowModal();
*/	
}
/******************************************************************************
*******************************************************************************
PREFERENCES MANAGEMENT CODE
*******************************************************************************
******************************************************************************/

void CMainFrame::OnEdicinOpcionesEntradamidi() 
{
	// TODO: Add your command handler code here
	/*
	     // ShowMessage("WARNING 08: Not fully implemented");
     unsigned char aux = ObtenerMIDIIn(VG.DispositivoEntradaMIDI);

     if (aux != VG.DispositivoEntradaMIDI)
     {
          EntradaMIDI->Stop();
          EntradaMIDI->Close();
          EntradaMIDI->EstablecerDispositivo(aux);
          VG.DispositivoEntradaMIDI = EntradaMIDI->Dispositivo();

          EntradaMIDI->Open(RSIMidiIn);

          if (EntradaMIDI->Abierto)
             EntradaMIDI->Start();
          else
          {
             MostrarError(  "FALLO: El dispositivo MidiIN" +
                            ListMIDIIn->Items->Strings[VG.DispositivoEntradaMIDI] + 
                            "\n no ha podido ser abierto. Comprueba que no esté siendo utilizado por otra aplicación.",
                            false);
          }
     }
	*/
}
/******************************************************************************
*******************************************************************************
RECENTFILES MANAGEMENT CODE
*******************************************************************************
******************************************************************************/
void CMainFrame::OnRecentfile00() 
{
	// TODO: Add your command handler code here
	CargarCancionAux( VG.oRcntFiles.LeerElemento(0) );		
}

void CMainFrame::OnRecentfile01() 
{
	// TODO: Add your command handler code here
	CargarCancionAux( VG.oRcntFiles.LeerElemento(1) );		
}

void CMainFrame::OnRecentfile02() 
{
	// TODO: Add your command handler code here
	CargarCancionAux( VG.oRcntFiles.LeerElemento(2) );	
}

void CMainFrame::OnRecentfile03() 
{
	// TODO: Add your command handler code here
	CargarCancionAux( VG.oRcntFiles.LeerElemento(3) );	
}
//------------------------------------------------------------------------------
void CMainFrame::SetupRecentFiles()
{

	if (VG.oRcntFiles.C.NumStrEnCola == 0)
	{
		GetMenu()->GetSubMenu(0)->EnableMenuItem(7,MF_BYPOSITION | MF_GRAYED | MF_DISABLED);
	}
	else
	{
		int i;

		GetMenu()->GetSubMenu(0)->EnableMenuItem(7,MF_BYPOSITION | MF_ENABLED);

		// Get recent files submenu
		CMenu *poMenu = GetMenu()->GetSubMenu(0)->GetSubMenu(7);

		for (i=0;i<4;i++)
		{	
		//	poMenu->EnableMenuItem(i,MF_BYPOSITION | MF_GRAYED | MF_DISABLED);
			poMenu->DeleteMenu(ID_RECENTFILES + i,MF_BYCOMMAND);
		}

		for (i=0;i<VG.oRcntFiles.C.NumStrEnCola;i++)
			poMenu->InsertMenu(i,MF_BYPOSITION,ID_RECENTFILES + i,VG.oRcntFiles.LeerElemento(i).c_str() );
	}
}
		/*
		char szStr[1024];

		MENUITEMINFO oMInfo;
		memset(&oMInfo,0,sizeof(MENUITEMINFO));
		oMInfo.cbSize		= sizeof(MENUITEMINFO);
		oMInfo.fMask		= MIIM_DATA | MIIM_TYPE | MIIM_SUBMENU;
		oMInfo.fType		= MFT_STRING;
		oMInfo.dwTypeData	= (LPTSTR)szStr;
		oMInfo.cch			= 1024;
		k = poMenu->GetMenuItemCount();
		oMInfo.fMask		= MIIM_DATA | MIIM_TYPE | MIIM_SUBMENU;
		// gpMainFrame->GetMenu()->m_hMenu;
		poMenu = GetMenu()->GetSubMenu(0);
		poMenu = poMenu->GetSubMenu(8);
			// oMInfo.cch = 1024;
			// ::GetMenuItemInfo(hMenu,ID_RECENTFILES + i,MF_BYCOMMAND,&oMInfo);
			// ::DeleteMenu(hMenu,ID_RECENTFILES + i,MF_BYCOMMAND);

				// Activate				
				GetMenu()->EnableMenuItem(ID_RECENTFILES + i,MF_BYCOMMAND | MF_ENABLED);
				GetMenu()->EnableMenuItem(ID_RECENTFILES + i,MF_BYCOMMAND | MF_ENABLED);			
				strcpy(oMInfo.dwTypeData,VG.oRcntFiles.LeerElemento(i).c_str());
				oMInfo.cch = strlen(oMInfo.dwTypeData)+1;

				GetMenu()->InsertMenu(20,MF_BYPOSITION,ID_RECENTFILES + i,VG.oRcntFiles.LeerElemento(i).c_str() );
				::InsertMenu(
				::SetMenuItemInfo(hMenu,ID_RECENTFILES + i,MF_BYCOMMAND,&oMInfo);
		*/


/******************************************************************************
*******************************************************************************
DISABLE NOT IMPLEMENTED OPTIONS
*******************************************************************************
******************************************************************************/

void CMainFrame::OnUpdateArchivoExportaramidi(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	// pCmdUI->Enable(false);
	
}

void CMainFrame::OnUpdateEdicinTablasdeefectos(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateEdicinTablasdeinstrumentos(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(false);
}
