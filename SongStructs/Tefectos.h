#ifndef TEfectosH
#define TEfectosH

#include "PascalCnv.h"
#include <stdio.h>

     // Lista de los Efectos permitidos
     #define MaxBytesEfecto         32
     #define MaxEfectosPropios      32
     #define NumeroDeEfectos        14
     #define MaxEfectos             256
     #define MaxCarNombreEfecto     8
     #define MaxCarNombreConfig     40

    typedef struct{
        unsigned char Data[32];
        unsigned char Num;
    }TBufferSysEx;
    typedef TBufferSysEx*  PBufferSysEx; 

    typedef struct
    {
        String35 Nombre;
        struct Datos
        {
              unsigned short B[MaxBytesEfecto];
              unsigned char  N;
        }Datos;

    }TEfectoMIDI;
    typedef TEfectoMIDI* PEfectoMIDI;

    class TEfectosMIDI
    {
        public: TEfectosMIDI() {};
                String35 NombreConfiguracion;

                void Inicializar();
                void EstablecerNombre(unsigned char i,String35 Nombre);
                String35 TomarNombreEfecto(unsigned char i);
                PEfectoMIDI TomarDatosEfecto(unsigned char i);

                bool CargarTablaEfectos(FILE *F);
                bool SalvarTablaEfectos(FILE *F);

                void ObtenerEfectoMIDI(unsigned char CanalMIDI,unsigned char EfectoMIDI,unsigned char Valor,PBufferSysEx Buf);

        public:
                TEfectoMIDI E[MaxEfectos];
    };

    typedef TEfectosMIDI* PEfectosMIDI;

#endif
