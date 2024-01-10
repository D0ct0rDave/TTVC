// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Consts2.pas' rev: 6.00

#ifndef Consts2HPP
#define Consts2HPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Consts2
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE char VersionID[4];
static const Shortint MaxTracks = 0x40;
static const Byte MaxBeats = 0x80;
static const Word MaxPatterns = 0x100;
static const Word MaxPatternsSequence = 0x100;
static const Shortint MaxDispositivosMIDIOut = 0x10;
static const Byte MaxInstrumentosMIDI = 0x80;
static const Shortint MaxBancos = 0x40;
static const Word MaxBPMS = 0x1f4;
static const Shortint MinBPMS = 0x32;
static const Shortint MaxSaltoLinea = 0x40;
static const Shortint MaxAcorde = 0x6;
static const Shortint MaxArpegio = 0x10;
#define AUTOR_POR_DEFECTO "Techno Autor"
#define TITULO_POR_DEFECTO "Techno Tema "
static const Byte BPMS_POR_DEFECTO = 0x91;
static const Shortint MaxBytesEfecto = 0x10;
static const Shortint MaxEfectosPropios = 0x20;
static const Shortint NumeroDeEfectos = 0xe;
static const Word MaxEfectos = 0x100;
static const Shortint MaxCarNombreEfecto = 0x8;
static const Shortint MaxCarNombreConfig = 0x28;
extern PACKAGE System::SmallString<8>  ListaDeEfectos[32];
static const Shortint MaxCarNombreCancion = 0x50;
static const Shortint MaxCarNombreAutor = 0x50;
static const Shortint MaxCarNombrePatron = 0x50;

}	/* namespace Consts2 */
using namespace Consts2;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Consts2
