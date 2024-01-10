// MDIClient.cpp : implementation file
//

#include "stdafx.h"
#include <afxwin.h>

#include "GUIS/PattEd/PattEd.h"
#include "GUIS/BRep/BRep.h"
#include "GUIS/BSeq/BSeq.h"
#include "MDIClient.h"
#include "Misc/WinVars.h"
#include "Misc/Msgs.h"
#include "MainFrm.h"
#include "MiscIns.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//---------------------------------------------------------------------------
#include "consts2.h"                                // Constantes de caracter global
#include "variables.h"                              // Variables globales
#include "inter.h"                                  // Estructura de datos cancion
#include "SongStructs/Ficheros.h"					//
#include "SongStructs/CrtError.h"
//---------------------------------------------------------------------------
#include "Misc/Variables.h"
#include "Player/tt98Player.h"
#include "Player/MIDIInClient/MIDIInClient.h"
#include "Player/OMidiIn.h"
#include "Player/OMidiOut.h"
#include "SongStructs/Inter.h"
//---------------------------------------------------------------------------

void BTAC_ResetMIDISystem();
void BTAC_PlayCancion();
void BTAC_StopCancion();

//---------------------------------------------------------------------------
TObjetoMidiIn*	EntradaMIDI;

//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// CMDIClient

CMDIClient::CMDIClient()
{
   if ( IsWindow( m_hWnd ) )

      Invalidate();
}

CMDIClient::~CMDIClient()
{
}

void CMDIClient::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd::PreSubclassWindow();
}


// ------------------------------------------------------------------------
LRESULT CMDIClient::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	switch (message)
	{
		case WM_KEYUP:
		{
			if ((int)wParam == VK_CONTROL) guiKeyFlags &= ~KEY_CONTROL;
			if ((int)wParam == VK_SHIFT) guiKeyFlags &= ~KEY_SHIFT;
			if ((int)wParam == VK_MENU) guiKeyFlags &= ~KEY_ALT;

			if ( PattEd::KeyUpHandler(guiKeyFlags,(int)wParam,(lParam >> 16) & 0x00ff) ) return(0);
			if ( BRep::  KeyUpHandler(guiKeyFlags,(int)wParam,(lParam >> 16) & 0x00ff) ) return(0);

			return CWnd::WindowProc(message, wParam, lParam);
		}
		break;

		case WM_KEYDOWN:
		{
			if ((int)wParam == VK_CONTROL) guiKeyFlags |= KEY_CONTROL;
			if ((int)wParam ==   VK_SHIFT) guiKeyFlags |= KEY_SHIFT;
			if ((int)wParam ==    VK_MENU) guiKeyFlags |= KEY_ALT;

			if ( PattEd::KeyDownHandler(guiKeyFlags,(int)wParam,(lParam >> 16) & 0x00ff) ) return(0);
			if ( BRep::  KeyDownHandler(guiKeyFlags,(int)wParam,(lParam >> 16) & 0x00ff) ) return(0);

			return CWnd::WindowProc(message, wParam, lParam);
		}
		break;

		case WM_SIZE:
			PattEd::WndProc(m_hWnd,WM_USER,wParam,lParam);
		break;
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}
//------------------------------------------------------------------------------
void ActualizarVariables(unsigned short PB,unsigned short BA,unsigned short PT,unsigned short TA)
{
     VG.AbsTrackActual		= PT+TA;
     VG.AbsBeatActual		= PB+BA;

     VG.PistaActual			= &(VG.PatronActual->Tracks[VG.AbsTrackActual]);
};

