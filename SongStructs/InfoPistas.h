#ifndef InfoPistasH
#define InfoPistasH

#include "PascalCnv.H"
#include "CONSTS2.h"

    /*Teoricamente debe haber una tabla de nombres por bancos de sonido*/
    typedef String35 TNombreInstrumentoMIDI;

    class TInstrumentosMIDI
    {
        public:
         TInstrumentosMIDI() {};
        ~TInstrumentosMIDI() {};
        String35               Nombre;
        TNombreInstrumentoMIDI Ins[MaxInstrumentosMIDI];

        void Inicializar();
        bool CargarTablaInstrumentosMIDI(FILE *F);
        bool SalvarTablaInstrumentosMIDI(FILE *F);
    };

    /*
    // WARNING
    typedef struct {
          String35 Nombre;
          unsigned char Disp;
          unsigned char Banco;
          unsigned char Inst;
          unsigned char Canal;
          unsigned char VolDef;
          bool Ctrl0;
          bool Ctrl32;
    }InfoTrack ;

    class TTablaIns
    {
        public:
            TTablaIns(){};
            void InicializarTablaIns();
            bool CargarTablaIns(FILE *F);
            bool SalvarTablaIns(FILE *F);

        InfoTrack T[MaxTracks];
    };
    */

#endif