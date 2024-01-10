//---------------------------------------------------------------------------
#ifdef USE_PRECOMPILED_HEADERS
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
#endif

//---------------------------------------------------------------------------
#include "OMidiIn.h"
// -----------------------------------------------------------------------------
int NumeroDispositivosMIDIIn()
{
     return ( midiInGetNumDevs() );
};
// -----------------------------------------------------------------------------
char *ObtenerNombreDispositivoMidiIn(int Dispositivo)
{
    static MIDIINCAPS  M;
    static MMRESULT    FError;

    midiInGetDevCaps(Dispositivo,&M,sizeof(MIDIINCAPS));
    if (FError == MMSYSERR_NOERROR)
        return (M.szPname);
    else
        return(NULL);
};
// -----------------------------------------------------------------------------
TObjetoMidiIn::TObjetoMidiIn()
{
    Abierto = false;
}
// -----------------------------------------------------------------------------
void  TObjetoMidiIn::Open(void *FuncionCallBack)
{
    MMRESULT FError;
    DWORD    Flags;

    if (! Abierto)
    {
        if (FuncionCallBack==NULL)
          Flags=CALLBACK_NULL;
        else
          Flags=CALLBACK_FUNCTION;

        FError = midiInOpen((LPHMIDIIN)&Handler,          // se supone que nos lo devuelve
                           DispositivoActual,             // dispositivo a usar
                           (DWORD)FuncionCallBack,    // direccion de la RSI
                           (DWORD)0,                  // parametro de usuario
                           Flags);                        // Flags

        if (FError==MMSYSERR_NOERROR)
          Abierto=true;
    };
};
// -----------------------------------------------------------------------------
void  TObjetoMidiIn::Close()
{
     if (Abierto)
     {
          midiInClose(Handler);
          Abierto = false;
     }
}
// -----------------------------------------------------------------------------
void  TObjetoMidiIn::Start()
{
     if (Abierto) midiInStart(Handler);
}
// -----------------------------------------------------------------------------
void  TObjetoMidiIn::Stop()
{
     if (Abierto) midiInStop(Handler);
}
// -----------------------------------------------------------------------------
void  TObjetoMidiIn::EstablecerDispositivo(int D)
{
     if (! Abierto) DispositivoActual = D;
}
// -----------------------------------------------------------------------------
int TObjetoMidiIn::Dispositivo()
{
     return (DispositivoActual);
}
// -----------------------------------------------------------------------------
