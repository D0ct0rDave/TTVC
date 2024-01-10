UNIT Play;

INTERFACE

USES
    Windows,  {Dejarlo aqui, puesto que se redeclarara el tipo TPattern y PPattern}
    Tipos;

{$I cab.inc}

IMPLEMENTATION

USES
    Inter,

    OMidiOut,
    Consts2,
    mmSystem;

{-----------------------------}

TYPE
    PBufferSysEx = ^TBufferSysEx;
    TBufferSysEx = RECORD
                        Data:ARRAY[0..31] OF BYTE;
                        Num:Byte;
                  END;

    TEstadoTrack = (ET_Activo,
                    ET_Inactivar,
                    ET_Inactivo);

    InfoEstadoTrack = RECORD
                            PitchLo :byte; {Estado inicial a 0 }
                            PitchHi :byte; {Estado inicial a 64}

                            {Los note offs, se deben hacer con estos
                            parametros, los note on, con los que hayan
                            en el puntero I}

                            Activo:BOOLEAN; {Beat en uso}

                            Canal,
                            Disp,
                            Nota :Byte;

                            I:PInfoTrack;  {Puntero a estructura en TablaIns}
                      END;

    TEstadoReproduccion = (ER_NotPlaying,
                           ER_PlayTrack,
                           ER_PlayPattern,
                           ER_PlaySong,
                           ER_PlayBeat,
                           ER_PlayLineaBeats);

    TConvertTCK2MidiEffect = PROCEDURE (Eff,Val:Byte;T:TTrackNum;EvEff:PBufferSysEx);

{-----------------------------}
VAR
   ConvertTCK2MidiEffect : TConvertTCK2MidiEffect;
   
   {Estado de reproduccion}
   LoopActivado : BOOLEAN;

   BeatsPerMinute,                     {BeatsPerMinute, no mantiene valor }
   MaxBPMS,
   MinBPMS,
   BPMS          :WORD;     {BPMS mantiene valor desde inicio reproduccion}


   EstadoReproduccion : TEstadoReproduccion;


   NumSalidasMidi:INTEGER;

   SalidasMIDI:Array[0..MaxDispositivosMidiOut-1] OF TObjetoMidiOut;
   // TablasEfectos:Array[0..MaxDispositivosMidiOut-1] OF TEfectosMidi;

   LineaBeats : ARRAY [0..MaxTracks-1] OF InfoEstadoTrack;
   EstadoTracks:ARRAY [0..MaxTracks-1] OF TEstadoTrack;

   ThreadPlayer   : THandle;
   ThreadPlayerID : DWORD;
   ThreadPlayerPar: DWORD;
{-----------------------------}
{Datos locales necesarios en las rutinas mas utilizadas}
VAR
   D:PDatosEvento;
   BufferCmds:TBufferSysEx;
{-----------------------------}
VAR
   NumTrackActual    : TTrackNum;
   NumBeatActual     : TBeatNum;
   NumPatternActual  : TPatternNum;
   NumIndexSActual   : TIndexSec;

   TrackActual       : PTrack;
   PatternActual     : PPattern;
   CancionActual     : PCancion;
   TablaInstrumentos : PTablaIns;
   InfoInstActual    : PInfoTrack;

{-----------------------------}
VAR
    BTP:boolean;               // Break the Pattern
    TC:TTimeCaps;
    PeriodoResolucion  :DWORD;

    TimerID:Word;
    TemporizadorActivo:Boolean;

    TiempoInicio       : Integer;
    TiempoTranscurrido : Real;
    TiempoReal         : Integer;
    MR                 : Integer;     {Milisegundos Restantes}
    MEL1               : Single;
    MEL2               : Integer;     {Milisegundos entre lineas}

procedure IniciaReproduccion(Tipo:INTEGER);Forward;
procedure PararReproduccion;Forward;
//----------------------------------------------------------------------------
PROCEDURE Delay(Tiempo:WORD);
VAR
   Comienzo:DWORD;
BEGIN
     Comienzo:=GetTickCount;
     WHILE (GetTickCount-Comienzo<Tiempo) DO; {Busy Wait}