void ProcesarMute(unsigned short _iTrack,bool _bMute)
{
     // true : Desactivar, false: Activar
     EstablecerEstadoTrack(_iTrack,_bMute);	
}
//--------------------------------------------------------------------------------
int CMDIClient::iInit(HWND _hWnd,HINSTANCE _hInst)
{
	int i,j;

	hWnd	= _hWnd;
	hInst	= _hInst;	

    // WARNING: Debug code
    /*
	BMPPlano = new TPicture;
    try
    {
      BMPPlano->LoadFromFile( ObtenerDirectorioConfig() + "\\plantt99.bmp");
    }
    catch (Exception &exception)
    {
         delete BMPPlano;
         BMPPlano = ElementosTT98->Picture;
    }
	
//    BMPPlano = ElementosTT98->Picture;
	*/

	// Cargar las imagenes utilizadas en el programa	
	oBck.Load("data\\fondo.bmp",hWnd,hInst);

	
	// set a timer to go off 5 times a second
    // at every timer message the input device will be read	
	UINT uiID = ::SetTimer(hWnd, 0,1000/5,NULL);

    // BaseBRep->Color = &oBMPRes->Canvas->Pixels[75][0];
    // GCTitulo.Inicializar(607, 4,190,80,&oBMPRes,0, 0,hWnd);
    // GCAutor.Inicializar (607,86,190,20,&oBMPRes,0,81,hWnd);

    // Pantalla de estado
    /*
	GCStatusLeft.Inicializar  (EstadoRep->Left-25    ,EstadoRep->Top     , 25,14,&oBMPRes,  0,253+11,hWnd);
    GCStatusRight.Inicializar (EstadoRep->Left-25+325,EstadoRep->Top     , 25,14,&oBMPRes,325,253+11,hWnd);
    GCStatusTop.Inicializar   (EstadoRep->Left-25    ,EstadoRep->Top-11  ,350,11,&oBMPRes,  0,253+ 0,hWnd);
    GCStatusBottom.Inicializar(EstadoRep->Left-25    ,EstadoRep->Top+14  ,350,11,&oBMPRes,  0,253+25,hWnd);
	*/

    // Pantalla de BPM's
    /*
	GCBPMSLeft.Inicializar  (BPMDis->Left-24    ,BPMDis->Top      , 24,14,&oBMPRes,199+  0,20,hWnd);
    GCBPMSRight.Inicializar (BPMDis->Left-24+124,BPMDis->Top      , 26,14,&oBMPRes,199+124,20,hWnd);
    GCBPMSTop.Inicializar   (BPMDis->Left-24    ,BPMDis->Top-20   ,150,20,&oBMPRes,199+  0, 0,hWnd);
    GCBPMSBottom.Inicializar(BPMDis->Left-24    ,BPMDis->Top+14   ,150,20,&oBMPRes,199+  0,34,hWnd);
	*/

    //Inicializacion de los botones utilizados en el programa
    // InicializarBotones();
    
	// TCB->Enabled = false;
    // TCB->Interval= 250;

    /*
	AnyadirBoton(540,22,50,70,&oBMPRes,350+ 0,96+  0,&oBMPRes,350+50,96+  0,TBUnDisparo  ,hWnd);  // Reset
    AnyadirBoton(135,75,75,30,&oBMPRes,199+ 0,60+  0,&oBMPRes,199+75,60+  0,TBInterruptor,hWnd);  // Play
    AnyadirBoton( 60,75,75,30,&oBMPRes,199+ 0,60+ 30,&oBMPRes,199+75,60+ 30,TBUnDisparo  ,hWnd);  // Stop
    AnyadirBoton(210,75,75,30,&oBMPRes,199+ 0,60+ 60,&oBMPRes,199+75,60+ 60,TBInterruptor,hWnd);  // Pause
    AnyadirBoton(100, 8,75,30,&oBMPRes,199+ 0,60+ 90,&oBMPRes,199+75,60+ 90,TBInterruptor,hWnd);  // PlayTrack
    AnyadirBoton(175, 8,75,30,&oBMPRes,199+ 0,60+120,&oBMPRes,199+75,60+120,TBInterruptor,hWnd);  // PlayPattern

    Botones[NBReset    ]->Activo = true;               //Activar el boton Reset
    Botones[NBPlay     ]->Activo = true;	           //  ""         ""  play
    Botones[NBStop     ]->Activo = false;              //Desactivar Boton Stop
    Botones[NBPause    ]->Activo = false;              //  ""         ""    pausa
    Botones[NBPlayTrack]->Activo = true;               //Activar play track
    Botones[NBPlayPatt ]->Activo = true;               //  ""    play pattern

    AnyadirBoton(505,14,30,19,&oBMPRes,350+0,19,&oBMPRes,350+30,19,TBAutoRepeticion,hWnd);
    AnyadirBoton(505,32,30,19,&oBMPRes,350+0,38,&oBMPRes,350+30,38,TBAutoRepeticion,hWnd);

    AnyadirBoton(375,60,60,40,&oBMPRes,199+0,211+0,&oBMPRes,199+60,211+0,TBInterruptor,hWnd);  //Loop
    AnyadirBoton(435,60,60,40,&oBMPRes,320+0,211+0,&oBMPRes,320+60,211+0,TBInterruptor,hWnd);  //Activa ED

    // Crear un bitmap para el símbolo de enmudecer, y pasarle la imagen
    // BitBlt(BMPMute->Picture->Bitmap->Canvas->Handle,0,0,120,20,&oBMPRes->Canvas->Handle,350,167,SRCCOPY);
	*/

    //--------------------------------------------------------------------------------

    // Estas variables se establecen al leer la configuracion
/*
    VG.PDP.Font     = "Lucida Console";
    VG.PDP.FontSize = 12;

    VG.PDP.MrkBtBkColor	= 0x0039594E;	// Marked beat line
	VG.PDP.MrkBtFnColor	= 0x00000000;
    VG.PDP.NrmBkColor	= 0x005A8C7B;	// Normal
	VG.PDP.NrmFnColor	= 0x00000000;
    VG.PDP.Nrm8BMBkColor= 0x005D9180;	// Normal 8 Beat Multiple
	VG.PDP.Nrm8BMFnColor= 0x00000000;
    VG.PDP.NrmBkColor	= 0x005A8C7B;	// Normal
	VG.PDP.NrmFnColor	= 0x00000000;
	VG.PDP.NumBkColor	= 0x005A8C7B;	// Numbers
	VG.PDP.NumFnColor	= 0x0019231D;
	VG.PDP.SelBkColor	= 0x00131E1B;	// Marked block
	VG.PDP.SelFnColor	= 0x005A8C7B;   
	VG.PDP.CurFnColor	= 0x00000000;	// Current beat
	VG.PDP.CurBkColor	= 0x0088DB9D;
	VG.PDP.PattEdBk		= 0x005A8C7B;
*/
    // VG.PDP.BMPMute    = BMPMute->Picture->Bitmap;
    
	VG.PDP.ProcMute   = ProcesarMute;
    VG.PDP.ActVar     = ActualizarVariables;
	  
	RECT r; ::GetClientRect(hWnd,&r);
	iWidth = r.right  - r.left;
	iHeight= r.bottom - r.top;

    /*
	VG.PDP.tx         = iWidth  - 60;
    VG.PDP.ty         = iHeight - 60;
	*/

	// Actualizar variables de pattern
    VG.PatternAux = NULL;
    CrearPattern(VG.PatternAux);

    VG.BloqueAux = NULL;
    CrearBloque(VG.BloqueAux);
    VG.BloqueAux2 = NULL;
    CrearBloque(VG.BloqueAux2);

/*
    VG.Acorde       =1; // Salto a cada pulsacion
    VG.Arpegio      =1; // Edición en una sola pista inicialmente
    VG.SaltoLinea   =8; // Salto de 8 lineas
    VG.Transponer   =0; // Notas a transponer

    // Contador de notas hasta el siguiente salto
    // VG.
    VG.ContAcordes  =0;
    VG.ContArpegios =0;
*/
    ResetSistemaMIDI();

    // Estado Inicial del Timer
//    EstadoTimer = ETInactivo;

    // Estado del loop
    VG.Loop = false;

    // Estado del patron
    VG.PatronActivo=false;

    //Inicializar las tablas de efectos
    for (i=0;i<=MaxDispositivosMIDIOut-1;i++)
		VG.ListaEfectos[i].Inicializar();

    //Leer datos del archivo de configuracion
    LeerConfig();

    // Creacion de la lista de Dispositivos MIDI
    // Obtencion de datos necesarios
    // ObtenerListaDispositivosMIDIOut(ListMIDIOut->Items);
    // ObtenerListaDispositivosMIDIIn (ListMIDIIn->Items );
    VG.NumPorts = NumeroDispositivosMIDIOut();
    
    
	// Configuracion de los dispositivos de salida MIDI	 
	for (i=0;i<VG.NumPorts;i++)
	{
		//Los bancos
		for (j=0;j<MaxBancos;j++)
		{
			if (VG.FicherosListasInstrumentos[i][j] != "")
				CargarLista(i,j,VG.FicherosListasInstrumentos[i][j]);
		}
	}


    #ifdef USE_MIDI

	// Leer del fichero de config, y decidir que dispositivo MIDI In crear, si el local,ç
	// o el remoto via MIDIInClient
	extern void *RSIMidiIn;

	// Obtener nombre via INI file
	if (VG.iRemoteMIDIInput)
	{
		EntradaMIDI = new MIDIInClient(VG.asMIDIInputHost.c_str());
		EntradaMIDI->Open(PattEd::RSIMidiIn);

		if (EntradaMIDI->Abierto)
			EntradaMIDI->Start();
		else
		  MostrarError("FALLO: El dispositivo remoto MidiIN\n"
					   "no ha podido ser abierto.",false);
	}
	else
	{
		//Entrada Midi solo cuando se ha leido el archivo de config

		// <WARNING: DEBUG CODE>
		EntradaMIDI = new TObjetoMidiIn;
		EntradaMIDI->EstablecerDispositivo( VG.DispositivoEntradaMIDI );
		EntradaMIDI->Open(PattEd::RSIMidiIn);


		for (int l=0;l<NumeroDispositivosMIDIIn();l++)
		{
			char *szStr = ObtenerNombreDispositivoMidiIn(l);
		}


		if (EntradaMIDI->Abierto)
			EntradaMIDI->Start();
		else
		{ 
			char szBuffer[1024];
			sprintf(szBuffer,"FALLO: El dispositivo MidiIN %d, no ha podido ser abierto.\n"
							 "Comprueba que no esté siendo utilizado por otra aplicación.",VG.DispositivoEntradaMIDI);

			MostrarError(szBuffer,false);
		}
	}

    /*
	//Entrada Midi solo cuando se ha leido el archivo de config

	// <WARNING: DEBUG CODE>
	EntradaMIDI->EstablecerDispositivo(VG.DispositivoEntradaMIDI);
    try
    {
        EntradaMIDI->Open(RSIMidiIn);
    }
    catch (Exception &exception)
    {
        MostrarError("FALLO: MIDI IN",false);
    }

    if (EntradaMIDI->Abierto)
      EntradaMIDI->Start();
    else
      MostrarError("FALLO: El dispositivo MidiIN" + ListMIDIIn->Items->Strings[VG.DispositivoEntradaMIDI]+ "/n no ha podido ser abierto. Comprueba que no esté si}o utilizado por otra aplicación.",false);
	*/

    #endif

	

    
    //Establecer funciones para las units que lo precisen
    // Unit Play
    // EstablecerFuncionConversionTCKEffect2MIDIEvent(CovertirEffectoTCKaEventoMidi);

    // Unit Tck2Mid
    // ShowMessage("WARNING 00: Not fully implemented");
    // EstablecerFuncionDeConversionTCK2Mid(CovertirEffectoTCKaEventoMidi);

    InitSongStructs();


	// PatternDisplay.Inicializar(hWnd,NULL,NULL,&VG.PDP);
	return(1);
}

