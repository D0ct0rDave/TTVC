program SongPlayer;

uses
  Forms,
  TestSongGUI in 'TestSongGUI.pas' {Form1},
  Tipos in 'Tipos.pas',
  INTER in 'Inter.pas',
  Consts2 in 'Consts2.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
