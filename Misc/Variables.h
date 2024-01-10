#ifndef VariablesH
#define VariablesH

#include "inter.h"
#include "StrList.h"
#include "Consts2.h"
#include "Tipos.h"
#include "InfoPistas.h"
#include "TEfectos.h"
#include "bloques.h"
#include "RList.h"

typedef void (* TMuteProc  )(unsigned short Track,bool Estado);
typedef void (* TUpdVarProc)(unsigned short PB,unsigned short BA,unsigned short PT,unsigned short TA);

typedef unsigned int TColor;

typedef struct
{
	AnsiString                      Font;				// Fuente a usar, debe ser  proporcional
	unsigned char                   FontSize;			// Tamanyo de la fuente a usar
	
	TMuteProc						ProcMute;
	TUpdVarProc						ActVar;
	
	TColor							PattEdBk;			// Color de background del pattern editor

	TColor							NumFnColor;			// Color de fuente de la numeración
	TColor							NumBkColor;

	TColor							MrkBtBkColor;		// Color de background	beats marcados
	TColor							MrkBtFnColor;		// Color de fuente de 0

	TColor							NrmFnColor;			// Color de fuente del patron normal
	TColor                          NrmBkColor;			// Color de fondo del patron normal	  

	TColor							Nrm8BMFnColor;		// Color de fuente del patron normal beat multiplo 8
	TColor                          Nrm8BMBkColor;		// Color de fondo del patron normal	beat multiplo 8

	TColor							CurFnColor;			// Color de fuente del beat actual
	TColor                          CurBkColor;			// Color de fondo del beat actual

	TColor							SelFnColor;			// Color de fuente del patron de un bloque seleccionado
	TColor                          SelBkColor;			// Color de fondo del patrón de un bloque seleccionado

	/*
	short                           cx,					// Offset dentro del Canvas
									cy;
	*/

	// TCanvas*                          Canvas;

}TPattEdProps;

typedef struct VariablesGlobales
{
	// Estructuras del tipo cancion y pattern y track
	PCancion					Sng;				// Guarda nuestra Canción
	PBloque                     BloqueAux;
	PBloque                     BloqueAux2;			// Para salvar Bloques
	PT98Pattern                 PatternAux;			// Patron auxiliar
	TTrack                      TrackAux;			// Track ""

	TIndexSec                   IndSecActual;		// Indice actual en la tabla de secuencia
	PT98Pattern                 PatronActual;		// Patron actual de trabajo (tb. en pantalla)

	TPatternNum                 PatternActual;		// Indice del vector de patterns de la canción
													// Donde esta el patron actual
	PTrack                      PistaActual;		// Pista actual de trabajo

	//--------------------------------------------------------------------------
	//                     Elementos de la malla de beats
	//--------------------------------------------------------------------------
	// <WARNING PASAR A TTVC>
	TPattEdProps				PDP;				//Propiedades del display

	// Determinan la posicion absoluta dentro de la canción
	unsigned short              AbsBeatActual,AbsTrackActual;

	//--------------------------------------------------------------------------
	// Miscelanea
	//--------------------------------------------------------------------------
	unsigned char				NumPorts;			// Numero de puertos midi Out
	unsigned char DispositivoSalidaMIDI;			// Identificador del dispositivo de salida MIDI por defecto
	unsigned char DispositivoEntradaMIDI;			// Identificador del dispositivo que usaremos como
													// entrada de notas y comandos midi
	
	int			  iRemoteMIDIInput;					// Usar dispositivo de entrada midi remoto
	AnsiString	  asMIDIInputHost;


	AnsiString					NombreFichero;		// Nombre de fichero a usar
	TEfectosMIDI  ListaEfectos[MaxDispositivosMIDIOut];
	unsigned char EfectoActual ;

	// Esta variable recoge la informacion del fichero de efectos de cada Disp Midi Out
	AnsiString	 FicherosListasEfectos[MaxDispositivosMIDIOut];
	TStrList     CFicherosListaEfectos[MaxDispositivosMIDIOut];

	// Esta variable recoge la info de todos los ficheros que contienen datos de instrumentos
	// de todos los bancos de los dispositivos
	AnsiString	 FicherosListasInstrumentos[MaxDispositivosMIDIOut][MaxBancos];

	unsigned char SaltoLinea,           // Cantidad de beats que se salta al dar un salto de linea
	Acorde,                             // Numero de notas que se graban antes de dar un salto de linea
	Arpegio;                            // Numero de tracks que se abarcan al grabar notas
	int   Transponer;                   // Tonos a transponer sobre las notas entradas en el teclado

	unsigned char			ContAcordes;
	unsigned char			ContArpegios;		// Contadores

	bool					Loop;				// Looping
	bool					PatronActivo;		// Edicion activada o no
	

	AnsiString				asLangFile;			// Fichero de idioma	
	
	TRecentList				oRcntFiles;			// Recent files

	// MD
	//--------------------------------------------------------------------------
} VariablesGlobales;

extern VariablesGlobales VG;

//---------------------------------------------------------------------------
#endif