AnsiString CMDIClient::GetConfigDir()
{
	return( AnsiString("Ninguno") );
}

void CMDIClient::LeerConfig()
{
/*
    TIniFile*   Ini;
    short        i,j,k;
    AnsiString   Seccion;
    AnsiString   Aux;
    AnsiString   AuxFontName;
    int          AuxFontSize;

     // Creacion del objeto archivo de configuración
     Ini = new TIniFile(ObtenerDirectorioConfig() + "\\TT99.INI");

     VG.PDP.CRojo =Ini->ReadInteger("Visualizacion","Componente_Roja" ,0x60);
     VG.PDP.CAzul =Ini->ReadInteger("Visualizacion","Componente_Azul" ,0xC0);
     VG.PDP.CVerde=Ini->ReadInteger("Visualizacion","Componente_Verde",0x00);

     VG.PDP.ColorSel = Ini->ReadInteger ("Visualizacion","ColorBloqueSeleccionado",clRed);

     VG.PDP.FuenteUsada= Ini->ReadString ("Visualizacion","Fuente"       ,"Courier");
     VG.PDP.TamFuente  = Ini->ReadInteger("Visualizacion","FuenteTamanyo",8);

     VG.PDP.ColorFuenteNums    = Ini->ReadInteger("Visualizacion","FuenteColorNums"   ,clYellow);
     VG.PDP.ColorFuenteTitulosT= Ini->ReadInteger("Visualizacion","FuenteColorTitulos",clYellow);
     VG.PDP.ColorFuenteMalla   = Ini->ReadInteger("Visualizacion","FuenteColorMalla"  ,clAqua);

     // Pequeña comprobacion acerca de algunas cosas sobre visualizacion:
     if (VG.PDP.TamFuente > 15)
     {
          MostrarError("ERROR: El tamaño de la fuente tiene proporciones demasiado grandes.\nUsando tamaño máximo=15.",false);
          VG.PDP.TamFuente=15;
     }
     else
     if (VG.PDP.TamFuente < 5)
     {
          MostrarError("ERROR: El tamaño de la fuente tiene proporciones demasiado pequeñas.\nUsando tamaño mínimo=5.",false);
          VG.PDP.TamFuente=5;
     }

     AuxFontName = Canvas->Font->Name;
     AuxFontSize = Canvas->Font->Size;

     Canvas->Font->Name = VG.PDP.FuenteUsada;
     Canvas->Font->Size = VG.PDP.TamFuente;

     if (Canvas->TextWidth("...") != Canvas->TextWidth("MMM"))
     {
         MostrarError("ERROR: La fuente usada no es una fuente proporcional.",false);
         VG.PDP.FuenteUsada = "Courier";
     }

     // Restaurar el anterior estado
     Canvas->Font->Name = AuxFontName;
     Canvas->Font->Size = AuxFontSize;

     //Leer lista de ficheros recientes: Cargar al reves para colocar en orden correcto
     oRcntFiles.Inicializar(4);

     for (i=3;i>=0;i--)
     {
          Aux =Ini->ReadString("oRcntFiles" ,"fichero_reciente_"+IntToStr(i+1),"");
          if (Aux!="")  oRcntFiles.AnyadirElemento(Aux);
     }

     // Dispositivo de entrada MIDI por defecto
     VG.DispositivoEntradaMIDI = Ini->ReadInteger("DispositivoMidiIn" ,"Id",0);
     VG.DispositivoSalidaMIDI  = Ini->ReadInteger("DispositivoMidiOut","Id",0);

     // Configuracion de los dispositivos de salida MIDI
     i=0;
     while ((i<MaxDispositivosMIDIOut) && (i<VG.NumPorts))
     {
          Seccion="DispositivoMidiOut_"+IntToStr(i);
          //Leemos la seccion al completo

          //La tabla de efectos
          Aux=Ini->ReadString(Seccion,"TablaEfectos","");
          VG.FicherosListasEfectos[i]=Aux;

          //Los bancos
          for (j=0;j<= MaxBancos-1;j++)
          {
               Aux=Ini->ReadString(Seccion,"Banco_"+IntToStr(j),"");
               if (Aux!="") CargarLista(i,j,Aux);

               VG.FicherosListasInstrumentos[i][j]=Aux;
          }

          i++;
     }

     //Ultimos temas de ayuda seleccionados

     // ShowMessage("WARNING 01: UncompleteCode");
     // <DMC: 2003>
     // BookMarkURLS.Inicializar(AYUDA_MAX_ENTRADAS_EN_BOOKMARK);
     // for i=0 <= AYUDA_MAX_ENTRADAS_EN_BOOKMARK-1 do
//          BookMarkURLS.AnyadirElemento(Ini->ReadString("PaginasAyuda","pagina_"+IntToStr(i),""));
  
     // Procesar la información que acabamos de leer en la configuración
     for (i=0;i<=MaxDispositivosMIDIOut-1;i++)
     {
          VG.ListaEfectos[i].Inicializar();
          VG.CFicherosListaEfectos[i].Clear();

          if (
              (VG.FicherosListasEfectos[i]!="") &&
              CargarListaEfectos(VG.FicherosListasEfectos[i],VG.CFicherosListaEfectos[i])
            )
          {
                // ShowMessage("WARNING 02: Uncomplete code");
                if (! CompilarListaEfectos(&(VG.CFicherosListaEfectos[i]),NULL,VG.ListaEfectos[i]))
                       ShowMessage("Error al compilar el archivo con tabla de efectos "+ VG.FicherosListasEfectos[i] +".");
          }
          else
          {
               // Inicializamos con algo de contenido
               VG.FicherosListasEfectos[i]="";

               VG.CFicherosListaEfectos[i].Add("# Editor de listas de efectos");
               VG.CFicherosListaEfectos[i].Add("# Techno Tracker '99");
               VG.CFicherosListaEfectos[i].Add("# por David Márquez de la Cruz.");
          }
     }
*/
}
// ------------------------------------------------------------------------
void CMDIClient::EscribirConfig()
{
/*
   AnsiString   dir;
   TIniFile *   Ini;
   short        i,j;
   AnsiString   Seccion;
   short        Aux;

     //Creacion del objeto archivo de configuración
     Ini = new TIniFile (ObtenerDirectorioConfig()+"\TT99.INI");

     // Escribir configuracion de la visualizacion
     Ini->WriteInteger("Visualizacion","Componente_Roja",VG.PDP.CRojo);
     Ini->WriteInteger("Visualizacion","Componente_Azul" ,VG.PDP.CAzul);
     Ini->WriteInteger("Visualizacion","Componente_Verde",VG.PDP.CVerde);

     Ini->WriteInteger ("Visualizacion","ColorBloqueSeleccionado",VG.PDP.ColorSel);

     Ini->WriteString ("Visualizacion","Fuente"     ,VG.PDP.FuenteUsada);
     Ini->WriteInteger("Visualizacion","FuenteTamanyo",VG.PDP.TamFuente);

     Ini->WriteInteger("Visualizacion","FuenteColorNums"   ,VG.PDP.ColorFuenteNums);
     Ini->WriteInteger("Visualizacion","FuenteColorTitulos",VG.PDP.ColorFuenteTitulosT);
     Ini->WriteInteger("Visualizacion","FuenteColorMalla"  ,VG.PDP.ColorFuenteMalla);

     //Escribir lista de ficheros recientes
     for (i=0;i<=oRcntFiles.C.NumStrEnCola-1;i++)
         Ini->WriteString("oRcntFiles" ,"fichero_reciente_"+IntToStr(i+1),oRcntFiles.LeerElemento(i));

     Ini->WriteInteger("DispositivoMidiIn" ,"Id",VG.DispositivoEntradaMIDI);
     Ini->WriteInteger("DispositivoMidiOut","Id",VG.DispositivoSalidaMIDI);

     i=0;
     while ((i<MaxDispositivosMIDIOut) && (i<VG.NumPorts))
     {
          Seccion="DispositivoMidiOut_"+IntToStr(i);
          Ini->WriteString(Seccion,"TablaEfectos",VG.FicherosListasEfectos[i]);

          for (j=0;j<=MaxBancos-1;j++)
              if (VG.FicherosListasInstrumentos[i][j]!="")
                 Ini->WriteString(Seccion,"Banco_"+IntToStr(j),VG.FicherosListasInstrumentos[i][j]);

          i++;
     }

     // Paginas en bookmark
     // ShowMessage("WARNING 03: UncompleteCode");
     // <DMC: 2003>
     //for i=0 <= AYUDA_MAX_ENTRADAS_EN_BOOKMARK-1 do
     //     Ini->WriteString("PaginasAyuda","pagina_"+IntToStr(i),BookMarkURLS.LeerElemento(i));     
*/
}
//-----------------------------------------------------------------------------
void CMDIClient::InitSongStructs()
{
     //Inicialización de todas las variables
     //CrearTrack(&TrackAux);        // Track para copiar/pegar ...
     CrearCancion(VG.Sng);           // Variable cancion global

     VG.PatronActual = VG.Sng->Pats[0];
     VG.IndSecActual = 0;
     VG.PatternActual= 0;

     for (int I=0;I<=MaxTracks-1;I++)
        VG.Sng->TIns->T[I].Disp = VG.DispositivoSalidaMIDI;
}
//------------------------------------------------------------------------------
// Menu de opciones
//--------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CMDIClient, CWnd)
	//{{AFX_MSG_MAP(CMDIClient)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIClient message handlers

