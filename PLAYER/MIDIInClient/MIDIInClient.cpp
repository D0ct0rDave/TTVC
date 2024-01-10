// --------------------------------------------------------------------------------------
// MIDIInClient.cpp : Defines the entry point for the console application.
//
// MIDIInServer.cpp : Defines the entry point for the console application.
//
// --------------------------------------------------------------------------------------
#include <windows.h>
#include <mmSystem.h>
#include "CNETSystem.h"
#include "CNETClient.h"

#include "MIDIInClient.h"
// --------------------------------------------------------------------------------------
#define MIDIIN_SERVER_PORT	12345
#define TIMER_INTERVAL		25
// --------------------------------------------------------------------------------------
//void __stdcall PattEd::RSIMidiIn(

typedef void (__stdcall  * MidiInProc)(HMIDIIN ldDisp,unsigned short Msj,DWORD dir,DWORD Mensaje,DWORD Tiempo);

/*

  HMIDIIN hMidiIn,  
  UINT wMsg,        
  DWORD dwInstance, 
  DWORD dwParam1,   
  DWORD dwParam2    
);
*/

// --------------------------------------------------------------------------------------

CNETClient		goClient;
MidiInProc		gpCallback;
unsigned int	guiTimerID;
bool			gbConnect;			
char			gszServer[256];


// --------------------------------------------------------------------------------------
MIDIInClient::MIDIInClient()
{
	Abierto = false;
}
// --------------------------------------------------------------------------------------
MIDIInClient::~MIDIInClient()
{
	if ((Abierto) || (gbConnect))
		Close();
}
// --------------------------------------------------------------------------------------
MIDIInClient::MIDIInClient(char *_szMIDIInServer)
{
	strncpy(gszServer,_szMIDIInServer,256);	gszServer[255] = 0;
	Abierto = false;
}
// --------------------------------------------------------------------------------------
void CALLBACK TIMERProc(
	HWND hwnd,		// handle of window for timer messages
	UINT uMsg,		// WM_TIMER message
	UINT idEvent,	// timer identifier
	DWORD dwTime	// current system time
)
{
	TEventoMidiIn	oMIDIEvent;
	int				iDataSize;

	// Poll for data
	if (goClient.bGetInfo(&oMIDIEvent,iDataSize))
	{
		// Mandar mensaje MIDI
		gpCallback(	NULL,
					MIM_DATA,
					0,
					(oMIDIEvent.Cmd) | (oMIDIEvent.Data1 << 8) | ( oMIDIEvent.Data2 << 16),
					0);
	}
}
// --------------------------------------------------------------------------------------
void MIDIInClient::Open(void *FuncionCallBack)
{
	CNETSystem::Init();

	// Connect client to server
	gbConnect = goClient.bConnect(gszServer,MIDIIN_SERVER_PORT);
	if (! gbConnect) return;
	
	// Create a timer to poll for MIDI In messages
	guiTimerID = SetTimer(0,(unsigned int)this,TIMER_INTERVAL,TIMERProc);

	// Set MIDIIn Callback func
	gpCallback = (MidiInProc)FuncionCallBack;

	// Device is now open
	Abierto = true;
}
// --------------------------------------------------------------------------------------
void MIDIInClient::Close()
{
	CNETSystem::Finish();	

	KillTimer(0,guiTimerID);
	
	gbConnect	= false;
	Abierto		= false;
}
// --------------------------------------------------------------------------------------
void MIDIInClient::Start()
{
}

void MIDIInClient::Stop()
{
}
// --------------------------------------------------------------------------------------