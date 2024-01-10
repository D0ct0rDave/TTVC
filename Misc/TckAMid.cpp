//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#endif
#pragma hdrstop

#include <stdlib.h>
#include <string.h>
#include <windows.h>	// For MessageBox

#include "TckAMid.h"
#include "PascalCnv.h"

#include "CrtError.h"

#include "Consts2.h"
#include "inter.h"
#include "Variables.h"

//------------------------------------------------------------------------------
#define MIDI_ID "MThd"
#define TRACK_ID "MTrk"

unsigned int uiBIGENDIAN(unsigned int _uiSrc)
{
	unsigned int uiDst;
	unsigned char* ucSrc=(unsigned char*)&_uiSrc;
	unsigned char* ucDst=(unsigned char*)&uiDst;

	ucDst[0] = ucSrc[3];
	ucDst[1] = ucSrc[2];
	ucDst[2] = ucSrc[1];
	ucDst[3] = ucSrc[0];

	return(uiDst);
}

unsigned int ui3BIGENDIAN(unsigned int _uiSrc)
{
	unsigned int uiDst;
	unsigned char* ucSrc=(unsigned char*)&_uiSrc;
	unsigned char* ucDst=(unsigned char*)&uiDst;

	ucDst[0] = ucSrc[2];
	ucDst[1] = ucSrc[1];
	ucDst[2] = ucSrc[0];

	return(uiDst);
}

unsigned short usBIGENDIAN(unsigned short _usSrc)
{
	unsigned short usDst;
	unsigned char* ucSrc=(unsigned char*)&_usSrc;
	unsigned char* ucDst=(unsigned char*)&usDst;

	ucDst[0] = ucSrc[1];
	ucDst[1] = ucSrc[0];
	
	return(usDst);
}
//------------------------------------------------------------------------------
   unsigned int             TiempoActualAbsoluto;
   unsigned int             BeatActualAbsoluto;

   unsigned int             IndiceSecuencia;
   unsigned int             BeatActual;

   PT98Pattern              PAct;
   PTrack                   TAct;

   bool                     EndOfModule;   
   unsigned int             PosTamTrack;    //Posicion donde esta el campo tamanyo del track
   unsigned int             PosDataTrack;   //Posicion donde estan los datos del track
   unsigned int             PosFinTrack;    //Posicion donde esta la posicion de fin del track
   unsigned int             SizeOfTrack;    //Tamaño de los datos del track
   unsigned int				CurrentBPMs;
   unsigned char            LastNoteON;

   PInfoTrack               InfTrackActual;
   TConvertTCK2MIDIEffect   ConvertTCK2MIDIEffect = NULL;