void CMDIClient::OnPaint() 
{
	if (! GetUpdateRect(&oUpdRect,false)) return;

	CPaintDC dc(this); // device context for painting
	hDC = dc.m_hDC;

	Draw();
}

BOOL CMDIClient::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
   return TRUE;    // Return TRUE so background is not erased   
}

void CMDIClient::Draw()
{	
	FillBackground();	
	oBtnMgr.Draw();
}

void CMDIClient::FillBackground()
{	
	// TODO: Add your message handler code here
	int iBMPWidth = oBck.iGetTX();
	int iBMPHeight= oBck.iGetTY();

	int iIXTile = oUpdRect.left / iBMPWidth;
	int iIYTile = oUpdRect.top  / iBMPHeight;
	int iFXTile = (oUpdRect.right/ iBMPWidth) + 1;
	int iFYTile = (oUpdRect.bottom/ iBMPHeight) + 1;

	for (int cJ=iIYTile;cJ<iFYTile;cJ++)
		for (int cI=iIXTile;cI<iFXTile;cI++)
			BitBlt(hDC,cI*iBMPWidth,cJ*iBMPHeight,iBMPWidth,iBMPHeight,oBck.GetDC(),0,0,SRCCOPY);

	
	// Do not call CWnd::OnPaint() for painting messages
}

/////////////////////////////////////////////////////////////////////////////
// MENU Option handlers
/////////////////////////////////////////////////////////////////////////////


