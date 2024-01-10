{Indices:
         Cuando tiene un valor establecido <>65535 denota el puesto que
         ocupa dentro de un grupo de estructuras del mismo tipo        }

{----------------------------------------------------------------------------}
PROCEDURE CrearPattern(VAR P:PPattern);
BEGIN
     IF (P<>NIL) THEN DestruirPattern(P);
     New(P);

     FILLCHAR(P^,SizeOF(TPattern),#0);
END;
{----------------------------------------------------------------------------}
PROCEDURE DestruirPattern(VAR P:PPattern);
BEGIN
     IF (P<>NIL) THEN Dispose(P);
     P:=NIL;
END;
{----------------------------------------------------------------------------}
PROCEDURE CopiarPattern(F,D:PPattern);
VAR
   I:TTrackNum;
BEGIN
     FOR I:=0 TO MaxTracks-1 DO CopiarTrack(@(F^.Tracks[I]),@(D^.Tracks[I]));
END;
{----------------------------------------------------------------------------}
PROCEDURE BorrarPattern(P:PPattern);
BEGIN
     FILLCHAR(P^,SizeOf(TPattern),#0);
END;
{----------------------------------------------------------------------------}
FUNCTION CargarPatternGeneral(VAR P:PPattern;VAR F:File;VAR ID:WORD):Boolean;
CONST
     PATTERN_ID:ARRAY[0..3] OF CHAR =('P','T','R','N');
VAR
   PTR:ARRAY[0..3] OF CHAR;
   I,M,IdentTrk:TTrackNum;
   Pos,N,Cantidad:LONGINT;

BEGIN
     CrearPattern(P);

     {Lectura del identificador}
     BLOCKREAD(F,PTR,4);
     IF (IORESULT<>0) THEN BEGIN CargarPatternGeneral:=False; EXIT;END;
     IF (PTR<>PATTERN_ID) THEN BEGIN CargarPatternGeneral:=False; EXIT;END;

     {Lectura del identificador de estructura}
     BLOCKREAD(F,ID,SizeOF(ID));
     IF (IORESULT<>0) THEN BEGIN CargarPatternGeneral:=False; EXIT;END;

     {Lectura del tama¤o de la estructura}
     BLOCKREAD(F,N,SizeOF(LONGINT));
     IF (IORESULT<>0) THEN BEGIN CargarPatternGeneral:=False; EXIT;END;

     {Lectura del nombre asociado al pattern}
     BLOCKREAD(F,P^.NOMBRE,MaxCarNombrePatron+1);
     IF (IORESULT<>0) THEN BEGIN CargarPatternGeneral:=False; EXIT;END;

     {Leer numero de tracks en pattern}
     BLOCKREAD(F,M,SizeOF(WORD));
     IF (IORESULT<>0) THEN BEGIN CargarPatternGeneral:=False; EXIT;END;


     Pos:=FilePos(F);
     FOR I:=0 TO M-1 DO
     BEGIN
          {----------------------------}
          {Lectura de info de estuctura}
          {----------------------------}
          BLOCKREAD(F,PTR,4);
          IF (IORESULT<>0) THEN BEGIN CargarPatternGeneral:=False; EXIT;END;

          BLOCKREAD(F,IdentTrk,SizeOf(WORD));
          IF (IORESULT<>0) THEN BEGIN CargarPatternGeneral:=False; EXIT;END;

          BLOCKREAD(F,Cantidad,SizeOf(LONGINT));
          IF (IORESULT<>0) THEN BEGIN CargarPatternGeneral:=False; EXIT;END;
          {----------------------------}

          SEEK(F,Pos);
          Pos:=Pos+Cantidad;

          IF (PTR='TRAK') THEN
          BEGIN
               CargarTrackGeneral(@(P^.Tracks[IdentTrk]),F,IdentTrk);
          END
     ELSE BEGIN

          END;

          SEEK(F,Pos);
     END;

     CargarPatternGeneral:=(TamanyoPattern(P)=N);
END;
{----------------------------------------------------------------------------}
FUNCTION SalvarPatternGeneral(P:PPattern;VAR F:File;ID:WORD):Boolean;
CONST
     PATTERN_ID:ARRAY[0..3] OF CHAR =('P','T','R','N');
VAR
   N:LONGINT;
   I:TPatternNum;
BEGIN
     {Escritura del identificador}
     BLOCKWRITE(F,PATTERN_ID,4);
     IF (IORESULT<>0) THEN BEGIN SalvarPatternGeneral:=False; EXIT;END;

     {Escritura del identificador de estructura}
     BLOCKWRITE(F,ID,SizeOF(WORD));
     IF (IORESULT<>0) THEN BEGIN SalvarPatternGeneral:=False; EXIT;END;

     {Escritura del bloque de info que viene despues:Nombre + InfoTracks}
     N:=TamanyoPattern(P);
     BLOCKWRITE(F,N,SizeOF(LONGINT));
     IF (IORESULT<>0) THEN BEGIN SalvarPatternGeneral:=False; EXIT;END;

     {Escritura del nombre asociado al pattern}
     BLOCKWRITE(F,P^.NOMBRE,MaxCarNombrePatron+1);
     IF (IORESULT<>0) THEN BEGIN SalvarPatternGeneral:=False; EXIT;END;

     {Escritura del numero de eventos (Tracks) en el pattern}
     I:=NumeroDeTracksEnPattern(P);
     BLOCKWRITE(F,I,SizeOF(WORD));
     IF (IORESULT<>0) THEN BEGIN SalvarPatternGeneral:=False; EXIT;END;

     FOR I:=0 TO MaxTracks-1 DO
         IF (NOT TrackVacio(@(P^.Tracks[I]))) THEN
         BEGIN
              IF NOT SalvarTrackGeneral(@(P^.Tracks[I]),F,I) THEN
              BEGIN
                   SalvarPatternGeneral:=False;
                   EXIT;
              END;
         END;

     SalvarPatternGeneral:=True;
END;
{----------------------------------------------------------------------------}
FUNCTION LeerTrackDePattern(P:PPattern;I:TTrackNum):PTrack;
BEGIN
     LeerTrackDePattern := @(P^.Tracks[I])
END;
{----------------------------------------------------------------------------}
FUNCTION NumeroDeTracksEnPattern(P:PPattern):TTrackNum;
VAR
   I,n:TTrackNum;
BEGIN
     N:=0;
     FOR i:=0 to MaxTracks-1 DO IF (NOT TrackVacio(@(P^.Tracks[i]))) THEN inc(n);
     NumeroDeTracksEnPattern:=n;
END;
{----------------------------------------------------------------------------}
FUNCTION TamanyoPattern(P:PPattern):LONGINT;
VAR
   I,R:INTEGER;
BEGIN
     R:=0;
     FOR I:=0 TO MaxTracks-1 DO
        IF Not TrackVacio(@(P^.Tracks[I])) THEN
           R:=R+TamanyoTrack(@(P^.Tracks[I])); {Tamanyo en tracks}

     TamanyoPattern:=R+MaxCarNombrePatron+1+SizeOF(WORD)+10;
END;
{----------------------------------------------------------------------------}
FUNCTION LeerNombreDelPattern(P:PPattern):STRING;
BEGIN
     LeerNombreDelPattern:=P^.Nombre;
END;
{----------------------------------------------------------------------------}
PROCEDURE EscribirNombreDelPattern(P:PPattern;Nombre:String);
BEGIN
     P^.Nombre:=Nombre;
END;
{----------------------------------------------------------------------------}
FUNCTION PatternVacio(P:PPattern):BOOLEAN;
VAR
   I:TTrackNum;
   V:BOOLEAN;
BEGIN
     IF (P=NIL) THEN BEGIN PatternVacio:=TRUE;EXIT;END;

     I:=0;
     V:=TrackVacio(@(P^.Tracks[i]));

     WHILE (V) AND (I<MaxTracks-1) DO
     BEGIN
          INC(I);
          V:=TrackVacio(@(P^.Tracks[i]));
     END;
     PatternVacio:=V;
END;
{----------------------------------------------------------------------------}