//------------------------------------------------------------------------------
void EscribirMetaEvento(TMetaEvent me,PEventoMIDI em,FILE *f)
{
	unsigned char aux;	

	// Write delta time of the event
	unsigned char zero = 0;
	BLOCKWRITE(f,&zero,1);

	switch (me)
	{
		case METAEVENT_TEXT:
		{
			aux = 0xff;BLOCKWRITE(f,&aux,1);
			aux = 0x01;BLOCKWRITE(f,&aux,1);
			BLOCKWRITE(f,&em->Tam,1);
			BLOCKWRITE(f,&em->D,em->Tam);
		}
		break;

		case METAEVENT_COPYRIGHT:
		{
			aux = 0xff;BLOCKWRITE(f,&aux,1);
			aux = 0x02;BLOCKWRITE(f,&aux,1);
			BLOCKWRITE(f,&em->Tam,1);
			BLOCKWRITE(f,&em->D,em->Tam);
		}
		break;

		case METAEVENT_TRACKNAME:
		{
			aux = 0xff;BLOCKWRITE(f,&aux,1);
			aux = 0x03;BLOCKWRITE(f,&aux,1);
			BLOCKWRITE(f,&em->Tam,1);
			BLOCKWRITE(f,&em->D,em->Tam);
		}
		break;

		case METAEVENT_ENDOFTRACK:
		{
			aux = 0xff;BLOCKWRITE(f,&aux,1);
			aux = 0x2f;BLOCKWRITE(f,&aux,1);
			aux = 0x00;BLOCKWRITE(f,&aux,1);
		}
		break;

		case METAEVENT_SETTEMPO: 
		{
			aux = 0xff;BLOCKWRITE(f,&aux,1);
			aux = 0x51;BLOCKWRITE(f,&aux,1);
			aux = 0x03;BLOCKWRITE(f,&aux,1);
			BLOCKWRITE(f,&em->D,3);
		}
		break;

		case METAEVENT_TIMESIGNATURE:
		{
			aux = 0xff;BLOCKWRITE(f,&aux,1);
			aux = 0x58;BLOCKWRITE(f,&aux,1);
			aux = 0x04;BLOCKWRITE(f,&aux,1);
			BLOCKWRITE(f,&em->D,4);
		}
		break;

		case METAEVENT_KEYSIGNATURE:
		{
			aux = 0xff;BLOCKWRITE(f,&aux,1);
			aux = 0x59;BLOCKWRITE(f,&aux,1);
			aux = 0x02;BLOCKWRITE(f,&aux,1);
			BLOCKWRITE(f,&em->D,2);
		}
		break;
	}
}
//------------------------------------------------------------------------------
unsigned int TCKTimeAMIDITime(unsigned int BeatAbsolutoActual,unsigned int BPMS,unsigned int &UltimoTiempoAbsoluto)
{
    unsigned int Res,FRes,TiempoAbsolutoActual;

     // Transformar el puntero de la cancion en un tiempo absoluto
     TiempoAbsolutoActual = BeatAbsolutoActual;

     // Restar el tiempo absoluto a TiempoAbsolutoActual
     Res = TiempoAbsolutoActual - UltimoTiempoAbsoluto;
     UltimoTiempoAbsoluto = TiempoAbsolutoActual;

     // Transformar el resultado en un valor tipo Variable Lenght Quantity
     FRes = Res & 0x7f;
     while (Res >>= 7)
     {
          FRes <<= 8;
          FRes |= (Res & 0x0000007f) | 0x80;
     }

     return(FRes);
}
//------------------------------------------------------------------------------
bool EventoTCK_A_EventoMIDI(PDatosEvento EvT,TTrackNum T,TEventoMIDI &EvNota,TEventoMIDI &EvEff)
// Retornara CIERTO cuando el beat contenga informacion util
{
	if (EvT == NULL) return(false);

	// Inicializar las estructuras
	EvNota.Tam=0;
	EvEff.Tam =0;

	// Establecer el contenido de las estructura4s
	if ( ((EvT->Nota!=255) && (EvT->Nota!=0)) || (EvT->Eff!=0)) 
	{
		//----------------------------------------------------------
		if ((EvT->Nota!=255) && (EvT->Nota!=0))
		{
			if (LastNoteON != 255) 
			{
				EvNota.D[0] = COMANDO_NOTEOFF | InfTrackActual->Canal;
				EvNota.D[1] = LastNoteON;
				EvNota.D[2] = 0;

				EvNota.D[3] = 0;	// !!!! DeltaT intercalado !!!!!

				EvNota.D[4] = COMANDO_NOTEON | InfTrackActual->Canal;
				EvNota.D[5] = EvT->Nota-1;
				EvNota.D[6] = EvT->Vol;
				EvNota.Tam  = 7;
			}
			else
			{
				EvNota.D[0] = COMANDO_NOTEON | InfTrackActual->Canal;
				EvNota.D[1] = EvT->Nota-1;
				EvNota.D[2] = EvT->Vol;
				EvNota.Tam  = 3;
			}

			LastNoteON = EvT->Nota-1;
		}

		//----------------------------------------------------------
		if (EvT->Eff != 0)
		{
			switch (EvT->Eff)
			{
				case 1:	//	E[1].Nombre="Note Off";
				{
					EvEff.Tam  = 3;
					EvEff.D[0] = COMANDO_NOTEOFF | InfTrackActual->Canal;
					EvEff.D[1] = LastNoteON;
					EvEff.D[2] = 0;

					LastNoteON = 255;
				}
				break;

				case 2:
				{
					//E[2].Nombre="EndOfMod";
					if ((EvT->Nota!=255) && (EvT->Nota!=0))
						return (true);
					else
						return (false);
				}
				break;				

				case 3:
				{
					// E[3].Nombre="PatBreak";				
					if ((EvT->Nota!=255) && (EvT->Nota!=0))
						return (true);
					else
						return (false);
				}
				break;				

				case 4:
				{
					// E[4].Nombre="IncTempo";
					EvEff.Tam  = 6;
					EvEff.D[0] = 0xff;
					EvEff.D[1] = 0x51;
					EvEff.D[2] = 0x03;
				
					unsigned int uiBPMS = CurrentBPMs + EvT->Val;
					*((unsigned int*)(&EvEff.D[3])) = ui3BIGENDIAN(60000000 / uiBPMS);
				}
				break;

				case 5:
				{
					//E[5].Nombre="DecTempo";
					EvEff.Tam  = 6;
					EvEff.D[0] = 0xff;
					EvEff.D[1] = 0x51;
					EvEff.D[2] = 0x03;
				
					unsigned int uiBPMS = CurrentBPMs - EvT->Val;
					*((unsigned int*)(&EvEff.D[3])) = ui3BIGENDIAN(60000000 / uiBPMS);

				}
				break;

				case 6:
				{
					//E[6].Nombre="BPMS * 2";
					EvEff.Tam  = 6;
					EvEff.D[0] = 0xff;
					EvEff.D[1] = 0x51;
					EvEff.D[2] = 0x03;
				
					unsigned int uiBPMS = CurrentBPMs*2;
					*((unsigned int*)(&EvEff.D[3])) = ui3BIGENDIAN(60000000 / uiBPMS);
				}
				break;

				case 7:
				{
					//E[7].Nombre="BPMS / 2";
					EvEff.Tam  = 6;
					EvEff.D[0] = 0xff;
					EvEff.D[1] = 0x51;
					EvEff.D[2] = 0x03;
				
					unsigned int uiBPMS = CurrentBPMs/2;
					*((unsigned int*)(&EvEff.D[3])) = ui3BIGENDIAN(60000000 / uiBPMS);
				}
				break;

				case 9:
				{
					//E[9].Nombre="PrChange";
					EvEff.Tam  = 2;
					EvEff.D[0] = COMANDO_CAMBIOPROGRAMA | InfTrackActual->Canal;
					EvEff.D[1] = EvT->Val;
				}
				break;

				case 10:
				{
					
					/*
					EvEff.Tam  = 3;
					EvEff.D[0] = COMANDO_CAMBIOPROGRAMA | InfTrackActual->Canal;
					EvEff.D[1] = EvT->Val;
					
					E[10].Nombre="Panning";
					E[10].Datos.N = 3;
					E[10].Datos.B[0]=0x01B0;
					E[10].Datos.B[1]=0x000A;
					E[10].Datos.B[2]=0x0200;
					*/
				}
				break;

				case 11:
				{
					/*
					E[11].Nombre="Express";
					E[11].Datos.N = 3;
					E[11].Datos.B[0]=0x01B0;
					E[11].Datos.B[1]=0x000B;
					E[11].Datos.B[2]=0x0200;
					*/
				}
				break;
				case 12:
				{
					/*
					E[12].Nombre="PitchHI";
					E[12].Datos.N = 3;
					E[12].Datos.B[0]=0x01E0;
					E[12].Datos.B[1]=0x0000;
					E[12].Datos.B[2]=0x0200;
					*/
				}
				break;
				case 13:
				{
					/*
					E[13].Nombre="PitchLO";
					E[13].Datos.N = 3;
					E[13].Datos.B[0]=0x01E0;
					E[13].Datos.B[1]=0x0200;
					E[13].Datos.B[2]=0x0000;
					*/
				}
				break;

				case 14:
				{
					/*
					E[14].Nombre="Reverb";
					E[14].Datos.N = 3;
					E[14].Datos.B[0]=0x01B0;
					E[14].Datos.B[1]=0x005B;
					E[14].Datos.B[2]=0x0200;
					*/
				}
				break;

				case 15:
				{
					/*
					E[15].Nombre="Chorus";
					E[15].Datos.N = 3;
					E[15].Datos.B[0]=0x01B0;
					E[15].Datos.B[1]=0x005D;
					E[15].Datos.B[2]=0x0200;
					*/
				}
				break;

				case 16:
				{
					/*
					E[16].Nombre="Modulat.";
					E[16].Datos.N = 3;
					E[16].Datos.B[0]=0x01B0;
					E[16].Datos.B[1]=0x0001;
					E[16].Datos.B[2]=0x0200;
					*/
				}
				break;

				case 17:
				{
					/*
					E[17].Nombre="Ch.Press";
					E[17].Datos.N = 3;
					E[17].Datos.B[0]=0x01D0;
					E[17].Datos.B[1]=0x0200;
					E[17].Datos.B[2]=0x0000;
					*/
				}
				break;


				default:
				{
					if (ConvertTCK2MIDIEffect!=NULL)
						ConvertTCK2MIDIEffect(EvT->Eff,EvT->Val,T,EvEff);
				}
			}
		}

		return(true);
	}
	else
		return(false);
}
//------------------------------------------------------------------------------
bool PistaVacia(PCancion S,TTrackNum T)
{
	int Max,I;
	bool Vacia;

	// No usar los patrones del vector de patrones para esto porque
	// hay veces en las que un patron puede no estar dentro de la secuencia
	// ejemplo: cuando tenemos secuencia 1 2 y cambiamos a 2 2: el patron 1 sigue estando

	Vacia=true;
	Max=NumPatronesEnSecuencia(S->Seq);
	I=0;
	while ((I<Max) && (Vacia))
	{
		Vacia=TrackVacio(&(PT98Pattern(ObtenerPatronDeSecuencia(S,I))->Tracks[T]));
		I++;
	}

	return(Vacia);
}
//------------------------------------------------------------------------------
void InicializarPunteros(PCancion C,TTrackNum T)
{
     IndiceSecuencia   = 0;
     BeatActual        = 0;
     BeatActualAbsoluto= 0;

     TiempoActualAbsoluto=0;

     PAct = ObtenerPatronDeSecuencia(C,IndiceSecuencia);
     TAct = LeerTrackDePattern(PAct,T);
}

