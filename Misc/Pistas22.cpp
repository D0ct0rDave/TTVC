//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#include "Pistas22.h"
#include <stdlib.h>
#include "consts2.h"
/*-------------------------------------------------------------------------------*/
short CalcularTrasposicionPista(PTrack T,TBeatNum Desde,TBeatNum Hasta,short Semitonos)
{
   short R;
   TBeatNum I,Cont;
   PDatosEvento E;

     Cont=0;
     for (I=Desde;I<=Hasta;I++)
     {
          E=TomarEventoPorTiempoTrack(T,I);

          if (E!=NULL)
          {
               if (E->Nota>0)
               {
                    R=E->Nota+Semitonos;
                    if ((R<0) || (R>95))  Cont++;
               };
          };
     };

     return(Cont);
};

void  TrasponerPista(PTrack T,TBeatNum Desde,TBeatNum Hasta,short Semitonos)
{
   short R;
   TBeatNum I;
   PDatosEvento E;

     for (I=Desde;I<=Hasta;I++)
     {
          E=TomarEventoPorTiempoTrack(T,I);

          if (E!=NULL)
          {
               if (E->Nota>0)
               {
                    R=E->Nota+Semitonos;
                    if (R<0)  R=0; else if (R>95) R=95;
                    E->Nota=R;
               };
          };
     };
};
/*-------------------------------------------------------------------------------*/
short CalcularVolumenPista(PTrack T,TBeatNum Desde,TBeatNum Hasta,unsigned char Tipo,short v1,short v2)
{
   short R;
   TBeatNum I,Cont;
   PDatosEvento E;

     Cont=0;
     for (I=Desde;I<=Hasta;I++)
     {
          E=TomarEventoPorTiempoTrack(T,I);

          if (E!=NULL)
          {
               if (E->Nota>0)
               {
                    switch (Tipo)
                    {
                      case 1:
                      {
                          R=E->Vol+v1;
                          if ((R<0) || (R>127)) Cont++;
                      };
                      break;

                      case 2:
                      {
                          if (short(E->Vol)*v1 > 127) Cont++;
                      }
                      break;
                    };
               };
          };
     };

     return(Cont);
};

void ModificarVolumenPista(PTrack T,TBeatNum Desde,TBeatNum Hasta,unsigned char Tipo,short v1,short v2)
{
   short R;
   TBeatNum I;
   PDatosEvento E;
   float Step;

     // Randomize; // Para tipo=4;
     if (Hasta>Desde)  Step=(float)(v2-v1)/(float)(Hasta-Desde); else Step=0; /*Controlar DIV 0*/

     for (I=Desde;I<=Hasta;I++)
     {
          E=TomarEventoPorTiempoTrack(T,I);

          if (E!=NULL)
          {
               if (E->Nota>0)
               {
                    switch (Tipo)
                    {
                      case 0:
                      {
                        E->Vol=v1;
                      }
                      break;

                      case 1:
                      {
                          R=E->Vol+v1;
                          if (R<0)  R=0; else if (R>127)  R=127;
                          E->Vol = R;
                      };
                      break;

                      case 2:
                      {
                          R = E->Vol*v1;
                          if (R>127) R = 127;
                          E->Vol = R;
                      }
                      break;

                      /*   Y    =        M  (   X   -  x0  ) + B*/
                      case 3:
                      {
                          E->Vol = int(Step*(I-Desde)) + v1;
                      }
                      break;

                      /*Random*/
                      case 4:
                      {
                          E->Vol = v1 + (rand() % (v2-v1));
                      }
                      break;
                    };
               };
          };
     };
};
/*-------------------------------------------------------------------------------*/
short CalcularEfectoPista(PTrack T,TBeatNum Desde,TBeatNum Hasta,unsigned char Eff,unsigned char Tipo,short v1,short v2)
{
   short        R;
   TBeatNum     I,Cont;
   PDatosEvento E;

     Cont=0;

     if ((Eff!=0) && (Tipo!=0) && (Tipo!=3))
     {
       for (I=Desde;I<=Hasta;I++)
       {
           E=TomarEventoPorTiempoTrack(T,I);

           if (E!=NULL)
           {
                switch (Tipo)
                {
                    case 1:{
                           R=E->Val+v1;
                           if ((R<0) || (R>127)) Cont++;
                      };
                    break;

                    case 2:if (short(E->Val)*v1>127)  Cont++;
                    break;
                };
           };
        };
    };

    return(Cont);
};