END;
//----------------------------------------------------------------------------
PROCEDURE ActivarPropiedadesMIDI(Info:PInfoTrack);
BEGIN
     IF (Info^.Inst<255) THEN
     BEGIN
          {Activar el Banco}
          IF (Info^.Ctrl0) THEN
             SalidasMidi[Info^.Disp].Comando($B0 OR Info^.Canal,$00,Info^.Banco);

          IF (Info^.Ctrl32) THEN {¨32 en decimal o hexadecimal?}
             SalidasMidi[Info^.Disp].Comando($B0 OR Info^.Canal,$32,Info^.Banco);

          {Activar el programa}
          SalidasMidi[Info^.Disp].Comando($C0 OR Info^.Canal,Info^.Inst,0);
     END;
END;
{----------------------------------------------------------------------------}
PROCEDURE InicializarSistemaReproduccion(T:PTablains);
VAR
   J:INTEGER;
BEGIN
     IF (EstadoReproduccion = ER_NotPlaying) THEN
     BEGIN
          {Volver a establecer los instrumentos de cada Track}
          FOR J:=0 TO MaxTracks-1 DO ActivarPropiedadesMIDI(@(T^.T[j]));
     END;
END;
{----------------------------------------------------------------------------}
PROCEDURE ResetSistemaMIDI;
VAR
   j,i:byte;
