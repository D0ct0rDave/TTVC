#ifndef OMidiInH
#define OMidiInH

#include <windows.h>
#include <mmSystem.h>

    typedef struct
    {
        unsigned char Cmd;
        unsigned char Data1;
        unsigned char Data2;
        unsigned char *Sysex;
        unsigned int TamSysex;

    }TEventoMidiIn;

    class TObjetoMidiIn
    {
        public:
            TObjetoMidiIn();
            ~TObjetoMidiIn() {};

            virtual void  Open (void *FuncionCallBack);
            virtual void  Close();
            virtual void  Start();
            virtual void  Stop ();
            virtual void  EstablecerDispositivo(int D);
            virtual int   Dispositivo();
		
		public:
            bool		  Abierto;

        protected:
                HMIDIIN      Handler;
                int			 DispositivoActual;
    };


    int NumeroDispositivosMIDIIn();
    char *ObtenerNombreDispositivoMidiIn(int Dispositivo);
#endif


 