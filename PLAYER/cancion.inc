{------------------------------------------------------------------------------}
PROCEDURE CrearCancion(VAR C:PCancion);
begin
     IF (C<>NIL) THEN DestruirCancion(C);
     NEW(C);
     FILLCHAR(C^,SizeOF(TCancion),#0); {Llenamos todo de 0's}

     C^.Titulo:= TITULO_POR_DEFECTO;
     C^.Autor := AUTOR_POR_DEFECTO;
     C^.BPMS  := BPMS_POR_DEFECTO;

     {En este modelo de estructura, solo se crean patrones a medida que
     se van utilizando. El primer patron es el unico que se crea}
     CrearPattern(C^.Pats[0]);
     CrearTablaIns(C^.TIns);
     CrearSecuencia(C^.Seq);
END;
{------------------------------------------------------------------------------}
PROCEDURE DestruirCancion(VAR C:PCancion);
var
   I:TPatternNum;
BEGIN
     IF (C<>NIL) THEN
     BEGIN
          FOR I:=0 TO MaxPatterns-1 DO
              IF (C^.Pats[I]<>NIL) THEN DestruirPattern(C^.Pats[I]);

          DestruirTablaIns(C^.TIns);
          DestruirSecuencia(C^.Seq);

          Dispose(C);
     END;
END;
{------------------------------------------------------------------------------}
FUNCTION CancionVacia(C:PCancion):BOOLEAN;
VAR
   I:BYTE;
   V:BOOLEAN;
BEGIN
     IF (C=NIL) THEN BEGIN CancionVacia:=TRUE;EXIT;END;

     I:=0;
     V:=PatternVacio(C^.Pats[i]);
     WHILE (V) and (i<MaxPatterns-1) DO
     BEGIN
          INC(I);
          V:=PatternVacio(C^.Pats[i]);
     END;
     CancionVacia:=V;
END;
{------------------------------------------------------------------------------}
FUNCTION NumeroDePatterns(C:PCancion):WORD;
VAR
   I,j:WORD;
BEGIN
     j:=0;
     FOR I:=0 TO MaxPatterns-1 DO IF NOT PatternVacio(C^.Pats[I]) THEN INC(j);
     NumeroDePatterns:=j;
END;
{------------------------------------------------------------------------------}
FUNCTION CargarCancionGeneral(VAR C:PCancion;VAR F:File):boolean;
CONST
     SONG_ID:ARRAY[0..3] OF CHAR =('T','C','K','2');
VAR
   SID:ARRAY[0..3]OF CHAR;
   Pos,Cantidad:LONGINT;
   ID:WORD;
BEGIN
      CrearCancion(C);

      {Lectura del identificador}
      BLOCKREAD(F,SID,4);
      IF (IORESULT<>0) THEN BEGIN CargarCancionGeneral:=False; EXIT;END;
      IF (SID<>SONG_ID) THEN BEGIN CargarCancionGeneral:=False; EXIT;END;

      {Lectura de la  versión}
      BLOCKREAD(F,VersionID,4);
      IF (IORESULT<>0) THEN BEGIN CargarCancionGeneral:=False; EXIT;END;

      {Cargar informacion relativa a la cancion}
      BLOCKREAD(F,C^.Titulo,MaxCarNombreCancion+1);
      IF (IORESULT<>0) THEN BEGIN C^.Titulo:=TITULO_POR_DEFECTO;CargarCancionGeneral:=False; EXIT;END;
      BLOCKREAD(F,C^.Autor,MaxCarNombreAutor+1);
      IF (IORESULT<>0) THEN BEGIN C^.Titulo:=TITULO_POR_DEFECTO;C^.Autor:=AUTOR_POR_DEFECTO;CargarCancionGeneral:=False; EXIT;END;
      BLOCKREAD(F,C^.BPMS,2);
      IF (IORESULT<>0) THEN BEGIN C^.Titulo:=TITULO_POR_DEFECTO;C^.Autor:=AUTOR_POR_DEFECTO;C^.BPMS:=BPMS_POR_DEFECTO;
                                  CargarCancionGeneral:=False; EXIT;END;

      Pos:=FilePos(F);
      WHILE NOT (EOF(F)) DO
      BEGIN
           {Leer identificador de tipo de estructura}
           BLOCKREAD(F,SID,4);

           {Leer identificador de grupo de estructura}
           BLOCKREAD(F,ID,SizeOF(WORD));

           {Leer cantidad de bytes que ocupa la estructura}
           BLOCKREAD(F,Cantidad,SizeOF(LONGINT));

           Seek(F,Pos); {Posicionar fichero a principio de bloque de estructura}
           Pos:=Pos+Cantidad;

           {-------------------------------}
           {Leer Estructura correspondiente}
           {-------------------------------}

           {-------------------------------} {Secuencia}
           IF (SID = 'SQNC') AND (ID=65535) THEN
           BEGIN
                CargarSecuenciaGeneral(C^.Seq,F,ID)
           END
           {-------------------------------} {Tabla Instrumentos}
      ELSE IF (SID = 'TINS') AND (ID=65535) THEN
           BEGIN
                CargarTablaInsGeneral(C^.TIns,F,ID)
           END
           {-------------------------------} {Patrones}
      ELSE IF (SID = 'PTRN') AND (ID<MaxPatterns) THEN
           BEGIN
                CargarPatternGeneral(C^.Pats[ID],F,ID)
           END;
           {-------------------------------}
           Seek(F,Pos);
      END;

      CargarCancionGeneral:=True;
END;
{------------------------------------------------------------------------------}
FUNCTION SalvarCancionGeneral(C:PCancion;VAR F:File):boolean;
CONST
     SONG_ID:ARRAY[0..3] OF CHAR =('T','C','K','2');
VAR
   I:TPatternNum;
BEGIN
     {Escritura del identificador}
     BLOCKWRITE(F,SONG_ID,4);
     IF (IORESULT<>0) THEN BEGIN SalvarCancionGeneral:=False; EXIT;END;

     {Escritura de la  versión}
     BLOCKWRITE(F,VersionID,4);
     IF (IORESULT<>0) THEN BEGIN SalvarCancionGeneral:=False; EXIT;END;

     {Salvar informacion relativa a la cancion}
     BLOCKWRITE(F,C^.Titulo,MaxCarNombreCancion+1);
     IF (IORESULT<>0) THEN BEGIN SalvarCancionGeneral:=False; EXIT;END;
     BLOCKWRITE(F,C^.Autor,MaxCarNombreAutor+1);
     IF (IORESULT<>0) THEN BEGIN SalvarCancionGeneral:=False; EXIT;END;
     BLOCKWRITE(F,C^.BPMS,2);
     IF (IORESULT<>0) THEN BEGIN SalvarCancionGeneral:=False; EXIT;END;

     {Escritura de la tabla de instrumentos}
     IF NOT SalvarTablaInsGeneral(C^.TIns,F,65535) THEN
     BEGIN
          {Ocurrió un error por ahí}
          SalvarCancionGeneral:=False;
          EXIT;
     END;

     {Escritura de la secuencia}
     IF NOT SalvarSecuenciaGeneral(C^.Seq,F,65535) THEN
     BEGIN
          {Ocurrió un error por ahí}
          SalvarCancionGeneral:=False;
          EXIT;
     END;

     {Escritura del numero de patterns en la cancion}
     FOR I:=0 TO MaxPatterns-1 DO
         IF (NOT PatternVacio(C^.Pats[I])) THEN
         BEGIN
              {Escritura del identificador del pattern}
              IF NOT SalvarPatternGeneral(C^.Pats[I],F,I) THEN
              BEGIN
                   {Ocurrió un error por ahí}
                   SalvarCancionGeneral:=False;
                   EXIT;
              END;
         END;

     SalvarCancionGeneral:=True;
END;
{------------------------------------------------------------------------------}
FUNCTION ObtenerPatronDeCancion(C:PCancion;P:TPatternNum):PPattern;
BEGIN
     ObtenerPatronDeCancion:=C^.Pats[P];
END;
{------------------------------------------------------------------------------}
FUNCTION ObtenerPatronDeSecuencia(C:PCancion;PActual:TIndexSec):PPattern;
BEGIN
     ObtenerPatronDeSecuencia:=C^.Pats[LeerIndicePatronEnSecuencia(C^.Seq,PActual)];
END;
{------------------------------------------------------------------------------}
