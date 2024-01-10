{----------------------------------------------------------------------------}
PROCEDURE CrearSecuencia(VAR S:PSecuencia);
BEGIN
     IF (S<>NIL) THEN DestruirSecuencia(S);
     NEW(S);
     FILLCHAR(S^,SizeOF(TSecuencia),#0);
     S^.Num    :=1;
END;
{----------------------------------------------------------------------------}
PROCEDURE DestruirSecuencia(VAR S:PSecuencia);
BEGIN
     IF (S<>NIL) THEN
     BEGIN
          Dispose(S);
          S:=NIL;
     END;
END;
{----------------------------------------------------------------------------}
procedure DesplazarDerecha(S:PSecuencia;PActual:TIndexSec);
var
   i:TIndexSec;
begin
     {Funcion insertar}
     FOR i:=S^.Num downto PActual+1 do S^.S[i]:=S^.S[i-1];
     inc(S^.Num);
end;
{----------------------------------------------------------------------------}
procedure DesplazarIzquierda(S:PSecuencia;PActual:TIndexSec);
var
   i:TIndexSec;
begin
     {Funcion borrar}
     for i:=PActual to S^.Num-1-1 do S^.s[i]:=S^.s[i+1];
     dec(S^.Num);
end;
{----------------------------------------------------------------------------}
FUNCTION InsertarPatronEnSecuencia(S:PSecuencia;Pos:TIndexSec):boolean;
BEGIN
     IF (S^.Num<MaxPatternsSequence) then
     BEGIN
          DesplazarDerecha(S,Pos);
          InsertarPatronEnSecuencia:=True;
          Error:='';
     END
     ELSE
     BEGIN
          Error:='Se ha alcanzado el máximo numero de posiciones de la secuencia';
          InsertarPatronEnSecuencia:=False;
     END;
END;
{----------------------------------------------------------------------------}
FUNCTION BorrarPatronDeSecuencia(S:PSecuencia;VAR Pos:TIndexSec):boolean;
BEGIN
     IF (S^.Num>1) THEN
     BEGIN
          DesplazarIzquierda(S,Pos);
          if (Pos>S^.Num-1) then Pos:=S^.Num-1; {¨?}

          BorrarPatronDeSecuencia:=True;
          Error:='';
     end
     else
     begin
         BorrarPatronDeSecuencia:=False;
         Error:='No se puede borrar el único elemento de la secuencia';
     end;
END;
{----------------------------------------------------------------------------}
FUNCTION LeerIndicePatronEnSecuencia(S:PSecuencia;I:TIndexSec):TPatternNum;
BEGIN
     LeerIndicePatronEnSecuencia:=S^.S[I];
END;
{----------------------------------------------------------------------------}
PROCEDURE EscribirIndicePatronDeSecuencia(S:PSecuencia;I:TIndexSec;P:TPatternNum);
BEGIN
     S^.S[I]:=P;
END;
{----------------------------------------------------------------------------}
FUNCTION NumPatronesEnSecuencia(S:PSecuencia):TIndexSec;
BEGIN
     NumPatronesEnSecuencia:=S^.Num;
END;
{----------------------------------------------------------------------------}
FUNCTION CargarSecuenciaGeneral(VAR S:PSecuencia;VAR F:File;VAR ID:WORD):BOOLEAN;
CONST
     SEC_ID:ARRAY[0..3] OF CHAR=('S','Q','N','C');
VAR
   Tam:TIndexSec;
   SID:ARRAY[0..3] OF CHAR;
   Cantidad:LONGINT;

BEGIN
     {Lectura del identificador}
     BLOCKREAD(F,SID,4);
     IF (IORESULT<>0) THEN BEGIN Close(f);CargarSecuenciaGeneral:=False; EXIT;END;
     IF (SID<>SEC_ID) THEN BEGIN CargarSecuenciaGeneral:=False; EXIT;END;

     {Lectura de identificador de estructura}
     BLOCKREAD(F,ID,SizeOF(WORD));
     IF (IORESULT<>0) THEN BEGIN Close(f);CargarSecuenciaGeneral:=False; EXIT;END;

     {Lectura de la cantidad de bytes}
     BLOCKREAD(F,Cantidad,SizeOF(LONGINT));
     IF (IORESULT<>0) THEN BEGIN Close(F);CargarSecuenciaGeneral:=False; EXIT;END;

     {Lectura de la cantidad de eventos}
     BLOCKREAD(F,Tam,SizeOF(TIndexSec));
     IF (IORESULT<>0) THEN BEGIN Close(f);CargarSecuenciaGeneral:=False; EXIT;END;

     CrearSecuencia(S);
     IF (Tam<=MaxPatternsSequence) THEN S^.Num:=Tam ELSE S^.Num:=MaxPatternsSequence-1;

     {Lectura del contenido}
     BLOCKREAD(F,S^.S,S^.Num*SizeOf(TPatternNum));
     IF (IORESULT<>0) THEN BEGIN Close(f);CargarSecuenciaGeneral:=False; EXIT;END;

     CargarSecuenciaGeneral:=True;
END;
{----------------------------------------------------------------------------}
FUNCTION SalvarSecuenciaGeneral(S:PSecuencia;VAR F:File;ID:WORD):BOOLEAN;
CONST
     SEC_ID:ARRAY[0..3] OF CHAR=('S','Q','N','C');
VAR
   Cantidad:Longint;
BEGIN
     {Escritura del identificador}
     BLOCKWRITE(F,SEC_ID,4);
     IF (IORESULT<>0) THEN BEGIN Close(f);SalvarSecuenciaGeneral:=False; EXIT;END;

     {Escritura de identificador de Estructura}
     BLOCKWRITE(F,ID,SizeOF(WORD));
     IF (IORESULT<>0) THEN BEGIN Close(f);SalvarSecuenciaGeneral:=False; EXIT;END;

     {Escritura de tamanyo de estructura}
     Cantidad :=TamanyoSecuencia(S);
     BLOCKWRITE(F,Cantidad,SizeOF(LONGINT));
     IF (IORESULT<>0) THEN BEGIN Close(f);SalvarSecuenciaGeneral:=False; EXIT;END;

     {Escritura de la cantidad de elementos en la estructura}
     BLOCKWRITE(F,S^.Num,SizeOF(TIndexSec));
     IF (IORESULT<>0) THEN BEGIN Close(f);SalvarSecuenciaGeneral:=False; EXIT;END;

     {Escritura del contenido}
     BLOCKWRITE(F,S^.S,S^.Num*SizeOf(TPatternNum));
     IF (IORESULT<>0) THEN BEGIN Close(f);SalvarSecuenciaGeneral:=False; EXIT;END;
     SalvarSecuenciaGeneral:=True;
END;
{----------------------------------------------------------------------------}
FUNCTION TamanyoSecuencia(S:PSecuencia):LONGINT;
BEGIN
     TamanyoSecuencia := S^.Num*SizeOf(TPatternNum) + SizeOf(TIndexSec) +10;
END;
{----------------------------------------------------------------------------}