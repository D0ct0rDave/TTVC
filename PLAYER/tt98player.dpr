LIBRARY tt98player;

USES OMIDIOUT in 'OMIDIOUT.PAS',
  Play in 'Play.pas',
  SMem in 'SMEM.PAS',
  Tipos in 'Tipos.pas',
  Consts2 in 'Consts2.pas',
  INTER in 'Inter.pas',
  TEfectos in 'Tefectos.pas';

EXPORTS

// LeerConfig INDEX 1,
ResetSistemaMIDI INDEX 2,

PlayNoteOn INDEX 3,
PlayNoteOff INDEX 4,
PlayBeat INDEX 5,
PlayLineaBeats INDEX 6,

PlayTrack INDEX 7,
StopTrack INDEX 8,

PlayPattern INDEX 9,
StopPattern INDEX 10,

PlayCancion INDEX 11,
StopCancion INDEX 12,

EstablecerEstadoTrack INDEX 13,
EstablecerLooping INDEX 14,
ObtenerEstadoReproduccion INDEX 15,
Reproduciendo INDEX 16,
Finalizacion INDEX 17,

InicializarSistemaReproduccion INDEX 18,
// ObtenerTablaEfectos INDEX 20,

EstablecerBeatsPerMinute INDEX 21,
TomarBeatsPerMinute   INDEX 22,
ObtenerMinMaxBPMS INDEX 23,
EstablecerEstadoReproduccion INDEX 24,
EstablecerFuncionConversionTCKEffect2MIDIEvent INDEX 25,
ActivarPropiedadesMIDI INDEX 26;

END.
