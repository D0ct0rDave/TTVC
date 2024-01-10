PROGRAM Test;
USES
    Tipos,
    INTER{,
    Play};
CONST
     DirectorioDatos = 'D:\DATOS\MUSICA\SONGS\';
VAR
   C :PCancion;
   F :FILE;
   ID:WORD;

BEGIN
     CrearCancion(C);
     {Cargamos patron 0}

     Assign(F,DirectorioDatos+'P0.PAT');
     Reset(F,1);
     CargarPattern(C^.Pats[0],F,ID);
     Close(F);

     {Cargamos patron 1}
     Assign(F,DirectorioDatos+'P1.PAT');
     Reset(F,1);
     CargarPattern(C^.Pats[1],F,ID);
     Close(F);

     Assign(F,DirectorioDatos+'TI.INS');
     Reset(F,1);
     CargarTablaIns(C^.TIns,F,ID);
     Close(F);

     InsertarPatronEnSecuencia(C^.Seq,0);
     InsertarPatronEnSecuencia(C^.Seq,0);
     InsertarPatronEnSecuencia(C^.Seq,0);

     EscribirIndicePatronDeSecuencia(C^.Seq,0,0);
     EscribirIndicePatronDeSecuencia(C^.Seq,1,1);
     EscribirIndicePatronDeSecuencia(C^.Seq,2,0);
     EscribirIndicePatronDeSecuencia(C^.Seq,3,1);

     {EstablecerEstadoReproduccion(0,0);
     PlayCancion(C);

     REPEAT UNTIL NOT Reproduciendo;}

     Assign(F,DirectorioDatos+'A.TCK');
     Rewrite(F,1);
     SalvarCancion(C,F);
     Close(F);

     DestruirCancion(C);
     {Finalizacion;}
END.
