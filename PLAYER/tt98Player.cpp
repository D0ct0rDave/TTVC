//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#include <windows.h>
#include "tt98Player.h"
#include "OMidiOut.h"
#include "inter.h"

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

// System Exclusive messages buffer definition
typedef struct
{
    unsigned char Data[32];
	unsigned char Num;

}TBufferSysEx;

typedef TBufferSysEx*PBufferSysEx;


// Estado Track
typedef enum {ET_Activo,ET_Inactivar,ET_Inactivo} TEstadoTrack;


// Info estado track
typedef struct
{		
	unsigned char	PitchLo;	// Estado inicial a 0
	unsigned char	PitchHi;	// Estado inicial a 64

								// Los note offs, se deben hacer con estos
								// parametros, los note on, con los que hayan
								// en el puntero I
								
	bool			Activo;		// Beat en uso

	unsigned char	Canal,
					Disp,
					Nota;

	PInfoTrack		I;			// Puntero a estructura en TablaIns

}InfoEstadoTrack;


// Estado de la reproduccion
typedef enum {ER_NotPlaying,ER_PlayTrack,ER_PlayPattern,ER_PlaySong,ER_PlayBeat,ER_PlayLineaBeats} TEstadoReproduccion;

typedef void (*TConvertTCK2MidiEffect)(unsigned char Eff,unsigned char Val,TTrackNum T,PBufferSysEx EvEff);
// TConvertTCK2MidiEffect = void (Eff,Val:Byte;T:TTrackNum;EvEff:PBufferSysEx);
	
//---------------------------------------------------------------------------
// Variable declarations
//---------------------------------------------------------------------------

// System is initialized?
bool						bInit = false;

// -----------------------------
// Estado de reproduccion
bool					LoopActivado;

// -----------------------------
unsigned short			BeatsPerMinute;		// Current BeatsPerMinute, no mantiene valor 
unsigned short			MAXBPMS;
unsigned short			MINBPMS;
unsigned short			BPMS;				// BPMS mantiene valor desde inicio reproduccion

// -----------------------------						
int						NumSalidasMIDI;
TObjetoMidiOut			SalidasMIDI[MaxDispositivosMIDIOut];
// TablasEfectos:Array[0..MaxDispositivosMidiOut-1] OF TEfectosMidi;

// -----------------------------
TEstadoReproduccion		EstadoReproduccion;

// -----------------------------
InfoEstadoTrack			LineaBeats[MaxTracks];
TEstadoTrack			EstadoTracks[MaxTracks];
						
HANDLE					ThreadPlayer;
DWORD					ThreadPlayerID;
DWORD					ThreadPlayerPar;
						
// -----------------------------
// Datos locales necesarios en las rutinas mas utilizadas
PDatosEvento			D;
TBufferSysEx			BufferCmds;
						
// -----------------------------
TTrackNum				NumTrackActual;
TBeatNum				NumBeatActual;
TPatternNum				NumPatternActual;
TIndexSec				NumIndexSActual;
						
PTrack					TrackActual;
PT98Pattern				PatternActual;
PCancion				CancionActual;
PTablaIns				TablaInstrumentos;
PInfoTrack				InfoInstActual;

// -----------------------------
bool					BTP;					// Break the Pattern
TIMECAPS 				TC;
DWORD					PeriodoResolucion;
						
unsigned short			TimerID;
bool					TemporizadorActivo;
						
int						TiempoInicio;
float					TiempoTranscurrido;
int						TiempoReal;
int						MR;						//	Milisegundos Restantes
float					MEL1;
int						MEL2;					//	Milisegundos entre lineas



TConvertTCK2MidiEffect	ConvertTCK2MidiEffect;

//---------------------------------------------------------------------------
// Function prototypes
//---------------------------------------------------------------------------
void PararReproduccion();
void ResetSistemaMIDI();
void IniciaReproduccion(int Tipo);

