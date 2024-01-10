//---------------------------------------------------------------------------
#ifndef tt98PlayerH
#define tt98PlayerH
//---------------------------------------------------------------------------
#include "tipos.h"

void ResetSistemaMIDI();

void ActivarPropiedadesMIDI(PInfoTrack Info);
void InicializarSistemaReproduccion(PTablaIns T);

void PlayNoteOn(unsigned char Dispositivo,unsigned char Canal,unsigned char Note,unsigned char Velocity);
void PlayNoteOff(unsigned char Dispositivo,unsigned char Canal,unsigned char Note);

void PlayBeat(PTrack T,PInfoTrack Info,TBeatNum Beat,TTrackNum Track);
void PlayLineaBeats(PT98Pattern P,PTablaIns TI,TBeatNum Beat);

void PlayTrack(PTrack Track,TTrackNum T,PInfoTrack Info);
void StopTrack();

void PlayPattern(PT98Pattern Pattern,PTablaIns TI);
void StopPattern();

void PlayCancion(PCancion C);
void StopCancion();

void EstablecerEstadoTrack(TTrackNum Track,bool Mute);
void EstablecerLooping(bool OnOff);

void EstablecerBeatsPerMinute(unsigned short B_P_M_S);
unsigned short TomarBeatsPerMinute();
void ObtenerMinMaxBPMS(unsigned short &Min,unsigned short &Max);

void ObtenerEstadoReproduccion(TBeatNum &Beat,TPatternNum &Pattern,TIndexSec &IndSec);
void EstablecerEstadoReproduccion(TBeatNum Beat,TIndexSec IndSec);

bool Reproduciendo();

void EstablecerFuncionConversionTCKEffect2MIDIEvent(void *funcion);
void Finalizacion();
//---------------------------------------------------------------------------
#endif
 