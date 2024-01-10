PROGRAM Test;
USES
    Tipos,
    INTER,
    Play;

VAR
   P :pPattern;
   TI:pTablaIns;
   F:FILE;
   ID:WORD;

BEGIN
     Assign(F,'D:\songs\p1.pat');
     Reset(F,1);
     CargarPattern(P,F,ID);
     Close(F);

     Assign(F,'D:\songs\ti.ins');
     Reset(F,1);
     CargarTablaIns(TI,F,ID);
     Close(F);

     PlayPattern(P,TI);
     REPEAT UNTIL NOT Reproduciendo;

     DestruirPattern(P);
     DestruirTablaIns(TI);

     Finalizacion;
END.
