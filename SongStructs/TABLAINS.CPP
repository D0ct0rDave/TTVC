/*----------------------------------------------------------------------------*/
void CrearTablaIns(PTablaIns &T)
{
	TIndexTIns i;
     
	 if (T!=NULL)  DestruirTablaIns(T);
     T = new TTablaIns;

     // Inicializar la tabla de instrumentos
     for( i=0; i<= MaxTracks-1;i++)
     {
          T->T[i].Nombre= "";
          T->T[i].Disp	= 0;
          T->T[i].Banco	= 0;
          T->T[i].Inst	= -1;
          T->T[i].Canal	= 0;
          T->T[i].VolDef= DEF_TRK_VOL;
          T->T[i].Ctrl0	= false;
          T->T[i].Ctrl32= false;
     };
	 
	 T->T[0].Inst  = 0;
};
/*----------------------------------------------------------------------------*/
void DestruirTablaIns(PTablaIns &T)
{
     if (T!=NULL) 
     {
          delete(T);
          T=NULL;
     };
};
/*----------------------------------------------------------------------------*/
#define	TINS_ID "TINS"

bool CargarTablaInsGeneral(PTablaIns &T,FILE *F,unsigned short &ID)
{
	char TID[4];
	TIndexTIns Tam;
	int Cantidad;

     CrearTablaIns(T);

     /*Lectura del identificador*/
     BLOCKREAD(F,TID,4);
     if (IORESULT!=0)  { return(false);};
     if (strncmp(TID,TINS_ID,4))  { return(false); ;};

     /*Lectura de identificador de estructura*/
     BLOCKREAD(F,&ID,sizeof(unsigned short));
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};

     /*Lectura de la cantidad de unsigned chars*/
     BLOCKREAD(F,&Cantidad,sizeof(int));
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};

     /*Lectura de la cantidad*/
     BLOCKREAD(F,&Tam,sizeof(TIndexTIns));
     if (IORESULT!=0)  { return(false); ;};
     if (Tam>sizeof(TTablaIns))  Tam=sizeof(TTablaIns);

     /*Lectura del contenido*/
     BLOCKREAD(F,T,Tam);

     return(IORESULT==0);
};
/*------------------------------------------------------------------------------*/
bool SalvarTablaInsGeneral(PTablaIns T,FILE *F,unsigned short ID)
{

   TIndexTIns Tam;
   int Cantidad;

     /*Escritura del identificador*/
     BLOCKWRITE(F,TINS_ID,4);
     if (IORESULT!=0)  { return(false); ;};

     /*Escritura del identificador de estructura*/
     BLOCKWRITE(F,&ID,sizeof(unsigned short));
     if (IORESULT!=0)  { return(false); ;};

     /*Escritura de tamanyo de estructura*/
     Cantidad =TamanyoTablaIns(T);
     BLOCKWRITE(F,&Cantidad,sizeof(int));
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};

     /*Escritura de la cantidad de eventos en la estructura*/
     Tam=sizeof(TTablaIns);
     BLOCKWRITE(F,&Tam,sizeof(TIndexTIns));
     if (IORESULT!=0)  { return(false); ;};

     /*Escritura del contenido de la estructura*/
     BLOCKWRITE(F,T,sizeof(TTablaIns));

     return(IORESULT==0);
};
/*------------------------------------------------------------------------------*/
PInfoTrack LeerInfoDeTablaIns(PTablaIns T,TTrackNum Track)
{
     if (Track<MaxTracks)  return(&T->T[Track]); else return(NULL);
};
/*------------------------------------------------------------------------------*/
int TamanyoTablaIns(PTablaIns T)
{
     // 10 : id(4) + idstruct(2)+cantidad(4) 
     return( sizeof(TTablaIns) + sizeof(TIndexTIns) + 10 );
};
/*------------------------------------------------------------------------------*/