//---------------------------------------------------------------------------
// #define USE_MIDI
void TT98PlayerLibInit()
{
	int i; 

	// Esta es la seccion de inicializacion, donde se deben obtener todos
	// los dispositivos MIDI Out e inicializarlos, si no es posible, se debe dar un mensaje
	// de error y salir del programa, o crear una funcion que haga esto mismo

	NumSalidasMIDI = NumeroDispositivosMIDIOut();

	// Obtener Dispositivos Midi Out
	for (i=0;i<NumSalidasMIDI;i++)
	{
		SalidasMIDI[i].EstablecerDispositivo(i);
		SalidasMIDI[i].Open(NULL);
	};

	for (i=0;i<MaxTracks;i++)
		EstadoTracks[i] = ET_Activo;

	// Suponemos que no se esta reproducciendo nada
	EstadoReproduccion = ER_NotPlaying;

	NumBeatActual    = 0;
	NumTrackActual   = 0;
	NumPatternActual = 0;
	NumIndexSActual  = 0;

	// Establecer valores minimo y maximo de los bpms}
	timeGetDevCaps(&TC,sizeof(TIMECAPS));

	if ( ((1000 / TC.wPeriodMin)*60/8) > 999 ) MAXBPMS = 1000; else MAXBPMS = (int ) ((1000/TC.wPeriodMin)*60/8);
	if ( ((1000 / TC.wPeriodMax)*60/8) < 10  ) MINBPMS = 10;   else MINBPMS = (int ) ((1000/TC.wPeriodMax)*60/8);

	if (TC.wPeriodMin <= 1) PeriodoResolucion = 1; else PeriodoResolucion = TC.wPeriodMin;

	BPMS = BPMS_POR_DEFECTO;
	TemporizadorActivo = false;

	bInit = true;
}
//---------------------------------------------------------------------------
void ResetSistemaMIDI()
{
    if (! bInit) TT98PlayerLibInit();

	unsigned char j,i;
	if (EstadoReproduccion == ER_NotPlaying)
	{
		// -----------------------------
		// Inicializar la linea de beats
		// -----------------------------
		memset(LineaBeats,0,sizeof(LineaBeats));

		for (j=0;j<MaxTracks;j++)
			LineaBeats[j].PitchHi = 64;

		// Para cada uno de los dispositivos
		for (j=0;j<NumSalidasMIDI;j++)
		{
			// Para cada uno de los canales
			for (i=0;i<16;i++)
			{
				// All sounds off
				SalidasMIDI[j].Comando(0xB0 | i,0x78,0);

				// Reset all controllers
				SalidasMIDI[j].Comando(0xB0 | i,0x79,0);
			}
		}
	}
}
//---------------------------------------------------------------------------
void ActivarPropiedadesMIDI(PInfoTrack Info)
{
    if (! bInit) TT98PlayerLibInit();

	if ((Info->Inst >= 0) && (Info->Inst<=127))
	{
	  // Activar el Banco
	  if (Info->Ctrl0)
		 SalidasMIDI[ Info->Disp ].Comando(0xB0 | Info->Canal,0x00,Info->Banco);

	  if (Info->Ctrl32)		// 32 en decimal o hexadecimal
		 SalidasMIDI[Info->Disp].Comando(0xB0 | Info->Canal,0x32,Info->Banco);

	  // Activar el programa
	  SalidasMIDI[Info->Disp].Comando(0xC0 | Info->Canal,Info->Inst,0);
	}
}
//---------------------------------------------------------------------------
void InicializarSistemaReproduccion(PTablaIns T)
{
    if (! bInit) TT98PlayerLibInit();

	if (EstadoReproduccion == ER_NotPlaying) 
	{
		// Volver a establecer los instrumentos de cada Track
		for (int j=0;j<MaxTracks;j++)
			ActivarPropiedadesMIDI( &(T->T[j]) );
	}
}
// --------------------------------------------------------------------------
void StopReproduccion()
{
     PararReproduccion();
}
//---------------------------------------------------------------------------
void PlayNoteOn(unsigned char Dispositivo,unsigned char Canal,unsigned char Note,unsigned char Velocity)
{
    if (! bInit) TT98PlayerLibInit();
	SalidasMIDI[Dispositivo].Comando(0x90 | Canal,Note,Velocity);
}
//---------------------------------------------------------------------------
void PlayNoteOff(unsigned char Dispositivo,unsigned char Canal,unsigned char Note)
{
    if (! bInit) TT98PlayerLibInit();
	SalidasMIDI[Dispositivo].Comando(0x90 | Canal,Note,0);
}
//---------------------------------------------------------------------------
void PlayBeat(PTrack T,PInfoTrack Info,TBeatNum Beat,TTrackNum Track)
{
    if (! bInit) TT98PlayerLibInit();


	int i;

	
	// Averiguar a que instrumento enviar la informacion
    D = TomarEventoPorTiempoTrack(T,Beat);

    if(D != NULL)
	{     
		// Mandamos primero el efecto, luego la nota
        if(D->Eff > 0)
        {
			if ((D->Eff > 1) && (D->Eff<0x09))
			{
				// Comandos que no modifican el sistema MIDI}
                switch (D->Eff)
				{
                    case 0x02:	// FDM
                    {
						StopReproduccion();
                    };
					break;

                    case 0x03:	// Break the Pattern
					{
						BTP = true;                            
					};
					break;

                    case 0x04:	// Inc Tempo
					{
						BeatsPerMinute = BeatsPerMinute + D->Val;
                        if (BeatsPerMinute>MAXBPMS) BeatsPerMinute = MAXBPMS;
                    };
					break;
                    
					case 0x05:	// Dec Tempo
                    {
                        BeatsPerMinute = BeatsPerMinute - D->Val;
                        if (BeatsPerMinute<MINBPMS)  BeatsPerMinute = MINBPMS;
                    };
					break;
                    
					case 0x06:	// bpms * 2
                    {
                             BeatsPerMinute = BeatsPerMinute << 1;
                             if (BeatsPerMinute>MAXBPMS)  BeatsPerMinute = MAXBPMS;
                    };
					break;

                    case 0x07:	// bpms / 2
                    {
                        BeatsPerMinute = BeatsPerMinute >> 1;
						if (BeatsPerMinute<MINBPMS)  BeatsPerMinute = MINBPMS;
                    };
					break;
                };
			}
            else
            {
				// Comandos que modifican estructuras, y
				// el sistema MIDI

				switch (D->Eff)
				{
					case 0x01: // NoteOff
					{
						// Procesar informacion
						if (ConvertTCK2MidiEffect != NULL) 
							ConvertTCK2MidiEffect(D->Eff,LineaBeats[Track].Nota,Track,&BufferCmds);

						// Actualizar informacion
						LineaBeats[Track].Disp  =0;
						LineaBeats[Track].Canal =0;
						LineaBeats[Track].Nota  =0;
						LineaBeats[Track].Activo=false;
					};
					break;

					case 0x12: // Efecto Pitch Bend Lo
					{
						// Procesar informacion
						if (ConvertTCK2MidiEffect != NULL) 
							ConvertTCK2MidiEffect(D->Eff,D->Val,Track,&BufferCmds);

						LineaBeats[Track].PitchHi = D->Val;
						BufferCmds.Data[1] = LineaBeats[Track].PitchLo;
					};
					break;

					case 0x13: // Efecto Pitch Bend Hi
					{
						// Procesar informacion
						if (ConvertTCK2MidiEffect!=NULL)
							ConvertTCK2MidiEffect(D->Eff,D->Val,Track,&BufferCmds);

						LineaBeats[Track].PitchLo = D->Val;
						BufferCmds.Data[1] = LineaBeats[Track].PitchHi;
					};
					break;

					default:
					{
						if (ConvertTCK2MidiEffect!=NULL) 
							ConvertTCK2MidiEffect(D->Eff,D->Val,Track,&BufferCmds);

					};
				};

				// ----------------------
                // Modificar Sistema MIDI
                // ----------------------

                // Comandos propios
                if (BufferCmds.Data[0] < 0xF0) 
                {
					// Comando compuesto
                    for (i=0;i<(BufferCmds.Num /3);i++)
                        SalidasMIDI[Info->Disp].Comando(BufferCmds.Data[i*3+0],
                                                        BufferCmds.Data[i*3+1],
                                                        BufferCmds.Data[i*3+2]);
                }
                else
					// Comando SysEx
                    SalidasMIDI[Info->Disp].SysEx(BufferCmds.Data,BufferCmds.Num);
			}; // END if ((D->Eff > 1) && (D->Eff<0x09))
		}; // END if(D->Eff > 0)
          

        // Mandar nota si es que hay
		if (D->Nota>0)
        {
			// Escribir en la tabla
            if (LineaBeats[Track].Activo)  // Si habia un sonido en note on ...
				//...Note off
				SalidasMIDI[LineaBeats[Track].Disp].Comando(0x90 | LineaBeats[Track].Canal,LineaBeats[Track].Nota,0);

            // Actualizar información}
            LineaBeats[Track].Disp   = Info->Disp;
            LineaBeats[Track].Canal  = Info->Canal;
            LineaBeats[Track].Nota   = D->Nota-1;
            LineaBeats[Track].Activo = true;
 
            // Note on
			SalidasMIDI[LineaBeats[Track].Disp].Comando(0x90 | LineaBeats[Track].Canal,D->Nota-1,D->Vol);
		};
	};
}
//---------------------------------------------------------------------------
void EstablecerFuncionConversionTCKEffect2MIDIEvent(void *funcion)
{
    if (! bInit) TT98PlayerLibInit();
	
	if (funcion!=NULL)        
		ConvertTCK2MidiEffect = (TConvertTCK2MidiEffect) funcion;

}
// ----------------------------------------------------------------------------
void PlayLBeats(PT98Pattern P,PTablaIns TI,TBeatNum Beat)
{
	TTrackNum i;
	
	for (i=0;i<MaxTracks;i++)
    {
		switch (EstadoTracks[i])
		{
            case ET_Activo:
			{
				PlayBeat(LeerTrackDePattern(P,i),LeerInfoDeTablaIns(TI,i),Beat,i);
			}
			break;

            case ET_Inactivar:
			{
				SalidasMIDI[ LineaBeats[i].Disp ].Comando(0xB0 | LineaBeats[i].Canal,0x78,0);
                EstadoTracks[i] = ET_Inactivo;
            };
			break;

            case ET_Inactivo:
			{ 
				// Nada
			};
			break;
		};
	}
}

