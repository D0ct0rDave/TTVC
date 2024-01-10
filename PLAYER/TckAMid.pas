unit TckAMid;

interface

uses
    Tipos;

    FUNCTION EscribirFicheroMidi(S:PCancion;VAR F:FILE):BOOLEAN;
    procedure EstablecerFuncionDeConversionTCK2Mid(FuncionDeConversion:pointer);

implementation

uses
    Consts2,inter;
const
     MaxBytesEM = 128;

     COMANDO_NOTEON        = $90;
     COMANDO_NOTEOFF       = $80;

     COMANDO_CONTROLCHANGE  = $B0;

     COMANDO_CAMBIOBANCO0  = $00;
     COMANDO_CAMBIOBANCO32 = $20;
     COMANDO_CAMBIOPROGRAMA= $C0;

type
    TMetaEvent = (METAEVENT_TEXT,METAEVENT_COPYRIGHT,METAEVENT_TRACKNAME,METAEVENT_ENDOFTRACK,
                  METAEVENT_SETTEMPO,METAEVENT_TIMESIGNATURE,METAEVENT_KEYSIGNATURE);

    TEventoMIDI = RECORD
                        Time:LONGINT;
                        D:ARRAY[0..MaxBytesEM-1] OF BYTE;
                        Tam:LONGINT;
                  END;
    PEventoMIDI =^TEventoMIDI;

    TConvertTCK2MidiEffect = PROCEDURE (Eff,Val:Byte;T:TTrackNum;var EvEff:TEventoMidi);

VAR
   TiempoActualAbsoluto:LONGINT;
   BeatActualAbsoluto:LONGINT;

   IndiceSecuencia:longint;
   BeatActual:LONGINT;

   PAct : PPattern;
   TAct : PTrack;

   EndOfModule:BOOLEAN;
   PosTamTrack:LONGINT;    //Posicion donde esta el campo tamanyo del track
   PosFinTrack:LONGINT;    //Posicion donde esta la posicion de fin del track
   LastNoteON : byte;

   InfTrackActual:PInfoTrack;
   ConvertTCK2MidiEffect:TConvertTCK2MidiEffect;
//------------------------------------------------------------------------------
procedure EscribirMetaEvento(me:TMetaEvent;em:PEventoMidi;VAR F:FILE);
var
   aux:byte;
begin
     case me of
          METAEVENT_TEXT:begin
                              aux := $ff;blockwrite(f,aux,1);
                              aux := $01;blockwrite(f,aux,1);
                              blockwrite(f,em.Tam,1);
                              blockwrite(f,em.D,em.Tam);
                         end;
          METAEVENT_COPYRIGHT:begin
                              aux := $ff;blockwrite(f,aux,1);
                              aux := $02;blockwrite(f,aux,1);
                              blockwrite(f,em.Tam,1);
                              blockwrite(f,em.D,em.Tam);
                              end;
          METAEVENT_TRACKNAME:begin
                              aux := $ff;blockwrite(f,aux,1);
                              aux := $03;blockwrite(f,aux,1);
                              blockwrite(f,em.Tam,1);
                              blockwrite(f,em.D,em.Tam);
                              end;
          METAEVENT_ENDOFTRACK:begin
                              aux := $ff;blockwrite(f,aux,1);
                              aux := $2f;blockwrite(f,aux,1);
                              aux := $00;blockwrite(f,aux,1);
                              end;
          METAEVENT_SETTEMPO: begin
                              aux := $ff;blockwrite(f,aux,1);
                              aux := $51;blockwrite(f,aux,1);
                              aux := $03;blockwrite(f,aux,1);
                              blockwrite(f,em.D,3);
                              end;
          METAEVENT_TIMESIGNATURE:begin
                              aux := $ff;blockwrite(f,aux,1);
                              aux := $58;blockwrite(f,aux,1);
                              aux := $04;blockwrite(f,aux,1);
                              blockwrite(f,em.D,4);
                              end;
          METAEVENT_KEYSIGNATURE:begin
                              aux := $ff;blockwrite(f,aux,1);
                              aux := $59;blockwrite(f,aux,1);
                              aux := $02;blockwrite(f,aux,1);
                              blockwrite(f,em.D,2);
                              end;
     end;
end;
//------------------------------------------------------------------------------
FUNCTION TCKTimeAMidiTime(BeatAbsolutoActual:LONGINT;BPMS:longint;VAR UltimoTiempoAbsoluto:LONGINT):LONGINT;
VAR
   Res,FRes,TiempoAbsolutoActual:LONGINT;
