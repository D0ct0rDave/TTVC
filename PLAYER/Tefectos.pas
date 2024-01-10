UNIT TEfectos;

INTERFACE
CONST
     {Lista de los Efectos permitidos}
     MaxBytesEfecto    = 32;
     MaxEfectosPropios = 32;
     NumeroDeEfectos   = 14;
     MaxEfectos        = 256;
     MaxCarNombreEfecto = 08;
     MaxCarNombreConfig = 40;

TYPE
    PBufferSysEx = ^TBufferSysEx;
    TBufferSysEx = RECORD
                        Data:ARRAY[0..31] OF BYTE;
                        Num:Byte;
                  END;

    PEfectoMidi = ^TEfectoMIDI;
    TEfectoMIDI = record
                        Nombre:String[MaxCarNombreEfecto];
                        Datos:record
                                    b:ARRAY[0..MaxBytesEfecto-1] OF WORD;
                                    n:byte;
                              end;
                  end;

    PEfectosMidi = ^TEfectosMIDI;
    TEfectosMidi = OBJECT
                         NombreConfiguracion:String[MaxCarNombreConfig];

                         procedure Inicializar;
                         procedure EstablecerNombre(i:byte;Nombre:String);
                         function TomarNombreEfecto(i:byte):String;
                         function TomarDatosEfecto(I:BYTE):PEfectoMIDI;

                         function CargarTablaEfectos(var F:File):boolean;
                         function SalvarTablaEfectos(var F:File):boolean;

                         PROCEDURE ObtenerEfectoMIDI(CanalMIDI,EfectoMIDI,Valor:BYTE;Buf:PBufferSysEx);
                         public
                         E:Array[0..MaxEfectos-1] OF TEfectoMIDI;
                   END;

IMPLEMENTATION

