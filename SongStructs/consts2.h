#ifndef Consts2H
#define Consts2H

	 // Identificadores
     extern char *VersionID;

     // Determinan la capacidad máxima de la cancion
     #define MaxTracks				64
     #define MaxBeats				128
     #define MaxPatterns			256
     #define MaxPatternsSequence	256

     // Dispositivos MIDI
     #define MaxDispositivosMIDIOut 16
	 #define MaxInstrumentosMIDI	128
     #define MaxBancos				128

     #define MaxBPMS				500
     #define MinBPMS				50

     #define MaxSaltoLinea			64
     #define MaxAcorde				6
     #define MaxArpegio				16

     extern char *AUTOR_POR_DEFECTO;
     extern char *TITULO_POR_DEFECTO;
     
	 #define	BPMS_POR_DEFECTO	145

     // Lista de los Efectos permitidos
     #define Maxunsigned charsEfecto			16
     #define MaxEfectosPropios		32
     #define NumeroDeEfectos		14
     #define MaxEfectos				256

     #define MaxCarNombreEfecto		8
     #define MaxCarNombreConfig		40

     extern char *ListaDeEfectos[MaxEfectosPropios];

    // onstantes para el formato del fichero
    #define MaxCarNombreCancion		80
    #define MaxCarNombreAutor		80
    #define MaxCarNombrePatron		80

#endif