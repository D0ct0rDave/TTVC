UNIT OMIDIOUT;

INTERFACE
uses
    windows,classes,mmSystem;
type
    PObjetoMidiOut =^TObjetoMidiOut;

    TObjetoMidiOut = object
                          Abierto:Boolean;

                          constructor init;
                          procedure Open(FuncionCallBack:pointer);
                          procedure Close;

                          procedure Comando(C,V1,V2:BYTE);
                          procedure SysEx(Buffer:POINTER;Num:LONGINT);

                          procedure EstablecerDispositivo(D:integer);
                          function Dispositivo:Integer;

                          Destructor Destruir;

                          private

                          Handler:Integer;
                          DispositivoActual:Integer;

                          BufferSysEx : POINTER;
                          BufferSysExHdr : PMIDIHDR;

                          BufferSysExHnd : THandle;
                          BufferSysExHdrHnd : THandle;
                    end;

    FUNCTION NumeroDispositivosMIDIOut:Integer;
    PROCEDURE ObtenerListaDispositivosMIDIOut(S:TStrings);
    FUNCTION ObtenerNombreDispositivoMidiOut(Dispositivo:integer):String;

implementation

uses
    SysUtils ,Smem;

function NumeroDispositivosMIDIOut:Integer;
begin
     NumeroDispositivosMIDIOut:=MidiOutGetNumDevs;
end;

FUNCTION ObtenerNombreDispositivoMidiOut(Dispositivo:integer):String;
VAR
   M:TMidiOutCaps;
   FError:Integer;
BEGIN
     FError :=midiInGetDevCaps(Dispositivo,@M,sizeof(TMidiInCaps));
     if (Ferror=MMSYSERR_NOERROR) then
        ObtenerNombreDispositivoMidiOut:=StrPas(M.szPName)
     else
         ObtenerNombreDispositivoMidiOut:='';
END;

procedure ObtenerListaDispositivosMIDIOut(S:TStrings);
var
   j,i:integer;
   MidiOutCaps:TMidiOutCaps;
begin
     j:=MidiOutGetNumDevs;

     S.Clear;
     for i:=0 to j-1 do
     begin
          midiOutGetDevCaps(i,@MidiOutCaps,sizeof(TMidiOutCaps));
          S.Add(StrPas(MidiOutCaps.szPname));
     end;
end;
{------------------------------------------------------------------------------}
constructor TObjetoMidiOut.init;
begin
     Abierto:=False;

     {Tomar memoria para el Buffer de SysEx}
     BufferSysEx    := GlobalAllocLockedMem(1024,BufferSysExHnd);
     BufferSysExHdr := GlobalAllocLockedMem(sizeof(TMIDIHDR), BufferSysExHdrHnd);

     BufferSysExHdr^.lpData        := BufferSysEx;
     BufferSysExHdr^.dwBufferLength:= 1024;
     BufferSysExHdr^.dwUser        := 0;
end;

Destructor TObjetoMidiOut.Destruir;
BEGIN
     GlobalFreeLockedMem(BufferSysExHnd, BufferSysEx);
     GlobalFreeLockedMem(BufferSysExHdrHnd, BufferSysExHdr);
END;

procedure TObjetoMidiOut.Open(FuncionCallBack:pointer);
VAR
   FError:INTEGER;
   Flags:longint;
begin
     IF (NOT Abierto) THEN
     BEGIN
          if (FuncionCallBack=nil) THEN Flags:=Callback_null else Flags:=Callback_Function;

          FError:=midiOutOpen(@Handler,                     {se supone que nos lo devuelve}
                             DispositivoActual,             {dispositivo a usar }
                             Integer(FuncionCallBack),      {direccion de la RSI }
                             longint(0),                    {parametro de usuario}
                             Flags);                        {Flags               }

          IF (FError=MMSYSERR_NOERROR) then Abierto:=True;
     END;
end;

procedure TObjetoMidiOut.Close;
begin
     if Abierto then
     begin
          midiOutClose(Handler);
          Abierto:=False;
     end;
end;

procedure TObjetoMidiOut.Comando(C,V1,V2:BYTE);
VAR
   CMD:LONGINT;
BEGIN
     if Abierto then
     BEGIN
          CMD:= LONGINT(C) OR
                (LONGINT(V1) SHL 8) OR
                (LONGINT(V2) SHL 16);

          midiOutShortMsg(Handler,CMD);
     END;
END;

procedure TObjetoMidiOut.SysEx(Buffer:POINTER;Num:LONGINT);
VAR
   FError:INTEGER;

begin
     IF Abierto THEN
     BEGIN
          {Copiar a la zona del buffer}
          StrMove(BufferSysEx,Buffer,Num);
          BufferSysExHdr^.dwBufferLength:=Num;

      	  FError := midiOutPrepareHeader(Handler,BufferSysExHdr,sizeof(TMIDIHDR));

	  IF Ferror = 0 THEN midiOutLongMsg(Handler,BufferSysEx,Num);
     END;
END;

procedure TObjetoMidiOut.EstablecerDispositivo(D:integer);
begin
     if (not Abierto) THEN DispositivoActual:=D;
end;

function TObjetoMidiOut.Dispositivo:Integer;
begin
     Dispositivo:=DispositivoActual;
end;
{------------------------------------------------------------------------------}
end.
