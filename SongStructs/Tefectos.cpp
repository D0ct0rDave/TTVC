//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#include "TEfectos.h"

#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------
void TEfectosMIDI::Inicializar()
{
     NombreConfiguracion="";
     memset(E,0,sizeof(E));

     E[1].Nombre="Note Off";
     E[1].Datos.N = 3;
     E[1].Datos.B[0]=0x0180;
     E[1].Datos.B[1]=0x0200;
     E[1].Datos.B[2]=0x0000;

     E[2].Nombre="EndOfMod";
     E[3].Nombre="PatBreak";
     E[4].Nombre="IncTempo";
     E[5].Nombre="DecTempo";
     E[6].Nombre="BPMS * 2";
     E[7].Nombre="BPMS / 2";

     E[9].Nombre="PrChange";
     E[9].Datos.N = 3;
     E[9].Datos.B[0]=0x01C0;
     E[9].Datos.B[1]=0x0200;
     E[9].Datos.B[2]=0x0000;

     E[10].Nombre="Panning";
     E[10].Datos.N = 3;
     E[10].Datos.B[0]=0x01B0;
     E[10].Datos.B[1]=0x000A;
     E[10].Datos.B[2]=0x0200;

     E[11].Nombre="Express";
     E[11].Datos.N = 3;
     E[11].Datos.B[0]=0x01B0;
     E[11].Datos.B[1]=0x000B;
     E[11].Datos.B[2]=0x0200;

     E[12].Nombre="PitchHI";
     E[12].Datos.N = 3;
     E[12].Datos.B[0]=0x01E0;
     E[12].Datos.B[1]=0x0000;
     E[12].Datos.B[2]=0x0200;

     E[13].Nombre="PitchLO";
     E[13].Datos.N = 3;
     E[13].Datos.B[0]=0x01E0;
     E[13].Datos.B[1]=0x0200;
     E[13].Datos.B[2]=0x0000;

     E[14].Nombre="Reverb";
     E[14].Datos.N = 3;
     E[14].Datos.B[0]=0x01B0;
     E[14].Datos.B[1]=0x005B;
     E[14].Datos.B[2]=0x0200;

     E[15].Nombre="Chorus";
     E[15].Datos.N = 3;
     E[15].Datos.B[0]=0x01B0;
     E[15].Datos.B[1]=0x005D;
     E[15].Datos.B[2]=0x0200;

     E[16].Nombre="Modulat.";
     E[16].Datos.N = 3;
     E[16].Datos.B[0]=0x01B0;
     E[16].Datos.B[1]=0x0001;
     E[16].Datos.B[2]=0x0200;

     E[17].Nombre="Ch.Press";
     E[17].Datos.N = 3;
     E[17].Datos.B[0]=0x01D0;
     E[17].Datos.B[1]=0x0200;
     E[17].Datos.B[2]=0x0000;
};
/*------------------------------------------------------------------------------*/
void TEfectosMIDI::EstablecerNombre(unsigned char i,String35 Nombre)
{
     E[i].Nombre=Nombre;
};
/*------------------------------------------------------------------------------*/
String35 TEfectosMIDI::TomarNombreEfecto(unsigned char i)
{
   String35 Aux;
     Aux=E[i].Nombre;
     // WARNING: DMC
     /*
     while (Aux < MaxCarNombreEfecto) Aux=Aux+" ";
     */
     return(Aux);
};
/*------------------------------------------------------------------------------*/
PEfectoMIDI TEfectosMIDI::TomarDatosEfecto(unsigned char i)
{
     return( &E[i] );
};
/*------------------------------------------------------------------------------*/
#define INS_ID "TEFF"

bool TEfectosMIDI::CargarTablaEfectos(FILE *F)
{
   char IID[4];
   unsigned short T;

     /*Lectura del identificador*/
     BLOCKREAD(F,&IID,4);
     if (IORESULT!=0) return(false);
     if (strncmp(IID,INS_ID,4)) return(false);

     /*Lectura del nombre de la configuracion*/
     BLOCKREAD(F,&NombreConfiguracion,MaxCarNombreConfig+1);
     if (IORESULT!=0)return(false);

     /*Lectura del numero de elementos*/
     BLOCKREAD(F,&T,2);
     if (IORESULT!=0)return(false);

     if (T<MaxEfectos) T=MaxEfectos;
     BLOCKREAD(F,&E,T*sizeof(TEfectoMIDI));
     if (IORESULT!=0)return(false);

     return(true);
};
/*------------------------------------------------------------------------------*/
bool TEfectosMIDI::SalvarTablaEfectos(FILE *F)
{
    unsigned short T;

     /*Escritura del identificador*/
     BLOCKWRITE(F,INS_ID,4);
     if (IORESULT!=0) return(false);

     /*Escritura del nombre de la configuracion*/
     BLOCKWRITE(F,&NombreConfiguracion,MaxCarNombreConfig+1);
     if (IORESULT!=0) return(false);

     /*Escritura del numero de elementos*/
     T=MaxEfectos;
     BLOCKWRITE(F,&T,2);
     if (IORESULT!=0) return(false);

     BLOCKWRITE(F,E,T*sizeof(TEfectoMIDI));
     if (IORESULT!=0) return(false);

     return(true);
};
/*------------------------------------------------------------------------------*/
void TEfectosMIDI::ObtenerEfectoMIDI(unsigned char CanalMIDI,unsigned char EfectoMIDI,unsigned char Valor,PBufferSysEx Buf)
{
   short r,i,j;
     /*Este es el compilador: Procesa una estructura con variables, y la
     transforma en una secuencia de bytes a mandar directamente al Sist. MIDI*/

     if ((E[EfectoMIDI].Datos.N>0) && (Buf!=NULL) )
     {
        for (i=0;i<= E[EfectoMIDI].Datos.N-1;i++)
        {
            switch (E[EfectoMIDI].Datos.B[i] >> 8)
            {
                  case 0:
                  Buf->Data[i] = E[EfectoMIDI].Datos.B[i] & 0x00ff;
                  break;

                  case 1:
                  Buf->Data[i] = (E[EfectoMIDI].Datos.B[i] & 0x00ff) | CanalMIDI;
                  break;

                  case 2:
                  Buf->Data[i]=Valor;
                  break;

                  case 3:
                  {
                    // Calculo del checksum
                    r=0;
                    for (j=5;j<=i-1;j++) r = r + Buf->Data[j];
                        Buf->Data[i]=128 - (r % 128);
                  };
                  break;
            };
        };

        Buf->Num=E[EfectoMIDI].Datos.N;
     };
};
/*------------------------------------------------------------------------------*/
