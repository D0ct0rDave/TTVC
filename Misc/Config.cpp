#include <afxwin.h>
#include "config.h"
#include "variables.h"
#include "winvars.h"
// #include "GUIS/Main/mainfrm.h"

// ----------------------------------------------------------
char*	TT_INIFILE;

#define TT_INI_SECT_VISUAL	"Visuals"
#define TT_INI_SECT_MIDIIN	"Midi In"
#define TT_INI_SECT_EDITBAR	"Edition Bar"
#define TT_INI_SECT_RFILES	"Recent Files"
#define TT_INI_SECT_MISC	"Misc"
#define TT_INI_SECT_INSTLST	"InstList"

// ----------------------------------------------------------

static char sszValBuffer[256];
static char sszDefValBuffer[256];

char *szGetString(char *_szVar,char* _szSection,char *_szDefVal)
{
	GetPrivateProfileString(_szSection,_szVar,_szDefVal,sszValBuffer,256,TT_INIFILE);
	return(sszValBuffer);
}

void SetString(char *_szVar,char* _szSection,char *_szValue)
{
	WritePrivateProfileString(_szSection,_szVar,_szValue,TT_INIFILE);
}

int iGetInteger(char *_szVar,char* _szSection,int _iDefValue)
{
	return ( GetPrivateProfileInt(_szSection,_szVar,_iDefValue,TT_INIFILE) );
}

void SetInteger(char *_szVar,char* _szSection,int _iValue)
{
	sprintf(sszValBuffer,"%d",_iValue);
	WritePrivateProfileString(_szSection,_szVar,sszValBuffer,TT_INIFILE);
}

unsigned int uiGetHex(char *_szVar,char* _szSection,unsigned int _uiDefValue)
{	
	unsigned int uiVal;

	// Get default value hexadecimal string
	sprintf(sszDefValBuffer,"%x",_uiDefValue);
	
	// retrieve string value
	GetPrivateProfileString(_szSection,_szVar,sszDefValBuffer,sszValBuffer,256,TT_INIFILE);

	// Convert string to int
	sscanf(sszValBuffer,"%x",&uiVal);

	return ( uiVal );
}

