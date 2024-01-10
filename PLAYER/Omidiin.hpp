// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OMidiIn.pas' rev: 6.00

#ifndef OMidiInHPP
#define OMidiInHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Omidiin
{
//-- type declarations -------------------------------------------------------
#pragma pack(push, 4)
struct TEventoMidiIn
{
	Byte Cmd;
	Byte Data1;
	Byte Data2;
	void *Sysex;
	int TamSysex;
} ;
#pragma pack(pop)

;

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE int __fastcall NumeroDispositivosMIDIIn(void);
extern PACKAGE AnsiString __fastcall ObtenerNombreDispositivoMidiIn(int Dispositivo);
extern PACKAGE void __fastcall ObtenerListaDispositivosMIDIIn(Classes::TStrings* S);

}	/* namespace Omidiin */
using namespace Omidiin;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OMidiIn
