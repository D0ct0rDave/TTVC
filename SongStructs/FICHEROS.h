#ifndef FicherosH
#define FicherosH

#include "PascalCnv.h"
#include "StrList.h"

bool CargarTablaDeInstrumentosMIDI(AnsiString Nombre);
bool SalvarTablaDeInstrumentosMIDI(AnsiString Nombre,bool Sobreescribir);

bool CargarSecuencia(AnsiString Nombre);
bool SalvarSecuencia(AnsiString Nombre,bool Sobreescribir);

bool CargarTablaEfectos(AnsiString Nombre,unsigned char I);
bool SalvarTablaEfectos(AnsiString Nombre,unsigned char I,bool Sobreescribir);

bool CargarListaEfectos(AnsiString Nombre,TStrList &L);
bool SalvarListaEfectos(AnsiString Nombre,TStrList &L,bool Sobreescribir);

bool CargarBloque(AnsiString Nombre);
bool SalvarBloque(AnsiString Nombre,bool Sobreescribir);

bool CargarPista(AnsiString Nombre);
bool SalvarPista(AnsiString Nombre,bool Sobreescribir);

bool CargarPatron(AnsiString Nombre);
bool SalvarPatron(AnsiString Nombre,bool Sobreescribir);

bool CargarCancion(AnsiString Nombre);
bool SalvarCancion(AnsiString Nombre,bool Sobreescribir);

bool SalvarMidifile(AnsiString Nombre,bool Sobreescribir);


#endif