unit SelMidiIn;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons;

type
  TSeleccionMIDIIN = class(TForm)
    ListaMidiIn: TListBox;
    Label1: TLabel;
    BitBtn1: TBitBtn;

  private
    { Private declarations }
  public
    { Public declarations }
  end;

  function ObtenerMIDIIn(ActualMidiIn:BYTE):byte;

var
  SeleccionMIDIIN: TSeleccionMIDIIN;

implementation

uses
    Unit3;

{$R *.DFM}

function ObtenerMIDIIn(ActualMidiIn:BYTE):byte;
begin
     Application.CreateForm(TSeleccionMIDIIN,SeleccionMIDIIN);
     SeleccionMIDIIn.ListaMidiIn.Items:=Main.ListMidiIn.Items;
     SeleccionMIDIIn.ListaMidiIn.ItemIndex:=ActualMidiIn;

     IF (SeleccionMidiIn.ShowModal=mrOk) THEN
        ObtenerMidiIn:=SeleccionMIDIIn.ListaMidiIn.ItemIndex
     else
        ObtenerMidiIn:=ActualMidiIn;
end;

end.
