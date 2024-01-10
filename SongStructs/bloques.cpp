//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------

#include "Bloques.h"
#include <string.h>

void CopiarBloque(PT98Pattern F,PBloque D,unsigned short BI,unsigned short BF,unsigned short TI,unsigned short TF)
{
    unsigned short i,j;
    PDatosEvento E;

     // Inicializar Patron destiono
     CrearPattern(D->bl);

     for (j=TI;j<=TF;j++)
         for (i=BI;i<=BF;i++)
            if (EstaEventoTrack(&(F->Tracks[j]),i))
            {
                E=TomarEventoPorTiempoTrack(&(F->Tracks[j]),i);
                EscribirBeatTrack(&(D->bl->Tracks[j-TI]),i-BI,E->Nota-1,E->Vol,E->Eff,E->Val);
            };

     D->NumTracks = TF-TI+1;
     D->NumBeats  = BF-BI+1;
};

void CortarBloque(PT98Pattern F,PBloque D,unsigned short BI,unsigned short BF,unsigned short TI,unsigned short TF)
{
     CopiarBloque(F,D,BI,BF,TI,TF);
     BorrarBloque(F,BI,BF,TI,TF);
};

void PegarBloque (PT98Pattern D,PBloque F,unsigned short BI,unsigned short TI)
{
   unsigned short   i,j;
   PDatosEvento     E;

     // Inicializar Patron destiono
     for (j=TI;j<=TI+F->NumTracks-1;j++)
         for (i=BI;i<= BI+F->NumBeats-1;i++)
         {
              if ((i<MaxBeats) && (j<MaxTracks))
              {
                   if (EstaEventoTrack(&(D->Tracks[j]),i))
                      EliminarEventoTrack(&(D->Tracks[j]),i);

                   if (EstaEventoTrack(&(F->bl->Tracks[j-TI]),i-BI))
                   {
                        E=TomarEventoPorTiempoTrack(&(F->bl->Tracks[j-TI]),i-BI);
                        EscribirBeatTrack(&(D->Tracks[j]),i,E->Nota-1,E->Vol,E->Eff,E->Val);
                   };
              };
         };
};

void BorrarBloque(PT98Pattern F,unsigned short BI,unsigned short BF,unsigned short TI,unsigned short TF)
{
   unsigned short j,i;

     for (j=TI;j <= TF;j++)
         for (i=BI;i<= BF;i++)
             if (EstaEventoTrack(&(F->Tracks[j]),i))
                EliminarEventoTrack(&(F->Tracks[j]),i);
};
//----------------------------------------------------------------------------
void CrearBloque(PBloque &B)
{
     if (B!=NULL)  DestruirBloque(B);
     B = new(TBloque);

     memset(B,0,sizeof(TBloque));
};
//----------------------------------------------------------------------------
void DestruirBloque(PBloque &B)
{
     if (B!=NULL)  delete(B);
     B=NULL;
};
//----------------------------------------------------------------------------
#define Bloque_ID "BLCK"

bool CargarBloqueGeneral(PBloque &B,FILE *F,unsigned short &ID)
{
   char PTR[4];
   int Pos,N,Cantidad;


     CrearBloque(B);

     /*Lectura del identificador*/
     BLOCKREAD(F,PTR,4);
     if (IORESULT!=0)  { return(false); ;};
     if (strncmp(PTR,Bloque_ID,4))  { return(false); ;};

     /*Lectura del identificador de estructura*/
     BLOCKREAD(F,&ID,sizeof(unsigned short));
     if (IORESULT!=0)  { return(false); ;};

     /*Lectura del tama¤o de la estructura*/
     BLOCKREAD(F,&N,sizeof(int));
     if (IORESULT!=0) { return(false); ;};

     BLOCKREAD(F,&B->NumTracks,sizeof(unsigned short));
     if (IORESULT!=0) { return(false); ;};
     if (B->NumTracks > MaxTracks)  B->NumTracks = MaxTracks;

     BLOCKREAD(F,&B->NumBeats,sizeof(unsigned short));
     if (IORESULT!=0){ return(false); ;};
     if (B->NumBeats > MaxBeats)  B->NumBeats = MaxBeats;

     if (! CargarPatternGeneral(B->bl,F,ID)){ return(false); ;};

     return ( TamanyoBloque(B) == N );
};
//----------------------------------------------------------------------------
bool SalvarBloqueGeneral(PBloque  B,FILE *F,unsigned short  ID)
{
    int N;

     /*Escritura del identificador*/
     BLOCKWRITE(F,Bloque_ID,4);
     if (IORESULT!=0){ return(false); ;};

     /*Escritura del identificador de estructura*/
     BLOCKWRITE(F,&ID,sizeof(unsigned short));
     if (IORESULT!=0){ return(false); ;};

     /*Escritura del bloque de info que viene despues*/
     N=TamanyoBloque(B);
     BLOCKWRITE(F,&N,sizeof(int));
     if (IORESULT!=0){ return(false); ;};

     /*Escritura de las dimensiones del bloque*/
     BLOCKWRITE(F,&B->NumTracks,sizeof(unsigned short));
     if (IORESULT!=0){ return(false); ;};

     BLOCKWRITE(F,&B->NumBeats,sizeof(unsigned short));
     if (IORESULT!=0){ return(false); ;};

     if (! SalvarPatternGeneral(B->bl,F,65535)) { return(false); ;};

     return(true);
};
//----------------------------------------------------------------------------
int TamanyoBloque(PBloque B)
{
     return (TamanyoPattern(B->bl) + 2*sizeof(unsigned short) + 10 );
};
//----------------------------------------------------------------------------
