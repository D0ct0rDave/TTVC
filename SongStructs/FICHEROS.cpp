//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#include "Ficheros.h"
#include <windows.h>

#include "CrtError.h"
#include "consts2.h"
#include "Tipos.h"
#include "Inter.h"
#include "Bloques.h"
#include "Variables.h"
#include "InfoPistas.h"

// WARNING: Uncompletecode
// #include "TckAMid.h"

// #include <Controls.hpp>
// #include <Dialogs.hpp>

#include "PascalCnv.h"
bool bRes = false;
/*------------------------------------------------------------------------------*/
bool CargarTablaDeInstrumentosMIDI(AnsiString Nombre)
{
    FILE *F;

      // Apertura del archivo
      ASSIGN(F,Nombre.c_str());

      RESET(F,1);
      if (IORESULT==0)
      {
           bRes = CargarCancionGeneral(VG.Sng,F);
           CLOSE(F);
      }
      else
      {
           MostrarError(AnsiString("ERROR: Error al leer el fichero ") + Nombre,false);
           bRes = false;
      };
    return(bRes);
};
/*------------------------------------------------------------------------------*/
bool SalvarTablaDeInstrumentosMIDI(AnsiString Nombre,bool Sobreescribir)
{
    ShowMessage("WARNING: Uncomplete code");
    return(false);

    /*
    FILE *F;
    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());

     RESET(F,1);
     if ((IORESULT==0) && (! Sobreescribir))
     {
          if (MessageDlg("El fichero "+Nombre+" ya existe. \n¿Deseas reemplazarlo?",mtConfirmation,[mbYes,mbNo],0)=mrYes)
               bRes  = SalvarCancionGeneral(VG.Sng,F)
          else
              bRes = false;

          CLOSE(F);
     }
     else
     {
          REWRITE(F,1);
          if (IORESULT!=0)
          {
               MostrarError("ERROR: Error al intentar escribir en el dispositivo.",false);
               bRes=false;
          }
          else
          {
               bRes=SalvarCancionGeneral(Vg.Sng,F);
               CLOSE(F);
          }
     };
    */
    return(bRes);
};
/*------------------------------------------------------------------------------*/
bool CargarSecuencia(AnsiString Nombre)
{
    ShowMessage("WARNING: Uncomplete code");
    return(false);

    /*
    FILE *F;
    unsigned short ID;
    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());

      RESET(F,1);
      if (IORESULT==0)
      {
           CargarSecuencia=CargarSecuenciaGeneral(VG.Sng.Seq,F,ID);
           CLOSE(F);
      }
      else
      {
           MostrarError("ERROR: Error al leer el fichero "+Nombre,false);
           CargarSecuencia=false;
      };
    */
    return(bRes);
};
/*------------------------------------------------------------------------------*/
bool SalvarSecuencia(AnsiString Nombre,bool Sobreescribir)
{
    ShowMessage("WARNING: Uncomplete code");
    return(false);

    /*
    FILE *F;

    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());

     RESET(F,1);
     if (IORESULT==0) && (! Sobreescribir)
     {
          if (MessageDlg("El fichero "+Nombre+" ya existe."+CHR(10)+CHR(13)+
                      "¿Deseas reemplazarlo?",mtConfirmation,[mbYes,mbNo],0)=mrYes)
               SalvarSecuencia=SalvarSecuenciaGeneral(VG.Sng.Seq,F,65535)
          else
              SalvarSecuencia=false;

          CLOSE(F);
     }
     else
     {
          REWRITE(F,1);
          if (IORESULT!=0)
          {
               MostrarError("ERROR: Error al intentar escribir en el dispositivo.",false);
               SalvarSecuencia=false;
          }
          else
          {
               SalvarSecuencia=SalvarSecuenciaGeneral(VG.Sng.Seq,F,65535);
               CLOSE(F);
          }
     };
     */
    return(bRes);
};
/*------------------------------------------------------------------------------*/
bool CargarCancion(AnsiString Nombre)
{
    FILE *F;

	// Apertura del archivo
	ASSIGN(F,Nombre.c_str());
	RESET(F,1);
	
    if (IORESULT==0)
    {
		bRes            = CargarCancionGeneral(VG.Sng,F);
        VG.PatronActual = VG.Sng->Pats[VG.PatternActual];
        VG.PistaActual  = &(VG.PatronActual->Tracks[VG.AbsTrackActual]);
        CLOSE(F);
	}
    else
    {
		MostrarError("ERROR: No se ha podido cargar el fichero con el tema.",false);
        bRes=false;
	};

    return(bRes);
};
/*------------------------------------------------------------------------------*/
bool SalvarCancion(AnsiString Nombre,bool Sobreescribir)
{
    FILE *F;

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
            bRes =SalvarCancionGeneral(VG.Sng,F);
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
               bRes =SalvarCancionGeneral(VG.Sng,F);
               CLOSE(F);
          }
     };
     
     return(bRes);
};
/*------------------------------------------------------------------------------*/
bool CargarPatron(AnsiString Nombre)
{
    FILE *F;
    unsigned short ID;
    
    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());


      RESET(F,1);
      if (IORESULT==0) 
      {
           bRes = CargarPatternGeneral(VG.Sng->Pats[VG.PatternActual],F,ID);
           VG.PatronActual = VG.Sng->Pats[VG.PatternActual];
           VG.PistaActual  = &(VG.PatronActual->Tracks[VG.AbsTrackActual]);

           VG.PistaActual = &(VG.PatronActual->Tracks[VG.AbsTrackActual]);
           CLOSE(F);
      }
      else
      {
           MostrarError("ERROR: No se ha podido cargar el fichero con el patrón.",false);
           bRes =false;
      };

      return(bRes);
};
/*------------------------------------------------------------------------------*/
bool SalvarPatron(AnsiString Nombre,bool Sobreescribir)
{
    FILE *F;

    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());

     RESET(F,1);
     if ((IORESULT==0) && (! Sobreescribir))
     {
        AnsiString asStr = AnsiString("El fichero ") + Nombre + AnsiString(" ya existe.\n¿Deseas reemplazarlo?");
        
		if (MessageBox(0,asStr.c_str(),gszAppTitle,MB_YESNO) == IDYES)
               bRes = SalvarPatternGeneral(VG.PatronActual,F,65535);
        else
              bRes = false;


          CLOSE(F);
     }
     else
     {
          REWRITE(F,1);
          if (IORESULT!=0)
          {
               MostrarError("ERROR: No se ha podido salvar el fichero con el patrón.",false);
               bRes = false;
          }
          else
          {
               bRes = SalvarPatternGeneral(VG.PatronActual,F,65535);
               CLOSE(F);
          }
     };
     return(bRes);
};
/*------------------------------------------------------------------------------*/
bool CargarPista(AnsiString Nombre)
{
    FILE *F;
    unsigned short ID;
    
    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());

      RESET(F,1);
      if (IORESULT==0)
      {
           bRes = CargarTrackGeneral(VG.PistaActual,F,ID);
           CLOSE(F);
      }
      else
      {
           MostrarError("ERROR: No se ha podido cargar el fichero con la pista.",false);
           bRes = false;
      };
      return(bRes);
};
/*------------------------------------------------------------------------------*/
bool SalvarPista(AnsiString Nombre,bool Sobreescribir)
{
    FILE *F;

    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());

     RESET(F,1);
     if ((IORESULT==0) && (! Sobreescribir))
     {
        AnsiString asStr = AnsiString("El fichero ") + Nombre + AnsiString(" ya existe.\n¿Deseas reemplazarlo?");
        if (MessageBox(0,asStr.c_str(),"TT98",MB_YESNO) == IDYES)
               bRes = SalvarTrackGeneral(VG.PistaActual,F,65535);
          else
              bRes = false;

          CLOSE(F);
     }
     else
     {
          REWRITE(F,1);
          if (IORESULT!=0)
          {
               MostrarError("ERROR: No se ha podido salvar el fichero con la pista.",false);
               bRes = false;
          }
          else
          {
               bRes = SalvarTrackGeneral(VG.PistaActual,F,65535);
               CLOSE(F);
          }
     };

    return(bRes);
};
//--------------------------------------------------------------------------------
bool CargarBloque(AnsiString Nombre)
{
    FILE *F;
    unsigned short ID;
    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());

      RESET(F,1);
      if (IORESULT==0)
      {
           bRes = CargarBloqueGeneral(VG.BloqueAux2,F,ID);
           CLOSE(F);
      }
      else
      {
           MostrarError("ERROR: No se ha podido cargar el fichero con el bloque.",false);
           bRes = false;
      };
      return(bRes);
};
//--------------------------------------------------------------------------------
bool SalvarBloque(AnsiString Nombre,bool Sobreescribir)
{
    FILE *F;

    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());
     RESET(F,1);
     if ((IORESULT==0) && (! Sobreescribir))
     {
        AnsiString asStr = AnsiString("El fichero ") + Nombre + AnsiString(" ya existe.\n¿Deseas reemplazarlo?");
        if (MessageBox(0,asStr.c_str(),"TT98",MB_YESNO) == IDYES)
              bRes = SalvarBloqueGeneral(VG.BloqueAux2,F,65535);
          else
              bRes = false;

          CLOSE(F);
     }
     else
     {
          REWRITE(F,1);
          if (IORESULT!=0)
          {
               MostrarError("ERROR: No se ha podido salvar el fichero con el bloque.",false);
               bRes = false;
          }
          else
          {
               bRes = SalvarBloqueGeneral(VG.BloqueAux2,F,65535);
               CLOSE(F);
          }
     };
     return(bRes);
};
//--------------------------------------------------------------------------------
bool CargarListaEfectos(AnsiString Nombre,TStrList &L)
{
    ShowMessage("WARNING: Uncomplete code");
    return(false);
   /*
   T:TextFile;
   Aux:String;
   i:int;

     // Apertura del archivo
     ASSIGN(T,Nombre);
     Reset(T);

     if (IORESULT==0)
     {
          i=0;
          L.Clear;
          while (! Eof(T)) && (i<256) DO
          {
               ReadLn(T,Aux);
               L.Add(Aux);
               inc(i);
          };

          CLOSE(T);
          if ((i=256) && (! Eof(T)))
             ShowMessage("El fichero contenia más de 256 lineas, y ha sido truncado");

          CargarListaEfectos=true;
      }
      else
      {
           MostrarError("ERROR: No se ha podido cargar el fichero con la lista de Efectos.",false);
           CargarListaEfectos=false;
      };
     */
     return(bRes);
};

