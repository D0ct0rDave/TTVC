program test02;
uses
    Windows,
    MMSystem,
    SysUtils,
    Dialogs;
var
   T:TTimeCaps;
begin
     {Muestra el periodo minimo y maximo soportado por la computadora}
     timeGetDevCaps(@T,SizeOf(T));
     ShowMessage('Periodo minimo de repeticion: '+IntToStr(T.wPeriodMin)+#13+
                 'Periodo manimo de repeticion: '+IntToStr(T.wPeriodMax));
end.