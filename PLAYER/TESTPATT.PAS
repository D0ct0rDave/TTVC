PROGRAM Test;
USES
    Windows,
    dialogs,
    Consts2,
    Tipos,
    INTER,
    Play;

VAR
   P :pPattern;
   TI:pTablaIns;
   It:PInfoTrack;
   T :PTrack;
   I :TBeatNum;
   F:FILE;
BEGIN
     CrearTablaIns(TI);
     CrearPattern (P);

     {-----------}
     IT := LeerInfoDeTablaIns(TI,0);

     IT^.Disp  := 2;
     IT^.Canal := 0;
     IT^.Banco := 4;
     IT^.Inst  := 0;
     IT^.Ctrl0 := true;
     {-----------}

     IT := LeerInfoDeTablaIns(TI,1);

     IT^.Disp  := 2;
     IT^.Canal := 1;
     IT^.Banco := 4;
     IT^.Inst  := 1;
     IT^.Ctrl0 := true;

     {-----------}

     IT := LeerInfoDeTablaIns(TI,2);

     IT^.Disp  := 2;
     IT^.Canal := 2;
     IT^.Banco := 4;
     IT^.Inst  := 1;
     IT^.Ctrl0 := true;
     {-----------}

     t:=LeerTrackDePattern(P,0);
     FOR I:=0 TO (MaxBeats-1) DIV 8 DO InsertarEventoTrack(T,I*8,42,90,0,0);

     {
     t:=LeerTrackDePattern(P,1);
     FOR I:=0 TO (MaxBeats-1) DIV 8 DO InsertarEventoTrack(T,I*8+4,42,80,0,0);
     }
     t:=LeerTrackDePattern(P,2);
     FOR I:=0 TO (MaxBeats-1) DIV 4 DO InsertarEventoTrack(T,I*4,52,80,0,0);



     {
     FOR I:=0 TO (MaxBeats-1) DO
         PlayLineaBeats(P,TI,I);
     }

     PlayPattern(P,TI);
     REPEAT UNTIL NOT Reproduciendo;

     Assign(F,'D:\songs\p0.pat');
     Rewrite(F,1);
     SalvarPattern(P,F,65535);
     CloseFile(F);

     Assign(F,'D:\songs\TI.INS');
     Rewrite(F,1);
     SalvarTablaIns(TI,F,65535);
     CloseFile(F);


     DestruirPattern(P);
     DestruirTablaIns(TI);

     Finalizacion;
END.