// Actualiza los punteros teniendo en cuenta la posible existencia de un
// BreakPattern en la ultima linea procesada.
void IncrementarPunteros(PCancion S,TTrackNum T)
{
	PT98Pattern Pattern;

    // Mirar si en la linea de beats actual existe un efecto tipo BreakPattern
    Pattern=ObtenerPatronDeSecuencia(S,IndiceSecuencia);

    if (Pattern!=NULL) 
    {
		// Mirar si para el beat actual
		// en alguno de los tracks hay un EndOfPattern o EndOfModule
		int i = 0;
		while ((i<MaxTracks) &&
	  		   (LeerEffBeat(&(Pattern->Tracks[i]),BeatActual) !=3 ) &&
			   (LeerEffBeat(&(Pattern->Tracks[i]),BeatActual) !=2 )) i++;

		if (i<MaxTracks)
		{
			EndOfModule = (LeerEffBeat(&(Pattern->Tracks[i]),BeatActual) == 2);
			if (EndOfModule) return;

			// End of pattern mark, simulate we are at real end of the pattern
			BeatActual = MaxBeats;
		}	

		BeatActual++;
		BeatActualAbsoluto++;		// siempre se actualiza
	
		if (BeatActual >= MaxBeats)
		{
			IndiceSecuencia++;
			BeatActual    = 0;

			if (IndiceSecuencia<NumPatronesEnSecuencia(S->Seq))
			{
				PAct = ObtenerPatronDeSecuencia(S,IndiceSecuencia);
				TAct = LeerTrackDePattern(PAct,T);
			}
			else
				EndOfModule = true;
		}
    }	
}
//------------------------------------------------------------------------------
void EscribirDeltaTime(unsigned int DeltaTime,FILE *F)
{
	while (1)
	{
		BLOCKWRITE(F,&DeltaTime,1);
		if (DeltaTime & 0x80)			
			DeltaTime = DeltaTime >> 8;
		else
			break;
	}
}