bool SalvarListaEfectos(AnsiString Nombre,TStrList &L,bool Sobreescribir)
{
    ShowMessage("WARNING: Uncomplete code");
    return(false);

/*
VAR
   T:TextFile;
   Aux:String;
   i:int;

   void SalvarListaEfectosAux(L:TStrList;VAR T:TEXT);
   var
      i:short;
   {
        for i=0 <= L.Count-1 do
            writeln(T,L.Strings(i));
   };

{
     // Apertura del archivo
     AssignFile(T,Nombre);
     RESET(T);

     if (IORESULT==0) && (! Sobreescribir)
     {
        AnsiString asStr = "El fichero " + Nombre + " ya existe.\n¿Deseas reemplazarlo?";
        if (MessageBox(0,asStr.c_str(),"TT98",MB_YESNO) == IDYES)
          {
                      SalvarListaEfectosAux(L,T);
                      SalvarListaEfectos=true;
          }
          else
              SalvarListaEfectos=false;

          CLOSE(T);
     }
     else
     {
          REWRITE(T);
          if (IORESULT!=0)
          {
               MostrarError("ERROR: No se ha podido salvar el fichero con el bloque.",false);
               SalvarListaEfectos=false;
          }
          else
          {
               SalvarListaEfectosAux(L,T);
               CLOSE(T);

               SalvarListaEfectos=true;
          }
     };
     */
     return(bRes);
};

