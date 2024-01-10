unit SelMidiOut;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons;

type
  TSeleccionMidiOut = class(TForm)
    Label1: TLabel;
    ListaMidiOut: TListBox;
    BitBtn1: TBitBtn;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  function ObtenerMIDIOut(ActualMidiOut:BYTE):byte;

var
  SeleccionMidiOut: TSeleccionMidiOut;

implementation
uses
    Unit3;

{$R *.DFM}
function ObtenerMIDIOut(ActualMidiOut:BYTE):byte;
begin
     Application.CreateForm(TSeleccionMIDIOut,SeleccionMIDIOut);
     SeleccionMIDIOut.ListaMidiOut.Items:=Main.ListMidiOut.Items;
     SeleccionMIDIOut.ListaMidiOut.ItemIndex:=ActualMidiOut;

     IF (SeleccionMidiOut.ShowModal=mrOk) THEN
        ObtenerMidiOut:=SeleccionMIDIOut.ListaMidiOut.ItemIndex
     else
        ObtenerMidiOut:=ActualMidiOut;
end;

end.
