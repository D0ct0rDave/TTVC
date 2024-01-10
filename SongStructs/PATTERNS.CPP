/*Indices:
         Cuando tiene un valor establecido !=65535 de!a el puesto que
         ocupa dentro de un grupo de estructuras del mismo tipo        */

/*----------------------------------------------------------------------------*/
void CrearPattern(PT98Pattern &P)
{
     if (P!=NULL)  DestruirPattern(P);
     P = new(TT98Pattern);

     memset(P,0,sizeof(TT98Pattern));
};
/*----------------------------------------------------------------------------*/
void DestruirPattern(PT98Pattern &P)
{
     if(P!=NULL) delete(P);
     P=NULL;
};
/*----------------------------------------------------------------------------*/
void CopiarPattern(PT98Pattern F,PT98Pattern D)
{
	TTrackNum I;
    for(I=0;I<=MaxTracks-1;I++) CopiarTrack(&F->Tracks[I],&D->Tracks[I]);
};
/*----------------------------------------------------------------------------*/
void BorrarPattern(PT98Pattern P)
{
	memset(P,0,sizeof(TT98Pattern));
};
/*----------------------------------------------------------------------------*/
#define PATTERN_ID "PTRN"

bool CargarPatternGeneral(PT98Pattern &P,FILE *F,unsigned short & ID)
{
   char			PTR[4];
   TTrackNum	I,M,IdentTrk;
   int		Pos,N,Cantidad;

     CrearPattern(P);

     /*Lectura del identificador*/
     BLOCKREAD(F,PTR,4);
     if (IORESULT!=0)  { return(false); };
     if (strncmp(PTR,PATTERN_ID,4))  { return(false); };

     /*Lectura del identificador de estructura*/
     BLOCKREAD(F,&ID,sizeof(ID));
     if (IORESULT!=0)  { return(false); ;};

     /*Lectura del tama¤o de la estructura*/
     BLOCKREAD(F,&N,sizeof(int));
     if (IORESULT!=0)  { return(false); ;};

     /*Lectura del nombre asociado al pattern*/
     BLOCKREAD(F,&P->Nombre,MaxCarNombrePatron+1);
     if (IORESULT!=0)  { return(false); ;};

     /*Leer numero de tracks en pattern*/
     BLOCKREAD(F,&M,sizeof(unsigned short));
     if (IORESULT!=0)  { return(false); ;};

     Pos=FILEPOS(F);
     for( I=0;I<=M-1;I++)
     {
          /*----------------------------*/
          /*Lectura de info de estuctura*/
          /*----------------------------*/
          BLOCKREAD(F,PTR,4);
          if (IORESULT!=0)  { return(false); ;};

          BLOCKREAD(F,&IdentTrk,sizeof(unsigned short));
          if (IORESULT!=0)  { return(false); ;};

          BLOCKREAD(F,&Cantidad,sizeof(int));
          if (IORESULT!=0)  { return(false); ;};
          /*----------------------------*/

          SEEK(F,Pos);
          Pos=Pos+Cantidad;

          if( !strncmp(PTR,"TRAK",4)) 
          {
               CargarTrackGeneral(&(P->Tracks[IdentTrk]),F,IdentTrk);
          }
		  else
		  {

          };

          SEEK(F,Pos);
     };

     return ( TamanyoPattern(P) == N);
};
/*----------------------------------------------------------------------------*/
bool SalvarPatternGeneral(PT98Pattern P,FILE *F,unsigned short ID)
{
	int		N;
	TPatternNum I;

     /*Escritura del identificador*/
     BLOCKWRITE(F,PATTERN_ID,4);
     if( IORESULT!=0)  { return(false); ;};

     /*Escritura del identificador de estructura*/
     BLOCKWRITE(F,&ID,sizeof(unsigned short));
     if( IORESULT!=0)  { return(false); ;};

     /*Escritura del bloque de info que viene despues:Nombre + InfoTracks*/
     N=TamanyoPattern(P);
     BLOCKWRITE(F,&N,sizeof(int));
     if( IORESULT!=0)  { return(false); ;};

     /*Escritura del nombre asociado al pattern*/
     BLOCKWRITE(F,&P->Nombre,MaxCarNombrePatron+1);
     if( IORESULT!=0)  { return(false); ;};

     /*Escritura del numero de eventos (Tracks) en el pattern*/
     I=NumeroDeTracksEnPattern(P);
     BLOCKWRITE(F,&I,sizeof(unsigned short));
     if( IORESULT!=0)  { return(false); ;};

     for( I=0 ;I<=MaxTracks-1;I++)
         if(! TrackVacio(&(P->Tracks[I]))) 
         {
              if( ! SalvarTrackGeneral(&P->Tracks[I],F,I) )
              {
                   return(false);
                   ;
              };
         };

     return(true);
};
/*----------------------------------------------------------------------------*/
PTrack LeerTrackDePattern(PT98Pattern P,TTrackNum I)
{
     return (& P->Tracks[I]);
}
/*----------------------------------------------------------------------------*/
TTrackNum NumeroDeTracksEnPattern(PT98Pattern P)
{
	TTrackNum i,n;

     n=0;
     for( i=0;i<=MaxTracks-1;i++) if (! TrackVacio(&(P->Tracks[i]))) n++;
     return(n);
};
/*----------------------------------------------------------------------------*/
int TamanyoPattern(PT98Pattern P)
{
	short I,R;

    R=0;
     for( I=0;I<=MaxTracks-1;I++)
        if( ! TrackVacio(&(P->Tracks[I])) )
           R=R+TamanyoTrack(&(P->Tracks[I])); /*Tamanyo en tracks*/

     return(R+MaxCarNombrePatron+1+sizeof(unsigned short)+10);
};
/*----------------------------------------------------------------------------*/
tt98String LeerNombreDelPattern(PT98Pattern P)
{
     return( tt98String(P->Nombre.CString()) );
};
/*----------------------------------------------------------------------------*/
void EscribirNombreDelPattern(PT98Pattern P,String Nombre)
{
     P->Nombre = String80(Nombre.CString());
};
/*----------------------------------------------------------------------------*/
bool PatternVacio(PT98Pattern P)
{
   TTrackNum I;
   bool V;

     if (P==NULL)  { return( true );;};

     I=0;
     V=TrackVacio(&(P->Tracks[I]));

     while( (V) && (I<MaxTracks-1))
     {
          I++;
          V=TrackVacio(&(P->Tracks[I]));
     };
     return(V);
};
/*----------------------------------------------------------------------------*/
