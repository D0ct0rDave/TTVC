//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#endif
#pragma hdrstop

#include "Parser.h"

// -----------------------------------------------------------------------------
/*
AnsiString LowerCase(AnsiString s)
{
    unsigned char i;
    AnsiString Aux;
    for (i=0;i<=s.Length();i++)
    {
        char a = s[i];

        if ((a >= 'A') && (a<='Z'))
            a -= 32;

        Aux += AnsiString(a);
    }

    return(Aux);
}
*/
// -----------------------------------------------------------------------------
const int ESPACIO   = 32;
const int TABULADOR = 9;

void SaltaEspacios(AnsiString S,unsigned char &Pos)
{
     while ((Pos < S.Length()) && ((S[Pos+1] == ESPACIO) || (S[Pos+1]==TABULADOR)) )
        Pos++;
}
// -----------------------------------------------------------------------------
void ObtenerPalabra(AnsiString S,unsigned char &Pos,AnsiString &Dest)
{
     Dest="";
     while ((Pos < S.Length()) && ((S[Pos+1] != ESPACIO) && (S[Pos+1]!=TABULADOR)) )
     {
          Dest += S[Pos+1];
          Pos++;
     }
}
// -----------------------------------------------------------------------------
void ObtenerFrase(AnsiString S,unsigned char &Pos,AnsiString &Dest)
{
     Dest = "";
     if (S[Pos+1] == '"') Pos++;

     while (( Pos < S.Length()) && (S[Pos+1] != '"'))
     {
          Dest += S[Pos+1];
          Pos++;
     }

     if (Pos < S.Length()) Pos++;
}
// -----------------------------------------------------------------------------
void ParseString(AnsiString S,TParsedStr &P)
{
    unsigned char LongStr;
    unsigned char Puntero;

     // Las cadenas pueden tener espacios o tabuladores
     // Los comentarios comienzan por #
     // Cuando se encuentra un par metro con "" se toma el interior como una sola palabra

     /*
     Mientras no final
        SaltaEspacios

        si primer_caracter = caracter_comentario -> retornar

        si primer_caracter = comillas -> ObtenerPalabraEntre comillas

        else
            obtener palabra
     end
     */

     LongStr   = S.Length();
     P.NumStrs = 0;
     Puntero   = 0;

     SaltaEspacios(S,Puntero);

     while ((Puntero<LongStr) && (P.NumStrs<MaxPalabras))
     {
          switch ( S[Puntero+1] )
          {
               case '#': return;               // Si es un comentario ya se ha acabado
               case '"': ObtenerFrase(S,Puntero,P.Strs[P.NumStrs]);

          default:
              ObtenerPalabra(S,Puntero,P.Strs[P.NumStrs]);
          }

          P.NumStrs++;
          SaltaEspacios(S,Puntero);
     }
}
// -----------------------------------------------------------------------------
