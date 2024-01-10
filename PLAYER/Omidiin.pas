unit OMidiIn;

interface
uses
    classes;
type
    TEventoMidiIn = RECORD
                          Cmd:byte;
                          Data1:byte;
                          Data2:byte;
                          Sysex:pointer;
                          TamSysex:longint;
                    END;

    TObjetoMidiIn = object
                          Abierto:Boolean;

                          constructor init;
                          procedure Open(FuncionCallBack:pointer);
                          procedure Close;
                          procedure Start;
                          procedure Stop;
                          procedure EstablecerDispositivo(D:integer);
                          function Dispositivo:Integer;

                          private
                          Handler:Integer;
                          DispositivoActual:Integer;
                    end;


    function NumeroDispositivosMIDIIn:Integer;
    procedure ObtenerListaDispositivosMIDIIn(S:TStrings);
    FUNCTION ObtenerNombreDispositivoMidiIn(Dispositivo:integer):String;

implementation
uses
    SysUtils,mmSystem;

function NumeroDispositivosMIDIIn:Integer;
begin
     NumeroDispositivosMIDIIn:=MidiInGetNumDevs;
end;

FUNCTION ObtenerNombreDispositivoMidiIn(Dispositivo:integer):String;
VAR
   M:TMidiInCaps;
   FError:Integer;
BEGIN
     FError :=midiInGetDevCaps(Dispositivo,@M,sizeof(TMidiInCaps));
     if (Ferror=MMSYSERR_NOERROR) then
        ObtenerNombreDispositivoMidiIn:=StrPas(M.szPName)
     else
         ObtenerNombreDispositivoMidiIn:='';
END;

procedure ObtenerListaDispositivosMIDIIn(S:TStrings);
var
   Ferror,j,i:integer;
   MidiInCaps:TMidiInCaps;
begin
     j:=MidiInGetNumDevs;

     S.Clear;
     for i:=0 to j-1 do
     begin
          FError :=midiInGetDevCaps(i,@MidiInCaps,sizeof(TMidiInCaps));
          S.Add(StrPas(MidiInCaps.szPname));
     end;
end;
{------------------------------------------------------------------------------}
constructor TObjetoMidiIn.init;
begin
     Abierto:=False;
end;

procedure TObjetoMidiIn.Open(FuncionCallBack:pointer);
VAR
   FError:INTEGER;
   Flags:longint;
begin
     IF (NOT Abierto) THEN
     BEGIN
          if (FuncionCallBack=nil) THEN Flags:=Callback_null else Flags:=Callback_Function;

          FError:=midiInOpen(@Handler,                      {se supone que nos lo devuelve}
                             DispositivoActual,             {dispositivo a usar }
                             Integer(FuncionCallBack),      {direccion de la RSI }
                             longint(0),                    {parametro de usuario}
                             Flags);                        {Flags               }

          IF (FError=MMSYSERR_NOERROR) then Abierto:=True;
     END;
end;

procedure TObjetoMidiIn.Close;
begin
     if Abierto then
     begin
          midiInClose(Handler);
          Abierto:=False;
     end;
end;

procedure TObjetoMidiIn.Start;
begin
     if Abierto then midiInStart(Handler);
end;

procedure TObjetoMidiIn.Stop;
begin
     if Abierto then midiInStop(Handler);
end;

procedure TObjetoMidiIn.EstablecerDispositivo(D:integer);
begin
     if (not Abierto) THEN DispositivoActual:=D;
end;

function TObjetoMidiIn.Dispositivo:Integer;
begin
     Dispositivo:=DispositivoActual;
end;
{------------------------------------------------------------------------------}
end.