void EscribirEventoMIDI(PEventoMIDI E,FILE *F)
{   
	EscribirDeltaTime(E->Time,F);
	BLOCKWRITE(F,E->D,E->Tam);
}

void EscribirEventoBeat(unsigned int Time,PEventoMIDI ENota,PEventoMIDI EEff,FILE *F)
{
     // Siempre se escribe primero el efecto y despues la Nota
     if (EEff->Tam >0) 
     {
          EEff->Time =Time;
          EscribirEventoMIDI(EEff,F);

          if (ENota->Tam>0)
          {
               ENota->Time = 0;
               EscribirEventoMIDI(ENota,F);
          }
     }
     else
     {
          ENota->Time = Time;
          EscribirEventoMIDI(ENota,F);
     }
}
//------------------------------------------------------------------------------
void EscribirCabeceraTrack(PCancion S,TTrackNum T,FILE *F)
{
	TEventoMIDI  EvM;

	// A parte de la informacion de cabecera, tambien se escribe la informacion
    // sobre el instrumento que utilizara este track y el banco que utilizara ...
	BLOCKWRITE(F,TRACK_ID,4);
	PosTamTrack = FILEPOS(F);

	// Dejar hueco para el tamanyo del track
	BLOCKWRITE(F,&SizeOfTrack,4);

	// Posicion de los datos del track
	PosDataTrack = FILEPOS(F);

	//Escribir eventos iniciales

	// Nombre del track
	EvM.Tam = strlen(InfTrackActual->Nombre.CString());
	memcpy(EvM.D,InfTrackActual->Nombre.CString(),EvM.Tam);
	EscribirMetaEvento(METAEVENT_TRACKNAME,&EvM,F);

	// Bancos:
	if (InfTrackActual->Ctrl0) 
	{
		EvM.Time = 0;
		EvM.D[0] = COMANDO_CONTROLCHANGE | InfTrackActual->Canal;
		EvM.D[1] = COMANDO_CAMBIOBANCO0;
		EvM.D[2] = InfTrackActual->Banco;
		EvM.Tam  = 3;
		EscribirEventoMIDI(&EvM,F);
	}

	if (InfTrackActual->Ctrl32) 
	{
		EvM.Time = 0;
		EvM.D[0] = COMANDO_CONTROLCHANGE | InfTrackActual->Canal;
		EvM.D[1] = COMANDO_CAMBIOBANCO32;
		EvM.D[2] = InfTrackActual->Banco;
		EvM.Tam  = 3;
		EscribirEventoMIDI(&EvM,F);
	}

	//Activar el programa:
	if (InfTrackActual->Inst != -1)
	{
		EvM.Time = 0;
		EvM.D[0] = COMANDO_CAMBIOPROGRAMA | InfTrackActual->Canal;
		EvM.D[1] = InfTrackActual->Inst;
		EvM.Tam  = 2;

		EscribirEventoMIDI(&EvM,F);
	}
}
//------------------------------------------------------------------------------
void InicializarVariablesTrack(PCancion S,TTrackNum T)
{
	EndOfModule		= false;
	LastNoteON		= 255;	
	CurrentBPMs		= S->BPMS;
    InfTrackActual	= LeerInfoDeTablaIns(S->TIns,T);
}
//------------------------------------------------------------------------------
void EscribirTrack(PCancion S,TTrackNum T,FILE *F)
{
	PDatosEvento	Ev;
	TEventoMIDI		EvNota,EvEff;
	unsigned int	Time;
	
	InicializarPunteros(S,T);
	InicializarVariablesTrack(S,T);

	// Escribir los datos iniciales de este track
	EscribirCabeceraTrack(S,T,F);

	while (! EndOfModule)
	{
		Ev = TomarEventoPorTiempoTrack(TAct,BeatActual);

		/*
		TODO:
		// Pudiera ser que se cambien los BPMs en otro track diferente a este y tambien en este
		for (int t=0;t<MaxTracks;t++)
		{
			if (LeerEffBeat(&(Pattern->Tracks[i]),BeatActual) == 4 )
			{
				CurrentBPMs	= 
			               while ((i<MaxTracks) &&
                      (LeerEffBeat(&(Pattern->Tracks[i]),BeatActual) !=3 ) &&
                      (LeerEffBeat(&(Pattern->Tracks[i]),BeatActual) !=2 )) i++;

			}
		}
		*/

		if (Ev)
		{
			if (EventoTCK_A_EventoMIDI(Ev,T,EvNota,EvEff))
			{
				// Esto indica que el Evento MIDI existe
				Time = TCKTimeAMIDITime(BeatActualAbsoluto,S->BPMS,TiempoActualAbsoluto);
				EscribirEventoBeat(Time,&EvNota,&EvEff,F);
			}

		}

		IncrementarPunteros(S,T);
	}
	
	// No dejar al final ninguna nota sonando.
	if (LastNoteON != 255)
	{
		EvNota.Tam  = 3;
		EvNota.D[0] = COMANDO_NOTEOFF | InfTrackActual->Canal;
		EvNota.D[1] = LastNoteON;
		EvNota.D[2] = 0;

		Time = TCKTimeAMIDITime(BeatActualAbsoluto,S->BPMS,TiempoActualAbsoluto);
		EscribirEventoBeat(Time,&EvNota,&EvEff,F);
	}

	// EnsOfTrack
	EscribirMetaEvento(METAEVENT_ENDOFTRACK,NULL,F);
	PosFinTrack = FILEPOS(F);
	
	SizeOfTrack = uiBIGENDIAN(PosFinTrack - PosDataTrack);

	// Escritura del tamanyo del track
	SEEK(F,PosTamTrack);
	BLOCKWRITE(F,&SizeOfTrack,4);

	// Posicionar el puntero al final del track
	SEEK(F,PosFinTrack);
}
//------------------------------------------------------------------------------
void EscribirCabeceraMIDI(PCancion S,FILE *f)
{
	unsigned int lon		= uiBIGENDIAN(6);		// longitud del bloque cabecera
	unsigned short format	= usBIGENDIAN(1);		// formato: tipo 1, #tracks variable
	unsigned short ntrks	= 1;					// 
	unsigned short division	= usBIGENDIAN(8);		// 

	// Contar tracks    
	for (int i=0;i<MaxTracks;i++)
		if (! PistaVacia(S,i))
			ntrks++;
	
	ntrks = usBIGENDIAN(ntrks);
     
	BLOCKWRITE(f,MIDI_ID	,4);					// Escritura del identificador de bloque
	BLOCKWRITE(f,&lon		,sizeof(unsigned int  ));
	BLOCKWRITE(f,&format	,sizeof(unsigned short));
	BLOCKWRITE(f,&ntrks		,sizeof(unsigned short));
	BLOCKWRITE(f,&division	,sizeof(unsigned short));
}
//------------------------------------------------------------------------------
void EscribirTrack0(PCancion S,FILE *F)
{
	TEventoMIDI EvM;
     
	// Este track se utiliza como MasterTrack, es decir, el que contiene informacion
	// importante acerca de la velocidad de la cancion, Key Signature ...
	BLOCKWRITE(F,TRACK_ID,4);
	PosTamTrack = FILEPOS(F);

	// Dejar hueco para el tamanyo del track
	BLOCKWRITE(F,&SizeOfTrack,4);

	// Posicion de los datos del track
	PosDataTrack = FILEPOS(F);

	// Set Key Signature : 4/4 : 8 MIDI clocks / quarter !e :
	EvM.Tam = 4; 
	EvM.D[0]= 4;
	EvM.D[1]= 2;
	EvM.D[2]= 8;
	EvM.D[3]= 8;
	EscribirMetaEvento(METAEVENT_TIMESIGNATURE,&EvM,F);


	// SetTempo
	EvM.Tam = 3;	*((unsigned int*)(EvM.D)) = ui3BIGENDIAN(60000000 / S->BPMS);
	EscribirMetaEvento(METAEVENT_SETTEMPO,&EvM,F);

	// EnsOfTrack
	EscribirMetaEvento(METAEVENT_ENDOFTRACK,NULL,F);
	PosFinTrack = FILEPOS(F);
	
	SizeOfTrack = uiBIGENDIAN(PosFinTrack - PosDataTrack);

	// Escritura del tamanyo del track
	SEEK(F,PosTamTrack);
	BLOCKWRITE(F,&SizeOfTrack,4);

	// Posicionar el puntero al final del track
	SEEK(F,PosFinTrack);
}
//------------------------------------------------------------------------------
bool EscribirFicheroMIDI_Interna(PCancion S,FILE *F)
{
	unsigned char i;
	
	EscribirCabeceraMIDI(S,F);
	EscribirTrack0(S,F);
	
	for (i=0;i<MaxTracks;i++)
		if (! PistaVacia(S,i))
			EscribirTrack(S,i,F);

	return(true);
}

