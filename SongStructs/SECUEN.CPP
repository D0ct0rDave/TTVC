/*----------------------------------------------------------------------------*/
void CrearSecuencia(PSecuencia &S)
{
	if (S!=NULL)  DestruirSecuencia(S);
	S = new TSecuencia;
	memset(S,0,sizeof(TSecuencia));
	S->Num = 1;
};
/*----------------------------------------------------------------------------*/
void DestruirSecuencia(PSecuencia &S)
{
     if (S!=NULL) 
     {
          delete(S);
          S=NULL;
     };
};
/*----------------------------------------------------------------------------*/
void DesplazarDerecha(PSecuencia S,TIndexSec PActual)
{
	TIndexSec i;
     
	 // Funcion insertar
     for (i=S->Num;i>=PActual+1;i--) S->S[i]=S->S[i-1];
     S->Num++;
};
/*----------------------------------------------------------------------------*/
void DesplazarIzquierda(PSecuencia S,TIndexSec PActual)
{
	TIndexSec i;
    // Funcion borrar
     for (i=PActual;i<=S->Num-1-1;i++) S->S[i]=S->S[i+1];
     S->Num--;
};
/*----------------------------------------------------------------------------*/
bool InsertarPatronEnSecuencia(PSecuencia S,TIndexSec Pos)
{
     if (S->Num<MaxPatternsSequence) 
     {
          DesplazarDerecha(S,Pos);
          Error="";
		  return(true);
     }
     else
     {
          Error="Se ha alcanzado el máximo numero de posiciones de la secuencia";
          return(false);
     };
};
/*----------------------------------------------------------------------------*/
bool BorrarPatronDeSecuencia(PSecuencia S,TIndexSec & Pos)
{
     if (S->Num>1) 
     {
          DesplazarIzquierda(S,Pos);
          if (Pos>S->Num-1)  Pos=S->Num-1; /*?*/

          Error="";
		  return(true);
     }
     else
     {
         Error="No se puede borrar el único elemento de la secuencia";
		 return(false);
     };
};
/*----------------------------------------------------------------------------*/
TPatternNum LeerIndicePatronEnSecuencia(PSecuencia S,TIndexSec I)
{
     return( S->S[I] );
};
/*----------------------------------------------------------------------------*/
void EscribirIndicePatronDeSecuencia(PSecuencia S,TIndexSec I,TPatternNum P)
{
     S->S[I]=P;
};
/*----------------------------------------------------------------------------*/
TIndexSec NumPatronesEnSecuencia(PSecuencia S)
{
     return ( S->Num );
};
/*----------------------------------------------------------------------------*/
#define SEC_ID "SQNC"

bool CargarSecuenciaGeneral(PSecuencia &S,FILE * F,unsigned short & ID)
{
	TIndexSec	Tam;
	char		SID[4];
	int		Cantidad;

     // Lectura del identificador
     BLOCKREAD(F,SID,4);
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};
     if (strncmp(SID,SEC_ID,4))  { return(false); ;};

     // Lectura de identificador de estructura
     BLOCKREAD(F,&ID,sizeof(unsigned short));
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};

     // Lectura de la cantidad de unsigned chars
     BLOCKREAD(F,&Cantidad,sizeof(int));
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};

     // Lectura de la cantidad de eventos
     BLOCKREAD(F,&Tam,sizeof(TIndexSec));
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};

     CrearSecuencia(S);
     if (Tam<=MaxPatternsSequence)	S->Num=Tam; else S->Num=MaxPatternsSequence-1;

     // Lectura del contenido
     BLOCKREAD(F,S->S,S->Num*sizeof(TPatternNum));
     if (IORESULT!=0)  { CLOSE(F);return(false);};

     return(true);
};
/*----------------------------------------------------------------------------*/
bool SalvarSecuenciaGeneral(PSecuencia S,FILE *F,unsigned short ID)
{
	int Cantidad;

     // Escritura del identificador
     BLOCKWRITE(F,SEC_ID,4);
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};

     // Escritura de identificador de Estructura
     BLOCKWRITE(F,&ID,sizeof(unsigned short));
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};

     // Escritura de tamanyo de estructura
     Cantidad =TamanyoSecuencia(S);
     BLOCKWRITE(F,&Cantidad,sizeof(int));
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};

     // Escritura de la cantidad de elementos en la estructura
     BLOCKWRITE(F,&S->Num,sizeof(TIndexSec));
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};

     // Escritura del contenido
     BLOCKWRITE(F,S->S,S->Num*sizeof(TPatternNum));
     if (IORESULT!=0)  { CLOSE(F);return(false); ;};
     return(true);
};
/*----------------------------------------------------------------------------*/
int TamanyoSecuencia(PSecuencia S)
{
     return ( S->Num*sizeof(TPatternNum) + sizeof(TIndexSec) +10 );
};
/*----------------------------------------------------------------------------*/
