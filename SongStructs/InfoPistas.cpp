//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#include "InfoPistas.h"
#include <string.h>
/*
//  WARNING
void TTablaIns::InicializarTablaIns()
{
   unsigned char i;

     //  Tabla de instrumentos por track
     for (i=0;i<=MaxTracks-1;i++)
     {
          T[i].Nombre="";
          T[i].Disp  =0;
          T[i].Banco =0;
          T[i].Inst  =255;
          T[i].Canal =0;
          T[i].VolDef=64;
          T[i].Ctrl0 =false;
          T[i].Ctrl32=false;
     };
};

#define     TINS_ID "TINS"

bool TTablaIns::CargarTablaIns(FILE *F)
{
   char TID[4];
   unsigned short Tam;

     //  Lectura del identificador
     BLOCKREAD(F,&TID,4);
     if (IORESULT!=0)  return(false);
     if (strncmp(TID,TINS_ID,4)) return(false);

     //  Lectura de la cantidad
     BLOCKREAD(F,&Tam,2);
     if (IORESULT!=0)return(false);

     //  Lectura del contenido
     if (Tam>sizeof(T))  Tam=sizeof(T);
     BLOCKREAD(F,&T,Tam);
     if (IORESULT!=0)return(false);
     return(true);
};
// ------------------------------------------------------------------------------
bool TTablaIns::SalvarTablaIns(FILE *F)
{
   unsigned short Tam;

     // Escritura del identificador
     BLOCKWRITE(F,TINS_ID,4);
     if (IORESULT!=0) return(false);

     // Escritura de la cantidad de eventos en el track
     Tam=sizeof(T);
     BLOCKWRITE(F,&Tam,2);
     if (IORESULT!=0) return(false);

     // Escritura del contenido del track
     BLOCKWRITE(F,&T,sizeof(T));
     if (IORESULT!=0) return(false);
     return(true);
};
*/
// ------------------------------------------------------------------------------
void TInstrumentosMIDI::Inicializar()
{

};

bool TInstrumentosMIDI::CargarTablaInstrumentosMIDI(FILE *F)
{
   unsigned short T;
     // Lectura del numero de elementos
     BLOCKREAD(F,&T,2);
     if (IORESULT!=0) return(false);

     if (T<MaxInstrumentosMIDI)  T=MaxInstrumentosMIDI;
     BLOCKREAD(F,&Ins,T*sizeof(TNombreInstrumentoMIDI));
     if (IORESULT!=0) return(false);

     return(true);
};

bool TInstrumentosMIDI::SalvarTablaInstrumentosMIDI(FILE *F)
{
   unsigned short T;

     // Escritura del numero de elementos
     T=MaxInstrumentosMIDI;
     BLOCKWRITE(F,&T,2);
     if (IORESULT!=0) return(false);

     BLOCKWRITE(F,&Ins,T*sizeof(TNombreInstrumentoMIDI));
     if (IORESULT!=0) return(false);

     return(true);
};
// ------------------------------------------------------------------------------