/*------------------------------------------------------------------------------*/
bool EscribirFicheroMIDI(AnsiString Nombre,bool Sobreescribir)
{
    FILE *F;
	bool bRes;

    // Apertura del archivo
     ASSIGN(F,Nombre.c_str());

     RESET(F,1);
     if ((IORESULT==0) && (! Sobreescribir))
     {
        AnsiString asStr = AnsiString("El fichero ") + Nombre + AnsiString(" ya existe.\n¿Deseas reemplazarlo?");
        
		if (MessageBox(0,asStr.c_str(),gszAppTitle,MB_YESNO) == IDYES)
        {
			CLOSE(F);
			REWRITE(F,1);
            bRes = EscribirFicheroMIDI_Interna(VG.Sng,F);
        }
        else
            bRes =false;

        CLOSE(F);
     }
     else
     {
          REWRITE(F,1);
          if (IORESULT!=0)
          {
               MostrarError("ERROR: No se ha podido salvar el fichero con el tema.",false);
               bRes =false;
          }
          else
          {
               bRes = EscribirFicheroMIDI_Interna(VG.Sng,F);
               CLOSE(F);
          }
     };

     return(bRes);
};

//------------------------------------------------------------------------------
void EstablecerFuncionDeConversionTCK2Mid(TConvertTCK2MIDIEffect FuncionDeConversion)
{
     if (FuncionDeConversion!=NULL)
		ConvertTCK2MIDIEffect = FuncionDeConversion;
}
//------------------------------------------------------------------------------

