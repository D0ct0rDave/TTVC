program corrector;
uses
    crt;
CONST
     MaxBeats            = 128;
     MaxTracks           = 64;
     MaxCarNombreCancion = 80;
     MaxCarNombreAutor   = 80;
     MaxCarNombrePatron  = 80;
TYPE
     Evento = RECORD
                   Time:WORD;

                   Nota,
                   Vol,
                   Eff,
                   Val:BYTE;
              END;

     Instrum = RECORD
                    Nombre :STRING[35];
                    Disp,
                    Banco,
                    Inst,
                    Canal,
                    VolDef:byte;
                    Ctrl0,
                   Ctrl32:BOOLEAN;
               END;

     IDENTIFICADOR = ARRAY[0..3] OF CHAR;

VAR
   F,D:FILE;

   ID : IDENTIFICADOR;
   PID: ^IDENTIFICADOR;


   PEv:WORD;
   Evs:ARRAY[0..MaxBeats] OF Evento;
   Ev : Evento;

   I,J,K,M,N,O,P : WORD;
   AUX :BYTE;
   S : STRING;
   ins:iNSTRUM;

   IDE : BYTE;

BEGIN
     TextColor(7);
     Nosound;
     WriteLn;

     ASSIGN(F,PARAMSTR(1));
     RESET(F,1);

     ASSIGN(D,'SALIDA.TCK');
     REWRITE(D,1);

     {Lectura de los campos iniciales}

     BLOCKREAD(F,ID,4);
     BLOCKWRITE(D,ID,4);

     WriteLn('Identificador: ',ID);

     BLOCKREAD(F,ID,4); {Version}
     BLOCKWRITE(D,ID,4);

     WriteLn('Version: ',ID);


     {Cargar informacion relativa a la cancion}
     BLOCKREAD(F,S,MaxCarNombreCancion+1);
     BLOCKWRITE(D,S,MaxCarNombreCancion+1);

     WriteLn('Titulo: ',S);

     BLOCKREAD(F,S,MaxCarNombreAutor+1);
     BLOCKWRITE(D,S,MaxCarNombreAutor+1);

     WriteLn('Autor: ',S);

     BLOCKREAD(F,I,2);
     BLOCKWRITE(D,I,2);
     WriteLn('BPMS: ',I);

     {Tabla de instrumentos}
     BLOCKREAD(F,ID,4);
     BLOCKWRITE(D,ID,4);

     BLOCKREAD(F,I,2);
     BLOCKWRITE(D,I,2);

     {WriteLn('Nombre:                            ','Disp Banco Inst Canal VolDef C0 C32');}

     for i:=0 to MaxTracks-1 do
     begin
          BLOCKREAD (F,Ins,Sizeof(Instrum));
          BLOCKWrite(D,Ins,Sizeof(Instrum));
          {WriteLn(Ins.Nombre,Ins.Disp,' ',Ins.Banco,'  ',Ins.Inst,' ',Ins.Canal,'  ',Ins.VolDef,'   ',Ins.Ctrl0,Ins.Ctrl32);}
     end;

     {Cargar Secuencia}
     BLOCKREAD(F,ID,4);
     BLOCKWRITE(D,ID,4);


     BLOCKREAD(F,I,2);
     BLOCKWRITE(D,I,2);
     WriteLn('Tama¤o de secuencia: ',I);

     FOR J:=0 TO I-1 DO
     BEGIN
          BLOCKREAD(F,AUX,1);
          BLOCKWRITE(D,AUX,1);

          Write(AUX,' ');
     END;
     WriteLn;

     {Comienza la informacion de los patrones}
     BLOCKREAD(F,I,2);
     DEC(I);
     BLOCKWRITE(D,I,2);
     WriteLn('Numero de patrones: ',I);


     FOR J:=0 TO I-1 DO
     BEGIN
          {Numero de pattern actual}
          BLOCKREAD(F,K,2);
          BLOCKWRITE(D,K,2);

          BLOCKREAD(F,ID,4);
          BLOCKWRITE(D,ID,4);

          {LeerNombre}
          BLOCKREAD(F,S,MaxCarNombrePatron+1);
          BLOCKWRITE(D,S,MaxCarNombrePatron+1);

          WriteLn('Patron ',K,' : ',S);

          {Eventos del patron: numero de tracks}
          BLOCKREAD(F,K,2);
          BLOCKWRITE(D,K,2);
          WriteLn(K,' tracks.');

          IDE :=0; {Identificador no encontrado}
          PID :=ADDR(Ev.Nota);

          {Leer contenido de cada track}
          FOR M:=0 TO K-1 DO
          BEGIN
               {Numero de track}
               BLOCKREAD(F,Ev,6);
               BLOCKREAD(F,P,2);
               N:=Ev.Time;

               PEv :=0;
               REPEAT
                     BLOCKREAD(F,Ev,6);

                     IF (PID^<>'TRAK') AND (PID^<>'PTRN') AND (IORESULT=0) THEN
                     BEGIN
                          IF (PEv<MaxBeats) THEN
                          BEGIN
                               Evs[PEv]:=Ev;
                               INC(Pev);
                          END;
                     END;

               UNTIL ((PID^='TRAK') OR (PID^='PTRN') OR EOF(F));

               IF (PEv<>P) THEN
               BEGIN
                    TextColor(14);
                    WriteLn('Numero de eventos difiere del encontrado: antes ',P,' ahora ',PEv);

                    SOUND(900);
                    DELAY(100);
                    NOSOUND;

                    TextColor(7);
               END;

               {Escribir identificador si hace falta}
               IF (PID^='TRAK') OR (PID^='PTRN') THEN
               BEGIN
                    SEEK(F,FILEPOS(F)-6);

                    ID:='TRAK';
                    BLOCKWRITE(D,N,2);    {Numero de track}
                    BLOCKWRITE(D,ID,4);   {Identificador estructura}
                    IDE:=1;

                    BLOCKWRITE(D,PEv,2);  {Numero de eventos de la pista}
                    BLOCKWRITE(D,Evs,SizeOf(Evento)*PEv);       {Eventos}
               END;
          END;
     END;

     CLOSE(F);
     CLOSE(D);

     WriteLn;
END.