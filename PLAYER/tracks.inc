{----------------------------------------------------------------------------}
PROCEDURE BorrarTrack(T:PTrack);
BEGIN
     FillChar(T^,Sizeof(TTrack),#0);
END;
{----------------------------------------------------------------------------}
PROCEDURE CrearTrack(T:PTrack);
BEGIN     // El puntero es un track creado que es "estático" (no mem.din.)
     BorrarTrack(T);
END;
{----------------------------------------------------------------------------}
PROCEDURE CopiarTrack(Fuente,Destino:PTrack);
VAR
   I:TBeatNum;
BEGIN
     {Tomamos como fuente este mismo Track}
     FOR I:=0 TO MaxBeats-1 DO Destino^.PE[I]:=Fuente^.PE[I];
     Destino^.NumEventos := Fuente^.NumEventos;
END;
{----------------------------------------------------------------------------}
FUNCTION CargarTrackGeneral(T:PTrack;VAR F:File;VAR ID:WORD):BOOLEAN;
CONST
     TRACK_ID:ARRAY[0..3] OF CHAR =('T','R','A','K');
VAR
   TRK:ARRAY[0..3] OF CHAR;
   Ev:TDatosEvento;
   NumEvents,I:TBeatNum;
   Cantidad:LONGINT;
   Time:TBeatNum;

BEGIN
     {Lectura del identificador}
     BLOCKREAD(F,TRK,4);
     IF (IORESULT<>0) THEN BEGIN CargarTrackGeneral:=False; EXIT;END;
     IF (TRK<>TRACK_ID) THEN BEGIN CargarTrackGeneral:=False; EXIT;END;

     {Lectura del identificador de estructura}
     BLOCKREAD(F,ID,SizeOF(ID));
     IF (IORESULT<>0) THEN BEGIN CargarTrackGeneral:=False; EXIT;END;

     {Lectura de la cantidad de bytes en el track}
     BLOCKREAD(F,Cantidad,SizeOF(Longint));
     IF (IORESULT<>0) THEN BEGIN CargarTrackGeneral:=False; EXIT;END;

     {Lectura de la cantidad de eventos que hay en el track}
     BLOCKREAD(F,NumEvents,SizeOF(TBeatNum));
     IF (IORESULT<>0) THEN BEGIN CargarTrackGeneral:=False; EXIT;END;

     {Lectura del contenido del track}
     FOR I:=0 TO NumEvents-1 DO
     BEGIN
          BLOCKREAD(F,Time,SizeOF(TBeatNum));
          BLOCKREAD(F,Ev,SizeOf(TDatosEvento));
          IF (IORESULT<>0) THEN BEGIN CargarTrackGeneral:=False; EXIT;END;

          {Controlar si esta lleno el campo nota}
          IF (Ev.Nota<>255) OR (Ev.Eff<>0) THEN InsertarEventoTrack(T,Time,Ev.Nota+1,Ev.Vol,Ev.Eff,Ev.Val)
     END;

     CargarTrackGeneral:=(TamanyoTrack(T)=Cantidad);
END;
{----------------------------------------------------------------------------}
FUNCTION SalvarTrackGeneral(T:PTrack;VAR F:File;ID:WORD):BOOLEAN;
CONST
     TRACK_ID:ARRAY[0..3] OF CHAR =('T','R','A','K');
VAR
   I:TBeatNum;
   Cantidad:LONGINT;
BEGIN
     {Escritura del identificador}
     BLOCKWRITE(F,TRACK_ID,4);
     IF (IORESULT<>0) THEN BEGIN SalvarTrackGeneral:=False; EXIT;END;

     {Escritura del identificador de estructura}
     BLOCKWRITE(F,ID,SizeOf(ID));
     IF (IORESULT<>0) THEN BEGIN SalvarTrackGeneral:=False; EXIT;END;

     {Escritura de la cantidad de bytes en el track}
     Cantidad:=TamanyoTrack(T);
     BLOCKWRITE(F,Cantidad,SizeOf(LONGINT));
     IF (IORESULT<>0) THEN BEGIN SalvarTrackGeneral:=False; EXIT;END;

     {Escritura en numero de eventos}
     BLOCKWRITE(F,T^.NumEventos,SizeOf(TBeatNum));
     IF (IORESULT<>0) THEN BEGIN SalvarTrackGeneral:=False; EXIT;END;

     {Escritura del contenido del track}
     FOR I:=0 TO MaxBeats-1 DO
     BEGIN
          IF (EstaEventoTrack(T,I)) THEN
          BEGIN
               BLOCKWRITE(F,I,SizeOF(TBeatNum)); {Escribimos el tiempo de evento}

               Dec(T^.PE[I].Nota);
               BLOCKWRITE(F,T^.PE[I],SizeOf(TDatosEvento));
               Inc(T^.PE[I].Nota);

               IF (IORESULT<>0) THEN BEGIN SalvarTrackGeneral:=False; EXIT;END;
          END;
     END;

     SalvarTrackGeneral:=True;
END;
{----------------------------------------------------------------------------}
PROCEDURE InsertarPosicionTrack(T:PTrack;B:TBeatNum);
VAR
   I:TBeatNum;
BEGIN
     IF EstaEventoTrack(T,MaxBeats-1) THEN {Lo perderemos al insertar} Dec(T^.NumEventos);

     FOR I:=MaxBeats-1 DOWNTO B+1 DO BEGIN T^.PE[I]:=T^.PE[I-1];{Inc(PE[I].Time);}END;
     FillChar(T^.PE[B],SizeOf(TDatosEvento),#0);T^.PE[B].Nota:=0;
END;
{----------------------------------------------------------------------------}
PROCEDURE BorrarPosicionTrack(T:PTrack;B:TBeatNum);
VAR
   I:BYTE;
BEGIN
     IF EstaEventoTrack(T,B) THEN {Lo perderemos al borrar} Dec(T^.NumEventos);

     FOR I:=B TO MaxBeats-2 DO BEGIN T^.PE[I]:=T^.PE[I+1];{Dec(PE[I].Time);}END;
     B:=MaxBeats-1;FillChar(T^.PE[B],SizeOf(TDatosEvento),#0);T^.PE[B].Nota:=0;
END;
{----------------------------------------------------------------------------}
PROCEDURE InsertarPosicionesTrack(T:PTrack;B,Num:TBeatNum);
BEGIN
     {Abrir hueco}
     {Limpiar posiciones insertadas}
END;
{----------------------------------------------------------------------------}
PROCEDURE BorrarPosicionesTrack(T:PTrack;B,num:TBeatNum);
BEGIN
     {Contraer pista}
     {Limpiar nuevas posiciones}
END;
{----------------------------------------------------------------------------}
PROCEDURE InsertarEventoTrack(T:PTrack;Time:TBeatNum;N,V,F,A:BYTE);
BEGIN
     IF (Time<MaxBeats) THEN
     BEGIN
          IF (T^.PE[Time].Nota=0) AND (T^.PE[Time].Eff=0) THEN Inc(T^.NumEventos);

          WITH T^.PE[Time] DO
          BEGIN
               Nota := N; {Rango de entrada de las notas: 1..96}
               Vol  := V;
               Eff  := F;
               Val  := A;
          END;
     END;
END;
{----------------------------------------------------------------------------}
PROCEDURE EliminarEventoTrack(T:PTrack;Time:TBeatNum);
BEGIN
     IF (T^.PE[Time].Nota<>0) OR (T^.PE[Time].Eff<>0) THEN
     BEGIN
          FILLCHAR(T^.PE[Time],SizeOf(TDatosEvento),#0);
          Dec(T^.NumEventos);
     END;
END;
{----------------------------------------------------------------------------}
FUNCTION TomarEventoPorTiempoTrack(T:PTrack;Time:TBeatNum):PDatosEvento;
BEGIN
     IF (Time < MaxBeats) THEN
     BEGIN
          IF (T^.PE[Time].Nota<>0) OR (T^.PE[Time].Eff<>0) THEN
              TomarEventoPorTiempoTrack:=@(T^.PE[Time])
          ELSE
              TomarEventoPorTiempoTrack:=NIL;
     END
     ELSE
        TomarEventoPorTiempoTrack:=NIL;
END;
{----------------------------------------------------------------------------}
FUNCTION EstaEventoTrack(T:PTrack;Time:TBeatNum):BOOLEAN;
BEGIN
     IF (Time < MaxBeats) THEN
        EstaEventoTrack:=(T^.PE[Time].Nota<>0) OR (T^.PE[Time].Eff<>0)
     ELSE
         EstaEventoTrack:=FALSE;
END;
{----------------------------------------------------------------------------}
FUNCTION TrackVacio(T:PTrack):BOOLEAN;
BEGIN
     IF (T=NIL) THEN BEGIN TrackVacio:=TRUE;EXIT;END;
     TrackVacio:=(NumEventosTrack(T)=0);
END;
{----------------------------------------------------------------------------}
FUNCTION NumEventosTrack(T:PTrack):Integer;
VAR
   I,R:INTEGER;
BEGIN
     R:=0;
     FOR I:=0 TO MaxBeats-1 DO
         IF (LONGINT(T^.PE[I])<>0) THEN INC(R);

     NumEventosTrack:=R;
END;
{----------------------------------------------------------------------------}
FUNCTION TamanyoTrack(T:PTrack):LONGINT;
BEGIN
     TamanyoTrack:=NumEventosTrack(T)*(SizeOF(TDatosEvento)+SizeOF(TBeatNum))+SizeOf(WORD)+10;
END;
{----------------------------------------------------------------------------}
{Tratamiento de los beats dentro del track                                   }
{----------------------------------------------------------------------------}
FUNCTION LeerBeatTrack(T:PTrack;Beat:TBeatNum):BeatString;
VAR
   D:PDatosEvento;
   S,Aux:BeatString;
begin
     D:=TomarEventoPorTiempoTrack(T,Beat);

     {Funcion muy importante para poder representar los datos en pantalla}
     IF (D = NIL) then
     {Pintar Vacio}
          AUX:='___ ···          ···'
     ELSE
     {Pintar con Algo}
     BEGIN
          IF (D^.Nota=0) THEN
             S:='___'
          ELSE
              BEGIN
                   STR((D^.NOTA-1) DIV 12:1,S);
                   CASE ((D^.NOTA-1) MOD 12) OF
                        00:S:='C '+S;
                        01:S:='C#'+S;
                        02:S:='D '+S;
                        03:S:='D#'+S;
                        04:S:='E '+S;
                        05:S:='F '+S;
                        06:S:='F#'+S;
                        07:S:='G '+S;
                        08:S:='G#'+S;
                        09:S:='A '+S;
                        10:S:='A#'+S;
                        11:S:='B '+S;
                   END;
          END;
          AUX:=S+' ';

          {OBTENCION DEL SEGUNDO CAMPO}
          IF (D^.Nota=0) THEN
             S:='···'
          ELSE
             STR(D^.VOL:3,S);

          AUX:=AUX+S+' ';

          {OBTENCION DEL TERCER CAMPO}
          IF (D^.Eff<MaxEfectosPropios) THEN
          BEGIN
               S:=ListaDeEfectos[D^.Eff];

               IF (S='        ') AND (D^.Eff>0) THEN
               BEGIN
                    Str(D^.Eff:4,S);
                    S:='Eff:'+S;
               END;
          END
          ELSE
              s:='-';

          AUX:=AUX+S+' ';

          {OBTENCION DEL CUARTO CAMPO}
          IF (D^.Eff=0) THEN
             S:='···'
          ELSE
             STR(D^.Val:3,S);

          AUX:=AUX+S;
     END;
     LeerBeatTrack:=AUX;
END;
{----------------------------------------------------------------------------}
PROCEDURE EscribirBeatTrack(T:PTrack;Beat:TBeatNum;Nota,V,F,A:Byte);
BEGIN
     InsertarEventoTrack(T,Beat,Nota+1,V,F,A);
END;
{----------------------------------------------------------------------------}
PROCEDURE EscribirNotaBeat(T:PTrack;Beat:TBeatNum;Nota:byte);
var
   E:PDatosEvento;
begin
     E:=TomarEventoPorTiempoTrack(T,Beat);
     if (E<>nil) then E^.Nota:=Nota+1 ELSE InsertarEventoTrack(T,Beat,Nota+1,0,0,0);
end;
{----------------------------------------------------------------------------}
PROCEDURE EscribirVolBeat(T:PTrack;Beat:TBeatNum;Vol :byte);
var
   E:PDatosEvento;
BEGIN
     E:=TomarEventoPorTiempoTrack(T,Beat);
     if (E<>nil) then E^.Vol:=Vol  ELSE InsertarEventoTrack(T,Beat,0,Vol,0,0);
END;
{----------------------------------------------------------------------------}
PROCEDURE EscribirEffBeat(T:PTrack;Beat:TBeatNum;Eff :byte);
var
   E:PDatosEvento;
BEGIN
     E:=TomarEventoPorTiempoTrack(T,Beat);
     if (E<>nil) then E^.Eff:=Eff  ELSE InsertarEventoTrack(T,Beat,0,0,Eff,0);
END;
{----------------------------------------------------------------------------}
PROCEDURE EscribirValBeat(T:PTrack;Beat:TBeatNum;Val :byte);
var
   E:PDatosEvento;
BEGIN
     E:=TomarEventoPorTiempoTrack(T,Beat);
     if (E<>nil) then E^.Val:=Val  ELSE InsertarEventoTrack(T,Beat,0,0,0,Val);
END;
{----------------------------------------------------------------------------}
FUNCTION LeerNotaBeat(T:PTrack;Beat:TBeatNum):Byte;
var
   E:PDatosEvento;
BEGIN
     E:=TomarEventoPorTiempoTrack(T,Beat);
     if (E<>nil) then LeerNotaBeat:=E^.Nota-1 else LeerNotaBeat:=255;
END;
{----------------------------------------------------------------------------}
FUNCTION LeerVolBeat(T:PTrack;Beat:TBeatNum):Byte;
var
   E:PDatosEvento;
BEGIN
     E:=TomarEventoPorTiempoTrack(T,Beat);
     if (E<>nil) then LeerVolBeat:=E^.Vol else LeerVolBeat:=0;
END;
{----------------------------------------------------------------------------}
FUNCTION LeerEffBeat(T:PTrack;Beat:TBeatNum):Byte;
var
   E:PDatosEvento;
BEGIN
     E:=TomarEventoPorTiempoTrack(T,Beat);
     if (E<>nil) then LeerEffBeat:=E^.Eff else LeerEffBeat:=0;
END;
{----------------------------------------------------------------------------}
FUNCTION LeerValBeat(T:PTrack;Beat:TBeatNum):Byte;
var
   E:PDatosEvento;
BEGIN
     E:=TomarEventoPorTiempoTrack(T,Beat);
     if (E<>nil) then LeerValBeat:=E^.Val else LeerValBeat:=0;
END;
{----------------------------------------------------------------------------}