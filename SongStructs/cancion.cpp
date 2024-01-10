//------------------------------------------------------------------------------
void CrearCancion(PCancion &C)
{
	if (C != NULL) DestruirCancion(C);
	C = new TCancion;
	memset(C,0,sizeof(TCancion));			// Llenamos todo de 0's

	C->Titulo = TITULO_POR_DEFECTO;
	C->Autor  = AUTOR_POR_DEFECTO;
	C->BPMS   = BPMS_POR_DEFECTO;

	// En este modelo de estructura, solo se crean patrones a medida que
    // se van utilizando. El primer patron es el unico que se crea
	CrearPattern	(C->Pats[0]);
    CrearTablaIns	(C->TIns);
    CrearSecuencia	(C->Seq);
}
//------------------------------------------------------------------------------
void DestruirCancion(PCancion &C)
{
	TPatternNum I;

	if (C!=NULL)
	{
		for (I=0;I<=MaxPatterns-1;I++)
			if(C->Pats[I]!=NULL) 
				DestruirPattern(C->Pats[I]);

		DestruirTablaIns(C->TIns);
		DestruirSecuencia(C->Seq);

		delete(C);
	};
};
//------------------------------------------------------------------------------
bool CancionVacia(PCancion C)
{
	unsigned char	I;
	bool	V;

     if(C==NULL)	{ return(true); };

     I = 0;
     V = PatternVacio(C->Pats[I]);
     while ((V) && (I<MaxPatterns-1))
     {
          I++;
          V = PatternVacio(C->Pats[I]);
     };

     return(V);
}
//------------------------------------------------------------------------------
unsigned short NumeroDePatterns(PCancion C)
{
	unsigned short I,J;

	J = 0;
	for (I=0;I<=MaxPatterns-1;I++)
		if (! PatternVacio(C->Pats[I]))
			J++;

	return(J);
};
//------------------------------------------------------------------------------
#define SONG_ID "TCK2"

bool CargarCancionGeneral(PCancion &C,FILE *F)
{
	char	SID[4];
	char	VID[4];
	int Pos,Cantidad;
	unsigned short	ID;

    CrearCancion(C);

      // Lectura del identificador
      BLOCKREAD(F,SID,4);
      if (IORESULT!=0)	return(false);
      if (strncmp(SID,SONG_ID,4)) return(false);

      // Lectura de la  versión
      BLOCKREAD(F,VID,4);
      if (IORESULT!=0) return(false);
      if (strncmp(VID,VersionID,4)) return(false);

      // Cargar informacion relativa a la cancion
      BLOCKREAD(F,&C->Titulo,MaxCarNombreCancion+1);
      if (IORESULT!=0) { C->Titulo = TITULO_POR_DEFECTO;	return(false);	};
      BLOCKREAD(F,&C->Autor,MaxCarNombreAutor+1);
      if (IORESULT!=0) { C->Titulo = TITULO_POR_DEFECTO;	C->Autor = AUTOR_POR_DEFECTO;	return(false);	};
      BLOCKREAD(F,&C->BPMS,2);
      if (IORESULT!=0) { C->Titulo = TITULO_POR_DEFECTO;	C->Autor = AUTOR_POR_DEFECTO;	C->BPMS = BPMS_POR_DEFECTO;	return(false);	};
	
      Pos = FILEPOS(F);
      while(! EoF(F))
      {
           // Leer identificador de tipo de estructura
           BLOCKREAD(F,SID,4);

           // Leer identificador de grupo de estructura
           BLOCKREAD(F,&ID,sizeof(unsigned short));

           // Leer cantidad de unsigned chars que ocupa la estructura
           BLOCKREAD(F,&Cantidad,sizeof(int));

           SEEK(F,Pos);		// Posicionar fichero a principio de bloque de estructura
           Pos = Pos+Cantidad;

           // -------------------------------
           // Leer Estructura correspondiente
           // -------------------------------

           // ------------------------------- Secuencia
           if ((! strncmp(SID,"SQNC",4)) && (ID == 65535))
           {
                CargarSecuenciaGeneral(C->Seq,F,ID);
           }
           // ------------------------------- Tabla Instrumentos
      else if ((! strncmp(SID,"TINS",4)) && (ID==65535))
           {
                CargarTablaInsGeneral(C->TIns,F,ID);
           }
           // ------------------------------- Patrones
      else if ((! strncmp(SID,"PTRN",4)) && (ID<MaxPatterns))
           {
                CargarPatternGeneral(C->Pats[ID],F,ID);
           };
           
		   // -------------------------------
           SEEK(F,Pos);
      };

      return(true);
}
//------------------------------------------------------------------------------
bool SalvarCancionGeneral(PCancion C,FILE *F)
{
	TPatternNum I;

	// Escritura del identificador
	BLOCKWRITE(F,SONG_ID,4);
	if (IORESULT!=0) { return(false); };

	// Escritura de la  versión
	BLOCKWRITE(F,VersionID,4);
    if (IORESULT!=0) { return(false); };


	// Salvar informacion relativa a la cancion
	BLOCKWRITE(F,&C->Titulo,MaxCarNombreCancion+1);
	if (IORESULT!=0) { return(false); };
    BLOCKWRITE(F,&C->Autor,MaxCarNombreAutor+1);
    if (IORESULT!=0) { return(false); };
    BLOCKWRITE(F,&C->BPMS,2);
	if (IORESULT!=0) { return(false); };


     // Escritura de la tabla de instrumentos
     if (! SalvarTablaInsGeneral(C->TIns,F,65535))
     {
          // Ocurrió un error por ahí
          return( false );
     };

	// Escritura de la secuencia
	if (! SalvarSecuenciaGeneral(C->Seq,F,65535) )
	{
		// Ocurrió un error por ahí
		return( false );  
	};

	// Escritura del numero de patterns en la cancion
	for (I=0;I<=MaxPatterns-1;I++)
	{
		// Si hacemos esto, perdemos el nombre del patron
		// if (! PatternVacio(C->Pats[I])) 

		if (C->Pats[I])

		{
			// Escritura del identificador del pattern
			if (! SalvarPatternGeneral(C->Pats[I],F,I) )
			{
			    // Ocurrió un error por ahí
				return(false);			   
			};
		};
	}
    
	return(true);
};
//------------------------------------------------------------------------------
PT98Pattern ObtenerPatronDeCancion(PCancion C,TPatternNum P)
{
     return ( C->Pats[P] );
}
//------------------------------------------------------------------------------
PT98Pattern ObtenerPatronDeSecuencia(PCancion C,TIndexSec PActual)
{
     return ( C->Pats[ LeerIndicePatronEnSecuencia(C->Seq,PActual) ] );
}
//------------------------------------------------------------------------------