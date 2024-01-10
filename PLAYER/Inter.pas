UNIT INTER;

INTERFACE

USES
    TIPOS;

{----------------------------------------------------------------------------}
{Tratamiento de la cancion                                                   }
{----------------------------------------------------------------------------}
PROCEDURE CrearCancion   (VAR C:PCancion);
PROCEDURE DestruirCancion(VAR C:PCancion);
FUNCTION CancionVacia    (C:PCancion):BOOLEAN;
FUNCTION NumeroDePatterns(C:PCancion):WORD;
FUNCTION CargarCancionGeneral   (VAR C:PCancion;VAR F:File):boolean;
FUNCTION SalvarCancionGeneral   (C:PCancion;VAR F:File):boolean;
FUNCTION ObtenerPatronDeCancion  (C:PCancion;P:TPatternNum):PPattern;
FUNCTION ObtenerPatronDeSecuencia(C:PCancion;PActual:TIndexSec):PPattern;
                              {.........}

{----------------------------------------------------------------------------}
{Tratamiento de la Secuencia                                                 }
{----------------------------------------------------------------------------}
PROCEDURE CrearSecuencia(VAR S:PSecuencia);
PROCEDURE DestruirSecuencia(VAR S:PSecuencia);

FUNCTION InsertarPatronEnSecuencia       (S:PSecuencia;Pos:TIndexSec):boolean;
FUNCTION BorrarPatronDeSecuencia         (S:PSecuencia;VAR Pos:TIndexSec):boolean;
function LeerIndicePatronEnSecuencia     (S:PSecuencia;I:TIndexSec):TPatternNum;
PROCEDURE EscribirIndicePatronDeSecuencia(S:PSecuencia;I:TIndexSec;P:TPatternNum);
FUNCTION NumPatronesEnSecuencia          (S:PSecuencia):TIndexSec;

FUNCTION CargarSecuenciaGeneral (VAR S:PSecuencia;VAR F:File;VAR ID:WORD):BOOLEAN;
FUNCTION SalvarSecuenciaGeneral (S:PSecuencia;VAR F:File;ID:WORD):BOOLEAN;
FUNCTION TamanyoSecuencia(S:PSecuencia):LONGINT;
{----------------------------------------------------------------------------}
{Tratamiento de la Tabla de Instrumentos                                     }
{----------------------------------------------------------------------------}
PROCEDURE CrearTablaIns    (VAR T:PTablaIns);
PROCEDURE DestruirTablaIns (VAR T:PTablaIns);
FUNCTION LeerInfoDeTablaIns(T:PTablaIns;Track:TTrackNum):PInfoTrack;

FUNCTION CargarTablaInsGeneral (VAR T:PTablaIns;VAR F:FILE;VAR ID:WORD):BOOLEAN;
FUNCTION SalvarTablaInsGeneral (T:PTablaIns;VAR F:FILE;ID:WORD):BOOLEAN;
FUNCTION TamanyoTablaIns(T:PTablaIns):LONGINT;

{----------------------------------------------------------------------------}
{Tratamiento del patron                                                      }
{----------------------------------------------------------------------------}
PROCEDURE CrearPattern(VAR P:PPattern);
PROCEDURE DestruirPattern(VAR P:PPattern);

PROCEDURE CopiarPattern(F,D:PPattern);
PROCEDURE BorrarPattern(P:PPattern);

FUNCTION LeerTrackDePattern(P:PPattern;I:TTrackNum):PTrack;

FUNCTION CargarPatternGeneral(VAR P:PPattern;VAR F:File;VAR ID:WORD):Boolean;
FUNCTION SalvarPatternGeneral(P:PPattern;VAR F:File;ID:WORD):Boolean;
FUNCTION NumeroDeTracksEnPattern(P:PPattern):TTrackNum;
FUNCTION PatternVacio(P:PPattern):BOOLEAN;
FUNCTION LeerNombreDelPattern(P:PPattern):STRING;
PROCEDURE EscribirNombreDelPattern(P:PPattern;Nombre:String);
FUNCTION TamanyoPattern(P:PPattern):LONGINT;

{----------------------------------------------------------------------------}
{Tratamiento del track                                                       }
{----------------------------------------------------------------------------}
{No hay creacion de tracks ni destruccion, la unidad minima de creaci¢n es el
patron. La unidad minima tratable es el beat con sus campos}
PROCEDURE CrearTrack(T:PTrack);
PROCEDURE CopiarTrack(Fuente,Destino:PTrack);
FUNCTION CargarTrackGeneral(T:PTrack;VAR F:File;VAR ID:WORD):BOOLEAN;
FUNCTION SalvarTrackGeneral(T:PTrack;VAR F:File;ID:WORD):BOOLEAN;

PROCEDURE InsertarPosicionTrack  (T:PTrack;B:TBeatNum);
PROCEDURE BorrarPosicionTrack    (T:PTrack;B:TBeatNum);
PROCEDURE InsertarPosicionesTrack(T:PTrack;B,Num:TBeatNum);
PROCEDURE BorrarPosicionesTrack  (T:PTrack;B,num:TBeatNum);

PROCEDURE InsertarEventoTrack(T:PTrack;Time:TBeatNum;N,V,F,A:BYTE);
PROCEDURE EliminarEventoTrack(T:PTrack;Time:TBeatNum);

FUNCTION TomarEventoPorTiempoTrack(T:PTrack;Time:TBeatNum):PDatosEvento;
FUNCTION EstaEventoTrack(T:PTrack;Time:TBeatNum):BOOLEAN;
FUNCTION TrackVacio(T:PTrack):BOOLEAN;
FUNCTION NumEventosTrack(T:PTrack):Integer;
FUNCTION TamanyoTrack(T:PTrack):LONGINT;

{----------------------------------------------------------------------------}
{Tratamiento de los beats dentro del track                                   }
{----------------------------------------------------------------------------}
FUNCTION LeerBeatTrack     (T:PTrack;Beat:TBeatNum):BeatString;
PROCEDURE EscribirBeatTrack(T:PTrack;Beat:TBeatNum;Nota,V,F,A:Byte);

PROCEDURE EscribirNotaBeat(T:PTrack;Beat:TBeatNum;Nota:byte);
PROCEDURE EscribirVolBeat (T:PTrack;Beat:TBeatNum;Vol :byte);
PROCEDURE EscribirEffBeat (T:PTrack;Beat:TBeatNum;Eff :byte);
PROCEDURE EscribirValBeat (T:PTrack;Beat:TBeatNum;Val :byte);

FUNCTION LeerNotaBeat(T:PTrack;Beat:TBeatNum):Byte;
FUNCTION LeerVolBeat (T:PTrack;Beat:TBeatNum):Byte;
FUNCTION LeerEffBeat (T:PTrack;Beat:TBeatNum):Byte;
FUNCTION LeerValBeat (T:PTrack;Beat:TBeatNum):Byte;

FUNCTION DescripcionDeError:tt98String;
{----------------------------------------------------------------------------}
IMPLEMENTATION
USES
    CONSTS2;
{----------------------------------------------------------------------------}
FUNCTION DescripcionDeError:tt98String;
BEGIN
     DescripcionDeError:=Error;
END;
{----------------------------------------------------------------------------}
{$I CANCION.INC}
{$I SECUEN.INC}
{$I TABLAINS.INC}
{$I PATTERNS.INC}
{$I TRACKS.INC}
{----------------------------------------------------------------------------}
END.
