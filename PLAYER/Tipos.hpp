// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Tipos.pas' rev: 6.00

#ifndef TiposHPP
#define TiposHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Consts2.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tipos
{
//-- type declarations -------------------------------------------------------
typedef Word TBeatNum;

typedef Word TTrackNum;

typedef Word TPatternNum;

typedef Word TIndexSec;

typedef Word TIndexTIns;

typedef AnsiString BeatString;

typedef AnsiString Ltt98String;

typedef SmallString<80>  tt98String;

typedef SmallString<35>  Stt98String;

struct TInfoTrack;
typedef TInfoTrack *PInfoTrack;

#pragma pack(push, 1)
struct TInfoTrack
{
	Stt98String Nombre;
	Byte Disp;
	Byte Banco;
	Byte Inst;
	Byte Canal;
	Byte VolDef;
	bool Ctrl0;
	bool Ctrl32;
} ;
#pragma pack(pop)

struct TTablaIns;
typedef TTablaIns *PTablaIns;

#pragma pack(push, 1)
struct TTablaIns
{
	TInfoTrack T[64];
} ;
#pragma pack(pop)

struct TSecuencia;
typedef TSecuencia *PSecuencia;

#pragma pack(push, 2)
struct TSecuencia
{
	Word S[256];
	Word Num;
} ;
#pragma pack(pop)

struct TDatosEvento;
typedef TDatosEvento *PDatosEvento;

#pragma pack(push, 1)
struct TDatosEvento
{
	Byte Nota;
	Byte Vol;
	Byte Eff;
	Byte Val;
} ;
#pragma pack(pop)

struct TTrack;
typedef TTrack *PTrack;

#pragma pack(push, 2)
struct TTrack
{
	TDatosEvento PE[128];
	Word NumEventos;
} ;
#pragma pack(pop)

struct TPattern;
typedef TPattern *PPattern;

#pragma pack(push, 2)
struct TPattern
{
	tt98String Nombre;
	TTrack Tracks[64];
} ;
#pragma pack(pop)

struct TCancion;
typedef TCancion *PCancion;

#pragma pack(push, 4)
struct TCancion
{
	tt98String Titulo;
	tt98String Autor;
	Word BPMS;
	TTablaIns *TIns;
	TSecuencia *Seq;
	TPattern *Pats[256];
} ;
#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE tt98String Error;

}	/* namespace Tipos */
using namespace Tipos;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Tipos
