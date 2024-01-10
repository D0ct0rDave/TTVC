{----------------------------------------------------------------------------}
PROCEDURE CrearTablaIns(VAR T:PTablaIns);
VAR
   i:TIndexTIns;
BEGIN
     IF (T<>NIL) THEN DestruirTablaIns(T);
     NEW(T);

     {Inicializar la tabla de instrumentos}
     FOR i:=0 to MaxTracks-1 do
     BEGIN
          T^.T[i].Nombre:='';
          T^.T[i].Disp  :=0;
          T^.T[i].Banco :=0;
          T^.T[i].Inst  :=255;
          T^.T[i].Canal :=0;
          T^.T[i].VolDef:=64;
          T^.T[i].Ctrl0 :=False;
          T^.T[i].Ctrl32:=False;
     END;
END;
{----------------------------------------------------------------------------}
PROCEDURE DestruirTablaIns(VAR T:PTablaIns);
BEGIN
     IF (T<>NIL) THEN
     BEGIN
          Dispose(T);
          T:=NIL;
     END;
END;
{----------------------------------------------------------------------------}
FUNCTION CargarTablaInsGeneral(VAR T:PTablaIns;VAR F:FILE;VAR ID:WORD):BOOLEAN;
CONST
     TINS_ID:ARRAY[0..3] OF CHAR=('T','I','N','S');
VAR
   TID:ARRAY[0..3] OF CHAR;
   Tam:TIndexTIns;
   Cantidad:LONGINT;
BEGIN
     CrearTablaIns(T);

     {Lectura del identificador}
     BLOCKREAD(F,TID,4);
     IF (IORESULT<>0) THEN BEGIN CargarTablaInsGeneral:=False; EXIT;END;
     IF (TID<>TINS_ID) THEN BEGIN CargarTablaInsGeneral:=False; EXIT;END;

     {Lectura de identificador de estructura}
     BLOCKREAD(F,ID,SizeOF(WORD));
     IF (IORESULT<>0) THEN BEGIN Close(F);CargarTablaInsGeneral:=False; EXIT;END;

     {Lectura de la cantidad de bytes}
     BLOCKREAD(F,Cantidad,SizeOF(LONGINT));
     IF (IORESULT<>0) THEN BEGIN Close(F);CargarTablaInsGeneral:=False; EXIT;END;

     {Lectura de la cantidad}
     BLOCKREAD(F,Tam,SizeOf(TIndexTIns));
     IF (IORESULT<>0) THEN BEGIN CargarTablaInsGeneral:=False; EXIT;END;
     IF (Tam>SizeOF(TTablaIns)) THEN Tam:=SizeOF(TTablaIns);

     {Lectura del contenido}
     BLOCKREAD(F,T^,Tam);

     CargarTablaInsGeneral:=(IORESULT=0);
END;
{------------------------------------------------------------------------------}
FUNCTION SalvarTablaInsGeneral(T:PTablaIns;VAR F:FILE;ID:WORD):BOOLEAN;
CONST
     TINS_ID:ARRAY[0..3] OF CHAR=('T','I','N','S');
VAR
   Tam:TIndexTIns;
   Cantidad:LONGINT;
BEGIN
     {Escritura del identificador}
     BLOCKWRITE(F,TINS_ID,4);
     IF (IORESULT<>0) THEN BEGIN SalvarTablaInsGeneral:=False; EXIT;END;

     {Escritura del identificador de estructura}
     BLOCKWRITE(F,ID,SizeOF(WORD));
     IF (IORESULT<>0) THEN BEGIN SalvarTablaInsGeneral:=False; EXIT;END;

     {Escritura de tamanyo de estructura}
     Cantidad :=TamanyoTablaIns(T);
     BLOCKWRITE(F,Cantidad,SizeOF(LONGINT));
     IF (IORESULT<>0) THEN BEGIN Close(f);SalvarTablaInsGeneral:=False; EXIT;END;

     {Escritura de la cantidad de eventos en la estructura}
     Tam:=SizeOF(TTablaIns);
     BLOCKWRITE(F,Tam,SizeOF(TIndexTIns));
     IF (IORESULT<>0) THEN BEGIN SalvarTablaInsGeneral:=False; EXIT;END;

     {Escritura del contenido de la estructura}
     BLOCKWRITE(F,T^,SizeOF(TTablaIns));

     SalvarTablaInsGeneral:=(IORESULT=0);
END;
{------------------------------------------------------------------------------}
FUNCTION LeerInfoDeTablaIns(T:PTablaIns;Track:TTrackNum):PInfoTrack;
BEGIN
     IF (Track<MaxTracks) THEN LeerInfoDeTablaIns:=@(T^.T[Track]) ELSE LeerInfoDeTablaIns:=NIL;
END;
{------------------------------------------------------------------------------}
FUNCTION TamanyoTablaIns(T:PTablaIns):LONGINT;
BEGIN
     // 10 : id(4) + idstruct(2)+cantidad(4) 
     TamanyoTablaIns:=SizeOf(TTablaIns) + SizeOf(TIndexTIns)+10;
END;
{------------------------------------------------------------------------------}