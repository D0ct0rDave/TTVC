unit
    SMem;

interface
Uses
    WinTypes,Windows;

         FUNCTION GlobalAllocLockedMem(Cantidad:longint;var Handler:THandle):pointer;
         PROCEDURE GlobalFreeLockedMem(Handler:THandle;P:POINTER);

implementation
uses
    WinProcs;

{------------------------------------------------------------------------------}
FUNCTION GlobalAllocLockedMem(Cantidad:longint;var Handler:THandle):pointer;
var
   p:pointer;
BEGIN
     Handler := GlobalAlloc(GMEM_SHARE Or GMEM_MOVEABLE Or GMEM_ZEROINIT,Cantidad);

     if (Handler<>0) then
     begin
          p:=GlobalLock(Handler);

          if (p=nil) then GlobalFree(Handler);
          GlobalAllocLockedMem:=p;
     end
     else
         GlobalAllocLockedMem:=nil;
END;

PROCEDURE GlobalFreeLockedMem(Handler:THandle;P:POINTER);
BEGIN
     GlobalUnLock(Handler);
     GlobalFree(Handler);
END;
{------------------------------------------------------------------------------}
end.