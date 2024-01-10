//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#include "RList.h"
//--------------------------------------------------------------------------------
void TColaStr::Inicializar(unsigned char MaxElem)
{
     if (MaxElem<MaxStringsEnCola)
     {
          NumStr = MaxElem;
          NumStrEnCola = 0;
     };
};
//--------------------------------------------------------------------------------
unsigned char TColaStr::EstaElemento(AnsiString elem)
{
   unsigned char i;
   bool encontrado;

     encontrado=false;
     i=0;
     while ((i<NumStrEnCola) &&  !(encontrado))
     {
          encontrado = (Strs[i]==elem);
          if (! encontrado) i++;
     };

     return(i);
};
//--------------------------------------------------------------------------------
void TColaStr::EliminarElemento(unsigned char i)
{
    unsigned char j;
     if ((NumStrEnCola>0) && (i<NumStrEnCola))
     {
          for (j=i+1;j<=NumStrEnCola-1;j++)
              Strs[j-1]=Strs[j];

          NumStrEnCola = NumStrEnCola-1;
     };
};
//--------------------------------------------------------------------------------
void TColaStr::InsertarElemento(unsigned char i)
{
    int j;
    if (NumStrEnCola==NumStr)
        EliminarElemento(NumStr-1);

    if ((NumStrEnCola > 0) && (i < NumStrEnCola))
    {
        for (j=NumStrEnCola-1; j>=i;j--)
            Strs[j+1]=Strs[j];
    };

    NumStrEnCola++;
};
//--------------------------------------------------------------------------------
void TColaStr::EscribirElemento(unsigned char i,AnsiString s)
{
     Strs[i]=s;
};
//--------------------------------------------------------------------------------
AnsiString TColaStr::LeerElemento(unsigned char i)
{
     return ( Strs[i] );
};

//--------------------------------------------------------------------------------
void TRecentList::Inicializar(unsigned char MaxFicheros)
{
     C.Inicializar(MaxFicheros);
};
//--------------------------------------------------------------------------------
AnsiString TRecentList::LeerElemento(unsigned char i)
{
     return( C.LeerElemento(i) );
};
//--------------------------------------------------------------------------------
void TRecentList::AnyadirElemento(AnsiString s)
{
    unsigned char Pos;
    Pos = C.EstaElemento(s);

    if (Pos < C.NumStrEnCola)
        C.EliminarElemento(Pos);

    C.InsertarElemento(0);
    C.EscribirElemento(0,s);
};
//--------------------------------------------------------------------------------