BEGIN
     // Transformar el puntero de la cancion en un tiempo absoluto
     TiempoAbsolutoActual := trunc(BeatAbsolutoActual / (BPMS*8));
     // Restar el tiempo absoluto a TiempoAbsolutoActual
     Res := UltimoTiempoAbsoluto - TiempoAbsolutoActual;
     UltimoTiempoAbsoluto := TiempoAbsolutoActual;

     // Transformar el resultado en un valor tipo Variable Lenght Quantity
     {FRes := Res and $7f;
     while ((Res shr 7) >0) do
     begin
          Res  := Res shr 7;
          FRes := FRes shl 8;
          FRes := FRes or $00000080;
          FRes := FRes OR (Res AND $0000007f);
     end;
     }
     FRes := 0;
     repeat
           FRes := FRes OR (Res and $7f);
           Res :=  (Res shr 7);

           if (Res >0) THEN
           BEGIN
                FRes := FRes SHL 8;
                FRes := (FRes OR 80);
           END
     until (Res=0);

     TCKTimeAMidiTime := FRes;
END;
//------------------------------------------------------------------------------
FUNCTION EventoTCK_A_EventoMIDI(EvT:PDatosEvento;T:TTrackNum;VAR EvNota,EvEff:TEventoMIDI):BOOLEAN;
// Retornara CIERTO cuando el beat contenga informacion util
BEGIN
     // Inicializar las estructuras
     EvNota.Tam:=0;
     EvEff.Tam :=0;

     // Establecer el contenido de las estructura4s
     if ((EvT^.Nota<>255) and (EvT^.Eff<>0)) then
     begin
          //----------------------------------------------------------
          if (EvT^.Nota<>255) then
          begin
               if (LastNoteON<>255) then
               begin
                    EvNota.D[0] := COMANDO_NOTEOFF OR InfTrackActual.Canal;
                    EvNota.D[1] := EvT^.Nota;
                    EvNota.D[2] := 0;

                    EvNota.D[3] := COMANDO_NOTEON OR InfTrackActual.Canal;
                    EvNota.D[4] := EvT^.Nota;
                    EvNota.D[5] := EvT^.Vol;
               end
               else
               begin
                    EvNota.D[0] := COMANDO_NOTEON OR InfTrackActual.Canal;
                    EvNota.D[1] := EvT^.Nota;
                    EvNota.D[2] := EvT^.Vol;
               end;

               LastNoteON := EvT^.Nota;
          end;
          //----------------------------------------------------------
          if ((EvT^.Eff<>0) and (ADDR(ConvertTCK2MidiEffect)<>NIL)) then
             ConvertTCK2MidiEffect(EvT^.Eff,EvT^.Val,T,EvEff);

          EventoTCK_A_EventoMIDI:=true;
     end
     else
         EventoTCK_A_EventoMIDI:=false;
END;
//------------------------------------------------------------------------------
FUNCTION PistaVacia(S:PCancion;T:TTrackNum):BOOLEAN;
VAR
   Max,I:INTEGER;
   VAcia:BOOLEAN;
BEGIN
     // No usar los patrones del vector de patrones para esto porque
     // hay veces en las que un patron puede no estar dentro de la secuencia
     // ejemplo: cuando tenemos secuencia 1 2 y cambiamos a 2 2: el patron 1 sigue estando

     Vacia:=TRUE;
     Max:=NumPatronesEnSecuencia(S^.Seq);
     I:=0;
     WHILE (I<Max) AND (Vacia) DO
     BEGIN
          Vacia:=TrackVacio(@(PPattern(ObtenerPatronDeSecuencia(S,I))^.Tracks[T]));
          INC(I);
     END;

     PistaVacia := Vacia;
END;
//------------------------------------------------------------------------------
PROCEDURE InicializarPunteros(C:PCancion;T:TTrackNum);
BEGIN
     IndiceSecuencia   :=0;
     BeatActual        :=0;
     BeatActualAbsoluto:=0;

     TiempoActualAbsoluto:=0;

     PAct := ObtenerPatronDeSecuencia(C,IndiceSecuencia);
     TAct := LeerTrackDePattern(PAct,T);
END;

PROCEDURE IncrementarPunteros(S:PCancion);
// Actualiza los punteros teniendo en cuenta la posible existencia de un
// BreakPattern en la ultima linea procesada.
VAR
   i:integer;
   Pattern:PPattern;
