#ifndef MiscInsH
#define MiscInsH

#include "StrList.h"
#include "PascalCnv.h"

         void InicializarLista(unsigned char Instrumento,unsigned char Banco);
         void CargarLista(unsigned char Instrumento,unsigned char Banco,AnsiString Archivo);
         void ObtenerListaInstrumentos(unsigned char Instrumento,unsigned char Banco,TStrList *L);
         void ObtenerListaBancos(unsigned char Instrumento,TStrList *L);
         AnsiString TomarNombreBanco(unsigned char Instrumento,unsigned char Banco);
#endif
