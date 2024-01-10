#ifndef TiposH
#define TiposH

#include "PascalCnv.h"
#include "Consts2.h"

typedef unsigned short	TBeatNum;
typedef unsigned short	TTrackNum;
typedef unsigned short	TPatternNum;
typedef unsigned short	TIndexSec;
typedef unsigned short	TIndexTIns;

#define BeatString	String256
#define Ltt98String	String256
#define tt98String	String80
#define Stt98String String35

#define DEF_TRK_VOL	127
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

    typedef struct{

	  Stt98String	Nombre;
      unsigned char			Disp;
      unsigned char			Banco;
      char					Inst;
      unsigned char			Canal;
      unsigned char			VolDef;
      bool		Ctrl0;
      bool		Ctrl32;

	}TInfoTrack;
	
	typedef TInfoTrack*	 PInfoTrack;
//----------------------------------------------------------------------------    
    typedef struct{
		TInfoTrack T[MaxTracks];
	}TTablaIns;

	typedef TTablaIns*	PTablaIns;
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------    
    typedef struct{
		TPatternNum	S[MaxPatternsSequence];
		TIndexSec	Num;
	}TSecuencia;

	typedef TSecuencia*	PSecuencia;
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------    
    typedef struct{
         // TBeatNum	Time;

         unsigned char		Nota;
         unsigned char		Vol;
         unsigned char		Eff;
         unsigned char		Val;
		
	}TDatosEvento;

	typedef TDatosEvento *PDatosEvento;
//----------------------------------------------------------------------------    
    typedef struct{
       TDatosEvento	PE[MaxBeats];
       TBeatNum		NumEventos;
	}TTrack;

	typedef TTrack*		PTrack;
//----------------------------------------------------------------------------
    typedef struct{
        tt98String	Nombre;
        TTrack		Tracks[MaxTracks];
	}TT98Pattern;

	typedef TT98Pattern*	PT98Pattern;
//----------------------------------------------------------------------------
    typedef struct{

		tt98String	Titulo;
		tt98String	Autor;
		unsigned short		BPMS;

		PTablaIns	TIns;
		PSecuencia	Seq;
		::PT98Pattern	Pats[MaxPatterns];

	}TCancion;
    
	typedef TCancion*	PCancion;
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
extern  tt98String Error;
#endif