BEGIN
     // Mirar si en la linea de beats actual existe un efecto tipo BreakPattern
     Pattern:=ObtenerPatronDeSecuencia(S,IndiceSecuencia);

     IF (Pattern<>NIL) THEN
     BEGIN
          i:=0;

          IF (BeatActual<MaxBeats-1) THEN
          BEGIN
               // Todos los Tracks examinados, o encontrado PatternBreak o EndOfMod
               WHILE (I<MaxTracks) AND
                     (LeerEffBeat(@(Pattern^.Tracks[i]),BeatActual)<>3) AND
                     (LeerEffBeat(@(Pattern^.Tracks[i]),BeatActual)<>2) DO INC(i);

               IF (i<MaxTracks) then
               BEGIN
                    EndOfModule:=(LeerEffBeat(@(Pattern^.Tracks[i]),BeatActual) = 2);

                    Inc(IndiceSecuencia);
                    BeatActual :=0 ;

                    IF (IndiceSecuencia<NumPatronesEnSecuencia(S^.Seq)) THEN
                    BEGIN
                         PAct := ObtenerPatronDeSecuencia(S,IndiceSecuencia);
                         TAct := LeerTrackDePattern(PAct,i);
                    END
                    ELSE
                        EndOfModule := TRUE;
               END
               ELSE
                   Inc(BeatActual);
          END
          ELSE
          BEGIN
               Inc(IndiceSecuencia);
               BeatActual :=0 ;

               IF (IndiceSecuencia<NumPatronesEnSecuencia(S^.Seq)) THEN
               BEGIN
                    PAct := ObtenerPatronDeSecuencia(S,IndiceSecuencia);
                    TAct := LeerTrackDePattern(PAct,i);
               END
               ELSE
                   EndOfModule := TRUE;
          END;

          INC(BeatActualAbsoluto);
     END;
END;
//------------------------------------------------------------------------------
PROCEDURE EscribirDeltaTime(DeltaTime:LONGINT;VAR F:FILE);
BEGIN
     REPEAT
           blockwrite(F,DeltaTime,1);
           DeltaTime := DeltaTime shr 8;
     UNTIL (DeltaTime>0);
END;

PROCEDURE EscribirEventoMIDI(E:PEventoMIDI;VAR F:FILE);
VAR
   I:BYTE;
BEGIN
     EscribirDeltaTime(E^.Time,F);
     FOR I:=0 TO E.Tam-1 DO BLOCKWRITE(F,E^.D[I],1);
END;

PROCEDURE EscribirEventoBeat(Time:Longint;ENota,EEff:PEventoMidi;VAR F:FILE);
BEGIN
     // Siempre se escribe primero el efecto y despues la nota
     IF (EEff^.Tam >0) THEN
     BEGIN
          EEff.Time :=Time;
          EscribirEventoMIDI(EEff,F);

          IF (ENota.Tam>0) THEN
          BEGIN
               ENota.Time := 0;
               EscribirEventoMIDI(ENota,F);
          END;
     END
     ELSE
     BEGIN
          ENota.Time := Time;
          EscribirEventoMIDI(ENota,F);
     END;
END;
//------------------------------------------------------------------------------
procedure EscribirCabeceraTrack(S:PCancion;T:TTrackNum;VAR F:FILE);
const
     id:array[0..3] of char = ('M','T','r','k');
var
   Tam:longint;
   EvM:TEventoMidi;

BEGIN
      // A parte de la informacion de cabecera, tambien se escribe la informacion
     // sobre el instrumento que utilizara este track y el banco que utilizara ...
     BlockWrite(F,ID,4);
     PosTamTrack := FilePos(F);

     // Dejar hueco para el tamanyo del track
     BlockWrite(F,Tam,4);

     //Escribir eventos iniciales

     // Nombre del track
     EvM.Tam := length(InfTrackActual^.Nombre);
     move(InfTrackActual^.Nombre[1],EvM.D,EvM.Tam);
     EscribirMetaEvento(METAEVENT_TRACKNAME,@EvM,F);

     // Bancos:
     IF (InfTrackActual^.Ctrl0) THEN
     BEGIN
          EvM.Time := 0;
          EvM.D[0] := COMANDO_CONTROLCHANGE OR InfTrackActual^.Canal;
          EvM.D[1] := COMANDO_CAMBIOBANCO0;
          EvM.D[2] := InfTrackActual^.Banco;
          EvM.Tam := 3;
          EscribirEventoMidi(@EvM,F);
     END;

     IF (InfTrackActual^.Ctrl32) THEN
     BEGIN
          EvM.Time := 0;
          EvM.D[0] := COMANDO_CONTROLCHANGE OR InfTrackActual^.Canal;
          EvM.D[1] := COMANDO_CAMBIOBANCO32;
          EvM.D[2] := InfTrackActual^.Banco;
          EvM.Tam := 3;
          EscribirEventoMidi(@EvM,F);
     END;

     //Activar el programa:
     EvM.Time := 0;
     EvM.D[0] := COMANDO_CAMBIOPROGRAMA OR InfTrackActual^.Canal;
     EvM.D[1] := InfTrackActual^.Inst;
     EvM.D[2] := 0;
     EvM.Tam := 3;
     EscribirEventoMidi(@EvM,F);
