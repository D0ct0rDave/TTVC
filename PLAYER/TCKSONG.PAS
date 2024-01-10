LIBRARY TCKSONG;
USES SHAREMEM,TIPOS,INTER;

EXPORTS

CrearCancion INDEX 1,
DestruirCancion INDEX 2,
CancionVacia INDEX 3,
NumeroDePatterns INDEX 4,
CargarCancion INDEX 5,
SalvarCancion INDEX 6,
ObtenerPatronDeCancion INDEX 7,
ObtenerPatronDeSecuencia INDEX 8,

{----------------------------------------------------------------------------}
{Tratamiento de la Secuencia                                                 }
{----------------------------------------------------------------------------}
CrearSecuencia INDEX 9,
DestruirSecuencia INDEX 10,
InsertarPatronEnSecuencia INDEX 11,
BorrarPatronDeSecuencia INDEX 12,
LeerIndicePatronEnSecuencia INDEX 13,
EscribirIndicePatronDeSecuencia INDEX 14,
NumPatronesEnSecuencia INDEX 15,

CargarSecuencia INDEX 16,
SalvarSecuencia INDEX 17,

{----------------------------------------------------------------------------}
{TratamientodelaTabladeInstrumentos}
{----------------------------------------------------------------------------}
CrearTablaIns INDEX 18,
DestruirTablaIns INDEX 19,
LeerInfoDeTablaIns INDEX 20,

CargarTablaIns INDEX 21,
SalvarTablaIns INDEX 22,

{----------------------------------------------------------------------------}
{Tratamiento del patron}
{----------------------------------------------------------------------------}
CrearPattern INDEX 23,
DestruirPattern INDEX 24,

CopiarPattern INDEX 25,
BorrarPattern INDEX 26,

LeerTrackDePattern INDEX 27,

CargarPattern INDEX 28,
SalvarPattern INDEX 29,
NumeroDeTracksEnPattern INDEX 30,
PatternVacio INDEX 31,

{----------------------------------------------------------------------------}
{Tratamiento del track}
{----------------------------------------------------------------------------}

CopiarTrack INDEX 32,
CargarTrack INDEX 33,
SalvarTrack INDEX 34,
InsertarPosicionTrack INDEX 35,
BorrarPosicionTrack INDEX 36,
InsertarPosicionesTrack INDEX 37,
BorrarPosicionesTrack INDEX 38,

InsertarEventoTrack INDEX 39,
EliminarEventoTrack INDEX 40,

TomarEventoPorTiempoTrack INDEX 41,
EstaEventoTrack INDEX 42,
TrackVacio INDEX 43,
NumEventosTrack INDEX 44,

{----------------------------------------------------------------------------}
{Tratamientodelosbeatsdentrodeltrack}
{----------------------------------------------------------------------------}
LeerBeatTrack INDEX 45,
EscribirBeatTrack INDEX 46,

EscribirNotaBeat INDEX 47,
EscribirVolBeat INDEX 48,
EscribirEffBeat INDEX 49,
EscribirValBeat INDEX 50,

LeerNotaBeat INDEX 51,
LeerVolBeat INDEX 52,
LeerEffBeat INDEX 53,
LeerValBeat INDEX 54;

END.