// ----------------------------------------------------------------------------
void PlayLineaBeats(PT98Pattern P,PTablaIns TI,TBeatNum Beat)
{
    if (! bInit) TT98PlayerLibInit();

     if (EstadoReproduccion == ER_NotPlaying) 
     {
          InicializarSistemaReproduccion(TI);
          PlayLBeats(P,TI,Beat);

          Sleep(250);
          ResetSistemaMIDI();
     };
};
// -----------------------------------------------------------------------
void PlayTrack(PTrack Track,TTrackNum T,PInfoTrack Info)
{
    if (! bInit) TT98PlayerLibInit();

	// Si el temporizador esta activo, entonces solo se puede parar si ha sido
	// desde aqui desde donde se ha iniciado}

	if (EstadoReproduccion == ER_NotPlaying) 
	{
		NumTrackActual    = T;
		TrackActual       = Track;
		InfoInstActual    = Info;

		BeatsPerMinute    = BPMS;
		BTP               = false;

		ActivarPropiedadesMIDI(Info); // Creo que esto debe ir aqui
		IniciaReproduccion(0);
	}
	else
		if (EstadoReproduccion == ER_PlayTrack)
			StopTrack();
};

// ----------------------------------------------------------------------------
void StopTrack()
{
    if (! bInit) TT98PlayerLibInit();

     PararReproduccion();
}
// ----------------------------------------------------------------------------
void PlayPattern(PT98Pattern Pattern,PTablaIns TI)
{
	if (EstadoReproduccion == ER_NotPlaying) 
	{
		PatternActual     = Pattern;
		TablaInstrumentos = TI;

		BeatsPerMinute    = BPMS;
		BTP               = false;
		InicializarSistemaReproduccion(TI);
		IniciaReproduccion(1);
	}
	else
	{
		if (EstadoReproduccion == ER_PlayPattern)
			StopPattern();
	}
};
// ----------------------------------------------------------------------------
void StopPattern()
{
    if (! bInit) TT98PlayerLibInit();
     PararReproduccion();
}
// ----------------------------------------------------------------------------
void PlayCancion(PCancion C)
{
	if (EstadoReproduccion == ER_NotPlaying) 
	{
		CancionActual     = C;
		NumPatternActual  = LeerIndicePatronEnSecuencia(CancionActual->Seq,NumIndexSActual);
		PatternActual     = ObtenerPatronDeSecuencia(CancionActual,NumIndexSActual);
		TablaInstrumentos = CancionActual->TIns;

		BeatsPerMinute    = BPMS;
		BTP               = false;

		InicializarSistemaReproduccion(TablaInstrumentos);
		IniciaReproduccion(2);
	}
	else
	{
		if (EstadoReproduccion == ER_PlayPattern)
			StopCancion();
	}
};
// ----------------------------------------------------------------------------
void StopCancion()
{
    if (! bInit) TT98PlayerLibInit();
     PararReproduccion();
};
// ----------------------------------------------------------------------------
void EstablecerEstadoTrack(TTrackNum Track,bool Mute)
{
     if (Mute)
         EstadoTracks[Track] = ET_Inactivar;
     else
         EstadoTracks[Track] = ET_Activo;
};
//---------------------------------------------------------------------------
void EstablecerLooping(bool OnOff)
{
    if (! bInit) TT98PlayerLibInit();
     LoopActivado = OnOff;
};
//---------------------------------------------------------------------------
void EstablecerBeatsPerMinute(unsigned short B_P_M_S)
{
    if (! bInit) TT98PlayerLibInit();
    BPMS			= B_P_M_S;
    BeatsPerMinute	= BPMS;
};
//---------------------------------------------------------------------------
unsigned short TomarBeatsPerMinute()
{
    if (! bInit) TT98PlayerLibInit();
    return(BeatsPerMinute);
};
//---------------------------------------------------------------------------
void ObtenerMinMaxBPMS(unsigned short &Min,unsigned short &Max)
{
    Min = MINBPMS;
    Max = MAXBPMS;
};
//---------------------------------------------------------------------------
void ObtenerEstadoReproduccion(TBeatNum &Beat,TPatternNum &Pattern,TIndexSec &IndSec)
{
    Beat    = NumBeatActual;
    Pattern = NumPatternActual;
    IndSec  = NumIndexSActual;
};
//---------------------------------------------------------------------------
void EstablecerEstadoReproduccion(TBeatNum Beat,TIndexSec IndSec)
{
    if (! bInit) TT98PlayerLibInit();
    
	NumBeatActual   = Beat;
    NumIndexSActual = IndSec;
};
//---------------------------------------------------------------------------
bool Reproduciendo()
{
    if (! bInit) TT98PlayerLibInit();
    return ( EstadoReproduccion != ER_NotPlaying) ;
};
// -------------------------------------------------------------------------------
void ParaTemporizador()
{
    if (TemporizadorActivo)
    {
		CloseHandle(ThreadPlayer);

        TemporizadorActivo = false;
		EstadoReproduccion = ER_NotPlaying;
	};
}
// -------------------------------------------------------------------------------
void __stdcall TimerCallBackTrack(unsigned int uTimerID,unsigned int uMessage,unsigned long dwUser,unsigned long dw1,unsigned long dw2)
{
     timeKillEvent(TimerID); // Libera el Handle interno

     PlayBeat(TrackActual,InfoInstActual,NumBeatActual,NumTrackActual);
     NumBeatActual = (NumBeatActual + 1) % MaxBeats;

     if (BTP) {BTP=false;NumBeatActual=0;};

     if (NumBeatActual==0)
	 {
		if (! LoopActivado)
        {
             ParaTemporizador();
             return;
        }
        else
            BeatsPerMinute = BPMS;
	}
    
	//Calculo de la velocidad y recalibracion del reloj si hace falta
    TiempoTranscurrido = TiempoTranscurrido + MEL1;        // MEL1 Contenia los
                                                           // milisegundos entre lineas iniciales
    TiempoReal         = TiempoReal + MEL2;

    MEL1= 1000.0f /((float)BeatsPerMinute*8.0f/60.0f);
    MEL2= MEL1;
    MR  = (TiempoTranscurrido-TiempoReal);

    if (MR>0) MEL2=MEL2+MR;

    // Reprogramar el temporizador
	if (TemporizadorActivo)
        TimerID = timeSetEvent(MEL2,PeriodoResolucion,TimerCallBackTrack,0,TIME_ONESHOT);
}
// -------------------------------------------------------------------------------
void __stdcall TimerCallBackPattern(unsigned int uTimerID,unsigned int uMessage,unsigned long dwUser,unsigned long dw1,unsigned long dw2)
{
    timeKillEvent(TimerID); // Libera el Handle interno

    PlayLBeats(PatternActual,TablaInstrumentos,NumBeatActual);
    NumBeatActual = (NumBeatActual + 1) % MaxBeats;

	if (BTP){BTP=false;NumBeatActual=0;};

     if (NumBeatActual==0)
	 {
        if (!LoopActivado)
        {
             ParaTemporizador();
             return;
		}
        else
            BeatsPerMinute = BPMS;
	}

     // Calculo de la velocidad y recalibracion del reloj si hace falta
     TiempoTranscurrido = TiempoTranscurrido + MEL1;			// MEL1 Contenia los
																// milisegundos entre lineas iniciales
     TiempoReal         = TiempoReal + MEL2;

     MEL1= 1000.0f /((float)BeatsPerMinute*8.0f/60.0f);
     MEL2= MEL1;
     MR  = TiempoTranscurrido - TiempoReal;

     if (MR>0) MEL2 = MEL2+MR;

     // Reprogramar el temporizador
     if (TemporizadorActivo)
        TimerID = timeSetEvent(MEL2,PeriodoResolucion,TimerCallBackPattern,0,TIME_ONESHOT);
}
// -------------------------------------------------------------------------------
void __stdcall TimerCallBackSong(unsigned int uTimerID,unsigned int uMessage,unsigned long dwUser,unsigned long dw1,unsigned long dw2)
{
     timeKillEvent(TimerID); // Libera el Handle interno

     PlayLBeats(PatternActual,TablaInstrumentos,NumBeatActual);
     NumBeatActual = (NumBeatActual + 1) % MaxBeats;

     if (BTP) { BTP=false;NumBeatActual=0;};

     if (NumBeatActual==0) 
     {
          if (NumIndexSActual+1 < NumPatronesEnSecuencia(CancionActual->Seq)) 
          {
               NumIndexSActual++;

               NumPatternActual = LeerIndicePatronEnSecuencia(CancionActual->Seq,NumIndexSActual);
               PatternActual    = ObtenerPatronDeSecuencia(CancionActual,NumIndexSActual);
          }
          else
          {
               if (! LoopActivado) 
               {
                    ParaTemporizador();
                    return;
               }
               else
               {
                    BeatsPerMinute = BPMS;

                    NumIndexSActual = 0;
                    PatternActual   = ObtenerPatronDeSecuencia(CancionActual,NumIndexSActual);
               };
          };
     };

     // Calculo de la velocidad y recalibracion del reloj si hace falta
     TiempoTranscurrido = TiempoTranscurrido + MEL1;        // MEL1 Contenia los
                                                            // milisegundos entre lineas iniciales}
     TiempoReal         = TiempoReal + MEL2;

     MEL1= 1000.0f /((float)BeatsPerMinute*8.0f/60.0f);
     MEL2= MEL1;
     MR  = TiempoTranscurrido-TiempoReal;

     if (MR>0)  MEL2 = MEL2+MR;

     // Reprogramar el temporizador
     if (TemporizadorActivo) 
        TimerID = timeSetEvent(MEL2,PeriodoResolucion,TimerCallBackSong,0,TIME_ONESHOT);
}
// -------------------------------------------------------------------------------
// ThreadFunc
// -------------------------------------------------------------------------------
unsigned long __stdcall IniciaTemporizador(void *Tipo)
{
	if (! TemporizadorActivo)
	{
		// EstablecerCanalesMIDI;
		TemporizadorActivo = true;
		TiempoTranscurrido = 0;
		TiempoReal         = 0;
		MEL1= 1000.0f /((float)BeatsPerMinute*8.0f/60.0f);
		MEL2 = MEL1;

		// Establecer Resolucion de eventos ocurren con una precision de +-10 milisegundos
		timeBeginPeriod(PeriodoResolucion);

		switch ( *(int*)(Tipo) )
		{
			case 0:	
			{
				EstadoReproduccion =ER_PlayTrack;
				TimerID = timeSetEvent(MEL2,PeriodoResolucion,TimerCallBackTrack,0,TIME_ONESHOT);
			};
			break;

			case 1:
			{
				EstadoReproduccion=ER_PlayPattern;
				TimerID=timeSetEvent(MEL2,PeriodoResolucion,TimerCallBackPattern,0,TIME_ONESHOT);
			};
			break;

			case 2:
			{
				EstadoReproduccion = ER_PlaySong;
				TimerID = timeSetEvent(MEL2,PeriodoResolucion,TimerCallBackSong,0,TIME_ONESHOT);
			};
			break;
		};

		// Este thread estara en espera hasta que se pulse Stop, que llamara a
		// PararTemporizador o hasta que acabe la cancion, que tambien llamara
		// a PararTemporizador

		WaitForSingleObject(ThreadPlayer,INFINITE);		

		// Busy Wait
		while(EstadoReproduccion != ER_NotPlaying);	

		timeEndPeriod(PeriodoResolucion);

		Sleep(250);
		ResetSistemaMIDI();
	};

	// Retornar un valor cualquiera
	return(0);
}
// -------------------------------------------------------------------------------
void IniciaReproduccion(int Tipo)
{
	// Crear el ThreadPlayer y retornar
    if (! TemporizadorActivo)
    {
		// ShowMessage('Antes');
        ThreadPlayerPar = Tipo;

        ThreadPlayer = CreateThread(NULL,
									65536,
                                    IniciaTemporizador,
                                    (void*)&ThreadPlayerPar,
                                    CREATE_SUSPENDED,
                                    &ThreadPlayerID);

		if (ThreadPlayer!=0)
        {
			SetThreadPriority(ThreadPlayer,THREAD_PRIORITY_NORMAL/*THREAD_PRIORITY_TIME_CRITICAL*/);
            ResumeThread(ThreadPlayer);
        };
     };
};
// -------------------------------------------------------------------------------
void PararReproduccion()
{
     ParaTemporizador();
}
//----------------------------------------------------------------------------
void Finalizacion()
{
    if (! bInit) return;
	
	// Seccion de finalizacion, donde se liberaran los recursos tomados
	for (int i=0;i<NumSalidasMIDI;i++)
		SalidasMIDI[i].Close();
}
//---------------------------------------------------------------------------
