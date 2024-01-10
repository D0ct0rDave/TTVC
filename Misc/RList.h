#ifndef RListH
#define RListH

#include "PascalCnv.h"

    const int MaxStringsEnCola   = 16;
    const int MaxFicherosEnRList = MaxStringsEnCola;

    class TColaStr
    {
        public:
            TColaStr() {};
            ~TColaStr() {};

            void Inicializar(unsigned char MaxElem);
            unsigned char EstaElemento(AnsiString elem);
            void EliminarElemento(unsigned char i);
            void InsertarElemento(unsigned char i);
            void EscribirElemento(unsigned char i,AnsiString s);
            AnsiString LeerElemento(unsigned char i);


            unsigned char NumStr;
            unsigned char NumStrEnCola;
            AnsiString Strs[MaxStringsEnCola];
    };

    class TRecentList
    {
        public:
            TRecentList() {};
            ~TRecentList() {};
            void Inicializar(unsigned char MaxFicheros);
            void AnyadirElemento(AnsiString s);
            AnsiString LeerElemento(unsigned char i);

            TColaStr C;
    };

#endif
