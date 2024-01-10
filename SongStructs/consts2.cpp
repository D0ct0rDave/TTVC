//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#include "Consts2.h"

     // Identificadores
     char *VersionID ="2.00";

     char *AUTOR_POR_DEFECTO ="Techno Autor";
     char *TITULO_POR_DEFECTO="Techno Tema ";
     
     // Lista de los Efectos permitidos
     char *ListaDeEfectos[MaxEfectosPropios] = {
			   "        ", // 00
                "Note Off",
                "EndOfMod",
                "PatBreak",
                "IncTempo",
                "DecTempo",
                "BPMS*2  ",
                "BPMS/2  ",
                "        ",
                "PrChange",
                "Panning ", // 10
                "Express.",
                "PitchHI ",
                "PitchLO ",
                "Reverb  ",
                "Chorus  ",
                "Modulat.",
                "Ch.Press",
                "        ",
                "        ",
                "        ", // 20
                "        ",
                "        ",
                "        ",
                "        ",
                "        ",
                "        ",
                "        ",
                "        ",
                "        ",
                "        ", // 30
                "        "
		};