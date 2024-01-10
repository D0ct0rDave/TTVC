unit Consts2;

interface
CONST
     {Identificadores}
     VersionID :array[0..3] of CHAR=('2','.','0','0');

     {Determinan la capacidad máxima de la cancion}
     MaxTracks   = 64;
     MaxBeats    = 128;
     MaxPatterns = 256;
     MaxPatternsSequence = 256;

     {Dispositivos MIDI}
     MaxDispositivosMIDIOut = 16;
     MaxInstrumentosMIDI = 128;
     MaxBancos           = 64;

     MaxBPMS = 500;
     MinBPMS = 50;

     MaxSaltoLinea = 64;
     MaxAcorde     = 6;
     MaxArpegio    = 16;

     AUTOR_POR_DEFECTO ='Techno Autor';
     TITULO_POR_DEFECTO='Techno Tema ';
     BPMS_POR_DEFECTO = 145;

     {Lista de los Efectos permitidos}
     MaxBytesEfecto    = 16;
     MaxEfectosPropios = 32;
     NumeroDeEfectos   = 14;
     MaxEfectos        = 256;

     MaxCarNombreEfecto = 08;
     MaxCarNombreConfig = 40;

     ListaDeEfectos:ARRAY[0..MaxEfectosPropios-1] OF STRING[MaxCarNombreEfecto]=
               ('        ', {00}
                'Note Off',
                'EndOfMod',
                'PatBreak',
                'IncTempo',
                'DecTempo',
                'BPMS*2  ',
                'BPMS/2  ',
                '        ',
                'PrChange',
                'Panning ', {10}
                'Express.',
                'PitchHI ',
                'PitchLO ',
                'Reverb  ',
                'Chorus  ',
                'Modulat.',
                'Ch.Press',
                '        ',
                '        ',
                '        ', {20}
                '        ',
                '        ',
                '        ',
                '        ',
                '        ',
                '        ',
                '        ',
                '        ',
                '        ',
                '        ',{30}
                '        ');

    {Constantes para el formato del fichero}
    MaxCarNombreCancion = 80;
    MaxCarNombreAutor   = 80;
    MaxCarNombrePatron  = 80;
implementation

end.

