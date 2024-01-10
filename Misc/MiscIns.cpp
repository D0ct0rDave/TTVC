//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#include "MiscIns.h"

#include "consts2.h"
#include <stdlib.h>
#include <string.h>

    typedef struct TCelda* PCelda;
    typedef struct TCelda{
        unsigned char   Numero;
        AnsiString      Nombre;
        PCelda          Next;
    }TCelda;

    typedef PCelda TListaInstrumentos;

    typedef struct
    {
         AnsiString         Nombre;
         TListaInstrumentos L;
    }TListaInstBank;

    TListaInstBank Listas[MaxDispositivosMIDIOut][MaxBancos];
/*------------------------------------------------------------------------------*/
void DestruirLista(TListaInstrumentos &L)
{
   PCelda Next;
     while (L!=NULL)
     {
          Next=L->Next;
          free(L);
          L=Next;
     };
};
/*------------------------------------------------------------------------------*/
void InsertarElemento(unsigned char Numero,AnsiString Nombre,TListaInstrumentos &L)
{
    PCelda p;

    p = new TCelda;
    p->Next=L;
    p->Nombre = Nombre;
    p->Numero = Numero;
    L = p;
};
/*------------------------------------------------------------------------------*/
char *UpperCase(char *_szStr)
{
	if (! _szStr) return(NULL);
	char *szLetter = _szStr;

	while (*szLetter)
	{
		if ((*szLetter >= 'a') && (*szLetter <= 'z'))
			*szLetter -= 32;
		
		szLetter++;
	}

	return(_szStr);
}

void CargarLista(unsigned char Instrumento,unsigned char Banco,AnsiString Archivo)
{
	FILE*			T;
	int				I;
	char			szStr[1024];
	unsigned char	Numero;
	AnsiString		Nombre;
	short			Code;

	if ((Instrumento<MaxDispositivosMIDIOut) && (Banco<MaxBancos))
    {
		T = fopen(Archivo.c_str(),"rt");
        if (! T) return;

        if (Listas[Instrumento][Banco].L != NULL)
        {
			DestruirLista(Listas[Instrumento][Banco].L);
            Listas[Instrumento][Banco].Nombre="";
        }

        while ( fgets(szStr,1024,T) && (I<256) )
        {
			if (szStr[strlen(szStr)-1] == '\n') // erase newline char
				szStr[strlen(szStr)-1] = 0;

            if ((szStr[0] != '#') && (szStr[0]))
            {
				Nombre = AnsiString( UpperCase(szStr) );

                if (Nombre.Pos("NOMBRE : ")==0)
                {
					// Se trata del nombre del banco
                    Nombre = Nombre.SubString(9,Nombre.Length()-9);
                    Listas[Instrumento][Banco].Nombre = Nombre;
                }                   // Se debe tratar de un instrumento
           else if (Nombre.Pos(" : ")==3)
				{					
					// Es posible que sea un instrumento
					AnsiString asNum = Nombre.SubString(0,3);
                    sscanf( asNum.c_str(),"%d",&Numero);

                    if (Numero>0)
                    {
						// Operacion válida
                        // Nombre=Copy(Aux,7,Length(Aux)-6); El nombre debe incluir el
                        // numero para tener una referencia de posicion
                        InsertarElemento(Numero,Nombre,Listas[Instrumento][Banco].L);
                    };
                };
			};

            I++;
		}

		fclose(T);
	};
}
/*------------------------------------------------------------------------------*/
void ObtenerListaInstrumentos(unsigned char Instrumento,unsigned char Banco,TStrList *L)
{
	short I;
	PCelda Next;

    /*
		Hay DOS tipos:
			listas desconocidas : lista vacia
			listas conocidas    : lista no vacia 
	*/

    L->Clear();
    
	for (I=1;I<=MaxInstrumentosMIDI;I++)
		L->Add( IntToStr(I) + " ---" );

	if ((Instrumento<MaxDispositivosMIDIOut) && (Banco<MaxBancos))
    {
		if (Listas[Instrumento][Banco].L != NULL)
        {
			Next = Listas[Instrumento][Banco].L;

            while (Next!=NULL)
            {
				L->Strings(Next->Numero-1) = Next->Nombre;
                Next = Next->Next;
            };
        };
    };
};
/*------------------------------------------------------------------------------*/
void ObtenerListaBancos(unsigned char Instrumento,TStrList *L)
{
   short I;
     if (Instrumento < MaxDispositivosMIDIOut)
     {
          L->Clear();
          for (I=0;I<=MaxBancos-1;I++)
              if (Listas[Instrumento][I].L!=NULL)
                  L->Add(IntToStr(I) + " : " + Listas[Instrumento][I].Nombre );
              else
                  L->Add(AnsiString("Banco ") + IntToStr(I));
     };
};
/*------------------------------------------------------------------------------*/
AnsiString TomarNombreBanco(unsigned char Instrumento,unsigned char Banco)
{
     if ((Instrumento<MaxDispositivosMIDIOut) && (Banco<MaxBancos))
     {

        if (Listas[Instrumento][Banco].Nombre=="")
           return( AnsiString("Banco ") + IntToStr(Banco) );
        else
			return(IntToStr(Banco) + " : " + Listas[Instrumento][Banco].Nombre);
     }
     else
         return ("");
};
/*------------------------------------------------------------------------------*/
void InicializarLista(unsigned char Instrumento,unsigned char Banco)
{
     DestruirLista(Listas[Instrumento][Banco].L);
     Listas[Instrumento][Banco].Nombre="";
};
/*------------------------------------------------------------------------------*/