void SetHex(char *_szVar,char* _szSection,int _iValue)
{
	sprintf(sszValBuffer,"0x%.8x",_iValue);
	WritePrivateProfileString(_szSection,_szVar,sszValBuffer,TT_INIFILE);
}
// ----------------------------------------------------------
void ReadConfig(char *_szIniFile)
{
	TT_INIFILE = _szIniFile;
	int i,j,k;

	VG.PDP.Font			= szGetString("Font"			,TT_INI_SECT_VISUAL,"Lucida Console");
	VG.PDP.FontSize     = iGetInteger("FontSize"		,TT_INI_SECT_VISUAL,12);
	if (VG.PDP.FontSize<4) VG.PDP.FontSize = 4; else if (VG.PDP.FontSize>48) VG.PDP.FontSize = 48;

	VG.PDP.MrkBtBkColor = uiGetHex   ("MrkBtBkColor"	,TT_INI_SECT_VISUAL,0x0039594e);
	VG.PDP.MrkBtFnColor	= uiGetHex   ("MrkBtFnColor"	,TT_INI_SECT_VISUAL,0x00000000);
    VG.PDP.NrmBkColor	= uiGetHex   ("NrmBkColor"		,TT_INI_SECT_VISUAL,0x005A8C7B);	// Normal
	VG.PDP.NrmFnColor	= uiGetHex   ("NrmFnColor"		,TT_INI_SECT_VISUAL,0x00000000);
    VG.PDP.Nrm8BMBkColor= uiGetHex   ("Nrm8BMBkColor"	,TT_INI_SECT_VISUAL,0x005D9180);	// Normal 8 Beat Multiple
	VG.PDP.Nrm8BMFnColor= uiGetHex   ("Nrm8BMFnColor"	,TT_INI_SECT_VISUAL,0x00000000);
    VG.PDP.NrmBkColor	= uiGetHex   ("NrmBkColor"		,TT_INI_SECT_VISUAL,0x005A8C7B);	// Normal
	VG.PDP.NrmFnColor	= uiGetHex   ("NrmFnColor"		,TT_INI_SECT_VISUAL,0x00000000);
	VG.PDP.NumBkColor	= uiGetHex   ("NumBkColor"		,TT_INI_SECT_VISUAL,0x005A8C7B);	// Numbers
	VG.PDP.NumFnColor	= uiGetHex   ("NumFnColor"		,TT_INI_SECT_VISUAL,0x0019231D);
	VG.PDP.SelBkColor	= uiGetHex   ("SelBkColor"		,TT_INI_SECT_VISUAL,0x00131E1B);	// Marked block
	VG.PDP.SelFnColor	= uiGetHex   ("SelFnColor"		,TT_INI_SECT_VISUAL,0x005A8C7B);   
	VG.PDP.CurFnColor	= uiGetHex   ("CurFnColor"		,TT_INI_SECT_VISUAL,0x00000000);	// Current beat
	VG.PDP.CurBkColor	= uiGetHex   ("CurBkColor"		,TT_INI_SECT_VISUAL,0x0088DB9D);
	VG.PDP.PattEdBk		= uiGetHex   ("PattEdBk"		,TT_INI_SECT_VISUAL,0x005A8C7B);

	VG.Acorde			= iGetInteger("ChordStep"		,TT_INI_SECT_EDITBAR,1); // Salto a cada pulsacion
	if (VG.Acorde<1) VG.Acorde = 1; else if (VG.Acorde>8) VG.Acorde = 8;

    VG.Arpegio			= iGetInteger("ArpStep"			,TT_INI_SECT_EDITBAR,1);	// Salto a cada pulsacion1; // Edición en una sola pista inicialmente
	if (VG.Arpegio<1) VG.Arpegio = 1; else if (VG.Arpegio>8) VG.Arpegio = 8;

    VG.SaltoLinea       = iGetInteger("LineStep"		,TT_INI_SECT_EDITBAR,8);	// Salto a cada pulsacion1; // Edición en una sola pista inicialmente
	if (VG.SaltoLinea<1) VG.SaltoLinea = 1; else if (VG.SaltoLinea>64) VG.SaltoLinea = 64;

    VG.Transponer       = iGetInteger("TransposeInput"	,TT_INI_SECT_EDITBAR,0);	// Salto a cada pulsacion1; // Edición en una sola pista inicialmente
	if (VG.Transponer<-24) VG.Transponer = -24; else if (VG.Transponer>24) VG.Transponer = 24;


	VG.DispositivoEntradaMIDI = iGetInteger("MIDIInputPort" ,TT_INI_SECT_MIDIIN,0);
	VG.iRemoteMIDIInput = iGetInteger("RemoteMIDIInput"		,TT_INI_SECT_MIDIIN,0);
	VG.asMIDIInputHost  = szGetString("MIDIInputHost"		,TT_INI_SECT_MIDIIN,"localhost");



    //Leer lista de ficheros recientes
	VG.oRcntFiles.Inicializar(4);

	int iNumFiles = iGetInteger("NumFiles",TT_INI_SECT_RFILES,0);
    for (i=0;i<iNumFiles;i++)
	{
		AnsiString asFile;
		char szVar[256];	
		
		sprintf(szVar,"recentfile%d",iNumFiles-i-1);	// insertamos en orden inverso
		asFile = szGetString(szVar,TT_INI_SECT_RFILES,"");

		if (asFile.Length() > 0)
			VG.oRcntFiles.AnyadirElemento(asFile);
	}

	// Listas de instrumentoss
	int iNumInstLists = 0;

	// Write the number of different configs
	iNumInstLists = iGetInteger("NumInstLists",TT_INI_SECT_INSTLST,0);

	// Write each config
	for (k = 0;k<iNumInstLists;k++)
	{
		char szVar[256];
		int iDev;
		int iBank;

		sprintf(szVar,"il_dev_%d",k);
		iDev = iGetInteger(szVar,TT_INI_SECT_INSTLST,0);
				
		sprintf(szVar,"il_bank_%d",k);
		iBank = iGetInteger(szVar,TT_INI_SECT_INSTLST,0);
				
		sprintf(szVar,"il_file_%d",k);
		VG.FicherosListasInstrumentos[iDev][iBank] = szGetString(szVar,TT_INI_SECT_INSTLST,"");
	}

	// Retrieve language file
	VG.asLangFile		= szGetString("LanguageFile"	,TT_INI_SECT_MISC,"spanish.txt");	
}
// ----------------------------------------------------------
void WriteConfig(char *_szIniFile)
{	
	TT_INIFILE = _szIniFile;
	int i,j,k;

	SetString ("Font"			,TT_INI_SECT_VISUAL,VG.PDP.Font.c_str());
	SetInteger("FontSize"		,TT_INI_SECT_VISUAL,VG.PDP.FontSize);

	SetHex    ("MrkBtBkColor"	,TT_INI_SECT_VISUAL,VG.PDP.MrkBtBkColor);
	SetHex    ("MrkBtFnColor"	,TT_INI_SECT_VISUAL,VG.PDP.MrkBtFnColor);
    SetHex    ("NrmBkColor"		,TT_INI_SECT_VISUAL,VG.PDP.NrmBkColor);	// Normal
	SetHex    ("NrmFnColor"		,TT_INI_SECT_VISUAL,VG.PDP.NrmFnColor);
    SetHex    ("Nrm8BMBkColor"	,TT_INI_SECT_VISUAL,VG.PDP.Nrm8BMBkColor);// Normal 8 Beat Multiple
	SetHex    ("Nrm8BMFnColor"	,TT_INI_SECT_VISUAL,VG.PDP.Nrm8BMFnColor);
    SetHex    ("NrmBkColor"		,TT_INI_SECT_VISUAL,VG.PDP.NrmBkColor);	// Normal
	SetHex    ("NrmFnColor"		,TT_INI_SECT_VISUAL,VG.PDP.NrmFnColor);
	SetHex    ("NumBkColor"		,TT_INI_SECT_VISUAL,VG.PDP.NumBkColor);	// Numbers
	SetHex    ("NumFnColor"		,TT_INI_SECT_VISUAL,VG.PDP.NumFnColor);
	SetHex    ("SelBkColor"		,TT_INI_SECT_VISUAL,VG.PDP.SelBkColor);	// Marked block
	SetHex    ("SelFnColor"		,TT_INI_SECT_VISUAL,VG.PDP.SelFnColor);   
	SetHex    ("CurFnColor"		,TT_INI_SECT_VISUAL,VG.PDP.CurFnColor);	// Current beat
	SetHex    ("CurBkColor"		,TT_INI_SECT_VISUAL,VG.PDP.CurBkColor);
	SetHex    ("PattEdBk"		,TT_INI_SECT_VISUAL,VG.PDP.PattEdBk	);

	SetInteger("ChordStep"		,TT_INI_SECT_EDITBAR,	VG.Acorde);			// Salto a cada pulsacion
    SetInteger("ArpStep"		,TT_INI_SECT_EDITBAR,	VG.Arpegio);		// Salto a cada pulsacion1; // Edición en una sola pista inicialmente
    SetInteger("LineStep"		,TT_INI_SECT_EDITBAR,	VG.SaltoLinea);		// Salto a cada pulsacion1; // Edición en una sola pista inicialmente
    SetInteger("TransposeInput"	,TT_INI_SECT_EDITBAR,	VG.Transponer);		// Salto a cada pulsacion1; // Edición en una sola pista inicialmente

	SetInteger("MIDIInputPort"	,TT_INI_SECT_MIDIIN,VG.DispositivoEntradaMIDI);
	SetInteger("RemoteMIDIInput",TT_INI_SECT_MIDIIN,VG.iRemoteMIDIInput);
	SetString ("MIDIInputHost"	,TT_INI_SECT_MIDIIN,VG.asMIDIInputHost.c_str());

    //Escribir lista de ficheros recientes
	SetInteger("NumFiles",TT_INI_SECT_RFILES,VG.oRcntFiles.C.NumStrEnCola);

    for (i=0;i<VG.oRcntFiles.C.NumStrEnCola;i++)
	{
		char szVar[256];
		sprintf(szVar,"recentfile%d",i);
		SetString(szVar,TT_INI_SECT_RFILES,VG.oRcntFiles.LeerElemento(i).c_str());
	}

	
	// Listas de instrumentoss
	int iNumInstLists = 0;
	for (i=0;i<VG.NumPorts;i++)
		for (j=0;j<MaxBancos;j++)
			if (VG.FicherosListasInstrumentos[i][j] != "")
				iNumInstLists++;
	
	// Write the number of different configs
	SetInteger("NumInstLists",TT_INI_SECT_INSTLST,iNumInstLists);

	// Write each config
	k = 0;
	for (i=0;i<VG.NumPorts;i++)
		for (j=0;j<MaxBancos;j++)		
			if (VG.FicherosListasInstrumentos[i][j] != "")
			{
				char szVar[256];

				sprintf(szVar,"il_dev_%d",k);
				SetInteger(szVar,TT_INI_SECT_INSTLST,i);
				
				sprintf(szVar,"il_bank_%d",k);
				SetInteger(szVar,TT_INI_SECT_INSTLST,j);
				
				sprintf(szVar,"il_file_%d",k);
				SetString(szVar,TT_INI_SECT_INSTLST,VG.FicherosListasInstrumentos[i][j].c_str());
				
				k++;
			}

	// Lenguaje
	SetString("LanguageFile"	,TT_INI_SECT_MISC,VG.asLangFile.c_str());
	
}
// ----------------------------------------------------------