bool CargarTablaEfectos(AnsiString Nombre,unsigned char I)
{
    ShowMessage("WARNING: Uncomplete code");
    return(false);

/*
    FILE *F;

    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());

      RESET(F,1);
      if (IORESULT==0)
      {
           CargarTablaEfectos=VG.ListaEfectos[i].CargarTablaEfectos(F);
           CLOSE(F);
      }
      else
      {
           MostrarError("ERROR: Error al leer el fichero "+Nombre,false);
           CargarTablaEfectos=false;
      };
    */
    return(bRes);
};
/*------------------------------------------------------------------------------*/
bool SalvarTablaEfectos(AnsiString Nombre,unsigned char I,bool Sobreescribir)
{
    ShowMessage("WARNING: Uncomplete code");
    return(false);

/*

    FILE *F;

    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());
     RESET(F,1);
     if (IORESULT==0) && (! Sobreescribir)
     {
        AnsiString asStr = "El fichero " + Nombre + " ya existe.\n¿Deseas reemplazarlo?";
        if (MessageBox(0,asStr.c_str(),"TT98",MB_YESNO) == IDYES)
               SalvarTablaEfectos=VG.ListaEfectos[i].SalvarTablaEfectos(F)
          else
              SalvarTablaEfectos=false;

          CLOSE(F);
     }
     else
     {
          REWRITE(F,1);
          if (IORESULT!=0)
          {
               MostrarError("ERROR: Error al intentar escribir en el dispositivo.",false);
               SalvarTablaEfectos=false;
          }
          else
          {
               SalvarTablaEfectos=VG.ListaEfectos[i].SalvarTablaEfectos(F);
               CLOSE(F);
          }
     };
    */
    return(bRes);
};
/*------------------------------------------------------------------------------*/
bool SalvarMidifile(AnsiString Nombre,bool Sobreescribir)
{
    ShowMessage("WARNING: Uncomplete code");
    return(false);

/*

    FILE *F;

    // Apertura del archivo
    ASSIGN(F,Nombre.c_str());

     RESET(F,1);
     if (IORESULT==0) && (! Sobreescribir)
     {
        AnsiString asStr = "El fichero " + Nombre + " ya existe.\n¿Deseas reemplazarlo?";
        if (MessageBox(0,asStr.c_str(),"TT98",MB_YESNO) == IDYES)
              SalvarMidifile=EscribirFicheroMidi(VG.Sng,F)
          else
              SalvarMidifile=false;

          CLOSE(F);
     }
     else
     {
          REWRITE(F,1);
          if (IORESULT!=0) 
          {
               MostrarError("ERROR: No se ha podido salvar el fichero midi.",false);
               SalvarMidifile=false;
          }
          else
          {
               SalvarMidifile=EscribirFicheroMidi(VG.Sng,F);
               CLOSE(F);
          }
     };
     */
    return(bRes);
};
/*------------------------------------------------------------------------------*/
