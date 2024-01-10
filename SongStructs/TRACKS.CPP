/*----------------------------------------------------------------------------*/
void BorrarTrack(PTrack T)
{
     memset(T,0,sizeof(TTrack));
};
/*----------------------------------------------------------------------------*/
void CrearTrack(PTrack T)
{     // El puntero es un track creado que es "estático" (no mem.din.)
     BorrarTrack(T);
};
/*----------------------------------------------------------------------------*/
void CopiarTrack(PTrack Fuente,PTrack Destino)
{
   TBeatNum I;
     /*Tomamos como fuente este mismo Track*/
     for( I=0;I<= MaxBeats-1;I++) Destino->PE[I]=Fuente->PE[I];
     Destino->NumEventos = Fuente->NumEventos;
};
/*----------------------------------------------------------------------------*/
#define  TRACK_ID "TRAK"

bool CargarTrackGeneral(PTrack T,FILE *F,unsigned short &ID)
{
	char			TRK[4];
	TDatosEvento	Ev;
	TBeatNum		NumEvents,I;
	int			Cantidad;
	TBeatNum		Time;

     // Lectura del identificador
     BLOCKREAD(F,TRK,4);
     if (IORESULT!=0)  { return(false); ;};
     if (strncmp(TRK,TRACK_ID,4))  { return(false) ;};

     // Lectura del identificador de estructura
     BLOCKREAD(F,&ID,sizeof(ID));
     if (IORESULT!=0)  { return(false) ;};

     // Lectura de la cantidad de unsigned chars en el track
     BLOCKREAD(F,&Cantidad,sizeof(int));
     if (IORESULT!=0)  { return(false); ;};

     // Lectura de la cantidad de eventos que hay en el track
     BLOCKREAD(F,&NumEvents,sizeof(TBeatNum));
     if (IORESULT!=0)  { return(false) ;};

     // Lectura del contenido del track}
     for( I=0;I<=NumEvents-1;I++)
     {
          BLOCKREAD(F,&Time,sizeof(TBeatNum));
          BLOCKREAD(F,&Ev,sizeof(TDatosEvento));
          if (IORESULT!=0)  { return(false); };

          //Controlar si esta lleno el campo nota
          if ((Ev.Nota!=255) || (Ev.Eff!=0))
			InsertarEventoTrack(T,Time,Ev.Nota+1,Ev.Vol,Ev.Eff,Ev.Val);
     };

     return( TamanyoTrack(T) == Cantidad);
};
/*----------------------------------------------------------------------------*/
bool SalvarTrackGeneral(PTrack T,FILE *F,unsigned short ID)
{
   TBeatNum I;
   int Cantidad;

     // Escritura del identificador
     BLOCKWRITE(F,TRACK_ID,4);
     if (IORESULT!=0)  { return(false); ;};

     // Escritura del identificador de estructura
     BLOCKWRITE(F,&ID,sizeof(ID));
     if (IORESULT!=0)  { return(false); ;};

     // Escritura de la cantidad de unsigned chars en el track
     Cantidad=TamanyoTrack(T);
     BLOCKWRITE(F,&Cantidad,sizeof(int));
     if (IORESULT!=0)  { return(false); ;};

     // Escritura en numero de eventos
     BLOCKWRITE(F,&T->NumEventos,sizeof(TBeatNum));
     if (IORESULT!=0)  { return(false); ;};

     // Escritura del contenido del track
     for( I=0;I<=MaxBeats-1;I++)
     {
          if (EstaEventoTrack(T,I)) 
          {
               BLOCKWRITE(F,&I,sizeof(TBeatNum));  // Escribimos el tiempo de evento

               T->PE[I].Nota--;
               BLOCKWRITE(F,&T->PE[I],sizeof(TDatosEvento));
               T->PE[I].Nota++;

               if (IORESULT!=0)  { return(false); ;};
          };
     };

     return(true);
};
/*----------------------------------------------------------------------------*/
void InsertarPosicionTrack(PTrack T,TBeatNum B)
{
	TBeatNum I;
    if (EstaEventoTrack(T,MaxBeats-1)) /* Lo perderemos al insertar */ T->NumEventos--;

    for( I=MaxBeats-1;I>=B+1;I--)
	{
		T->PE[I] = T->PE[I-1];
		/* Inc(PE[I].Time);*/
	};
    
	memset(&T->PE[B],0,sizeof(TDatosEvento));
	T->PE[B].Nota=0;
};
/*----------------------------------------------------------------------------*/
void BorrarPosicionTrack(PTrack T,TBeatNum B)
{
	unsigned char I;
    if (EstaEventoTrack(T,B))  /* Lo perderemos al borrar */ T->NumEventos--;

    for( I=B; I<= MaxBeats-2;I++)
	{
		T->PE[I] = T->PE[I+1];
		/* Dec(PE[I].Time); */
	} 
	
    B=MaxBeats-1;
	memset(&T->PE[B],0,sizeof(TDatosEvento));
	T->PE[B].Nota = 0;
};
/*----------------------------------------------------------------------------*/
void InsertarPosicionesTrack(PTrack T,TBeatNum B,TBeatNum Num)
{
     // Abrir hueco
     // Limpiar posiciones insertadas
};
/*----------------------------------------------------------------------------*/
void BorrarPosicionesTrack(PTrack T,TBeatNum B,TBeatNum num)
{
     // Contraer pista
     // Limpiar nuevas posiciones
};
/*----------------------------------------------------------------------------*/
void InsertarEventoTrack(PTrack T,TBeatNum Time,unsigned char N,unsigned char V,unsigned char F,unsigned char A)
{
     if (Time<MaxBeats) 
     {
        if ((T->PE[Time].Nota==0) && (T->PE[Time].Eff==0))  T->NumEventos++;

		T->PE[Time].Nota = N; /* Rango de entrada de las notas: 1..96 */
		T->PE[Time].Vol  = V;
		T->PE[Time].Eff  = F;
		T->PE[Time].Val  = A;
     };
};
/*----------------------------------------------------------------------------*/
void EliminarEventoTrack(PTrack T,TBeatNum Time)
{
     if ((T->PE[Time].Nota!=0) || (T->PE[Time].Eff!=0) )
     {
          memset(&T->PE[Time],0,sizeof(TDatosEvento));
          T->NumEventos--;
     };
};
/*----------------------------------------------------------------------------*/
PDatosEvento TomarEventoPorTiempoTrack(PTrack T,TBeatNum Time)
{
     if (Time < MaxBeats)
     {
          if ((T->PE[Time].Nota!=0) || (T->PE[Time].Eff!=0))
              return( &T->PE[Time] );
          else
              return( NULL );
     }
     else
        return( NULL );
};
/*----------------------------------------------------------------------------*/
bool EstaEventoTrack(PTrack T,TBeatNum Time)
{
     if (Time < MaxBeats) 
        return ( (T->PE[Time].Nota!=0) || (T->PE[Time].Eff != 0) );
     else
        return( false );
};
/*----------------------------------------------------------------------------*/
bool TrackVacio(PTrack T)
{
	if (T==NULL)  { return( true ); };
    return( NumEventosTrack(T)==0 );
};
/*----------------------------------------------------------------------------*/
short NumEventosTrack(PTrack T)
{
	short I,R;

    R=0;
	for(I=0;I<=MaxBeats-1;I++)
		if ( *((int *)(&T->PE[I])) != 0) 
			R++;

     return( R );
};
/*----------------------------------------------------------------------------*/
int TamanyoTrack(PTrack T)
{
     return ( NumEventosTrack(T) * (sizeof(TDatosEvento)+sizeof(TBeatNum))+sizeof(unsigned short)+10 );
};
/*----------------------------------------------------------------------------*/
/* Tratamiento de los beats dentro del track                                  */
/*----------------------------------------------------------------------------*/
BeatString LeerBeatTrack(PTrack T,TBeatNum Beat)
{
	PDatosEvento D;
	BeatString S,AUX;	

	D = TomarEventoPorTiempoTrack(T,Beat);

	// Funcion muy importante para poder representar los datos en pantalla
	if (D == NULL) 
		// Pintar Vacio
		AUX = "___ ···          ···";
	else
	// Pintar con Algo
	{
		if (D->Nota == 0) 
			S="___";
		else
		{
			int iOct = ((D->Nota-1) / 12);
			char *Nota;
			char CNota[256];

			switch ((D->Nota-1) % 12)
			{
				case  0:Nota="C "; break;
				case  1:Nota="C#"; break;
				case  2:Nota="D "; break;
				case  3:Nota="D#"; break;
				case  4:Nota="E "; break;
				case  5:Nota="F "; break;
				case  6:Nota="F#"; break;
				case  7:Nota="G "; break;
				case  8:Nota="G#"; break;
				case  9:Nota="A "; break;
				case 10:Nota="A#"; break;
				case 11:Nota="B "; break;
			};

			sprintf(CNota,"%s%d",Nota,iOct);
			S = CNota;
		};

		AUX=S+" ";

		// OBTENCION DEL SEGUNDO CAMPO
		if (D->Nota==0) 
			S="···";
		else
		{
			char szString[4];
			sprintf(szString,"%3d",D->Vol);
			S = szString;
		}

		AUX = AUX+S+" ";

		// OBTENCION DEL TERCER CAMPO
		if (D->Eff<MaxEfectosPropios) 
		{
			S = ListaDeEfectos[D->Eff];

			if ( (S == "        ") && (D->Eff>0) )
			{
				char szString[4];
				sprintf(szString,"%4d",D->Eff);
				S = "Eff:";
				S = S + szString;
			};
		}
		else
			S = "-";

		AUX = AUX + S +" ";

		// OBTENCION DEL CUARTO CAMPO
		if (D->Eff==0)
			S="···";
		else
		{
			char szString[4];
			sprintf(szString,"%3d",D->Val);
			S = szString;
		}

		AUX=AUX+S;
	};

	return (AUX);
};
/*----------------------------------------------------------------------------*/
void EscribirBeatTrack(PTrack T,TBeatNum Beat,unsigned char Nota,unsigned char V,unsigned char F,unsigned char A)
{
     InsertarEventoTrack(T,Beat,Nota+1,V,F,A);
};
/*----------------------------------------------------------------------------*/
void EscribirNotaBeat(PTrack T,TBeatNum Beat,unsigned char Nota)
{
	PDatosEvento E;
    E=TomarEventoPorTiempoTrack(T,Beat);
    if (E!=NULL)  E->Nota=Nota+1; else InsertarEventoTrack(T,Beat,Nota+1,0,0,0);
};
/*----------------------------------------------------------------------------*/
void EscribirVolBeat(PTrack T,TBeatNum Beat,unsigned char Vol)
{
	PDatosEvento E;
    E=TomarEventoPorTiempoTrack(T,Beat);
    if (E!=NULL)  E->Vol=Vol;  else InsertarEventoTrack(T,Beat,0,Vol,0,0);
};
/*----------------------------------------------------------------------------*/
void EscribirEffBeat(PTrack T,TBeatNum Beat,unsigned char Eff)
{
	PDatosEvento E;
    E = TomarEventoPorTiempoTrack(T,Beat);
    if (E!=NULL)  E->Eff=Eff;  else InsertarEventoTrack(T,Beat,0,0,Eff,0);
};
/*----------------------------------------------------------------------------*/
void EscribirValBeat(PTrack T,TBeatNum Beat,unsigned char Val)
{
	PDatosEvento E;
    E=TomarEventoPorTiempoTrack(T,Beat);
    if (E!=NULL)  E->Val=Val;  else InsertarEventoTrack(T,Beat,0,0,0,Val);
};
/*----------------------------------------------------------------------------*/
unsigned char LeerNotaBeat(PTrack T,TBeatNum Beat)
{
	PDatosEvento E;
	E=TomarEventoPorTiempoTrack(T,Beat);
    if (E!=NULL)  return( E->Nota-1 ); else return(255);
};
/*----------------------------------------------------------------------------*/
unsigned char LeerVolBeat(PTrack T,TBeatNum Beat)
{
	PDatosEvento E;
	E=TomarEventoPorTiempoTrack(T,Beat);
	if (E!=NULL)  return(E->Vol); else return(0);
};
/*----------------------------------------------------------------------------*/
unsigned char LeerEffBeat(PTrack T,TBeatNum Beat)
{
	PDatosEvento E;
	E=TomarEventoPorTiempoTrack(T,Beat);
	if (E!=NULL)  return(E->Eff); else return(0);
};
/*----------------------------------------------------------------------------*/
unsigned char LeerValBeat(PTrack T,TBeatNum Beat)
{
	PDatosEvento E;
	E = TomarEventoPorTiempoTrack(T,Beat);
	if (E!=NULL)  return(E->Val); else return(0);
};
/*----------------------------------------------------------------------------*/
