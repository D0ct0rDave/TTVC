// ----------------------------------------------------------------------------
#ifndef InterH
#define InterH

// ----------------------------------------------------------------------------
#include "tipos.h"
#include <stdio.h>

/* ---------------------------------------------------------------------------- */
/* Tratamiento de la cancion                                                    */
/* ---------------------------------------------------------------------------- */
void CrearCancion(PCancion &C);
void DestruirCancion(PCancion &C);
bool CancionVacia(PCancion C);
unsigned short NumeroDePatterns(PCancion C);
bool CargarCancionGeneral(PCancion &C,FILE *F);
bool SalvarCancionGeneral(PCancion C,FILE *F);
PT98Pattern ObtenerPatronDeCancion(PCancion C,TPatternNum P);
PT98Pattern ObtenerPatronDeSecuencia(PCancion C,TIndexSec PActual);


/*----------------------------------------------------------------------------}
{Tratamiento de la Secuencia                                                 }
{----------------------------------------------------------------------------*/
void CrearSecuencia(PSecuencia &S);
void DestruirSecuencia(PSecuencia &S);

bool InsertarPatronEnSecuencia(PSecuencia S,TIndexSec Pos);
bool BorrarPatronDeSecuencia(PSecuencia S,TIndexSec & Pos);
TPatternNum LeerIndicePatronEnSecuencia(PSecuencia S,TIndexSec I);
void EscribirIndicePatronDeSecuencia(PSecuencia S,TIndexSec I,TPatternNum P);
TIndexSec NumPatronesEnSecuencia(PSecuencia S);

bool CargarSecuenciaGeneral(PSecuencia &S,FILE * F,unsigned short & ID);
bool SalvarSecuenciaGeneral(PSecuencia S,FILE *F,unsigned short ID);

int TamanyoSecuencia(PSecuencia S);
/*----------------------------------------------------------------------------}
{Tratamiento de la Tabla de Instrumentos                                     }
{----------------------------------------------------------------------------*/
void CrearTablaIns(PTablaIns &T);
void DestruirTablaIns(PTablaIns &T);
PInfoTrack LeerInfoDeTablaIns(PTablaIns T,TTrackNum Track);

bool CargarTablaInsGeneral(PTablaIns &T,FILE *F,unsigned short &ID);
bool SalvarTablaInsGeneral(PTablaIns T,FILE *F,unsigned short ID);
int TamanyoTablaIns(PTablaIns T);

/*----------------------------------------------------------------------------}
{Tratamiento del patron                                                      }
{----------------------------------------------------------------------------*/
void CrearPattern(PT98Pattern &P);
void DestruirPattern(PT98Pattern &P);

void CopiarPattern(PT98Pattern F,PT98Pattern D);
void BorrarPattern(PT98Pattern P);

PTrack LeerTrackDePattern(PT98Pattern P,TTrackNum I);

bool CargarPatternGeneral(PT98Pattern &P,FILE *F,unsigned short & ID);
bool SalvarPatternGeneral(PT98Pattern P,FILE *F,unsigned short ID);
tt98String LeerNombreDelPattern(PT98Pattern P);
void EscribirNombreDelPattern(PT98Pattern P,String Nombre);
bool PatternVacio(PT98Pattern P);
TTrackNum NumeroDeTracksEnPattern(PT98Pattern P);
int TamanyoPattern(PT98Pattern P);

/*----------------------------------------------------------------------------}
{Tratamiento del track                                                       }
{----------------------------------------------------------------------------*/
/* No hay creacion de tracks ni destruccion, la unidad minima de creaci¢n es el
patron. La unidad minima tratable es el beat con sus campos */
void CrearTrack(PTrack T);
void CopiarTrack(PTrack Fuente,PTrack Destino);
bool CargarTrackGeneral(PTrack T,FILE *F,unsigned short &ID);
bool SalvarTrackGeneral(PTrack T,FILE *F,unsigned short ID);

void InsertarPosicionTrack(PTrack T,TBeatNum B);
void BorrarPosicionTrack(PTrack T,TBeatNum B);
void InsertarPosicionesTrack(PTrack T,TBeatNum B,TBeatNum Num);
void BorrarPosicionesTrack(PTrack T,TBeatNum B,TBeatNum num);

void InsertarEventoTrack(PTrack T,TBeatNum Time,unsigned char N,unsigned char V,unsigned char F,unsigned char A);
void EliminarEventoTrack(PTrack T,TBeatNum Time);

PDatosEvento TomarEventoPorTiempoTrack(PTrack T,TBeatNum Time);
bool EstaEventoTrack(PTrack T,TBeatNum Time);
bool TrackVacio(PTrack T);
short NumEventosTrack(PTrack T);
int TamanyoTrack(PTrack T);

/*----------------------------------------------------------------------------}
{Tratamiento de los beats dentro del track                                   }
{----------------------------------------------------------------------------*/
BeatString LeerBeatTrack(PTrack T,TBeatNum Beat);
void EscribirBeatTrack(PTrack T,TBeatNum Beat,unsigned char Nota,unsigned char V,unsigned char F,unsigned char A);

void EscribirNotaBeat(PTrack T,TBeatNum Beat,unsigned char Nota);
void EscribirVolBeat(PTrack T,TBeatNum Beat,unsigned char Vol);
void EscribirEffBeat(PTrack T,TBeatNum Beat,unsigned char Eff);
void EscribirValBeat(PTrack T,TBeatNum Beat,unsigned char Val);

unsigned char LeerNotaBeat(PTrack T,TBeatNum Beat);
unsigned char LeerVolBeat(PTrack T,TBeatNum Beat);
unsigned char LeerEffBeat(PTrack T,TBeatNum Beat);
unsigned char LeerValBeat(PTrack T,TBeatNum Beat);
// ----------------------------------------------------------------------------
tt98String DescripcionDeError();
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------