{------------------------------------------------------------------------------}
PROCEDURE TEfectosMidi.Inicializar;
BEGIN
     NombreConfiguracion:='';
     FillChar(e,SizeOF(E),#0);

     WITH E[01] DO
     BEGIN
     Nombre:='Note Off';
     Datos.N := 3;
     Datos.B[0]:=$0180;
     Datos.B[1]:=$0200;
     Datos.B[2]:=$0000;
     END;

     E[02].Nombre:='EndOfMod';
     E[03].Nombre:='PatBreak';
     E[04].Nombre:='IncTempo';
     E[05].Nombre:='DecTempo';
     E[06].Nombre:='BPMS * 2';
     E[07].Nombre:='BPMS / 2';

     WITH E[09] DO
     BEGIN
     Nombre:='PrChange';
     Datos.N := 3;
     Datos.B[0]:=$01C0;
     Datos.B[1]:=$0200;
     Datos.B[2]:=$0000;
     END;

     WITH E[10] DO
     BEGIN
     Nombre:='Panning';
     Datos.N := 3;
     Datos.B[0]:=$01B0;
     Datos.B[1]:=$000A;
     Datos.B[2]:=$0200;
     END;

     WITH E[11] DO
     BEGIN
     Nombre:='Express';
     Datos.N := 3;
     Datos.B[0]:=$01B0;
     Datos.B[1]:=$000B;
     Datos.B[2]:=$0200;
     END;

     WITH E[12] DO
     BEGIN
     Nombre:='PitchHI';
     Datos.N := 3;
     Datos.B[0]:=$01E0;
     Datos.B[1]:=$0000;
     Datos.B[2]:=$0200;
     END;

     WITH E[13] DO
     BEGIN
     Nombre:='PitchLO';
     Datos.N := 3;
     Datos.B[0]:=$01E0;
     Datos.B[1]:=$0200;
     Datos.B[2]:=$0000;
     END;

     WITH E[14] DO
     BEGIN
     Nombre:='Reverb';
     Datos.N := 3;
     Datos.B[0]:=$01B0;
     Datos.B[1]:=$005B;
     Datos.B[2]:=$0200;
     END;

     WITH E[15] DO
     BEGIN
     Nombre:='Chorus';
     Datos.N := 3;
     Datos.B[0]:=$01B0;
     Datos.B[1]:=$005D;
     Datos.B[2]:=$0200;
     END;

     WITH E[16] DO
     BEGIN
     Nombre:='Modulat.';
     Datos.N := 3;
     Datos.B[0]:=$01B0;
     Datos.B[1]:=$0001;
     Datos.B[2]:=$0200;
     END;

     WITH E[17] DO
     BEGIN
     Nombre:='Ch.Press';
     Datos.N := 3;
     Datos.B[0]:=$01D0;
     Datos.B[1]:=$0200;
     Datos.B[2]:=$0000;
     END;
END;
{------------------------------------------------------------------------------}
PROCEDURE TEfectosMidi.EstablecerNombre(i:byte;Nombre:String);
BEGIN
     e[i].Nombre:=Nombre;
END;
{------------------------------------------------------------------------------}
FUNCTION TEfectosMidi.TomarNombreEfecto(i:byte):String;
var
   aux:string;
begin
     Aux:=e[i].Nombre;
     WHILE (Length(Aux)<MaxCarNombreEfecto) DO Aux:=Aux+' ';
     TomarNombreEfecto:=Aux;
end;
{------------------------------------------------------------------------------}
FUNCTION TEfectosMidi.TomarDatosEfecto(I:BYTE):PEfectoMIDI;
begin
     TomarDatosEfecto:=@e[i];
end;
{------------------------------------------------------------------------------}
FUNCTION TEfectosMidi.CargarTablaEfectos(VAR F:File):boolean;
CONST
     INS_ID :ARRAY[0..3] OF CHAR =('T','E','F','F');
VAR
   IID:ARRAY[0..3] OF CHAR;
   T:WORD;
BEGIN
     {Lectura del identificador}
     BLOCKREAD(F,IID,4);
     IF (IORESULT<>0) THEN BEGIN CargarTablaEfectos:=False;exit;END;
     IF (IID<>INS_ID) THEN BEGIN CargarTablaEfectos:=False;EXIT;END;

     {Lectura del nombre de la configuracion}
     BLOCKREAD(F,NombreConfiguracion,MaxCarNombreConfig+1);
     IF (IORESULT<>0) THEN BEGIN CargarTablaEfectos:=False;exit;END;

     {Lectura del numero de elementos}
     BLOCKREAD(F,T,2);
     IF (IORESULT<>0) THEN BEGIN CargarTablaEfectos:=False;exit;END;

     IF (T<MaxEfectos) THEN T:=MaxEfectos;
     BLOCKREAD(F,E,T*SizeOf(TEfectoMIDI));
     IF (IORESULT<>0) THEN BEGIN CargarTablaEfectos:=False;exit;END;

     CargarTablaEfectos:=True;
END;
{------------------------------------------------------------------------------}
FUNCTION TEfectosMidi.SalvarTablaEfectos(VAR F:File):boolean;
CONST
     INS_ID :ARRAY[0..3] OF CHAR =('T','E','F','F');
VAR
   T:WORD;
BEGIN
     {Escritura del identificador}
     BLOCKWRITE(F,INS_ID,4);
     IF (IORESULT<>0) THEN BEGIN SalvarTablaEfectos:=False;exit;END;

     {Escritura del nombre de la configuracion}
     BLOCKWRITE(F,NombreConfiguracion,MaxCarNombreConfig+1);
     IF (IORESULT<>0) THEN BEGIN SalvarTablaEfectos:=False;exit;END;

     {Escritura del numero de elementos}
     T:=MaxEfectos;
     BLOCKWRITE(F,T,2);
     IF (IORESULT<>0) THEN BEGIN SalvarTablaEfectos:=False;exit;END;

     BLOCKWRITE(F,E,T*SizeOf(TEfectoMIDI));
     IF (IORESULT<>0) THEN BEGIN SalvarTablaEfectos:=False;exit;END;

     SalvarTablaEfectos:=True;
END;
{------------------------------------------------------------------------------}
PROCEDURE TEfectosMidi.ObtenerEfectoMIDI(CanalMIDI,EfectoMIDI,Valor:BYTE;Buf:PBufferSysEx);
VAR
   R,I,J:INTEGER;
BEGIN
     {Este es el compilador: Procesa una estructura con variables, y la
     transforma en una secuencia de bytes a mandar directamente al Sist. MIDI}

     IF (E[EfectoMidi].Datos.N>0) AND (Buf<>nil) THEN
     BEGIN
          FOR i:=0 TO E[EfectoMidi].Datos.N-1 DO
          BEGIN
               CASE (E[EfectoMidi].Datos.B[i] SHR 8) OF
                    0:Buf^.Data[i]:=lo(E[EfectoMidi].Datos.b[i]);
                    1:Buf^.Data[i]:=lo(E[EfectoMidi].Datos.b[i]) OR CanalMIDI;
                    2:Buf^.Data[i]:=Valor;

                    3:BEGIN{Calculo del checksum}
                           r:=0;
                           FOR j:=5 TO i-1 DO r:=r+Buf^.Data[j];
                               Buf^.Data[i]:=128-(r MOD 128);
                      END;
               END;
          END;

          Buf^.Num:=E[EfectoMidi].Datos.N;
     END;

END;
{------------------------------------------------------------------------------}
END.
