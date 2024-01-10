unit TestSongGUI;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
    Tipos,
    INTER{,
    Play};

type
  TForm1 = class(TForm)
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure PlayCancion(C:PCancion); cdecl;external '../tt98Player.DLL';
FUNCTION Reproduciendo:BOOLEAN; cdecl;external '../tt98Player.DLL';

procedure TForm1.FormCreate(Sender: TObject);
CONST
     DirectorioDatos = 'D:\DATOS\MUSICA\SONGS\';
VAR
   C :PCancion;
   F :FILE;
   ID:WORD;
     s : String;
BEGIN
     AssignFile(F,'..\prueba.tck');
     Reset(F,1);

     C := NIL;
     if ( CargarCancionGeneral(C,F) ) THEN
     BEGIN
             {EstablecerEstadoReproduccion(0,0);}
             PlayCancion(C);

             REPEAT UNTIL NOT Reproduciendo;
     END;

     // Close(F);

     DestruirCancion(C);
     {Finalizacion;}
end;

end.
