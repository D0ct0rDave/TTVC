#ifndef Pistas22
#define Pistas22

#include "Tipos.h"
#include "INTER.h"


short CalcularTrasposicionPista(PTrack T,TBeatNum Desde,TBeatNum Hasta,short Semitonos);
void  TrasponerPista           (PTrack T,TBeatNum Desde,TBeatNum Hasta,short Semitonos);

short CalcularVolumenPista     (PTrack T,TBeatNum Desde,TBeatNum Hasta,unsigned char Tipo,short v1,short v2);
void ModificarVolumenPista     (PTrack T,TBeatNum Desde,TBeatNum Hasta,unsigned char Tipo,short v1,short v2);

short CalcularEfectoPista      (PTrack T,TBeatNum Desde,TBeatNum Hasta,unsigned char Eff,unsigned char Tipo,short v1,short v2);
void ModificarEfectoPista      (PTrack T,TBeatNum Desde,TBeatNum Hasta,unsigned char Eff,unsigned char Tipo,short v1,short v2);

short CalcularVolumenPistaCancion(PCancion S,TTrackNum Track,TBeatNum Desde,TBeatNum Hasta,unsigned char Tipo,short v1,short v2);
void ModificarVolumenPistaCancion(PCancion S,TTrackNum Track,TBeatNum Desde,TBeatNum Hasta,unsigned char Tipo,short v1,short v2);

short CalcularTrasposicionPistaCancion(PCancion S,TTrackNum Track,TBeatNum Desde,TBeatNum Hasta,short Semitonos);
void TrasponerPistaCancion(PCancion S,TTrackNum Track,TBeatNum Desde,TBeatNum Hasta, short Semitonos);

#endif 