END;
//------------------------------------------------------------------------------
PROCEDURE InicializarVariablesTrack(S:PCancion;T:TTrackNum);
BEGIN
     LastNoteON:=255;
     InfTrackActual:=LeerInfoDeTablaIns(@(S^.TIns),T);
END;
//------------------------------------------------------------------------------
PROCEDURE EscribirTrack(S:PCancion;T:TTrackNum;VAR F:FILE);
VAR
   Ev:PDatosEvento;
   EvNota,EvEff:TEventoMidi;
   Time:LONGINT;
BEGIN
     InicializarPunteros(S,T);
     InicializarVariablesTrack(S,T);

     // Escribir los datos iniciales de este track
     EscribirCabeceraTrack(S,T,F);

     WHILE (NOT EndOfModule) DO
     BEGIN
          Ev :=TomarEventoPorTiempoTrack(TAct,BeatActual);

          IF (EventoTCK_A_EventoMIDI(Ev,T,EvNota,EvEff)) THEN
          BEGIN
               // Esto indica que el Evento MIDI existe
               Time := TCKTimeAMidiTime(BeatActualAbsoluto,S^.BPMS,TiempoActualAbsoluto);
               EscribirEventoBeat(Time,@EvNota,@EvEff,F);
          END;

          IncrementarPunteros(S);
     END;

     EscribirMetaEvento(METAEVENT_ENDOFTRACK,nil,F);

     PosFinTrack := FilePos(F);
     Seek(F,PosTamTrack);
     PosTamTrack := PosFinTrack-PosTamTrack;

     // Escritura del tamanyo del track
     BLOCKWRITE(F,PosTamTrack,4);

     // Posicionar el puntero al final del track
     Seek(F,PosFinTrack);
END;
//------------------------------------------------------------------------------
PROCEDURE EscribirCabeceraMIDI(S:PCancion;VAR F:FILE);
CONST
     id :array[0..3] of char = ('M','T','h','d');
VAR
   lon:longint;
   format:word;
   ntrks:word;
   division:word;
BEGIN
     lon:=6;                     // longitud del bloque cabecera
     format := 1;                // formato: tipo 1, #tracks variable
     division := 0;              // nºtics por beat

     blockwrite(f,id,4);         // Escritura del identificador de bloque
     blockwrite(f,lon,sizeof(longint));
     blockwrite(f,format,sizeof(word));
     blockwrite(f,ntrks,sizeof(word));
     blockwrite(f,division,sizeof(word));
END;
//------------------------------------------------------------------------------
procedure EscribirTrack0(S:PCancion;VAR F:FILE);
const
     id:array[0..3] of char = ('M','T','r','k');
     LongMasterTrack:longint = 16;      // La longitud del MasterTrack es fija
                                        // porque siempre contiene la misma info
var
   EvM:TEventoMidi;
begin
     // Este track se utiliza como MasterTrack, es decir, el que contiene informacion
     // importante acerca de la velocidad de la cancion, Key Signature ...
     BlockWrite(F,ID,4);

     // Dejar hueco para el tamanyo del track
     BlockWrite(F,LongMasterTrack,4);

     // Set Key Signature : 4/4 : 8 midi clocks / quarter note :
     EvM.Tam := 4; EvM.D[0]:=04;EvM.D[1]:=2;EvM.D[1]:=8;EvM.D[2]:=8;EvM.D[3]:=8;
     EscribirMetaEvento(METAEVENT_TIMESIGNATURE,@EvM,F);

     // SetTempo
     EvM.Tam := 3; longint((@EvM.D)^):=longint(trunc(60*1000000/S^.BPMS));
     EscribirMetaEvento(METAEVENT_SETTEMPO,@EvM,F);

     // EndOfTrack
     EscribirMetaEvento(METAEVENT_ENDOFTRACK,nil,F);
end;
//------------------------------------------------------------------------------
FUNCTION EscribirFicheroMidi(S:PCancion;VAR F:FILE):BOOLEAN;
VAR
   I:BYTE;
BEGIN
     EscribirCabeceraMIDI(S,F);
     EscribirTrack0(S,F);

     for i:=0 to MaxTracks-1 do
        IF (NOT PistaVacia(S,i)) THEN EscribirTrack(S,i,F);

     EscribirFicheroMidi := true;
END;
//------------------------------------------------------------------------------
procedure EstablecerFuncionDeConversionTCK2Mid(FuncionDeConversion:pointer);
begin
     IF (FuncionDeConversion<>NIL) THEN ADDR(ConvertTCK2MidiEffect):= FuncionDeConversion;
end;
//------------------------------------------------------------------------------
END.