void ModificarEfectoPista(PTrack T,TBeatNum Desde,TBeatNum Hasta,unsigned char Eff,unsigned char Tipo,short v1,short v2)
{
    short R;
    TBeatNum I;
    PDatosEvento E;
    float Step;

    // Randomize;   // Para tipo=4;
    if (Hasta>Desde)  Step=float(v2-v1)/float(Hasta-Desde); else Step=0; /*Controlar DIV 0*/

    if (Eff!=0)
    {
        for (I=Desde;I<=Hasta;I++)
        {
            E=TomarEventoPorTiempoTrack(T,I);

            if (E!=NULL)
            {
                E->Eff=Eff;

                switch (Tipo)
                {
                    case 0:
                    {
                        E->Val=v1;
                    }
                    break;

                    case 1:
                    {
                        R=E->Val+v1;
                        if (R<0)  R=0; else if (R>127)  R=127;
                        E->Val=R;
                    };
                    break;

                    case 2:
                    {
                        if (short(E->Val)*v1>127)  E->Val=127; else E->Val=E->Val*v1;
                    }
                    break;

                    /*   Y    =        M  (   X   -  x0  ) + B*/
                    case 3:
                    {
                        E->Val=int(Step*(I-Desde))+v1;
                    }
                    break;

                    case 4:
                    {
                    /*Random*/
                        E->Val=v1 + (rand() % (v2-v1));
                    }
                };
            };
        };
    };
};
/*-------------------------------------------------------------------------------*/
short CalcularVolumenPistaCancion(PCancion S,TTrackNum Track,TBeatNum Desde,TBeatNum Hasta,unsigned char Tipo,short v1,short v2)
{
    TPatternNum I;
    short R;

    R=0;
    for (I=0;I<= MaxPatterns-1;I++)
        if (S->Pats[I]!=NULL)
            if (! TrackVacio(LeerTrackDePattern(S->Pats[I],Track)))
                R=R+CalcularVolumenPista(LeerTrackDePattern(S->Pats[I],Track),Desde,Hasta,Tipo,v1,v2);

    return(R);
};

void ModificarVolumenPistaCancion(PCancion S,TTrackNum Track,TBeatNum Desde,TBeatNum Hasta,unsigned char Tipo,short v1,short v2)
{
    TPatternNum I;

    for (I=0;I<= MaxPatterns-1;I++)
        if (S->Pats[I]!=NULL)
            if (! TrackVacio(LeerTrackDePattern(S->Pats[I],Track)))
                ModificarVolumenPista(LeerTrackDePattern(S->Pats[I],Track),Desde,Hasta,Tipo,v1,v2);
};

short CalcularTrasposicionPistaCancion(PCancion S,TTrackNum Track,TBeatNum Desde,TBeatNum Hasta,short Semitonos)
{
    TPatternNum I;
    short R;

    R=0;
    for (I=0;I<= MaxPatterns-1;I++)
        if (S->Pats[I]!=NULL)
            if (! TrackVacio(LeerTrackDePattern(S->Pats[I],Track)))
                R=R+CalcularTrasposicionPista(LeerTrackDePattern(S->Pats[I],Track),Desde,Hasta,Semitonos);

    return(R);
};

void TrasponerPistaCancion(PCancion S,TTrackNum Track,TBeatNum Desde,TBeatNum Hasta, short Semitonos)
{
    TPatternNum I;

    for (I=0;I<= MaxPatterns-1;I++)
        if (S->Pats[I]!=NULL)
            if (! TrackVacio(LeerTrackDePattern(S->Pats[I],Track)))
                TrasponerPista(LeerTrackDePattern(S->Pats[I],Track),Desde,Hasta,Semitonos);
};
/*------------------------------------------------------------------------------*/
