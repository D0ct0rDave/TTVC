//---------------------------------------------------------------------------
#ifdef USE_PRECOMPILED_HEADERS
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
#endif
//---------------------------------------------------------------------------
#include "OMidiOut.h"

// #include <WinTypes.h>
#include <Windows.h>
// #include <WinProcs.h>

// ------------------------------------------------------------------------------
void *GlobalAllocLockedMem(unsigned int Cantidad,HANDLE &Handler)
{
    void *p;
    Handler = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE | GMEM_ZEROINIT,Cantidad);

    if (Handler)
    {
        p = GlobalLock(Handler);

        if (p == NULL)
            GlobalFree(Handler);

        return ( p );
    }
    else
        return ( NULL );
}

void GlobalFreeLockedMem(HANDLE Handler,void *P)
{
     GlobalUnlock(Handler);
     GlobalFree  (Handler);
}
// ------------------------------------------------------------------------------
int NumeroDispositivosMIDIOut()
{
     return(midiOutGetNumDevs());
}

char *ObtenerNombreDispositivoMidiOut(int Dispositivo)
{
    static MIDIOUTCAPS  M;
	static MMRESULT     FError;

	FError = midiOutGetDevCaps(Dispositivo,&M,sizeof(MIDIOUTCAPS));
	
	if (FError == MMSYSERR_NOERROR)
		return ( M.szPname);
	else
		return(NULL);
}
// ------------------------------------------------------------------------------
TObjetoMidiOut::TObjetoMidiOut()
{
     Abierto = false;
     // Tomar memoria para el Buffer de SysEx
     BufferSysEx    = (char*)GlobalAllocLockedMem(1024,BufferSysExHnd);
     BufferSysExHdr = (PMIDIHDR)GlobalAllocLockedMem(sizeof(MIDIHDR),BufferSysExHdrHnd);

     BufferSysExHdr->lpData         = BufferSysEx;
     BufferSysExHdr->dwBufferLength = 1024;
     BufferSysExHdr->dwUser         = 0;
};

TObjetoMidiOut::~TObjetoMidiOut()
{
     GlobalFreeLockedMem(BufferSysExHnd,    BufferSysEx);
     GlobalFreeLockedMem(BufferSysExHdrHnd, BufferSysExHdr);
};

void  TObjetoMidiOut::Open(void *FuncionCallBack)
{
    MIDIOUTCAPS  M;
    MMRESULT     FError;
    DWORD        Flags;

    if (! Abierto)
    {
        if (FuncionCallBack==NULL)
          Flags=CALLBACK_NULL;
        else
          Flags=CALLBACK_FUNCTION;

        FError = midiOutOpen((LPHMIDIOUT)&Handler,      // se supone que nos lo devuelve
                           DispositivoActual,           // dispositivo a usar
                           (DWORD)FuncionCallBack,		// direccion de la RSI
                           (DWORD)0,					// parametro de usuario
                           Flags);                      // Flags

        if (FError==MMSYSERR_NOERROR)
            Abierto = true;
    }
}

void TObjetoMidiOut::Close()
{
     if (Abierto)
     {
          midiOutClose(Handler);
          Abierto = false;
     };
};

void TObjetoMidiOut::Comando(unsigned char C,unsigned char V1,unsigned char V2)
{
    DWORD CMD;

    if (Abierto)
    {
        CMD = DWORD(C) | (DWORD(V1) << 8) | (DWORD(V2) << 16);
        midiOutShortMsg(Handler,CMD);
    };
};


void TObjetoMidiOut::SysEx(unsigned char *pucBuffer,unsigned int Num)
{
    MMRESULT     FError;

    if (Abierto)
    {
        // Copiar a la zona del buffer
        memcpy(BufferSysEx,pucBuffer,Num);
        BufferSysExHdr->dwBufferLength=Num;

        FError = midiOutPrepareHeader(Handler,BufferSysExHdr,sizeof(MIDIHDR));

        if (FError == 0)
            midiOutLongMsg(Handler,(LPMIDIHDR)BufferSysEx,Num);
    }
}

void  TObjetoMidiOut::EstablecerDispositivo(int D)
{
     if (! Abierto)
        DispositivoActual = D;
};

int TObjetoMidiOut::Dispositivo()
{
     return( DispositivoActual );
};
// ------------------------------------------------------------------------------

