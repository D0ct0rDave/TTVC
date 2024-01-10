PROCEDURE ResetSistemaMIDI;cdecl;

PROCEDURE ActivarPropiedadesMIDI(Info:PInfoTrack);cdecl;
PROCEDURE InicializarSistemaReproduccion(T:PTablains);cdecl;

PROCEDURE PlayNoteOn(Dispositivo,Canal,Note,Velocity:BYTE);cdecl;
PROCEDURE PlayNoteOff(Dispositivo,Canal,Note:BYTE);cdecl;

PROCEDURE PlayBeat(T:PTrack;Info:PInfoTrack;Beat:TBeatNum;Track:TTrackNum);cdecl;
PROCEDURE PlayLineaBeats(P:PPattern;TI:PTablaIns;Beat:TBeatNum);cdecl;

PROCEDURE PlayTrack(Track:PTrack;T:TTrackNum;Info:PInfoTrack);cdecl;
PROCEDURE StopTrack;cdecl;

PROCEDURE PlayPattern(Pattern:PPattern;TI:PTablaIns);cdecl;
PROCEDURE StopPattern;cdecl;

PROCEDURE PlayCancion(C:PCancion);cdecl;
PROCEDURE StopCancion;cdecl;

PROCEDURE EstablecerEstadoTrack(Track:TTrackNum;Mute:BOOLEAN);cdecl;
PROCEDURE EstablecerLooping(OnOff:BOOLEAN);cdecl;

PROCEDURE EstablecerBeatsPerMinute(B_P_M_S:WORD);cdecl;
FUNCTION TomarBeatsPerMinute:WORD;cdecl;
PROCEDURE ObtenerMinMaxBPMS(VAR Min,Max:WORD);cdecl;

PROCEDURE ObtenerEstadoReproduccion(VAR Beat:TBeatNum;VAR Pattern:TPatternNum;VAR IndSec:TIndexSec);cdecl;
PROCEDURE EstablecerEstadoReproduccion(Beat:TBeatNum;IndSec:TIndexSec);cdecl;

FUNCTION Reproduciendo:BOOLEAN;cdecl;

procedure EstablecerFuncionConversionTCKEffect2MIDIEvent(funcion:pointer);cdecl;
PROCEDURE Finalizacion;cdecl;
