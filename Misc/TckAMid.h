//------------------------------------------------------------------------------
#ifndef TckAMidH
#define TckAMidH
//------------------------------------------------------------------------------
#include "inter.h"
//------------------------------------------------------------------------------


const int MaxBytesEM            = 128;

const int COMANDO_NOTEON        = 0x90;
const int COMANDO_NOTEOFF       = 0x80;

const int COMANDO_CONTROLCHANGE = 0xB0;

const int COMANDO_CAMBIOBANCO0  = 0x00;
const int COMANDO_CAMBIOBANCO32 = 0x20;
const int COMANDO_CAMBIOPROGRAMA= 0xC0;

//------------------------------------------------------------------------------

    typedef enum TMetaEvent {
        METAEVENT_TEXT,
        METAEVENT_COPYRIGHT,
        METAEVENT_TRACKNAME,
        METAEVENT_ENDOFTRACK,
        METAEVENT_SETTEMPO,
        METAEVENT_TIMESIGNATURE,
        METAEVENT_KEYSIGNATURE
    }TMetaEvent;

//------------------------------------------------------------------------------

    typedef struct {
        unsigned int Time;
        unsigned char D[MaxBytesEM];
        unsigned int Tam;
    }TEventoMIDI;

    typedef TEventoMIDI* PEventoMIDI;

//------------------------------------------------------------------------------
    typedef void (* TConvertTCK2MIDIEffect)(unsigned char Eff,unsigned char Val,TTrackNum T,TEventoMIDI &EvEff);

//------------------------------------------------------------------------------

bool EscribirFicheroMIDI(AnsiString Nombre,bool Sobreescribir);
void EstablecerFuncionDeConversionTCK2Mid(TConvertTCK2MIDIEffect FuncionDeConversion);
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------

 