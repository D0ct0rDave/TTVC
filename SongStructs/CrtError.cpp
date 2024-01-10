//---------------------------------------------------------------------------
#include "CrtError.h"
#include <windows.h>

char *gszAppTitle = NULL;

void ShowMessage(AnsiString S)
{
	MostrarError(S,false);
}

void MostrarError(AnsiString S,bool Salir)
{
	if (gszAppTitle)	
		MessageBox(0,S.c_str(),gszAppTitle,MB_OK);
	else
		MessageBox(0,S.c_str(),"App",MB_OK);
     
	if (Salir) exit(1);
};

