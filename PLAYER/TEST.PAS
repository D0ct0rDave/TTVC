PROGRAM Test;
USES
    Windows,
    dialogs,
    Consts2,
    Tipos,
    INTER,
    Play;

VAR
   T :TTrack;
   IT:array [0..1] of TInfoTrack;
   I :TBeatNum;

BEGIN
     IT[0].Disp  := 2;
     IT[0].Canal := 0;
     IT[0].Banco := 4;
     IT[0].Inst  := 0;
     IT[0].Ctrl0 := true;

     ResetSistemaMidi;

     ActivarPropiedadesMidi(@IT[0]);
     FOR I:=0 TO (MaxBeats-1) DIV 8 DO
     BEGIN
          InsertarEventoTrack(@T,I*8,42,127,0,0);
          InsertarEventoTrack(@T,I*8,42,127,0,0);
     END;

     PlayTrack(@T,0,@IT[0]);

     REPEAT UNTIL NOT Reproduciendo;

     ResetSistemaMidi;
     Finalizacion;
END.