BEGIN
     IF (EstadoReproduccion = ER_NotPlaying) THEN
     BEGIN
          {-----------------------------}
          {Inicializar la linea de beats}
          {-----------------------------}

          FillChar(LineaBeats,SizeOF(LineaBeats),#0);
          FOR j:=0 TO MaxTracks-1 DO LineaBeats[j].PitchHi := 64;

          {Para cada uno de los dispositivos}
          FOR j:=0 TO NumSalidasMidi-1 DO
          BEGIN
               {Para cada uno de los canales}
               FOR i:=0 TO 15 DO
               BEGIN
                    {All sounds off}
                    SalidasMIDI[j].Comando($B0 OR i,$78,0);

                    {Reset all controllers}
                    SalidasMIDI[j].Comando($B0 OR i,$79,0);
               END;
          END;
     END;
END;
{----------------------------------------------------------------------------}
PROCEDURE StopReproduccion;
BEGIN
     PararReproduccion;
END;
{----------------------------------------------------------------------------}
PROCEDURE PlayNoteOn(Dispositivo,Canal,Note,Velocity:BYTE);
BEGIN
     SalidasMidi[Dispositivo].Comando($90 OR Canal,Note,Velocity);
END;
{----------------------------------------------------------------------------}
PROCEDURE PlayNoteOff(Dispositivo,Canal,Note:BYTE);
BEGIN
     SalidasMidi[Dispositivo].Comando($90 OR Canal,Note,0);
END;
{----------------------------------------------------------------------------}
PROCEDURE PlayBeat(T:PTrack;Info:PInfoTrack;Beat:TBeatNum;Track:TTrackNum);
VAR
   i:integer;

BEGIN
     {Averiguar a que instrumento enviar la informacion}
     D:=TomarEventoPorTiempoTrack(T,Beat);

     IF (D<>nil) THEN
     BEGIN
          {Mandamos primero el efecto, luego la nota}
          IF (D^.Eff>0) THEN
          BEGIN
               IF (D^.Eff>1) AND (D^.Eff<$09) THEN
               BEGIN
                    {Comandos que no modifican el sistema MIDI}

                    CASE D^.Eff OF
                        $02:{FDM}
                            BEGIN
                                 StopReproduccion;
                            END;
                        $03:{Break the Pattern}
                            BEGIN
                                 BTP:=True;
                            END;
                        $04:{Inc Tempo}
                            BEGIN
                                 BeatsPerMinute:=BeatsPerMinute + D^.Val;
                                 IF BeatsPerMinute>MaxBPMS THEN BeatsPerMinute:=MaxBPMS;
                            END;
                        $05:{Dec Tempo}
                            BEGIN
                                 BeatsPerMinute:=BeatsPerMinute - D^.Val;
                                 IF BeatsPerMinute<MinBPMS THEN BeatsPerMinute:=MinBPMS;
                            END;
                        $06:{bpms * 2}
                            BEGIN
                                 BeatsPerMinute:=BeatsPerMinute shl 1;
                                 IF BeatsPerMinute>MaxBPMS THEN BeatsPerMinute:=MaxBPMS;
                            END;
                        $07:{bpms / 2}
                            BEGIN
                                 BeatsPerMinute:=BeatsPerMinute shr 1;
                                 IF BeatsPerMinute<MinBPMS THEN BeatsPerMinute:=MinBPMS;
                            END;
                    END;
               END
               ELSE
               BEGIN
                    {Comandos que modifican estructuras, y
                     el sistema MIDI}

                    CASE D^.Eff OF
                        $01:{NoteOff}
                            BEGIN
                                 {Procesar informacion}
                                 if (addr(ConvertTCK2MidiEffect)<>nil) then
                                    ConvertTCK2MidiEffect(D^.Eff,LineaBeats[Track].Nota,Track,@BufferCmds);

                                 {Actualizar informacion}
                                 LineaBeats[Track].Disp  :=0;
                                 LineaBeats[Track].Canal :=0;
                                 LineaBeats[Track].Nota  :=0;
                                 LineaBeats[Track].Activo:=False;
                            END;

                        $12:{Efecto Pitch Bend Hi}
                            BEGIN
                                 {Procesar informacion}
                                 if (addr(ConvertTCK2MidiEffect)<>nil) then
                                    ConvertTCK2MidiEffect(D^.Eff,D^.Val,Track,@BufferCmds);

                                 LineaBeats[Track].PitchHI := D^.Val;
                                 BufferCmds.Data[1] := LineaBeats[Track].PitchLO;
                            END;

                        $13:{Efecto Pitch Bend Hi}
                            BEGIN
                                 {Procesar informacion}
                                 if (addr(ConvertTCK2MidiEffect)<>nil) then
                                    ConvertTCK2MidiEffect(D^.Eff,D^.Val,Track,@BufferCmds);

                                 LineaBeats[Track].PitchLO := D^.Val;
                                 BufferCmds.Data[1] := LineaBeats[Track].PitchHI;
                            END;
                    ELSE
                        BEGIN
                             if (addr(ConvertTCK2MidiEffect)<>nil) then
                                    ConvertTCK2MidiEffect(D^.Eff,D^.Val,Track,@BufferCmds);

                        END;
                    END;

                   {----------------------}
                   {Modificar Sistema MIDI}
                   {----------------------}

                   {Comandos propios}
                   IF (BufferCmds.Data[0]<$F0) THEN
                   BEGIN
                        {Comando compuesto}
                        FOR I:=0 TO (BufferCmds.Num DIV 3)-1 DO
                            SalidasMIDI[Info^.Disp].Comando(BufferCmds.Data[I*3+0],
                                                           BufferCmds.Data[I*3+1],
                                                           BufferCmds.Data[I*3+2]);
                   END
                   ELSE
                       {Comando SysEx}
                       SalidasMIDI[Info^.Disp].SysEx(@BufferCmds.Data,BufferCmds.Num);
               END;
          END;

          {Mandar nota si es que hay}
          IF (D^.Nota>0) THEN
          BEGIN
               {Escribir en la tabla}
               IF (LineaBeats[Track].Activo=True) THEN {Si habia un sonido en note on ...}
               {...Note off} SalidasMIDI[LineaBeats[Track].Disp].
                             Comando($90 OR LineaBeats[Track].Canal,LineaBeats[Track].Nota,0);

               {Actualizar información}
               LineaBeats[Track].Disp  :=Info^.Disp;
               LineaBeats[Track].Canal :=Info^.Canal;
               LineaBeats[Track].Nota  :=D^.Nota-1;
               LineaBeats[Track].Activo:=True;

               {Note on}
               SalidasMIDI[LineaBeats[Track].Disp].Comando($90 OR LineaBeats[Track].Canal,D^.Nota-1,D^.Vol);
          END;
     END;
END;
{----------------------------------------------------------------------------}
PROCEDURE PlayLBeats(P:PPattern;TI:PTablaIns;Beat:TBeatNum);
VAR
   I:TTrackNum;
BEGIN
     FOR I:=0 TO MaxTracks-1 DO
     BEGIN
          CASE EstadoTracks[i] OF

             ET_Activo   :PlayBeat(LeerTrackDePattern(P,I),LeerInfoDeTablaIns(TI,I),Beat,I);

             ET_Inactivar:BEGIN
                             SalidasMIDI[LineaBeats[I].Disp].Comando($B0 OR LineaBeats[I].Canal,$78,0);
                             EstadoTracks[i]:=ET_Inactivo;
                          END;

             ET_Inactivo :BEGIN {Nada} END;
          END;
     END;
END;
{----------------------------------------------------------------------------}
PROCEDURE PlayLineaBeats(P:PPattern;TI:PTablaIns;Beat:TBeatNum);
BEGIN
     IF (EstadoReproduccion = ER_NotPlaying) then
     BEGIN
          InicializarSistemaReproduccion(TI);
          PlayLBeats(P,TI,Beat);

          Delay(250);
          ResetSistemaMidi;
     END;
END;
{----------------------------------------------------------------------------}
PROCEDURE PlayTrack(Track:PTrack;T:TTrackNum;Info:PInfoTrack);
BEGIN
     {Si el temporizador esta activo, entonces solo se puede parar si ha sido
      desde aqui desde donde se ha iniciado}

     IF (EstadoReproduccion = ER_NotPlaying) then
     BEGIN
          NumTrackActual    := T;
          TrackActual       := Track;
          InfoInstActual    := Info;

          BeatsPerMinute    := BPMS;
          BTP               := False;

          ActivarPropiedadesMIDI(Info); {Creo que esto debe ir aqui}
          IniciaReproduccion(0);
     END
     ELSE
          IF (EstadoReproduccion = ER_PlayTrack) Then StopTrack;
END;

{----------------------------------------------------------------------------}
PROCEDURE StopTrack;
BEGIN
     PararReproduccion;
END;

{----------------------------------------------------------------------------}
PROCEDURE PlayPattern(Pattern:PPattern;TI:PTablaIns);
BEGIN
     IF (EstadoReproduccion = ER_NotPlaying) then
     BEGIN
          PatternActual     := Pattern;
          TablaInstrumentos := TI;

          BeatsPerMinute    := BPMS;
          BTP               := False;
          InicializarSistemaReproduccion(TI);
          IniciaReproduccion(1);
     END
     ELSE
          IF (EstadoReproduccion = ER_PlayPattern) Then StopPattern;
END;

{----------------------------------------------------------------------------}
PROCEDURE StopPattern;
BEGIN
     PararReproduccion;
END;

{----------------------------------------------------------------------------}
PROCEDURE PlayCancion(C:PCancion);
BEGIN
     IF (EstadoReproduccion = ER_NotPlaying) then
     BEGIN
          CancionActual     := C;
          NumPatternActual  := LeerIndicePatronEnSecuencia(CancionActual^.Seq,NumIndexSActual);
          PatternActual     := ObtenerPatronDeSecuencia(CancionActual,NumIndexSActual);
          TablaInstrumentos := CancionActual^.TIns;

          BeatsPerMinute    := BPMS;
          BTP               := False;

          InicializarSistemaReproduccion(TablaInstrumentos);
          IniciaReproduccion(2);
     END
     ELSE
          IF (EstadoReproduccion = ER_PlayPattern) Then StopCancion;
END;
{----------------------------------------------------------------------------}
PROCEDURE StopCancion;
BEGIN
     PararReproduccion;
END;
{----------------------------------------------------------------------------}
PROCEDURE EstablecerEstadoTrack(Track:TTrackNum;Mute:BOOLEAN);
BEGIN
     IF Mute THEN
         EstadoTracks[Track] := ET_Inactivar
     ELSE
         EstadoTracks[Track] := ET_Activo;
END;
{----------------------------------------------------------------------------}
PROCEDURE EstablecerLooping(OnOff:BOOLEAN);
BEGIN
     LoopActivado:=OnOff;
END;
{----------------------------------------------------------------------------}
PROCEDURE EstablecerBeatsPerMinute(B_P_M_S:WORD);
BEGIN
     BPMS:=B_P_M_S;
     BeatsPerMinute:=BPMS;
END;
{----------------------------------------------------------------------------}
FUNCTION TomarBeatsPerMinute:WORD;
BEGIN
     TomarBeatsPerMinute:=BeatsPerMinute;
END;
{----------------------------------------------------------------------------}
PROCEDURE ObtenerMinMaxBPMS(VAR Min,Max:WORD);
BEGIN
     Min:=MinBPMS;
     Max:=MaxBPMS;
END;
{----------------------------------------------------------------------------}
PROCEDURE ObtenerEstadoReproduccion(VAR Beat:TBeatNum;VAR Pattern:TPatternNum;VAR IndSec:TIndexSec);
BEGIN
     Beat    := NumBeatActual;
     Pattern := NumPatternActual;
     IndSec  := NumIndexSActual;
END;
{----------------------------------------------------------------------------}
PROCEDURE EstablecerEstadoReproduccion(Beat:TBeatNum;IndSec:TIndexSec);
BEGIN
     NumBeatActual   := Beat;
     NumIndexSActual := IndSec;
END;
{----------------------------------------------------------------------------}
FUNCTION Reproduciendo:BOOLEAN;
BEGIN
     Reproduciendo:=(EstadoReproduccion <> ER_NotPlaying);
END;
{-------------------------------------------------------------------------------}
PROCEDURE ParaTemporizador;
BEGIN
     IF TemporizadorActivo THEN
     BEGIN
          CloseHandle(ThreadPlayer);

          TemporizadorActivo := False;
          EstadoReproduccion := ER_NotPlaying;
     END;
END;
{-------------------------------------------------------------------------------}
PROCEDURE TimerCallBackTrack(uTimerID,uMessage:word;dwUser,dw1,dw2:longint);stdcall;
BEGIN
     timeKillEvent(TimerID); {Libera el Handle interno}

     PlayBeat(TrackActual,InfoInstActual,NumBeatActual,NumTrackActual);
     NumBeatActual:=(NumBeatActual + 1) MOD MaxBeats;

     IF (BTP) THEN BEGIN BTP:=FALSE;NumBeatActual:=0;END;

     IF (NumBeatActual=0) THEN
        IF (NOT LoopActivado) THEN
        BEGIN
             ParaTemporizador;
             Exit;
        END
        ELSE
            BeatsPerMinute:=BPMS;

     {Calculo de la velocidad y recalibracion del reloj si hace falta}
     TiempoTranscurrido := TiempoTranscurrido + MEL1;        {MEL1 Contenia los
                                                              milisegundos entre lineas iniciales}
     TiempoReal         := TiempoReal + MEL2;

     MEL1:=1000/(BeatsPerMinute*8/60);
     MEL2:= Trunc(MEL1);
     MR  := Trunc(TiempoTranscurrido-TiempoReal);

     IF (MR>0) THEN MEL2:=MEL2+MR;

     {Reprogramar el temporizador}
     IF (TemporizadorActivo) then
        TimerID:=timeSetEvent(MEL2,PeriodoResolucion,@TimerCallBackTrack,0,time_Oneshot);
END;
{-------------------------------------------------------------------------------}
PROCEDURE TimerCallBackPattern(uTimerID,uMessage:word;dwUser,dw1,dw2:longint);stdcall;
BEGIN
     timeKillEvent(TimerID); {Libera el Handle interno}

     PlayLBeats(PatternActual,TablaInstrumentos,NumBeatActual);
     NumBeatActual:=(NumBeatActual + 1) MOD MaxBeats;

     IF (BTP) THEN BEGIN BTP:=FALSE;NumBeatActual:=0;END;

     IF (NumBeatActual=0) THEN
        IF (NOT LoopActivado) THEN
        BEGIN
             ParaTemporizador;
             Exit;
        END
        ELSE
            BeatsPerMinute:=BPMS;

     {Calculo de la velocidad y recalibracion del reloj si hace falta}
     TiempoTranscurrido := TiempoTranscurrido + MEL1;        {MEL1 Contenia los
                                                              milisegundos entre lineas iniciales}
     TiempoReal         := TiempoReal + MEL2;

     MEL1:=1000/(BeatsPerMinute*8/60);
     MEL2:= Trunc(MEL1);
     MR  := Trunc(TiempoTranscurrido-TiempoReal);

     IF (MR>0) THEN MEL2:=MEL2+MR;

     {Reprogramar el temporizador}
     IF (TemporizadorActivo) then
        TimerID:=timeSetEvent(MEL2,PeriodoResolucion,@TimerCallBackPattern,0,time_Oneshot);
END;
{-------------------------------------------------------------------------------}
PROCEDURE TimerCallBackSong(uTimerID,uMessage:word;dwUser,dw1,dw2:longint);stdcall;
BEGIN
     timeKillEvent(TimerID); {Libera el Handle interno}

     PlayLBeats(PatternActual,TablaInstrumentos,NumBeatActual);
     NumBeatActual:=(NumBeatActual + 1) MOD MaxBeats;

     IF (BTP) THEN
        BEGIN BTP:=FALSE;NumBeatActual:=0;END;

     IF (NumBeatActual=0) THEN
     BEGIN
          IF (NumIndexSActual+1 < NumPatronesEnSecuencia(CancionActual^.Seq)) THEN
          BEGIN
               inc(NumIndexSActual);
               NumPatternActual := LeerIndicePatronEnSecuencia(CancionActual^.Seq,NumIndexSActual);
               PatternActual    := ObtenerPatronDeSecuencia(CancionActual,NumIndexSActual);
          END
          ELSE
          BEGIN
               IF (NOT LoopActivado) THEN
               BEGIN
                    ParaTemporizador;
                    Exit;
               END
               ELSE
               BEGIN
                    BeatsPerMinute:=BPMS;

                    NumIndexSActual:= 0;
                    PatternActual   := ObtenerPatronDeSecuencia(CancionActual,NumIndexSActual);
               END;
          END;
     END;

     {Calculo de la velocidad y recalibracion del reloj si hace falta}
     TiempoTranscurrido := TiempoTranscurrido + MEL1;        {MEL1 Contenia los
                                                              milisegundos entre lineas iniciales}
     TiempoReal         := TiempoReal + MEL2;

     MEL1:=1000/(BeatsPerMinute*8/60);
     MEL2:= Trunc(MEL1);
     MR  := Trunc(TiempoTranscurrido-TiempoReal);

     IF (MR>0) THEN MEL2:=MEL2+MR;

     {Reprogramar el temporizador}
     IF (TemporizadorActivo) then
        TimerID:=timeSetEvent(MEL2,PeriodoResolucion,@TimerCallBackSong,0,time_Oneshot);
END;
{-------------------------------------------------------------------------------}
{ ThreadFunc
{-------------------------------------------------------------------------------}
FUNCTION IniciaTemporizador(Tipo:POINTER):INTEGER;STDCALL;
BEGIN
     IF (NOT TemporizadorActivo) THEN
     BEGIN
          {EstablecerCanalesMIDI;}
          TemporizadorActivo :=True;
          TiempoTranscurrido :=0;
          TiempoReal         :=0;
          MEL1:=  1000/(BeatsPerMinute*8/60);
          MEL2 := Trunc(MEL1);

          {Establecer Resolucion de eventos ocurren con una precision de +-10 milisegundos}
          timeBeginPeriod(PeriodoResolucion);

          CASE INTEGER(Tipo^) OF
               0:BEGIN
                      EstadoReproduccion:=ER_PlayTrack;
                      TimerID:=timeSetEvent(MEL2,PeriodoResolucion,@TimerCallBackTrack,0,time_Oneshot);
                 END;
               1:BEGIN
                      EstadoReproduccion:=ER_PlayPattern;
                      TimerID:=timeSetEvent(MEL2,PeriodoResolucion,@TimerCallBackPattern,0,time_Oneshot);
                 END;
               2:begin
                      EstadoReproduccion:=ER_PlaySong;
                      TimerID:=timeSetEvent(MEL2,PeriodoResolucion,@TimerCallBackSong,0,time_Oneshot);
                 end;
          END;

          {Este thread estara en espera hasta que se pulse Stop, que llamara a
          PararTemporizador o hasta que acabe la cancion, que tambien llamara
          a PararTemporizador}

          WaitForSingleObject(ThreadPlayer,INFINITE);

          {WHILE (EstadoReproduccion<>ER_NotPlaying) DO;} {Busy Wait}

          timeEndPeriod(PeriodoResolucion);

          Delay(250);
          ResetSistemaMidi;
     END;

     {Retornar un valor cualquiera}
     IniciaTemporizador:=0;
END;
{-------------------------------------------------------------------------------}
PROCEDURE IniciaReproduccion(Tipo:INTEGER);
BEGIN
     {Crear el ThreadPlayer y retornar}
     IF NOT TemporizadorActivo THEN
     BEGIN
          {ShowMessage('Antes');}
          ThreadPlayerPar := Tipo;

          ThreadPlayer := CreateThread(NIL,
                                       65536,
                                       @IniciaTemporizador,
                                       @ThreadPlayerPar,
                                       CREATE_SUSPENDED,
                                       ThreadPlayerID);

          IF (ThreadPlayer<>0) THEN
          BEGIN
               SetThreadPriority(ThreadPlayer,THREAD_PRIORITY_TIME_CRITICAL);
               ResumeThread(ThreadPlayer);
          END;
     END;
END;

PROCEDURE PararReproduccion;
BEGIN
     ParaTemporizador;
END;
//----------------------------------------------------------------------------
procedure EstablecerFuncionConversionTCKEffect2MidiEvent(funcion:pointer);
begin
     if (funcion<>nil) then
        addr(ConvertTCK2MidiEffect) := funcion;
end;
//----------------------------------------------------------------------------
PROCEDURE Inicializacion;
VAR
   I:INTEGER;
BEGIN
     {Esta es la seccion de inicializacion, donde se deben obtener todos
     los dispositivos MIDI Out e inicializarlos, si no es posible, se debe dar un mensaje
     de error y salir del programa, o crear una funcion que haga esto mismo}
     NumSalidasMidi := NumeroDispositivosMidiOut;

     {Obtener Dispositivos Midi Out}
     FOR I:=0 TO NumSalidasMidi-1 DO
     BEGIN
          SalidasMidi[i].Init;
          SalidasMidi[i].EstablecerDispositivo(I);
          SalidasMidi[i].Open(NIL);
     END;

     FOR I:=0 TO MaxTracks-1 DO EstadoTracks[I]:=ET_Activo;

     {Suponemos que no se esta reproducciendo nada}
     EstadoReproduccion:=ER_NotPlaying;

     NumBeatActual    := 0;
     NumTrackActual   := 0;
     NumPatternActual := 0;
     NumIndexSActual  := 0;

     {Establecer valores minimo y maximo de los bpms}
     timeGetDevCaps(@TC,SizeOf(TTimeCaps));
     IF ((1000 / TC.wPeriodMin)*60/8) > 999 THEN MaxBpms := 1000 ELSE MaxBPMS :=TRUNC((1000 / TC.wPeriodMin)*60/8);
     IF ((1000 / TC.wPeriodMax)*60/8) < 10  THEN MinBpms := 10   ELSE MinBPMS :=TRUNC((1000 / TC.wPeriodMax)*60/8);

     if (TC.wPeriodMin<=5) THEN PeriodoResolucion:=5 ELSE PeriodoResolucion:=TC.wPeriodMin;

     BPMS := BPMS_POR_DEFECTO;
     TemporizadorActivo := False;
END;
//----------------------------------------------------------------------------
PROCEDURE Finalizacion;
VAR
   I:INTEGER;
BEGIN
     {Seccion de finalizacion, donde se liberaran los recursos tomados}
     FOR I:=0 TO NumSalidasMidi-1 DO
     BEGIN
          SalidasMidi[i].Close;
          SalidasMidi[i].Destruir;
     END;
END;
//----------------------------------------------------------------------------
BEGIN
     Inicializacion;
END.