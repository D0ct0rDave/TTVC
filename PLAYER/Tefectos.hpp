// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TEfectos.pas' rev: 6.00

#ifndef TEfectosHPP
#define TEfectosHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tefectos
{
//-- type declarations -------------------------------------------------------
struct TBufferSysEx;
typedef TBufferSysEx *PBufferSysEx;

#pragma pack(push, 1)
struct TBufferSysEx
{
	Byte Data[32];
	Byte Num;
} ;
#pragma pack(pop)

struct TEfectoMIDI;
typedef TEfectoMIDI *PEfectoMidi;

#pragma pack(push, 2)
struct TEfectos__1
{
	Word b[32];
	Byte n;
} ;
#pragma pack(pop)

#pragma pack(push, 2)
struct TEfectoMIDI
{
	System::SmallString<8>  Nombre;
	TEfectos__1 Datos;
} ;
#pragma pack(pop)

typedef /*???*/ *PEfectosMidi;

;

//-- var, const, procedure ---------------------------------------------------
static const Shortint MaxBytesEfecto = 0x20;
static const Shortint MaxEfectosPropios = 0x20;
static const Shortint NumeroDeEfectos = 0xe;
static const Word MaxEfectos = 0x100;
static const Shortint MaxCarNombreEfecto = 0x8;
static const Shortint MaxCarNombreConfig = 0x28;

}	/* namespace Tefectos */
using namespace Tefectos;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TEfectos
